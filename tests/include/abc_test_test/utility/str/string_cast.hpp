#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str/concepts.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for cast_u8string_to_string "
                             "and cast_string_to_u8string",
         .path             = "abc_test_test::utility::str::string_cast",
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

    auto _l_unit_tests = _MULTI_MATCHER(
        fmt::format("Unit testing cast_u8string_to_string and "
                    "cast_string_to_u8string using")
    );
    using unit_test_data_1 = tuple<u8string, string>;
    for (const auto& [_l_u8string, _l_string] :
         read_data_from_file<unit_test_data_1>(fmt::format("unit_test_1")))
    {
        _l_unit_tests << _CHECK(
            _EXPR(cast_u8string_to_string(_l_u8string) == _l_string)
            && _EXPR(
                cast_u8string_to_string(u8string(_l_u8string)) == _l_string
            )
            && _EXPR(cast_string_to_u8string(_l_string) == _l_u8string)
            && _EXPR(cast_string_to_u8string(string(_l_string)) == _l_u8string)
        );
        _l_unit_tests << _CHECK(
            _EXPR(
                cast_string_to_u8string(cast_u8string_to_string(_l_u8string))
                == _l_u8string
            )
            && _EXPR(
                cast_string_to_u8string(
                    cast_u8string_to_string(u8string(_l_u8string))
                )
                == _l_u8string
            )
            && _EXPR(
                cast_u8string_to_string(cast_string_to_u8string(_l_string))
                == _l_string
            )
            && _EXPR(
                cast_u8string_to_string(cast_string_to_u8string(string(_l_string
                )))
                == _l_string
            )
        );
    }
    _CHECK(_l_unit_tests);

    auto _l_property_tests = _MULTI_MATCHER(
        fmt::format("Property testing cast_u8string_to_string and "
                    "cast_string_to_u8string using")
    );
    using fuzzy_test_data_1 = tuple<u8string>;
    for (const auto& [_l_u8string] :
         generate_data_randomly<fuzzy_test_data_1>()
             & generate_data_randomly<fuzzy_test_data_1>(
                 default_random_generator<fuzzy_test_data_1>(
                     default_random_generator<u8string>(
                         default_random_generator<char8_t>()
                     )
                 )
             ))
    {
        _l_property_tests << _CHECK(
            _EXPR(
                cast_string_to_u8string(cast_u8string_to_string(_l_u8string))
                == _l_u8string
            )
            && (_EXPR(
                cast_string_to_u8string(
                    cast_u8string_to_string(u8string(_l_u8string))
                )
                == _l_u8string
            ))
        );
    }
    using fuzzy_test_data_2 = tuple<string>;
    for (const auto& [_l_u8string] :
         generate_data_randomly<fuzzy_test_data_2>()
             & generate_data_randomly<fuzzy_test_data_2>(
                 default_random_generator<fuzzy_test_data_2>(
                     default_random_generator<string>(
                         default_random_generator<char>()
                     )
                 )
             ))
    {
        _l_property_tests << _CHECK(
            _EXPR(
                cast_u8string_to_string(cast_string_to_u8string(_l_u8string))
                == _l_u8string
            )
            && (_EXPR(
                cast_u8string_to_string(
                    cast_string_to_u8string(string(_l_u8string))
                )
                == _l_u8string
            ))
        );
    }
    _CHECK(_l_property_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for cast_wstring_to_unicode_string "
                             "and cast_unicode_string_to_wstring",
         .path             = "abc_test_test::utility::str::string_cast",
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
    using T = decltype(cast_wstring_to_unicode_string(L"hello"));
    auto _l_unit_tests = _MULTI_MATCHER(
        fmt::format("Unit testing cast_wstring_to_unicode_string and "
                    "cast_unicode_string_to_wstring using")
    );
    using unit_test_data_1 = tuple<wstring, T>;
    for (const auto& [_l_wstring, _l_unicode_string] :
         read_data_from_file<unit_test_data_1>(fmt::format("unit_test_1")))
    {
        _l_unit_tests << _CHECK(
            _EXPR(
                cast_wstring_to_unicode_string(_l_wstring) == _l_unicode_string
            )
            && _EXPR(
                cast_wstring_to_unicode_string(wstring(_l_wstring))
                == _l_unicode_string
            )
            && _EXPR(
                cast_unicode_string_to_wstring(_l_unicode_string) == _l_wstring
            )
            && _EXPR(
                cast_unicode_string_to_wstring(T(_l_unicode_string))
                == _l_wstring
            )
        );
        _l_unit_tests << _CHECK(
            _EXPR(
                cast_unicode_string_to_wstring(
                    cast_wstring_to_unicode_string(_l_wstring)
                )
                == _l_wstring
            )
            && _EXPR(
                cast_unicode_string_to_wstring(
                    cast_wstring_to_unicode_string(wstring(_l_wstring))
                )
                == _l_wstring
            )
            && _EXPR(
                cast_wstring_to_unicode_string(
                    cast_unicode_string_to_wstring(_l_unicode_string)
                )
                == _l_unicode_string
            )
            && _EXPR(
                cast_wstring_to_unicode_string(
                    cast_unicode_string_to_wstring(T(_l_unicode_string))
                )
                == _l_unicode_string
            )
        );
    }
    _CHECK(_l_unit_tests);

    auto _l_property_tests = _MULTI_MATCHER(
        fmt::format("Property testing cast_wstring_to_unicode_string and "
                    "cast_unicode_string_to_wstring")
    );
    using fuzzy_test_data_1 = tuple<wstring>;
    for (const auto& [_l_wstring] :
         generate_data_randomly<fuzzy_test_data_1>()
             & generate_data_randomly<fuzzy_test_data_1>(
                 default_random_generator<fuzzy_test_data_1>(
                     default_random_generator<wstring>(
                         default_random_generator<wchar_t>()
                     )
                 )
             ))
    {
        _l_property_tests << _CHECK(
            _EXPR(
                cast_unicode_string_to_wstring(
                    cast_wstring_to_unicode_string(_l_wstring)
                )
                == _l_wstring
            )
            && (_EXPR(
                cast_unicode_string_to_wstring(
                    cast_wstring_to_unicode_string(wstring(_l_wstring))
                )
                == _l_wstring
            ))
        );
    }
    using fuzzy_test_data_2 = tuple<T>;
    for (const auto& [_l_unicode_str] :
         generate_data_randomly<fuzzy_test_data_2>()
             & generate_data_randomly<fuzzy_test_data_2>(
                 default_random_generator<fuzzy_test_data_2>(
                     default_random_generator<T>(
                         default_random_generator<typename T::value_type>()
                     )
                 )
             ))
    {
        _l_property_tests << _CHECK(
            _EXPR(
                cast_wstring_to_unicode_string(
                    cast_unicode_string_to_wstring(_l_unicode_str)
                )
                == _l_unicode_str
            )
            && (_EXPR(
                cast_wstring_to_unicode_string(
                    cast_unicode_string_to_wstring(T(_l_unicode_str))
                )
                == _l_unicode_str
            ))
        );
    }
    _CHECK(_l_property_tests);
}