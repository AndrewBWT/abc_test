#pragma once
#include "abc_test/utility/limits/max_value_object.hpp"
#include "abc_test/utility/limits/min_value_object.hpp"
#include <optional>

_BEGIN_ABC_UTILITY_NS

template <typename T, std::size_t N>
requires min_value_c<T>
struct min_value_t<std::array<T, N>>
{
    __constexpr_imp std::array<T, N>
                    min_value() const noexcept;
};

template <typename T>
struct min_value_t<std::optional<T>>
{
    __constexpr_imp std::optional<T>
        min_value() const noexcept
    {
        return std::nullopt;
    }
};

template <typename... Ts>
requires (min_value_c<Ts> && ...)
struct min_value_t<std::tuple<Ts...>>
{
    __constexpr std::tuple<Ts...>
                min_value() const noexcept;
private:
    template<std::size_t I>
    __constexpr void
        initialise_tuple(std::tuple<Ts...>& _a_tuple) const noexcept;
};

template <typename T, std::size_t N>
requires max_value_c<T>
struct max_value_t<std::array<T, N>>
{
    __constexpr_imp std::array<T, N>
                    max_value() const noexcept;
};
template <typename T>
    requires max_value_c<T>
struct max_value_t<std::optional<T>>
{
    __constexpr_imp std::optional<T>
        max_value() const noexcept
    {
        return std::make_optional(max_value_t<T>{}.max_value());
    }
};
template <typename... Ts>
    requires (max_value_c<Ts> && ...)
struct max_value_t<std::tuple<Ts...>>
{
    __constexpr std::tuple<Ts...>
        max_value() const noexcept;
private:
    template<std::size_t I>
    __constexpr void
        initialise_tuple(std::tuple<Ts...>& _a_tuple) const noexcept;
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
    for (size_t _l_idx{0}; _l_idx < N; ++_l_idx)
    {
        _l_rv[_l_idx] = min_value_t<T>().min_value();
    }
    return _l_rv;
}

template <typename... Ts>
requires (min_value_c<Ts> && ...)
__constexpr_imp std::tuple<Ts...>
                min_value_t<std::tuple<Ts...>>::min_value() const noexcept
{
    using namespace std;
    tuple<Ts...> _l_rv;
    initialise_tuple<0>(_l_rv);
    return _l_rv;
}
template <typename... Ts>
    requires (min_value_c<Ts> && ...)
template<std::size_t I>
__constexpr void
min_value_t<std::tuple<Ts...>>::initialise_tuple(std::tuple<Ts...>& _a_tuple) const noexcept
{
    using namespace std;
    auto& _l_elem{ get<I>(_a_tuple) };
    _l_elem = min_value_t<tuple_element_t<I, tuple<Ts...>>>{}.min_value();
    if constexpr (I + 1 < tuple_size<tuple<Ts...>>{})
    {
        initialise_tuple<I + 1>(_a_tuple);
    }
}

template <typename T, std::size_t N>
requires max_value_c<T>
__constexpr_imp std::array<T, N>
                max_value_t<std::array<T, N>>::max_value() const noexcept
{
    using namespace std;
    array<T, N> _l_rv;
    for (size_t _l_idx{0}; _l_idx < N; ++_l_idx)
    {
        _l_rv[_l_idx] = max_value_t<T>().max_value();
    }
    return _l_rv;
}
template <typename... Ts>
    requires (max_value_c<Ts> && ...)
__constexpr_imp std::tuple<Ts...>
max_value_t<std::tuple<Ts...>>::max_value() const noexcept
{
    using namespace std;
    tuple<Ts...> _l_rv;
    initialise_tuple<0>(_l_rv);
    return _l_rv;
}
template <typename... Ts>
    requires (max_value_c<Ts> && ...)
template<std::size_t I>
__constexpr void
max_value_t<std::tuple<Ts...>>::initialise_tuple(std::tuple<Ts...>& _a_tuple) const noexcept
{
    using namespace std;
    auto& _l_elem{ get<I>(_a_tuple) };
    _l_elem = max_value_t<tuple_element_t<I, tuple<Ts...>>>{}.max_value();
    if constexpr (I + 1 < tuple_size<tuple<Ts...>>{})
    {
        initialise_tuple<I + 1>(_a_tuple);
    }
}
_END_ABC_UTILITY_NS