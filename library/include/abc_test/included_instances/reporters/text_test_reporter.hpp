#pragma once

#include "abc_test/core/ds/test_data/invoked_test_data.hpp"
#include "abc_test/core/reporters/test_reporter.hpp"
#include "abc_test/core/test_reports/assertion_status/fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/core/test_reports/assertion_status/terminate.hpp"
#include "abc_test/core/test_reports/unexpected_thrown_exception.hpp"
#include "abc_test/core/test_reports/unexpected_thrown_non_descript_entity.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/multi_element_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/after_execution_test_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/finalised_test_set_data_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/matcher_based_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/matcher_based_assertion_single_line.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/multi_element_assertion_block.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/pre_test_set_data_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/static_assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_thrown_exception.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/unexpected_thrown_non_descript_entity.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.hpp"
#include "abc_test/utility/io/threaded_text_output_reporter.hpp"

#include <array>
#include <fmt/color.h>
#include <fmt/std.h>
#include <iostream>
#include <stacktrace>
_BEGIN_ABC_REPORTERS_NS

/*!
 * Object used to print data about tests to some text output - either the
 * console or a file
 */
struct text_test_reporter_t
    : public test_reporter_t
    , public utility::io::threated_text_output_reporter_t
{
public:
    /*!
    Constructor for std::cout to be where the output is posted.
    */
    __no_constexpr
        text_test_reporter_t() noexcept;
    __no_constexpr
        text_test_reporter_t(const bool _a_coloured) noexcept;
    /*!
    Constructor whcih allows a file to be where the output is posted.
    */
    __no_constexpr
        text_test_reporter_t(const std::filesystem::path& _a_file_output
        ) noexcept;
    __no_constexpr virtual void
        report_test(const ds::invoked_test_data_t& _a_itd) const override;
    __no_constexpr virtual void
        final_report(ds::finalised_test_set_data_t& _a_test_set_data
        ) const noexcept override;
    __no_constexpr virtual void
        pre_test_run_report(ds::pre_test_run_report_t& _a_pre_test_run_report
        ) const noexcept override;
    __constexpr bool has_colour_output() const noexcept
    {
        return _m_has_colour_output;
    }
private:
    bool           _m_has_colour_output;
    print_config_t _m_print_config;
    __constexpr void
        process_termination(
            const reports::unexpected_report_t<true>& _a_unexpected_report
        ) const;
    __constexpr void
        process_assertion(
            const reports::assertion_base_t* _a_gur,
            const std::size_t                _a_idx,
            const std::size_t                _a_total_indexes
        ) const;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__no_constexpr_imp
    text_test_reporter_t::text_test_reporter_t() noexcept
    : threated_text_output_reporter_t(std::cout)
    , _m_has_colour_output(false)
    , _m_print_config(print_config_t(false))
{}
__no_constexpr_imp
text_test_reporter_t::text_test_reporter_t(const bool _a_coloured) noexcept
    : threated_text_output_reporter_t(std::cout)
    , _m_has_colour_output(_a_coloured)
    , _m_print_config(print_config_t(_a_coloured))
{
}
__no_constexpr_imp
    text_test_reporter_t::text_test_reporter_t(
        const std::filesystem::path& _a_file_output
    ) noexcept
    : threated_text_output_reporter_t(_a_file_output)
    , _m_has_colour_output(false)
    , _m_print_config(print_config_t(false))
{}

__no_constexpr_imp void
    text_test_reporter_t::report_test(
        const ds::invoked_test_data_t& _a_itd
    ) const
{
    using namespace std;
    using namespace ds;
    using namespace errors;
    using namespace utility::str;
    using namespace reporters;
    using namespace reports;
    const u8string _l_line_break{_m_print_config.line_break()};
    write_line(fmt::format(u8"{1}\n{0}", _l_line_break, u8"TEST INFO"));
    test_report_list_formatter().process_all_data(
        _m_print_config.after_execution_test_report_fields(),
        _a_itd,
        _m_print_config,
        *this
    );
    // Print the output
    if (_a_itd.unexpected_termination() != nullptr)
    {
        const unexpected_report_t<true>& _l_unexpected_termination{
            *_a_itd.unexpected_termination()
        };
        write_line(fmt::format(
            u8"{1}{0}{0}", _l_line_break, u8"UNEXPECTED TERMINATION INFORMATION\n"
        ));
        process_termination(_l_unexpected_termination);
    }
    if (_a_itd.warnings_recieved() > 0)
    {
        u8string _l_warning_str;
        write_line(fmt::format(
            u8"{2}{0}{1}{0}", _l_line_break, _l_warning_str, u8"WARNINGS\n"
        ));
    }
    const assertion_base_collection_t& _l_reports{_a_itd.assertions()};
    if (_l_reports.size() > 0)
    {
        write_line(fmt::format(u8"{0}\n{1}\n{0}", _l_line_break, u8"ASSERTION INFO"));
        for (const assertion_base_ptr_t& _l_report : _l_reports)
        {
            process_assertion(
                _l_report.get(),
                _l_report.get()->assertion_index(),
                _a_itd.largest_assertion_index_added()
            );
        }
    }
    write_line(_l_line_break);
}

__no_constexpr_imp void
    text_test_reporter_t::final_report(
        ds::finalised_test_set_data_t& _a_test_set_data
    ) const noexcept
{
    using namespace std;
    using namespace ds;
    using namespace errors;
    using namespace utility::str;
    using namespace reporters;
    using namespace reports;
    const u8string _l_line_break{_m_print_config.line_break()};
    write_line(fmt::format(u8"{0}\n{1}\n{0}", _l_line_break, u8"TEST SUITE RESULTS"));
    finalised_test_set_data_list_formatter().process_all_data(
        _m_print_config.finalised_test_set_data_fields(),
        _a_test_set_data,
        _m_print_config,
        *this
    );
    write_line(_l_line_break);
}

__no_constexpr_imp void
    text_test_reporter_t::pre_test_run_report(
        ds::pre_test_run_report_t& _a_pre_test_run_report
    ) const noexcept
{
    using namespace std;
    using namespace ds;
    using namespace errors;
    using namespace utility::str;
    using namespace reporters;
    using namespace reports;
    const u8string _l_line_break{_m_print_config.line_break()};
    write_line(
        fmt::format(u8"{0}\n{1}\n{0}", _l_line_break, u8"TEST SUITE CONFIGURATION")
    );
    pre_test_set_data_list_formatter().process_all_data(
        _m_print_config.pre_test_set_data_fields(),
        _a_pre_test_run_report,
        _m_print_config,
        *this
    );
    write_line(_l_line_break);
}

__constexpr void
    text_test_reporter_t::process_termination(
        const reports::unexpected_report_t<true>& _a_unexpected_report
    ) const
{
    using namespace reports;
    using namespace _ABC_NS_UTILITY_STR;
    if (auto _l_ptr{dynamic_cast<const unexpected_thrown_exception_t*>(
            &_a_unexpected_report
        )};
        _l_ptr != nullptr)
    {
        unexpected_thrown_exception_list_formatter_t()
            .list_formattable_t<
                reports::unexpected_thrown_exception_t,
                combined_enum_unexpected_exception_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.unexpected_thrown_exception_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{dynamic_cast<
                 const unexpected_thrown_not_derived_from_std_exception_t*>(
                 &_a_unexpected_report
             )};
             _l_ptr != nullptr)
    {
        unexpected_thrown_non_descript_entity_list_formatter_t()
            .list_formattable_t<
                reports::unexpected_thrown_not_derived_from_std_exception_t,
                combined_enum_unexpected_thrown_non_descript_entity_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config
                        .unexpected_thrown_non_descript_entity_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else
    {
        throw errors::test_library_exception_t(fmt::format(
            u8"Could not find function to format item of abstract class {0}. ",
            type_id<decltype(_a_unexpected_report)>()
        ));
    }
}

__constexpr_imp void
    text_test_reporter_t::process_assertion(
        const reports::assertion_base_t* _a_gur,
        const std::size_t                _a_idx,
        const std::size_t                _a_total_indexes
    ) const
{
    using namespace reports;
    using namespace _ABC_NS_UTILITY_STR;
    if (auto _l_ptr{dynamic_cast<
            const matcher_based_assertion_single_line_t<pass_or_fail_t>*>(_a_gur
        )};
        _l_ptr != nullptr)
    {
        matcher_based_assertion_single_line_list_formatter_t<pass_or_fail_t>(
            _a_idx, _a_total_indexes
        )
            .list_formattable_t<
                reports::matcher_based_assertion_single_line_t<pass_or_fail_t>,
                combined_enum_matcher_based_assertion_single_line_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.matcher_based_assertion_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{
                 dynamic_cast<const matcher_based_assertion_single_line_t<
                     pass_or_terminate_t>*>(_a_gur)
             };
             _l_ptr != nullptr)
    {
        matcher_based_assertion_single_line_list_formatter_t<
            pass_or_terminate_t>(_a_idx, _a_total_indexes)
            .list_formattable_t<
                reports::matcher_based_assertion_single_line_t<
                    pass_or_terminate_t>,
                combined_enum_matcher_based_assertion_single_line_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.matcher_based_assertion_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{dynamic_cast<
                 const matcher_based_assertion_block_t<pass_or_fail_t>*>(_a_gur)
             };
             _l_ptr != nullptr)
    {
        matcher_based_assertion_block_list_formatter_t<pass_or_fail_t>(
            _a_idx, _a_total_indexes
        )
            .list_formattable_t<
                reports::matcher_based_assertion_block_t<pass_or_fail_t>,
                combined_enum_matcher_based_assertion_block_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.matcher_based_assertion_block_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{dynamic_cast<
                 const matcher_based_assertion_block_t<pass_or_terminate_t>*>(
                 _a_gur
             )};
             _l_ptr != nullptr)
    {
        matcher_based_assertion_block_list_formatter_t<pass_or_terminate_t>(
            _a_idx, _a_total_indexes
        )
            .list_formattable_t<
                reports::matcher_based_assertion_block_t<pass_or_terminate_t>,
                combined_enum_matcher_based_assertion_block_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.matcher_based_assertion_block_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{dynamic_cast<
                 const multi_element_assertion_block_t<pass_or_fail_t>*>(_a_gur)
             };
             _l_ptr != nullptr)
    {
        multi_element_test_block_list_formatter_t<pass_or_fail_t>(
            _a_idx, _a_total_indexes
        )
            .list_formattable_t<
                reports::multi_element_assertion_block_t<pass_or_fail_t>,
                combined_enum_multi_element_assertion_block_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.multi_element_test_block_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{dynamic_cast<
                 const multi_element_assertion_block_t<pass_or_terminate_t>*>(
                 _a_gur
             )};
             _l_ptr != nullptr)
    {
        multi_element_test_block_list_formatter_t<pass_or_terminate_t>(
            _a_idx, _a_total_indexes
        )
            .list_formattable_t<
                reports::multi_element_assertion_block_t<pass_or_terminate_t>,
                combined_enum_multi_element_assertion_block_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.multi_element_test_block_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{dynamic_cast<const static_assertion_t<pass_t>*>(_a_gur)
             };
             _l_ptr != nullptr)
    {
        static_assertion_list_formatter_t<pass_t>(_a_idx, _a_total_indexes)
            .list_formattable_t<
                reports::static_assertion_t<pass_t>,
                combined_enum_static_assertion_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.static_assertion_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{dynamic_cast<const static_assertion_t<fail_t>*>(_a_gur)
             };
             _l_ptr != nullptr)
    {
        static_assertion_list_formatter_t<fail_t>(_a_idx, _a_total_indexes)
            .list_formattable_t<
                reports::static_assertion_t<fail_t>,
                combined_enum_static_assertion_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.static_assertion_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else if (auto _l_ptr{
                 dynamic_cast<const static_assertion_t<terminate_t>*>(_a_gur)
             };
             _l_ptr != nullptr)
    {
        static_assertion_list_formatter_t<terminate_t>(_a_idx, _a_total_indexes)
            .list_formattable_t<
                reports::static_assertion_t<terminate_t>,
                combined_enum_static_assertion_fields_t,
                print_config_t>::
                process_all_data(
                    _m_print_config.static_assertion_fields(),
                    *_l_ptr,
                    _m_print_config,
                    *this
                );
    }
    else
    {
        throw errors::test_library_exception_t(fmt::format(
            u8"Could not find function to format item of abstract class {0}. ",
            type_id<decltype(*_a_gur)>()
        ));
    }
}

_END_ABC_REPORTERS_NS