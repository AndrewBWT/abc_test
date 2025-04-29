#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <charconv>
#include <concepts>
#include <string>

_BEGIN_ABC_UTILITY_NS
/*!
 * @brief This concept describes a container which can have elements added on to
 * its "back" - that is, appended past its last entity.
 */
template <typename T>
concept has_back_inserter_c
    = requires (T& _a_range, std::ranges::range_value_t<T> _a_element) {
          { _a_range.push_back(_a_element) };
      };
/*!
 * @brief This concept describes a container which can have elements added on to
 * its "front" - that is, appended before its first entity.
 */
template <typename T>
concept has_front_inserter_c
    = requires (T& _a_range, std::ranges::range_value_t<T> _a_element) {
          { _a_range.push_front(_a_element) };
      };
/*!
 * @brief This concept describes a container which can have elements added to it
 * - that is, some generic appending to the object. Internally, the object may
 * maintain its own ordering on objects.
 */
template <typename T>
concept has_unordered_inserter_c = requires (
    T&                            _a_range,
    std::ranges::range_value_t<T> _a_element,
    std::ranges::iterator_t<T>    _a_range_itt
) {
    { _a_range.insert(_a_range_itt, _a_element) };
};
template <typename R>
concept has_inserter_c
    = (has_back_inserter_c<R> || has_unordered_inserter_c<R> || has_front_inserter_c<R>);
template<typename R1, typename R2>
concept has_append_range_c = std::ranges::range<R1> && std::ranges::range<R2> && requires (R1 & _a_range1, R2 && _a_range2)
{
    { _a_range1.append_range(_a_range2) };
};
/*!
 * @brief Concept for an entity that can be built from the std::from_chars
 * function.
 *
 * Note that bool cannot be built using this, as well as wchar_t, char8_t,
 * char16_t and char32_t.
 */
template <typename T>
concept from_chars_convertable_c = requires (const char* _a_char, T _a_object) {
    {
        std::from_chars(_a_char, _a_char, _a_object)
    } -> std::same_as<std::from_chars_result>;
};
/*!
 * @brief Concept for an entity which can be converted using the function
 * std::to_chars.
 */
template <typename T>
concept to_chars_convertable_c = requires (T& _a_object, char* _a_char) {
    {
        std::to_chars(_a_char, _a_char, _a_object)
    } -> std::same_as<std::to_chars_result>;
};
_END_ABC_UTILITY_NS