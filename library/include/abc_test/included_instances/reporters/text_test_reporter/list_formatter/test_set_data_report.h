#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/test_set_data_report.h"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.h"
#include "abc_test/internal/ds/test_data/test_set_data.h"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.h"
_BEGIN_ABC_REPORTERS_NS

struct test_set_data_list_formatter
    : public list_formattable_t<
    ds::test_set_data_t,
    enum_test_set_data_fields_t,
    print_config_t>
{
public:
    __constexpr virtual bool
        check_data(
            const enum_test_set_data_fields_t& _a_fid,
            const ds::test_set_data_t& _a_element
        ) const;
    __constexpr virtual std::vector<std::string>
        get_data(
            const enum_test_set_data_fields_t& _a_fid,
            const ds::test_set_data_t& _a_element,
            const print_config_t& _a_pc
        ) const;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp bool
test_set_data_list_formatter::check_data(
    const enum_test_set_data_fields_t& _a_fid,
    const ds::test_set_data_t& _a_element
) const
{
    using enum enum_test_set_data_fields_t;
    switch (_a_fid)
    {
    case TOTAL_TESTS_RAN:
        return true;
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

__constexpr_imp std::vector<std::string>
test_set_data_list_formatter::get_data(
    const enum_test_set_data_fields_t& _a_fid,
    const ds::test_set_data_t& _a_element,
    const print_config_t& _a_pc
) const
{
    using namespace std;
    using enum enum_test_set_data_fields_t;
    switch (_a_fid)
    {
    case TOTAL_TESTS_RAN:
        return {
            _a_pc.space(_a_pc.colon(_a_pc.str_total_tests_ran())),
            _a_pc.print_integer(_a_element.total_tests_ran())
        };
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

_END_ABC_REPORTERS_NS