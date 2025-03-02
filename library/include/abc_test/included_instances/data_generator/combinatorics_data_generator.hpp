#pragma once

#include "abc_test/included_instances/data_generator/combinatorics_data_generator/permutations_generator.hpp"
#include "abc_test/internal/data_generator/data_generator_collection.hpp"
#include "abc_test/internal/data_generator/data_generator_with_file_support.hpp"

#include <unordered_set>

_BEGIN_ABC_DG_NS

using combinatorics_schema_t = std::pair<std::size_t, std::size_t>;

template <typename T>
using combinatorics_return_type = std::vector<T>;

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>

class combinatorics_data_generator_t
{
public:
    using generator_type = combinatorics_return_type<T>;
    using tertiary_type  = std::size_t;
    __constexpr
    combinatorics_data_generator_t(
        const std::vector<T>&         _a_elements,
        const combinatorics_schema_t& _a_edo
    ) noexcept;
    __constexpr bool
        has_current_element() const noexcept;
    __constexpr bool
        generate_next();
    __constexpr const generator_type&
        current_element() const noexcept;
    __constexpr const tertiary_type&
        tertiary_data() const noexcept;
    __constexpr void
        set_generator_using_tertiary_data(const tertiary_type& _a_tertiary_data
        );
    __constexpr void
        reset() noexcept;
private:
    std::size_t    _m_min_number_of_elements_to_generate;
    std::size_t    _m_max_number_of_elements_to_generate;
    std::size_t    _m_current_size_bound;
    std::vector<T> _m_current_element;
    std::vector<T> _m_complete_set_of_elements;
    tertiary_type  _m_tertiary_data;
    integer_mask_t _m_combination_mask;
    bool           _m_has_current_element;
    std::conditional_t<
        Generate_Permutations,
        permutations_generator_t<T, Generate_Elements_Forwards>,
        std::monostate>
        _m_permutation_combinator;
    __constexpr
    combinatorics_data_generator_t(
        const std::vector<T>&         _a_elements,
        const combinatorics_schema_t& _a_edo,
        const std::size_t             _a_starting_size
    ) noexcept;
    __constexpr
    combinatorics_data_generator_t(
        const std::vector<T>&         _a_elements,
        const combinatorics_schema_t& _a_edo,
        const std::size_t             _a_starting_size,
        const integer_mask_t&         _a_starting_mask
    ) noexcept;
    __constexpr
    combinatorics_data_generator_t(
        const std::vector<T>&         _a_elements,
        const combinatorics_schema_t& _a_edo,
        const std::size_t             _a_starting_size,
        const integer_mask_t&         _a_starting_mask,
        const std::vector<T>&         _a_starting_combination
    ) noexcept;

    __constexpr void
        set_combination_mask(const bool _a_start_at_offset) noexcept;
    __constexpr void
        set_current_result_from_mask(
            const size_t _a_min_value,
            const size_t _a_max_value
        ) noexcept;
    __constexpr bool
        check_combination_mask_element(
            const size_t _a_comb_mask_index,
            const size_t _a_expected_sequence_element
        ) const noexcept;
    template <bool Is_Increment>
    __constexpr bool
        setup_next_index() noexcept;
    __constexpr void
        incr_combination_mask(const size_t _a_index) noexcept;
    __constexpr void
        increment_combinations_mask_and_set_result(
            const size_t _a_begin_index,
            const size_t _a_end_index,
            const size_t _a_first_comb_mask_new_value
        ) noexcept;
    __constexpr void
        decr_combination_mask(const size_t _a_index) noexcept;
};

namespace
{
template <bool Allow_Duplicates, typename R>
__constexpr std::vector<std::ranges::range_value_t<R>>
            normalise_elements(R&& _a_elements) noexcept;
__constexpr combinatorics_schema_t
    normalise_size(
        const combinatorics_schema_t& _a_schema,
        const std::size_t             _a_n_elements
    ) noexcept;
template <bool Generate_Elements_Forwards>
__constexpr std::size_t
    deduce_beginning_index(const combinatorics_schema_t& _a_schema) noexcept;
template <bool Generate_Elements_Forwards>
__constexpr integer_mask_t
    create_initial_mask(
        const std::size_t _a_current_size,
        const std::size_t _a_total_elements
    ) noexcept;
template <typename T>
__constexpr std::vector<T>
            create_initial_combination(
                const integer_mask_t& _a_mask,
                const std::vector<T>& _a_all_elements
            ) noexcept;
template <
    typename T,
    bool Generate_Permutations,
    bool Generate_Elements_Forwards>
auto
    initialise_permutation(
        const std::size_t     _a_element,
        const std::vector<T>& _a_starting_combination
    ) noexcept;
} // namespace

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <
    typename T,
    bool Generate_Permutations      = false,
    bool Allow_Duplicates           = false,
    bool Generate_Elements_Forwards = true,
    typename... Args>
__constexpr _ABC_NS_DG::data_generator_collection_t<
    _ABC_NS_DG::combinatorics_return_type<T>>
    using_combinatorics_generator_(
        const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
        const _ABC_NS_DG::enumeration_t<T>&        _a_edo,
        Args... _a_file_reader_writers
    );
template <
    typename T,
    bool Generate_Permutations      = false,
    bool Allow_Duplicates           = false,
    bool Generate_Elements_Forwards = true,
    typename R                      = std::initializer_list<T>,
    typename... Args>
__constexpr _ABC_NS_DG::data_generator_collection_t<
    _ABC_NS_DG::combinatorics_return_type<T>>
    using_combinatorics_generator(
        R&& _a_collection_of_elements,
        Args... _a_file_reader_writers
    );
template <
    typename T,
    bool Generate_Permutations      = false,
    bool Allow_Duplicates           = true,
    bool Generate_Elements_Forwards = true,
    typename R                      = std::initializer_list<T>,
    typename... Args>
__constexpr _ABC_NS_DG::data_generator_collection_t<
    _ABC_NS_DG::combinatorics_return_type<T>>
    using_combinatorics_generator(
        R&&                                       _a_collection_of_elements,
        const _ABC_NS_DG::combinatorics_schema_t& _a_combinations_sizes,
        Args... _a_file_reader_writers
    );
__constexpr _ABC_NS_DG::combinatorics_schema_t
            of_size(const std::size_t _a_element) noexcept;
__constexpr _ABC_NS_DG::combinatorics_schema_t
            of_sizes_m_to_n(
                const std::size_t _a_element_l,
                const std::size_t _A_element_r
            ) noexcept;

_END_ABC_NS

_BEGIN_ABC_DG_NS
template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        combinatorics_data_generator_t(
            const std::vector<T>&         _a_elements,
            const combinatorics_schema_t& _a_edo
        ) noexcept
    : combinatorics_data_generator_t<
          T,
          Generate_Permutations,
          Allow_Duplicates,
          Generate_Elements_Forwards>(
          _a_elements,
          _a_edo,
          deduce_beginning_index<Generate_Elements_Forwards>(_a_edo)
      )
{}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp bool
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::has_current_element() const noexcept
{
    return _m_has_current_element;
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp bool
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::generate_next()
{
    if constexpr (Generate_Permutations)
    {
        if (_m_permutation_combinator.generate_next(_m_current_size_bound))
        {
            ++_m_tertiary_data;
            return true;
        }
    }
    if constexpr (Generate_Elements_Forwards)
    {
        if (_m_current_size_bound == 0)
        {
            return setup_next_index<true>();
        }
        else
        {
            // Set the mask index, and the expected value.
            size_t _l_comb_mask_index{_m_current_size_bound - 1};
            size_t _l_expected_sequence_element{
                _m_complete_set_of_elements.size() - _m_current_size_bound
                + _l_comb_mask_index
            };
            while (check_combination_mask_element(
                _l_comb_mask_index, _l_expected_sequence_element
            ))
            {
                // If we're at the end of the list, get the next valid sized
                // combination.
                if (_l_comb_mask_index == 0)
                {
                    return setup_next_index<true>();
                }
                else
                {
                    // Otherwise work way down the combination mask until we
                    // fail to find what we're looking for
                    --_l_comb_mask_index;
                    --_l_expected_sequence_element;
                }
            }
            // Increment the combination mask.
            incr_combination_mask(_l_comb_mask_index);
            // Set the rest of the elemnets in the combination mask, and
            // update the result.
            increment_combinations_mask_and_set_result(
                _l_comb_mask_index,
                _m_current_size_bound,
                _m_combination_mask[_l_comb_mask_index] + 1
            );
            if constexpr (Generate_Permutations)
            {
                _m_permutation_combinator.initialise(
                    _m_current_size_bound, _m_current_element
                );
            }
            ++_m_tertiary_data;
            return true;
        }
    }
    else
    {
        if (_m_current_size_bound == 0)
        {
            _m_has_current_element = false;
            return false;
        }
        else
        {
            // This is the index of the combination_mask we're looking at.
            bool   _m_increment_comb{true};
            size_t _l_comb_mask_index{_m_current_size_bound - 1};
            size_t _l_expected_sequence_element{
                _m_combination_mask[_l_comb_mask_index]
            };
            // Check if the index is above zero and the element is what we would
            // expect for a sequence
            while (check_combination_mask_element(
                _l_comb_mask_index, _l_expected_sequence_element
            ))
            // while (_l_comb_mask_index >= 0 &&
            // _m_combination_mask[_l_comb_mask_index] ==
            // _l_expected_sequence_element)
            {
                // If its the last index, we're doing something different.
                if (_l_comb_mask_index == 0)
                {
                    // If we're reached the end and the sequence is in the form
                    // 0...k, then this is the last element for a combination of
                    // this size. We should set up the next index.
                    if (_l_expected_sequence_element == 0)
                    {
                        return setup_next_index<false>();
                    }
                    else
                    {
                        // The sequence is in the form n...k, where n > 0. No
                        // decrement is necessary, Just decrement the value at n
                        // and reset all elements after it using iota. Differs
                        // from regular behaiouvr, hence _m_incrment_comb needs
                        // to be set to false. So that the next element is not
                        // incorrectly incremented.
                        _m_increment_comb = false;
                        break;
                    }
                }
                else
                {
                    // Otherwise move backwards, update the expected next
                    // element in sequence.
                    --_l_comb_mask_index;
                    --_l_expected_sequence_element;
                }
            }
            if (_m_increment_comb)
            {
                // The expected element in the sequence has not been found.
                // Therefore increment the next element in the sequence, then
                // reset all elements after it This ++ says move to the next
                // element.
                ++_l_comb_mask_index;
            }
            // Decrement the combination mask at this index.
            decr_combination_mask(_l_comb_mask_index);
            // Set the rest of the combination mask to the right of the set
            // element, and update the result to reflect the changes in the
            // combination mask.
            increment_combinations_mask_and_set_result(
                _l_comb_mask_index,
                _m_current_size_bound,
                _m_complete_set_of_elements.size() -
                    // Sequence size
                    (_m_current_size_bound - _l_comb_mask_index) + 1
            );
            if constexpr (Generate_Permutations)
            {
                _m_permutation_combinator.initialise(
                    _m_current_size_bound, _m_current_element
                );
            }
            // initialise_permutation<false>();
        }
    }
    ++_m_tertiary_data;
    return true;
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp const combinatorics_data_generator_t<
    T,
    Generate_Permutations,
    Allow_Duplicates,
    Generate_Elements_Forwards>::generator_type&
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::current_element() const noexcept
{
    if constexpr (Generate_Permutations)
    {
        return _m_permutation_combinator.current_element();
    }
    else
    {
        return _m_current_element;
    }
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp const combinatorics_data_generator_t<
    T,
    Generate_Permutations,
    Allow_Duplicates,
    Generate_Elements_Forwards>::tertiary_type&
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::tertiary_data() const noexcept
{
    return _m_tertiary_data;
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp void
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        set_generator_using_tertiary_data(
            const tertiary_type& _a_tertiary_data
        )
{
    while (_m_tertiary_data < _a_tertiary_data)
    {
        const bool _l_res{generate_next()};
        if (not _l_res)
        {
            return;
        }
    }
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp void
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::reset() noexcept
{}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        combinatorics_data_generator_t(
            const std::vector<T>&         _a_elements,
            const combinatorics_schema_t& _a_edo,
            const std::size_t             _a_current_size
        ) noexcept
    : combinatorics_data_generator_t<
          T,
          Generate_Permutations,
          Allow_Duplicates,
          Generate_Elements_Forwards>(
          _a_elements,
          _a_edo,
          _a_current_size,
          create_initial_mask<Generate_Elements_Forwards>(
              _a_current_size,
              _a_elements.size()
          )
      )
{}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        combinatorics_data_generator_t(
            const std::vector<T>&         _a_elements,
            const combinatorics_schema_t& _a_edo,
            const std::size_t             _a_current_size,
            const integer_mask_t&         _a_starting_mask
        ) noexcept
    : combinatorics_data_generator_t<
          T,
          Generate_Permutations,
          Allow_Duplicates,
          Generate_Elements_Forwards>(
          _a_elements,
          _a_edo,
          _a_current_size,
          _a_starting_mask,
          create_initial_combination(_a_starting_mask, _a_elements)
      )
{}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        combinatorics_data_generator_t(
            const std::vector<T>&         _a_elements,
            const combinatorics_schema_t& _a_edo,
            const std::size_t             _a_current_size,
            const integer_mask_t&         _a_starting_mask,
            const std::vector<T>&         _a_starting_combination
        ) noexcept
    : _m_complete_set_of_elements(_a_elements)
    , _m_min_number_of_elements_to_generate(_a_edo.first)
    , _m_max_number_of_elements_to_generate(_a_edo.second)
    , _m_current_size_bound(_a_current_size)
    , _m_current_element(_a_starting_combination)
    , _m_combination_mask(_a_starting_mask)
    , _m_permutation_combinator(initialise_permutation<
                                T,
                                Generate_Permutations,
                                Generate_Elements_Forwards>(
          _a_current_size,
          _a_starting_combination
      ))
    , _m_has_current_element(true)
    , _m_tertiary_data(0)
{}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp void
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        set_combination_mask(
            const bool _a_start_at_offset
        ) noexcept
{
    std::ranges::iota(
        _m_combination_mask,
        _a_start_at_offset
            ? _m_complete_set_of_elements.size() - _m_current_size_bound
            : 0
    );
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp void
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        set_current_result_from_mask(
            const size_t _a_min_value,
            const size_t _a_max_value
        ) noexcept
{
    for (size_t _l_idx{_a_min_value}; _l_idx < _a_max_value; ++_l_idx)
    {
        _m_current_element[_l_idx]
            = _m_complete_set_of_elements[_m_combination_mask[_l_idx]];
    }
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp bool
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        check_combination_mask_element(
            const size_t _a_comb_mask_index,
            const size_t _a_expected_sequence_element
        ) const noexcept
{
    return _a_comb_mask_index >= 0
           && _m_combination_mask[_a_comb_mask_index]
                  == _a_expected_sequence_element;
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
template <bool Is_Increment>
__constexpr_imp bool
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::setup_next_index() noexcept
{
    using namespace std;
    // Increase or decrease current_size_bound.
    if constexpr (Is_Increment)
    {
        ++_m_current_size_bound;
        if (_m_current_size_bound > _m_max_number_of_elements_to_generate)
        {
            _m_has_current_element = false;
            return false;
        }
    }
    else
    {
        --_m_current_size_bound;
        if (_m_current_size_bound < _m_min_number_of_elements_to_generate)
        {
            _m_has_current_element = false;
            return false;
        }
    }
    // Set the combination mask for the new value. If we're going backwards,
    // the offset should be set accordinly (i.e., not zero).
    set_combination_mask(! Is_Increment);
    if constexpr (Is_Increment)
    {
        // If we're incrementing add a new element.
        _m_combination_mask.push_back(_m_combination_mask.size());
        // We don't need to resize current_result as its already been
        // reserved.
    }
    else
    {
        // If decrementing take one away.
        _m_combination_mask.pop_back();
        _m_current_element.pop_back();
    }
    // Set the current result from the mask.
    set_current_result_from_mask(
        0, _m_current_size_bound - (Is_Increment ? 1 : 0)
    );
    if constexpr (Is_Increment)
    {
        // If we're incrementing then add the element according to the value
        // in the mask.
        const size_t _l_elem_idx{_m_combination_mask[_m_current_element.size()]
        };
        _m_current_element.push_back(
            _l_elem_idx < _m_complete_set_of_elements.size()
                ? _m_complete_set_of_elements[_l_elem_idx]
                : T{}
        );
    }
    // Finally initialise the permutation.
    if constexpr (Generate_Permutations)
    {
        _m_permutation_combinator.initialise(
            _m_current_size_bound, _m_current_element
        );
    }
    ++_m_tertiary_data;
    return true;
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp void
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        incr_combination_mask(
            const size_t _a_index
        ) noexcept
{
    ++_m_combination_mask[_a_index];
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp void
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        increment_combinations_mask_and_set_result(
            const size_t _a_begin_index,
            const size_t _a_end_index,
            const size_t _a_first_comb_mask_new_value
        ) noexcept
{
    std::ranges::iota(
        _m_combination_mask.begin() + _a_begin_index + 1,
        _m_combination_mask.begin() + _a_end_index,
        _a_first_comb_mask_new_value
    );
    set_current_result_from_mask(_a_begin_index, _a_end_index);
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards>
__constexpr_imp void
    combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>::
        decr_combination_mask(
            const size_t _a_index
        ) noexcept
{
    --_m_combination_mask[_a_index];
}

namespace
{
template <bool Allow_Duplicates, typename R>
__constexpr_imp std::vector<std::ranges::range_value_t<R>>
                normalise_elements(
                    R&& _a_elements
                ) noexcept
{
    using namespace std;
    using T = std::ranges::range_value_t<R>;
    if constexpr (Allow_Duplicates)
    {
        return vector<T>(ranges::begin(_a_elements), ranges::end(_a_elements));
    }
    else
    {
        const unordered_set<T> _l_set(
            ranges::begin(_a_elements), ranges::end(_a_elements)
        );
        return vector<T>(ranges::begin(_l_set), ranges::end(_l_set));
    }
}

__constexpr_imp combinatorics_schema_t
    normalise_size(
        const combinatorics_schema_t& _a_schema,
        const std::size_t             _a_n_elements
    ) noexcept
{
    using namespace std;
    const size_t& _l_lower{
        _a_schema.first > _a_schema.second ? _a_schema.second : _a_schema.first
    };
    const size_t& _l_higher{
        _a_schema.first > _a_schema.second ? _a_schema.first : _a_schema.second
    };
    if (_a_n_elements <= _l_lower)
    {
        //_N_ELEMENTS <= LOWER < HIGHER.
        // Omt warning. Having to return _n_elements,_n_elements.
        return make_pair(_a_n_elements, _a_n_elements);
    }
    else
    {
        // LOWER < _N_ELEMENTS && LOWER <= HIGHER.
        if (_a_n_elements < _l_higher)
        {
            // LOWER < _N_ELEMENTS < HIGHER.
            // Omit warning. Having to return _l_lower,_a_n_elements.
            return make_pair(_l_lower, _a_n_elements);
        }
        else
        {
            // LOWER < HIGHER <= _A_N_ELEMENTS
            return _a_schema;
        }
    }
}

template <bool Generate_Elements_Forwards>
__constexpr_imp std::size_t
                deduce_beginning_index(
                    const combinatorics_schema_t& _a_schema
                ) noexcept
{
    if constexpr (Generate_Elements_Forwards)
    {
        return _a_schema.first;
    }
    else
    {
        return _a_schema.second;
    }
}

template <bool Generate_Elements_Forwards>
__constexpr_imp integer_mask_t
    create_initial_mask(
        const std::size_t _a_current_size,
        const std::size_t _a_total_elements
    ) noexcept
{
    using namespace std;
    integer_mask_t _l_rv(_a_current_size);
    if constexpr (Generate_Elements_Forwards)
    {
        ranges::iota(_l_rv, 0);
    }
    else
    {
        ranges::iota(_l_rv, _a_total_elements - _a_current_size);
    }
    return _l_rv;
}

template <typename T>
__constexpr_imp std::vector<T>
                create_initial_combination(
                    const integer_mask_t& _a_mask,
                    const std::vector<T>& _a_all_elements
                ) noexcept
{
    using namespace std;
    vector<T> _l_rv(_a_mask.size());
    for (size_t _l_idx{0}; _l_idx < _a_mask.size(); ++_l_idx)
    {
        _l_rv[_l_idx] = _a_all_elements[_a_mask[_l_idx]];
    }
    return _l_rv;
}

template <
    typename T,
    bool Generate_Permutations,
    bool Generate_Elements_Forwards>
auto
    initialise_permutation(
        const std::size_t     _a_element,
        const std::vector<T>& _a_starting_combination
    ) noexcept
{
    if constexpr (Generate_Permutations)
    {
        return permutations_generator_t<T, Generate_Elements_Forwards>(
            _a_element, _a_starting_combination
        );
    }
    else
    {
        return std::monostate();
    }
}
} // namespace

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards,
    typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<
    _ABC_NS_DG::combinatorics_return_type<T>>
    using_combinatorics_generator_(
        const _ABC_NS_DG::enumeration_schema_t<T>& _a_es,
        const _ABC_NS_DG::enumeration_t<T>&        _a_edo,
        Args... _a_file_reader_writers
    )
{
    using namespace _ABC_NS_DG;
    auto ki = _a_es.get()->enumerate_all_values<std::vector<T>>(_a_edo);
    const std::size_t _l_range_size{std::ranges::size(ki)};
    return using_combinatorics_generator<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards,
        std::vector<T>,
        Args...>(std::move(ki), {0, _l_range_size}, _a_file_reader_writers...);
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards,
    typename R,
    typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<
    _ABC_NS_DG::combinatorics_return_type<T>>
    using_combinatorics_generator(
        R&& _a_collection_of_elements,
        Args... _a_file_reader_writers
    )
{
    using namespace _ABC_NS_DG;
    const std::size_t _l_range_size{std::ranges::size(_a_collection_of_elements)
    };
    return using_combinatorics_generator<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards,
        R,
        Args...>(
        std::forward<R>(_a_collection_of_elements),
        {0, _l_range_size},
        _a_file_reader_writers...
    );
}

template <
    typename T,
    bool Generate_Permutations,
    bool Allow_Duplicates,
    bool Generate_Elements_Forwards,
    typename R,
    typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<
    _ABC_NS_DG::combinatorics_return_type<T>>
    using_combinatorics_generator(
        R&&                                       _a_collection_of_elements,
        const _ABC_NS_DG::combinatorics_schema_t& _a_combinations_sizes,
        Args... _a_file_reader_writers
    )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    const vector<T> _l_normalised_elements{normalise_elements<Allow_Duplicates>(
        std::forward<R>(_a_collection_of_elements)
    )};
    return make_data_generator_with_file_support<combinatorics_data_generator_t<
        T,
        Generate_Permutations,
        Allow_Duplicates,
        Generate_Elements_Forwards>>(
        combinatorics_data_generator_t<
            T,
            Generate_Permutations,
            Allow_Duplicates,
            Generate_Elements_Forwards>(
            _l_normalised_elements,
            normalise_size(_a_combinations_sizes, _l_normalised_elements.size())
        ),
        _a_file_reader_writers...
    );
}

__constexpr _ABC_NS_DG::combinatorics_schema_t
            of_size(
                const std::size_t _a_element
            ) noexcept
{
    using namespace _ABC_NS_DG;
    return {_a_element, _a_element};
}

__constexpr _ABC_NS_DG::combinatorics_schema_t
            of_sizes_m_to_n(
                const std::size_t _a_element_l,
                const std::size_t _a_element_r
            ) noexcept
{
    return {
        std::min(_a_element_l, _a_element_r),
        std::max(_a_element_l, _a_element_r)
    };
}

_END_ABC_NS

_BEGIN_ABC_NS
#if 0
template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
enumerate_data(
    const std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>& _a_es,
    const std::shared_ptr<_ABC_NS_DG::enumeration_data_object_t<T>>&
    _a_enumerate_base,
    Args... _a_file_reader_writers
)
{
    using namespace _ABC_NS_DG;
    return make_data_generator_with_file_support<
        enumeration_data_generator_t<T>>(
            enumeration_data_generator_t<T>(_a_es, _a_enumerate_base),
            _a_file_reader_writers...
            );
}

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
enumerate_data(
    const std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>& _a_es,
    Args... _a_file_reader_writers
)
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return make_data_generator_with_file_support<
        enumeration_data_generator_t<T>>(
            enumeration_data_generator_t<T>(
                _a_es, make_shared<enumeration_data_object_t<T>>()
            ),
            _a_file_reader_writers...
            );
}

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
enumerate_data(
    const std::shared_ptr<_ABC_NS_DG::enumeration_data_object_t<T>>&
    _a_enumerate_base,
    Args... _a_file_reader_writers
)
{
    using namespace _ABC_NS_DG;
    return make_data_generator_with_file_support<
        enumeration_data_generator_t<T>>(
            enumeration_data_generator_t<T>(all_values<T>(), _a_enumerate_base),
            _a_file_reader_writers...
            );
}

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
enumerate_data(
    Args... _a_file_reader_writers
)
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return enumerate_data<T>(
        make_shared<enumeration_data_object_t<T>>(), _a_file_reader_writers...
    );
}
#endif
_END_ABC_NS