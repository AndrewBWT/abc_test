#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"

_BEGIN_ABC_UTILITY_NS

template <typename T>
struct min_value_t
{
    __constexpr T
        min_value() const noexcept;
};
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
template <typename T>
__constexpr_imp T
min_value_t<T>::min_value() const noexcept
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "min_value_t<T>::min_value."
    );
}

_END_ABC_UTILITY_NS