#pragma once

#include <iostream>
#include <stacktrace>
#include "fmt/color.h"

#include <fmt/std.h>
#include <array>
#include "abc_test/utility/io/threaded_text_output_reporter.h"

#include "abc_test/core/ds/test_data/invoked_test_data.h"

#include "abc_test/core/errors/test_failures_info.h"
#include "abc_test/core/reporters/mid_execution_test_report/manual_failure.h"
#include "abc_test/core/reporters/mid_execution_test_report/test_assertion_result.h"
#include "abc_test/core/reporters/mid_execution_test_report/unhandled_exception.h"
#include "abc_test/core/reporters/mid_execution_test_report/unhandled_exception_not_derived_from_std_exception.h"


_BEGIN_ABC_REPORTERS_NS

enum class enum_info_data_to_print_t
{
	TITLE,
	NAME,
	LOCATION,
	TEST_PATH,
	SEED_USED,
	SEED_USED_HEX,
	SEED_TO_USE_TO_RE_RUN_TEST,
	SEED_TO_USE_TO_RE_RUN_TEST_HEX,
	TOTAL_ASSERTIONS_RAN,
	TOTAL_ASSERTIONS_PASSED,
	TOTAL_ASSERTIONS_FAILED,
	CONDENSED_ASSERTIONS_PASSED_INFO,
	TOTAL_WARNINGS,
	TERMINATED_EARLY
};
enum class enum_test_report_data_t
{
	PASSED,
	SOURCE_LOCATION,
	EARLY_TERMINATION,
	LOG_INFOS
};
enum class enum_manual_failure_t
{
	SOURCE_CODE_REPRESENTATION,
	STR_REPRESENTATION
};
enum class enum_test_assertion_t
{
	STR_REPRESENTATION,
	MATCHER_SOURCE_MAP,
	SOURCE_CODE_REPRESENTATION,
	MATCHER_STRING
};
using combined_enum_manual_failure_t = std::variant<enum_test_report_data_t, enum_manual_failure_t>;
using combined_enum_test_assertion_t = std::variant<enum_test_report_data_t, enum_test_assertion_t>;
using enum_manual_failure_collection_t = std::vector<std::pair<combined_enum_manual_failure_t,bool>>;
using enum_test_assertion_collection_t = std::vector<std::pair<combined_enum_test_assertion_t, bool>>;
using enum_info_data_to_print_collection_t = std::vector<std::pair<enum_info_data_to_print_t,bool>>;
using string_table_t = std::vector<std::vector<std::string>>;
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
		) noexcept override;
private:
	std::size_t _m_indent = 3;
	bool _m_print_successes;
	bool _m_has_colour_output;
	enum_info_data_to_print_collection_t _m_data_to_print_collection;
	enum_manual_failure_collection_t _m_mid_exception_test_report_collection;
	enum_test_assertion_collection_t _m_test_assertion_collection;
	std::string _m_info_line_padding;
	std::size_t _m_report_element_indent_size;
	__constexpr
		std::size_t
		get_largest_info_identifier(
			const reporters::after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		std::string
		get_info_data_from_test_report(
			const enum_info_data_to_print_t _a_enum_info_data,
			const reporters::after_execution_test_report_t& _a_aetr,
			const test_options_t& _a_test_options
		) const;
	__constexpr
		std::string
		style_string(
			const std::string_view _a_str,
			const fmt::text_style& _a_style
		) const noexcept;
	__constexpr
		virtual
		std::string
		format_warning(
			const std::shared_ptr<reporters::mid_execution_test_warning_t>& _a_warning
		) const noexcept;
	__constexpr
		virtual
		std::string
		format_report(
			const std::shared_ptr<reporters::mid_execution_test_report_t>& _a_report,
			const test_options_t* _a_test_options,
			const std::size_t _a_idx,
			const std::size_t _a_maximum_idx_str_size
		) const noexcept;
	__constexpr
		void
		append_indented_line(
			std::string& _a_str,
			const string_table_t& _a_print_vector,
			const std::string_view _a_indented_str
		) const noexcept;
	__no_constexpr
		std::string
		format_manual_failure(
			const manual_failure_t& _a_mf,
			const std::size_t _a_idx,
			const std::size_t _a_largest_idx,
			const test_options_t* _a_test_options
		) const noexcept;
	__constexpr
		bool
		check_if_execution_test_report_data_to_be_included(
			const enum_test_report_data_t _a_etrd,
			const mid_execution_test_report_t* _a_metr
		) const;
	__constexpr
		bool
		check_if_manual_data_to_be_included(
			const combined_enum_manual_failure_t _a_cemf,
			const manual_failure_t& _a_mf
		) const;
	__constexpr
		bool
		check_if_test_assertion_data_to_be_included(
			const combined_enum_test_assertion_t _a_ceta,
			const test_assertion_result_t& _a_tar
		) const;
	__constexpr
		std::string
		prepare_indexed_table(
			const string_table_t& _a_str_table
		) const noexcept;
	__constexpr
		std::string
		indent(
			const std::string_view _a_str
		) const noexcept;
	__constexpr
		void
		get_execution_test_report_data_as_table(
			string_table_t& _a_str_table,
			const enum_test_report_data_t _a_etrd,
			const mid_execution_test_report_t& _a_metr,
			const test_options_t* _a_test_options,
			const std::string_view _a_other_initial_cells
		) const;
	__constexpr
		string_table_t
		get_manual_data_as_table(
			const combined_enum_manual_failure_t _a_cemf,
			const manual_failure_t& _a_mf,
			const test_options_t* _a_test_options,
			const std::string_view _a_other_initial_cells
		) const;
	__constexpr
		string_table_t
		get_test_assertion_data_as_table(
			const combined_enum_test_assertion_t _a_cemf,
			const test_assertion_result_t& _a_mf,
			const test_options_t* _a_test_options,
			const std::string_view _a_other_initial_cells
		) const;
	__constexpr
		std::string
		format_test_assertion_result(
			const test_assertion_result_t& _a_mf,
			const std::size_t _a_idx,
			const std::size_t _a_maximum_idx_str_size,
			const test_options_t* _a_test_options
		) const noexcept;
	__constexpr
		std::string
		format_unhandled_exception(
			const unhandled_exception_t& _a_mf
		) const noexcept;
	__constexpr
		std::string
		format_unhandled_exception_not_derived_from_std_exception(
			const unhandled_exception_not_derived_from_std_exception_t& _a_mf
		) const noexcept;
};

namespace
{
	__constexpr
		const char*
		get_info_data_enum_text_data(
			const enum_info_data_to_print_t _a_enum_info_data
		);
	__constexpr
		bool
		check_if_info_data_to_be_included(
			const enum_info_data_to_print_t _a_enum_info_data,
			const reporters::after_execution_test_report_t& _a_aetr
		);
	/*!
	* Takes a string created by a CHECK_EXPR macro and ensures that it looks identical to what the
	* user wrote - as in, removing the placeholder dummy object.
	*/
	__constexpr
		std::string
		normalise_line_str_rep(
			const std::string_view _a_str
		) noexcept;

	//Const char*s
	const char* _c_unknown{ "<unknown>" };
	const char* _c_no_seed_used{ "No seed used" };
	const std::size_t _c_test_result_str_size{ 2 };
	const char* _c_test_info{ "TEST RESULT" };
	const char* _c_test_result_str[_c_test_result_str_size] = {
		"TEST PASSED",
		"TEST FAILED"
	};
	const std::size_t _c_terminated_early_size{ 2 };
	const char* _c_terminated_early[_c_terminated_early_size] = {
		"Yes",
		"No"
	};
	const char* _c_condensed_assertions_passed
	{
		"all assertions passed."
	};


	const std::size_t _c_strs_identifiers_size{ 3 };
	const char* _c_strs_identifiers[_c_strs_identifiers_size] = { "Function","Location","Seed used" };
	const char* _c_test_failure_str{ "TEST_FAILURE" };
	const char* _c_alt_seed_str{ "To repeat test, use seed" };
	const char* _c_normalised_line_str_prefix_1{ "(abc::placeholder_t(" };
	const char* _c_normalised_line_str_prefix_2{ ") < " };
	const char _c_quote_char{ '"' };
	const char* _c_lbracket{ "(" };
	const char* _c_rbracket{ ")" };
	const std::size_t _c_error_strs_size{ 3 };
	const char* _c_error_strs[_c_error_strs_size] = {
		"Test assertion failure",
		"Manual test failure encountered",
		"Unexpected exception encountered"
	};
	const char* _c_error_str_terminated{ ", function terminated" };
	const std::size_t _c_location_strs_size{ 2 };
	const char* _c_location_strs[_c_location_strs_size] = { "Location: ", "Most recent test invokation, assertion or log occoured on line: " };
	const std::size_t _c_failed_expr_strs_size{ 2 };
	const char* _c_failed_expr_strs[_c_failed_expr_strs_size] = { "Failed expression:","Matcher's output:" };
	const std::size_t _c_exception_strs_size{ 3 };
	const char* _c_exception_strs[_c_exception_strs_size] = {
		"Exception's type: ",
		"Exception was not derived from std::exception, therefore we have no additional information pertaining to it",
		"Exception's what() function returned:" };
	const std::size_t _c_logged_strs_size{ 2 };
	const char* _c_logged_strs[_c_logged_strs_size] = {
		"Logged info",
		"Logged info (cached, may be inaccurate)"
	};
	const char* _c_internal_error_string{ "The following internal failures found:" };
	const char* _c_no_rtd_description{ "<No valid test name, internal error>" };
	const char* _c_no_sl{ "<No valid source location, internal error>" };
	const char* _c_no_pstd{ "<invoked_test_info_t has no post_setup_test_data_t member variable>" };
	const char* _c_no_rtd{ "<post_setup_test_data_t has no valid registered_test_data_t member variable>" };
	const char* _l_has_sl{ "<registered_test_data_t has no valid source_location variable>" };
}
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
	__no_constexpr_imp
		text_test_reporter_t::text_test_reporter_t(
		) noexcept
		: threated_text_output_reporter_t(std::cout)
	{
		{
			using enum enum_info_data_to_print_t;
			_m_data_to_print_collection.push_back({ TITLE,true });
			_m_data_to_print_collection.push_back({ NAME,true });
			_m_data_to_print_collection.push_back({ LOCATION,true });
			_m_data_to_print_collection.push_back({ TEST_PATH,true });
			_m_data_to_print_collection.push_back({ SEED_USED,false });
			//_m_data_to_print_collection.push_back({ SEED_USED_HEX,true });
			//_m_data_to_print_collection.push_back({ SEED_TO_USE_TO_RE_RUN_TEST,true });
			_m_data_to_print_collection.push_back({ SEED_TO_USE_TO_RE_RUN_TEST_HEX,false });
			//_m_data_to_print_collection.push_back({ TOTAL_ASSERTIONS_RAN,true });
			//_m_data_to_print_collection.push_back({ TOTAL_ASSERTIONS_PASSED,true });
			//_m_data_to_print_collection.push_back({ TOTAL_ASSERTIONS_FAILED,true });
			_m_data_to_print_collection.push_back({ CONDENSED_ASSERTIONS_PASSED_INFO,true });
			_m_data_to_print_collection.push_back({ TOTAL_WARNINGS,true });
			_m_data_to_print_collection.push_back({ TERMINATED_EARLY,true });
		}
	{
		using enum enum_test_report_data_t;
		using enum enum_manual_failure_t;
		_m_mid_exception_test_report_collection.push_back({ STR_REPRESENTATION, false });
		//_m_mid_exception_test_report_collection.push_back({ PASSED, false });
		_m_mid_exception_test_report_collection.push_back({ SOURCE_LOCATION, false });
		_m_mid_exception_test_report_collection.push_back({ SOURCE_CODE_REPRESENTATION, false });
		//_m_mid_exception_test_report_collection.push_back({ EARLY_TERMINATION, false });
		_m_mid_exception_test_report_collection.push_back({ LOG_INFOS, false });
		//_m_mid_exception_test_report_collection.push_back({ STR_REPRESENTATION, false });
	}
	{
		using enum enum_test_report_data_t;
		using enum enum_test_assertion_t;
		_m_test_assertion_collection.push_back({ STR_REPRESENTATION, false });
		//_m_mid_exception_test_report_collection.push_back({ PASSED, false });
		_m_test_assertion_collection.push_back({ MATCHER_SOURCE_MAP, false });
		_m_test_assertion_collection.push_back({ SOURCE_CODE_REPRESENTATION, false });
		//_m_mid_exception_test_report_collection.push_back({ EARLY_TERMINATION, false });
		_m_test_assertion_collection.push_back({ MATCHER_STRING, false });
		_m_test_assertion_collection.push_back({ SOURCE_LOCATION, false });
		_m_test_assertion_collection.push_back({ LOG_INFOS, false });
		//_m_mid_exception_test_report_collection.push_back({ STR_REPRESENTATION, false });
	}

	}
	__constexpr_imp
		text_test_reporter_t::text_test_reporter_t(
			const utility::io::file_name_t& _a_file_output
		) noexcept
		: threated_text_output_reporter_t(_a_file_output)
	{
		using enum enum_info_data_to_print_t;
		_m_data_to_print_collection.push_back({ TITLE,true });
		_m_data_to_print_collection.push_back({ NAME,true });
		_m_data_to_print_collection.push_back({ LOCATION,true });
		_m_data_to_print_collection.push_back({ TEST_PATH,true });
		_m_data_to_print_collection.push_back({ SEED_USED,true });
		_m_data_to_print_collection.push_back({ SEED_USED_HEX,true });
		_m_data_to_print_collection.push_back({ SEED_TO_USE_TO_RE_RUN_TEST,true });
		_m_data_to_print_collection.push_back({ SEED_TO_USE_TO_RE_RUN_TEST_HEX,true });
		_m_data_to_print_collection.push_back({ TOTAL_ASSERTIONS_RAN,true });
		_m_data_to_print_collection.push_back({ TOTAL_ASSERTIONS_PASSED,true });
		_m_data_to_print_collection.push_back({ TOTAL_ASSERTIONS_FAILED,true });
		_m_data_to_print_collection.push_back({ CONDENSED_ASSERTIONS_PASSED_INFO,true });
		_m_data_to_print_collection.push_back({ TOTAL_WARNINGS,true });
		_m_data_to_print_collection.push_back({ TERMINATED_EARLY,true });
	}
	__no_constexpr_imp
		void
		text_test_reporter_t::report_test(
			const reporters::after_execution_test_report_t& _a_aetr,
			const test_options_t& _a_test_options
		) noexcept 
	{
		using namespace std;
		using namespace ds;
		using namespace errors;
		using namespace utility::str;
		//Return value
		string _l_rv;
		//Gather references to requried things
		//If the test has not passed, print the error.
		if (_m_print_successes || not _a_aetr.passed())
		{
			//Get references to some things used a few times.
			const fmt::text_style& _l_failure_ts{ _a_test_options._m_failure_text_style };
			const fmt::text_style& _l_pass_ts{ _a_test_options._m_pass_text_style };
			const fmt::text_style& _l_highlighted_info{ _a_test_options._m_highlighted_info_style };
			//Define information break-line.
			const size_t _l_line_len{ _a_test_options._m_console_line_length };
			const char _l_pretty_char{ _a_test_options._m_separator_chars[0] };
			const string _l_line_break{
				fmt::format("{0}\n",
					string(
				_l_line_len, _l_pretty_char)) };
			//Define the info lines
			string _l_info_lines;
			const std::size_t _l_info_line_padding { get_largest_info_identifier(_a_aetr) };
			for (const pair<enum_info_data_to_print_t,bool> _l_eidto : _m_data_to_print_collection)
			{
				if (_l_eidto.second || check_if_info_data_to_be_included(_l_eidto.first, _a_aetr))
				{
					const string _l_identifier{ get_info_data_enum_text_data(_l_eidto.first) };
					const string _l_padding{ string(_l_info_line_padding - _l_identifier.size(),' ')};
					_l_info_lines.append(fmt::format("{0}: {1}{2}\n",
						_l_identifier,
						_l_padding,
						get_info_data_from_test_report(_l_eidto.first, _a_aetr, _a_test_options)));
				}
			}
			//Print the output
			string _l_rv{ fmt::format("{0}{1}{0}"
				,_l_line_break
				,_l_info_lines) };
			if (_a_aetr.test_warnings_recieved() > 0)
			{
				string _l_warning_str;
				//for (const std::shared_ptr<mid_execution_test_warning_t>& _l_warning : _a_aetr.warnings())
				//{
				//	_l_warning_str.append(fmt::format("{0}\n", format_warning(_l_warning)));
				//}
				_l_rv.append(fmt::format("{1}{0}", _l_line_break, _l_warning_str));
			}
			if (_a_aetr.test_reports_recieved() > 0)
			{
				string _l_report_str;
				size_t _l_idx{ 1 };
				size_t _l_maximum_idx_str_size{ to_string(_a_aetr.reports().size()).size()};
				for (const std::shared_ptr<mid_execution_test_report_t>& _l_report : _a_aetr.reports())
				{
					_l_report_str.append(fmt::format("{0}", format_report(_l_report,&_a_test_options, _l_idx, _l_maximum_idx_str_size)));
					++_l_idx;
				}
				_l_rv.append(fmt::format("{0}{1}", _l_report_str, _l_line_break));
			}
			write(_l_rv);
		}
	}
	__constexpr
		std::size_t
		text_test_reporter_t::get_largest_info_identifier(
			const reporters::after_execution_test_report_t& _a_aetr
		) const noexcept
	{
		using namespace std;
		size_t _l_largest_identifier{ 0 };
		for (const pair<enum_info_data_to_print_t, bool> _l_eidto : _m_data_to_print_collection)
		{
			if (_l_eidto.second || check_if_info_data_to_be_included(_l_eidto.first, _a_aetr))
			{
				const string _l_identifier{ get_info_data_enum_text_data(_l_eidto.first) };
				_l_largest_identifier = std::max(_l_largest_identifier,
					_l_identifier.size());
			}
		}
		return _l_largest_identifier;
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::get_info_data_from_test_report(
			const enum_info_data_to_print_t _a_enum_info_data,
			const reporters::after_execution_test_report_t& _a_aetr,
			const test_options_t& _a_test_options
		) const
	{
		using enum enum_info_data_to_print_t;
		switch (_a_enum_info_data)
		{
		case TITLE:
			return fmt::format("{0}",
				style_string(_a_aetr.passed() ? _c_test_result_str[0] : _c_test_result_str[1],
					_a_aetr.passed() ? _a_test_options._m_pass_text_style : _a_test_options._m_failure_text_style));
			break;
		case NAME:
			return fmt::format("{0}",
				style_string(_a_aetr.name().has_value() ? _a_aetr.name().value() : _c_unknown,
					_a_test_options._m_highlighted_info_style));
		case LOCATION:
			return fmt::format("{0}",
				style_string(
					_a_aetr.location().has_value() ? 
					utility::str::location_string(_a_aetr.location().value()) : _c_unknown,
					_a_test_options._m_highlighted_info_style));
		case TEST_PATH:
			return fmt::format("{0}",
				style_string(_a_aetr.test_path(),
					_a_test_options._m_highlighted_info_style));
		case SEED_USED:
			return fmt::format("{0}",
				style_string(
					_a_aetr.seed_used() == "" ? "None" :
					_a_aetr.seed_used(), _a_test_options._m_slight_highlight_style));
		case SEED_USED_HEX:
			return fmt::format("{0}",
				style_string(fmt::format("\"{0}\"",_a_aetr.seed_used_in_hex()), _a_test_options._m_slight_highlight_style));
		case SEED_TO_USE_TO_RE_RUN_TEST:
			return fmt::format("{0}",
				style_string(_a_aetr.seed_to_use_to_re_run_text(), _a_test_options._m_slight_highlight_style));
		case SEED_TO_USE_TO_RE_RUN_TEST_HEX:
			return fmt::format("{0}",
				style_string(fmt::format("\"{0}\"", _a_aetr.seed_to_use_to_re_run_text_in_hex()), _a_test_options._m_slight_highlight_style));
		case TOTAL_ASSERTIONS_RAN:
			return fmt::format("{0}", _a_aetr.test_reports_recieved());
		case TOTAL_ASSERTIONS_PASSED:
			return fmt::format("{0}", 
				style_string(std::to_string(_a_aetr.test_reports_passed()),
					_a_test_options._m_pass_text_style));
		case TOTAL_ASSERTIONS_FAILED:
			return fmt::format("{0}",
				style_string(std::to_string(_a_aetr.test_reports_failed()),
					_a_test_options._m_failure_text_style));
		case TOTAL_WARNINGS:
			return fmt::format("{0}",
				style_string(std::to_string(_a_aetr.test_warnings_recieved()),
					_a_aetr.test_warnings_recieved() == 0 ?
					_a_test_options._m_pass_text_style :
				_a_test_options._m_failure_text_style));
		case TERMINATED_EARLY:
			return fmt::format("{0}",
				style_string(_a_aetr.terminated_early() ? _c_terminated_early[0] :
					_c_terminated_early[1],
					_a_aetr.terminated_early() ?
					_a_test_options._m_failure_text_style :
					_a_test_options._m_pass_text_style));
		case CONDENSED_ASSERTIONS_PASSED_INFO:
			return fmt::format("{0}",
				_a_aetr.test_reports_recieved() == _a_aetr.test_reports_passed() ?
				style_string(
					fmt::format("{0} / {1}, {2}",
						_a_aetr.test_reports_passed(), _a_aetr.test_reports_recieved(),
						_c_condensed_assertions_passed)
					, _a_test_options._m_pass_text_style) :
				style_string(
					fmt::format("{0} / {1}, ({2} tests failed)",
						_a_aetr.test_reports_passed(), _a_aetr.test_reports_recieved(),
						_a_aetr.test_reports_failed())
					, _a_test_options._m_failure_text_style));
		default:
			throw errors::unaccounted_for_enum_exception(_a_enum_info_data);
		}
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::style_string(
			const std::string_view _a_str,
			const fmt::text_style& _a_style
		) const noexcept
	{
		return this->_m_file_name.has_value() ?
			fmt::format("{0}", _a_str) :
			fmt::format("{0}", fmt::styled(_a_str, _a_style));
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::format_warning(
			const std::shared_ptr<reporters::mid_execution_test_warning_t>& _a_warning
		) const noexcept
	{
		return "warning!";
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::format_report(
			const std::shared_ptr<reporters::mid_execution_test_report_t>& _a_report,
			const test_options_t* _a_test_options,
			const std::size_t _a_idx,
			const std::size_t _a_maximum_idx_str_size
		) const noexcept
	{
		using namespace reporters;
		mid_execution_test_report_t* _l_ptr{ _a_report.get() };
		if (_l_ptr == nullptr)
		{
			return
				style_string(fmt::format("Unable to read test. {0} element "
					"contains a nullptr. ", typeid(_a_report)),
					_a_test_options->_m_failure_text_style);
		}
		manual_failure_t* _l_mf{ dynamic_cast<manual_failure_t*>(_l_ptr) };
		if (_l_mf != nullptr)
		{
			return format_manual_failure(*_l_mf, _a_idx, _a_maximum_idx_str_size,_a_test_options);
		}
		test_assertion_result_t* _l_tar{ dynamic_cast<test_assertion_result_t*>(_l_ptr) };
		if (_l_tar != nullptr)
		{
			return format_test_assertion_result(*_l_tar, _a_idx, _a_maximum_idx_str_size, _a_test_options);
		}
		unhandled_exception_t* _l_ue{ dynamic_cast<unhandled_exception_t*>(_l_ptr) };
		if (_l_ue != nullptr)
		{
			return format_unhandled_exception(*_l_ue);
		}
		unhandled_exception_not_derived_from_std_exception_t* _l_uendfse{
			dynamic_cast<unhandled_exception_not_derived_from_std_exception_t*>(_l_ptr) };
		if (_l_uendfse != nullptr)
		{
			return format_unhandled_exception_not_derived_from_std_exception(*_l_uendfse);
		}
		else
		{
			return _l_ptr->unformatted_string();
		}
	}
	__constexpr_imp
		void
		text_test_reporter_t::append_indented_line(
			std::string& _a_str,
			const string_table_t& _a_print_vector,
			const std::string_view _a_indented_str
		) const noexcept
	{
		using namespace std;
		_a_str.append(_a_indented_str);
		for (const vector<string>& _l_line : _a_print_vector)
		{
			for (const string& _l_str : _l_line)
			{
				_a_str.append(_l_str);
			}
			_a_str.append("\n");
		}
	}
	__no_constexpr_imp
		std::string
		text_test_reporter_t::format_manual_failure(
			const manual_failure_t& _a_mf,
			const std::size_t _a_idx,
			const std::size_t _a_maximum_idx_str_size,
			const test_options_t* _a_test_options
		) const noexcept
	{
		using namespace std;
		string _l_rv;
		std::size_t _l_additional = 3;
		const string _l_idx_as_str{ to_string(_a_idx) };
		const string _l_id_identifier_cell{ fmt::format("{0}){1}{2}",
			_l_idx_as_str,
			string(_a_maximum_idx_str_size - _l_idx_as_str.size(), ' '),
			string(_l_additional, ' ')
		) };
		const string _l_other_cells(_l_id_identifier_cell.size(), ' ');
		size_t _l_data_idx{ 0 };
		for (const pair<combined_enum_manual_failure_t,bool> _l_eidto : _m_mid_exception_test_report_collection)
		{
			if (_l_eidto.second || check_if_manual_data_to_be_included(_l_eidto.first, _a_mf))
			{
				_l_rv.append(prepare_indexed_table(
					get_manual_data_as_table(
						_l_eidto.first, _a_mf, _a_test_options,
						_l_data_idx == 0 ? _l_id_identifier_cell : _l_other_cells
					)
				));
				++_l_data_idx;
			}
		}
		return _l_rv;
	}
	__constexpr
		bool
		text_test_reporter_t::check_if_execution_test_report_data_to_be_included(
			const enum_test_report_data_t _a_etrd,
			const mid_execution_test_report_t* _a_metr
		) const
	{
		using enum enum_test_report_data_t;
		if (_a_metr == nullptr)
		{
			throw errors::unaccounted_for_nullptr(_a_metr);
		}
		switch (_a_etrd)
		{
		case PASSED:
		case SOURCE_LOCATION:
		case EARLY_TERMINATION:
			return true;
		case LOG_INFOS:
			return (_a_metr->log_infos().size() > 0);
		default:
			throw errors::unaccounted_for_enum_exception(_a_etrd);
		}
	}
	__constexpr_imp
		bool
		text_test_reporter_t::check_if_manual_data_to_be_included(
			const combined_enum_manual_failure_t _a_cemf,
			const manual_failure_t& _a_mf
		) const
	{
		using namespace std;
		if (holds_alternative< enum_test_report_data_t>(_a_cemf))
		{
			enum_test_report_data_t _l_enum{ get< enum_test_report_data_t>(_a_cemf) };
			const mid_execution_test_report_t* _l_base_class{ static_cast<const mid_execution_test_report_t*>(&_a_mf) };
			return check_if_execution_test_report_data_to_be_included(_l_enum, _l_base_class);
		}
		else if (holds_alternative< enum_manual_failure_t>(_a_cemf))
		{
			using enum enum_manual_failure_t;
			enum_manual_failure_t _l_emf{ get< enum_manual_failure_t>(_a_cemf) };
			switch (_l_emf)
			{
			case SOURCE_CODE_REPRESENTATION:
			case STR_REPRESENTATION:
				return true;
			default:
				throw errors::unaccounted_for_enum_exception(_l_emf);
			}
		}
		else
		{
			throw errors::unaccounted_for_variant_exception(_a_cemf);
		}
	}
	__constexpr
		bool
		text_test_reporter_t::check_if_test_assertion_data_to_be_included(
			const combined_enum_test_assertion_t _a_ceta,
			const test_assertion_result_t& _a_tar
		) const
	{
		using namespace std;
		if (holds_alternative< enum_test_report_data_t>(_a_ceta))
		{
			enum_test_report_data_t _l_enum{ get< enum_test_report_data_t>(_a_ceta) };
			const mid_execution_test_report_t* _l_base_class{ static_cast<const mid_execution_test_report_t*>(&_a_tar) };
			return check_if_execution_test_report_data_to_be_included(_l_enum, _l_base_class);
		}
		else if (holds_alternative< enum_test_assertion_t>(_a_ceta))
		{
			using enum enum_test_assertion_t;
			enum_test_assertion_t _l_emf{ get< enum_test_assertion_t>(_a_ceta) };
			switch (_l_emf)
			{
			case SOURCE_CODE_REPRESENTATION:
			case STR_REPRESENTATION:
			case MATCHER_STRING:
				return true;
			case MATCHER_SOURCE_MAP:
				return _a_tar.matcher_source_map().size() > 0;
			default:
				throw errors::unaccounted_for_enum_exception(_l_emf);
			}
		}
		else
		{
			throw errors::unaccounted_for_variant_exception(_a_ceta);
		}
	}
	__constexpr 
		std::string 
		text_test_reporter_t::prepare_indexed_table(
			const string_table_t& _a_str_table
		) const noexcept
	{
		using namespace std;
		string _l_rv{};
		for (auto& _l_row : _a_str_table)
		{
			for (auto& _l_table_element : _l_row)
			{
				_l_rv.append(_l_table_element);
			}
			_l_rv.append("\n");
		}
		return _l_rv;
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::indent(
			const std::string_view _a_str
		) const noexcept
	{
		using namespace std;
		return (string(_m_indent, ' ') + string(_a_str));
	}
	__constexpr_imp
		void
		text_test_reporter_t::get_execution_test_report_data_as_table(
			string_table_t& _a_str_table,
			const enum_test_report_data_t _a_etrd,
			const mid_execution_test_report_t& _a_metr,
			const test_options_t* _a_test_options,
			const std::string_view _a_other_initial_cells
		) const
	{
		using namespace std;
		using enum enum_test_report_data_t;
		switch (_a_etrd)
		{
		case PASSED:
			_a_str_table[0].push_back("Passed: ");
			_a_str_table[0].push_back(
				style_string(_a_metr.passed() ? "Yes" : "No",
					_a_metr.passed() ? _a_test_options->_m_pass_text_style :
					_a_test_options->_m_failure_text_style));
			break;
		case SOURCE_LOCATION:
			_a_str_table[0].push_back("Location: ");
			_a_str_table.push_back(std::vector<std::string>());
			_a_str_table[1].push_back(string(_a_other_initial_cells));
			_a_str_table[1].push_back(
				indent(style_string(utility::str::location_string(_a_metr.source_location()),
					_a_test_options->_m_highlighted_info_style)));
			break;
		case EARLY_TERMINATION:
			_a_str_table[0].push_back("Early Termination: ");
			_a_str_table[0].push_back(
				style_string(_a_metr.early_termination() ? "Yes" : "No",
					_a_metr.early_termination() ? _a_test_options->_m_pass_text_style :
					_a_test_options->_m_failure_text_style));
			break;
		case LOG_INFOS:
			_a_str_table[0].push_back("Associated Log Messages:");
			for (const std::string& _l_element : _a_metr.log_infos())
			{
				_a_str_table.push_back(std::vector<std::string>());
				_a_str_table.back().push_back(string(_a_other_initial_cells));
				_a_str_table.back().push_back(indent(style_string(_l_element,
					_a_test_options->_m_slight_highlight_style)));
			}
			break;
		default:
			throw errors::unaccounted_for_enum_exception(_a_etrd);
		}
	}
	__constexpr_imp
		string_table_t
		text_test_reporter_t::get_manual_data_as_table(
			const combined_enum_manual_failure_t _a_cemf,
			const manual_failure_t& _a_mf,
			const test_options_t* _a_test_options,
			const std::string_view _a_other_initial_cells
		) const
	{
		size_t _l_additional = 2;
		using namespace std;
		string_table_t _l_st(1,std::vector<std::string>());
		_l_st[0].push_back(string(_a_other_initial_cells));
		if (holds_alternative< enum_test_report_data_t>(_a_cemf))
		{
			enum_test_report_data_t _l_enum{ get< enum_test_report_data_t>(_a_cemf) };
			const mid_execution_test_report_t* _l_base_class{ static_cast<const mid_execution_test_report_t*>(&_a_mf) };
			if (_l_base_class == nullptr)
			{
				
			}
			else
			{
				get_execution_test_report_data_as_table(_l_st, _l_enum, *_l_base_class, _a_test_options, _a_other_initial_cells);
			}
		}
		else if (holds_alternative< enum_manual_failure_t>(_a_cemf))
		{
			using enum enum_manual_failure_t;
			enum_manual_failure_t _l_emf{ get< enum_manual_failure_t>(_a_cemf) };
			switch (_l_emf)
			{
			case STR_REPRESENTATION:
				_l_st[0].push_back(
					_a_mf.passed() && _a_mf.early_termination() ?
					style_string("Manual test passed, function terminated early.",
						_a_test_options->_m_pass_text_style) :
					(_a_mf.passed() && not _a_mf.early_termination()) ?
					style_string("Manual test passed.",
						_a_test_options->_m_pass_text_style) :
					(not _a_mf.passed() && _a_mf.early_termination()) ?
					style_string("Manual test failed, function terminated early.",
						_a_test_options->_m_failure_text_style) :
					style_string("Manual test failed.",
						_a_test_options->_m_failure_text_style));
				break;
			case SOURCE_CODE_REPRESENTATION:
				_l_st[0].push_back("Source:");
				_l_st.push_back(std::vector<std::string>());
				_l_st[1].push_back(string(_a_other_initial_cells));
				_l_st[1].push_back(
					indent(style_string(_a_mf.source_code_representation(),
						_a_test_options->_m_slight_highlight_style)));
				break;
			default:
				throw errors::unaccounted_for_enum_exception(_l_emf);
			}
		}
		else
		{
			throw errors::unaccounted_for_variant_exception(_a_cemf);
		}
		return _l_st;
	}
	__constexpr_imp
		string_table_t
		text_test_reporter_t::get_test_assertion_data_as_table(
			const combined_enum_test_assertion_t _a_cemf,
			const test_assertion_result_t& _a_mf,
			const test_options_t* _a_test_options,
			const std::string_view _a_other_initial_cells
		) const
	{
		size_t _l_additional = 2;
		using namespace std;
		string_table_t _l_st(1, std::vector<std::string>());
		_l_st[0].push_back(string(_a_other_initial_cells));
		if (holds_alternative< enum_test_report_data_t>(_a_cemf))
		{
			enum_test_report_data_t _l_enum{ get< enum_test_report_data_t>(_a_cemf) };
			const mid_execution_test_report_t* _l_base_class{ static_cast<const mid_execution_test_report_t*>(&_a_mf) };
			if (_l_base_class == nullptr)
			{

			}
			else
			{
				get_execution_test_report_data_as_table(_l_st, _l_enum, *_l_base_class, _a_test_options, _a_other_initial_cells);
			}
		}
		else if (holds_alternative< enum_test_assertion_t>(_a_cemf))
		{
			/*
	MATCHER_STRING
			*/
			using enum enum_test_assertion_t;
			enum_test_assertion_t _l_emf{ get< enum_test_assertion_t>(_a_cemf) };
			switch (_l_emf)
			{
			case STR_REPRESENTATION:
				_l_st[0].push_back(
					_a_mf.passed() && _a_mf.early_termination() ?
					style_string("Assertion passed, function terminated early.",
						_a_test_options->_m_pass_text_style) :
					(_a_mf.passed() && not _a_mf.early_termination()) ?
					style_string("Assertion passed.",
						_a_test_options->_m_pass_text_style) :
					(not _a_mf.passed() && _a_mf.early_termination()) ?
					style_string("Assertion failed, function terminated early.",
						_a_test_options->_m_failure_text_style) :
					style_string("Assertion failed.",
						_a_test_options->_m_failure_text_style));
				break;
			case SOURCE_CODE_REPRESENTATION:
				_l_st[0].push_back("Source:");
				_l_st.push_back(std::vector<std::string>());
				_l_st[1].push_back(string(_a_other_initial_cells));
				_l_st[1].push_back(
					indent(style_string(_a_mf.source_code_representation(),
						_a_test_options->_m_slight_highlight_style)));
				break;
			case MATCHER_SOURCE_MAP:
				_l_st[0].push_back("Additional relevant sources:");
				for (const pair<string, vector<string>>& _l_m : _a_mf.matcher_source_map().map())
				{
					_l_st.push_back(std::vector<std::string>());
					_l_st.back().push_back(string(_a_other_initial_cells));
					_l_st.back().push_back(indent("Location:"));
					_l_st.back().push_back(indent(indent(style_string(_l_m.first, _a_test_options->_m_highlighted_info_style))));
					if (_l_m.second.size() > 0)
					{
						_l_st.push_back(std::vector<std::string>());
						_l_st.back().push_back(string(_a_other_initial_cells));
						_l_st.back().push_back(indent("Sources:"));
						for (const string& _a_str : _l_m.second)
						{
							_l_st.back().push_back(indent(indent(style_string(_a_str, _a_test_options->_m_slight_highlight_style))));
						}
					}
				}
				break;
			case MATCHER_STRING:
				_l_st.back().push_back("Matcher's output:");
				if (_a_mf.matcher_str().has_value())
				{
					_l_st.push_back(std::vector<std::string>());
					_l_st.back().push_back(string(_a_other_initial_cells));
					_l_st.back().push_back(style_string(_a_mf.matcher_str().has_value() ? _a_mf.matcher_str().value() : "<no output>",
						_a_test_options->_m_slight_highlight_style));
				}
				break;
			default:
				throw errors::unaccounted_for_enum_exception(_l_emf);
			}
		}
		else
		{
			throw errors::unaccounted_for_variant_exception(_a_cemf);
		}
		return _l_st;
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::format_test_assertion_result(
			const test_assertion_result_t& _a_mf,
			const std::size_t _a_idx,
			const std::size_t _a_maximum_idx_str_size,
			const test_options_t* _a_test_options
		) const noexcept
	{
		using namespace std;
		string _l_rv;
		std::size_t _l_additional = 3;
		const string _l_idx_as_str{ to_string(_a_idx) };
		const string _l_id_identifier_cell{ fmt::format("{0}){1}{2}",
			_l_idx_as_str,
			string(_a_maximum_idx_str_size - _l_idx_as_str.size(), ' '),
			string(_l_additional, ' ')
		) };
		const string _l_other_cells(_l_id_identifier_cell.size(), ' ');
		size_t _l_data_idx{ 0 };
		for (const pair<combined_enum_test_assertion_t, bool> _l_eidto : _m_test_assertion_collection)
		{
			if (_l_eidto.second || check_if_test_assertion_data_to_be_included(_l_eidto.first, _a_mf))
			{
				_l_rv.append(prepare_indexed_table(
					get_test_assertion_data_as_table(
						_l_eidto.first, _a_mf, _a_test_options,
						_l_data_idx == 0 ? _l_id_identifier_cell : _l_other_cells
					)
				));
				++_l_data_idx;
			}
		}
		return _l_rv;
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::format_unhandled_exception(
			const unhandled_exception_t& _a_mf
		) const noexcept
	{
		return "";
	}
	__constexpr_imp
		std::string
		text_test_reporter_t::format_unhandled_exception_not_derived_from_std_exception(
			const unhandled_exception_not_derived_from_std_exception_t& _a_mf
		) const noexcept
	{
		return "";
	}
	namespace
	{
		__constexpr_imp
			const char*
			get_info_data_enum_text_data(
				const enum_info_data_to_print_t _a_enum_info_data
			)
		{
			using enum enum_info_data_to_print_t;
			switch (_a_enum_info_data)
			{
			case TITLE:
				return "TEST_INFO";
			case NAME:
				return "Test Name";
			case LOCATION:
				return "Location";
			case TEST_PATH:
				return "Test Path";
			case SEED_USED:
				return "Seed Used";
			case SEED_USED_HEX:
				return "Seed Used (hex)";
			case SEED_TO_USE_TO_RE_RUN_TEST:
				return "Seed to re-run test";
			case SEED_TO_USE_TO_RE_RUN_TEST_HEX:
				return "Seed to re-run test (hex)";
			case TOTAL_ASSERTIONS_RAN:
				return "Total Assertions Ran";
			case TOTAL_ASSERTIONS_PASSED:
				return "Total Assertions Passed";
			case TOTAL_ASSERTIONS_FAILED:
				return "Total Assertions Failed";
			case CONDENSED_ASSERTIONS_PASSED_INFO:
				return "Assertions";
			case TOTAL_WARNINGS:
				return "Total Warnings Issued";
			case TERMINATED_EARLY:
				return "Terminated Early";
			default:
				throw errors::unaccounted_for_enum_exception(_a_enum_info_data);
			}
		}
		__constexpr_imp
			bool
			check_if_info_data_to_be_included(
				const enum_info_data_to_print_t _a_enum_info_data,
				const reporters::after_execution_test_report_t& _a_aetr
			)
		{
			using enum enum_info_data_to_print_t;
			switch (_a_enum_info_data)
			{
			case TITLE:
			case NAME:
			case LOCATION:
			case TEST_PATH:
			case TOTAL_ASSERTIONS_RAN:
			case TOTAL_ASSERTIONS_PASSED:
			case TOTAL_ASSERTIONS_FAILED:
			case CONDENSED_ASSERTIONS_PASSED_INFO:
				return true;
			case SEED_USED:
				return _a_aetr.seed_used() != "";
			case SEED_USED_HEX:
				return _a_aetr.seed_used() != "";
			case SEED_TO_USE_TO_RE_RUN_TEST:
				return _a_aetr.seed_used() == "";
			case SEED_TO_USE_TO_RE_RUN_TEST_HEX:
				return _a_aetr.seed_used() == "";
			case TOTAL_WARNINGS:
				return _a_aetr.test_warnings_recieved() > 0;
			case TERMINATED_EARLY:
				return _a_aetr.terminated_early();
			default:
				throw errors::unaccounted_for_enum_exception(_a_enum_info_data);
			}
		}
		__constexpr_imp
			std::string
			normalise_line_str_rep(
				const std::string_view _a_str
			) noexcept
		{
			using namespace std;
			//Put the string view into a string
			string _l_rv{ _a_str };
			//Find the position of the first prefix
			while (true)
			{
				size_t _l_pos{ _l_rv.find(_c_normalised_line_str_prefix_1) };
				//If the prefix exists, we have to remove it...
				if (_l_pos != std::string::npos)
				{
					//Erase the first part of the prefix.
					_l_rv.erase(_l_pos, std::strlen(_c_normalised_line_str_prefix_1));
					//If the next part is a ")", there is no string to replace it with
					if (_l_rv[_l_pos] == _c_normalised_line_str_prefix_2[0])
					{
						//Remove the second prefix.
						const size_t _l_pos_second{ _l_rv.find(_c_normalised_line_str_prefix_2) };
						_l_rv.erase(_l_pos_second, std::strlen(_c_normalised_line_str_prefix_2));
					}
					//Else there is a string to replace it with
					else if (_l_rv[_l_pos] == _c_quote_char)
					{
						//Find the position of the first quote
						const size_t _l_quotation_pos_one{ _l_rv.find(_c_quote_char,_l_pos) };
						//Remove it.
						_l_rv.erase(_l_quotation_pos_one, 1);
						//Find the second quote, remove it
						const size_t _l_quotation_pos_two{ _l_rv.find(_c_quote_char,_l_quotation_pos_one) };
						_l_rv.erase(_l_quotation_pos_two, 1);
						//Insert brackets after the string contained in the quotes
						_l_rv.insert(_l_quotation_pos_two, _c_lbracket);
						//Finally erase the second prefix.
						const size_t _l_pos_second{ _l_rv.find(_c_normalised_line_str_prefix_2) };
						_l_rv.erase(_l_pos_second, std::strlen(_c_normalised_line_str_prefix_2));
						//Insert a bracket
					//	_l_rv.insert(_l_rv.size(), _c_rbracket);
					}
				}
				else
				{
					break;
				}
			}
			return _l_rv;
		}
	}
_END_ABC_REPORTERS_NS