#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <charconv>
#include <concepts>
#include <string>

_BEGIN_ABC_UTILITY_NS
template <typename T>
concept has_back_inserter_c
= requires (T & _a_range, std::ranges::range_value_t<T> _a_element) {
    { _a_range.push_back(_a_element) };
};
template <typename T>
concept has_front_inserter_c
= requires (T & _a_range, std::ranges::range_value_t<T> _a_element) {
    { _a_range.push_front(_a_element) };
};
template <typename T>
concept has_inserter_c
= requires (T & _a_range, std::ranges::range_value_t<T> _a_element,
    std::ranges::iterator_t<T> _a_range_itt) {
        { _a_range.insert(_a_range_itt, _a_element) };
};
template <typename T>
concept sized_and_reservable_range_c
    = std::ranges::sized_range<T>
      && requires (T& _a_range, const std::ranges::range_size_t<T> _a_size) {
             _a_range._a_range(_a_size);
             {
                 _a_range.capacity()
             } -> std::same_as<std::ranges::range_size_t<T>>;
             {
                 _a_range.max_size()
             } -> std::same_as<std::ranges::range_size_t<T>>;
         };
template <typename T>
concept is_from_chars_convertable_c
    = requires (const char* _a_char, T _a_object) {
          {
              std::from_chars(_a_char, _a_char, _a_object)
          } -> std::same_as<std::from_chars_result>;
      };

template <typename T>
concept is_to_string_convertable_c = requires (T _a_element) {
    { std::to_string(_a_element) } -> std::same_as<std::string>;
};
template <typename T>
concept range_allocatable_c = requires (typename T::size_type _a_size) {
    { T(_a_size) };
};
_END_ABC_UTILITY_NS
#if 0
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
#endif