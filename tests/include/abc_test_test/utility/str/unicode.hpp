#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str/unicode.hpp"

namespace
{
template <typename T>
constexpr auto
    get_name() -> std::string
{
    using namespace std;
    if constexpr (same_as<T, string>)
    {
        return "string";
    }
    else if constexpr (same_as<T, u8string>)
    {
        return "u8string";
    }
    else if constexpr (same_as<T, u16string>)
    {
        return "u16string";
    }
    else if constexpr (same_as<T, u32string>)
    {
        return "u32string";
    }
    else if constexpr (same_as<T, wstring>)
    {
        return "wstring";
    }
    else
    {
        return "unknown";
    }
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for convert_unicode_to_ascii and "
                             "convert_ascii_to_unicode",
         .path             = "abc_test_test::utility::str::unicode",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_type_name{typeid(T).name()};
        auto _l_name{get_name<T>()};
        _TVLOG(_l_name);
        using CharT = typename T::value_type;
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing convert_unicode_to_ascii and "
                "convert_ascii_to_unicode using {0}",
                _l_type_name
            )
        );
        // Tests converting a unicode string to an ascii string.
        // Anything outside of the ascii range is caught.
        using unit_test_data_1 = tuple<T, result_t<string>>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name),
                 fmt::format("unit_test_1", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str, _l_result]{_l_data};
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(convert_unicode_to_ascii(_l_unicode_str) == _l_result)

                && _EXPR(
                    convert_unicode_to_ascii(T(_l_unicode_str)) == _l_result
                )

            );
        }
        // Running unit tests the other way.
        using unit_test_data_3 = std::tuple<string, result_t<T>>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_3>(
                 fmt::format("unit_test_2_{0}", _l_name),
                 fmt::format("unit_test_2", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_str, _l_result]{_l_data};
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(convert_ascii_to_unicode<CharT>(_l_str) == _l_result)
                && _EXPR(
                    convert_ascii_to_unicode<CharT>(string(_l_str)) == _l_result
                )
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing convert_unicode_to_ascii, "
                "checkless_convert_unicode_to_ascii, "
                "convert_ascii_to_unicode and "
                "checkless_convert_ascii_to_unicode using {0}",
                _l_type_name
            )
        );
        // Either generate valid unicode strings, or (potentially) invalid
        // unicode strings.
        using fuzzy_test_data_1 = std::tuple<T>;
        for (const auto& _l_data :
             generate_data_randomly<fuzzy_test_data_1>()
                 & generate_data_randomly<fuzzy_test_data_1>(
                     default_random_generator<fuzzy_test_data_1>(
                         default_random_generator<T>(
                             default_random_generator<typename T::value_type>()
                         )
                     )
                 ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(convert_unicode_to_ascii(_l_unicode_str));
            do_not_optimise(convert_unicode_to_ascii(T(_l_unicode_str)));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        // Same as above; generate valid ascii strings and invalid strings.
        using fuzzy_test_data_2 = std::tuple<string>;
        for (const auto& _l_data :
             generate_data_randomly<fuzzy_test_data_2>()
                 & generate_data_randomly<fuzzy_test_data_2>(
                     default_random_generator<fuzzy_test_data_2>(
                         default_random_generator<string>(
                             default_random_generator<char>()
                         )
                     )
                 ))
        {
            _TVLOG(_l_data);
            const auto& [_l_str]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(convert_ascii_to_unicode<CharT>(_l_str));
            do_not_optimise(convert_ascii_to_unicode<CharT>(string(_l_str)));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format(
                "Property testing convert_unicode_to_ascii, "
                "and convert_ascii_to_unicode using {0}",
                _l_type_name
            )
        );
        using property_test_data_1 = std::tuple<string>;
        // This will generate strings with characters in the ASCII range, and
        // convert ascii/convert unicode will always work.
        for (const auto& _l_data :
             generate_data_randomly<property_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_str]{_l_data};
            auto _l_f1 = [](const T& _a_str)
            {
                return convert_unicode_to_ascii(_a_str);
            };
            _l_property_tests += _BLOCK_CHECK(
                _EXPR(
                    convert_ascii_to_unicode<CharT>(_l_str).and_then(_l_f1)
                    == _l_str
                )
                && _EXPR(
                    convert_ascii_to_unicode<CharT>(string(_l_str))
                        .and_then(_l_f1)
                    == _l_str
                )
            );
        }
        _END_BBA_CHECK(_l_property_tests);
    };
    using data_types_t = abc::utility::type_list<
     u8string, u16string, u32string, wstring>;
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
        {.name             = "Tests for unicode_conversion",
         .path             = "abc_test_test::utility::str::conversion",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    auto _l_test_func = [&]<typename T, typename U>()
    {
        auto _l_name{get_name<T>()};
        using CharT = typename T::value_type;
        using CharU = typename U::value_type;
        auto _l_type_name{
            fmt::format("{0} {1}", typeid(T).name(), typeid(U).name())
        };
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format("Unit testing unicode_conversion {0}", _l_type_name)
        );
        using unit_test_data_1 = std::tuple<T, U>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str1, _l_unicode_str2]{_l_data};
            /*_l_unit_tests += _BLOCK_CHECK(
                _EXPR(
                    checkless_unicode_conversion<CharU>(_l_unicode_str1)
                    == _l_unicode_str2
                )
                && _EXPR(
                    checkless_unicode_conversion<CharT>(_l_unicode_str2)
                    == _l_unicode_str1
                )
                && _EXPR(
                    checkless_unicode_conversion<CharU>(T(_l_unicode_str1))
                    == _l_unicode_str2
                )
                && _EXPR(
                    checkless_unicode_conversion<CharT>(U(_l_unicode_str2))
                    == _l_unicode_str1
                )
            );*/
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing unicode_conversion using {0}", _l_type_name
            )
        );
        using fuzzy_test_data_1 = std::tuple<T, U>;
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str1, _l_unicode_str2]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            // do_not_optimise(checkless_unicode_conversion<CharU>(_l_unicode_str1)
            // );
            // do_not_optimise(checkless_unicode_conversion<CharT>(_l_unicode_str2)
            // );
            // do_not_optimise(
            //     checkless_unicode_conversion<CharU>(T(_l_unicode_str1))
            // );
            // do_not_optimise(
            // checkless_unicode_conversion<CharT>(U(_l_unicode_str2))
            // );
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format(
                "Property testing unicode_conversion using {0}", _l_type_name
            )
        );
        using property_test_data_1 = std::tuple<T>;
        for (const auto& _l_data :
             generate_data_randomly<property_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_str]{_l_data};
            /*_l_property_tests += _BLOCK_CHECK(
                _EXPR(
                    checkless_unicode_conversion<CharT>(
                        checkless_unicode_conversion<CharU>(_l_str)
                    )
                    == _l_str
                )
                && _EXPR(
                    checkless_unicode_conversion<CharT>(
                        checkless_unicode_conversion<CharU>(T(_l_str))
                    )
                    == _l_str
                )
            );*/
        }

        _END_BBA_CHECK(_l_property_tests);
    };
    using data_types_t
        = abc::utility::type_list<u8string, u16string, u32string, wstring>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T1>()
        {
            RUN(_l_mdg,
                (
                    [&]()
                    {
                        manual_data_generator_t _l_mdg2;
                        for_each_type<data_types_t>(
                            [&]<typename T2>()
                            {
                                RUN(_l_mdg2,
                                    (_l_test_func.operator()<T1, T2>()));
                            }
                        );
                    }
                ));
        }
    );
}

/*_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Tests for pack_u8string_into_string and
unpack_string_to_u8string", .path             =
"abc_test_test::utility::str::conversion", .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        fmt::format("Unit testing pack_u8string_into_string and
unpack_string_to_u8string")
    );
    using unit_test_data_1 = std::tuple<>;
    for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
             fmt::format("unit_test_1_{0}", _l_type_name)
         ))
    {
        _TVLOG(_l_data);
        const auto& [_l_unicode_str1, _l_unicode_str2]{_l_data};
        _l_unit_tests += _BLOCK_CHECK(
            _EXPR(unicode_conversion<U>(_l_unicode_str1) == _l_unicode_str2)
            && _EXPR(unicode_conversion<T>(_l_unicode_str2) ==
_l_unicode_str1)
            && _EXPR(
                unicode_conversion<U>(T(_l_unicode_str1)) == _l_unicode_str2
            )
            && _EXPR(
                unicode_conversion<T>(U(_l_unicode_str2)) == _l_unicode_str1
            )
        );
    }
    _END_BBA_CHECK(_l_unit_tests);

    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests,
        fmt::format("Fuzzy testing unicode_conversion using {0}",
_l_type_name)
    );
    using fuzzy_test_data_1 = std::tuple<T, U>;
    for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
    {
        _TVLOG(_l_data);
        const auto& [_l_unicode_str1, _l_unicode_str2]{_l_data};
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(unicode_conversion<U>(_l_unicode_str1));
        do_not_optimise(unicode_conversion<T>(_l_unicode_str2));
        do_not_optimise(unicode_conversion<U>(T(_l_unicode_str1)));
        do_not_optimise(unicode_conversion<T>(U(_l_unicode_str2)));
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);

    _BEGIN_MULTI_ELEMENT_BBA(
        _l_property_tests,
        fmt::format(
            "Property testing unicode_conversion using {0}", _l_type_name
        )
    );
    using property_test_data_1 = std::tuple<T>;
    for (const auto& _l_data :
generate_data_randomly<property_test_data_1>())
    {
        _TVLOG(_l_data);
        const auto& [_l_str]{_l_data};
        _l_property_tests += _BLOCK_CHECK(
            _EXPR(
                unicode_conversion<T>(unicode_conversion<U>(_l_str)) ==
_l_str
            )
            && _EXPR(
                unicode_conversion<T>(unicode_conversion<U>(T(_l_str)))
                == _l_str
            )
        );
    }

    _END_BBA_CHECK(_l_property_tests);
}*/