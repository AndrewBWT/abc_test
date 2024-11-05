#pragma once
#include "abc_test/internal/utility/limits/min_value_object.h"
#include "abc_test/internal/utility/limits/max_value_object.h"

_BEGIN_ABC_UTILITY_NS
template <typename T>
concept has_limits_c = (not std::is_enum_v<T>) && requires () {
    { std::numeric_limits<T>::is_specialized == true };
};

template <typename T>
    requires has_limits_c<T>
struct min_value_t<T>
{
    __constexpr T
        min_value() const noexcept
    {
        return std::numeric_limits<T>::min();
    }
};

template <typename T>
    requires has_limits_c<T>
struct max_value_t<T>
{
    __constexpr T
        max_value() const noexcept
    {
        return std::numeric_limits<T>::max();
    }
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
/*template <typename T>
    requires has_limits_c<T>
__constexpr_imp T
min_value_t<T>::min_value() const noexcept
{
    return std::numeric_limits<T>::min();
}

template <typename T>
    requires has_limits_c<T>
__constexpr_imp T
max_value_t<T>::max_value() const noexcept
{
    return std::numeric_limits<T>::max();
}*/
_END_ABC_UTILITY_NS