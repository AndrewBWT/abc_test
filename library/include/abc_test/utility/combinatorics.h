#pragma once

#include <vector>
#include <set>
#include <numeric>
#include <algorithm>

#include <iostream>
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_UTILITY_NS
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		bool Is_Forward_Iterator
	>
	class combinatorics_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = std::vector<T>;
		using return_type = std::vector<T>;
		using element_type = std::vector<T>;
		using mask = std::vector<std::size_t>;
		//The thing that is returned.
		return_type _m_current_result;
		//A copy which is stored when performing permutations.
		return_type _m_original_permutation;
		//The complete set of elements.
		element_type _m_elements;
		//The min size bound.
		size_t _m_min_size_bound;
		//The max size bound.
		size_t _m_max_size_bound;
		//The current size bound.
		size_t _m_current_size_bound;
		bool _m_current_size_bound_before_zero;
		//Whether permutations are finished.
	//	bool _m_permutations_finished;
		//A mask for the current combination.
		mask _m_combination_mask;
		//A mask for the current permutation of the current combination.
		mask _m_permutation_mask;
		//The current index in the permutation.
		std::size_t _m_permutation_index;
		size_t _l_calls = 0;
		constexpr
			combinatorics_iterator(
			) noexcept
		{}
		template<
			typename R
		>
		constexpr
			combinatorics_iterator(
				const R& _a_elements,
				const std::pair<size_t, size_t>& _a_size_bounds,
				const size_t _a_size_bound_to_start_at,
				const bool _a_current_size_bound_before_zero,
				const bool _a_comb_offset
			) noexcept
			: _m_elements(process_elements<R, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>(_a_elements))
			, _m_min_size_bound(_a_size_bounds.first)
			, _m_max_size_bound(_a_size_bounds.second)
			, _m_current_size_bound(_a_size_bound_to_start_at)
			, _m_current_size_bound_before_zero(_a_current_size_bound_before_zero)
			, _m_current_result(return_type(_a_size_bound_to_start_at))
			, _m_combination_mask(mask(_a_size_bound_to_start_at))
			, _m_permutation_mask(mask(Generate_Permutations ? _a_size_bound_to_start_at : 0))
			, _m_permutation_index(Generate_Permutations ? 1 : 0)
			, _m_original_permutation(Generate_Permutations ? return_type(_a_size_bounds.first) :
				return_type())
		{
			using namespace std;
			//Ensure that _m_current_result has the correct amount of space for enumerating all elements.
			_m_current_result.reserve(_a_size_bounds.second);
			//Set combination mask. If comb_offset, set with an offset otherwise from 0..n
			set_combination_mask(_a_comb_offset);
			//Then set the current result from the mask between 0 and the mim of the number of elemetns and size
			set_current_result_from_mask(0, std::min(_m_elements.size(), _m_current_result.size()));
			//Finally initialise permutations
			initialise_permutation< Is_Forward_Iterator>();

		}
		constexpr
			combinatorics_iterator<T,Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>&
			operator++(
				)
		{
			if constexpr (Is_Forward_Iterator)
			{
				return iterate_forward();
			}
			else
			{
				return iterate_backward();
			}
		}
		constexpr
			combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>&
			iterate_forward(
			)
		{
			if constexpr (Generate_Permutations)
			{
				if (not increment_permutations())
				{
					increment_combinations();
				}
			}
			else
			{
				increment_combinations();
			}
			return *this;
		}
		constexpr
			combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>
			operator++(
				int
				) noexcept
		{
			auto _l_cpy{ *this };
			++(*this);
			return _l_cpy;
		}
		constexpr
			combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>&
			operator--(
				) noexcept
		{
			if constexpr (Is_Forward_Iterator)
			{
				return iterate_backward();
			}
			else
			{
				return iterate_forward();
			}
		}
		constexpr
			combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>&
			iterate_backward(
				) noexcept
		{
			//If not at the minimum value
			if (not check_min_value(true))
			{
				if constexpr (Generate_Permutations)
				{
					if (not decrement_permutations())
					{
						decrement_combinations();
					}
				}
				else
				{
					decrement_combinations();
				}
			}
			return *this;
		}
		constexpr
			combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>
			operator--(
				int
				) noexcept
		{
			auto _l_cpy{ *this };
			--(*this);
			return _l_cpy;
		}
		constexpr
			bool
			operator==(
				const combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>& _a_rhs
				) const noexcept
		{
		//	if (_m_current_result != _a_rhs._m_current_result)
		//	{
		//		return false;
		//	}
		//	if (_m_original_permutation != _a_rhs._m_original_permutation)
		//	{
		//		return false;
		//	}
		//	if (_m_elements != _a_rhs._m_elements)
		//	{
		//		return false;
		//	}
			if (_m_min_size_bound != _a_rhs._m_min_size_bound)
			{
				return false;
			}
			if (_m_max_size_bound != _a_rhs._m_max_size_bound)
			{
				return false;
			}
			if (_m_current_size_bound != _a_rhs._m_current_size_bound)
			{
				return false;
			}
			if (_m_current_size_bound_before_zero != _a_rhs._m_current_size_bound_before_zero)
			{
				return false;
			}
			if (_m_combination_mask != _a_rhs._m_combination_mask)
			{
				return false;
			}
			if (_m_permutation_mask != _a_rhs._m_permutation_mask)
			{
				return false;
			}
			if (_m_permutation_index != _a_rhs._m_permutation_index)
			{
				return false;
			}
			return true;
		}
		constexpr
			bool
			operator!=(
				const combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>& _a_rhs
				) const noexcept
		{
			return !(*this == _a_rhs);
		}
		using reference = const combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>::value_type&;
		reference
			operator*(
				) const
		{
			if (_m_current_size_bound_before_zero || _m_current_size_bound > _m_max_size_bound)
			{
				throw std::runtime_error("Could not deference iterator");
			}
			return _m_current_result;
		}
	private:
		inline constexpr
			bool
			is_at_minimum_value(
			) const noexcept
		{
			return check_min_value(true);
		}
		inline constexpr
			bool
			is_just_above_minimum_value(
			) const noexcept
		{
			return check_min_value(false);
		}
		inline constexpr
			bool
			check_min_value(
				const bool _a_current_size_bound_before_zero_value_to_check
			) const noexcept
		{
			return _m_current_size_bound == 0 && _m_current_size_bound_before_zero == _a_current_size_bound_before_zero_value_to_check;
		}
		inline constexpr
			void
			set_to_min_value(
			) noexcept
		{
			_m_current_size_bound_before_zero = true;
		}
		inline constexpr
			void
			set_to_just_above_min_value(
			) noexcept
		{
			_m_current_size_bound_before_zero = false;
		}
		inline constexpr
			void
			incr_combination_mask(
				const size_t _a_index
			) noexcept
		{
			++_m_combination_mask[_a_index];
		}
		inline constexpr
			void
			decr_combination_mask(
				const size_t _a_index
			) noexcept
		{
			--_m_combination_mask[_a_index];
		}
		inline constexpr
			bool
			check_combination_mask_element(
				const size_t _a_comb_mask_index,
				const size_t _a_expected_sequence_element
			) const noexcept
		{
			return  (_a_comb_mask_index >= 0 && _m_combination_mask[_a_comb_mask_index] == _a_expected_sequence_element);
		}
		inline constexpr
			void
			decrement_combinations(
			) noexcept
		{
			if (is_just_above_minimum_value())
			{
				set_to_min_value();
			}
			else if (not is_at_minimum_value())
			{
				//This is the index of the combination_mask we're looking at.
				bool _m_increment_comb{ true };
				size_t _l_comb_mask_index{ _m_current_size_bound - 1 };
				size_t _l_expected_sequence_element{ _m_combination_mask[_l_comb_mask_index] };
				//Check if the index is above zero and the element is what we would expect for a sequence
				while (check_combination_mask_element(_l_comb_mask_index, _l_expected_sequence_element))
				//while (_l_comb_mask_index >= 0 && _m_combination_mask[_l_comb_mask_index] == _l_expected_sequence_element)
				{
					//If its the last index, we're doing something different.
					if (_l_comb_mask_index == 0)
					{
						//If we're reached the end and the sequence is in the form 0...k, then
						//this is the last element for a combination of this size. We should set
						//up the next index.
						if (_l_expected_sequence_element == 0)
						{
							setup_next_index<false>();
							return;
						}
						else
						{
							//The sequence is in the form n...k, where n > 0. No decrement is necessary,
							//Just decrement the value at n and reset all elements after it using iota.
							//Differs from regular behaiouvr, hence _m_incrment_comb needs to be set to false.
							//So that the next element is not incorrectly incremented.
							_m_increment_comb = false;
							break;
						}
					}
					else
					{
						//Otherwise move backwards, update the expected next element in sequence.
						--_l_comb_mask_index;
						--_l_expected_sequence_element;
					}
				}
				if (_m_increment_comb)
				{
					//The expected element in the sequence has not been found.
					//Therefore increment the next element in the sequence, then reset all elements after it
					//This ++ says move to the next element.
					++_l_comb_mask_index;
				}
				//Decrement the combination mask at this index.
				decr_combination_mask(_l_comb_mask_index);
				//Set the rest of the combination mask to the right of the set element,
				//and update the result to reflect the changes in the combination mask.
				increment_combinations_mask_and_set_result(_l_comb_mask_index, _m_current_size_bound, 
					_m_elements.size() - 
					//Sequence size
					(_m_current_size_bound - _l_comb_mask_index) + 1);
				initialise_permutation<false>();
			}
		}
		inline constexpr
			void
			increment_combinations_mask_and_set_result(
				const size_t _a_begin_index,
				const size_t _a_end_index,
				const size_t _a_first_comb_mask_new_value
			) noexcept
		{
			std::ranges::iota(_m_combination_mask.begin() + _a_begin_index + 1,
				_m_combination_mask.begin() + _a_end_index,
				_a_first_comb_mask_new_value);
			set_current_result_from_mask(_a_begin_index, _a_end_index);
		}
		inline constexpr
			void
			increment_combinations(
			) noexcept
		{
			//If at minimum value, set to the one above the minimum value.
			if (is_at_minimum_value())
			{
				set_to_just_above_min_value();
			}
			//If we're just above the minimum value (returning the empty index), increment to the next required combination
			else if (is_just_above_minimum_value())
			{
				setup_next_index<true>();
			}
			else
			{
				//Set the mask index, and the expected value.
				size_t _l_comb_mask_index{ _m_current_size_bound - 1 };
				size_t _l_expected_sequence_element{ _m_elements.size() - _m_current_size_bound + _l_comb_mask_index };
				while (check_combination_mask_element(_l_comb_mask_index, _l_expected_sequence_element))
				{
					//If we're at the end of the list, get the next valid sized combination.
					if (_l_comb_mask_index == 0)
					{
						setup_next_index<true>();
						return;
					}
					else
					{
						//Otherwise work way down the combination mask until we fail to find what we're looking for
						--_l_comb_mask_index;
						--_l_expected_sequence_element;
					}
				}
				//Increment the combination mask.
				incr_combination_mask(_l_comb_mask_index);
				//Set the rest of the elemnets in the combination mask, and update the result.
				increment_combinations_mask_and_set_result(_l_comb_mask_index,
					_m_current_size_bound, _m_combination_mask[_l_comb_mask_index] + 1);
				//Initialise permutations
				initialise_permutation<true>();
			}
		}
		constexpr inline
			bool
			decrement_permutations(

			) noexcept
		{
			if (_m_permutation_mask.size() <= 1)
			{
				std::ranges::fill(_m_permutation_mask, 0);
				return false;
			}
			while (_m_permutation_index < _m_current_size_bound)
			{
				if (_m_permutation_mask[_m_permutation_index] > 0)
				{
					std::swap(_m_current_result[(_m_permutation_index % 2) == 0 ?
						0 :
						_m_permutation_mask[_m_permutation_index] - 1
					], _m_current_result[_m_permutation_index]);
					_m_permutation_mask[_m_permutation_index]--;
					_m_permutation_index = 1;
					return true;
				}
				else
				{
					_m_permutation_mask[_m_permutation_index] = _m_permutation_index;
					++_m_permutation_index;
				}
			}
			_m_permutation_index = 1;
			std::copy(_m_original_permutation.begin(), _m_original_permutation.end(), _m_current_result.begin());
			std::ranges::fill(_m_permutation_mask, 0);
			return false;
		}
		constexpr inline
			bool
			increment_permutations(
			) noexcept
		{
			if (_m_permutation_mask.size() <= 1)
			{
				std::ranges::fill(_m_permutation_mask, 0);
				return false;
			}
			while (_m_permutation_index < _m_current_size_bound)
			{
				if (_m_permutation_mask[_m_permutation_index] < _m_permutation_index)
				{
					std::swap(_m_current_result[(_m_permutation_index % 2) == 0 ?
						0 :
						_m_permutation_mask[_m_permutation_index]
					], _m_current_result[_m_permutation_index]);
					++_m_permutation_mask[_m_permutation_index];
					_m_permutation_index = 1;
					return true;
				}
				else
				{
					_m_permutation_mask[_m_permutation_index] = 0;
					++_m_permutation_index;
				}
			}
			std::copy(_m_original_permutation.begin(), _m_original_permutation.end(), _m_current_result.begin());
			std::ranges::fill(_m_permutation_mask, 0);
			return false;
		}
		template<
			bool Is_Increment
		>
		constexpr inline
			void
			setup_next_index(
			) noexcept
		{
			using namespace std;
			//Increase or decrease current_size_bound.
			if constexpr (Is_Increment)
			{
				++_m_current_size_bound;
			}
			else
			{
				--_m_current_size_bound;
			}
			//Set the combination mask for the new value. If we're going backwards,
			//the offset should be set accordinly (i.e., not zero).
			set_combination_mask(!Is_Increment);
			if constexpr (Is_Increment)
			{
				//If we're incrementing add a new element.
				_m_combination_mask.push_back(_m_combination_mask.size());
				//We don't need to resize current_result as its already been reserved.
			}
			else
			{
				//If decrementing take one away.
				_m_combination_mask.pop_back();
				_m_current_result.pop_back();
			}
			//Set the current result from the mask.
			set_current_result_from_mask(0, _m_current_size_bound - (Is_Increment ? 1 : 0));
			if constexpr (Is_Increment)
			{
				//If we're incrementing then add the element according to the value in the mask.
				const size_t _l_elem_idx{ _m_combination_mask[_m_current_result.size()] };
				_m_current_result.push_back(
					_l_elem_idx < _m_elements.size() ?
					_m_elements[_l_elem_idx] : T{});
			}
			//Finally initialise the permutation.
			initialise_permutation< Is_Increment>();
		}
		template<
			bool Is_Begin
		>
		constexpr inline
			void
			initialise_permutation(
			) noexcept
		{
			//Wrap it all up in a constexpr so that if we don't have to do the work, we don't.
			if constexpr (Generate_Permutations)
			{
				//Store the original result in original_permutation.
				_m_original_permutation = _m_current_result;
				//Resize the mask to the current size.
				_m_permutation_mask.resize(_m_current_size_bound);
				//Fill the mask with zeros. Note its not a mask like the combination mask, its the
				//values of the stack if we're using a recursive variant of Heap's algorithm.
				std::ranges::fill(_m_permutation_mask, 0);
				//Set the permutation mask to 1 if its going to be used, zero otherwise.
				_m_permutation_index = _m_current_size_bound >= 1 ? 1 : 0;
				//If we're going backwards the logic is not simple
				if constexpr (not Is_Begin)
				{
					//If its greater than zero... (as in, there are actuall permutations we want to generate).
					if (_m_current_size_bound > 0)
					{
						//Set everything from 0..n. This denotes the state of the stack when
						//generating the last element according to heaps algorithm.
						std::iota(_m_permutation_mask.begin(), _m_permutation_mask.end(), 0);
						//If the size is even...
						if ((_m_current_size_bound % 2) == 0)
						{
							//Its even. The numbers are in the pattern
							//n-2,n-1,2 .. n-3,n,1.
							if (_m_current_size_bound > 2)
							{
								_m_current_result[0] = _m_original_permutation[_m_current_size_bound - 3];
								_m_current_result[1] = _m_original_permutation[_m_current_size_bound - 2];
								for (size_t _l_jdx{ 2 }; _l_jdx <= _m_current_size_bound - 3; ++_l_jdx)
								{
									_m_current_result[_l_jdx] = _m_original_permutation[_l_jdx - 1];
								}
							}
							_m_current_result[_m_current_size_bound - 2] = _m_original_permutation[_m_current_size_bound - 1];
							_m_current_result[_m_current_size_bound - 1] = _m_original_permutation[0];
						}
						else
						{
							//Its odd. The numbers are in the pattern
							//n,2 .. n-1,1.
							if (_m_current_size_bound > 1)
							{
								_m_current_result[0] = _m_original_permutation[_m_current_size_bound - 1];
								for (size_t _l_jdx{ 2 }; _l_jdx <= _m_current_size_bound - 1; ++_l_jdx)
								{
									_m_current_result[_l_jdx - 1] = _m_original_permutation[_l_jdx - 1];
								}
								_m_current_result[_m_current_size_bound - 1] = _m_original_permutation[0];
							}
						}
						//
					}
				}
			}
		}
		constexpr inline
			void
			set_current_result_from_mask(
				const size_t _a_min_value,
				const size_t _a_max_value
			) noexcept
		{
			for (size_t _l_idx{ _a_min_value }; _l_idx < _a_max_value; ++_l_idx)
			{
				_m_current_result[_l_idx] = _m_elements[_m_combination_mask[_l_idx]];
			}
		}
		constexpr inline
			void
			set_combination_mask(
				const bool _a_start_at_offset
			) noexcept
		{
			std::ranges::iota(_m_combination_mask, _a_start_at_offset ?
				_m_elements.size() - _m_current_size_bound :
				0
			);
		}
	};
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R = std::vector<T>
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	class combinatorics
	{
	public:
		using iterator = combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, true>;
		using const_iterator = iterator;
		using reverse_iterator = combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, false>;
		using const_reverse_iterator = reverse_iterator;
		using size_type = std::size_t;
		R elements;
		size_type min_number_of_elements_to_generate;
		size_type max_number_of_elements_to_generate;
		constexpr
			combinatorics(
			) noexcept;
		constexpr
			bool
			has_at_least_one_result(
			) const noexcept;
		constexpr
			iterator
			begin(
			) noexcept;
		constexpr
			iterator
			end(
			) noexcept;
		constexpr
			const_iterator
			begin(
			) const noexcept;
		constexpr
			const_iterator
			end(
			) const noexcept;
		constexpr
			const_iterator
			cbegin(
			) const noexcept;
		constexpr
			const_iterator
			cend(
			) const noexcept;
		constexpr
			const_reverse_iterator
			rbegin(
			) const noexcept;
		constexpr
			const_reverse_iterator
			rend(
			) const noexcept;
		constexpr
			reverse_iterator
			rbegin(
			) noexcept;
		constexpr
			reverse_iterator
			rend(
			) noexcept;
		constexpr
			const_reverse_iterator
			crbegin(
			) const noexcept;
		constexpr
			const_reverse_iterator
			crend(
			) const noexcept;
	private:
		template<
			bool Is_Begin
		>
		constexpr
			const_iterator
			make_iterator(
			) const noexcept;
		template<
			bool Is_Begin
		>
		constexpr
			const_reverse_iterator
			make_reverse_iterator(
			) const noexcept;
		constexpr
			std::pair<size_t, size_t>
			normalise_number_of_elements_to_generate(
			) const noexcept;
	};
	namespace
	{
		template<
			typename R
		>
		constexpr
			size_t
			count_unique_elements(
				const R&
			) noexcept;
		template<
			typename R
		>
		constexpr
			std::set<std::ranges::range_value_t<R>>
			create_set_from_r(
				const R&
			) noexcept;
	}
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	inline constexpr
		combinatorics<T,Allow_Duplicates, Generate_Permutations, R>::combinatorics(
		) noexcept
		: elements{ R{} }
		, min_number_of_elements_to_generate{ size_type(0) }
		, max_number_of_elements_to_generate{ size_type(0) }
	{
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	inline constexpr
		bool
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::has_at_least_one_result(
		) const noexcept
	{
		size_t _l_combination_elements{ 0 };
		if constexpr (std::ranges::sized_range<R>)
		{
			if constexpr (Allow_Duplicates)
			{
				_l_combination_elements = std::ranges::size(elements);
			}
			else
			{
				_l_combination_elements = count_unique_elements(elements);
			}
		}
		else
		{
			if constexpr (Allow_Duplicates)
			{
				for (auto&& _l_element : elements)
				{
					++_l_combination_elements;
				}
			}
			else
			{
				_l_combination_elements = count_unique_elements(elements);
			}
		}
		return (min_number_of_elements_to_generate <= max_number_of_elements_to_generate &&
			max_number_of_elements_to_generate <= _l_combination_elements);
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::begin(
		) const noexcept
	{
		return cbegin();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::end(
		) const noexcept
	{
		return cend();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::begin(
		) noexcept
	{
		return cbegin();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::end(
		) noexcept
	{
		return cend();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::cbegin(
		) const noexcept
	{
		return make_iterator<true>();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::cend(
		) const noexcept
	{
		return make_iterator<false>();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_reverse_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::rbegin(
		) const noexcept
	{
		return crbegin();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_reverse_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::rend(
		) const noexcept
	{
		return crend();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::reverse_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::rbegin(
		) noexcept
	{
		return crbegin();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::reverse_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::rend(
		) noexcept
	{
		return crend();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_reverse_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::crbegin(
		) const noexcept
	{
		return make_reverse_iterator<true>();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_reverse_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::crend(
		) const noexcept
	{
		return make_reverse_iterator<false>();
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	template<
		bool Is_Begin
	>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::make_iterator(
		) const noexcept
	{
		using namespace std;
		pair<size_t, size_t> _l_normalised_numb_elemnets_to_generate{ normalise_number_of_elements_to_generate() };
		const size_t _l_size_to_start_at{ Is_Begin ? _l_normalised_numb_elemnets_to_generate.first :
			_l_normalised_numb_elemnets_to_generate.second + 1 };
		return const_iterator(elements,
			_l_normalised_numb_elemnets_to_generate, 
			_l_size_to_start_at, false, false);
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	template<
		bool Is_Begin
	>
	constexpr
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::const_reverse_iterator
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::make_reverse_iterator(
		) const noexcept
	{
		using namespace std;
		pair<size_t, size_t> _l_normalised_numb_elemnets_to_generate{ normalise_number_of_elements_to_generate() };
		//Is_Begin, past the end of what we need. 
		const size_t _l_begin_index{ Is_Begin ? _l_normalised_numb_elemnets_to_generate.second : _l_normalised_numb_elemnets_to_generate.first };
		return const_reverse_iterator(elements,
			_l_normalised_numb_elemnets_to_generate,
			Is_Begin ? _l_normalised_numb_elemnets_to_generate.second : _l_normalised_numb_elemnets_to_generate.first == 0 ? 0 : _l_normalised_numb_elemnets_to_generate.first - 1,
			Is_Begin ? false : _l_normalised_numb_elemnets_to_generate.first == 0 && !Is_Begin,
			true);
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations,
		typename R
	>
		requires std::ranges::range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
	constexpr
		std::pair<size_t, size_t>
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R>::normalise_number_of_elements_to_generate(
		) const noexcept
	{
		using namespace std;
		return has_at_least_one_result() ?
			make_pair(min_number_of_elements_to_generate, max_number_of_elements_to_generate) :
			make_pair(size_type(0),size_type(0));
	}
	namespace
	{
		template<
			typename R
		>
		inline constexpr
			size_t
			count_unique_elements(
				const R& _a_elements
			) noexcept
		{
			return create_set_from_r(_a_elements).size();
		}
		template<
			typename R
		>
		inline constexpr
			std::set<std::ranges::range_value_t<R>>
			create_set_from_r(
				const R& _a_elements
			) noexcept
		{
			using namespace std;
			set<ranges::range_value_t<R>> _l_unique_set(_a_elements.begin(), _a_elements.end());
			return _l_unique_set;
		}
		template<
			typename R, 
			bool Allow_Duplicates,
			bool Generate_Permutations,
			bool Is_Forward_Iterator
		>
		inline constexpr
			combinatorics_iterator<std::ranges::range_value_t<R>, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>::element_type
			process_elements(
				const R& _a_elements
			) noexcept
		{
			using namespace std;
			using T = ranges::range_value_t<R>;
			using Container = combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>::element_type;
			if constexpr (Allow_Duplicates)
			{
				return Container(_a_elements.begin(), _a_elements.end());
			}
			else
			{
				const set<T> _l_unique_set(create_set_from_r(_a_elements));
				return Container(_l_unique_set.begin(), _l_unique_set.end());
			}
		}
	}
	_END_ABC_UTILITY_NS