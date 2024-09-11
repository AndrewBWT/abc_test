#pragma once

#include <iostream>
#include <stacktrace>
#include "fmt/color.h"
#include <array>
#include "abc_test/utility/io/threaded_text_output_reporter.h"

#include "abc_test/core/ds/test_data/invoked_test_data.h"

#include "abc_test/core/errors/test_failures_info.h"


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
		/*!
		Function called to report a test.
		*/
		__no_constexpr
			virtual
			void
			report_test(
				const ds::invoked_test_info_t& _a_iti,
				const errors::test_failures_info_t& _a_error_infos,
				const test_options_t& _a_test_options
			) noexcept;
	private:
		__constexpr
			std::string
			style_string(
				const std::string_view _a_str,
				const fmt::text_style& _a_style
			) const noexcept;
		__no_constexpr
			std::string
			print_error_info(
				const errors::test_failure_info_t& _a_error_info,
				const std::string_view _a_line_prefix,
				const size_t _a_line_prefix_size,
				const size_t _a_idx,
				const test_options_t& _a_test_options
			) noexcept;
	};

	namespace
	{
		/*!
		* Takes a string created by a CHECK_EXPR macro and ensures that it looks identical to what the
		* user wrote - as in, removing the placeholder dummy object.
		*/
		__constexpr
			std::string
			normalise_line_str_rep(
				const std::string_view _a_str
			) noexcept;

		__no_constexpr
			std::string
			location_string(
				const std::source_location& _a_sl
			) noexcept;
		__constexpr
			std::string
			create_padded_space(
				const size_t _a_largest_string,
				const std::string_view _a_string_to_help_pad
			) noexcept;
		__no_constexpr
			std::string
			make_aligned_line(
				const char* _a_identifier,
				const std::size_t _a_largest_identifer_size,
				const std::string_view _a_element_to_print
			) noexcept;
		//Const char*s
		const std::size_t _c_strs_identifiers_size{ 3 };
		const char* _c_strs_identifiers[_c_strs_identifiers_size] = { "Function","Location","Seed used" };
		const char* _c_test_failure_str{ "TEST_FAILURE" };
		const char* _c_alt_seed_str{ "To repeat test, use seed" };
		const char* _c_normalised_line_str_prefix_1{ "ab::test::cmp::placeholder(" };
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
	_END_ABC_NS
		_END_NS

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
			const ds::invoked_test_info_t& _a_iti,
			const errors::test_failures_info_t& _a_error_infos,
			const test_options_t& _a_test_options
		) noexcept
	{
		using namespace std;
		using namespace ds;
		using namespace errors;
		//Return value
		string _l_rv;
		//Gather references to requried things
		//If the test has not passed, print the error.
		if (not _a_error_infos.passed())
		{
			string _l_internal_errors_string{};
			const source_location _l_default_sl;
			const registered_test_data_t _l_default_rtd;
			const post_setup_test_data_t _l_default_pstd;
			const bool _l_has_pstd{ _a_iti.has_post_setup_test_data() };
			_l_internal_errors_string.append(_l_has_pstd ? "" :
				fmt::format(
					"{0}\n",
					_c_no_pstd
				));
			const post_setup_test_data_t& _l_pstd{ _l_has_pstd ? _a_iti.post_setup_test_data() : _l_default_pstd };
			const bool _l_has_rtd{ _l_has_pstd && _l_pstd.has_registered_test_data() };
			_l_internal_errors_string.append(_l_has_pstd ? "" :
				fmt::format(
					"{0}\n",
					_c_no_rtd
				));
			const registered_test_data_t& _l_rtd{ _l_has_rtd ? _l_pstd.registered_test_data() : _l_default_rtd };
			const bool _l_has_sl{ _l_has_rtd && _l_rtd._m_source_location.has_value() };
			_l_internal_errors_string.append(_l_has_sl ? "" :
				fmt::format(
					"{0}\n",
					_c_no_sl
				));
			const source_location& _l_sl{ _l_has_rtd ? _l_rtd._m_source_location.value() : _l_default_sl };
			const size_t _l_line_len{ _a_test_options._m_console_line_length };
			const char _l_pretty_char{ _a_test_options._m_separator_chars[0] };
			const fmt::text_style& _l_failure_ts{ _a_test_options._m_failure_text_style };
			const fmt::text_style& _l_highlighted_info{ _a_test_options._m_highlighted_info_style };
			//	const array<string, 3> _l_strs_identifiers{ "Function","Location","Seed used"};
			const size_t _l_largest_identifier_size{ std::strlen(*((std::max_element(_c_strs_identifiers, _c_strs_identifiers + _c_strs_identifiers_size, [](const auto& s1, const auto& s2) {
				return std::strlen(s1) < std::strlen(s2);
				})))) };
			const string _l_line_indent(_a_test_options._m_indent_size, ' ');
			//Define each line
			//Pretty printed line
			const string _l_line_zero(_l_line_len, _l_pretty_char);
			//Line saying the test has failed, together with the name of the function
			const string _l_line_one{
				fmt::format(
					"{0}",
					style_string(_c_test_failure_str, _l_failure_ts)) };
			const string _l_line_two{
				_l_internal_errors_string.size() > 0 ?
				fmt::format(
					"{0}{1}\n{2}",
					_l_line_indent,
					style_string(_c_internal_error_string, _l_failure_ts),
					style_string(_l_internal_errors_string, _l_failure_ts)
					) :
				""
			};
			//Line presenting function name of test
			const string _l_line_three{
				make_aligned_line(_c_strs_identifiers[0], _l_largest_identifier_size,
					_l_has_rtd ?
					style_string(_l_rtd._m_description, _l_highlighted_info) :
					style_string(_c_no_rtd_description, _l_failure_ts)) };
			//Line presenting location of beginning of test
			const string _l_line_four{
				make_aligned_line(_c_strs_identifiers[1], _l_largest_identifier_size,
					fmt::format("{0}",
						_l_has_sl ? style_string(location_string(_l_sl), _l_highlighted_info) :
						style_string(_c_no_sl, _l_failure_ts))) };
			//Line repreesnting seed data about test
			const string _l_line_five{
				_l_pstd.has_repetition_data() ?
				make_aligned_line(_c_strs_identifiers[2], _l_largest_identifier_size,
					_l_pstd.repetition_data().print_repetition_tree(_a_test_options))
				:
				fmt::format(
					"{0}: \"{1}\"",
					_c_alt_seed_str,
					_a_iti.repetition_tree().print_repetition_tree(_a_test_options))
			};
			//Put together the output
			_l_rv.append(fmt::format(
				"{0}\n"
				"{1}\n"
				"{2}" //doesn't have return as this line isn't awlays used.
				"{3}\n"
				"{4}\n"
				"{5}\n"
				"{0}\n",
				_l_line_zero,
				_l_line_one,
				_l_line_two,
				_l_line_three,
				_l_line_four,
				_l_line_five
			));
			//The index used for the errors themsevles
			size_t _l_idx{ 1 };
			//The largest numerical identifer of the failed test
			const size_t _l_prefix_spaces_size{ std::to_string(_a_error_infos.size()).size() };
			//A string which contains the size of the largest test, for pretty printing
			const string _l_line_prefix(_l_prefix_spaces_size + 2, ' ');
			//Go through each error and print it
			for (const test_failure_info_t& _l_error_info : _a_error_infos)
			{
				_l_rv.append(print_error_info(_l_error_info, _l_line_prefix, _l_prefix_spaces_size, _l_idx++, _a_test_options));
			}
			//Print the output
			write(_l_rv);
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

	__no_constexpr_imp
		std::string
		text_test_reporter_t::print_error_info(
			const errors::test_failure_info_t& _a_error_info,
			const std::string_view _a_line_prefix,
			const size_t _a_line_prefix_size,
			const size_t _a_idx,
			const test_options_t& _a_options
		) noexcept
	{
		using namespace std;
		using namespace errors;
		using enum test_failure_info_enum_t;
		const fmt::text_style& _l_slight_highlight_ts{ _a_options._m_slight_highlight_style };
		const fmt::text_style& _l_highlighted_info{ _a_options._m_highlighted_info_style };
		//Const strings used
		const string _l_str_idx{ to_string(_a_idx) };
		const string _l_normalised_line_str_rep{
			normalise_line_str_rep(_a_error_info.line_str_representation())
		};
		const string _l_line_indent(_a_options._m_indent_size, ' ');
		//First line containing description of error
		const string _l_error_str{
			(
				_a_error_info.test_failure_info_enum() == TEST_FAILURE ? _c_error_strs[0] :
				_a_error_info.test_failure_info_enum() == MANUAL_FAILURE ? _c_error_strs[1] :
				_c_error_strs[2]
			) + std::string(_a_error_info.early_termination() ? _c_error_str_terminated : "")
		};
		const string _l_prefix_str_for_use_with_number{ string((_a_line_prefix_size - _l_str_idx.size()) + 1, ' ') };
		const string _l_error_info_line_zero{
			fmt::format(
			"{0}){1}{2}",
			_a_idx,
			_l_prefix_str_for_use_with_number,
			_l_error_str)
		};
		//Second line. Contains location info
		const string _l_location_identifier{
			(
				_a_error_info.test_failure_info_enum() == TEST_FAILURE ||
				_a_error_info.test_failure_info_enum() == MANUAL_FAILURE
			) ?
				_c_location_strs[0] : _c_location_strs[1]
		};
		const string _l_error_info_line_one{
			fmt::format(
				"{0}{1}{2}",
				_a_line_prefix,
				_l_location_identifier,
				style_string(location_string(_a_error_info.source_location()), _l_highlighted_info)) };
		//Third line contains the expression information
		const string _l_error_info_line_two{
		(
			_a_error_info.test_failure_info_enum() == TEST_FAILURE ||
			_a_error_info.test_failure_info_enum() == MANUAL_FAILURE
		) ?
		fmt::format(
			"{0}{1}\n"
			"{0}{2}{3}",
			_a_line_prefix,
			_c_failed_expr_strs[0],
			_l_line_indent,
			style_string(_l_normalised_line_str_rep, _l_slight_highlight_ts))
		:
			(_a_error_info.test_failure_info_enum() == UNHANDLED_EXCEPTION) ?
			fmt::format("{0}{1}{2}",_a_line_prefix,_c_exception_strs[0],
				style_string(_a_error_info.type_id(), _l_highlighted_info)) :
			fmt::format(
				"{0}{1}",
				_a_line_prefix,
				_c_exception_strs[1]
			)
		};
		//Fourth line contains either matcher info, exception's what() info, or no info
		const string _l_error_info_line_three{
			 (_a_error_info.test_failure_info_enum() == TEST_FAILURE) ?
			 fmt::format(
				 "{0}{1}\n"
				 "{0}{2}{3}\n",
				 _l_line_indent,
				 _c_failed_expr_strs[1],
				_a_line_prefix,
				 style_string(_a_error_info.error_msg(), _l_slight_highlight_ts)) :
			(_a_error_info.test_failure_info_enum() == UNHANDLED_EXCEPTION) ?
			fmt::format(
				"{0}{1}\n"
				"{0}{2}{4}{3}{4}\n",
				_l_line_indent,
				_c_exception_strs[2],
				_a_line_prefix,
				style_string(_a_error_info.error_msg(), _l_slight_highlight_ts),
				style_string("\"", _l_slight_highlight_ts)) : "" };
		string _l_rv;
		_l_rv.append(fmt::format(
			"{0}\n"
			"{1}\n"
			"{2}\n"
			"{3}",
			_l_error_info_line_zero,
			_l_error_info_line_one,
			_l_error_info_line_two,
			_l_error_info_line_three
		));
		//Logging info line
		const string _l_log_description{
			(
				_a_error_info.test_failure_info_enum() == TEST_FAILURE ||
				_a_error_info.test_failure_info_enum() == MANUAL_FAILURE
			) ?
				_c_logged_strs[0] :
				_c_logged_strs[1]
		};
		if (_a_error_info.log_info().size() > 0)
		{
			_l_rv.append(fmt::format(
				"{0}{1}:\n",
				_a_line_prefix,
				_l_log_description
			));
		}
		//Show all the log info
		for (const string& _l_log_info : _a_error_info.log_info())
		{
			_l_rv.append(fmt::format(
				"{0}{1}{2}\n",
				_l_line_indent,
				_a_line_prefix,
				style_string(_l_log_info, _l_slight_highlight_ts)
			));
		}
		return _l_rv;
	}

	namespace
	{
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
					_l_rv.insert(_l_rv.size(), _c_rbracket);
				}
			}
			return _l_rv;
		}
		__no_constexpr_imp
			std::string
			location_string(
				const std::source_location& _a_sl
			) noexcept
		{
			using namespace std;
			//A formalised way of showing a location
			return string{ fmt::format(
				"{0}:{1}",
				_a_sl.file_name(),
				_a_sl.line()
			) };
		}
		//Pad a string based on a maximum size and the string which will be used currently
		__constexpr_imp
			std::string
			create_padded_space(
				const size_t _a_largest_string,
				const std::string_view _a_string_to_help_pad
			) noexcept
		{
			using namespace std;
			return string(_a_largest_string - _a_string_to_help_pad.size(), ' ');
		}
		__no_constexpr_imp
			std::string
			make_aligned_line(
				const char* _a_identifier,
				const std::size_t _a_largest_identifer_size,
				const std::string_view _a_element_to_print
			) noexcept
		{
			using namespace std;
			return string{
					fmt::format(
						"{0}: {1}{2}",
						_a_identifier,
						create_padded_space(_a_largest_identifer_size,_a_identifier),
						_a_element_to_print
					) };
		}
	}
	_END_ABC_NS
		_END_NS