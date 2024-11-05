#pragma once
#include "abc_test/internal/utility/limits/min_value_object.h"
#include "abc_test/internal/utility/limits/max_value_object.h"

_BEGIN_ABC_UTILITY_NS

template <typename T, std::size_t N>
    requires min_value_c<T>
struct min_value_t<std::array<T, N>>
{
    __constexpr_imp std::array<T, N>
        min_value() const noexcept;
};

template <typename T, std::size_t N>
    requires max_value_c<T>
struct max_value_t<std::array<T, N>>
{
    __constexpr_imp std::array<T, N>
        max_value() const noexcept;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

template <typename T, std::size_t N>
    requires min_value_c<T>
__constexpr_imp std::array<T, N>
min_value_t<std::array<T, N>>::min_value() const noexcept
{
    using namespace std;
    array<T, N> _l_rv;
    for (size_t _l_idx{ 0 }; _l_idx < N; ++_l_idx)
    {
        _l_rv[_l_idx] = min_value_t<T>().min_value();
    }
    return _l_rv;
}

template <typename T, std::size_t N>
    requires max_value_c<T>
__constexpr_imp std::array<T, N>
max_value_t<std::array<T, N>>::max_value() const noexcept
{
    using namespace std;
    array<T, N> _l_rv;
    for (size_t _l_idx{ 0 }; _l_idx < N; ++_l_idx)
    {
        _l_rv[_l_idx] = max_value_t<T>().max_value();
    }
    return _l_rv;
}
_END_ABC_UTILITY_NS