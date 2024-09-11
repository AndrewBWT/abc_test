#pragma once

#include "abc_test/utility/combinatorics.h"

#include <numeric>


/*namespace ab::test::utility
{
	template<
		typename T,
		bool Duplicates_Allowed = true,
		bool Return_Permutations = false,
		bool Return_Combinations = false
	>
	class combinations_and_permutations : public combinatorics<T>
	{
	public:
		constexpr
			combinations(
			) noexcept;
		template<
			typename R
		>
			requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
		constexpr
			combinations(
				R&& _a_range,
				const size_t _a_elements_per_vector
			) noexcept;
		constexpr virtual
			bool
			generate_next_result(
			) noexcept;
		constexpr virtual
			bool
			reset(
			) noexcept;
		constexpr
			bool
			reset(
				const size_t _a_elements_per_vector
			) noexcept;
		constexpr
			size_t
			get_numb_elements_in_result_combination(
			) const noexcept;
	private:
		constexpr
			combinations(
				element_collection<T>&& _a_combination_collection,
				size_t _a_size_of_combinations
			) noexcept;
		constexpr
			combinations(
				size_t _a_guaranteed_valid_size_of_combinations,
				element_collection<T>&& _a_combination_collection
			) noexcept;
		constexpr
			void
			initialise_mask_and_current_combination(
			) noexcept;
		using mask_t = std::vector<size_t>;
		size_t _m_numb_elemnets_in_combination;
		mask_t _m_mask;
	};
}

namespace ab::test::utility
{
	template<
		typename T,
		bool Duplicates_Allowed
	>
	inline constexpr
		combinations<T, Duplicates_Allowed>::combinations(
		) noexcept
		: combinatorics<T>()
		, _m_numb_elemnets_in_combination(0)
		, _m_mask(mask_t(0))
	{

	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	template<
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	inline constexpr
		combinations<T, Duplicates_Allowed>::combinations(
			R&& _a_range,
			const size_t _a_elements_per_vector
		) noexcept
		: combinations<T, Duplicates_Allowed>(process_range<T,R, Duplicates_Allowed>(std::forward<R>(_a_range)),_a_elements_per_vector)
	{
	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	constexpr inline 
		bool
		combinations<T, Duplicates_Allowed>::generate_next_result(
		) noexcept
	{
		if (this->_m_finished)
		{
			return false;
		}
		if (_m_numb_elemnets_in_combination == 0)
		{
			this->_m_finished = true;
			return false;
		}
		size_t _l_pivot{ _m_numb_elemnets_in_combination - 1 };
		size_t _l_pivot_cmp{ this->_m_total_elements - _m_numb_elemnets_in_combination + _l_pivot };
		while (_l_pivot >= 0 && _m_mask[_l_pivot] == _l_pivot_cmp)
		{
			if (_l_pivot == 0)
			{
				this->_m_finished = true;
				return false;
			}
			else
			{
				--_l_pivot;
				--_l_pivot_cmp;
			}
		}
		++_m_mask[_l_pivot];
		this->_m_current_result[_l_pivot] = this->_m_elements[_m_mask[_l_pivot]];
		size_t _m_mask_offset{ _m_mask[_l_pivot] + 1 };
		for (size_t _l_index{ _l_pivot + 1 }; _l_index < _m_numb_elemnets_in_combination; ++_l_index)
		{
			_m_mask[_l_index] = _m_mask_offset++;
			this->_m_current_result[_l_index] = this->_m_elements[_m_mask[_l_index]];
		}
		return true;
	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	inline constexpr
		bool
		combinations<T, Duplicates_Allowed>::reset(
		) noexcept
	{
		reset(_m_numb_elemnets_in_combination);
		return true;
	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	inline constexpr
		bool
		combinations<T, Duplicates_Allowed>::reset(
			const size_t _a_elements_per_vector
		) noexcept
	{
		using namespace std;
		const bool _l_reset_allowed{
			_a_elements_per_vector <= this->_m_total_elements
		};
		const size_t _m_elements_per_vector_checked{
			_l_reset_allowed ? _a_elements_per_vector : this->_m_total_elements };
		_m_numb_elemnets_in_combination = _m_elements_per_vector_checked;
		this->_m_current_result.resize(_m_numb_elemnets_in_combination);
		_m_mask.resize(_m_numb_elemnets_in_combination);
		this->_m_finished = false;
		initialise_mask_and_current_combination();
		return _l_reset_allowed;
	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	constexpr
		size_t
		combinations<T, Duplicates_Allowed>::get_numb_elements_in_result_combination(
		) const noexcept
	{
		return _m_numb_elemnets_in_combination;
	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	inline constexpr
		combinations<T, Duplicates_Allowed>::combinations(
			element_collection<T>&& _a_combination_collection,
			size_t _a_size_of_combinations
		) noexcept
		: combinations<T, Duplicates_Allowed>(_a_size_of_combinations > _a_combination_collection.size() ?
			_a_combination_collection.size() : _a_size_of_combinations, std::move(_a_combination_collection))
	{
	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	inline constexpr
		combinations<T, Duplicates_Allowed>::combinations(
			size_t _a_guaranteed_valid_size_of_combinations,
			element_collection<T>&& _a_combination_collection
		) noexcept
		: combinatorics<T>(_a_combination_collection, _a_guaranteed_valid_size_of_combinations)
		, _m_numb_elemnets_in_combination(_a_guaranteed_valid_size_of_combinations)
		, _m_mask(mask_t(_a_guaranteed_valid_size_of_combinations))
	{
		initialise_mask_and_current_combination();
	}
	template<
		typename T,
		bool Duplicates_Allowed
	>
	inline constexpr
		void
		combinations<T, Duplicates_Allowed>::initialise_mask_and_current_combination(
		) noexcept
	{
		using namespace std;
		// c++23 ranges::iota(_m_mask, 0);
		std::iota(_m_mask.begin(), _m_mask.end(), 0);
		ranges::copy(
			std::begin(this->_m_elements),
			std::begin(this->_m_elements) + _m_numb_elemnets_in_combination,
			std::begin(this->_m_current_result)
		);
	}
}*/