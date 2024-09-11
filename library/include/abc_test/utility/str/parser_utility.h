#pragma once
#include <string>
#include <vector>
#include <fmt/base.h>

_BEGIN_ABC_UTILITY_STR_NS
	const char* const _c_quote_and_round_brackets{ "()\"" };
	const char* const _c_comma{ "," };
	const char* const _c_quote{ "\"" };

	const char* const _c_l_square_bracket{ "[" };
	const char* const _c_r_square_bracket{ "]" };
	const char* const _c_l_round_bracket("(");
	const char* const _c_l_round_bracket_or_r_square_bracket("(]");
	const char* const _c_r_round_bracket{ ")" };
	const char* const _c_end_or_comma{ ",]" };
	const char _c_backslash = '\\';
	constexpr
		void
		process_mode(
			std::size_t& _a_mode,
			const std::size_t _a_current_pos,
			const std::size_t _a_next_mode
		) noexcept;
	constexpr
		void
		locate_relevant_character(
			const std::string_view _a_str,
			const char* _l_c_string_to_locate,
			std::size_t& _a_current_pos_ref,
			std::string& _a_error_string
		) noexcept;
	constexpr
		std::size_t
		maybe_set_data(
			const std::size_t _a_current_pos,
			const std::size_t _a_offset = 0
		) noexcept;
	constexpr
		void
		process_string(
			const std::string_view _a_str,
			std::size_t& _a_current_pos,
			std::string& _a_error_string,
			std::size_t& _a_mode,
			const std::size_t _a_finish_string_mode
		) noexcept;
	constexpr
		void
		process_list_elements(
			const std::string_view _a_str,
			std::size_t& _a_current_pos,
			std::string& _a_error_string,
			std::size_t& _a_depth,
			std::vector<std::string>& _a_strs,
			const std::size_t _a_start_pos,
			std::size_t& _a_mode,
			const std::size_t _a_mode_for_escape_string,
			const std::size_t _a_mode_for_comma_or_end_list
		) noexcept
	{
		using namespace std;
		locate_relevant_character(_a_str, _c_quote_and_round_brackets, _a_current_pos, _a_error_string);
		_a_depth = (_a_current_pos == string::npos) ?
			_a_depth : (_a_str[_a_current_pos] == _c_quote_and_round_brackets[1] && _a_depth != 0) ? _a_depth - 1 :
			(_a_str[_a_current_pos] == _c_quote_and_round_brackets[0]) ? _a_depth + 1 : _a_depth;
		_a_mode =
			(_a_current_pos == string::npos) ? numeric_limits<size_t>::max() :
			(_a_str[_a_current_pos] == _c_quote_and_round_brackets[1] && _a_depth == 0) ? _a_mode_for_comma_or_end_list :
			(_a_str[_a_current_pos] == _c_quote_and_round_brackets[2]) ? _a_mode_for_escape_string : _a_mode;
		if (_a_depth == 0 && _a_str[_a_current_pos] == _c_quote_and_round_brackets[1])
		{
			_a_strs.push_back(string(_a_str.substr(_a_start_pos, (_a_current_pos - _a_start_pos) + 1)));
		}
	}
	constexpr
		void
		process_end_of_list(
			const std::string_view _a_str,
			std::size_t& _a_current_pos,
			std::string& _a_error_string,
			std::size_t& _a_mode,
			const std::size_t _a_mode_if_comma_found
		) noexcept
	{
		using namespace std;
		locate_relevant_character(_a_str, _c_end_or_comma, _a_current_pos, _a_error_string);
		_a_mode =
			(_a_current_pos == string::npos || _a_str[_a_current_pos] == _c_end_or_comma[1]) ? numeric_limits<size_t>::max() :
			_a_mode_if_comma_found;
	}
_END_ABC_NS
_END_NS

_BEGIN_ABC_UTILITY_STR_NS
	constexpr
		void
		process_mode(
			std::size_t& _a_mode,
			const std::size_t _a_current_pos,
			const std::size_t _a_next_mode
		) noexcept
	{
		using namespace std;
		_a_mode = (_a_current_pos == string::npos) ? numeric_limits<size_t>::max() : _a_next_mode;
	}
	constexpr
		void
		locate_relevant_character(
			const std::string_view _a_str,
			const char* _l_c_string_to_locate,
			std::size_t& _a_current_pos_ref,
			std::string& _a_error_string
		) noexcept
	{
		using namespace std;
		const size_t _l_old_current_pos{ _a_current_pos_ref };
		_a_current_pos_ref = _a_str.find_first_of(_l_c_string_to_locate, _a_current_pos_ref);
		_a_error_string = (_a_current_pos_ref != string::npos) ? "" :
			fmt::format("Expecting one of \"{0}\", but unable to find in string \"{1}\". "
				"Searching from position {2} (substring \"{3}\")",
				_l_c_string_to_locate, _a_str, _l_old_current_pos, _a_str.substr(_l_old_current_pos));
	}
	constexpr
		std::size_t
		maybe_set_data(
			const std::size_t _a_current_pos,
			const std::size_t _a_offset
		) noexcept
	{
		using namespace std;
		return (_a_current_pos == string::npos) ? 0 : (_a_current_pos + _a_offset);
	}
	constexpr
		void
		process_string(
			const std::string_view _a_str,
			std::size_t& _a_current_pos,
			std::string& _a_error_string,
			std::size_t& _a_mode,
			const std::size_t _a_finish_string_mode
		) noexcept
	{
		using namespace std;
		locate_relevant_character(_a_str, _c_quote, _a_current_pos, _a_error_string);
		_a_mode =
			_a_current_pos == string::npos ? numeric_limits<size_t>::max() :
			(_a_str[_a_current_pos - 1] == _c_backslash) ? _a_mode : _a_finish_string_mode;
	}
_END_ABC_NS
_END_NS