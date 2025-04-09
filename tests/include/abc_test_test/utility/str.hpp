#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Unit tests for convert_u8string_to_string",
         .path             = "abc_test_test::utility::str",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        _l_unit_tests, "Unit tests for convert_u8string_to_string"
    );
    // Third index is either 0 or 1; 0 if valid result, non-zero if not
    using test_data
        = std::tuple<u8string, std::variant<std::string, std::u8string>>;
    for (const auto& [_l_input_str, _l_output] :
         read_data_from_file<test_data>("unit_tests"))
    {
        const result_t<string> _l_res{convert_u8string_to_string(_l_input_str)};
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
                    convert_string_to_u8string(_l_res.value()).value(),
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
                    convert_string_to_u8string(get<string>(_l_output)).value()
                ));
            }
        }
    }
    _END_BBA_CHECK(_l_unit_tests);
}