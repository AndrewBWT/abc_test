#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/cli.hpp"

namespace test
{
struct abc_test_option_class_t
{
    std::string var_1;
};

inline std::vector<std::string>
    make_args(
        const char             _a_char,
        const std::string_view _a_str
    )
{
    using namespace std;
    return vector<string>{};
}
enum class enum_cli_add_instr_t
{
    ADD_GENERIC_OPTION
};
} // namespace test

namespace abc
{
template <>
auto
    utility::get_enum_list() -> utility::enum_list_t<test::enum_cli_add_instr_t>
{
    using enum test::enum_cli_add_instr_t;
    return {ADD_GENERIC_OPTION};
}
} // namespace abc

// Tests

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing \"make_cli\" with incorrect values",
         .description
         = "Ensuring that \"make_cli\" doesn't accept incorrect values",
         .path             = "abc_test_test::utility::cli",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace test;
    using test_data_t = tuple<char, string, optional<string>>;
    using cli_type_t  = cli_t<abc_test_option_class_t>;
    using enum enum_cli_add_instr_t;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_cli_bba, "Testing make_cli with different arguments"
    );
    // Error strings are only produced when a cli_t cannot be created.
    for (const auto& [_l_char, _l_string, _l_error_string] :
         read_data_from_file<test_data_t>("make_cli")
             & generate_data_randomly<test_data_t>(gdf("make_cli_random_data")))
    {
        result_t<cli_type_t> _l_cli_result{
            make_cli<abc_test_option_class_t>(_l_char, _l_string)
        };
        const bool _l_cli_result_is_expected{
            _l_cli_result.has_value() == (not _l_error_string.has_value())
        };
        _l_cli_bba += _BLOCK_CHECK(annotate(
            "Checking result type of make_cli matches the given "
            "_l_error_string value",
            _EXPR(_l_cli_result_is_expected == true)
        ));
        if (_l_cli_result_is_expected)
        {
            // Either _l_cli_result has value and _l_error_string is empty.
            // In which case, nothing to check.
            // Else, _l_cli_result has error and _l_error_string is not empty.
            // Check strings.
            if (not _l_cli_result.has_value())
            {
                _l_cli_bba += _BLOCK_CHECK(annotate(
                    "Checking _l_error_string matches error produced from "
                    "make_cli",
                    _EXPR(_l_error_string.value() == _l_cli_result.error())
                ));
            }
            else
            {
                // We should be able to come up with an option, and have it
                // processed.
                cli_type_t&   _l_cli{_l_cli_result.value()};
                cli_results_t _l_cli_results;
                char          _l_option_char{'i'};
                const string  _l_option_flag{
                    fmt::format("{0}{1}", _l_char, _l_option_char)
                };
                _l_cli.add_option(
                    cli_option_config_t{_l_option_flag, "", {_l_option_char}},
                    &abc_test_option_class_t::var_1,
                    _l_cli_results
                );
                if (_l_cli_results.has_errors())
                {
                    _l_cli_bba += _BLOCK_FAIL_WITH_MSG(fmt::format(
                        "cli_t should have been able to create a single option "
                        "with the flag \"{0}\" and small flag \"{1}\", however "
                        "this was not possible. Error was \"{2}\"",
                        _l_option_flag,
                        _l_option_flag,
                        _l_cli_results.errors()
                    ));
                    continue;
                }
                abc_test_option_class_t _l_object;
                _l_cli.parse_arguments(
                    _l_object, {_l_option_flag, "hello"}, _l_cli_results
                );
                if (_l_cli_results.has_errors())
                {
                    _l_cli_bba += _BLOCK_FAIL_WITH_MSG(fmt::format(
                        "cli_t should have been able to process a single "
                        "option "
                        "in the form \"{0}\", however "
                        "this was not possible. Error was \"{1}\"",
                        std::vector<std::string>({_l_option_flag, "hello"}),
                        _l_cli_results.errors()
                    ));
                    continue;
                }
                _l_cli_bba += _BLOCK_CHECK(annotate(
                    "Checking var_1 set correctly",
                    _EXPR(_l_object.var_1 == "hello")
                ));
            }
        }
    }
    _END_BBA_CHECK(_l_cli_bba);
}

_TEST_CASE(
    abc::test_case_t(
        {.name        = "Testing factory method \"make_cli\"",
         .description = "Checking specific values used with the factory method "
                        "make_cli and how they effect the created cli_t object",
         .path        = "abc_test_test::utility::cli",
         .threads_required = 1}
    )
)
{
    // I believe that there are specific values which should not be allowed to
    // be used for the flag settings in a cli_t object.

    // I think these will cause issues when reading some data.
    using namespace abc;
    using namespace std;
    using namespace test;
    using test_data_t = tuple<char, string, bool>;
    using cli_type_t  = cli_t<abc_test_option_class_t>;
    using enum enum_cli_add_instr_t;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_cli_bba, "Testing cli_t object with different arguments"
    );
    for (const auto& [_l_char, _l_string, _l_constructable] :
         iterate_over<test_data_t>({
             {'-', "--", true}
    }))
    {
        const result_t<cli_type_t> _l_cli_result{
            make_cli<abc_test_option_class_t>(_l_char, _l_string)
        };
        if (_l_cli_result.has_value())
        {
            cli_type_t _l_cli{_l_cli_result.value()};
            _l_cli_bba += _BLOCK_CHECK(annotate(
                "Checking that constructable data matches "
                "with the function",
                _EXPR(_l_constructable == true)
            ));
            // This will generate all permutations and combinations of
            // enum_cli_add_instr_t, going from an empty vector to a full
            // vector.
            for (const vector<enum_cli_add_instr_t>& _l_cli_add_instrs :
                 using_combinatorics_generator_<
                     enum_cli_add_instr_t,
                     true,
                     false,
                     true>(
                     all_values<enum_cli_add_instr_t>(),
                     default_enumeration<enum_cli_add_instr_t>()
                 ))
            {
                cli_results_t _l_cli_results;
                for (const enum_cli_add_instr_t _l_add_instr :
                     _l_cli_add_instrs)
                {
                    switch (_l_add_instr)
                    {
                    case ADD_GENERIC_OPTION:
                        _l_cli.add_option(
                            cli_option_config_t{"hi", "", {'i'}},
                            &abc_test_option_class_t::var_1,
                            _l_cli_results
                        );
                        if (_l_cli_results.has_errors())
                        {
                            _l_cli_bba += _BLOCK_FAIL_WITH_MSG(fmt::format(
                                "Failure caused by unexpected errors as "
                                "follows: {0}",
                                _l_cli_results.errors()
                            ));
                        }
                        break;
                    default:
                        _l_cli_bba += _BLOCK_FAIL_WITH_MSG(
                            "Failure caused by unhandled enum_cli_add_instr_t"
                        );
                    }
                }
            }

            // We will use parsing arguments from vector so as to make things
            // less complicated.
            vector<string> _l_args{make_args(_l_char, _l_string)};
        }
        else
        {
            _l_cli_bba += _BLOCK_CHECK(annotate(
                "As cli_t is not constructable, checking that data matches "
                "with the function",
                _EXPR(_l_constructable == false)
            ));
        }
    }
    _END_BBA_CHECK(_l_cli_bba);
}