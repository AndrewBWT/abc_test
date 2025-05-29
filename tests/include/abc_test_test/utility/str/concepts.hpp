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