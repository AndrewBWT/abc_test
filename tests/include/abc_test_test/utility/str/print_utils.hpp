#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str/concepts.hpp"

namespace nameity
{
class X
{};

struct Y
{};

template <typename T>
struct Z
{};
} // namespace nameity

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for type_id",
         .path             = "abc_test_test::utility::str::print_utils",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace nameity;
    auto _l_func = [&]<typename T>(const std::u8string& _a_str)
    {
        _CHECK_EXPR(type_id<T>() == _a_str);
    };
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (_l_func.operator()<char>(u8"char")));
    RUN(_l_mdg, (_l_func.operator()<X>(u8"class nameity::X")));
    RUN(_l_mdg, (_l_func.operator()<Y>(u8"struct nameity::Y")));
    RUN(_l_mdg,
        (_l_func.operator()<Z<X>>(u8"struct nameity::Z<class nameity::X>")));
    RUN(_l_mdg, (_l_func.operator()<int>(u8"int")));
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for positive_integer_to_placement",
         .path             = "abc_test_test::utility::str::print_utils",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_type_name{typeid(T).name()};
        auto _l_name{get_name<T>()};
        _TVLOG(_l_name);
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing positive_integer_to_placement using {0}",
                _l_type_name
            )
        );
        // Tests converting a unicode string to an ascii string.
        // Anything outside of the ascii range is caught.
        using unit_test_data_1 = tuple<uint64_t, u8string>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_uint64, _l_str]{_l_data};
            if (_l_uint64 <= static_cast<uint64_t>(numeric_limits<T>::max()))
            {
                const T _l_as_t{static_cast<T>(_l_uint64)};
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(positive_integer_to_placement(_l_as_t) == _l_str)
                );
            }
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing positive_integer_to_placement using {0}",
                _l_type_name
            )
        );
        // Either generate valid unicode strings, or (potentially) invalid
        // unicode strings.
        using fuzzy_test_data_1 = std::tuple<T>;
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_n]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(positive_integer_to_placement(_l_n));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t
        = abc::utility::type_list<uint8_t, uint16_t, uint32_t, uint64_t>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
    auto _l_compile_time_integer_test = [&]<size_t I>(const u8string _a_str)
    {
        _CHECK_EXPR(positive_integer_to_placement<I>() == _a_str);
    };
    manual_data_generator_t _l_mdg2;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg2, (_l_compile_time_integer_test.operator()<0>(u8"0th")));
            RUN(_l_mdg2, (_l_compile_time_integer_test.operator()<1>(u8"1st")));
            RUN(_l_mdg2, (_l_compile_time_integer_test.operator()<2>(u8"2nd")));
            RUN(_l_mdg2, (_l_compile_time_integer_test.operator()<3>(u8"3rd")));
            RUN(_l_mdg2,
                (_l_compile_time_integer_test.operator()<11>(u8"11th")));
            RUN(_l_mdg2,
                (_l_compile_time_integer_test.operator()<21>(u8"21st")));
            RUN(_l_mdg2,
                (_l_compile_time_integer_test.operator()<10'912>(u8"10912th")));
            RUN(_l_mdg2,
                (_l_compile_time_integer_test.operator()<10'922>(u8"10922nd")));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for represent_char_as_hex_for_printing and "
                             "represent_char_as_hex_for_output",
         .path             = "abc_test_test::utility::str::print_utils",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto         _l_type_name{typeid(T).name()};
        const string _l_name{typeid(T).name()};
        _TVLOG(_l_name);
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing represent_char_as_hex_for_printing and "
                "represent_char_as_hex_for_output using {0}",
                _l_type_name
            )
        );
        // Tests converting a unicode string to an ascii string.
        // Anything outside of the ascii range is caught.
        using unit_test_data_1 = tuple<T, u8string, u8string>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_char, _l_output, _l_for_printing]{_l_data};
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(
                    represent_char_as_hex_for_printing(_l_char)
                    == _l_for_printing
                )
                && _EXPR(represent_char_as_hex_for_output(_l_char) == _l_output)
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing represent_char_as_hex_for_printing and "
                "represent_char_as_hex_for_output using {0}",
                _l_type_name
            )
        );
        // Either generate valid unicode strings, or (potentially) invalid
        // unicode strings.
        using fuzzy_test_data_1 = std::tuple<T>;
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_n]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(represent_char_as_hex_for_printing(_l_n));
            do_not_optimise(represent_char_as_hex_for_output(_l_n));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t
        = abc::utility::type_list<char, char8_t, char16_t, char32_t, wchar_t>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for mk_str_representing_function_call and "
                             "mk_str_representing_function_argsusing",
         .path             = "abc_test_test::utility::str::print_utils",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto         _l_type_name{typeid(T).name()};
        const string _l_name{get_name<T>()};
        _TVLOG(_l_name);
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing mk_str_representing_function_call and "
                "mk_str_representing_function_argsusing {0}",
                _l_type_name
            )
        );
        // Tests converting a unicode string to an ascii string.
        // Anything outside of the ascii range is caught.
        using unit_test_data_1 = tuple<T, vector<T>, tuple<T, T>>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_prefix, _l_args, _l_expected_results]{_l_data};
            tuple<vector<T>, vector<T>> _l_results;
            auto _l_inserter   = std::back_inserter(get<0>(_l_results));
            auto _l_inserter_2 = std::back_inserter(get<1>(_l_results));
            switch (_l_args.size())
            {
            case 0:
                _l_inserter = mk_str_representing_function_call(_l_prefix);
                break;
            case 1:
                _l_inserter
                    = mk_str_representing_function_call(_l_prefix, _l_args[0]);
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, T(_l_args[0])
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0]
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), T(_l_args[0])
                );
                _l_inserter_2
                    = mk_str_representing_function_args(T(_l_args[0]));
                _l_inserter_2 = mk_str_representing_function_args(_l_args[0]);
                break;
            case 2:
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, _l_args[0], _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0], _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, T(_l_args[0]), _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, _l_args[0], T(_l_args[1])
                );
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, T(_l_args[0]), T(_l_args[1])
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0], T(_l_args[1])
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), T(_l_args[0]), _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), T(_l_args[0]), T(_l_args[1])
                );

                _l_inserter_2
                    = mk_str_representing_function_args(_l_args[0], _l_args[1]);
                _l_inserter_2 = mk_str_representing_function_args(
                    T(_l_args[0]), _l_args[1]
                );
                _l_inserter_2 = mk_str_representing_function_args(
                    _l_args[0], T(_l_args[1])
                );
                _l_inserter_2 = mk_str_representing_function_args(
                    T(_l_args[0]), T(_l_args[1])
                );
                break;
            case 3:
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0], _l_args[1], _l_args[2]
                );
                _l_inserter_2 = mk_str_representing_function_args(
                    T(_l_args[0]), T(_l_args[1]), T(_l_args[2])
                );
                _l_inserter_2 = mk_str_representing_function_args(
                    T(_l_args[0]), T(_l_args[1]), T(_l_args[2])
                );
                _l_inserter_2 = mk_str_representing_function_args(
                    T(_l_args[0]), T(_l_args[1]), T(_l_args[2])
                );
                break;
            }
            //_l_unit_tests += _BLOCK_REQUIRE(_EXPR(_l_results.size() > 0));
            auto& [_l_mk_str_rep_function_call, _l_mk_str_rep_funcion_args_res]{
                _l_expected_results
            };
            if (get<0>(_l_results).size() > 0)
            {
                _l_unit_tests += _BLOCK_CHECK(
                    all_equal(get<0>(_l_results))
                    && _EXPR(
                        get<0>(_l_results)[0] == _l_mk_str_rep_function_call
                    )
                );
            }
            if (get<1>(_l_results).size() > 0)
            {
                _l_unit_tests += _BLOCK_CHECK(
                    all_equal(get<1>(_l_results))
                    && _EXPR(
                        get<1>(_l_results)[0] == _l_mk_str_rep_funcion_args_res
                    )
                );
            }
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing mk_str_representing_function_call and "
                "mk_str_representing_function_argsusing using {0}",
                _l_type_name
            )
        );
        // Either generate valid unicode strings, or (potentially) invalid
        // unicode strings.
        using fuzzy_test_data_1 = tuple<T, vector<T>>;
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_prefix, _l_args]{_l_data};
            vector<T> _l_results;
            auto      _l_inserter = std::back_inserter(_l_results);
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            switch (_l_args.size())
            {
            case 0:
                _l_inserter = mk_str_representing_function_call(_l_prefix);
                break;
            case 1:
                _l_inserter
                    = mk_str_representing_function_call(_l_prefix, _l_args[0]);
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, T(_l_args[0])
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0]
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), T(_l_args[0])
                );
                break;
            case 2:
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, _l_args[0], _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0], _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, T(_l_args[0]), _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, _l_args[0], T(_l_args[1])
                );
                _l_inserter = mk_str_representing_function_call(
                    _l_prefix, T(_l_args[0]), T(_l_args[1])
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0], T(_l_args[1])
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), T(_l_args[0]), _l_args[1]
                );
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), T(_l_args[0]), T(_l_args[1])
                );
                break;
            case 3:
                _l_inserter = mk_str_representing_function_call(
                    T(_l_prefix), _l_args[0], _l_args[1], _l_args[2]
                );
                break;
            }
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(all_equal(_l_results));
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t = abc::utility::
        type_list<string, u8string, u16string, u32string, wstring>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Tests for mk_str_appending_function_name_and_function_args",
         .path = "abc_test_test::utility::str::print_utils",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto         _l_type_name{typeid(T).name()};
        const string _l_name{get_name<T>()};
        _TVLOG(_l_name);
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing mk_str_appending_function_name_and_function_args "
                "{0}",
                _l_type_name
            )
        );
        // Tests converting a unicode string to an ascii string.
        // Anything outside of the ascii range is caught.
        using unit_test_data_1 = tuple<T, T, T>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_prefix, _l_args, _l_expected_result]{_l_data};
            vector<T> _l_results;
            auto      _l_inserter = std::back_inserter(_l_results);
            _l_inserter = mk_str_appending_function_name_and_function_args(
                _l_prefix, _l_args
            );
            _l_inserter = mk_str_appending_function_name_and_function_args(
                T(_l_prefix), _l_args
            );
            _l_inserter = mk_str_appending_function_name_and_function_args(
                _l_prefix, T(_l_args)
            );
            _l_inserter = mk_str_appending_function_name_and_function_args(
                T(_l_prefix), T(_l_args)
            );
            _l_unit_tests += _BLOCK_CHECK(
                all_equal(_l_results) && _EXPR(_l_results[0] == _l_expected_result)
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing mk_str_representing_function_call and "
                "mk_str_representing_function_argsusing using {0}",
                _l_type_name
            )
        );
        // Either generate valid unicode strings, or (potentially) invalid
        // unicode strings.
        using fuzzy_test_data_1 = tuple<T, T>;
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_prefix, _l_args]{_l_data};
            vector<T> _l_results;
            auto      _l_inserter = std::back_inserter(_l_results);
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            _l_inserter = mk_str_appending_function_name_and_function_args(
                _l_prefix, _l_args
            );
            _l_inserter = mk_str_appending_function_name_and_function_args(
                T(_l_prefix), _l_args
            );
            _l_inserter = mk_str_appending_function_name_and_function_args(
                _l_prefix, T(_l_args)
            );
            _l_inserter = mk_str_appending_function_name_and_function_args(
                T(_l_prefix), T(_l_args)
            );
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(all_equal(_l_results));
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t = abc::utility::
        type_list<string, u8string, u16string, u32string, wstring>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}
#if 0
_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for make_focused_string",
         .path             = "abc_test_test::utility::str::print_utils",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_type_name{typeid(T).name()};
        auto _l_name{get_name<T>()};
        _TVLOG(_l_name);
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing make_focused_string using {0}", _l_type_name
            )
        );
        // Tests converting a unicode string to an ascii string.
        // Anything outside of the ascii range is caught.
        using unit_test_data_1 = tuple<T, vector<tuple<size_t, size_t, T>>>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_str, _l_results]{_l_data};
            for (auto& [_l_idx, _l_limit, _l_result] : _l_results)
            {
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(
                        make_focused_string(_l_str, _l_idx, _l_limit) == _l_result
                    )
                    && _EXPR(
                        make_focused_string(T(_l_str), _l_idx, _l_limit)
                        == _l_result
                    )
                );
            }
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing make_focused_string using {0}", _l_type_name
            )
        );
        // Either generate valid unicode strings, or (potentially) invalid
        // unicode strings.
        using fuzzy_test_data_1 = std::tuple<T, size_t, size_t>;
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_str, _a_idx, _a_limit]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(make_focused_string(_l_str, _a_idx, _a_limit));
            do_not_optimise(make_focused_string(
                T(_l_str), _a_idx, _a_limit));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t
        = abc::utility::type_list<string,u8string,u16string,u32string,wstring>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}
#endif