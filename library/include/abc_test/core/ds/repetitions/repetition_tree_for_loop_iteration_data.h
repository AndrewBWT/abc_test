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
		* Default constructor
		*/
		__constexpr
			repetition_tree_for_loop_iteration_data_t(
			) noexcept;
		/*!
		* Constructor using generation index and additional data.
		*/
		__constexpr
			repetition_tree_for_loop_iteration_data_t(
				const size_t _a_generation_index,
				const std::size_t _a_mode,
				const std::string_view _a_additional_data
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
		* Gets the additional data of the element.
		*/
		__constexpr
			const std::string_view
			additional_data(
			) const noexcept;
		/*!
		* Gets the generation index of the element.
		*/
		__constexpr
			size_t
			generation_index(
			) const noexcept;
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
		* Get the mode of the element.
		*/
		__constexpr
			std::size_t
			mode(
			) const noexcept;
		/*!
		* Prints a repetition for loop element.
		*/
		std::string
			print(
			) const noexcept;
		/*!
		* Comparison operator for equality operators. 
		*/
		__constexpr
			bool
			operator==(
				const repetition_data_t& _a_rhs
				) const noexcept;
		/*!
		* Spaceship operator for element. NOte this doesn't check chidlren.
		*/
		__constexpr
			auto
			operator<=>(
				const repetition_tree_for_loop_iteration_data_t& _a_rhs
				) const noexcept;
	private:
		std::string _m_additional_data;
		size_t _m_generation_index;
		std::size_t _m_mode;
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
		) noexcept
		: repetition_tree_for_loop_iteration_data_t(0,0, "")
	{

	}
	__constexpr_imp
		repetition_tree_for_loop_iteration_data_t::repetition_tree_for_loop_iteration_data_t(
			const size_t _a_generation_index,
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		) noexcept
		: _m_generation_index(_a_generation_index)
		, _m_additional_data(_a_additional_data)
		, _m_mode(_a_mode)
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
		const std::string_view
		repetition_tree_for_loop_iteration_data_t::additional_data(
		) const noexcept
	{
		return _m_additional_data;
	}
	__constexpr_imp
		size_t
		repetition_tree_for_loop_iteration_data_t::generation_index(
		) const noexcept
	{
		return _m_generation_index;
	}
	__constexpr_imp
		const std::vector<repetition_tree_for_loop_index_data_t>&
		repetition_tree_for_loop_iteration_data_t::get_children(
		) const noexcept
	{
		return _m_for_loops;
	}
	__constexpr_imp
		bool
		repetition_tree_for_loop_iteration_data_t::operator==(
			const repetition_data_t& _a_rhs
			) const noexcept
	{
		__cmp_test_2(_m_additional_data, _a_rhs.additional_data());
		__cmp_test_2(_m_generation_index, _a_rhs.generation_collection_index());
		__cmp_test_2(_m_mode, _a_rhs.mode());
		return true;
	}
	__constexpr_imp
		auto
		repetition_tree_for_loop_iteration_data_t::operator<=>(
			const repetition_tree_for_loop_iteration_data_t& _a_rhs
			) const noexcept
	{
		using namespace std;
		return
			tie(
				_m_additional_data,
				_m_generation_index,
				_m_mode
			)
			<=>
			tie(
				_a_rhs._m_additional_data,
				_a_rhs._m_generation_index,
				_a_rhs._m_mode
			);
	}
	__constexpr_imp
		std::size_t
		repetition_tree_for_loop_iteration_data_t::mode(
		) const noexcept
	{
		return _m_mode;
	}
	_END_ABC_DS_NS