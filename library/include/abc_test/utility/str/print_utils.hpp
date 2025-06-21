#pragma once
#include "abc_test/external/utfcpp/utf8.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/string_cast.hpp"

#include <fmt/xchar.h>
#include <string>

_BEGIN_ABC_UTILITY_STR_NS
/*!
 * @brief Returns a u8string representing a type's name. Used instead of
 * type_id(T).name(), where a u8string is required.
 * @tparam T The type to get the name of.
 * @return A u8string representing T's type.
 */
template <typename T>
__constexpr std::u8string
            type_id() noexcept;
/*!
 * @brief Returns a textual representation of a number with a suffix
 * representing its position.
 *
 * For example, "1" would return "1st".
 *
 * @tparam T The number type. Must be unsigned.
 * @param _a_number The number to get the textual representation of.
 * @return The textual representation of _a_number.
 */
template <typename T>
requires std::unsigned_integral<T>
__no_constexpr std::u8string
               positive_integer_to_placement(const T _a_number) noexcept;
/*!
 * @brief Same as above, except it works with a tempalte numeric parameter.
 * @tparam I The number to print out.
 * @return A u8string representation of I.
 */
template <std::size_t I>
__no_constexpr std::u8string
               positive_integer_to_placement() noexcept;
/*!
 * @brief Prints out a character in its hex form in the following format.
 * 
 * "\x<HEX-DIGITS-HERE>"
 * 
 * @tparam T The character parmaeter. Must be a character type.
 * @param _a_char The character in question.
 * @return A u8string representation the character in hex.
 */
template <typename T>
requires is_char_type_c<T>
__constexpr std::u8string
            represent_char_as_hex_for_printing(const T _a_char) noexcept;
/*!
 * @brief Prints out a character in its hex form in the following format.
 *
 * "0x<HEX-DIGITS-HERE>"
 *
 * @tparam T The character parmaeter. Must be a character type.
 * @param _a_char The character in question.
 * @return A u8string representation the character in hex.
 */
template <typename T>
requires is_char_type_c<T>
__constexpr std::u8string
            represent_char_as_hex_for_output(const T _a_char) noexcept;

namespace detail
{
template <typename T>
struct char_underlying_type
{
    using type = void;
};

template <>
struct char_underlying_type<char>
{
    using type = char8_t;
};

template <>
struct char_underlying_type<char8_t>
{
    using type = uint8_t;
};

template <>
struct char_underlying_type<char16_t>
{
    using type = uint16_t;
};

template <>
struct char_underlying_type<char32_t>
{
    using type = uint32_t;
};

template <typename T>
requires is_wchar_and_16_bit_c<T>
struct char_underlying_type<T>
{
    using type = uint16_t;
};

template <typename T>
requires is_wchar_and_32_bit_c<T>
struct char_underlying_type<T>
{
    using type = uint32_t;
};

template <typename T>
using char_underlying_type_t = typename char_underlying_type<T>::type;

template <typename T, bool Use_Capitols = true>
requires is_char_type_c<T>
__constexpr std::u8string
            make_hex_from_char_with_prefix(
                const T                  _a_char,
                const std::u8string_view _a_prefix
            ) noexcept;
} // namespace detail

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
template <typename T>
__constexpr_imp std::u8string
                type_id() noexcept
{
    using namespace std;
    const string_view _l_str{typeid(T).name()};
    return cast_string_to_u8string(_l_str);
}

template <typename T>
requires std::unsigned_integral<T>
__no_constexpr_imp std::u8string
                   positive_integer_to_placement(
                       const T _a_number
                   ) noexcept
{
    using namespace std;
    auto _l_suffix_func = [&]() -> u8string_view
    {
        const T _l_normalised{static_cast<T>(_a_number % T(100))};
        if (_l_normalised > 10 && _l_normalised < 14)
        {
            return u8"th";
        }
        else
        {
            switch (static_cast<T>(_a_number % T(10)))
            {
            case 1:
                return u8"st";
            case 2:
                return u8"nd";
            case 3:
                return u8"rd";
            default:
                return u8"th";
            }
        }
    };
    return fmt::format(u8"{0}{1}", _a_number, _l_suffix_func());
}

template <std::size_t I>
__no_constexpr std::u8string
               positive_integer_to_placement() noexcept
{
    return positive_integer_to_placement(I);
}

template <typename T>
requires is_char_type_c<T>
__constexpr std::u8string
            represent_char_as_hex_for_printing(
                const T _a_char
            ) noexcept
{
    return detail::make_hex_from_char_with_prefix(_a_char, u8"\\x");
}

template <typename T>
requires is_char_type_c<T>
__constexpr std::u8string
            represent_char_as_hex_for_output(
                const T _a_char
            ) noexcept
{
    return detail::make_hex_from_char_with_prefix(_a_char, u8"0x");
}

namespace detail
{
template <typename T, bool Use_Capitols>
requires is_char_type_c<T>
__constexpr std::u8string
            make_hex_from_char_with_prefix(
                const T                  _a_char,
                const std::u8string_view _a_prefix
            ) noexcept
{
    using namespace std;
    using Type_To_Cast_To = char_underlying_type_t<T>;
    u8string _l_format_specifier;
    if constexpr (Use_Capitols)
    {
        _l_format_specifier = u8"{:X}";
    }
    else
    {
        _l_format_specifier = u8"{:x}";
    }
    const u8string _l_char_as_hex{
        fmt::format(_l_format_specifier, static_cast<Type_To_Cast_To>(_a_char))
    };
    return fmt::format(
        u8"{0}{1}{2}",
        _a_prefix,
        u8string((sizeof(Type_To_Cast_To) * 2) - _l_char_as_hex.size(), u8'0'),
        _l_char_as_hex
    );
}
} // namespace detail

_END_ABC_UTILITY_STR_NS