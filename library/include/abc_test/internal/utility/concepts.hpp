#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"
#include <concepts>
#include <charconv>

_BEGIN_ABC_UTILITY_NS
template <typename T>
concept is_from_chars_convertable_c
= requires (const char* _a_char, T _a_object) {
    {
        std::from_chars(_a_char, _a_char, _a_object)
    } -> std::same_as<std::from_chars_result>;
};

template<typename T>
concept is_to_string_convertable_c
= requires(T _a_element)
{
    { std::to_string(_a_element) } -> std::same_as<std::string>;
};

template <typename>
struct is_tuple : std::false_type
{};

template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type
{};

template <typename>
struct is_pair : std::false_type
{};

template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type
{};
_END_ABC_UTILITY_NS