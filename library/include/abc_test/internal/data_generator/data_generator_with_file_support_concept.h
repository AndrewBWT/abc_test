#pragma once
#include "abc_test/internal/utility/internal/macros.h"

#include <concepts>

_BEGIN_ABC_DG_NS
template <typename T>
concept concept_for_data_generator_with_file_support = requires (
    const T&                _a_t,
    T&                      _a_t_non_const,
    const T::tertiary_type& _a_tertiary_data
) {
    { _a_t.has_current_element() } -> std::same_as<bool>;
    { _a_t_non_const.generate_next() } -> std::same_as<bool>;
    {
        _a_t.current_element()
    } -> std::same_as<const typename T::generator_type&>;
    { _a_t.tertiary_data() } -> std::same_as<const typename T::tertiary_type&>;
    { _a_t_non_const.set_generator_using_tertiary_data(_a_tertiary_data) };
    { _a_t_non_const.reset() };
};
_END_ABC_DG_NS