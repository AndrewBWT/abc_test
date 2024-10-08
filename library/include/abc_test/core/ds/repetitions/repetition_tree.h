#pragma once



#include "abc_test/core/ds/repetitions/for_loop_data_collection.h"

#include "abc_test/core/ds/repetitions/repetition_tree_for_loop_index_data.h"

_BEGIN_ABC_NS
class test_options_t;
_END_ABC_NS

_BEGIN_ABC_DS_NS
	/*!
	* Object contains a set of repetitions for a specific test; each node represents the point of a for loop,
	* with the arguments used to generate values.
	*/
	class repetition_tree_t
	{
	public:
		/*!
		* Default constructor.
		*/
		__constexpr
			repetition_tree_t(
			) noexcept;
		/*!
		* Adds a reptition_tree_for_l;oop_index_data object to the structure. Simply adds it to the vector.
		*/
		__constexpr
			void
			add_repetition_for_loop(
				const repetition_tree_for_loop_index_data_t& _a_rfl
			) noexcept;
		/*!
		* Prints the object in a format as described by the test_options. 
		*/
		__no_constexpr_or_inline
			std::string
			print_repetition_tree(
				const test_options_t& _a_options
			) const noexcept;
		/*!
		* Prints a repretition tree without a test options.
		*/
		__constexpr
			std::string
			print_repetition_tree(
			) const noexcept;
		/*!
		* When given a set of for_loop_colelction_t, finds the first index of the next for loop.
		*/
		__constexpr
			std::optional<repetition_data_t>
			find_next_for_loop(
				const for_loop_data_collection_t& _a_for_loop_data_collection
			) const noexcept;
		/*!
		* Incrmeents a given for loop;
		*/
		__constexpr
			std::optional<repetition_data_t>
			increment_last_index(
				const for_loop_data_collection_t& _a_for_loop_data_collection
			) const noexcept;
		/*!
		* Checks if the given for_loop_data_collection_t is contained in this repetition tree.
		*/
		__constexpr
			bool
			is_repetition_to_be_repeated(
				const for_loop_data_collection_t& _a_for_loop_data_collection
			) const noexcept;
		/*!
		* Adds a repetition data sequence to the underlying structure.
		*/
		__constexpr
			bool
			add_repetition(
				const repetition_data_sequence_t& _a_rds
			) noexcept;
	private:
		std::vector<repetition_tree_for_loop_index_data_t> _m_top_level_for_loops;
		__constexpr
			std::vector< repetition_tree_for_loop_index_data_t>::const_iterator
			find_with_for_loop_index(
				const std::size_t _a_for_loop_to_check
			) const noexcept;
	};
	/*!
	* Parses a repetition tree object, or returns an error string.
	*/
	__no_constexpr_or_inline
		std::expected<repetition_tree_t, std::string>
		parse_repetition_tree(
			const std::string_view _a_str,
			const test_options_t& _a_options,
			const std::size_t _a_depth
		) noexcept;
	_END_ABC_DS_NS

template
<
>
struct fmt::formatter<abc::ds::repetition_tree_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
		//Can't be constexpr due to use of fmt
	__no_constexpr
		auto
		format(
			abc::ds::repetition_tree_t _a_rt,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
	__constexpr_imp
		repetition_tree_t::repetition_tree_t(
		) noexcept
		: _m_top_level_for_loops(std::vector<repetition_tree_for_loop_index_data_t>())
	{

	}
	__constexpr_imp
		void
		repetition_tree_t::add_repetition_for_loop(
			const repetition_tree_for_loop_index_data_t& _a_rfl
		) noexcept
	{
		_m_top_level_for_loops.push_back(_a_rfl);
	}
	__constexpr_imp
		std::string
		repetition_tree_t::print_repetition_tree(
		) const noexcept
	{
		using namespace std;
		using namespace utility::str;
		string _l_rv{ _c_l_square_bracket };
		for (size_t _l_idx{ 0 }; _l_idx < _m_top_level_for_loops.size(); ++_l_idx)
		{
			_l_rv.append(_m_top_level_for_loops[_l_idx].print());
			_l_rv.append(_l_idx + 1 < _m_top_level_for_loops.size() ? _c_comma : "");
		}
		_l_rv.append(_c_r_square_bracket);
		return _l_rv;
	}
	__constexpr_imp
		std::optional<repetition_data_t>
		repetition_tree_t::find_next_for_loop(
			const for_loop_data_collection_t& _a_for_loop_data_collection
		) const noexcept
	{
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		const repetition_data_sequence_t& _l_rds{ _a_for_loop_data_collection.repetition_data_sequence() };
		const size_t _l_for_loop_to_check{ _l_rds.size() == 0 ? _a_for_loop_data_collection.current_for_loop_index() :
			_l_rds[0].for_loop_index};
		_LIBRARY_LOG(REPETITION_INFO, fmt::format("Find next for loop, _l_rds = {0}, "
			"_l_for_loop_to_check = {1}", _l_rds, _l_for_loop_to_check));
		const vector< repetition_tree_for_loop_index_data_t>::const_iterator _l_itt{ find_with_for_loop_index(_l_for_loop_to_check) };
		if (_l_itt != std::cend(_m_top_level_for_loops))
		{
			_LIBRARY_LOG(REPETITION_INFO, fmt::format("for loop value chekced = {0}, value found",
				_l_for_loop_to_check));
			return _l_itt->find_next_for_loop(_l_rds, 0);
		}
		else
		{
			_LIBRARY_LOG(REPETITION_INFO, fmt::format("for loop value chekced = {0}, unable to find",
				_l_for_loop_to_check));
			return optional<repetition_data_t>{};
		}
	}
	__constexpr_imp
		std::optional<repetition_data_t>
		repetition_tree_t::increment_last_index(
			const for_loop_data_collection_t& _a_for_loop_data_collection
		) const noexcept
	{
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		const repetition_data_sequence_t& _l_rds{ _a_for_loop_data_collection.repetition_data_sequence() };
		const size_t _l_for_loop_to_check{ _l_rds[0].for_loop_index };
		_LIBRARY_LOG(REPETITION_INFO, fmt::format("Increment last index, _l_rds = {0}, "
			"_l_for_loop_to_check = {1}", _l_rds, _l_for_loop_to_check));
		const vector< repetition_tree_for_loop_index_data_t>::const_iterator _l_itt{ find_with_for_loop_index(_l_for_loop_to_check) };
		if (_l_itt != std::cend(_m_top_level_for_loops))
		{
			_LIBRARY_LOG(REPETITION_INFO, fmt::format("for loop value chekced = {0}, value found",
				_l_for_loop_to_check));
			return _l_itt->increment_last_index(_l_rds, 0);
		}
		else
		{
			_LIBRARY_LOG(REPETITION_INFO, fmt::format("for loop value chekced = {0}, unable to find",
				_l_for_loop_to_check));
			return optional<repetition_data_t>{};
		}
	}
	__constexpr_imp
		bool
		repetition_tree_t::is_repetition_to_be_repeated(
			const for_loop_data_collection_t& _a_for_loop_data_collection
		) const noexcept
	{
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		const repetition_data_sequence_t& _l_rds{ _a_for_loop_data_collection.repetition_data_sequence() };
		if (_l_rds.empty())
		{
			return false;
		}
		else
		{
			const size_t _l_for_loop_to_check{ _l_rds[0].for_loop_index };
			_LIBRARY_LOG(REPETITION_INFO, fmt::format("Is repetition to be repeated. _l_rds = {0}, "
				"_l_for_loop_to_check = {1}", _l_rds, _l_for_loop_to_check));
			const vector< repetition_tree_for_loop_index_data_t>::const_iterator _l_itt{ find_with_for_loop_index(_l_for_loop_to_check) };
			if (_l_itt != std::cend(_m_top_level_for_loops))
			{
				_LIBRARY_LOG(REPETITION_INFO, fmt::format("for loop value chekced = {0}, value found",
					_l_for_loop_to_check));
				return _l_itt->is_repetition_to_be_repeated(_l_rds, 0);
			}
			else
			{
				return false;
			}
		}
	}
	__constexpr_imp
		bool
		repetition_tree_t::add_repetition(
			const repetition_data_sequence_t& _a_rds
		) noexcept
	{
		using enum utility::internal::internal_log_enum_t;
		_LIBRARY_LOG(REPETITION_INFO, fmt::format("Adding repetition sequence {0}", _a_rds));
		if (_a_rds.size() == 0)
		{
			return true;
		}
		else
		{
			const repetition_data_t& _l_fst{ _a_rds[0] };
			for (repetition_tree_for_loop_index_data_t& _l_rfl : _m_top_level_for_loops)
			{
				if (_l_rfl.for_loop_index() == _l_fst.for_loop_index)
				{
					//Its added. Move on down.
					return _l_rfl.add_repetition(_a_rds, 0);
				}
			}
			repetition_tree_for_loop_index_data_t _l_to_insert{ _l_fst.for_loop_index };
			auto _l_x = upper_bound(_m_top_level_for_loops.begin(), _m_top_level_for_loops.end(), _l_to_insert,
				[](const repetition_tree_for_loop_index_data_t& _a_left, const repetition_tree_for_loop_index_data_t& _a_right) {return _a_left.for_loop_index() < _a_right.for_loop_index(); });
			auto _l_itt = _m_top_level_for_loops.insert(
				_l_x,
				_l_to_insert
			);
			return _l_itt->add_repetition(_a_rds, 0);

		}
	}
	__constexpr_imp
		std::vector< repetition_tree_for_loop_index_data_t>::const_iterator
		repetition_tree_t::find_with_for_loop_index(
			const std::size_t _a_for_loop_to_check
		) const noexcept
	{
		using namespace std;
		using enum utility::internal::internal_log_enum_t;
		auto _l_end{ _m_top_level_for_loops.end() };
		for (auto _l_itt{ _m_top_level_for_loops.begin() }; _l_itt != _l_end; ++_l_itt)
		{
			if (_a_for_loop_to_check == _l_itt->for_loop_index())
			{
				return _l_itt;
			}
		}
		return _l_end;
	}
	_END_ABC_DS_NS


__no_constexpr_imp
auto
fmt::formatter<abc::ds::repetition_tree_t>::format(
	abc::ds::repetition_tree_t _a_rt,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_rv{ fmt::format(
		"{0} {{"
		"{1} = {2}}}"
		, typeid(_a_rt).name()
		, "_m_top_level_for_loops"
		, _a_rt.print_repetition_tree()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}