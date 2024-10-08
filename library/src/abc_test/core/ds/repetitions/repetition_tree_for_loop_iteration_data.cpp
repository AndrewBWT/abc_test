#include "abc_test/core/ds/repetitions/repetition_tree_for_loop_iteration_data.h"
#include "abc_test/core/ds/repetitions/repetition_tree.h"

#include "abc_test/utility/internal/log.h"


_BEGIN_ABC_NS
namespace ds
{
	bool
		repetition_tree_for_loop_iteration_data_t::add_repetition(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) noexcept
	{
		if (_a_idx >= _a_rds.size())
		{
			return true;
		}
		else
		{
			const repetition_data_t& _l_element{ _a_rds[_a_idx] };
			for (repetition_tree_for_loop_index_data_t& _l_rfl : _m_for_loops)
			{
				if (_l_element.for_loop_index == _l_rfl.for_loop_index())
				{
					//Its added. Move on down.
					return _l_rfl.add_repetition(_a_rds, _a_idx);
				}
			}
			repetition_tree_for_loop_index_data_t _l_to_insert{ _l_element.for_loop_index };
			auto _l_x = upper_bound(_m_for_loops.begin(), _m_for_loops.end(), _l_to_insert,
				[](const repetition_tree_for_loop_index_data_t& _a_left, const repetition_tree_for_loop_index_data_t& _a_right) {return _a_left.for_loop_index() < _a_right.for_loop_index(); });
			auto _l_itt = _m_for_loops.insert(
				_l_x,
				_l_to_insert
			);
			return _l_itt->add_repetition(_a_rds, _a_idx);
		}
	}

	std::optional<repetition_data_t>
		repetition_tree_for_loop_iteration_data_t::find_next_for_loop(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept
	{
		using enum utility::internal::internal_log_enum_t;
		if (_a_idx >= _a_rds.size())
		{
			const repetition_tree_for_loop_index_data_t& _l_rfl = _m_for_loops[0];
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_a_idx ({0}) >= _a_rds.size() ({1}), "
				"Moving down on = {2}", _a_idx, _a_rds.size(), _l_rfl));
			std::optional<repetition_data_t> _l_rv{ _l_rfl.find_next_for_loop(_a_rds, _a_idx) };
			if (_l_rv.has_value())
			{
				auto _l_rv_val{ _l_rv.value() };
				repetition_data_t _l_rrv(_m_for_loops[0].for_loop_index(),
					{ _l_rv_val.for_loop_index,
					_l_rv_val.for_loop_iteration_data.mode,
					_l_rv_val.for_loop_iteration_data.additional_data });
				_LIBRARY_LOG(REPETITION_INFO, fmt::format(
					"Returning {0}", _l_rrv));
				return _l_rv;
			}
			else
			{
				return std::optional<repetition_data_t>{};
			}
		}
		else
		{
			for (const repetition_tree_for_loop_index_data_t& _l_rfl : _m_for_loops)
			{
				if (_l_rfl.for_loop_index() == _a_rds[_a_idx].for_loop_index)
				{
					//Its added. Move on down.
					_LIBRARY_LOG(REPETITION_INFO, fmt::format(
						"_l_for_loop_to_check = {0}, "
						"Found element with correct for loop index. _l_rlf ",
						_a_rds[_a_idx].for_loop_index()));
					return _l_rfl.find_next_for_loop(_a_rds, _a_idx);
				}
			}
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_l_for_loop_to_check = {0}, Couldn't find value.", _a_rds[_a_idx].for_loop_index()));
			return std::optional<repetition_data_t>{};
		}
	}
	std::optional<repetition_data_t>
		repetition_tree_for_loop_iteration_data_t::increment_last_index(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept
	{
		using enum utility::internal::internal_log_enum_t;
		if (_a_idx >= _a_rds.size())
		{
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_a_idx ({0}) >= _a_rds.size() ({1})", _a_idx, _a_rds.size()));
			return std::optional<repetition_data_t>{};
		}
		else
		{
			for (const repetition_tree_for_loop_index_data_t& _l_rfl : _m_for_loops)
			{
				if (_l_rfl.for_loop_index() == _a_rds[_a_idx].for_loop_index)
				{
					//Its added. Move on down.
					_LIBRARY_LOG(REPETITION_INFO, fmt::format(
						"_l_for_loop_to_check = {0}, "
						"Found element with correct for loop index.",
						_a_rds[_a_idx].for_loop_index()));
					return _l_rfl.increment_last_index(_a_rds, _a_idx);
				}
			}
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_l_for_loop_to_check = {0}, Couldn't find value.", _a_rds[_a_idx].for_loop_index()));
			return std::optional<repetition_data_t>{};
		}
	}
	__no_constexpr_or_inline_imp
		bool
		repetition_tree_for_loop_iteration_data_t::is_repetition_to_be_repeated(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept
	{
		using enum utility::internal::internal_log_enum_t;
		if (_a_idx >= _a_rds.size())
		{
			return false;
		}
		else
		{
			for (const repetition_tree_for_loop_index_data_t& _l_rfl : _m_for_loops)
			{
				if (_l_rfl.for_loop_index() == _a_rds[_a_idx].for_loop_index)
				{
					//Its added. Move on down.
					_LIBRARY_LOG(REPETITION_INFO, fmt::format(
						"_l_for_loop_to_check = {0}, "
						"Found element with correct for loop index.",
						_a_rds[_a_idx].for_loop_index()));
					return _l_rfl.is_repetition_to_be_repeated(_a_rds, _a_idx);
				}
			}
			return false;
		}
	}
	std::string
		repetition_tree_for_loop_iteration_data_t::print(
		) const noexcept
	{
		using namespace std;
		string _l_rv{ fmt::format("({0},{1},\"{2}\",[", for_loop_iteration_data.generation_collection_index,
			for_loop_iteration_data.mode,for_loop_iteration_data.additional_data) };
		for (size_t _l_idx{ 0 }; _l_idx < _m_for_loops.size(); ++_l_idx)
		{
			const repetition_tree_for_loop_index_data_t& _l_elem{ _m_for_loops[_l_idx] };
			_l_rv.append(_l_elem.print());
			_l_rv.append(_l_idx + 1 < _m_for_loops.size() ? "," : "");
		}
		_l_rv.append("])");
		return _l_rv;
	}
	std::expected<repetition_tree_for_loop_iteration_data_t, std::string>
		parse_repeition_for_loop_member(
			const std::string_view _a_str,
			const std::size_t _a_depth
		) noexcept
	{
		using namespace std;
		using namespace utility::str;
		using enum utility::internal::internal_log_enum_t;
		size_t _l_mode{ 0 };
		size_t _l_previous_mode;
		size_t _l_old_pos;
		size_t _l_current_pos{ 0 };
		size_t _l_start_pos;
		string _l_size_t_str;
		size_t _l_size_t = 0;
		size_t _l_depth;
		vector<string> _l_vect, _l_children;
		string _l_error_string;
		size_t _l_gen_data_mode{ numeric_limits<size_t>::max() };
		_LIBRARY_LOG(PARSING_SEED, fmt::format("Parsing repetition_tree_for_loop_iteration_data_t \"{0}\"",
			_a_str));
		while (_l_current_pos < _a_str.size())
		{
			_l_previous_mode = _l_mode;
			_l_old_pos = _l_current_pos;
			switch (_l_mode)
			{
			case 0:
				//Searching for opening bracket
				locate_relevant_character(_a_str, _c_l_round_bracket, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 1);
				_l_start_pos = _l_current_pos + 1;
				break;
			case 1:
				locate_relevant_character(_a_str, _c_comma, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 2);
				_l_vect.push_back(string(_a_str.substr(_l_start_pos, _l_current_pos - _l_start_pos)));
				_l_start_pos = _l_current_pos + 1;
				break;
			case 2:
				locate_relevant_character(_a_str, _c_comma, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 3);
				_l_vect.push_back(string(_a_str.substr(_l_start_pos, _l_current_pos - _l_start_pos)));
				break;
			case 3:
				locate_relevant_character(_a_str, _c_quote, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 4);
				_l_start_pos = _l_current_pos + 1;
				break;
			case 4:
				process_string(_a_str, _l_current_pos, _l_error_string, _l_mode, 5);
				_l_vect.push_back(string(_a_str.substr(_l_start_pos, _l_current_pos - _l_start_pos)));
				break;
			case 5:
				locate_relevant_character(_a_str, _c_comma, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 6);
				break;
			case 6:
				//Searching for square bracket stating the start of the vector of data.
				locate_relevant_character(_a_str, _c_l_square_bracket, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 7);
				break;
			case 7:
				//Looking for a new element in the vector of data.
				locate_relevant_character(_a_str, _c_l_round_bracket_or_r_square_bracket, _l_current_pos, _l_error_string);
				if (_l_current_pos != string::npos && _a_str[_l_current_pos] == _c_l_round_bracket_or_r_square_bracket[1])
				{
					_l_mode = numeric_limits<size_t>::max();
					break;
				}
				else
				{
					process_mode(_l_mode, _l_current_pos, 8);
					_l_start_pos = maybe_set_data(_l_current_pos);
					_l_depth = 1;
				}
				break;
			case 8:
				//Looking for either a start or end bracket with the correct depth, and also to escape strings
				process_list_elements(_a_str, _l_current_pos, _l_error_string, _l_depth, _l_children, _l_start_pos, _l_mode, 10, 9);
				break;
			case 9:
				process_end_of_list(_a_str, _l_current_pos, _l_error_string, _l_mode, 7);
				break;
			case 10:
				process_string(_a_str, _l_current_pos, _l_error_string, _l_mode, 8);
				break;
			default:
				_l_mode = numeric_limits<size_t>::max();
				_l_current_pos = _a_str.size();
			}
			if (not _l_error_string.empty())
			{
				return unexpected(_l_error_string);
			}
			_l_current_pos++;
		}
		if (_l_vect.size() != 3)
		{
			return unexpected(fmt::format(
				"Expected exactly three elemnets in tuple. Instead found {0}. Tuple of strings = {1}",
				_l_vect.size(), _l_vect));
		}
		else
		{
			auto res = std::from_chars(_l_vect[0].data(), _l_vect[0].data() + _l_vect[0].size(), _l_size_t);
			auto res2 = std::from_chars(_l_vect[1].data(), _l_vect[1].data() + _l_vect[1].size(), _l_gen_data_mode);
		}
		repetition_tree_for_loop_iteration_data_t _l_rflm(for_loop_iteration_data_t{ _l_size_t, _l_gen_data_mode, _l_vect[2] });
		for (auto& _l_c : _l_children)
		{
			expected< repetition_tree_for_loop_index_data_t,string> _l_opt{ parse_repetition_for_loop(_l_c, _a_depth+1) };
			if (_l_opt.has_value())
			{
				_l_rflm.add_repetition_for_loop(_l_opt.value());
			}
			else
			{
				return unexpected(_l_opt.error());
			}
		}
		_LIBRARY_LOG(PARSING_SEED, fmt::format("Parsed repetition_tree_for_loop_iteration_data_t {0}", _l_rflm.print()));
		return _l_rflm;
	}
}
_END_ABC_NS