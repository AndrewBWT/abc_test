#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <concepts>

_BEGIN_ABC_UTILITY_STR_NS
/*!
 * @brief This concept identifies a wchar_t type which is 4 bytes long - 32
 * bits.
 */
constexpr bool wchar_is_16_bit = sizeof(wchar_t) == 2;
constexpr bool wchar_is_32_bit = sizeof(wchar_t) == 4;
template <typename T>
concept is_wchar_and_32_bit_c
    = std::same_as<T, wchar_t> && wchar_is_32_bit;
/*!
 * @brief This concept identifies a wchar_t type which is 2 bytes long - 16
 * bits.
 */
template <typename T>
concept is_wchar_and_16_bit_c
    = std::same_as<T, wchar_t> && wchar_is_16_bit;
/*!
 * @brief This concept identifies a unicode-like type - everything but char's.
 */
template <typename T>
concept char_type_is_unicode_c
    = std::same_as<T, char8_t> || std::same_as<T, char16_t>
      || std::same_as<T, char32_t> || std::same_as<wchar_t, T>;
/*!
 * @brief This concept identifies all character-like types.
 */
template <typename T>
concept is_char_type_c = char_type_is_unicode_c<T> || std::same_as<T, char>;
_END_ABC_UTILITY_STR_NS