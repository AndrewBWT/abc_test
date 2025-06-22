#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str/concepts.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Tests for concepts is_wchar_and_32_bit_c, is_wchar_and_16_bit_c, "
           "char_type_is_unicode_c and is_char_type_c",
         .path             = "abc_test_test::utility::str::concepts",
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
    using Value_Type = int;
    using data_types_t
        = abc::utility::type_list<char, char8_t, char16_t, char32_t, wchar_t>;
    file_based_map_t<u8string, std::tuple<bool, bool, bool, bool>> _l_data(
        "unit_test"
    );
    auto _l_test_func = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const std::tuple<bool, bool, bool, bool>& _a_expected_results)
            {
                _TVLOG(_a_expected_results);
                auto& [_l_whcar_and_32, _l_wchar_and_16, _l_unicode, _l_char]{
                    _a_expected_results
                };
                return _EXPR(is_wchar_and_32_bit_c<T> == _l_whcar_and_32)
                       && _EXPR(is_wchar_and_16_bit_c<T> == _l_wchar_and_16)
                       && _EXPR(char_type_is_unicode_c<T> == _l_unicode)
                       && _EXPR(is_char_type_c<T> == _l_char);
            }
        ));
    };
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
        {.name             = "Tests for has_string_like_underlying_type_c and "
                             "string_like_underlying_char_type_t",
         .path             = "abc_test_test::utility::str::concepts",
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
    using Value_Type   = int;
    using data_types_t = abc::utility::type_list<
        const char*,
        const wchar_t*,
        const char8_t*,
        const char16_t*,
        const char32_t*,
        const char[1],
        const wchar_t[1],
        const char8_t[1],
        const char16_t[1],
        const char32_t[1],
        string,
        wstring,
        u8string,
        u16string,
        u32string,
        string_view,
        wstring_view,
        u8string_view,
        u16string_view,
        u32string_view,
        vector<char>,
        vector<int>,
        basic_string_view<int>,
        basic_string<int>>;
    file_based_map_t<u8string, tuple<u8string>> _l_data("unit_test");
    auto _l_test_func = [&]<typename T>()
    {
        _TVLOG(type_id<T>());
        _CHECK(_l_data.generate_matcher(
            type_id<T>(),
            [&](const tuple<u8string>& _a_expected_result)
            {
                _TVLOG(_a_expected_result);
                auto [_l_str] {_a_expected_result};
                if constexpr (has_string_like_underlying_type_c<T>)
                {
                    if (not _l_str.empty())
                    {
                        return _EXPR(
                                   has_string_like_underlying_type_c<T> == true
                               )
                               && _EXPR(
                                   type_id<
                                       string_like_underlying_char_type_t<T>>()
                                   == _l_str
                               );
                    }
                    else
                    {
                        return _EXPR(
                            has_string_like_underlying_type_c<T> == false
                        );
                    }
                }
                else
                {
                    if (not _l_str.empty())
                    {
                        return _EXPR(
                            has_string_like_underlying_type_c<T> == true
                        );
                    }
                    else
                    {
                        return _EXPR(
                            has_string_like_underlying_type_c<T> == false
                        );
                    }
                }
            }
        ));
    };
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
        {.name             = "Tests for all_string_like_with_same_char_c",
         .path             = "abc_test_test::utility::str::concepts",
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
    _CHECK_EXPR(all_string_like_with_same_char_c<> == false);
    _CHECK_EXPR(all_string_like_with_same_char_c<const char[1]> == true);
    _CHECK_EXPR(
        ( all_string_like_with_same_char_c<u8string, const char8_t*> ) == true
    );
    _CHECK_EXPR(
        ( all_string_like_with_same_char_c<vector<char>, string> ) == false
    );
    _CHECK_EXPR(
        ( all_string_like_with_same_char_c<u8string, string> ) == false
    );
    _CHECK_EXPR(
        ( all_string_like_with_same_char_c<
            const char*,
            const char[1],
            string,
            string_view> )
        == true
    );
}