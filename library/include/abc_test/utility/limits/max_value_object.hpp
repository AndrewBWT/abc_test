#pragma once
#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_UTILITY_NS

template <typename T>
struct max_value_t
{
    __constexpr max_value_t()
    {

    }
    __constexpr T
        max_value() const noexcept;
};
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
template <typename T>
__constexpr_imp T
max_value_t<T>::max_value() const noexcept
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "max_value_t<T>::max_value."
    );
}

_END_ABC_UTILITY_NS