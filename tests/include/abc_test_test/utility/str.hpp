#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str/conversion.hpp"

// Tests for convert_u8string_to_string.
namespace
{
template <typename T>
inline void
    fuzzy_convert_unicode_string_to_ascii_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests,
        fmt::format(
            "Fuzzy tests for convert_unicode_string_to_ascii_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<T>;
    using CharT     = typename T::value_type;
    for (const auto& [_l_input_str] :
         generate_data_randomly<test_data>()
             & generate_data_randomly<test_data>(
                 default_random_generator<test_data>(
                     default_random_generator<T>(
                         default_random_generator<CharT>()
                     )
                 )
             ))
    {
        _TVLOG(_l_input_str);
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(abc::convert_unicode_string_to_ascii_string(_l_input_str
        ));
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name = "Fuzzy tests for convert_unicode_string_to_ascii_string",
         .path = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (fuzzy_convert_unicode_string_to_ascii_string<u8string>()));
    RUN(_l_mdg, (fuzzy_convert_unicode_string_to_ascii_string<u16string>()));
    RUN(_l_mdg, (fuzzy_convert_unicode_string_to_ascii_string<u32string>()));
    RUN(_l_mdg, (fuzzy_convert_unicode_string_to_ascii_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    unit_convert_unicode_string_to_ascii_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        fmt::format(
            "Unit tests for convert_unicode_string_to_ascii_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = tuple<T, variant<string, u8string>>;
    for (const test_data& _l_input :
         read_data_from_file<test_data>("unit_test"))
    {
        _TVLOG(_l_input);
        const auto& [_l_str, _l_output]{_l_input};
        const result_t<string> _l_res{
            convert_unicode_string_to_ascii_string(_l_str)
        };
        if (_l_res.has_value())
        {
            if (holds_alternative<string>(_l_output))
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.value(), get<string>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_unicode_string_to_ascii_string returned valid "
                    u8"result of "
                    u8"\"{0}\", however unit test suggested a failure should "
                    u8"occour, with the result message being \"{1}\".",
                    checkless_convert_ascii_to_unicode_string<u8string>(
                        _l_res.value()
                    ),
                    get<u8string>(_l_output)
                ));
            }
        }
        else
        {
            if (holds_alternative<u8string>(_l_output))
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.error(), get<u8string>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_u8string_to_string returned a failure result of "
                    u8"\"{0}\", however unit test suggested a valid result "
                    u8"should have occoured, with the result message being "
                    u8"\"{1}\".",
                    _l_res.error(),
                    abc::checkless_convert_ascii_to_unicode_string<u8string>(
                        get<string>(_l_output)
                    )
                ));
            }
        }
    }
    _END_BBA_CHECK(_l_unit_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests for convert_unicode_string_to_ascii_string",
         .path = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (unit_convert_unicode_string_to_ascii_string<u8string>()));
    RUN(_l_mdg, (unit_convert_unicode_string_to_ascii_string<u16string>()));
    RUN(_l_mdg, (unit_convert_unicode_string_to_ascii_string<u32string>()));
    RUN(_l_mdg, (unit_convert_unicode_string_to_ascii_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    fuzzy_checkless_convert_unicode_string_to_ascii_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests,
        fmt::format(
            "Fuzzy tests for checkless_convert_unicode_string_to_ascii_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<T>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(
            abc::checkless_convert_unicode_string_to_ascii_string(_l_input_str)
        );
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Fuzzy tests for checkless_convert_unicode_string_to_ascii_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (fuzzy_checkless_convert_unicode_string_to_ascii_string<u8string>()));
    RUN(_l_mdg,
        (fuzzy_checkless_convert_unicode_string_to_ascii_string<u16string>()));
    RUN(_l_mdg,
        (fuzzy_checkless_convert_unicode_string_to_ascii_string<u32string>()));
    RUN(_l_mdg,
        (fuzzy_checkless_convert_unicode_string_to_ascii_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    unit_checkless_convert_unicode_string_to_ascii_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        fmt::format(
            "Unit tests for checkless_convert_unicode_string_to_ascii_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = tuple<T, string>;
    for (const test_data& _l_input :
         read_data_from_file<test_data>("unit_test"))
    {
        _TVLOG(_l_input);
        const auto& [_l_str, _l_output]{_l_input};
        const string _l_res{
            checkless_convert_unicode_string_to_ascii_string(_l_str)
        };
        _l_unit_tests += _BLOCK_CHECK(strings_equal(_l_res, _l_output));
    }
    _END_BBA_CHECK(_l_unit_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Unit tests for checkless_convert_unicode_string_to_ascii_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg,
        (unit_checkless_convert_unicode_string_to_ascii_string<u8string>()));
    RUN(_l_mdg,
        (unit_checkless_convert_unicode_string_to_ascii_string<u16string>()));
    RUN(_l_mdg,
        (unit_checkless_convert_unicode_string_to_ascii_string<u32string>()));
    RUN(_l_mdg,
        (unit_checkless_convert_unicode_string_to_ascii_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    fuzzy_convert_ascii_to_unicode_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests,
        fmt::format(
            "Fuzzy tests for convert_ascii_to_unicode_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<std::string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(abc::convert_ascii_to_unicode_string<T>(_l_input_str));
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for convert_ascii_to_unicode_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (fuzzy_convert_ascii_to_unicode_string<u8string>()));
    RUN(_l_mdg, (fuzzy_convert_ascii_to_unicode_string<u16string>()));
    RUN(_l_mdg, (fuzzy_convert_ascii_to_unicode_string<u32string>()));
    RUN(_l_mdg, (fuzzy_convert_unicode_string_to_ascii_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    unit_convert_ascii_to_unicode_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        fmt::format(
            "Unit tests for convert_ascii_to_unicode_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = tuple<string, variant<T, u8string>>;
    for (const test_data& _l_input :
         read_data_from_file<test_data>("unit_test"))
    {
        _TVLOG(_l_input);
        const auto& [_l_str, _l_output]{_l_input};
        const result_t<T> _l_res{convert_ascii_to_unicode_string<T>(_l_str)};
        if (_l_res.has_value())
        {
            if (_l_output.index() == 0)
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.value(), get<0>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_ascii_to_unicode_string returned valid result "
                    u8"of "
                    u8"\"{0}\", however unit test suggested a failure should "
                    u8"occour, with the result message being \"{1}\".",
                    unicode_conversion<u8string>(_l_res.value()),
                    get<1>(_l_output)
                ));
            }
        }
        else
        {
            if (_l_output.index() == 1)
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.error(), get<1>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_ascii_to_unicode_string returned a failure "
                    u8"result of "
                    u8"\"{0}\", however unit test suggested a valid result "
                    u8"should have occoured, with the result message being "
                    u8"\"{1}\".",
                    _l_res.error(),
                    abc::unicode_conversion<u8string>(get<0>(_l_output))
                ));
            }
        }
    }
    _END_BBA_CHECK(_l_unit_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for convert_ascii_to_unicode_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (unit_convert_ascii_to_unicode_string<u8string>()));
    RUN(_l_mdg, (unit_convert_ascii_to_unicode_string<u16string>()));
    RUN(_l_mdg, (unit_convert_ascii_to_unicode_string<u32string>()));
    RUN(_l_mdg, (unit_convert_ascii_to_unicode_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    fuzzy_checkless_convert_ascii_to_unicode_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests,
        fmt::format(
            "Fuzzy tests for checkless_convert_ascii_to_unicode_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        matcher_t _l_matcher;
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(
            abc::checkless_convert_ascii_to_unicode_string<T>(_l_input_str)
        );
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name = "Fuzzy tests for checkless_convert_ascii_to_unicode_string",
         .path = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (fuzzy_checkless_convert_ascii_to_unicode_string<u8string>()));
    RUN(_l_mdg, (fuzzy_checkless_convert_ascii_to_unicode_string<u16string>()));
    RUN(_l_mdg, (fuzzy_checkless_convert_ascii_to_unicode_string<u32string>()));
    RUN(_l_mdg, (fuzzy_checkless_convert_ascii_to_unicode_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    unit_checkless_convert_ascii_to_unicode_string()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        fmt::format(
            "Unit tests for checkless_convert_ascii_to_unicode_string "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = tuple<string, T>;
    for (const test_data& _l_input :
         read_data_from_file<test_data>("unit_test"))
    {
        _TVLOG(_l_input);
        const auto& [_l_str, _l_output]{_l_input};
        const T _l_res{checkless_convert_ascii_to_unicode_string<T>(_l_str)};
        _l_unit_tests += _BLOCK_CHECK(strings_equal(_l_res, _l_output));
    }
    _END_BBA_CHECK(_l_unit_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests for checkless_convert_ascii_to_unicode_string",
         .path = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (unit_checkless_convert_ascii_to_unicode_string<u8string>()));
    RUN(_l_mdg, (unit_checkless_convert_ascii_to_unicode_string<u16string>()));
    RUN(_l_mdg, (unit_checkless_convert_ascii_to_unicode_string<u32string>()));
    RUN(_l_mdg, (unit_checkless_convert_ascii_to_unicode_string<wstring>()));
}

namespace
{
template <typename T>
inline void
    property_checkless_convert_ascii()
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_property_tests,
        fmt::format(
            "Property tests for checkless_convert_ascii functions "
            "using {0}",
            typeid(T).name()
        )
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data_1 = std::tuple<string>;
    /*for (const auto& [_l_input_str] : generate_data_randomly<test_data_1>())
    {
        _TVLOG(_l_input_str);
        _l_property_tests += _BLOCK_CHECK(strings_equal(
            _l_input_str,
            checkless_convert_unicode_string_to_ascii_string(
                checkless_convert_ascii_to_unicode_string<T>(_l_input_str)
            )
        ));
    }*/
    using test_data_2 = T;
    using CharT       = typename T::value_type;
    for (const auto& _l_input_str :
         generate_data_randomly<test_data_2>(default_random_generator<T>(
             default_random_generator<CharT>(0, 255)
         )))
    {
        _TVLOG(_l_input_str);
        _l_property_tests += _BLOCK_CHECK(strings_equal(
            _l_input_str,
            checkless_convert_ascii_to_unicode_string<T>(
                checkless_convert_unicode_string_to_ascii_string(_l_input_str)
            )
        ));
    }
    _END_BBA_CHECK(_l_property_tests);
}
} // namespace

_TEST_CASE(
    abc::test_case_t(
        {.name = "Property tests for checkless_convert_ascii functions",
         .path = "abc_test_test::utility::str::property",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    for (auto& k : iterate_over<int>({ 1 }))
    {
        _CHECK(_EXPR(1 == 1));
    }
    for (auto& k : iterate_over<int>({ 1 }))
    {
        _CHECK(_EXPR(1 == 1));
    }
    manual_data_generator_t _l_mdg;
    if (_l_mdg.run_next()) {
        (property_checkless_convert_ascii<u8string>());
    } ++_l_mdg;;
    //RUN(_l_mdg, (property_checkless_convert_ascii<u16string>()));
    //RUN(_l_mdg, (property_checkless_convert_ascii<u32string>()));
    //RUN(_l_mdg, (property_checkless_convert_ascii<wstring>()));
}

/*_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Fuzzy tests for checkless_convert_unicode_string_to_ascii_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests,
        "Fuzzy tests for checkless_convert_unicode_string_to_ascii_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u8string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(
            abc::checkless_convert_unicode_string_to_ascii_string(_l_input_str)
        );
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Unit tests for checkless_convert_unicode_string_to_ascii_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests,
        "Unit tests for checkless_convert_unicode_string_to_ascii_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data
        = std::tuple<u8string, std::variant<std::string, std::u8string>>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const result_t<string> _l_res{
            convert_unicode_string_to_ascii_string(_l_input_str)
        };
        if (_l_res.has_value())
        {
            if (holds_alternative<string>(_l_output))
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.value(), get<string>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_u8string_to_string returned valid result of "
                    u8"\"{0}\", however unit test suggested a failure should "
                    u8"occour, with the result message being \"{1}\".",
                    checkless_convert_ascii_to_unicode_string<u8string>(
                        _l_res.value()
                    ),
                    get<u8string>(_l_output)
                ));
            }
        }
        else
        {
            if (holds_alternative<u8string>(_l_output))
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.error(), get<u8string>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_u8string_to_string returned a failure result of "
                    u8"\"{0}\", however unit test suggested a valid result "
                    u8"should have occoured, with the result message being "
                    u8"\"{1}\".",
                    _l_res.error(),
                    abc::checkless_convert_ascii_to_unicode_string<u8string>(
                        get<string>(_l_output)
                    )
                ));
            }
        }
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for pack_u8string_into_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, "Fuzzy tests for pack_u8string_into_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u8string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(pack_u8string_into_string(_l_input_str));
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for pack_u8string_into_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for pack_u8string_into_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u8string, std::string>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const string _l_res{pack_u8string_into_string(_l_input_str)};
        _l_unit_tests += _BLOCK_CHECK(strings_equal(_l_res, _l_output));
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for convert_string_to_u8string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, "Fuzzy tests for convert_string_to_u8string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(convert_ascii_to_unicode_string<u8string>(_l_input_str)
        );
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for convert_string_to_u8string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for convert_string_to_u8string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data
        = std::tuple<string, std::variant<std::u8string, std::u8string>>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const result_t<u8string> _l_res{
            convert_ascii_to_unicode_string<u8string>(_l_input_str)
        };
        if (_l_res.has_value())
        {
            if (_l_output.index() == 0)
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.value(), get<0>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_string_to_u8string returned valid result of "
                    u8"\"{0}\", however unit test suggested a failure should "
                    u8"occour, with the result message being \"{1}\".",
                    _l_res.value(),
                    get<1>(_l_output)
                ));
            }
        }
        else
        {
            if (_l_output.index() == 1)
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.error(), get<1>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_string_to_u8string returned a failure result of "
                    u8"\"{0}\", however unit test suggested a valid result "
                    u8"should have occoured, with the result message being "
                    u8"\"{1}\".",
                    _l_res.error(),
                    get<0>(_l_output)
                ));
            }
        }
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for unpack_string_to_u8string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, "Fuzzy tests for unpack_string_to_u8string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(unpack_string_to_u8string(_l_input_str));
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for unpack_string_to_u8string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for unpack_string_to_u8string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<string, std::u8string>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const u8string _l_res{unpack_string_to_u8string(_l_input_str)};
        _l_unit_tests += _BLOCK_CHECK(strings_equal(_l_res, _l_output));
    }
    _END_BBA_CHECK(_l_unit_tests);
}

template <typename T>
__constexpr void
    type_id_test(
        const std::string_view _a_str
    )
{
    using namespace abc;
    using namespace std;
    _CHECK_EXPR(
        type_id<T>()
        == checkless_convert_ascii_to_unicode_string<u8string>(_a_str)
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for type_id",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (type_id_test<int>("int")));
    RUN(_l_mdg, (type_id_test<char>("char")));
    RUN(_l_mdg,
        (type_id_test<vector<int>>("std::vector<int, std::allocator<int>>")));
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for convert_u32string_to_u8string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, "Fuzzy tests for convert_u32string_to_u8string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u32string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(unicode_conversion<u8string>(_l_input_str));
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for convert_u32string_to_u8string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for convert_u32string_to_u8string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u32string, std::u8string>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const u8string _l_res{unicode_conversion<u8string>(_l_input_str)};
        _l_unit_tests += _BLOCK_CHECK(strings_equal(_l_res, _l_output));
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for convert_u8string_to_u32string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, "Fuzzy tests for convert_u8string_to_u32string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u8string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(unicode_conversion<u32string>(_l_input_str));
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for convert_u8string_to_u32string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for convert_u8string_to_u32string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u8string, std::u32string>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const u32string _l_res{unicode_conversion<u32string>(_l_input_str)};
        _l_unit_tests += _BLOCK_CHECK(strings_equal(_l_res, _l_output));
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Fuzzy tests for convert_u32string_to_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, "Fuzzy tests for convert_u32string_to_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u32string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(convert_unicode_string_to_ascii_string(_l_input_str));
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for convert_u32string_to_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for convert_u32string_to_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data
        = std::tuple<u32string, std::variant<std::string, std::u8string>>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const result_t<string> _l_res{
            convert_unicode_string_to_ascii_string(_l_input_str)
        };
        if (_l_res.has_value())
        {
            if (holds_alternative<string>(_l_output))
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.value(), get<string>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_u32string_to_string returned valid result of "
                    u8"\"{0}\", however unit test suggested a failure should "
                    u8"occour, with the result message being \"{1}\".",
                    checkless_convert_ascii_to_unicode_string<u8string>(
                        _l_res.value()
                    ),
                    get<u8string>(_l_output)
                ));
            }
        }
        else
        {
            if (holds_alternative<u8string>(_l_output))
            {
                _l_unit_tests += _BLOCK_CHECK(
                    strings_equal(_l_res.error(), get<u8string>(_l_output))
                );
            }
            else
            {
                _l_unit_tests += _BLOCK_FAIL_WITH_MSG(fmt::format(
                    u8"convert_u32string_to_string returned a failure result "
                    u8"of "
                    u8"\"{0}\", however unit test suggested a valid result "
                    u8"should have occoured, with the result message being "
                    u8"\"{1}\".",
                    _l_res.error(),
                    checkless_convert_ascii_to_unicode_string<u8string>(
                        get<string>(_l_output)
                    )
                ));
            }
        }
    }
    _END_BBA_CHECK(_l_unit_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Fuzzy tests for checkless_convert_u32string_to_string",
         .path = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_fuzzy_tests, "Fuzzy tests for checkless_convert_u32string_to_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u32string>;
    for (const auto& [_l_input_str] : generate_data_randomly<test_data>())
    {
        _TVLOG(_l_input_str);
        _BEGIN_NO_THROW_MATCHER(_l_matcher);
        do_not_optimise(
            checkless_convert_unicode_string_to_ascii_string(_l_input_str)
        );
        // End the checks for an exception being thrown.
        _END_NO_THROW_MATCHER(_l_matcher);
        _l_fuzzy_tests += _BLOCK_CHECK(_l_matcher);
    }
    _END_BBA_CHECK(_l_fuzzy_tests);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Unit tests for checkless_convert_u32string_to_string",
         .path = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for checkless_convert_u32string_to_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data = std::tuple<u32string, std::string>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const string _l_res{
            checkless_convert_unicode_string_to_ascii_string(_l_input_str)
        };
        _l_unit_tests += _BLOCK_CHECK(strings_equal(_l_res, _l_output));
    }
    _END_BBA_CHECK(_l_unit_tests);
}*/