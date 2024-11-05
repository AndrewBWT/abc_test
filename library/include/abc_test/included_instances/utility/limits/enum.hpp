#pragma once
#include "abc_test/internal/utility/limits/min_value_object.hpp"
#include "abc_test/internal/utility/limits/max_value_object.hpp"

_BEGIN_ABC_UTILITY_NS

template <typename T>
    requires _ABC_NS_UTILITY::enum_has_list_c<T>
struct min_value_t<T>
{
    __constexpr T
        min_value() const noexcept;
};

template <typename T>
    requires _ABC_NS_UTILITY::enum_has_list_c<T>
struct max_value_t<T>
{
    __constexpr_imp T
        max_value() const noexcept;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
template <typename T>
    requires _ABC_NS_UTILITY::enum_has_list_c<T>
__constexpr_imp T
min_value_t<T>::min_value() const noexcept
{
    return get_thread_local_enumerate_enum_helper<T>().min();
}

template <typename T>
    requires _ABC_NS_UTILITY::enum_has_list_c<T>
__constexpr_imp T
max_value_t<T>::max_value() const noexcept
{
    return get_thread_local_enumerate_enum_helper<T>().max();
}
_END_ABC_UTILITY_NS