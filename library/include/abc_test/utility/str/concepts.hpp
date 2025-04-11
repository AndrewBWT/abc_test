#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <concepts>

_BEGIN_ABC_NS
template <typename T>
concept is_wchar_and_32_bit_c
    = std::same_as<T, wchar_t> && sizeof(wchar_t) == 4;
template <typename T>
concept is_wchar_and_16_bit_c
    = std::same_as<T, wchar_t> && sizeof(wchar_t) == 2;
template <typename T>
concept char_type_is_unicode_c
    = std::same_as<T, char8_t> || std::same_as<T, char16_t>
      || std::same_as<T, char32_t> || is_wchar_and_32_bit_c<T>
      || is_wchar_and_16_bit_c<T>;
_END_ABC_NS