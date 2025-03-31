#pragma once

#include "abc_test/core/ds/test_data/pre_test_run_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/pre_test_set_data_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/io/threaded_text_output_reporter.hpp"

_BEGIN_ABC_REPORTERS_NS

struct pre_test_set_data_list_formatter
    : public list_vertical_formattable_t<
          ds::pre_test_run_report_t,
          enum_pre_test_set_data_fields_t,
          print_config_t>
{
public:
    __constexpr virtual bool
        check_data(
            const enum_pre_test_set_data_fields_t& _a_fid,
            const ds::pre_test_run_report_t&       _a_element
        ) const;
    __no_constexpr_or_inline std::optional<std::u8string>
                             get_cli_info(
                                 const enum_pre_test_set_data_fields_t& _a_fid,
                                 const ds::pre_test_run_report_t&       _a_element,
                                 const print_config_t&                  _a_pc
                             ) const;
    __no_constexpr_or_inline std::optional<std::u8string>
                             get_explanation_data(
                                 const enum_pre_test_set_data_fields_t& _a_fid,
                                 const ds::pre_test_run_report_t&       _a_element,
                                 const print_config_t&                  _a_pc
                             ) const;
    __no_constexpr_or_inline std::pair<std::u8string, std::u8string>
                             get_output(
                                 const enum_pre_test_set_data_fields_t& _a_fid,
                                 const ds::pre_test_run_report_t&       _a_element,
                                 const print_config_t&                  _a_pc
                             ) const;
    __no_constexpr_or_inline virtual std::pair<std::u8string, std::vector<std::u8string>>
        get_data(
            const enum_pre_test_set_data_fields_t&                   _a_fid,
            const ds::pre_test_run_report_t&                         _a_element,
            const print_config_t&                                    _a_pc
           
        ) const;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp bool
    pre_test_set_data_list_formatter::check_data(
        const enum_pre_test_set_data_fields_t& _a_fid,
        const ds::pre_test_run_report_t&       _a_element
    ) const
{
    using enum enum_pre_test_set_data_fields_t;
    switch (_a_fid)
    {
    case USE_GLOBAL_TEST_LIST:
    case WRITE_DATA_TO_FILES:
    case PATH_DELIMITER:
    case ROOT_PATH:
    case THREADS:
    case COMMENT_STR:
    case GENERAL_DATA_FILE_EXTENSION:
    case N_INTEGERS_USED_FOR_CREATION_OF_RNG:
    case REPETITION_CONFIG_USED:
    case GLOBAL_SEED:
    case FORCE_RUN_ALL_TESTS:
    case TEST_PATHS_TO_RUN:
        return true;
    default:
        throw errors::unaccounted_for_enum_exception(_a_fid);
    }
}

_END_ABC_REPORTERS_NS