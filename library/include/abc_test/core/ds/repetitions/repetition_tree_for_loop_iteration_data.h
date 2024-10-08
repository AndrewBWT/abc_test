#pragma once
#include <string>
#include <expected>

#include <optional>

#include "abc_test/core/ds/repetitions/repetition_data.h"

_BEGIN_ABC_DS_NS
//Forward declaration
class repetition_tree_for_loop_index_data_t;
class repetition_tree_for_loop_iteration_data_t
{
public:
	/*!
	* Constructor using generation index and additional data.
	*/
	__constexpr
		repetition_tree_for_loop_iteration_data_t(
			const for_loop_iteration_data_t& _a_for_loop_iteration_data
		) noexcept;
	/*!
	* Simply adds an element to the inner vector.
	*/
	__constexpr
		void
		add_repetition_for_loop(
			const repetition_tree_for_loop_index_data_t& _a_rfl
		) noexcept;
	/*!
	* Adds a data sequence to the instance.
	*/
	__no_constexpr_or_inline
		bool
		add_repetition(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) noexcept;
	/*!
	* Finds the next element in the entire tree, based on the prefvious.
	*/
	__no_constexpr_or_inline
		std::optional<repetition_data_t>
		find_next_for_loop(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept;
	/*!
	* Incremnet for loop indexer.
	*/
	__no_constexpr_or_inline
		std::optional<repetition_data_t>
		increment_last_index(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept;
	__no_constexpr_or_inline
		bool
		is_repetition_to_be_repeated(
			const repetition_data_sequence_t& _a_rds,
			const size_t _a_idx
		) const noexcept;
	/*!
	* Gets the elmenets children.
	*/
	__constexpr
		const std::vector<repetition_tree_for_loop_index_data_t>&
		get_children(
		) const noexcept;
	/*!
	* Prints a repetition for loop element.
	*/
	std::string
		print(
		) const noexcept;
	ds::for_loop_iteration_data_t for_loop_iteration_data;
private:
	std::vector<repetition_tree_for_loop_index_data_t> _m_for_loops;
};
/*!
* Parses a repetition for loop element.
*/
__no_constexpr_or_inline
std::expected<repetition_tree_for_loop_iteration_data_t, std::string>
parse_repeition_for_loop_member(
const std::string_view _a_str,
	const std::size_t _a_depth
) noexcept;
_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__constexpr_imp
repetition_tree_for_loop_iteration_data_t::repetition_tree_for_loop_iteration_data_t(
	const for_loop_iteration_data_t& _a_for_loop_iteration_data
) noexcept
	: for_loop_iteration_data(_a_for_loop_iteration_data)
{

}
__constexpr_imp
void
repetition_tree_for_loop_iteration_data_t::add_repetition_for_loop(
	const repetition_tree_for_loop_index_data_t& _a_rfl
) noexcept
{
	_m_for_loops.push_back(_a_rfl);
}
__constexpr_imp
const std::vector<repetition_tree_for_loop_index_data_t>&
repetition_tree_for_loop_iteration_data_t::get_children(
) const noexcept
{
	return _m_for_loops;
}
	_END_ABC_DS_NS