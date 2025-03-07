#pragma once
#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_DG_NS

using integer_mask_t = std::vector<std::size_t>;

template <typename T, bool Generate_Elements_Forwards>
struct permutations_generator_t
{
public:
    __constexpr
        permutations_generator_t(
            const std::size_t     _a_current_size,
            const std::vector<T>& _a_current_combination
        ) noexcept;
    __constexpr void initialise(
        const std::size_t     _a_current_size,
        const std::vector<T>& _a_current_combination
    ) noexcept;
    __constexpr bool
        generate_next(const size_t _a_current_size_bound);
    __constexpr const std::vector<T>&
        current_element() const noexcept
    {
        return _m_current_permutation;
    }
private:
    integer_mask_t _m_permutation_mask;
    std::size_t    _m_permutation_index{ 1 };
    std::vector<T> _m_current_permutation;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T, bool Generate_Elements_Forwards>
__constexpr_imp
permutations_generator_t<T, Generate_Elements_Forwards>::
permutations_generator_t(
    const std::size_t     _a_current_size,
    const std::vector<T>& _a_current_combination
) noexcept
    : _m_permutation_mask(integer_mask_t(_a_current_size))
{
    initialise(_a_current_size, _a_current_combination);
}
template <typename T, bool Generate_Elements_Forwards>
__constexpr_imp void
permutations_generator_t<T, Generate_Elements_Forwards>::
initialise(
    const std::size_t     _a_current_size,
    const std::vector<T>& _a_current_combination
) noexcept
{
    using namespace std;
    _m_permutation_mask.resize(_a_current_size);
    if constexpr (Generate_Elements_Forwards)
    {
        ranges::fill(_m_permutation_mask, 0);
        _m_current_permutation = _a_current_combination;
    }
    else
    {
        _m_current_permutation.resize(_a_current_size);
        if (_a_current_size == 0)
        {
            return;
        }
        // Set everything from 0..n. This denotes the state of the stack
        // when generating the last element according to heaps algorithm.
        std::iota(_m_permutation_mask.begin(), _m_permutation_mask.end(), 0);
        // If the size is even...
        if ((_a_current_size % 2) == 0)
        {
            // Its even. The numbers are in the pattern
            // n-2,n-1,2 .. n-3,n,1.
            if (_a_current_size > 2)
            {
                _m_current_permutation[0]
                    = _a_current_combination[_a_current_size - 3];
                _m_current_permutation[1]
                    = _a_current_combination[_a_current_size - 2];
                for (size_t _l_jdx{ 2 }; _l_jdx <= _a_current_size - 3; ++_l_jdx)
                {
                    _m_current_permutation[_l_jdx]
                        = _a_current_combination[_l_jdx - 1];
                }
            }
            _m_current_permutation[_a_current_size - 2]
                = _a_current_combination[_a_current_size - 1];
            _m_current_permutation[_a_current_size - 1]
                = _a_current_combination[0];
        }
        else
        {
            // Its odd. The numbers are in the pattern
            // n,2 .. n-1,1.
            if (_a_current_size > 1)
            {
                _m_current_permutation[0]
                    = _a_current_combination[_a_current_size - 1];
                for (size_t _l_jdx{ 2 }; _l_jdx <= _a_current_size - 1; ++_l_jdx)
                {
                    _m_current_permutation[_l_jdx - 1]
                        = _a_current_combination[_l_jdx - 1];
                }
                _m_current_permutation[_a_current_size - 1]
                    = _a_current_combination[0];
            }
            else
            {
                _m_current_permutation = _a_current_combination;
            }
        }
        //
    }
}
template <typename T, bool Generate_Elements_Forwards>
__constexpr_imp bool
permutations_generator_t<T, Generate_Elements_Forwards>::generate_next(const size_t _a_current_size_bound)
{
    if constexpr (Generate_Elements_Forwards)
    {
        if (_m_permutation_mask.size() <= 1)
        {
            std::ranges::fill(_m_permutation_mask, 0);
            return false;
        }
        while (_m_permutation_index < _a_current_size_bound)
        {
            if (_m_permutation_mask[_m_permutation_index] < _m_permutation_index)
            {
                std::swap(_m_current_permutation[(_m_permutation_index % 2) == 0 ?
                    0 :
                    _m_permutation_mask[_m_permutation_index]
                ], _m_current_permutation[_m_permutation_index]);
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
        //std::copy(_m_original_permutation.begin(), _m_original_permutation.end(), _m_current_result.begin());
        //std::ranges::fill(_m_permutation_mask, 0);
        return false;
    }
    else
    {
        if (_m_permutation_mask.size() <= 1)
        {
            std::ranges::fill(_m_permutation_mask, 0);
            return false;
        }
        while (_m_permutation_index < _a_current_size_bound)
        {
            if (_m_permutation_mask[_m_permutation_index] > 0)
            {
                std::swap(_m_current_permutation[(_m_permutation_index % 2) == 0 ?
                    0 :
                    _m_permutation_mask[_m_permutation_index] - 1
                ], _m_current_permutation[_m_permutation_index]);
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
        //std::copy(_m_original_permutation.begin(), _m_original_permutation.end(), _m_current_result.begin());
        //std::ranges::fill(_m_permutation_mask, 0);
        return false;
    }
}
_END_ABC_DG_NS