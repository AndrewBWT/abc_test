#pragma once
#include "abc_test/core/ds/data_generator_memoization/map_unique_id_to_tdg_collection_stack_trie.hpp"
#include "abc_test/core/ds/test_data/enum_test_status.hpp"
#include "abc_test/core/matchers/matcher_result.hpp"
#include "abc_test/core/test_reports/assertion_status/fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/core/test_reports/assertion_status/terminate.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/after_execution_test_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/finalised_test_set_data_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_single_line.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/multi_element_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/pre_test_set_data_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/static_assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_exception.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_non_descript_entity.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp"
#include "abc_test/utility/rng.hpp"

#include <fmt/color.h>
_BEGIN_ABC_REPORTERS_NS

class print_config_t
{
public:
    __constexpr
                      print_config_t(const bool _a_colours_enabled) noexcept;
    __constexpr       std::u8string
                      line_break() const noexcept;
    __constexpr const std::u8string
        matcher_output_str(const bool _a_passed, const bool _a_terminate)
            const noexcept;
    __constexpr std::u8string
        static_assertion_output_str(const bool _a_passed) const noexcept;
    __constexpr const std::u8string_view
                      matcher_source_map_str() const noexcept;
    __constexpr       std::u8string
                      matcher_output(const std::u8string_view) const noexcept;
    __constexpr const std::u8string_view
                      test_path_str() const noexcept;
    __constexpr const std::u8string_view
                      str_total_tests_ran() const noexcept;
    __no_constexpr    std::u8string
                      print_tdg_collection_stack_tries_map(
                          const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t& _a_map
                      ) const noexcept;
    __no_constexpr std::u8string
        print_seed(const utility::complete_global_seed_t& _a_seed) const;
    __no_constexpr std::u8string
                   print_global_seed(
                       const utility::global_seed_t&          _a_seed,
                       const utility::complete_global_seed_t& _a_seed_global
                   ) const noexcept;
    __constexpr const std::u8string_view
                      str_total_tests_passed() const noexcept;
    __constexpr const std::u8string_view
                      str_total_tests_failed() const noexcept;
    __constexpr const std::u8string_view
                      str_total_tests_failed_and_terminated() const noexcept;
    __constexpr const std::u8string_view
        str_total_tests_failed_but_not_terminated() const noexcept;
    __constexpr const std::u8string_view
        str_total_tests_designated_failure_due_to_exception() const noexcept;
    __constexpr const std::u8string_view
                      str_total_assertions_ran() const noexcept;
    __constexpr const std::u8string_view
                      str_total_assertions_passed() const noexcept;
    __constexpr const std::u8string_view
                      str_total_assertions_failed() const noexcept;
    __constexpr const std::u8string_view
                      str_seed() const noexcept;
    __constexpr const std::u8string_view
                      str_tdg_collection_stack_tries_map() const noexcept;
    __constexpr       std::u8string
        test_path(const std::u8string_view _a_passed) const noexcept;
    __constexpr const std::u8string_view
                      seed_used_str() const noexcept;
    __constexpr       std::u8string
        seed_used(const std::optional<std::u8string_view>& _a_opt_seed
        ) const noexcept;
    __constexpr const std::u8string_view
                      seed_used_in_hex_str() const noexcept;
    __constexpr       std::u8string
        seed_used_in_hex(const std::optional<std::u8string_view>& _a_opt_seed
        ) const noexcept;
    __constexpr const std::u8string_view
                      seed_to_use_to_re_run_test_str() const noexcept;
    __constexpr       std::u8string
        seed_to_use_to_re_run_test(const std::u8string_view _a_passed
        ) const noexcept;
    __constexpr const std::u8string_view
                      seed_to_use_to_re_run_test_in_hex_str() const noexcept;
    __constexpr       std::u8string
        seed_to_use_to_re_run_test_in_hex(const std::u8string_view _a_passed
        ) const noexcept;
    __constexpr const std::u8string_view
                      test_warnings_recieved_str() const noexcept;
    __constexpr       std::u8string
        test_warnings_recieved(const std::size_t _a_passed) const noexcept;
    __constexpr const std::u8string_view
                      assertions_recieved_str() const noexcept;
    __no_constexpr    std::u8string
                      print_integer(const std::size_t _a_passed) const noexcept;
    __constexpr const std::u8string_view
                      assertions_passed_str() const noexcept;
    __constexpr       std::u8string
        assertions_passed(const std::size_t _a_passed) const noexcept;
    __constexpr const std::u8string_view
                      assertions_failed_str() const noexcept;
    __constexpr       std::u8string
        assertions_failed(const std::size_t _a_passed) const noexcept;
    __constexpr const std::u8string_view
                      compat_assertion_data_str() const noexcept;
    __constexpr       std::u8string
                      compat_assertion_data(
                          const std::size_t _a_passed,
                          const std::size_t _a_failed,
                          const std::size_t _a_total_recieved
                      ) const noexcept;
    __constexpr const std::u8string_view
                      termination_status_str() const noexcept;
    __constexpr       std::u8string
        test_status(const ds::enum_test_status_t _a_test_status) const;
    __constexpr const std::u8string_view
                      passed_str() const noexcept;
    __constexpr       std::u8string
                      passed(const bool _a_passed) const noexcept;
    __constexpr const std::u8string_view
                      name_str() const noexcept;
    __constexpr       std::u8string
                      name(const std::u8string_view _a_name) const noexcept;
    __constexpr const std::u8string_view
                      str_status_str() const noexcept;
    __constexpr       std::u8string
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
    __constexpr enum_print_pair_collection_t<enum_pre_test_set_data_fields_t>
                pre_test_set_data_fields() const noexcept;
    __constexpr
        enum_print_pair_collection_t<combined_enum_static_assertion_fields_t>
        static_assertion_fields() const noexcept;
    __constexpr enum_print_pair_collection_t<
        combined_enum_unexpected_thrown_non_descript_entity_fields_t>
        unexpected_thrown_non_descript_entity_fields() const noexcept;
    __constexpr
    enum_print_pair_collection_t<combined_enum_unexpected_exception_fields_t>
                      unexpected_thrown_exception_fields() const noexcept;
    __constexpr const std::u8string_view
                      source_location_str() const noexcept;
    __constexpr const std::u8string_view
                      matcher_annotation() const noexcept;
    __constexpr const std::u8string_view
                      source_location_pair_begin_str() const noexcept;
    __constexpr const std::u8string_view
                      source_location_pair_end_str() const noexcept;
    __constexpr const std::u8string_view
                      no_end_source() const noexcept;
    __constexpr const std::u8string_view
        potential_source_str(const bool _a_exact_source) const noexcept;
    __constexpr const std::u8string_view
        potential_code_str(const bool _a_exact_source) const noexcept;
    __constexpr const std::u8string_view
                      source_code_str() const noexcept;
    __constexpr       std::u8string
                source_location(const std::optional<std::source_location>& _a_sl
                ) const noexcept;
    __constexpr std::u8string
        source_representation(const std::optional<std::u8string_view>& _a_str
        ) const noexcept;
    __constexpr const std::u8string_view
                      log_info_str() const noexcept;
    __constexpr       std::u8string
                      log_info(const std::u8string_view _a_str) const noexcept;
    __constexpr const std::u8string_view
                      status_str() const noexcept;
    template <typename T>
    __constexpr std::u8string
                status(const bool _a_status) const noexcept;
    template <typename T>
    __constexpr       std::u8string
                      status() const noexcept;
    __constexpr const std::u8string_view
                      time_taken_str() const noexcept;
    __constexpr const std::u8string_view
                      test_description_str() const noexcept;
    __constexpr const std::u8string_view
                      static_test_annotation_str() const noexcept;
    /*__constexpr
        const std::string_view
        fail_message_str(
        ) const noexcept;
    __constexpr
        const std::string_view
        pass_message_str(
        ) const noexcept;*/
    __constexpr std::u8string
                message_str(const std::u8string& _a_str) const noexcept;
    __constexpr std::u8string
        test_description(const std::u8string_view _a_str) const noexcept;
    __constexpr std::u8string
                message_str(const std::u8string_view& _a_str) const noexcept;
    __no_constexpr_imp std::u8string
                       time_taken(
                           const std::chrono::time_point<std::chrono::high_resolution_clock>&
                               _a_begin_time,
                           const std::chrono::time_point<std::chrono::high_resolution_clock>&
                               _a_end_time
                       ) const noexcept;
    __constexpr std::u8string
        message_str(const std::optional<std::u8string>& _a_str) const noexcept;
    __constexpr std::u8string
                message_str(const std::optional<std::u8string_view>& _a_str
                ) const noexcept;
    enum_print_pair_collection_t<enum_after_execution_test_report_fields_t>
        _m_after_execution_test_report_fields_t;
    enum_print_pair_collection_t<enum_finalised_test_set_data_fields_t>
        _m_finalised_test_set_data_fields;
    enum_print_pair_collection_t<enum_pre_test_set_data_fields_t>
        _m_pre_test_set_data_fields;
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
    __constexpr std::u8string
        slight_highlight(const std::u8string_view _a_str) const noexcept;
    __constexpr std::u8string
                highlight(const std::u8string_view _a_str) const noexcept;
    __constexpr std::u8string
                make_source_info(const std::vector<std::u8string>& _a_vect
                ) const noexcept;
    __constexpr std::u8string
        highlight_pass_or_fail(const bool _a_pass, const std::u8string _a_str)
            const noexcept;
    __constexpr    std::u8string
                   highlight_pass(const std::u8string _a_str) const noexcept;
    __constexpr    std::u8string
                   highlight_fail(const std::u8string _a_str) const noexcept;
    __no_constexpr std::u8string
                   quote(const std::u8string_view _a_str) const noexcept;
    __constexpr    std::u8string
                   space(const std::u8string_view _a_str) const noexcept;
    __constexpr    std::u8string
                   colon(const std::u8string_view _a_str) const noexcept;
    __constexpr    std::u8string
                   indent(
                       const std::u8string_view _a_str,
                       const std::size_t        _a_n_indent = 1
                   ) const noexcept;
    __constexpr       std::u8string
                      multi_element_collection_str() const noexcept;
    __constexpr       std::u8string
                      multi_element_collection_grouped_str() const noexcept;
    __constexpr const std::u8string_view
                      exception_type_str() const noexcept;
    __constexpr const std::u8string_view
        explanation_str() const noexcept {
        return u8"Explanation";
    }
    __constexpr       std::u8string
        exception_type(const std::u8string_view _a_str) const noexcept;
    __constexpr const std::u8string_view
                      exception_message_str() const noexcept;
    __constexpr       std::u8string
        exception_message(const std::u8string_view _a_str) const noexcept;
    __constexpr    std::u8string
                   global_test_list_used() const noexcept;
    __no_constexpr std::u8string
                   use_global_test_list_info(const bool _a_use_global_test_list
                   ) const noexcept;
    __no_constexpr std::u8string
                   write_data_to_files_info(const bool _a_use_global_test_list
                   ) const noexcept;
    __no_constexpr std::u8string
        path_delimiter_used_info(const std::u8string_view _a_file_delimiter
        ) const noexcept;
    __no_constexpr std::u8string
        root_path_info(const std::filesystem::path& _a_path) const noexcept;
    __no_constexpr std::u8string
        threads_used_info(const std::size_t _a_thrads) const noexcept;
    __no_constexpr std::u8string
        comment_str_info(const std::u8string_view _a_str) const noexcept;
    __no_constexpr std::u8string
        general_data_file_extension_info(const std::u8string_view _a_str
        ) const noexcept;
    __no_constexpr std::u8string
                   n_integers_used_for_creation_of_rng_info(
                       const std::size_t _a_n_integers_to_use
                   ) const noexcept;
    __no_constexpr std::u8string
                   repetition_config_used_info() const noexcept;
    __no_constexpr std::u8string
                   global_seed_info(const utility::global_seed_t& _a_global_seed
                   ) const noexcept;
    __no_constexpr std::u8string
                   force_run_all_tests_info(const bool _a_force_run_all_tests
                   ) const noexcept;
    __no_constexpr std::u8string
                   test_paths_to_run_info(
                       const std::vector<std::u8string>& _a_force_run_all_tests
                   ) const noexcept;
    __constexpr std::u8string
                write_data_to_files() const noexcept;
    __constexpr std::u8string
                path_delimiter_used() const noexcept;
    __constexpr std::u8string
                root_path_used() const noexcept;
    __constexpr std::u8string
                threads_used() const noexcept;
    __constexpr std::u8string
                comment_str_used() const noexcept;
    __constexpr std::u8string
                general_data_file_extension_used() const noexcept;
    __constexpr std::u8string
                n_integers_used_for_creation_of_rng_used() const noexcept;
    __constexpr std::u8string
                repetition_config_used() const noexcept;
    __constexpr std::u8string
                global_seed_used() const noexcept;
    __constexpr std::u8string
                force_run_all_tests_used() const noexcept;
    __constexpr std::u8string
                test_paths_to_run_used() const noexcept;
    __constexpr std::u8string
                show_test_paths_to_run(const std::vector<std::u8string>& _a_strs
                ) const noexcept;
    __no_constexpr std::u8string
                   show_repetition_config(
                       const _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t&
                           _a_config
                   ) const noexcept;
    __no_constexpr std::u8string
                show_path(const std::filesystem::path& _a_path) const noexcept;
    __constexpr std::u8string
                question(const std::u8string_view _a_str) const noexcept;
    __no_constexpr std::u8string
                print_global_test_list_used(const bool _a_lobal_test_list_used
                ) const noexcept;

    __constexpr std::u8string
                style(
                    const std::u8string_view _a_str,
                    const fmt::text_style&   _a_style
                ) const noexcept
    {
        return _m_colours_enabled
                   ? fmt::format(u8"{0}", fmt::styled(_a_str, _a_style))
                   : fmt::format(u8"{0}", _a_str);
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
    , _m_pre_test_set_data_fields(default_pre_test_set_data_fields())
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

__constexpr_imp std::u8string
                print_config_t::line_break() const noexcept
{
    using namespace std;
    return unpack_string_to_u8string(
        string(_m_line_break_len, _m_line_break_char)
    );
}

__constexpr_imp const std::u8string
                      print_config_t::matcher_output_str(
        const bool _a_passed,
        const bool _a_terminate
    ) const noexcept
{
    return _a_passed
               ? highlight_pass(
                     colon(u8"Matcher-based assertion passed with output")
                 )
               : (_a_terminate
                      ? highlight_fail(
                            colon(u8"Matcher-based assertion failed, assertion "
                                  "terminated function with output")
                        )
                      : highlight_fail(colon(
                            u8"Matcher-based assertion failed with output"
                        )));
}

__constexpr std::u8string
            print_config_t::static_assertion_output_str(
        const bool _a_passed
    ) const noexcept
{
    return _a_passed ? highlight_pass(u8"Static assertion passed.")
                     : highlight_fail(u8"Static assertion failed.");
}

__constexpr_imp const std::u8string_view
                      print_config_t::matcher_source_map_str() const noexcept
{
    return u8"Matcher's other sources";
}

__constexpr_imp std::u8string
                print_config_t::matcher_output(
        const std::u8string_view _a_str
    ) const noexcept
{
    return slight_highlight(_a_str);
}

__constexpr_imp const std::u8string_view
                      print_config_t::test_path_str() const noexcept
{
    return u8"Test Path";
}

__constexpr_imp const std::u8string_view
                      print_config_t::str_total_tests_ran() const noexcept
{
    return u8"Total tests ran";
}

__no_constexpr_imp std::u8string
                   print_config_t::print_tdg_collection_stack_tries_map(
        const ds::map_unique_id_to_tdg_collection_stack_trie_t& _a_map
    ) const noexcept
{
    return quote(print_map_of_unique_ids_to_tdg_collection_stack_tries(_a_map));
}

__no_constexpr_imp std::u8string
                   print_config_t::print_seed(
        const utility::complete_global_seed_t& _a_seed
    ) const
{
    using namespace abc::utility::printer;
    using namespace utility;
    using namespace std;
    return quote(print(
        _a_seed,
        default_printer<variant<unsigned int, seed_t>>(
            variant_print_parse_e::no_indexes
        )
    ));
}

__no_constexpr_imp std::u8string
                   print_config_t::print_global_seed(
        const utility::global_seed_t&          _a_seed,
        const utility::complete_global_seed_t& _a_seed_global
    ) const noexcept
{
    using namespace utility::printer;
    using namespace utility;
    using namespace std;
    if (not _a_seed.has_value())
    {
        if (const unsigned int* _l_ptr{get_if<unsigned int>(&_a_seed_global)};
            _l_ptr != nullptr)
        {
            return fmt::format(
                u8"Not set by user. Global seed has been set randomly by "
                u8"system to "
                u8"the integer {0}",
                *_l_ptr
            );
        }
        else
        {
            return fmt::format(
                u8"Not set by user. Global seed should have been set randomly "
                u8"to an integer, however this was not performed. "
            );
        }
    }
    else
    {
        using namespace abc::utility::printer;
        return fmt::format(
            u8"{0}",
            default_printer_t<variant<unsigned int, seed_t>>().run_printer(
                _a_seed.value()
            )
        );
    }
}

__constexpr_imp const std::u8string_view
                      print_config_t::str_total_tests_passed() const noexcept
{
    return u8"Total tests passed";
}

__constexpr_imp const std::u8string_view
                      print_config_t::str_total_tests_failed() const noexcept
{
    return u8"Total tests failed";
}

__constexpr_imp const std::u8string_view
    print_config_t::str_total_tests_failed_and_terminated() const noexcept
{
    return u8"Total tests failed and terminated";
}

__constexpr_imp const std::u8string_view
    print_config_t::str_total_tests_failed_but_not_terminated() const noexcept
{
    return u8"Total tests failed (but not terminated)";
}

__constexpr_imp const std::u8string_view
    print_config_t::str_total_tests_designated_failure_due_to_exception(
    ) const noexcept
{
    return u8"Total tests which threw unexpected exception";
}

__constexpr_imp const std::u8string_view
                      print_config_t::str_total_assertions_ran() const noexcept
{
    return u8"Total assertions ran";
}

__constexpr_imp const std::u8string_view
    print_config_t::str_total_assertions_passed() const noexcept
{
    return u8"Total assertions passed";
}

__constexpr_imp const std::u8string_view
    print_config_t::str_total_assertions_failed() const noexcept
{
    return u8"Total assertions failed";
}

__constexpr_imp const std::u8string_view
                      print_config_t::str_seed() const noexcept
{
    return u8"Global random seed";
}

__constexpr const std::u8string_view
    print_config_t::str_tdg_collection_stack_tries_map() const noexcept
{
    return u8"Repetition loop stack seed";
}

__constexpr_imp std::u8string
                print_config_t::test_path(
        const std::u8string_view _a_passed
    ) const noexcept
{
    return highlight(quote(_a_passed));
}

__constexpr_imp const std::u8string_view
                      print_config_t::seed_used_str() const noexcept
{
    return u8"Seed used";
}

__constexpr_imp std::u8string
                print_config_t::seed_used(
        const std::optional<std::u8string_view>& _a_opt_seed
    ) const noexcept
{
    return slight_highlight(
        _a_opt_seed.has_value() ? _a_opt_seed.value() : u8"<no seed used>"
    );
}

__constexpr_imp const std::u8string_view
                      print_config_t::seed_used_in_hex_str() const noexcept
{
    return u8"Seed used (hex)";
}

__constexpr_imp std::u8string
                print_config_t::seed_used_in_hex(
        const std::optional<std::u8string_view>& _a_opt_seed
    ) const noexcept
{
    return slight_highlight(
        _a_opt_seed.has_value() ? quote(_a_opt_seed.value())
                                : u8"<no seed used>"
    );
}

__constexpr_imp const std::u8string_view
    print_config_t::seed_to_use_to_re_run_test_str() const noexcept
{
    return u8"Seed to re-run test";
}

__constexpr_imp std::u8string
                print_config_t::seed_to_use_to_re_run_test(
        const std::u8string_view _a_passed
    ) const noexcept
{
    return slight_highlight(_a_passed);
}

__constexpr_imp const std::u8string_view
    print_config_t::seed_to_use_to_re_run_test_in_hex_str() const noexcept
{
    return u8"Seed to re-run test (hex)";
}

__constexpr_imp std::u8string
                print_config_t::seed_to_use_to_re_run_test_in_hex(
        const std::u8string_view _a_passed
    ) const noexcept
{
    return slight_highlight(quote(_a_passed));
}

__constexpr_imp const std::u8string_view
    print_config_t::test_warnings_recieved_str() const noexcept
{
    return u8"Warnings";
}

__constexpr_imp std::u8string
                print_config_t::test_warnings_recieved(
        const std::size_t _a_warnings
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_warnings == 0, fmt::format(u8"{0}", _a_warnings)
    );
}

__constexpr_imp const std::u8string_view
                      print_config_t::assertions_recieved_str() const noexcept
{
    return u8"Assertions evaluated";
}

__no_constexpr_imp std::u8string
                   print_config_t::print_integer(
        const std::size_t _a_assertions
    ) const noexcept
{
    return fmt::format(u8"{0}", _a_assertions);
}

__constexpr_imp const std::u8string_view
                      print_config_t::assertions_passed_str() const noexcept
{
    return u8"Assertions passed";
}

__constexpr_imp std::u8string
                print_config_t::assertions_passed(
        const std::size_t _a_assertions
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_assertions > 0, fmt::format(u8"{0}", _a_assertions)
    );
}

__constexpr_imp const std::u8string_view
                      print_config_t::assertions_failed_str() const noexcept
{
    return u8"Assertions failed";
}

__constexpr_imp std::u8string
                print_config_t::assertions_failed(
        const std::size_t _a_assertions
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_assertions == 0, fmt::format(u8"{0}", _a_assertions)
    );
}

__constexpr_imp const std::u8string_view
                      print_config_t::compat_assertion_data_str() const noexcept
{
    return u8"Assertion information";
}

__constexpr_imp std::u8string
                print_config_t::compat_assertion_data(
        const std::size_t _a_passed,
        const std::size_t _a_total_recieved,
        const std::size_t _a_failed
    ) const noexcept
{
    return (_a_failed == 0)
               ? highlight_pass(
                     _a_total_recieved == 0
                         ? u8"No assertions ran."
                         : fmt::format(
                               u8"{0} / {1} assertions passed. All assertions "
                               "passed.",
                               _a_passed,
                               _a_total_recieved
                           )
                 )
               : highlight_fail(fmt::format(
                     u8"{0} / {1} assertions passed. {2} {3} failed.",
                     _a_passed,
                     _a_total_recieved,
                     (_a_passed == 0) ? u8"All"
                                      : fmt::format(u8"{0}", _a_failed),
                     (_a_failed == 1) ? u8"assertion" : u8"assertions"
                 ));
}

__constexpr_imp const std::u8string_view
                      print_config_t::termination_status_str() const noexcept
{
    return u8"Termination status";
}

__constexpr_imp std::u8string
                print_config_t::test_status(
        const ds::enum_test_status_t _a_test_status
    ) const
{
    using enum ds::enum_test_status_t;
    switch (_a_test_status)
    {
    case NO_TERMINATION_TEST_PASSED:
        return highlight_pass(u8"Function exited normally.");
    case NO_TERMINATION_TEST_FAILED:
        return highlight_pass(u8"Function exited normally with a failed status."
        );
    case TERMINATION_OCCOURED_TEST_FAILED:
        return highlight_fail(
            u8"Function terminated early due to an assertion terminating."
        );
    case TERMINATION_OCCOURED_UNEXPECTED_THROW:
        return highlight_fail(
            u8"Function terminated due to an unexpected thrown entity."
        );
    default:
        throw errors::unaccounted_for_enum_exception(_a_test_status);
    }
}

__constexpr_imp const std::u8string_view
                      print_config_t::passed_str() const noexcept
{
    return u8"Test passed";
}

__constexpr_imp std::u8string
                print_config_t::passed(
        const bool _a_passed
    ) const noexcept
{
    return highlight_pass_or_fail(_a_passed, _a_passed ? u8"Yes" : u8"No");
}

__constexpr_imp const std::u8string_view
                      print_config_t::name_str() const noexcept
{
    return u8"Test name";
}

__constexpr_imp std::u8string
                print_config_t::name(
        const std::u8string_view _a_name
    ) const noexcept
{
    return highlight(quote(_a_name));
}

__constexpr_imp const std::u8string_view
                      print_config_t::str_status_str() const noexcept
{
    return u8"Test result";
}

__constexpr_imp std::u8string
                print_config_t::str_status(
        const bool _a_passed
    ) const noexcept
{
    return highlight_pass_or_fail(
        _a_passed, _a_passed ? u8"TEST PASSED" : u8"TEST FAILED"
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

__constexpr enum_print_pair_collection_t<enum_pre_test_set_data_fields_t>
            print_config_t::pre_test_set_data_fields() const noexcept
{
    return _m_pre_test_set_data_fields;
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

__constexpr_imp const std::u8string_view
                      print_config_t::source_location_str() const noexcept
{
    return u8"Source location";
}

__constexpr const std::u8string_view
                  print_config_t::matcher_annotation() const noexcept
{
    return u8"Assertion's annotation";
}

__constexpr const std::u8string_view
    print_config_t::source_location_pair_begin_str() const noexcept
{
    return u8"Source block begin";
}

__constexpr const std::u8string_view
                  print_config_t::source_location_pair_end_str() const noexcept
{
    return u8"Source block end";
}

__constexpr const std::u8string_view
                  print_config_t::no_end_source() const noexcept
{
    return u8"No end source";
}

__constexpr_imp const std::u8string_view
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
        return u8"Exact source unable to be verified. Most recently registered "
               "source location is";
    }
}

__constexpr_imp const std::u8string_view
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
        return u8"Most recently registered source code representation";
    }
}

__constexpr_imp const std::u8string_view
                      print_config_t::source_code_str() const noexcept
{
    return u8"Source code representation";
}

__constexpr_imp std::u8string
                print_config_t::source_location(
        const std::optional<std::source_location>& _a_sl
    ) const noexcept
{
    using namespace std;
    if (_a_sl.has_value())
    {
        return highlight(fmt::format(
            u8"{0}:{1}",
            unpack_string_to_u8string(_a_sl.value().file_name(
            )),
            _a_sl.value().line()
        ));
    }
    else
    {
        return highlight(u8"<no source location>");
    }
}

__constexpr_imp std::u8string
                print_config_t::source_representation(
        const std::optional<std::u8string_view>& _a_str
    ) const noexcept
{
    if (_a_str.has_value())
    {
        return slight_highlight(quote(_a_str.value()));
    }
    else
    {
        return highlight(u8"<no source representation>");
    }
}

__constexpr_imp const std::u8string_view
                      print_config_t::log_info_str() const noexcept
{
    return u8"Logged information";
}

__constexpr_imp std::u8string
                print_config_t::log_info(
        const std::u8string_view _a_str
    ) const noexcept
{
    return slight_highlight(_a_str);
}

__constexpr_imp const std::u8string_view
                      print_config_t::status_str() const noexcept
{
    return u8"Assertion status";
}

template <typename T>
__constexpr_imp std::u8string
                print_config_t::status(
        const bool _a_status
    ) const noexcept
{
    using namespace std;
    using namespace reports;
    if constexpr (same_as<T, pass_t>)
    {
        return u8"Assertion always passes.";
    }
    else if constexpr (same_as<T, fail_t>)
    {
        return u8"Assertion always fails.";
    }
    else if constexpr (same_as<T, terminate_t>)
    {
        return u8"Assertion always fails and terminates the function.";
    }
    else if constexpr (same_as<T, pass_or_fail_t>)
    {
        return fmt::format(
            u8"Assertion will either pass or fail. This assertion {0}.",
            _a_status ? u8"passes" : u8"fails"
        );
    }
    else if constexpr (same_as<T, pass_or_terminate_t>)
    {
        return fmt::format(
            u8"Assertion will either pass or terminate. This assertion {0}.",
            _a_status ? u8"passes" : u8"terminates"
        );
    }
    else
    {
        __STATIC_ASSERT(T, "Cannot instantiate");
    }
}

template <typename T>
__constexpr_imp std::u8string
                print_config_t::status() const noexcept
{
    using namespace std;
    using namespace reports;
    if constexpr (same_as<T, pass_t>)
    {
        return u8"Assertion always passes.";
    }
    else if constexpr (same_as<T, fail_t>)
    {
        return u8"Assertion always fails.";
    }
    else if constexpr (same_as<T, terminate_t>)
    {
        return u8"Assertion always fails and terminates the function.";
    }
    else
    {
        __STATIC_ASSERT(T, "Cannot instantiate");
    }
}

__constexpr_imp const std::u8string_view
                      print_config_t::time_taken_str() const noexcept
{
    return u8"Time taken";
}

__constexpr_imp const std::u8string_view
                      print_config_t::test_description_str() const noexcept
{
    return u8"Assertion's annotation";
}

__constexpr const std::u8string_view
                  print_config_t::static_test_annotation_str() const noexcept
{
    return u8"Assertion's annotation";
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
__constexpr std::u8string
            print_config_t::message_str(
        const std::u8string& _a_str
    ) const noexcept
{
    return slight_highlight(quote(_a_str));
}

__constexpr std::u8string
print_config_t::test_description(const std::u8string_view _a_str) const noexcept
{
    return slight_highlight(quote(_a_str));
}

__constexpr std::u8string
            print_config_t::message_str(
        const std::u8string_view& _a_str
    ) const noexcept
{
    return slight_highlight(_a_str);
}

__constexpr_imp std::u8string
                print_config_t::message_str(
        const std::optional<std::u8string>& _a_str
    ) const noexcept
{
    return slight_highlight(
        _a_str.has_value() ? quote(_a_str.value()) : u8"<no message>"
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::time_taken(
        const std::chrono::time_point<std::chrono::high_resolution_clock>&
            _a_begin_time,
        const std::chrono::time_point<std::chrono::high_resolution_clock>&
            _a_end_time
    ) const noexcept
{
    return fmt::format(
        u8"{0} microseconds",
        std::chrono::duration_cast<std::chrono::microseconds>(
            _a_end_time - _a_begin_time
        )
            .count()
    );
}

__constexpr_imp std::u8string
                print_config_t::message_str(
        const std::optional<std::u8string_view>& _a_str
    ) const noexcept
{
    using namespace std;
    return message_str(
        _a_str.has_value() ? optional<u8string>(u8string(_a_str.value()))
                           : optional<u8string>{}
    );
}

__constexpr_imp std::u8string
                print_config_t::slight_highlight(
        const std::u8string_view _a_str
    ) const noexcept
{
    return style(_a_str, _m_slight_highlight_style);
}

__constexpr_imp std::u8string
                print_config_t::highlight(
        const std::u8string_view _a_str
    ) const noexcept
{
    return style(_a_str, _m_highlight_style);
}

__constexpr std::u8string
            print_config_t::make_source_info(
        const std::vector<std::u8string>& _a_vect
    ) const noexcept
{
    using namespace std;
    u8string _l_rv{u8"[  "};
    if (_a_vect.size() == 0)
    {
        _l_rv.append(u8"Set using default value");
    }
    else
    {
        _l_rv.append(u8"Set using ");
        for (size_t _l_idx{0}; const u8string_view _l_str : _a_vect)
        {
            _l_rv.append(_l_str);
            if (_l_idx + 2 < _a_vect.size())
            {
                _l_rv.append(u8", ");
            }
            else if (_l_idx + 1 < _a_vect.size())
            {
                _l_rv.append(u8" and ");
            }
            ++_l_idx;
        }
    }
    _l_rv.append(u8".  ]");
    return _l_rv;
}

__constexpr_imp std::u8string
                print_config_t::highlight_pass_or_fail(
        const bool          _a_pass,
        const std::u8string _a_str
    ) const noexcept
{
    return style(_a_str, _a_pass ? _m_pass_style : _m_failure_style);
}

__constexpr_imp std::u8string
                print_config_t::highlight_pass(
        const std::u8string _a_str
    ) const noexcept
{
    return style(_a_str, _m_pass_style);
}

__constexpr_imp std::u8string
                print_config_t::highlight_fail(
        const std::u8string _a_str
    ) const noexcept
{
    return style(_a_str, _m_failure_style);
}

__no_constexpr_imp std::u8string
                   print_config_t::quote(
        const std::u8string_view _a_str
    ) const noexcept
{
    return fmt::format(u8"\"{0}\"", _a_str);
}

__constexpr_imp std::u8string
                print_config_t::space(
        const std::u8string_view _a_str
    ) const noexcept
{
    using namespace std;
    return u8string(_a_str).append(u8" ");
}

__constexpr_imp std::u8string
                print_config_t::colon(
        const std::u8string_view _a_str
    ) const noexcept
{
    using namespace std;
    return u8string(_a_str).append(u8":");
}

__constexpr_imp std::u8string
                print_config_t::indent(
        const std::u8string_view _a_str,
        const std::size_t        _a_n_indents
    ) const noexcept
{
    using namespace std;
    return u8string(_a_n_indents * _m_indent_size, char8_t(' ')).append(_a_str);
}

__constexpr_imp std::u8string
                print_config_t::multi_element_collection_str() const noexcept
{
    return u8"Assertion data";
}

__constexpr_imp std::u8string
    print_config_t::multi_element_collection_grouped_str() const noexcept
{
    return u8"Assertion data (grouped by source)";
}

__constexpr const std::u8string_view
                  print_config_t::exception_type_str() const noexcept
{
    return u8"Exception type";
}

__constexpr std::u8string
            print_config_t::exception_type(
        const std::u8string_view _a_str
    ) const noexcept
{
    return slight_highlight(_a_str);
}

__constexpr const std::u8string_view
                  print_config_t::exception_message_str() const noexcept
{
    return u8"Exception's what() function returned:";
}

__constexpr std::u8string
            print_config_t::exception_message(
        const std::u8string_view _a_str
    ) const noexcept
{
    return slight_highlight(quote(_a_str));
}

__constexpr_imp std::u8string
                print_config_t::global_test_list_used() const noexcept
{
    return u8"Global test list used";
}

__no_constexpr_imp std::u8string
                   print_config_t::use_global_test_list_info(
        const bool _a_use_global_test_list
    ) const noexcept
{
    return fmt::format(
        u8"Tests created using the macros \"_TEST_CASE\" "
        u8"and \"_NAMED_TEST_CASE\" are {0}included in "
        u8"the set of tests to be ran.",
        _a_use_global_test_list ? u8"" : u8"not "
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::write_data_to_files_info(
        const bool _a_use_global_test_list
    ) const noexcept
{
    return fmt::format(
        u8"When an assertoin that is contained within a data generator linked "
        u8"to "
        u8"a file fails, the data generator will {0}write that data to its "
        u8"associated "
        u8"file.",
        _a_use_global_test_list ? u8"" : u8"not "
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::path_delimiter_used_info(
        const std::u8string_view _a_file_delimiter
    ) const noexcept
{
    return fmt::format(
        u8"The string \"{0}\" is used to separate a test's path. For "
        "example, the test path \"aa{0}bb{0}cc\" becomes {{aa,bb,cc}}. Any "
        "default-initialised file data for that test will be stored at "
        "root_path/aa/bb/cc",
        _a_file_delimiter
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::root_path_info(
        const std::filesystem::path& _a_file_delimiter
    ) const noexcept
{
    return fmt::format(
        u8"All files for the tests created when this instance of the test "
        u8"framework are ran are stored within the root folder {0}",
        _a_file_delimiter
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::threads_used_info(
        const std::size_t _a_threads
    ) const noexcept
{
    return fmt::format(
        u8"{0} threads are available to the system. Any test which requires "
        u8"more "
        u8"threads than this will not be ran.",
        _a_threads
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::comment_str_info(
        const std::u8string_view _a_str
    ) const noexcept
{
    return fmt::format(
        u8"The string \"{0}\" is used at the beginning of a file line to show "
        u8"its a comment. Note that files which have "
        u8"been read before this point - such as configuration files - use the "
        u8"default comment string \"#\".",
        _a_str
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::general_data_file_extension_info(
        const std::u8string_view _a_str
    ) const noexcept
{
    return fmt::format(
        u8"The extension \"{0}\" is used for general data files.", _a_str
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::n_integers_used_for_creation_of_rng_info(
        const std::size_t _a_n_integers_to_use
    ) const noexcept
{
    return fmt::format(
        u8"{0} integers are taken from the global RNG to seed each individual "
        u8"test's RNG",
        _a_n_integers_to_use
    );
}

__no_constexpr_imp std::u8string
                   print_config_t::repetition_config_used_info() const noexcept
{
    return fmt::format(u8"This string denotes the repetition config used. It "
                       u8"describes the set of tests to be ran, and in those "
                       u8"tests which data points to re-generate. abc_test is "
                       u8"designed for this to be generated by a computer.");
}

__no_constexpr_imp std::u8string
                   print_config_t::global_seed_info(
        const utility::global_seed_t& _a_global_seed
    ) const noexcept
{
    using namespace utility;
    using namespace std;
    if (not _a_global_seed.has_value())
    {
        return u8"The global seed has not been set by the user, and will "
               u8"therefore be set using the current time.";
    }
    else
    {
        using namespace abc::utility::printer;
        const variant<unsigned int, seed_t>& _l_complete_global_seed{
            _a_global_seed.value()
        };
        return fmt::format(
            u8"The global seed is set using the {0} {1}",
            holds_alternative<unsigned int>(_l_complete_global_seed)
                ? u8"single integer"
                : u8"vector of integers",
            default_printer_t<variant<unsigned int, seed_t>>{}.run_printer(
                _l_complete_global_seed
            )
        );
    }
}

__no_constexpr_imp std::u8string
                   print_config_t::force_run_all_tests_info(
        const bool _a_force_run_all_tests
    ) const noexcept
{
    return _a_force_run_all_tests
               ? u8"All tests, irrispective of whether they have a repetition "
                 u8"configuration associated with them, but only those which "
                 u8"are "
                 u8"denoted by the test path, are ran"
               : u8"Only those tests denoted by the test path, and if a "
                 u8"repetition "
                 u8"configuration is given, are ran";
}

__no_constexpr_imp std::u8string
                   print_config_t::test_paths_to_run_info(
        const std::vector<std::u8string>& _a_force_run_all_tests
    ) const noexcept
{
    return fmt::format(u8"The test paths {0} are ran", _a_force_run_all_tests);
}

__constexpr std::u8string
            print_config_t::write_data_to_files() const noexcept
{
    return u8"Write data to files";
}

__constexpr std::u8string
            print_config_t::path_delimiter_used() const noexcept
{
    return u8"Path delimiter used";
}

__constexpr std::u8string
            print_config_t::root_path_used() const noexcept
{
    return u8"Root path used";
}

__constexpr std::u8string
            print_config_t::threads_used() const noexcept
{
    return u8"Threads used";
}

__constexpr std::u8string
            print_config_t::comment_str_used() const noexcept
{
    return u8"Comment used";
}

__constexpr std::u8string
            print_config_t::general_data_file_extension_used() const noexcept
{
    return u8"general data extension used";
}

__constexpr std::u8string
    print_config_t::n_integers_used_for_creation_of_rng_used() const noexcept
{
    return u8"Number of values used to seed RNGs";
}

__constexpr std::u8string
            print_config_t::repetition_config_used() const noexcept
{
    return u8"Repetition config";
}

__constexpr std::u8string
            print_config_t::global_seed_used() const noexcept
{
    return u8"Global seed";
}

__constexpr std::u8string
            print_config_t::force_run_all_tests_used() const noexcept
{
    return u8"Force run all tests used";
}

__constexpr std::u8string
            print_config_t::test_paths_to_run_used() const noexcept
{
    return u8"test paths to run";
}

__constexpr std::u8string
            print_config_t::show_test_paths_to_run(
        const std::vector<std::u8string>& _a_strs
    ) const noexcept
{
    using namespace std;
    u8string _l_rv{};
    if (_a_strs.size() == 1 && _a_strs[0] == u8"")
    {
        return u8"No test paths given. All tests will be ran.";
    }
    for (size_t _l_idx{0}; const u8string_view _l_str : _a_strs)
    {
        _l_rv.append(_l_str);
        if (_l_idx + 2 < _a_strs.size())
        {
            _l_rv.append(u8", ");
        }
        else if (_l_idx + 1 < _a_strs.size())
        {
            _l_rv.append(u8" and ");
        }
        else
        {
            _l_rv.append(u8".");
        }
    }
    return _l_rv;
}

__no_constexpr_imp std::u8string
                   print_config_t::show_repetition_config(
        const _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t&
            _a_config
    ) const noexcept
{
    using namespace utility::printer;
    if (_a_config.size() == 0)
    {
        return u8"No repetition config given.";
    }
    else
    {
        return default_printer_t<
                   _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t>{}
            .run_printer(_a_config);
    }
}

__no_constexpr_imp std::u8string
                   print_config_t::show_path(
        const std::filesystem::path& _a_path
    ) const noexcept
{
    return _a_path.u8string();
}

__constexpr_imp std::u8string
                print_config_t::question(
        const std::u8string_view _a_str
    ) const noexcept
{
    using namespace std;
    return u8string(_a_str).append(u8"?");
}

__no_constexpr_imp std::u8string
                   print_config_t::print_global_test_list_used(
        const bool _a_lobal_test_list_used
    ) const noexcept
{
    return fmt::format(u8"{0}", _a_lobal_test_list_used);
}

_END_ABC_REPORTERS_NS