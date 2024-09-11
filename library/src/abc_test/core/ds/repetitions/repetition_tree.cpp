#include "abc_test/core/ds/repetitions/repetition_tree.h"
#include "abc_test/core/test_options.h"

_BEGIN_ABC_NS
namespace ds
{
	__no_constexpr_or_inline_imp
		std::string
		repetition_tree_t::print_repetition_tree(
			const test_options_t& _a_options
		) const noexcept
	{
		using namespace std;
		string _l_rv{ print_repetition_tree() };
		vector<unsigned char> _l_chars;
		for (char _l_char : _l_rv)
		{
			_l_chars.push_back(static_cast<unsigned char>(_l_char));
		}
		_l_rv.clear();
		for (unsigned char _l_char : _l_chars)
		{
			_l_rv.append(fmt::format("{:x}", _l_char));
		}
		return _l_rv;
	}
	__no_constexpr_or_inline_imp
		std::expected<repetition_tree_t, std::string>
		parse_repetition_tree(
			const std::string_view _a_str,
			const test_options_t& _a_options,
			const std::size_t _a_depth
		) noexcept
	{
		using namespace utility::str;
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		std::string _l_str{};
		//Parse bytes
		const char* _l_position = _a_str.data();
		vector<unsigned char> _l_chars;
		for (size_t _l_idx{ 0 }; _l_idx < (_a_str.size() / 2); ++_l_idx)
		{
			unsigned char value;
			if (_l_idx * 2 + 1 >= _a_str.size())
			{
				return unexpected(
					fmt::format(
						"Expected pairs of bytes, there was no element at index {0} after element {1}",
						_l_idx * 2, *_l_position));
			}
			auto [_l_ptr, _l_ec] = from_chars(_l_position, _l_position + 2, value, 16);
			_l_position += 2;
			//If no error, pass the byte to the list of chars
			if (_l_ec == errc())
			{
				_l_chars.push_back(value);
			}
			else
			{
				//Else return failure
				return unexpected{
					fmt::format("Could not parse byte pair {0}{1}", *_l_position, *(_l_position+1))};
			}
		}
		//Add all the chars to the string.
		_l_str = string(_l_chars.begin(), _l_chars.end());
		_LIBRARY_LOG(PARSING_SEED, fmt::format("Depth {0}. Repetition tree after conversion from hex is in form \"{1}\"",
			_a_depth, _l_str));
		//Separete the string into strings...
		size_t _l_current_pos{ 0 };
		size_t _l_mode{ 0 };
		size_t _l_start{ 0 };
		vector<string> _l_strs;
		size_t _l_depth;
		size_t _l_found_pos;
		bool _l_end;
		char _l_char;
		size_t _l_previous_mode;
		string _l_error_string;
		size_t _l_old_pos;
		while (_l_current_pos < _l_str.size())
		{
			_l_previous_mode = _l_mode;
			_l_old_pos = _l_current_pos;
			switch (_l_mode)
			{
			case 0:
				//Searching for opening bracket
				locate_relevant_character(_l_str, _c_l_square_bracket, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 1);
				break;
			case 1:
				//Mode 1, searhicng for a round bracket to begin a tuple of data.
				locate_relevant_character(_l_str, _c_l_round_bracket, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 2);
				_l_start = _l_current_pos;
				_l_depth = 1;
				_l_start = _l_current_pos;
				break;
			case 2:
				process_list_elements(_l_str, _l_current_pos, _l_error_string, _l_depth, _l_strs, _l_start, _l_mode, 3, 4);
				//Check for end brackets, commas and begin next brackets.
				break;
			case 3:
				//Escaping quotes
				process_string(_l_str, _l_current_pos, _l_error_string, _l_mode, 2);
				break;
			case 4:
				//Searching for next comma or end square bracket (, or ])
				process_end_of_list(_l_str, _l_current_pos, _l_error_string, _l_mode, 1);
				break;
			}
			if (not _l_error_string.empty())
			{
				return unexpected(_l_error_string);
			}
			_l_current_pos++;
		}
		repetition_tree_t _l_rv;
		size_t _l_idx{ 1 };
		_LIBRARY_LOG(PARSING_SEED, fmt::format("Depth {0}. "
			"Elements identified = {1}",
			_a_depth,
			_l_strs));
		for (const string& _l_c : _l_strs)
		{
			_LIBRARY_LOG(PARSING_SEED, fmt::format("Depth {0}. "
				"Attempting to parse into repetition_tree_for_loop_index_data_t \"{1}\"",
				_a_depth,
				_l_c));
			const expected<repetition_tree_for_loop_index_data_t,string> _l_op{ parse_repetition_for_loop(_l_c, _a_depth+1) };
			if (_l_op.has_value())
			{
				_LIBRARY_LOG(PARSING_SEED, fmt::format("Depth {0}. "
					"Inserting repetition_tree_for_loop_index_data_t = {1}",
					_a_depth,
					_l_op.value().print()));
				_l_rv.add_repetition_for_loop(_l_op.value());
			}
			else
			{
				return unexpected(
					fmt::format("Error found in substring at index {0} (\"{1}\", "
						"error given = \"{2}\"",
						_l_idx, _l_c, _l_op.error()));
			}
			_l_idx++;
		}
		return _l_rv;
	}
}
_END_ABC_NS