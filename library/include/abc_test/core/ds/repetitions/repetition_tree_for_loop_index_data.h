#pragma once

#include "abc_test/core/ds/repetitions/repetition_tree_for_loop_iteration_data.h"
#include <charconv>
#include "abc_test/utility/str/parser_utility.h"


_BEGIN_ABC_DS_NS
	/*!
	* Object representing internal repetition tree (for repeating tests).
	* Specifically the for loop index, and all that for loop's values to generate the failed tests.
	*/
	class repetition_tree_for_loop_index_data_t
	{
	public:
		/*!
		* Default constructor
		*/
		__constexpr
			repetition_tree_for_loop_index_data_t(
			) noexcept;
		/*!
		* Initialises the object
		*/
		__constexpr
			repetition_tree_for_loop_index_data_t(
				const size_t _a_for_loop_index
			) noexcept;
		/*!
		* Adds an element to the internal vector.
		*/
		__constexpr
			void
			add_repetition_for_loop_member(
				const repetition_tree_for_loop_iteration_data_t& _a_rflm
			) noexcept;
		/*!
		* Gets the internal vector holding the children.
		*/
		__constexpr
			const std::vector<repetition_tree_for_loop_iteration_data_t>&
			get_children(
			) const noexcept;
		/*!
		* Gets the for loop index of the object.
		*/
		__constexpr
			size_t
			for_loop_index(
			) const noexcept;
		/*!
		* Adds a repetition to the data structure.
		*/
		__constexpr
			bool
			add_repetition(
				const repetition_data_sequence_t& _a_rds,
				const size_t _a_idx
			) noexcept;
		/*!
		* Finds the next required element the repeittion_data element, which describes
		* how to generate a repeated test.
		*/
		__constexpr
			std::optional<repetition_data_t>
			find_next_for_loop(
				const repetition_data_sequence_t& _a_rds,
				const size_t _a_idx
			) const noexcept;
		/*!
		* Increments the index
		*/
		__constexpr
			std::optional<repetition_data_t>
			increment_last_index(
				const repetition_data_sequence_t& _a_rds,
				const size_t _a_idx
			) const noexcept;
		/*!
		* Checks if the repetition is to be repeated.
		*/
		__constexpr
			bool
			is_repetition_to_be_repeated(
				const repetition_data_sequence_t& _a_rds,
				const size_t _a_idx
			) const noexcept;
		/*!
		* Prints out a repetition_Tree_for_loop_iteration_data element.
		*/
		__constexpr
			std::string
			print(
			) const noexcept;
	private:
		size_t _m_for_loop_index;
		std::vector<repetition_tree_for_loop_iteration_data_t> _m_for_loop_member;
	};
	/*!
	* Parses a repetition_tree_for_loop_index_data object, or returns a string describing the error.
	*/
	__constexpr
		std::expected<repetition_tree_for_loop_index_data_t, std::string>
		parse_repetition_for_loop(
			const std::string_view _a_str,
			const std::size_t _a_depth
		) noexcept;
	_END_ABC_DS_NS


template
<
>
struct fmt::formatter<abc::ds::repetition_tree_for_loop_index_data_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
		//Can't be constexpr due to use of fmt::format.
	__no_constexpr
		auto
		format(
			abc::ds::repetition_tree_for_loop_index_data_t _a_rt,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

	_BEGIN_ABC_DS_NS
	__constexpr_imp
		repetition_tree_for_loop_index_data_t::repetition_tree_for_loop_index_data_t(
		) noexcept
		: repetition_tree_for_loop_index_data_t(0)
	{

	}
	__constexpr_imp
		repetition_tree_for_loop_index_data_t::repetition_tree_for_loop_index_data_t(
			const size_t _a_for_loop_index
		) noexcept
		: _m_for_loop_index(_a_for_loop_index)
	{

	}
	__constexpr_imp
		void
		repetition_tree_for_loop_index_data_t::add_repetition_for_loop_member(
			const repetition_tree_for_loop_iteration_data_t& _a_rflm
		) noexcept
	{
		_m_for_loop_member.push_back(_a_rflm);
	}
	__constexpr_imp
		const std::vector<repetition_tree_for_loop_iteration_data_t>&
		repetition_tree_for_loop_index_data_t::get_children(
		) const noexcept
	{
		return _m_for_loop_member;
	}
	__constexpr_imp
		size_t
		repetition_tree_for_loop_index_data_t::for_loop_index(
		) const noexcept
	{
		return _m_for_loop_index;
	}
	__constexpr_imp
		bool
		repetition_tree_for_loop_index_data_t::add_repetition(
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
			for (repetition_tree_for_loop_iteration_data_t& _l_rfl : _m_for_loop_member)
			{
				if (_l_rfl.for_loop_iteration_data == _l_element.for_loop_iteration_data)
				{
					//Its added. Move on down.
					return _l_rfl.add_repetition(_a_rds, _a_idx + 1);
				}
			}
			repetition_tree_for_loop_iteration_data_t _l_to_insert(_l_element.for_loop_iteration_data);
			auto _l_x = upper_bound(_m_for_loop_member.begin(), _m_for_loop_member.end(), _l_to_insert,
				[](const auto& _a_lhs, const auto& _a_rhs) {return _a_lhs.for_loop_iteration_data < _a_rhs.for_loop_iteration_data; });// ,
			auto _l_itt = _m_for_loop_member.insert(
				_m_for_loop_member.end(),
				_l_to_insert
			);
			return _l_itt->add_repetition(_a_rds, _a_idx + 1);
		}
	}
	__constexpr_imp
		std::optional<repetition_data_t>
		repetition_tree_for_loop_index_data_t::find_next_for_loop(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept
	{
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		if (_a_idx >= _a_rds.size())
		{
			const repetition_tree_for_loop_iteration_data_t& _l_rfl{ _m_for_loop_member[0] };
			const repetition_data_t _l_rv(_a_idx, _l_rfl.for_loop_iteration_data);
			_LIBRARY_LOG(REPETITION_INFO, fmt::format("_a_idx ({0}) >= _a_rds.size() ({1}), "
				"returning {2}", _a_idx, _a_rds.size(), _l_rv));
			return _l_rv;
		}
		else
		{
			const repetition_data_t& _l_fst{ _a_rds[_a_idx] };
			for (const repetition_tree_for_loop_iteration_data_t& _l_rfl : _m_for_loop_member)
			{
				if (_l_rfl.for_loop_iteration_data == _l_fst.for_loop_iteration_data)
				{
					//Its added. Move on down.
					_LIBRARY_LOG(REPETITION_INFO, fmt::format(
						"_a_idx ({0}) < _a_rds.size() ({1}), "
						"_a_rds[_a_idx] = {2}, "
						"Found matching element", _a_idx, _a_rds.size(), _l_fst));
					return _l_rfl.find_next_for_loop(_a_rds, _a_idx + 1);
				}
			}
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_a_idx ({0}) < _a_rds.size() ({1}), "
				"_a_rds[_a_idx] = {2}, "
				"Unable to find matching element", _a_idx, _a_rds.size(), _l_fst));
			return std::optional<repetition_data_t>{};
		}
	}
	__constexpr_imp
		std::optional<repetition_data_t>
		repetition_tree_for_loop_index_data_t::increment_last_index(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept
	{
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		if (_a_idx >= _a_rds.size())
		{
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_a_idx ({0}) >= _a_rds.size() ({1}), "
				"Returning nothing", _a_idx, _a_rds.size()));
			return std::optional<repetition_data_t>{};
		}
		else if (_a_idx + 1 == _a_rds.size())
		{
			const repetition_data_t& _l_fst{ _a_rds[_a_idx] };
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"Checking _l_fst {0}", _l_fst));
			size_t _l_idx{ 0 };
			for (const repetition_tree_for_loop_iteration_data_t& _l_rfl : _m_for_loop_member)
			{
				if (_l_rfl.for_loop_iteration_data == _l_fst.for_loop_iteration_data &&
					_l_idx + 1 < _m_for_loop_member.size())
				{
					const repetition_data_t _l_rv{
						_m_for_loop_index,_m_for_loop_member[_l_idx + 1].for_loop_iteration_data };
					_LIBRARY_LOG(REPETITION_INFO, fmt::format(
						"_a_idx + 1 ({0}) == _a_rds.size() ({1}), "
						"_a_rds[_a_idx] = {2}, "
						"Element returned will be {3}", _a_idx+1, _a_rds.size(), _l_fst, _l_rv));
					return _l_rv;
				}
				_LIBRARY_LOG(REPETITION_INFO, fmt::format(
					"Unable to match with _l_rfl {0}", _l_rfl.print()));
				_l_idx++;
			}
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_a_idx + 1 ({0}) == _a_rds.size() ({1}), "
				"_a_rds[_a_idx] = {2}, "
				"Unable to find matching element in set", _a_idx+1, _a_rds.size(), _l_fst));
			return std::optional<repetition_data_t>{};
		}
		else
		{
			const repetition_data_t& _l_fst{ _a_rds[_a_idx] };
			for (const repetition_tree_for_loop_iteration_data_t& _l_rfl : _m_for_loop_member)
			{
				if (_l_rfl.for_loop_iteration_data == _l_fst.for_loop_iteration_data)
				{
					//Its added. Move on down.
					_LIBRARY_LOG(REPETITION_INFO, fmt::format(
						"_a_idx + 1 ({0}) < _a_rds.size() ({1}), "
						"_a_rds[_a_idx] = {2}, "
						"Moving down, element found", _a_idx + 1, _a_rds.size(), _l_fst));
					return _l_rfl.increment_last_index(_a_rds, _a_idx + 1);
				}
			}
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_a_idx ({0}) < _a_rds.size() ({1}), "
				"_a_rds[_a_idx] = {2}, "
				"Unable to find matching element", _a_idx, _a_rds.size(), _l_fst));
			return std::optional<repetition_data_t>{};
		}
	}
	__constexpr_imp
		bool
		repetition_tree_for_loop_index_data_t::is_repetition_to_be_repeated(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept
	{
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		if (_a_idx >= _a_rds.size())
		{
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"_a_idx ({0}) >= _a_rds.size() ({1}), "
				"Returning nothing", _a_idx, _a_rds.size()));
			return false;
		}
		else if (_a_idx + 1 == _a_rds.size())
		{
			const repetition_data_t& _l_fst{ _a_rds[_a_idx] };
			_LIBRARY_LOG(REPETITION_INFO, fmt::format(
				"Checking _l_fst {0}", _l_fst));
			size_t _l_idx{ 0 };
			for (const repetition_tree_for_loop_iteration_data_t& _l_rfl : _m_for_loop_member)
			{
				if (_l_rfl.for_loop_iteration_data == _l_fst.for_loop_iteration_data)
				{
					return true;
				}
				_l_idx++;
			}
			return false;
		}
		else
		{
			const repetition_data_t& _l_fst{ _a_rds[_a_idx] };
			for (const repetition_tree_for_loop_iteration_data_t& _l_rfl : _m_for_loop_member)
			{
				if (_l_rfl.for_loop_iteration_data == _l_fst.for_loop_iteration_data)
				{
					//Its added. Move on down.
					_LIBRARY_LOG(REPETITION_INFO, fmt::format(
						"_a_idx + 1 ({0}) < _a_rds.size() ({1}), "
						"_a_rds[_a_idx] = {2}, "
						"Moving down, element found", _a_idx + 1, _a_rds.size(), _l_fst));
					return _l_rfl.is_repetition_to_be_repeated(_a_rds, _a_idx + 1);
				}
			}
			return false;
		}
	}
	__constexpr_imp
		std::string
		repetition_tree_for_loop_index_data_t::print(
		) const noexcept
	{
		using namespace std;
		string _l_rv{ "({},[" + to_string(_m_for_loop_index) };
		for (size_t _l_idx{ 0 }; _l_idx < _m_for_loop_member.size(); ++_l_idx)
		{
			_l_rv.append(_m_for_loop_member[_l_idx].print());
			_l_rv.append(_l_idx + 1 < _m_for_loop_member.size() ? "," : "");
		}
		_l_rv.append("])");
		return _l_rv;
	}
	__constexpr_imp
		std::expected<repetition_tree_for_loop_index_data_t, std::string>
		parse_repetition_for_loop(
			const std::string_view _a_str,
			const std::size_t _a_depth
		) noexcept
	{
		using namespace std;
		using namespace utility::str;
		using enum utility::internal::internal_log_enum_t;
		size_t _l_current_pos{ 0 };
		size_t _l_mode{ 0 };
		size_t _l_start_pos;
		string _l_integer_str;
		size_t _l_size_t;
		vector<string> _l_vects;
		size_t _l_depth{ 0 };
		string _l_error_string;
		while (_l_current_pos < _a_str.size())
		{
			size_t _l_old_current_pos = _l_current_pos;
			char _l_current_char{ _a_str[_l_current_pos] };
			switch (_l_mode)
			{
			case 0:
				//Searhcing for opening bracket
				locate_relevant_character(_a_str, _c_l_round_bracket, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 1);
				_l_start_pos = maybe_set_data(_l_current_pos, 1);
				break;
			case 1:
				//Searching for comma after the first integer
				locate_relevant_character(_a_str, _c_comma, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 2);
				_l_integer_str = _l_current_pos == string::npos ? "" : _a_str.substr(_l_start_pos, (_l_current_pos - _l_start_pos));
				break;
			case 2:
				//Searching for square bracket stating the start of the vector of data.
				locate_relevant_character(_a_str, _c_l_square_bracket, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 3);
				break;
			case 3:
				//Looking for a new element in the vector of data.
				locate_relevant_character(_a_str, _c_l_round_bracket, _l_current_pos, _l_error_string);
				process_mode(_l_mode, _l_current_pos, 4);
				_l_start_pos = maybe_set_data(_l_current_pos);
				_l_depth = 1;
				break;
			case 4:
				//Looking for either a start or end bracket with the correct depth, and also to escape strings
				process_list_elements(_a_str, _l_current_pos, _l_error_string, _l_depth, _l_vects, _l_start_pos, _l_mode, 6, 5);
				break;
			case 5:
				process_end_of_list(_a_str, _l_current_pos, _l_error_string, _l_mode, 3);
				break;
			case 6:
				process_string(_a_str, _l_current_pos, _l_error_string, _l_mode, 4);
				break;
			default:
				_l_mode = numeric_limits<size_t>::max();
			}
			if (not _l_error_string.empty())
			{
				return unexpected(_l_error_string);
			}
			_l_current_pos++;
		}
		auto res = std::from_chars(_l_integer_str.data(), _l_integer_str.data() + _l_integer_str.size(), _l_size_t);
		repetition_tree_for_loop_index_data_t _l_rv(_l_size_t);
		size_t _l_idx = 0;
		_LIBRARY_LOG(PARSING_SEED, fmt::format("Depth {0}. "
			"Elements identified = {1}",
			_a_depth,
			_l_vects));
		for (auto& _l_c : _l_vects)
		{
			_LIBRARY_LOG(PARSING_SEED, fmt::format("Depth {0}. "
				"Attempting to parse into repetition_tree_for_loop_iteration_data_t \"{1}\"",
				_a_depth,
				_l_c));
			expected< repetition_tree_for_loop_iteration_data_t, string> _l_rflm =
				parse_repeition_for_loop_member(_l_c, _a_depth+1);
			if (_l_rflm.has_value())
			{
				_l_rv.add_repetition_for_loop_member(_l_rflm.value());
			}
			else
			{
				return unexpected(_l_rflm.error());
			}
		}
		return _l_rv;
	}
	_END_ABC_DS_NS


__no_constexpr_imp
auto
fmt::formatter<abc::ds::repetition_tree_for_loop_index_data_t>::format(
	abc::ds::repetition_tree_for_loop_index_data_t _a_rt,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_children_str{ "[" };
	size_t _l_idx{ 0 };
	for (auto& k : _a_rt.get_children())
	{
		_l_children_str.append(k.print());
		if (_l_idx + 1 == _a_rt.get_children().size())
		{

		}
		else
		{
			_l_children_str.append(", ");
		}
	}
	_l_children_str.append("]");
	const string _l_rv{ fmt::format(
		"{0} {{"
		"{1} = {2}"
		", {3} = {4}"
		"}}"
		, typeid(_a_rt).name()
		, "_m_for_loop_index", _a_rt.for_loop_index()
		, "_m_for_loop_member", _l_children_str
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}