#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <concepts>
#include <string>

_BEGIN_ABC_UTILITY_STR_NS
/*!
 * @brief This concept identifies a wchar_t type which is 4 bytes long - 32
 * bits.
 */
constexpr bool wchar_is_16_bit = (sizeof(wchar_t) == 2);
constexpr bool wchar_is_32_bit = (sizeof(wchar_t) == 4);
template <typename T>
concept is_wchar_and_32_bit_c = std::same_as<T, wchar_t> && wchar_is_32_bit;
/*!
 * @brief This concept identifies a wchar_t type which is 2 bytes long - 16
 * bits.
 */
template <typename T>
concept is_wchar_and_16_bit_c = std::same_as<T, wchar_t> && wchar_is_16_bit;
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

namespace detail
{
template <typename T>
struct string_like_underlying_char_type_object_t;

template <typename CharT, std::size_t N>
requires is_char_type_c<CharT>
struct string_like_underlying_char_type_object_t<CharT[N]>
{
    using type = CharT;
};

template <typename CharT>
requires is_char_type_c<CharT>
struct string_like_underlying_char_type_object_t<CharT*>
{
    using type = CharT;
};
template <typename CharT>
    requires is_char_type_c<CharT>
struct string_like_underlying_char_type_object_t<const CharT*>
{
    using type = CharT;
};

template <typename CharT, typename Traits, typename Alloc>
requires is_char_type_c<CharT>
struct string_like_underlying_char_type_object_t<
    std::basic_string<CharT, Traits, Alloc>>
{
    using type = CharT;
};

template <typename CharT, typename Traits>
requires is_char_type_c<CharT>
struct string_like_underlying_char_type_object_t<
    std::basic_string_view<CharT, Traits>>
{
    using type = CharT;
};
} // namespace detail

/*!
 * @brief Type synonym which gets a string-like objects underlying string type.
 *
 * It is specialised for basic_string, basic_string_view, char arrays and
 * char*'s.
 *
 * In turn, the internal type is only defined for char_type_is_unicode_c - that
 * is, the character types.
 *
 * @tparam T
 */
template <typename T>
using string_like_underlying_char_type_t =
    typename detail::string_like_underlying_char_type_object_t<
        std::remove_cvref_t<T>>::type;

/*!
 * @brief Concept that ensures the object is an underlying string-like object.
 */
template <typename T>
concept has_string_like_underlying_type_c
    = requires { typename string_like_underlying_char_type_t<T>; };
/*!
 * @brief Checks that all arguments are convertable to a string, and that they
 * have a common type. Requires atleast one element in the parameter pack.
 */
template <typename... Args>
concept all_string_like_with_same_char_c = requires {
    typename std::common_type_t<string_like_underlying_char_type_t<Args>...>;
} && (std::convertible_to<Args, std::basic_string_view<std::common_type_t<string_like_underlying_char_type_t<Args>...>>> && ...);

namespace detail
{
template <typename T>
struct char_underlying_type_object_t
{
    using type = void;
};

template <>
struct char_underlying_type_object_t<char>
{
    using type = char8_t;
};

template <>
struct char_underlying_type_object_t<char8_t>
{
    using type = uint8_t;
};

template <>
struct char_underlying_type_object_t<char16_t>
{
    using type = uint16_t;
};

template <>
struct char_underlying_type_object_t<char32_t>
{
    using type = uint32_t;
};

template <typename T>
requires is_wchar_and_16_bit_c<T>
struct char_underlying_type_object_t<T>
{
    using type = uint16_t;
};

template <typename T>
requires is_wchar_and_32_bit_c<T>
struct char_underlying_type_object_t<T>
{
    using type = uint32_t;
};
} // namespace detail

/*!
 * @brief Gets a characters underlying type.
 *
 * Only defined for char, char8_t, char16_t, char32_t and wchar_t.
 * @tparam T
 */
template <typename T>
using char_underlying_type_t = typename detail::char_underlying_type_object_t<
    std::remove_cvref_t<T>>::type;
/*!
 * @brief Checks that a character has a convertable type inside it.
 */
template <typename T>
concept has_char_underlying_type_c
    = requires { typename char_underlying_type_t<T>; };
_END_ABC_UTILITY_STR_NS
