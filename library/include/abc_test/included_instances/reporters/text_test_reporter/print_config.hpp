#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/after_execution_test_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/finalised_test_set_data_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_single_line.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/multi_element_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/static_assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_exception.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_non_descript_entity.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp"
#include "abc_test/internal/matchers/matcher_result.hpp"
#include "abc_test/internal/test_reports/assertion_status/fail.hpp"
#include "abc_test/internal/test_reports/assertion_status/pass.hpp"
#include "abc_test/internal/test_reports/assertion_status/pass_or_fail.hpp"
#include "abc_test/internal/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/internal/test_reports/assertion_status/terminate.hpp"

#include <fmt/color.h>
_BEGIN_ABC_REPORTERS_NS

class print_config_t
{
public:
    __constexpr
                      print_config_t(const bool _a_colours_enabled) noexcept;
    __constexpr       std::string
                      line_break() const noexcept;
    __constexpr const std::string
                      matcher_output_str(const bool _a_passed) const noexcept;
    __constexpr const std::string_view
                      matcher_source_map_str() const noexcept;
    __constexpr       std::string
                      matcher_output(const std::string_view) const noexcept;
    __constexpr const std::string_view
                      test_path_str() const noexcept;
    __constexpr const std::string_view
                      str_total_tests_ran() const noexcept;
    __no_constexpr    std::string
                      print_tdg_collection_stack_tries_map(
                          const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t& _a_map
                      ) const noexcept;
    __no_constexpr std::string
        print_seed(const utility::complete_global_seed_t& _a_seed) const;
    __constexpr const std::string_view
                      str_total_tests_passed() const noexcept;
    __constexpr const std::string_view
                      str_total_tests_failed() const noexcept;
    __constexpr const std::string_view
                      str_total_tests_failed_and_terminated() const noexcept;
    __constexpr const std::string_view
        str_total_tests_failed_but_not_terminated() const noexcept;
    __constexpr const std::string_view
        str_total_tests_designated_failure_due_to_exception() const noexcept;
    __constexpr const std::string_view
                      str_total_assertions_ran() const noexcept;
    __constexpr const std::string_view
                      str_total_assertions_passed() const noexcept;
    __constexpr const std::string_view
                      str_total_assertions_failed() const noexcept;
    __constexpr const std::string_view
                      str_seed() const noexcept;
    __constexpr const std::string_view
                      str_tdg_collection_stack_tries_map() const noexcept;
    __constexpr       std::string
        test_path(const std::string_view _a_passed) const noexcept;
    __constexpr const std::string_view
                      seed_used_str() const noexcept;
    __constexpr       std::string
        seed_used(const std::optional<std::string>& _a_opt_seed) const noexcept;
    __constexpr const std::string_view
                      seed_used_in_hex_str() const noexcept;
    __constexpr       std::string
        seed_used_in_hex(const std::optional<std::string>& _a_opt_seed
        ) const noexcept;
    __constexpr const std::string_view
                      seed_to_use_to_re_run_test_str() const noexcept;
    __constexpr       std::string
        seed_to_use_to_re_run_test(const std::string_view _a_passed
        ) const noexcept;
    __constexpr const std::string_view
                      seed_to_use_to_re_run_test_in_hex_str() const noexcept;
    __constexpr       std::string
        seed_to_use_to_re_run_test_in_hex(const std::string_view _a_passed
        ) const noexcept;
    __constexpr const std::string_view
                      test_warnings_recieved_str() const noexcept;
    __constexpr       std::string
        test_warnings_recieved(const std::size_t _a_passed) const noexcept;
    __constexpr const std::string_view
                      assertions_recieved_str() const noexcept;
    __no_constexpr    std::string
                      print_integer(const std::size_t _a_passed) const noexcept;
    __constexpr const std::string_view
                      assertions_passed_str() const noexcept;
    __constexpr       std::string
        assertions_passed(const std::size_t _a_passed) const noexcept;
    __constexpr const std::string_view
                      assertions_failed_str() const noexcept;
    __constexpr       std::string
        assertions_failed(const std::size_t _a_passed) const noexcept;
    __constexpr const std::string_view
                      compat_assertion_data_str() const noexcept;
    __constexpr       std::string
                      compat_assertion_data(
                          const std::size_t _a_passed,
                          const std::size_t _a_failed,
                          const std::size_t _a_total_recieved
                      ) const noexcept;
    __constexpr const std::string_view
                      termination_status_str() const noexcept;
    __constexpr       std::string
        test_status(const ds::enum_test_status_t _a_test_status) const;
    __constexpr const std::string_view
                      passed_str() const noexcept;
    __constexpr       std::string
                      passed(const bool _a_passed) const noexcept;
    __constexpr const std::string_view
                      name_str() const noexcept;
    __constexpr       std::string
                      name(const std::string_view _a_name) const noexcept;
    __constexpr const std::string_view
                      str_status_str() const noexcept;
    __constexpr       std::string
                      str_status(const bool _a_passed) const noexcept;
    __constexpr       enum_print_pair_collection_t<
              combined_enum_matcher_based_assertion_single_line_fields_t>
                matcher_based_assertion_fields() const noexcept;
    __constexpr enum_print_pair_collection_t<
        combined_enum_matcher_based_assertion_block_fields_t>
        matcher_based_assertion_block_fields() const noexcept;
    __constexpr_imp
        enum_print_pair_collection_t<enum_assertion_block_matcher_data_fields_t>
        matcher_assertion_single_block_assertion_list_fields() const noexcept;
    __constexpr_imp
        enum_print_pair_collection_t<enum_assertion_block_matcher_data_fields_t>
        matcher_assertion_multi_block_assertion_list_fields() const noexcept;
    __constexpr enum_print_pair_collection_t<
        combined_enum_multi_element_assertion_block_fields_t>
        multi_element_test_block_fields() const noexcept;
    __constexpr
        enum_print_pair_collection_t<enum_after_execution_test_report_fields_t>
        after_execution_test_report_fields() const noexcept;
    __constexpr
        enum_print_pair_collection_t<enum_finalised_test_set_data_fields_t>
        finalised_test_set_data_fields() const noexcept;
    __constexpr
        enum_print_pair_collection_t<combined_enum_static_assertion_fields_t>
        static_assertion_fields() const noexcept;
    __constexpr enum_print_pair_collection_t<
        combined_enum_unexpected_thrown_non_descript_entity_fields_t>
        unexpected_thrown_non_descript_entity_fields() const noexcept;
    __constexpr
    enum_print_pair_collection_t<combined_enum_unexpected_exception_fields_t>
                      unexpected_thrown_exception_fields() const noexcept;
    __constexpr const std::string_view
                      source_location_str() const noexcept;
    __constexpr const std::string_view
                      matcher_annotation() const noexcept;
    __constexpr const std::string_view
                      source_location_pair_begin_str() const noexcept;
    __constexpr const std::string_view
                      source_location_pair_end_str() const noexcept;
    __constexpr const std::string_view
                      no_end_source() const noexcept;
    __constexpr const std::string_view
        potential_source_str(const bool _a_exact_source) const noexcept;
    __constexpr const std::string_view
        potential_code_str(const bool _a_exact_source) const noexcept;
    __constexpr const std::string_view
                      source_code_str() const noexcept;
    __constexpr       std::string
                source_location(const std::optional<std::source_location>& _a_sl
                ) const noexcept;
    __constexpr std::string
        source_representation(const std::optional<std::string_view>& _a_str
        ) const noexcept;
    __constexpr const std::string_view
                      log_info_str() const noexcept;
    __constexpr       std::string
                      log_info(const std::string_view _a_str) const noexcept;
    __constexpr const std::string_view
                      status_str() const noexcept;
    template <typename T>
    __constexpr std::string
                status(const bool _a_status) const noexcept;
    template <typename T>
    __constexpr       std::string
                      status() const noexcept;
    __constexpr const std::string_view
                      test_description_str() const noexcept;
    __constexpr const std::string_view
                      static_test_annotation_str() const noexcept;
    /*__constexpr
        const std::string_view
        fail_message_str(
        ) const noexcept;
    __constexpr
        const std::string_view
        pass_message_str(
        ) const noexcept;*/
    __constexpr std::string
                message_str(const std::string& _a_str) const noexcept;
    __constexpr std::string
                message_str(const std::string_view& _a_str) const noexcept;
    __constexpr std::string
        message_str(const std::optional<std::string>& _a_str) const noexcept;
    __constexpr std::string
                message_str(const std::optional<std::string_view>& _a_str
                ) const noexcept;
    enum_print_pair_collection_t<enum_after_execution_test_report_fields_t>
        _m_after_execution_test_report_fields_t;
    enum_print_pair_collection_t<enum_finalised_test_set_data_fields_t>
        _m_finalised_test_set_data_fields;
    enum_print_pair_collection_t<
        combined_enum_matcher_based_assertion_single_line_fields_t>
        _m_matcher_assertion_fields;
    enum_print_pair_collection_t<
        combined_enum_matcher_based_assertion_block_fields_t>
        _m_matcher_assertion_block_fields;
    enum_print_pair_collection_t<
        combined_enum_multi_element_assertion_block_fields_t>
        _m_multi_element_test_block_fields;
    enum_print_pair_collection_t<combined_enum_static_assertion_fields_t>
        _m_static_assertion_fields;
    enum_print_pair_collection_t<
        combined_enum_unexpected_thrown_non_descript_entity_fields_t>
        _m_unexpected_thrown_non_descript_entity_fields;
    enum_print_pair_collection_t<combined_enum_unexpected_exception_fields_t>
        _m_thrown_exception_fields;
    enum_print_pair_collection_t<enum_assertion_block_matcher_data_fields_t>
        _m_matcher_assertion_single_block_assertion_list_fields;
    enum_print_pair_collection_t<enum_assertion_block_matcher_data_fields_t>
         _m_matcher_assertion_multi_block_assertion_list_fields;
    bool _m_colours_enabled                   = true;
    //! The failure style used when highlighting information in text output.
    fmt::text_style _m_failure_style          = fmt::fg(fmt::color::red);
    //! The style used when highlighting information in text output.
    fmt::text_style _m_highlight_style        = fmt::fg(fmt::color::teal);
    //! The (slight) style used when highlighting information in text output.
    fmt::text_style _m_slight_highlight_style = fmt::fg(fmt::color::gray);
    //! The style used when highlighting good information in the text output
    fmt::text_style _m_pass_style             = fmt::fg(fmt::color::green);
    //! N indents
    std::size_t _m_indent_size                = 2;
    char        _m_line_break_char            = '=';
    std::size_t _m_line_break_len             = 90;
    __constexpr std::string
                slight_highlight(const std::string_view _a_str) const noexcept;
    __constexpr std::string
                highlight(const std::string_view _a_str) const noexcept;
    __constexpr std::string
        highlight_pass_or_fail(const bool _a_pass, const std::string _a_str)
            const noexcept;
    __constexpr    std::string
                   highlight_pass(const std::string _a_str) const noexcept;
    __constexpr    std::string
                   highlight_fail(const std::string _a_str) const noexcept;
    __no_constexpr std::string
                   quote(const std::string_view _a_str) const noexcept;
    __constexpr    std::string
                   space(const std::string_view _a_str) const noexcept;
    __constexpr    std::string
                   colon(const std::string_view _a_str) const noexcept;
    __constexpr    std::string
        indent(const std::string_view _a_str, const std::size_t _a_n_indent = 1)
            const noexcept;
    __constexpr       std::string
                      multi_element_collection_str() const noexcept;
    __constexpr       std::string
                      multi_element_collection_grouped_str() const noexcept;
    __constexpr const std::string_view
                      exception_type_str() const noexcept;
    __constexpr       std::string
        exception_type(const std::string_view _a_str) const noexcept;
    __constexpr const std::string_view
                      exception_message_str() const noexcept;
    __constexpr       std::string
                exception_message(const std::string_view _a_str) const noexcept;

    __constexpr std::string
                style(
                    const std::string_view _a_str,
                    const fmt::text_style& _a_style
                ) const noexcept
    {
        return _m_colours_enabled
                   ? fmt::format("{0}", fmt::styled(_a_str, _a_style))
                   : fmt::format("{0}", _a_str);
    }
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
    print_config_t::print_config_t(
        const bool _a_colours_enabled
    ) noexcept
    : _m_matcher_assertion_fields(default_matcher_based_assertion_fields())
    , _m_matcher_assertion_block_fields(
          default_matcher_based_assertion_block_fields()
      )
    , _m_static_assertion_fields(default_static_fields())
    , _m_after_execution_test_report_fields_t(
          default_after_execution_test_report_fields()
      )
    , _m_colours_enabled(_a_colours_enabled)
    , _m_unexpected_thrown_non_descript_entity_fields(
          default_unexpected_thrown_non_descript_entity_fields()
      )
    , _m_thrown_exception_fields(default_unexpected_exception_fields())
    , _m_finalised_test_set_data_fields(default_finalised_test_set_data_fields()
      )
    , _m_matcher_assertion_single_block_assertion_list_fields(
          default_matcher_based_assertion_single_block_assertion_fields()
      )
    , _m_matcher_assertion_multi_block_assertion_list_fields(
          default_matcher_based_assertion_multi_block_assertion_fields()
      )
    , _m_multi_element_test_block_fields(
          default_multi_element_assertion_block_fields()
      )
{}

__constexpr_imp std::string
                print_config_t::line_break() const noexcept
{
    using namespace std;
    return string(_m_line_break_len, _m_line_break_char).append("\n");
}

__constexpr_imp const std::string
                      print_config_t::matcher_output_str(
        const bool _a_passed
    ) const noexcept
{
    return _a_passed ? highlight_pass(colon("Matcher passed with output"))
                     : highlight_fail(colon("Matcher failed with output"));
}

__constexpr_imp const std::string_view
                      print_config_t::matcher_source_map_str() const noexcept
{
    return "Matcher's other sources";
}

__constexpr_imp std::string
                print_config_t::matcher_output(
        const std::string_view _a_str
    ) const noexcept
{
    return slight_highlight(quote(_a_str));
}

__constexpr_imp const std::string_view
                      print_config_t::test_path_str() const noexcept
{
    return "Test Path";
}

__constexpr_imp const std::string_view
                      print_config_t::str_total_tests_ran() const noexcept
{
    return "Total tests ran";
}

__no_constexpr_imp std::string
                   print_config_t::print_tdg_collection_stack_tries_map(
        const ds::map_unique_id_to_tdg_collection_stack_trie_t& _a_map
    ) const noexcept
{
    return quote(
        print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(_a_map)
    );
}

__no_constexpr std::string
               print_config_t::print_seed(
        const utility::complete_global_seed_t& _a_seed
    ) const
{
    return quote(_a_seed.print_in_hex());
}

__constexpr_imp const std::string_view
                      print_config_t::str_total_tests_passed() const noexcept
{
    return "Total tests passed";
}

__constexpr_imp const std::string_view
                      print_config_t::str_total_tests_failed() const noexcept
{
    return "Total tests failed";
}

__constexpr_imp const std::string_view
    print_config_t::str_total_tests_failed_and_terminated() const noexcept
{
    return "Total tests failed and terminated";
}

__constexpr_imp const std::string_view
    print_config_t::str_total_tests_failed_but_not_terminated() const noexcept
{
    return "Total tests failed (but not terminated)";
}

__constexpr_imp const std::string_view
    print_config_t::str_total_tests_designated_failure_due_to_exception(
    ) const noexcept
{
    return "Total tests which threw unexpected exception";
}

__constexpr_imp const std::string_view
                      print_config_t::str_total_assertions_ran() const noexcept
{
    return "Total assertions ran";
}

__constexpr_imp const std::string_view
    print_config_t::str_total_assertions_passed() const noexcept
{
    return "Total assertions passed";
}

__constexpr_imp const std::string_view
    print_config_t::str_total_assertions_failed() const noexcept
{
    return "Total assertions failed";
}

__constexpr_imp const std::string_view
                      print_config_t::str_seed() const noexcept
{
    return "Global random seed";
}

__constexpr const std::string_view
    print_config_t::str_tdg_collection_stack_tries_map() const noexcept
{
    return "Repetition loop stack seed";
}

__constexpr_imp std::string
                print_config_t::test_path(
        const std::string_view _a_passed
    ) const noexcept
{
    return highlight(quote(_a_passed));
}

__constexpr_imp const std::string_view
                      print_config_t::seed_used_str() const noexcept
{
    return "Seed used";
}

__constexpr_imp std::string
                print_config_t::seed_used(
        const std::optional<std::string>& _a_opt_seed
    ) const noexcept
{
    return slight_highlight(
        _a_opt_seed.has_value() ? _a_opt_seed.value() : "<no seed used>"
    );
}

__constexpr_imp const std::string_view
                      print_config_t::seed_used_in_hex_str() const noexcept
{
    return "Seed used (hex)";
}

__constexpr_imp std::string
                print_config_t::seed_used_in_hex(
        const std::optional<std::string>& _a_opt_seed
    ) const noexcept
{
    return slight_highlight(
        _a_opt_seed.has_value() ? quote(_a_opt_seed.value()) : "<no seed used>"
    );
}

__constexpr_imp const std::string_view
    print_config_t::seed_to_use_to_re_run_test_str() const noexcept
{
    return "Seed to re-run test";
}

__constexpr_imp std::string
                print_config_t::seed_to_use_to_re_run_test(
        const std::string_view _a_passed
    ) const noexcept
{
    return slight_highlight(_a_passed);
}

__constexpr_imp const std::string_view
    print_config_t::seed_to_use_to_re_run_test_in_hex_str() const noexcept
{
    return "Seed to re-run test (hex)";
}

__constexpr_imp std::string
                print_config_t::seed_to_use_to_re_run_test_in_hex(
        const std::string_view _a_passed
    ) const noexcept
{
    return slight_highlight(quote(_a_passed));
}

__constexpr_imp const std::string_view
    print_config_t::test_warnings_recieved_str() const noexcept
{
    return "Warnings";
}

__constexpr_imp std::string
                print_config_t::test_warnings_recieved(
        const std::size_t _a_warnings
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_warnings == 0, fmt::format("{0}", _a_warnings)
    );
}

__constexpr_imp const std::string_view
                      print_config_t::assertions_recieved_str() const noexcept
{
    return "Assertions evaluated";
}

__no_constexpr_imp std::string
                   print_config_t::print_integer(
        const std::size_t _a_assertions
    ) const noexcept
{
    return fmt::format("{0}", _a_assertions);
}

__constexpr_imp const std::string_view
                      print_config_t::assertions_passed_str() const noexcept
{
    return "Assertions passed";
}

__constexpr_imp std::string
                print_config_t::assertions_passed(
        const std::size_t _a_assertions
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_assertions > 0, fmt::format("{0}", _a_assertions)
    );
}

__constexpr_imp const std::string_view
                      print_config_t::assertions_failed_str() const noexcept
{
    return "Assertions failed";
}

__constexpr_imp std::string
                print_config_t::assertions_failed(
        const std::size_t _a_assertions
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_assertions == 0, fmt::format("{0}", _a_assertions)
    );
}

__constexpr_imp const std::string_view
                      print_config_t::compat_assertion_data_str() const noexcept
{
    return "Assertion information";
}

__constexpr_imp std::string
                print_config_t::compat_assertion_data(
        const std::size_t _a_passed,
        const std::size_t _a_total_recieved,
        const std::size_t _a_failed
    ) const noexcept
{
    return (_a_failed == 0)
               ? highlight_pass(
                     _a_total_recieved == 0
                         ? fmt::format("No assertions ran.")
                         : fmt::format(
                               "{0} / {1} assertions passed. All assertions "
                               "passed.",
                               _a_passed,
                               _a_total_recieved
                           )
                 )
               : highlight_fail(fmt::format(
                     "{0} / {1} assertions passed. {2} {3} failed.",
                     _a_passed,
                     _a_total_recieved,
                     (_a_passed == 0) ? "All" : fmt::format("{0}", _a_failed),
                     (_a_failed == 1) ? "assertion" : "assertions"
                 ));
}

__constexpr_imp const std::string_view
                      print_config_t::termination_status_str() const noexcept
{
    return "Termination status";
}

__constexpr_imp std::string
                print_config_t::test_status(
        const ds::enum_test_status_t _a_test_status
    ) const
{
    using enum ds::enum_test_status_t;
    switch (_a_test_status)
    {
    case NO_TERMINATION_TEST_PASSED:
        return highlight_pass("Function exited normally.");
    case NO_TERMINATION_TEST_FAILED:
        return highlight_pass("Function exited normally with a failed status.");
    case TERMINATION_OCCOURED_TEST_FAILED:
        return highlight_fail(
            "Function terminated early due to an assertion terminating."
        );
    case TERMINATION_OCCOURED_UNEXPECTED_THROW:
        return highlight_fail(
            "Function terminated due to an unexpected thrown entity."
        );
    default:
        throw errors::unaccounted_for_enum_exception(_a_test_status);
    }
}

__constexpr_imp const std::string_view
                      print_config_t::passed_str() const noexcept
{
    return "Test passed";
}

__constexpr_imp std::string
                print_config_t::passed(
        const bool _a_passed
    ) const noexcept
{
    return highlight_pass_or_fail(_a_passed, _a_passed ? "Yes" : "No");
}

__constexpr_imp const std::string_view
                      print_config_t::name_str() const noexcept
{
    return "Test name";
}

__constexpr_imp std::string
                print_config_t::name(
        const std::string_view _a_name
    ) const noexcept
{
    return highlight(quote(_a_name));
}

__constexpr_imp const std::string_view
                      print_config_t::str_status_str() const noexcept
{
    return "Test result";
}

__constexpr_imp std::string
                print_config_t::str_status(
        const bool _a_passed
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_passed, _a_passed ? "TEST PASSED" : "TEST FAILED"
    );
}

__constexpr_imp enum_print_pair_collection_t<
    combined_enum_matcher_based_assertion_single_line_fields_t>
    print_config_t::matcher_based_assertion_fields() const noexcept
{
    return _m_matcher_assertion_fields;
}

__constexpr_imp enum_print_pair_collection_t<
    combined_enum_matcher_based_assertion_block_fields_t>
    print_config_t::matcher_based_assertion_block_fields() const noexcept
{
    return _m_matcher_assertion_block_fields;
}

__constexpr_imp
    enum_print_pair_collection_t<enum_assertion_block_matcher_data_fields_t>
    print_config_t::matcher_assertion_single_block_assertion_list_fields(
    ) const noexcept
{
    return _m_matcher_assertion_single_block_assertion_list_fields;
}

__constexpr_imp
    enum_print_pair_collection_t<enum_assertion_block_matcher_data_fields_t>
    print_config_t::matcher_assertion_multi_block_assertion_list_fields(
    ) const noexcept
{
    return _m_matcher_assertion_multi_block_assertion_list_fields;
}

__constexpr enum_print_pair_collection_t<
    combined_enum_multi_element_assertion_block_fields_t>
    print_config_t::multi_element_test_block_fields() const noexcept
{
    return _m_multi_element_test_block_fields;
}

__constexpr_imp
    enum_print_pair_collection_t<enum_after_execution_test_report_fields_t>
    print_config_t::after_execution_test_report_fields() const noexcept
{
    return _m_after_execution_test_report_fields_t;
}

__constexpr enum_print_pair_collection_t<enum_finalised_test_set_data_fields_t>
            print_config_t::finalised_test_set_data_fields() const noexcept
{
    return _m_finalised_test_set_data_fields;
}

__constexpr_imp
    enum_print_pair_collection_t<combined_enum_static_assertion_fields_t>
    print_config_t::static_assertion_fields() const noexcept
{
    return _m_static_assertion_fields;
}

__constexpr_imp enum_print_pair_collection_t<
    combined_enum_unexpected_thrown_non_descript_entity_fields_t>
    print_config_t::unexpected_thrown_non_descript_entity_fields(
    ) const noexcept
{
    return _m_unexpected_thrown_non_descript_entity_fields;
}

__constexpr_imp
    enum_print_pair_collection_t<combined_enum_unexpected_exception_fields_t>
    print_config_t::unexpected_thrown_exception_fields() const noexcept
{
    return _m_thrown_exception_fields;
}

__constexpr_imp const std::string_view
                      print_config_t::source_location_str() const noexcept
{
    return "Source location";
}

__constexpr const std::string_view
                  print_config_t::matcher_annotation() const noexcept
{
    return "Matcher's annotation";
}

__constexpr const std::string_view
    print_config_t::source_location_pair_begin_str() const noexcept
{
    return "Source block begin";
}

__constexpr const std::string_view
                  print_config_t::source_location_pair_end_str() const noexcept
{
    return "Source block end";
}

__constexpr const std::string_view
                  print_config_t::no_end_source() const noexcept
{
    return "No end source";
}

__constexpr_imp const std::string_view
                      print_config_t::potential_source_str(
        const bool _a_exact_source
    ) const noexcept
{
    if (_a_exact_source)
    {
        return source_location_str();
    }
    else
    {
        return "Exact source unable to be verified. Most recently registered "
               "source location is";
    }
}

__constexpr_imp const std::string_view
                      print_config_t::potential_code_str(
        const bool _a_exact_source
    ) const noexcept
{
    if (_a_exact_source)
    {
        return source_code_str();
    }
    else
    {
        return "Most recently registered source code representation";
    }
}

__constexpr_imp const std::string_view
                      print_config_t::source_code_str() const noexcept
{
    return "Source code representation";
}

__constexpr_imp std::string
                print_config_t::source_location(
        const std::optional<std::source_location>& _a_sl
    ) const noexcept
{
    if (_a_sl.has_value())
    {
        return highlight(fmt::format(
            "{0}:{1}", _a_sl.value().file_name(), _a_sl.value().line()
        ));
    }
    else
    {
        return highlight("<no source location>");
    }
}

__constexpr_imp std::string
                print_config_t::source_representation(
        const std::optional<std::string_view>& _a_str
    ) const noexcept
{
    if (_a_str.has_value())
    {
        return slight_highlight(quote(_a_str.value()));
    }
    else
    {
        return highlight("<no source representation>");
    }
}

__constexpr_imp const std::string_view
                      print_config_t::log_info_str() const noexcept
{
    return "Logged information";
}

__constexpr_imp std::string
                print_config_t::log_info(
        const std::string_view _a_str
    ) const noexcept
{
    return slight_highlight(_a_str);
}

__constexpr_imp const std::string_view
                      print_config_t::status_str() const noexcept
{
    return "Assertion status";
}

template <typename T>
__constexpr_imp std::string
                print_config_t::status(
        const bool _a_status
    ) const noexcept
{
    using namespace std;
    using namespace reports;
    if constexpr (same_as<T, pass_t>)
    {
        return "Assertion always passes.";
    }
    else if constexpr (same_as<T, fail_t>)
    {
        return "Assertion always fails.";
    }
    else if constexpr (same_as<T, terminate_t>)
    {
        return "Assertion always fails and terminates the function.";
    }
    else if constexpr (same_as<T, pass_or_fail_t>)
    {
        return fmt::format(
            "Assertion will either pass or fail. This assertion {0}.",
            _a_status ? "passes" : "fails"
        );
    }
    else if constexpr (same_as<T, pass_or_terminate_t>)
    {
        return fmt::format(
            "Assertion will either pass or terminate. This assertion {0}.",
            _a_status ? "passes" : "terminates"
        );
    }
    else
    {
        __STATIC_ASSERT(T, "Cannot instantiate");
    }
}

template <typename T>
__constexpr_imp std::string
                print_config_t::status() const noexcept
{
    using namespace std;
    using namespace reports;
    if constexpr (same_as<T, pass_t>)
    {
        return "Assertion always passes.";
    }
    else if constexpr (same_as<T, fail_t>)
    {
        return "Assertion always fails.";
    }
    else if constexpr (same_as<T, terminate_t>)
    {
        return "Assertion always fails and terminates the function.";
    }
    else
    {
        __STATIC_ASSERT(T, "Cannot instantiate");
    }
}

__constexpr_imp const std::string_view
                      print_config_t::test_description_str() const noexcept
{
    return "Assertion description";
}

__constexpr const std::string_view
                  print_config_t::static_test_annotation_str() const noexcept
{
    return "Test annotation";
}

/*__constexpr_imp
const std::string_view
print_config_t::fail_message_str(
) const noexcept
{
    return "Fail message";
}
__constexpr_imp
const std::string_view
print_config_t::pass_message_str(
) const noexcept
{
    return "Pass message";
}*/
__constexpr std::string
            print_config_t::message_str(
        const std::string& _a_str
    ) const noexcept
{
    return slight_highlight(quote(_a_str));
}

__constexpr std::string
            print_config_t::message_str(
        const std::string_view& _a_str
    ) const noexcept
{
    return slight_highlight(quote(_a_str));
}

__constexpr_imp std::string
                print_config_t::message_str(
        const std::optional<std::string>& _a_str
    ) const noexcept
{
    return slight_highlight(
        _a_str.has_value() ? quote(_a_str.value()) : "<no message>"
    );
}

__constexpr_imp std::string
                print_config_t::message_str(
        const std::optional<std::string_view>& _a_str
    ) const noexcept
{
    using namespace std;
    return message_str(
        _a_str.has_value() ? optional<string>(string(_a_str.value()))
                           : optional<string>{}
    );
}

__constexpr_imp std::string
                print_config_t::slight_highlight(
        const std::string_view _a_str
    ) const noexcept
{
    return style(_a_str, _m_slight_highlight_style);
}

__constexpr_imp std::string
                print_config_t::highlight(
        const std::string_view _a_str
    ) const noexcept
{
    return style(_a_str, _m_highlight_style);
}

__constexpr_imp std::string
                print_config_t::highlight_pass_or_fail(
        const bool        _a_pass,
        const std::string _a_str
    ) const noexcept
{
    return style(_a_str, _a_pass ? _m_pass_style : _m_failure_style);
}

__constexpr_imp std::string
                print_config_t::highlight_pass(
        const std::string _a_str
    ) const noexcept
{
    return style(_a_str, _m_pass_style);
}

__constexpr_imp std::string
                print_config_t::highlight_fail(
        const std::string _a_str
    ) const noexcept
{
    return style(_a_str, _m_failure_style);
}

__no_constexpr_imp std::string
                   print_config_t::quote(
        const std::string_view _a_str
    ) const noexcept
{
    return fmt::format("\"{0}\"", _a_str);
}

__constexpr_imp std::string
                print_config_t::space(
        const std::string_view _a_str
    ) const noexcept
{
    using namespace std;
    return string(_a_str).append(" ");
}

__constexpr_imp std::string
                print_config_t::colon(
        const std::string_view _a_str
    ) const noexcept
{
    using namespace std;
    return string(_a_str).append(string(1, ':'));
}

__constexpr_imp std::string
                print_config_t::indent(
        const std::string_view _a_str,
        const std::size_t      _a_n_indents
    ) const noexcept
{
    using namespace std;
    return string(_a_n_indents * _m_indent_size, ' ').append(_a_str);
}

__constexpr_imp std::string
                print_config_t::multi_element_collection_str() const noexcept
{
    return "Matchers data";
}

__constexpr_imp std::string
    print_config_t::multi_element_collection_grouped_str() const noexcept
{
    return "Matchers data (grouped by source)";
}

__constexpr const std::string_view
                  print_config_t::exception_type_str() const noexcept
{
    return "Exception type";
}

__constexpr std::string
            print_config_t::exception_type(
        const std::string_view _a_str
    ) const noexcept
{
    return slight_highlight(_a_str);
}

__constexpr const std::string_view
                  print_config_t::exception_message_str() const noexcept
{
    return "Exception's what() function returned:";
}

__constexpr std::string
            print_config_t::exception_message(
        const std::string_view _a_str
    ) const noexcept
{
    return slight_highlight(quote(_a_str));
}

_END_ABC_REPORTERS_NS