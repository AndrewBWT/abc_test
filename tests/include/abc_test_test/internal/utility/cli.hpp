#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/internal/utility/cli.hpp"

namespace test
{
class abc_test_option_class_t
{};

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
};
} // namespace test

namespace abc
{
template <>
auto
    utility::get_enum_list() -> utility::enum_list_t<test::enum_cli_add_instr_t>
{
    return {};
}
} // namespace abc

_TEST_CASE(
    abc::test_case_t(
        {.name        = "Testing constructor for cli_t",
         .description = "Checking specific values in the cli_t constructor and "
                        "how they effect the object",
         .path        = "abc_test_test::internal::utility::cli",
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
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_cli_bba, "Testing cli_t object with different arguments"
    );
    for (const auto& [_l_char, _l_string, _l_constructable] :
         iterate_over<test_data_t>({}))
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
                for (const enum_cli_add_instr_t _l_add_instr :
                     _l_cli_add_instrs)
                {
                    switch (_l_add_instr)
                    {
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