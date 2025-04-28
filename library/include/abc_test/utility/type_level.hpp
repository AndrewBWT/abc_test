#pragma once

#include "abc_test/utility/internal/macros.hpp"

#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <utility>

_BEGIN_ABC_UTILITY_NS
template <typename... Ts, typename F>
constexpr void
for_each_type_lambda(
    F&& f
)
{
    (f.template operator() < Ts > (), ...);
}

template <typename... Ts>
struct type_list
{
};

// --- Size of Type List ---

template <typename List>
struct type_list_size;

template <typename... Ts>
struct type_list_size<type_list<Ts...>>
    : std::integral_constant<std::size_t, sizeof...(Ts)>
{
};

template <typename List>
inline constexpr std::size_t type_list_size_v = type_list_size<List>::value;

// --- Pick N-th Type ---

template <std::size_t I, typename List>
struct pick_type;

template <std::size_t I, typename T, typename... Ts>
struct pick_type<I, type_list<T, Ts...>> : pick_type<I - 1, type_list<Ts...>>
{
};

template <typename T, typename... Ts>
struct pick_type<0, type_list<T, Ts...>>
{
    using type = T;
};

template <std::size_t I, typename List>
using pick_type_t = typename pick_type<I, List>::type;

// --- for_each_type over Type List ---

template<typename List, typename Func, std::size_t... Is>
constexpr void for_each_type_indexed(Func&& func, std::index_sequence<Is...>)
{
    (func.template operator() < pick_type_t<Is, List> > (), ...);
}

template <typename List, typename Func>
constexpr void
for_each_type(
    Func&& func
)
{
    constexpr std::size_t N = type_list_size_v<List>;
    for_each_type_indexed<List>(
        std::forward<Func>(func), std::make_index_sequence<N>{}
    );
}
_END_ABC_UTILITY_NS