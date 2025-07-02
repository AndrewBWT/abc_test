#pragma once
#include "abc_test/utility/limits/min_value_object.hpp"
#include "abc_test/utility/limits/max_value_object.hpp"
#include <limits>

_BEGIN_ABC_UTILITY_NS
template <typename T>
concept has_limits_c = (not std::is_enum_v<T>) && std::numeric_limits<T>::is_specialized == true;
// Primary template: triggers error if not specialized or matched by concept
template <typename T>
    requires has_limits_c<T>
struct min_value_t<T>
{
    static_assert(std::is_same_v<T, size_t> || true, "min_value_t<size_t> seen");
    __constexpr T
        min_value() const noexcept
    {
        return std::numeric_limits<T>::min();
    }
};
/*template <>
struct min_value_t<size_t>
{
    constexpr size_t min_value() const noexcept {
        return std::numeric_limits<size_t>::min();
    }
};*/
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