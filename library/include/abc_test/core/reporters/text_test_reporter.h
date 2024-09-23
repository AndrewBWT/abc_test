#pragma once

#include <iostream>
#include <stacktrace>
#include "fmt/color.h"

#include <fmt/std.h>
#include <array>
#include "abc_test/utility/io/threaded_text_output_reporter.h"

#include "abc_test/core/ds/test_data/invoked_test_data.h"

#include "abc_test/core/reporters/mid_execution_test_report/manual_failure.h"
#include "abc_test/core/reporters/mid_execution_test_report/test_assertion_result.h"
#include "abc_test/core/reporters/mid_execution_test_report/unhandled_exception.h"
#include "abc_test/core/reporters/mid_execution_test_report/unhandled_exception_not_derived_from_std_exception.h"

#include "abc_test/utility/str/string_table.h"

#include "abc_test/core/reporters/text_test_reporter/test_overview_printer_config.h"
#include "abc_test/core/reporters/text_test_reporter/derived_unhandled_exception_printer_config.h"
#include "abc_test/core/reporters/text_test_reporter/non_derived_unhandled_exception_printer_config.h"
#include "abc_test/core/reporters/text_test_reporter/user_defined_assertion_printer_config.h"

#include "abc_test/core/reporters/text_test_reporter/text_test_options.h"

#include "abc_test/core/reporters/text_test_reporter/test_overview_printer_config.h"

#include "abc_test/core/reporters/text_test_reporter/manual_assertion_printer_config.h"

_BEGIN_ABC_REPORTERS_NS
/*!
* Object used to print data about tests to some text output - either the console or a file
*/
struct text_test_reporter_t :
	public test_reporter_t,
	public utility::io::threated_text_output_reporter_t
{
public:
	/*!
	Constructor for std::cout to be where the output is posted.
	*/
	__constexpr
		text_test_reporter_t(
		) noexcept;
	/*!
	Constructor whcih allows a file to be where the output is posted.
	*/
	__constexpr
		text_test_reporter_t(
			const utility::io::file_name_t& _a_file_output
		) noexcept;
	__no_constexpr
		virtual
		void
		report_test(
			const reporters::after_execution_test_report_t& _a_aetr,
			const test_options_t& _a_test_options
		) override;
private:
	text_test_options_t _m_tex_test_options;
	test_overview_printer_config_t _m_test_overview_printer_config;
	colour_palette_t _m_colour_palette;
	manual_assertion_printer_config_t _m_manual_assertion_printer_config;
	non_derived_unhandlded_exception_printer_config_t _m_non_derived_unhandled_exception_printer_config;
	user_defined_printer_config_t _m_user_defined_printer_config;
	derived_unhandled_exception_printer_config_t _m_derived_unhandlded_exception_printer_config;
	bool _m_has_colour_output;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
	__no_constexpr_imp
		text_test_reporter_t::text_test_reporter_t(
		) noexcept
		: threated_text_output_reporter_t(std::cout)
	{
	}
	__constexpr_imp
		text_test_reporter_t::text_test_reporter_t(
			const utility::io::file_name_t& _a_file_output
		) noexcept
		: threated_text_output_reporter_t(_a_file_output)
	{
	}
	__no_constexpr_imp
		void
		text_test_reporter_t::report_test(
			const reporters::after_execution_test_report_t& _a_aetr,
			const test_options_t& _a_test_options
		) 
	{
		using namespace std;
		using namespace ds;
		using namespace errors;
		using namespace utility::str;
		//Define information break-line.
		const size_t _l_line_len{ _a_test_options._m_console_line_length };
		const char _l_pretty_char{ _a_test_options._m_separator_chars[0] };
		const string _l_line_break{fmt::format("{0}\n",string(
			_l_line_len, _l_pretty_char)) };
		string_table_t _l_st({ 0 });
		for (const test_info_element_t& _a_option : _m_tex_test_options.test_info_list())
		{
			if (_m_test_overview_printer_config.check_data(_a_option, _a_aetr))
			{
				_l_st.push_back(string(_m_test_overview_printer_config.info_id(_a_option.first)).append(": "));
				_l_st.push_back(_m_test_overview_printer_config.get_data(_a_option.first, _m_colour_palette, _a_aetr));
				_l_st.new_line();
			}
		}
		//Print the output
		string _l_rv{ fmt::format("{0}{1}{0}",_l_line_break,_l_st()) };
		if (_a_aetr.test_warnings_recieved() > 0)
		{
			string _l_warning_str;
			_l_rv.append(fmt::format("{1}{0}", _l_line_break, _l_warning_str));
		}
		if (_a_aetr.test_reports_recieved() > 0)
		{
			string_table_t _l_st({ 0 });
			size_t _l_idx{ 1 };
			const mid_execution_test_reporters_t& _l_reports{ _a_aetr.reports() };
			for (const mid_execution_test_report_ptr_t& _l_report : _l_reports)
			{
				vector<string> _l_strs;
				const manual_assertion_t* _l_ma{ dynamic_cast<manual_assertion_t*>(_l_report.get()) };
				const user_defined_assertion_t* _l_uda{ dynamic_cast<user_defined_assertion_t*>(_l_report.get()) };
				const unhandled_exception_t* _l_ue{ dynamic_cast<unhandled_exception_t*>(_l_report.get()) };
				const unhandled_exception_not_derived_from_std_exception_t* _l_uend{
					dynamic_cast<unhandled_exception_not_derived_from_std_exception_t*>(_l_report.get()) };
				//Ordering here is important!
				//Hierarchy is like so:
				/*
				* + mid_execution_test_report (abstract)
				* |+ manual_assertion_t
				* ||- user_defined_assertion_t
				* |+ unhandled_exception_not_derived_from_std_exception_t
				*  |- unhandled_exception_t
				* 
				* So checks must be leaf before parent node.
				*/
				if (_l_ue != nullptr)
				{
					_l_strs = _m_derived_unhandlded_exception_printer_config.get_all_data(
						_m_tex_test_options.derived_exception_info_list(),
						_m_colour_palette,
						*_l_ue, _m_tex_test_options.indent());
				}
				else if (_l_uend != nullptr)
				{
					_l_strs = _m_non_derived_unhandled_exception_printer_config.get_all_data(
						_m_tex_test_options.non_derived_exception_info_list(),
						_m_colour_palette,
						*_l_uend,_m_tex_test_options.indent());
				}
				else if (_l_uda != nullptr)
				{
					_l_strs = _m_user_defined_printer_config.get_all_data(
						_m_tex_test_options.user_defined_assertion_info_list(),
						_m_colour_palette,
						*_l_uda, _m_tex_test_options.indent());
				}
				else if (_l_ma != nullptr)
				{
					_l_strs = _m_manual_assertion_printer_config.get_all_data(
						_m_tex_test_options.manual_assertion_info_list(),
						_m_colour_palette,
						*_l_ma, _m_tex_test_options.indent());
				}
				else
				{
					throw errors::test_library_exception_t(fmt::format(
						"Unable to find non-abstract subclass of {0}. ",
						typeid(mid_execution_test_report_t).name()
					));
				}
				size_t _l_data_idx{ 0 };
				for (const string_view _l_str : _l_strs)
				{
					_l_st.push_back(
						_l_data_idx++ == 0 ?
						fmt::format(" {0})  ", _l_idx) :
						"");
					_l_st.push_back(_l_str);
					_l_st.new_line();
				}
				_l_idx++;
			}
			_l_rv.append(fmt::format("{0}{1}", _l_st(), _l_line_break));
		}
		write(_l_rv);
	}
_END_ABC_REPORTERS_NS