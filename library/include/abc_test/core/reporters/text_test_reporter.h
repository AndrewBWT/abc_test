#pragma once

#include <iostream>
#include <stacktrace>
#include "fmt/color.h"

#include <fmt/std.h>
#include <array>
#include "abc_test/utility/io/threaded_text_output_reporter.h"

#include "abc_test/core/ds/test_data/invoked_test_data.h"

#include "abc_test/utility/str/string_table.h"

#include "abc_test/core/reporters/text_test_reporter/list_formatter/generic_matcher_based_assertion.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/matcher_based_assertion.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/matcher_based_assertion_block.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/static_assertion.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/after_execution_test_report.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass_or_fail.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass_or_terminate.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/fail.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/terminate.h"
#include "abc_test/core/reporters/text_test_reporter/print_config.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/unexpected_thrown_exception.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/unexpected_thrown_non_descript_entity.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/unexpected_thrown_exception.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/unexpected_thrown_non_descript_entity.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/matcher_based_assertion.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/matcher_based_assertion_block.h"
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
	__no_constexpr
		text_test_reporter_t(
		) noexcept;
	/*!
	Constructor whcih allows a file to be where the output is posted.
	*/
	__no_constexpr
		text_test_reporter_t(
			const utility::io::file_name_t& _a_file_output
		) noexcept;
	__no_constexpr
		virtual
		void
		report_test(
			const after_execution_test_report_t& _a_aetr,
			const test_options_t& _a_test_options
		) override;
private:
	bool _m_has_colour_output;
	print_config_t _m_print_config;
	__constexpr
		std::vector<std::string>
		process_termination(
			const reports::unexpected_report_t<true>* _a_unexpected_report
		) const;
	__constexpr
		std::vector<std::string>
		process_assertion(
			const reports::generic_assertion_type_t* _a_gur
		) const;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
	__no_constexpr_imp
		text_test_reporter_t::text_test_reporter_t(
		) noexcept
		: threated_text_output_reporter_t(std::cout)
	, _m_print_config(print_config_t(true))
	, _m_has_colour_output(false)
	{
	}
__no_constexpr_imp
		text_test_reporter_t::text_test_reporter_t(
			const utility::io::file_name_t& _a_file_output
		) noexcept
		: threated_text_output_reporter_t(_a_file_output)
		, _m_print_config(print_config_t(false))
		, _m_has_colour_output(true)
	{
	}
	__no_constexpr_imp
		void
		text_test_reporter_t::report_test(
			const after_execution_test_report_t& _a_aetr,
			const test_options_t& _a_test_options
		) 
	{
		using namespace std;
		using namespace ds;
		using namespace errors;
		using namespace utility::str;
		using namespace reporters;
		using namespace reports;
		//list_formattable_t<int, char> _l_y;
		const size_t _l_line_len{ _a_test_options._m_console_line_length };
		const char _l_pretty_char{ _a_test_options._m_separator_chars[0] };
		const string _l_line_break{fmt::format("{0}\n",string(
			_l_line_len, _l_pretty_char)) };
		string_table_t _l_st({ 0 });
		const vector<vector<string>> _l_strs = get_all_data<false>(_m_print_config.after_execution_test_report_fields(),
			_a_aetr, _m_print_config, test_report_list_formatter());
		for (const vector<string>& _l_row : _l_strs)
		{
			for (const string& _l_cell : _l_row)
			{
				_l_st.push_back(_l_cell);
			}
			_l_st.new_line();
		}
		//Print the output
		string _l_rv{ fmt::format("{0}{1}{0}{2}{0}",_l_line_break,"TEST INFO\n",_l_st())};
		_l_st = string_table_t({ 0 });
		if (_a_aetr.unexpected_termination().has_value())
		{
			const vector<string> _l_strs = process_termination(_a_aetr.unexpected_termination().value().get());
			for (const string_view _l_str : _l_strs)
			{
				_l_st.push_back(_l_str);
				_l_st.new_line();
			}
			_l_rv.append(fmt::format("{2}{1}{0}{1}", _l_st(), _l_line_break, "UNEXPECTED TERMINATION INFORMATION\n"));
		}
		if (_a_aetr.test_warnings_recieved() > 0)
		{
			string _l_warning_str;
			_l_rv.append(fmt::format("{2}{0}{1}{0}", _l_line_break, _l_warning_str, "WARNINGS\n"));
		}
		if (_a_aetr.assertions_recieved() > 0)
		{
			string_table_t _l_st({ 0 });
			size_t _l_idx{ 1 };
			const generic_user_report_collection_t& _l_reports{ _a_aetr.assertions() };
			for (const generic_user_report_ptr_t& _l_report : _l_reports)
			{
				const vector<string> _l_strs = process_assertion(_l_report.get());
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
			/*for (const function_report_ptr_t& _l_report : _l_reports)
			{
				const vector<string> _l_strs = _m_text_test_options.process_function_report(_l_report.get());
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
			}*/
			_l_rv.append(fmt::format("{2}{1}{0}{1}", _l_st(), _l_line_break, "ASSERTION INFO\n"));
		}
		write(_l_rv);
	}
__constexpr
	std::vector<std::string>
	text_test_reporter_t::process_termination(
		const reports::unexpected_report_t<true>* _a_unexpected_report
	) const
{
	using namespace reports;
	if (auto _l_ptr{ dynamic_cast<const unexpected_thrown_exception_t*>(_a_unexpected_report) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.unexpected_thrown_exception_fields(), *_l_ptr,
			_m_print_config, unexpected_thrown_exception_list_formatter_t());
	}
	else if (auto _l_ptr{ dynamic_cast<const unexpected_thrown_non_descript_entity_t*>(_a_unexpected_report) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.unexpected_thrown_non_descript_entity_fields(), *_l_ptr,
			_m_print_config, unexpected_thrown_non_descript_entity_list_formatter_t());
	}
	else
	{
		throw errors::test_library_exception_t(fmt::format(
			"Could not find function to format item of abstract class {0}. ",
			typeid(*_a_unexpected_report).name()
		));
	}
	return {};
}
__constexpr_imp
	std::vector<std::string>
	text_test_reporter_t::process_assertion(
		const reports::generic_assertion_type_t* _a_gur
	) const
{
	using namespace reports;
	if (auto _l_ptr{ dynamic_cast<const matcher_based_assertion_t<pass_or_fail_t>*>(_a_gur) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.matcher_based_assertion_fields(), *_l_ptr,
			_m_print_config, matcher_based_assertion_list_formatter_t<pass_or_fail_t>());
	}
	else if (auto _l_ptr{ dynamic_cast<const matcher_based_assertion_t<pass_or_terminate_t>*>(_a_gur) };
			_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.matcher_based_assertion_fields(), *_l_ptr,
			_m_print_config, matcher_based_assertion_list_formatter_t<pass_or_terminate_t>());
	}
	else if (auto _l_ptr{ dynamic_cast<const matcher_based_assertion_block_t<pass_or_fail_t>*>(_a_gur) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.matcher_based_assertion_block_fields(), *_l_ptr,
			_m_print_config, matcher_based_assertion_block_list_formatter_t<pass_or_fail_t>());
	}
	else if (auto _l_ptr{ dynamic_cast<const matcher_based_assertion_block_t<pass_or_terminate_t>*>(_a_gur) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.matcher_based_assertion_block_fields(), *_l_ptr,
			_m_print_config, matcher_based_assertion_block_list_formatter_t<pass_or_terminate_t>());
	}
	else if (auto _l_ptr{ dynamic_cast<const static_assertion_t<pass_t>*>(_a_gur) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.static_assertion_fields(), *_l_ptr,
			_m_print_config, static_assertion_list_formatter_t<pass_t>());
	}
	else if (auto _l_ptr{ dynamic_cast<const static_assertion_t<fail_t>*>(_a_gur) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.static_assertion_fields(), *_l_ptr,
			_m_print_config, static_assertion_list_formatter_t<fail_t>());
	}
	else if (auto _l_ptr{ dynamic_cast<const static_assertion_t<terminate_t>*>(_a_gur) };
		_l_ptr != nullptr)
	{
		return get_all_data(_m_print_config.static_assertion_fields(), *_l_ptr,
			_m_print_config, static_assertion_list_formatter_t<terminate_t>());
	}
	else
	{
		throw errors::test_library_exception_t(fmt::format(
			"Could not find function to format item of abstract class {0}. ",
			typeid(*_a_gur).name()
		));
	}
	return {};
}
_END_ABC_REPORTERS_NS