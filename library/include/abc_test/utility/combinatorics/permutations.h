#pragma once

#include "abc_test/utility/combinatorics.h"

#include <numeric>
#include "abc_test/utility/internal/macros.h"


_BEGIN_ABC_UTILITY_NS
	template<
		typename T
	>
	class permutations : public combinatorics<T>
	{
	public:
		constexpr
			permutations(
			) noexcept;
		template<
			typename R
		>
			requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
		constexpr
			permutations(
				R&& _a_range
			) noexcept;
		constexpr virtual
			bool
			generate_next_result(
			) noexcept;
		constexpr virtual
			bool
			reset(
			) noexcept;
	private:
		constexpr
			void
			initialise_mask_and_current_permutation(
			) noexcept;
		using mask_t = std::vector<size_t>;
		mask_t _m_mask;
	};
	_END_ABC_UTILITY_NS

		_BEGIN_ABC_UTILITY_NS
	template<
		typename T
	>
	inline constexpr
		permutations<T>::permutations(
		) noexcept
		: combinatorics<T>()
		, _m_mask(mask_t(0))
	{

	}
	template<
		typename T
	>
	template<
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	inline constexpr
		permutations<T>::permutations(
			R&& _a_range
		) noexcept
		: combinatorics<T>(std::forward<R>(_a_range))
	{
	}
	template<
		typename T
	>
	constexpr inline
		bool
		permutations<T>::generate_next_result(
		) noexcept
	{
		if (this->_m_finished)
		{
			return false;
		}
		while (_m_idx < this->_m_total_elements)
		{
			if (_m_mask[_m_idx] < _m_idx)
			{
				std::swap(
					_m_current_result[
						(_m_idx % 2) == 0 ?
							0 :
							_m_mask[_m_idx]
					],
					_m_current_result[_m_idx]);
				++_m_mask[_m_idx];
				_m_idx = 1;
				return true;
			}
			else
			{
				_m_mask[_m_idx] = 0;
				++_m_idx;
			}
		}
		this->_m_finished = true;
		return false;
	}
	template<
		typename T
	>
	inline constexpr
		bool
		permutations<T>::reset(
		) noexcept
	{
		reset(_m_numb_elemnets_in_combination);
		return true;
	}
	template<
		typename T
	>
	inline constexpr
		void
		permutations<T>::initialise_mask_and_current_permutation(
		) noexcept
	{
		using namespace std;
		std::ranges::fill(_m_mask, 0);
		ranges::copy(this->_m_elements, std::begin(this->_m_current_result));
	}
	_END_ABC_UTILITY_NS