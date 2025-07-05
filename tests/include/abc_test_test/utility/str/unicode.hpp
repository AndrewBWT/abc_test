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
    using namespace _ABC_NS_UTILITY_STR;
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
        for (const auto& [_l_unicode_str, _l_result] : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name),
                 fmt::format("unit_test_1", _l_name)
             ))
        {
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(convert_unicode_to_ascii(_l_unicode_str) != _l_result)
                && _EXPR(
                    convert_unicode_to_ascii(T(_l_unicode_str)) == _l_result
                )
            );
        }
        // Running unit tests the other way.
        using unit_test_data_3 = std::tuple<string, result_t<T>>;
        for (const auto& [_l_str, _l_result] : read_data_from_file<unit_test_data_3>(
                 fmt::format("unit_test_2", _l_name)
             ))
        {
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
        for (const auto& [_l_unicode_str] :
             generate_data_randomly<fuzzy_test_data_1>()
                 & generate_data_randomly<fuzzy_test_data_1>(
                     default_random_generator<fuzzy_test_data_1>(
                         default_random_generator<T>(
                             default_random_generator<typename T::value_type>()
                         )
                     )
                 ))
        {
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(convert_unicode_to_ascii(_l_unicode_str));
            do_not_optimise(convert_unicode_to_ascii(T(_l_unicode_str)));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        // Same as above; generate valid ascii strings and invalid strings.
        using fuzzy_test_data_2 = std::tuple<string>;
        for (const auto& [_l_str] :
             generate_data_randomly<fuzzy_test_data_2>()
                 & generate_data_randomly<fuzzy_test_data_2>(
                     default_random_generator<fuzzy_test_data_2>(
                         default_random_generator<string>(
                             default_random_generator<char>()
                         )
                     )
                 ))
        {
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
        for (const auto& [_l_str] :
             generate_data_randomly<property_test_data_1>())
        {
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
        using property_test_data_2 = std::tuple<T>;
        for (const auto& [_l_str] : generate_data_randomly<property_test_data_2>(
                 default_random_generator<
                     property_test_data_2>(default_random_generator<T>(
                     default_random_generator<typename T::value_type>(0x0, 0x7F)
                 ))
             ))
        {
            auto _l_f1 = [](const std::string& _a_str)
            {
                return convert_ascii_to_unicode<CharT>(_a_str);
            };
            _l_property_tests += _BLOCK_CHECK(
                _EXPR(
                    convert_unicode_to_ascii(_l_str).and_then(_l_f1) == _l_str
                )
                && _EXPR(
                    convert_unicode_to_ascii(T(_l_str)).and_then(_l_f1)
                    == _l_str
                )
            );
        }
        _END_BBA_CHECK(_l_property_tests);
    };
    using data_types_t
        = abc::utility::type_list<u8string, u16string, u32string, wstring>;
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
        {.name
         = "Tests for unicode_conversion_with_exception and unicode_conversion",
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
    using namespace _ABC_NS_UTILITY_STR;
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
            fmt::format(
                "Unit testing unicode_conversion_with_exception and "
                "unicode_conversion {0}",
                _l_type_name
            )
        );
        using unit_test_data_1
            = std::tuple<T, variant<U, pair<u8string, u8string>>>;
        for (const auto& [_l_unicode_str1, _l_result] : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1", _l_name),
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            // Checking conversion one way.
            result_t<U> _l_result_str;
            switch (_l_result.index())
            {
            case 0:
                _l_result_str = get<0>(_l_result);
                break;
            case 1:
                _l_result_str = unexpected(get<1>(_l_result).first);
                break;
            }
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(
                    unicode_conversion<CharU>(_l_unicode_str1) == _l_result_str
                )
                && _EXPR(
                    unicode_conversion<CharU>(T(_l_unicode_str1))
                    == _l_result_str
                )
            );
            if (_l_result.index() == 0)
            {
                // If it has a positive result, we can run checking with
                // exception and run the function backwards.
                const auto& _l_unicode_str2{get<0>(_l_result)};
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(
                        unicode_conversion_with_exception<CharU>(_l_unicode_str1
                        )
                        == _l_unicode_str2
                    )
                    && _EXPR(
                        unicode_conversion_with_exception<CharU>(
                            T(_l_unicode_str1)
                        )
                        == _l_unicode_str2
                    )
                    && _EXPR(
                        unicode_conversion_with_exception<CharT>(_l_unicode_str2
                        )
                        == _l_unicode_str1
                    )
                    && _EXPR(
                        unicode_conversion_with_exception<CharT>(
                            U(_l_unicode_str2)
                        )
                        == _l_unicode_str1
                    )
                    && _EXPR(
                        unicode_conversion<CharT>(_l_unicode_str2)
                        == abc::result_t<basic_string<CharT>>(_l_unicode_str1)
                    )
                    && _EXPR(
                        unicode_conversion<CharT>(U(_l_unicode_str2))
                        == abc::result_t<basic_string<CharT>>(_l_unicode_str1)
                    )
                );
            }
            else
            {
                const auto _l_expection_str{get<1>(_l_result).second};
                // If theres an error, let us check for the exception.
                _l_unit_tests += _BLOCK_CHECK(
                    _MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                        errors::test_library_exception_t,
                        _l_expection_str,
                        [&]()
                        {
                            do_not_optimise(
                                unicode_conversion_with_exception<CharU>(
                                    _l_unicode_str1
                                )
                            );
                        }
                    )
                );
                _l_unit_tests += _BLOCK_CHECK(
                    _MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(
                        errors::test_library_exception_t,
                        _l_expection_str,
                        [&]()
                        {
                            do_not_optimise(
                                unicode_conversion_with_exception<CharU>(
                                    T(_l_unicode_str1)
                                )
                            );
                        }
                    )
                );
            }
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing unicode_conversion_with_exception and "
                "unicode_conversion using {0}",
                _l_type_name
            )
        );
        using fuzzy_test_data_1 = std::tuple<T>;
        // Normal unicode strings shouldn't throw exceptions.
        for (const auto& [_l_unicode_str1] : generate_data_randomly<fuzzy_test_data_1>())
        {
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(
                unicode_conversion_with_exception<CharU>(_l_unicode_str1)
            );
            do_not_optimise(
                unicode_conversion_with_exception<CharU>(T(_l_unicode_str1))
            );
            do_not_optimise(unicode_conversion<CharU>(_l_unicode_str1));
            do_not_optimise(unicode_conversion<CharU>(T(_l_unicode_str1)));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        // These can generate valid or invalid unicode strings.
        using fuzzy_test_data_2 = std::tuple<T>;
        for (const auto& [_l_unicode_str1] : generate_data_randomly<fuzzy_test_data_2>(
                 default_random_generator<fuzzy_test_data_2>(
                     default_random_generator<T>(
                         default_random_generator<CharT>(
                             0, numeric_limits<CharT>::max()
                         )
                     )
                 )
             ))
        {
            matcher_t _l_matcher_1;
            _BEGIN_NO_THROW_MATCHER(_l_matcher_1);
            do_not_optimise(unicode_conversion<CharU>(_l_unicode_str1));
            do_not_optimise(unicode_conversion<CharU>(T(_l_unicode_str1)));
            _END_NO_THROW_MATCHER(_l_matcher_1);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher_1);
            matcher_t _l_matcher;
            if (is_valid_unicode_string(_l_unicode_str1))
            {
                _BEGIN_NO_THROW_MATCHER(_l_matcher);
                do_not_optimise(
                    unicode_conversion_with_exception<CharU>(_l_unicode_str1)
                );
                do_not_optimise(
                    unicode_conversion_with_exception<CharU>(T(_l_unicode_str1))
                );
                do_not_optimise(unicode_conversion<CharU>(_l_unicode_str1));
                do_not_optimise(unicode_conversion<CharU>(T(_l_unicode_str1)));
                _END_NO_THROW_MATCHER(_l_matcher);
                _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
            }
            else
            {
                _l_fuzzy_tests
                    += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE(
                        errors::test_library_exception_t,
                        [&]()
                        {
                            do_not_optimise(
                                unicode_conversion_with_exception<CharU>(
                                    _l_unicode_str1
                                )
                            );
                        }
                    ));
                _l_fuzzy_tests
                    += _BLOCK_CHECK(_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE(
                        errors::test_library_exception_t,
                        [&]()
                        {
                            do_not_optimise(
                                unicode_conversion_with_exception<CharU>(
                                    T(_l_unicode_str1)
                                )
                            );
                        }
                    ));
            }
        }
        _END_BBA_CHECK(_l_fuzzy_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_property_tests,
            fmt::format(
                "Property testing unicode_conversion using {0}", _l_type_name
            )
        );
        // Valid unicode strings should be able to go back and forth.
        using property_test_data_1 = std::tuple<T>;
        for (const auto& [_l_str] :
             generate_data_randomly<property_test_data_1>())
        {
            _l_property_tests += _BLOCK_CHECK(
                _EXPR(
                    unicode_conversion_with_exception<CharT>(
                        unicode_conversion_with_exception<CharU>(_l_str)
                    )
                    == _l_str
                )
                && _EXPR(
                    unicode_conversion_with_exception<CharT>(
                        unicode_conversion_with_exception<CharU>(T(_l_str))
                    )
                    == _l_str
                )
                && _EXPR(
                    unicode_conversion<CharU>(_l_str).and_then(
                        [&](auto _a_str)
                        {
                            return unicode_conversion<CharT>(_a_str);
                        }
                    )
                    == result_t<decltype(_l_str)>(_l_str)
                )
                && _EXPR(
                    unicode_conversion<CharU>(_l_str).and_then(
                        [&](auto _a_str)
                        {
                            return unicode_conversion<CharT>(_a_str);
                        }
                    )
                    == result_t<decltype(_l_str)>(_l_str)
                )
            );
        }

        _END_BBA_CHECK(_l_property_tests);
    };
    using data_types_t
        = abc::utility::type_list<u8string, u16string, u32string, wstring>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T1>()
        {
            auto _l_second_level = [&]()
            {
                manual_data_generator_t _l_mdg2;
                for_each_type<data_types_t>(
                    [&]<typename T2>()
                    {
                        RUN(_l_mdg2, (_l_test_func.operator()<T1, T2>()));
                    }
                );
            };
            RUN(_l_mdg, (_l_second_level()));
        }
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for is_valid_unicode_string",
         .path             = "abc_test_test::utility::str::unicode",
         .threads_required = 1}
    )
)
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_name{get_name<T>()};
        using CharT = typename T::value_type;
        auto _l_type_name{fmt::format("{0}", typeid(T).name())};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing is_valid_unicode_string {0}", _l_type_name
            )
        );
        using unit_test_data_1 = std::tuple<T, bool>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1", _l_name),
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str, _l_result]{_l_data};
            // Checking conversion one way.
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(is_valid_unicode_string(_l_unicode_str) == _l_result)
                && _EXPR(
                    is_valid_unicode_string(T(_l_unicode_str)) == _l_result
                )
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing is_valid_unicode_string using {0}", _l_type_name
            )
        );
        using fuzzy_test_data_1 = std::tuple<T>;
        // Normal unicode strings shouldn't throw exceptions.
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str]{_l_data};
            _l_fuzzy_tests += _BLOCK_CHECK(
                _EXPR(is_valid_unicode_string(_l_unicode_str) == true)
            );
            _l_fuzzy_tests += _BLOCK_CHECK(
                _EXPR(is_valid_unicode_string(T(_l_unicode_str)) == true)
            );
        }
        // These can generate valid or invalid unicode strings. Lets just check
        // that it doesn't throw an exception.
        using fuzzy_test_data_2 = std::tuple<T>;
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_2>(
                 default_random_generator<fuzzy_test_data_2>(
                     default_random_generator<T>(
                         default_random_generator<CharT>(
                             0, numeric_limits<CharT>::max()
                         )
                     )
                 )
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(is_valid_unicode_string(_l_unicode_str));
            do_not_optimise(is_valid_unicode_string(T(_l_unicode_str)));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t
        = abc::utility::type_list<u8string, u16string, u32string, wstring>;
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
        {.name             = "Tests for is_valid_char and is_valid_ascii_char",
         .path             = "abc_test_test::utility::str::unicode",
         .threads_required = 1}
    )
)
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_name{cast_u8string_to_string(type_id<T>())};
        _TVLOG(type_id<T>());
        auto _l_type_name{fmt::format("{0}", typeid(T).name())};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing is_valid_char and is_valid_ascii_char {0}",
                _l_type_name
            )
        );
        using unit_test_data_1 = std::tuple<T, bool>;
        if constexpr (not (same_as<T, char>))
        {
            for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                     fmt::format("unit_test_1_valid_char_{0}", _l_name)
                 ))
            {
                _TVLOG(_l_data);
                const auto& [_l_unicode_char, _l_result_valid_char]{_l_data};
                _l_unit_tests += _BLOCK_CHECK(_EXPR(
                    is_valid_char(_l_unicode_char) == _l_result_valid_char
                ));
            }
        }
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_valid_ascii_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_char, _l_result_valid_ascii]{_l_data};
            _l_unit_tests += _BLOCK_CHECK(_EXPR(
                is_valid_ascii_char(_l_unicode_char) == _l_result_valid_ascii
            ));
        }
        auto kidf = default_enumeration<size_t>();
        for (const auto& _l_data : enumerate_data<T>(
                 from_m_to_n(static_cast<T>(0x00), static_cast<T>(0x7F))
             ))
        {
            _TVLOG(_l_data);
            _l_unit_tests
                += _BLOCK_CHECK(_EXPR(is_valid_ascii_char(_l_data) == true));
            if constexpr (not (same_as<T, char>))
            {
                _l_unit_tests
                    += _BLOCK_CHECK(_EXPR(is_valid_char(_l_data) == true));
            }
        }
        for (const auto& _l_data : enumerate_data<T>(
                 from_m_to_n(static_cast<T>(0x80), static_cast<T>(0xFF))
             ))
        {
            _TVLOG(_l_data);
            _l_unit_tests
                += _BLOCK_CHECK(_EXPR(is_valid_ascii_char(_l_data) == false));
            if constexpr (same_as<T, char8_t>)
            {
                _l_unit_tests
                    += _BLOCK_CHECK(_EXPR(is_valid_char(_l_data) == false));
            }
        }

        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing is_valid_char and is_valid_ascii_char using {0}",
                _l_type_name
            )
        );
        using fuzzy_test_data_1 = std::tuple<T>;
        // Normal unicode strings shouldn't throw exceptions.
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str]{_l_data};
            matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            if constexpr (not (same_as<T, char>))
            {
                do_not_optimise(is_valid_char(_l_unicode_str));
            }
            do_not_optimise(is_valid_ascii_char(_l_unicode_str));
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
        {.name
         = "Tests for next_char32_t and next_char32_t_and_increment_iterator",
         .path             = "abc_test_test::utility::str::unicode",
         .threads_required = 1}
    )
)
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_name{get_name<T>()};
        auto _l_type_name{fmt::format("{0}", typeid(T).name())};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing next_char32_t and "
                "next_char32_t_and_increment_iterator {0}",
                _l_type_name
            )
        );
        using unit_test_data_1
            = tuple<T, vector<tuple<int, variant<pair<char32_t,size_t>,
            pair<u8string,u8string>>>>>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1", _l_name),
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str, _l_inner_test_data]{_l_data};
            typename T::const_iterator _l_begin_c_itt{std::cbegin(_l_unicode_str
            )};
            typename T::const_iterator _l_end_c_itt{std::cend(_l_unicode_str)};
            for (auto& [_l_offset, _l_result] : _l_inner_test_data)
            {
                typename T::const_iterator _l_c_itt{
    std::begin(_l_unicode_str) + _l_offset
                };
                result_t < pair<char32_t, size_t>> _l_next_char32_t_res;
                optional < pair<char32_t, size_t>> _l_next_char32_t_opt;
                result_t<char32_t> _l_next_char32_with_increment_res;
                optional<char32_t> _l_next_char32_with_increment_opt;
                auto _l_expected_itt{ _l_c_itt };
                switch (_l_result.index())
                {
                case 0:
                    _l_next_char32_t_res = get<0>(_l_result);
                    _l_next_char32_t_opt = get<0>(_l_result);
                    _l_next_char32_with_increment_res = get<0>(_l_result).first;
                    _l_next_char32_with_increment_opt = get<0>(_l_result).first;
                    _l_expected_itt += get<0>(_l_result).second;
                    break;
                case 1:
                    _l_next_char32_t_res = unexpected(get<1>(_l_result).first);
                    _l_next_char32_t_opt = std::nullopt;
                    _l_next_char32_with_increment_res = unexpected(get<1>(_l_result).second);
                    _l_next_char32_with_increment_opt = std::nullopt;
                    break;
                }
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(
                        next_char32_t<true>(_l_c_itt, _l_end_c_itt)
                        == _l_next_char32_t_res
                    )
                    && _EXPR(
                        next_char32_t<false>(_l_c_itt, _l_end_c_itt)
                        == _l_next_char32_t_opt
                    )
                );
                auto _l_itt_cpy{_l_c_itt};
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(
                        next_char32_t_and_increment_iterator<true>(
                            _l_itt_cpy, _l_end_c_itt
                        )
                        == _l_next_char32_with_increment_res
                    )
                    && _EXPR(std::distance(_l_expected_itt, _l_itt_cpy) == 0)
                );
                _l_itt_cpy = _l_c_itt;
                _l_unit_tests += _BLOCK_CHECK(
                    _EXPR(
                        next_char32_t_and_increment_iterator<false>(
                            _l_itt_cpy, _l_end_c_itt
                        )
                        == _l_next_char32_with_increment_opt
                    )
                    && _EXPR(std::distance(_l_expected_itt, _l_itt_cpy) == 0)
                );
            }
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format("Fuzzy testing next_char32_t using {0}", _l_type_name)
        );
        using fuzzy_test_data_1 = std::tuple<T>;
        // Normal unicode strings shouldn't throw exceptions.
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str]{_l_data};
            typename T::const_iterator _l_begin_c_itt{std::cbegin(_l_unicode_str
            )};
            typename T::const_iterator _l_end_c_itt{std::cend(_l_unicode_str)};
            for (int _l_idx{0};
                 _l_idx < static_cast<int>(std::distance(
                              std::begin(_l_unicode_str), _l_end_c_itt
                          )) + 1;
                 ++_l_idx)
            {
                abc::matcher_t             _l_matcher;
                typename T::const_iterator _l_c_itt{
                    std::begin(_l_unicode_str) + _l_idx
                };
                _BEGIN_NO_THROW_MATCHER(_l_matcher);
                do_not_optimise(next_char32_t<true>(_l_c_itt, _l_end_c_itt));
                do_not_optimise(next_char32_t<false>(_l_c_itt, _l_end_c_itt));
                auto _l_cpy_itt = _l_c_itt;
                do_not_optimise(next_char32_t_and_increment_iterator<true>(_l_cpy_itt, _l_end_c_itt));
                _l_cpy_itt = _l_c_itt;
                do_not_optimise(next_char32_t_and_increment_iterator<false>(_l_cpy_itt, _l_end_c_itt));
                _END_NO_THROW_MATCHER(_l_matcher);
                _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
            }
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t
        = abc::utility::type_list<u8string, u16string, u32string, wstring>;
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
        {.name             = "Tests for unicode_string_to_u8string",
         .path             = "abc_test_test::utility::str::unicode",
         .threads_required = 1}
    )
)
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_name{get_name<T>()};
        auto _l_type_name{fmt::format("{0}", typeid(T).name())};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing unicode_string_to_u8string {0}", _l_type_name
            )
        );
        using unit_test_data_1 = tuple<T, u8string>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1", _l_name),
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str, _l_result]{_l_data};
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(unicode_string_to_u8string(_l_unicode_str) == _l_result)
                && _EXPR(
                    unicode_string_to_u8string(T(_l_unicode_str)) == _l_result
                )
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing unicode_string_to_u8string using {0}",
                _l_type_name
            )
        );
        using fuzzy_test_data_1 = std::tuple<T>;
        // Normal unicode strings shouldn't throw exceptions.
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str]{_l_data};
            abc::matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(unicode_string_to_u8string(_l_unicode_str));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t
        = abc::utility::type_list<u8string, u16string, u32string, wstring>;
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
        {.name             = "Tests for unicode_char_to_u8string",
         .path             = "abc_test_test::utility::str::unicode",
         .threads_required = 1}
    )
)
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_name{typeid(T).name()};
        auto _l_type_name{fmt::format("{0}", typeid(T).name())};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format(
                "Unit testing unicode_char_to_u8string {0}", _l_type_name
            )
        );
        using unit_test_data_1 = tuple<T, u8string>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1", _l_name),
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str, _l_result]{_l_data};
            _l_unit_tests += _BLOCK_CHECK(
                _EXPR(unicode_char_to_u8string(_l_unicode_str) == _l_result)
            );
        }
        _END_BBA_CHECK(_l_unit_tests);

        _BEGIN_MULTI_ELEMENT_BBA(
            _l_fuzzy_tests,
            fmt::format(
                "Fuzzy testing unicode_char_to_u8string using {0}", _l_type_name
            )
        );
        using fuzzy_test_data_1 = std::tuple<T>;
        // Normal unicode strings shouldn't throw exceptions.
        for (const auto& _l_data : generate_data_randomly<fuzzy_test_data_1>())
        {
            _TVLOG(_l_data);
            const auto& [_l_unicode_str]{_l_data};
            abc::matcher_t _l_matcher;
            _BEGIN_NO_THROW_MATCHER(_l_matcher);
            do_not_optimise(unicode_char_to_u8string(_l_unicode_str));
            _END_NO_THROW_MATCHER(_l_matcher);
            _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
        }
        _END_BBA_CHECK(_l_fuzzy_tests);
    };
    using data_types_t
        = abc::utility::type_list<char8_t, char16_t, char32_t, wchar_t>;
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
        {.name             = "Tests for unicode constants",
         .path             = "abc_test_test::utility::str::unicode",
         .threads_required = 1}
    )
)
{
    using namespace std;
    using namespace abc;
    using namespace utility;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_test_func = [&]<typename T>()
    {
        auto _l_name{typeid(T).name()};
        auto _l_type_name{fmt::format("{0}", typeid(T).name())};
        _BEGIN_MULTI_ELEMENT_BBA(
            _l_unit_tests,
            fmt::format("Unit testing unicode constants {0}", _l_type_name)
        );
        using unit_test_data_1 = tuple<T, T, T, T, T, T>;
        for (const auto& _l_data : read_data_from_file<unit_test_data_1>(
                 fmt::format("unit_test_1_{0}", _l_name)
             ))
        {
            _TVLOG(_l_data);
            const auto& [_l_ascii_limit, _l_single_char16_limit, _l_two_char8_limit, _l_high_surrogate_lower, _l_low_surrogate_higher, _l_char32_limit]{
                _l_data
            };
            if constexpr (sizeof(T) >= 1)
            {
                _l_unit_tests
                    += _BLOCK_CHECK(_EXPR(ascii_limit<T>() == _l_ascii_limit));
            }
            if constexpr (not (same_as<T, char>))
            {
                if constexpr (sizeof(T) >= 2)
                {
                    _l_unit_tests += _BLOCK_CHECK(
                        _EXPR(
                            single_char16_limit_and_three_char8_limit<T>()
                            == _l_single_char16_limit
                        )
                        && _EXPR(two_char8_limit<T>() == _l_two_char8_limit)
                        && _EXPR(
                            high_surrogate_lower_value<T>()
                            == _l_high_surrogate_lower
                        )
                        && _EXPR(
                            low_surrogate_upper_value<T>()
                            == _l_low_surrogate_higher
                        )
                    );
                }
                if constexpr (sizeof(T) >= 4)
                {
                    _l_unit_tests += _BLOCK_CHECK(
                        _EXPR(char32_limit<T>() == _l_char32_limit)
                    );
                }
            }
        }
        _END_BBA_CHECK(_l_unit_tests);
    };
    using data_types_t = abc::utility::
        type_list<char, char8_t, char8_t, char16_t, char32_t, wchar_t>;
    manual_data_generator_t _l_mdg;
    for_each_type<data_types_t>(
        [&]<typename T>()
        {
            RUN(_l_mdg, (_l_test_func.operator()<T>()));
        }
    );
}