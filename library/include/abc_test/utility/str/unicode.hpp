#pragma once

#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/internal/unreachable.hpp"
#include "abc_test/utility/str/concepts.hpp"
#include "abc_test/utility/str/printing_utilities.hpp"
#include "abc_test/utility/types.hpp"

#include <array>
#include <string>

_BEGIN_ABC_NS
/*!
 * @brief This function converts a unicode string to an ASCII string.
 *
 * If any of the characters are outside the ASCII range, then the function
 * returns an appropriate error message. Otherwise, it returns the constructed
 * std::string.
 *
 * @tparam T The character type of the input string.
 * @param _a_str The input string.
 * @return Eitehr the output string, or an appropriate failure message.
 */
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr result_t<std::string>
    convert_unicode_to_ascii(const std::basic_string<T>& _a_str) noexcept;
/*!
 * @brief This function converts a unicode string to an ASCII string.
 *
 * If any of the characters are outside the ASCII range, then the function
 * returns an appropriate error message. Otherwise, it returns the constructed
 * std::string. Same as previous function, except this takes a string_view as
 * the argument.
 *
 * @tparam T The character type of the input string.
 * @param _a_str The input string.
 * @return Eitehr the output string, or an appropriate failure message.
 */
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr result_t<std::string>
    convert_unicode_to_ascii(const std::basic_string_view<T> _a_str) noexcept;
/*!
 * @brief Converts an ASCII string to unicode.
 *
 * If any of the characters from the input are outside the basic ASCII range,
 * this function will return an appropriate error message.
 *
 * @tparam T The type of character type to use in the output.
 * @param _a_str The input std::string object.
 * @return Either a std::basic_string<T> representing _a_str, or an appropriate
 * error message.
 */
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr result_t<std::basic_string<T>>
            convert_ascii_to_unicode(const std::string& _a_str) noexcept;
/*!
 * @brief Converts an ASCII string to unicode.
 *
 * If any of the characters from the input are outside the basic ASCII range,
 * this function will return an appropriate error message.
 *
 * Same as the function above except this takes an std::string_view as its
 * argument.
 *
 * @tparam T The type of character type to use in the output.
 * @param _a_str The input std::string_view object.
 * @return Either a std::basic_string<T> representing _a_str, or an appropriate
 * error message.
 */
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr result_t<std::basic_string<T>>
            convert_ascii_to_unicode(const std::string_view _a_str) noexcept;

/*!
 * @brief Converts a unicode string from one format to another.
 *
 * If the input unicode string is invalid, then this function returns expected
 * value with an error mesage in it. Otherwise, it returns the converted string.
 *
 * @tparam T The internal character type of the output argument.
 * @tparam U The internal character type of the input argument.
 * @param _a_str The input argument.
 * @return An expected value containing either the converted output, or an error
 * message.
 */
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr result_t<std::basic_string<T>>
            unicode_conversion(const std::basic_string_view<U> _a_str) noexcept;
/*!
 * @brief Converts a unicode string from one format to another.
 *
 * If the input unicode string is invalid, then this function returns expected
 * value with an error mesage in it. Otherwise, it returns the converted string.
 *
 * Same as the above function, except it takes a reference of a
 * std::basic_string.
 *
 * @tparam T The internal character type of the output argument.
 * @tparam U The internal character type of the input argument.
 * @param _a_str The input argument.
 * @return An expected value containing either the converted output, or an error
 * message.
 */
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr result_t<std::basic_string<T>>
            unicode_conversion(const std::basic_string<U>& _a_str) noexcept;
/*!
 * @brief Converts a unicode string from one format to another.
 *
 * Will throw an exception if there is an error with the input unicode encoding.
 *
 * @tparam T The character type of the output basic string.
 * @tparam U The character type of the input std::basic_string.
 * @param _a_str The string entity to convert.
 * @return The converted unicode string.
 */
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr std::basic_string<T>
    unicode_conversion_with_exception(const std::basic_string_view<U> _a_str);
/*!
 * @brief Converts a unicode string from one format to another.
 *
 * Will throw an exception if there is an error with the input unicode encoding.
 *
 * @tparam T The character type of the output basic string.
 * @tparam U The character type of the input std::basic_string.
 * @param _a_str The string entity to convert.
 * @return The converted unicode string.
 */
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr std::basic_string<T>
    unicode_conversion_with_exception(const std::basic_string<U>& _a_str);
/*!
 * @brief Converts a unicode character from one format to another formatted
 * unicode string.
 *
 * Will throw an exception if there is an error with the input unicode encoding.
 *
 * @tparam T The character type of the output basic string.
 * @tparam U The character type of the input character.
 * @param _a_str The character entity to convert.
 * @return The converted unicode string.
 */
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr std::basic_string<T>
            unicode_conversion_with_exception(const U _a_char);
/*!
 * @brief Checks if a unicode string is valid.
 *
 * @param _a_str The string to check.
 * @return True if _a_str is valid, false otherwise.
 */
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_unicode_string(const std::basic_string_view<T> _a_str) noexcept;
/*!
 * @brief Checks if a unicode string is valid.
 *
 * @param _a_str The string to check.
 * @return True if _a_str is valid, false otherwise.
 */
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_unicode_string(const std::basic_string<T>& _a_str) noexcept;
/*!
 * @brief Checks if a character is valid unicode on its own.
 *
 * That is to say, if its a char8, checks that it is in the ascii range. If
 * char16_t, that it is not a surrogate character, and a char32_t that it is
 * valid.
 *
 * @param _a_str The character to check
 * @return True if _a_char is valid, false otherwise.
 */
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_char(const T _a_char) noexcept;
/*!
 * @brief Checks if a character is valid ascii.
 *
 * @param _a_char The character to check.
 * @return True if _a_char is valid, false otherwise.
 */
template <typename T>
requires is_char_type_c<T>
__constexpr bool
    is_valid_ascii_char(const T _a_char) noexcept;
/*!
 * @brief Gets the next character from a stream of characters.
 *
 * This function assumes _a_itt and _a_end are part of the same iterator.
 *
 * @tparam T The type of the iterators _a_itt and _a_end.
 * @tparam Return_Reason A boolean paramter which controls the return type; if
 * its true, then the return type is an std::expected of pair of char32_t and
 * size. If false an optional of pair of char32_t and size. The char32_t is the
 * next char32_t character from the stream, and the size is the number of
 * elements of the iterator _a_itt which were read to create this chracter.
 * @param _a_itt The current iterator.
 * @param _a_end The end iterator.
 * @return See above.
 */
template <bool Return_Reason, typename T>
requires char_type_is_unicode_c<typename T::value_type>
__constexpr std::conditional_t<
    Return_Reason,
    result_t<std::pair<char32_t, std::size_t>>,
    std::optional<std::pair<char32_t, std::size_t>>>
    next_char32_t(const T _a_begin, const T _a_itt, const T _a_end) noexcept;
/*!
 * @brief Converts an arbitrary unicode string to a u8string.
 *
 * This function assumes that the input string is a valid unicode string.
 *
 * @tparam CharT The character type of the input string.
 * @param _a_str The string to convert.
 * @return The converted string.
 */
template <typename CharT>
__constexpr std::u8string
    unicode_string_to_u8string(const std::basic_string_view<CharT> _a_str
    ) noexcept;
/*!
 * @brief Converts an arbitrary unicode string to a u8string.
 *
 * This function assumes that the input string is a valid unicode string.
 *
 * @tparam CharT The character type of the input string.
 * @param _a_str The string to convert.
 * @return The converted string.
 */
template <typename CharT>
__constexpr std::u8string
    unicode_string_to_u8string(const std::basic_string<CharT>& _a_str) noexcept;
/*!
 * @brief Converts an arbitrary unicode character to a u8string.
 *
 * This function assumes that the character is a valid unicode string.
 *
 * @tparam CharT The character type of the input character.
 * @param _a_str The character to convert.
 * @return The converted character in string form.
 */
template <typename CharT>
__constexpr std::u8string
            unicode_char_to_u8string(const CharT _a_char) noexcept;

/*!
 * @brief Gets the specified constant.
 *
 * Equal to 0x7F.
 *
 * @tparam T The return type paramter.
 * @return The encoded constant.
 */
template <typename T>
requires is_char_type_c<T> && (sizeof(T) >= 1)
__constexpr T ascii_limit() noexcept;
/*!
 * @brief Gets the specified constant.
 *
 * Equal to 0xFFFF.
 *
 * @return The encoded constant.
 */

template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T single_char16_limit_and_three_char8_limit() noexcept;
/*!
 * @brief Gets the specified constant.
 *
 * Equal to 0x7FF.
 *
 * @tparam T The return type paramter.
 * @return The encoded constant.
 */
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T two_char8_limit() noexcept;
/*!
 * @brief Gets the specified constant.
 *
 * Equal to 0x10'FFFF.
 *
 * @tparam T The return type paramter.
 * @return The encoded constant.
 */
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 4)
__constexpr T char32_limit() noexcept;
/*!
 * @brief Gets the specified constant.
 *
 * Equal to 0xD800.
 *
 * @tparam T The return type paramter.
 * @return The encoded constant.
 */
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T high_surrogate_lower_value() noexcept;
/*!
 * @brief Gets the specified constant.
 *
 * Equal to 0xDFFF.
 *
 * @tparam T The return type paramter.
 * @return The encoded constant.
 */
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T low_surrogate_upper_value() noexcept;

namespace detail
{
__constexpr std::optional<std::u8string>
    if_invalid_u32string_show_reason(const std::u32string_view _a_str) noexcept;

template <bool Return_u32string>
__constexpr
result_t<std::conditional_t<Return_u32string, std::u32string, std::monostate>>
    validate_u8string_and_convert_to_u32string(const std::u8string_view _a_str
    ) noexcept;

template <bool Return_u32string>
__constexpr
result_t<std::conditional_t<Return_u32string, std::u32string, std::monostate>>
    validate_u16string_and_convert_to_u32string(const std::u16string_view _a_str
    ) noexcept;
template <typename T>
requires is_wchar_and_32_bit_c<T> || std::same_as<T, char32_t>
__constexpr std::optional<std::u8string>
            if_invalid_char32_show_reason(const T _a_char) noexcept;
__constexpr void
    add_char32_to_u8string(
        const char32_t                                         _a_char,
        std::back_insert_iterator<std::basic_string<char8_t>>& _a_inserter
    ) noexcept;
template <bool Return_Reason, typename T>
requires char_type_is_unicode_c<typename T::value_type>
__constexpr
std::conditional_t<Return_Reason, result_t<char32_t>, std::optional<char32_t>>
    next_char32_t_and_increment_iterator(
        const T& _a_begin,
        T&       _a_itt,
        const T  _a_end
    ) noexcept;

template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 4)
__constexpr T char16_offset_for_char32_conversion() noexcept;

template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T high_surrogate_upper_value() noexcept;

template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T low_surrogate_lower_value() noexcept;
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr bool is_surrogate(const T _a_char) noexcept;
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr bool is_high_surrogate(const T _a_char) noexcept;

template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr bool is_low_surrogate(const T _a_char) noexcept;
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr std::u8string
            convert_string_view_to_hex_vector(const std::basic_string<T>& _a_str
            ) noexcept;
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr std::u8string
    convert_string_view_to_hex_vector(const std::basic_string_view<T> _a_str
    ) noexcept;
} // namespace detail

_END_ABC_NS

_BEGIN_ABC_NS
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr_imp result_t<std::string>
                convert_unicode_to_ascii(
                    const std::basic_string<T>& _a_str
                ) noexcept
{
    using namespace std;
    return convert_unicode_to_ascii(basic_string_view<T>(_a_str));
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr_imp result_t<std::string>
                convert_unicode_to_ascii(
                    const std::basic_string_view<T> _a_str
                ) noexcept
{
    using namespace std;
    using ArgType = std::basic_string_view<T>;
    using CharT   = typename ArgType::value_type;
    using itt     = typename ArgType::const_iterator;
    itt    _l_itt = _a_str.begin();
    itt    _l_end = _a_str.end();
    string _l_output;
    auto   _l_back_inserter(back_inserter(_l_output));
    for (size_t _l_idx{1}; _l_itt != _l_end; ++_l_idx)
    {
        // Gets each char32_t from the string view.
        const result_t<char32_t> _l_char_res{
            detail::next_char32_t_and_increment_iterator<true>(
                _a_str.begin(), _l_itt, _l_end
            )
        };
        // If it sa valid unicode character.
        if (_l_char_res.has_value())
        {
            const auto& _l_char{_l_char_res.value()};
            // If doesn't fit within ASCII.
            if (not (is_valid_ascii_char(_l_char)))
            {
                return unexpected(fmt::format(
                    u8"The function convert_unicode_to_ascii "
                    u8"could not encode the character '{0}' into basic "
                    u8"ASCII, which is the encoding the function uses to "
                    u8"encode std::string objects. This error occoured "
                    u8"when processing the {1} character in the unicode "
                    u8"string \"{2}\".",
                    unicode_char_to_u8string(_l_char),
                    positive_integer_to_placement(_l_idx),
                    unicode_string_to_u8string(_a_str)
                ));
            }
            else
            {
                _l_back_inserter = static_cast<char>(_l_char);
            }
        }
        else
        {
            // Invalid unicode character.
            return unexpected(fmt::format(
                u8"The function convert_unicode_to_ascii "
                u8"encountered an error when processing "
                u8"the input \"{0}\". The error encountered is as follows; {1}",
                unicode_string_to_u8string(_a_str),
                _l_char_res.error()
            ));
        }
    }
    return _l_output;
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr result_t<std::basic_string<T>>
            convert_ascii_to_unicode(
                const std::string& _a_str
            ) noexcept
{
    using namespace std;
    return convert_ascii_to_unicode<T>(string_view(_a_str));
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr result_t<std::basic_string<T>>
            convert_ascii_to_unicode(
                const std::string_view _a_str
            ) noexcept
{
    using namespace std;
    using namespace std;
    using ResultType  = std::basic_string<T>;
    using CharT       = typename ResultType::value_type;
    using itt         = typename string_view::const_iterator;
    // Use utf8cpp to iterate over code points
    itt        _l_itt = _a_str.begin();
    itt        _l_end = _a_str.end();
    ResultType _l_output;
    auto       _l_back_inserter(back_inserter(_l_output));
    for (size_t _l_idx{1}; _l_itt != _l_end; ++_l_idx)
    {
        const char _l_char{*_l_itt};
        if (not is_valid_ascii_char(_l_char))
        {
            return unexpected(fmt::format(
                u8"The function convert_ascii_to_unicode could not "
                u8"encode the char represented by the integer {0} into "
                u8"unicode. This is because it lies outside the basic "
                u8"ASCII range of 0 to 127. It is probable that this "
                u8"character is representable in your local character set, "
                u8"however convert_ascii_to_unicode only recognises "
                u8"basic ASCII, and therefore this character cannot be "
                u8"encoded into a unicode string representative of the "
                u8"input given.",
                static_cast<uint8_t>(_l_char)
            ));
        }
        else
        {
            _l_back_inserter = static_cast<T>(_l_char);
            ++_l_itt;
        }
    }
    return _l_output;
}

template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr result_t<std::basic_string<T>>
            unicode_conversion(
                const std::basic_string_view<U> _a_str
            ) noexcept
{
    using namespace std;
    // Function converts an already valid u32string to a u16string.
    auto _l_convert_u32string_to_u16string
        = [](const u32string_view _a_str_2) -> u16string
    {
        u16string _l_rv;
        auto      _l_back_inserter{std::back_inserter(_l_rv)};
        auto      _l_end{std::end(_a_str_2)};
        for (auto _l_itt{std::begin(_a_str_2)}; _l_itt != _l_end; ++_l_itt)
        {
            const char32_t _l_char{*_l_itt};
            // Encode as UTF-16
            if (_l_char
                <= single_char16_limit_and_three_char8_limit<char32_t>())
            {
                // BMP (Basic Multilingual Plane)
                _l_back_inserter = static_cast<char16_t>(_l_char);
            }
            else if (_l_char <= char32_limit<char32_t>())
            {
                // Supplementary Plane → surrogate pair
                // Moves the character from the range 0x10000 to 0x10FFFF to
                // 0... 0xFFFFF.
                const char32_t _l_char_cpy{
                    _l_char
                    - detail::char16_offset_for_char32_conversion<char32_t>()
                };
                _l_back_inserter = static_cast<char16_t>(
                    (_l_char_cpy >> 10) + high_surrogate_lower_value<char32_t>()
                );
                _l_back_inserter = static_cast<char16_t>(
                    (_l_char_cpy & 0b0011'1111'1111)
                    + detail::low_surrogate_lower_value<char32_t>()
                );
            }
            else
            {
                std::unreachable();
            }
        }
        return _l_rv;
    };
    // Function converts an already valid u32string to a valid u8string.
    auto _l_convert_u32string_to_u8string = [](const u32string_view _a_str_2)
    {
        u8string _l_rv;
        auto     _l_back_inserter{std::back_inserter(_l_rv)};
        for (const char32_t _l_char : _a_str_2)
        {
            detail::add_char32_to_u8string(_l_char, _l_back_inserter);
        }
        return _l_rv;
    };
    // Runs the code for converting a UTF16 string to any other unicode string
    // type.
    auto _l_run_char16 = [&](const std::basic_string_view<char16_t> _a_str_2
                         ) -> result_t<std::basic_string<T>>
    {
        // If the output is UTF16, then only validates the input. Otherwise,
        // either converts and validates a UTF16 to UTF32, or reports the error.
        auto _l_result{detail::validate_u16string_and_convert_to_u32string<
            not (same_as<char16_t, T>)>(_a_str_2)};
        if constexpr (same_as<char8_t, T>)
        {
            // Converts UTF32 to UTF8
            return _l_result.transform(
                [&](const auto _a_str_3)
                {
                    return _l_convert_u32string_to_u8string(_a_str_3);
                }
            );
        }
        else if constexpr (same_as<char16_t, T>)
        {
            // Either reports invalid UTF16 string, or returns the original
            // stringif its valid.
            return _l_result.transform(
                [&](const std::monostate _a_monostate) -> std::u16string
                {
                    return u16string(_a_str_2);
                }
            );
        }
        // Just return the result if UTF32.
        else if constexpr (same_as<char32_t, T>)
        {
            return _l_result;
        }
        else if constexpr (same_as<wchar_t, T>)
        {
            if constexpr (wchar_is_16_bit)
            {
                if (_l_result.has_value())
                {
                    return unpack_wstring(_a_str_2);
                }
                else
                {
                    return unexpected(_l_result.error());
                }
            }
            else if constexpr (wchar_is_32_bit)
            {
                return _l_result.transform(
                    [](const auto _a_str_3)
                    {
                        unpack_wstring(_a_str_3);
                    }
                );
            }
            else
            {
                __STATIC_ASSERT(
                    T,
                    "unicode_conversion valid for wchar_t, however Invalid for "
                    "this "
                    "wchar_t size."
                );
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion invalid for this character type"
            );
        }
    };
    // Contains the logic for converting UTF32 into all other unicode types.
    auto _l_run_char32 = [&](const std::basic_string_view<char32_t> _a_str_2
                         ) -> result_t<std::basic_string<T>>
    {
        // Checks the input is valid.
        const optional<u8string> _l_result{
            detail::if_invalid_u32string_show_reason(_a_str_2)
        };
        // If invalid, return the reason its invalid.
        if (_l_result.has_value())
        {
            return unexpected(_l_result.value());
        }
        // Otherwise convert the already validated unicode string.
        if constexpr (same_as<char8_t, T>)
        {
            return _l_convert_u32string_to_u8string(_a_str_2);
        }
        else if constexpr (same_as<char16_t, T>)
        {
            return _l_convert_u32string_to_u16string(_a_str_2);
        }
        else if constexpr (same_as<char32_t, T>)
        {
            return u32string(_a_str_2);
        }
        else if constexpr (same_as<wchar_t, T>)
        {
            if constexpr (wchar_is_16_bit)
            {
                return unpack_wstring(_l_convert_u32string_to_u16string(_a_str_2
                ));
            }
            else if constexpr (wchar_is_32_bit)
            {
                // return unpack_wstring(_a_str_2);
            }
            else
            {
                __STATIC_ASSERT(
                    T,
                    "unicode_conversion valid for wchar_t, however Invalid for "
                    "this "
                    "wchar_t size."
                );
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion invalid for this character type"
            );
        }
    };
    if constexpr (same_as<char8_t, U>)
    {
        // Converts UTF8 to all other character types.
        auto _l_result{detail::validate_u8string_and_convert_to_u32string<
            not (same_as<char8_t, T>)>(_a_str)};
        // If invalid then report why, otherwise just returns original string.
        if constexpr (same_as<char8_t, T>)
        {
            if (_l_result.has_value())
            {
                return basic_string<T>(_a_str);
            }
            else
            {
                return _l_result.error();
            }
        }
        // Converts UTF32 string to U16.
        else if constexpr (same_as<char16_t, T>)
        {
            return _l_result.transform(
                [&](auto _a_str2)
                {
                    return _l_convert_u32string_to_u16string(_a_str2);
                }
            );
        }
        else if constexpr (same_as<char32_t, T>)
        {
            return _l_result;
        }
        else if constexpr (same_as<wchar_t, T>)
        {
            if constexpr (wchar_is_16_bit)
            {
                return _l_result
                    .transform(
                        [&](auto _a_str2)
                        {
                            return _l_convert_u32string_to_u16string(_a_str2);
                        }
                    )
                    .transform(
                        [](auto _a_str2)
                        {
                            return unpack_wstring(_a_str2);
                        }
                    );
            }
            else if constexpr (wchar_is_32_bit)
            {
                return _l_result.transform(
                    [](auto _a_str2)
                    {
                        return unpack_wstring(_a_str2);
                    }
                );
            }
            else
            {
                __STATIC_ASSERT(
                    T,
                    "unicode_conversion valid for wchar_t, however Invalid for "
                    "this "
                    "wchar_t size."
                );
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion invalid for this character type"
            );
        }
    }
    else if constexpr (same_as<char16_t, U>)
    {
        return _l_run_char16(_a_str);
    }
    else if constexpr (same_as<char32_t, U>)
    {
        return _l_run_char32(_a_str);
    }
    else if constexpr (same_as<wchar_t, U>)
    {
        if constexpr (wchar_is_16_bit)
        {
            return _l_run_char16(pack_wstring(_a_str));
        }
        else if constexpr (wchar_is_32_bit)
        {
            return _l_run_char32(pack_wstring(_a_str));
        }
        else
        {
            __STATIC_ASSERT(
                T,
                "unicode_conversion valid for wchar_t, however Invalid for "
                "this "
                "wchar_t size."
            );
        }
    }
    else
    {
        __STATIC_ASSERT(
            T, "unicode_conversion invalid for this character type"
        );
    }
}

template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr_imp result_t<std::basic_string<T>>
                unicode_conversion(
                    const std::basic_string<U>& _a_str
                ) noexcept
{
    using namespace std;
    return unicode_conversion<T>(basic_string_view<U>(_a_str));
}

template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr_imp std::basic_string<T>
                unicode_conversion_with_exception(
                    const std::basic_string_view<U> _a_str
                )
{
    using namespace std;
    using namespace abc::errors;
    if (auto _l_result{unicode_conversion<T>(_a_str)}; _l_result.has_value())
    {
        return _l_result.value();
    }
    else
    {
        throw test_library_exception_t(fmt::format(
            u8"checkless_unicode_conversion failed. Input {0} "
            u8"argument \"_a_str\" contains invalid unicode "
            u8"character(s). In hex _a_str = \"{1}\".",
            type_id<basic_string<U>>(),
            detail::convert_string_view_to_hex_vector(_a_str)
        ));
    }
}

template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr_imp std::basic_string<T>
                unicode_conversion_with_exception(
                    const std::basic_string<U>& _a_str
                )
{
    using namespace std;
    return unicode_conversion_with_exception<T>(basic_string_view<U>(_a_str));
}

template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr_imp std::basic_string<T>
                unicode_conversion_with_exception(
                    const U _a_char
                )
{
    using namespace std;
    return unicode_conversion_with_exception<T>(basic_string<U>(1, _a_char));
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_unicode_string(
        const std::basic_string_view<T> _a_str
    ) noexcept
{
    using namespace std;
    if constexpr (same_as<T, wchar_t>)
    {
        return is_valid_unicode_string(pack_wstring(_a_str));
    }
    else if constexpr (same_as<T, char32_t>)
    {
        return not detail::if_invalid_u32string_show_reason(_a_str).has_value();
    }
    else if constexpr (same_as<T, char16_t>)
    {
        return detail::validate_u16string_and_convert_to_u32string<false>(_a_str
        )
            .has_value();
    }
    else if constexpr (same_as<T, char8_t>)
    {
        return detail::validate_u8string_and_convert_to_u32string<false>(_a_str)
            .has_value();
    }
    else
    {
        __STATIC_ASSERT(
            T, "is_valid_unicode_string invalid for this character type"
        );
    }
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_unicode_string(
        const std::basic_string<T>& _a_str
    ) noexcept
{
    using namespace std;
    return is_valid_unicode_string(basic_string_view<T>(_a_str));
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_char(
        const T _a_char
    ) noexcept
{
    using namespace std;
    if constexpr (same_as<T, char8_t>)
    {
        return is_valid_ascii_char(_a_char);
    }
    else if constexpr (same_as<T, char16_t> || is_wchar_and_16_bit_c<T>)
    {
        return not (detail::is_surrogate(_a_char));
    }
    else if constexpr (same_as<T, char32_t> || is_wchar_and_32_bit_c<T>)
    {
        return not detail::if_invalid_char32_show_reason(_a_char).has_value();
    }
    else
    {
        __STATIC_ASSERT(T, "is_valid_char invalid for this character type");
    }
}

template <typename T>
requires is_char_type_c<T>
__constexpr bool
    is_valid_ascii_char(
        const T _a_char
    ) noexcept
{
    if constexpr (std::signed_integral<T>)
    {
        return _a_char >= 0 && _a_char <= ascii_limit<T>();
    }
    else
    {
        return _a_char <= ascii_limit<T>();
    }
}

template <bool Return_Reason, typename T>
requires char_type_is_unicode_c<typename T::value_type>
__constexpr std::conditional_t<
    Return_Reason,
    result_t<std::pair<char32_t, std::size_t>>,
    std::optional<std::pair<char32_t, std::size_t>>>
    next_char32_t(
        const T _a_begin,
        const T _a_itt,
        const T _a_end
    ) noexcept
{
    using namespace std;
    auto _l_next_from_u8string
        = [](const T _a_begin, const T _a_itt, const T _a_end)
        -> std::conditional_t<
            Return_Reason,
            result_t<std::pair<char32_t, std::size_t>>,
            std::optional<std::pair<char32_t, std::size_t>>>
    {
        auto       _l_itt{_a_itt};
        const auto _l_byte_1{*_l_itt};
        if (not (is_valid_ascii_char(_l_byte_1)))
        {
            constexpr array<char32_t, 3> _l_and_array{
                0b0001'1111, 0b0000'1111, 0b0000'0111
            };
            constexpr array<pair<char8_t, char8_t>, 3>
                _l_byte_1_and_equal_values{
                    {{0b1110'0000, 0b1100'0000},
                     {0b1111'0000, 0b1110'0000},
                     {0b1111'1000, 0b1111'0000}}
            };
            size_t _l_code_point_size{_l_byte_1_and_equal_values.size()};
            for (size_t _l_idx{0}; _l_idx < _l_byte_1_and_equal_values.size();
                 ++_l_idx)
            {
                const auto& [_l_and_val, _l_equal_val]{
                    _l_byte_1_and_equal_values[_l_idx]
                };
                if ((_l_byte_1 & _l_and_val) == _l_equal_val)
                {
                    _l_code_point_size = _l_idx;
                    break;
                }
            }
            if (_l_code_point_size == _l_byte_1_and_equal_values.size())
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(fmt::format(
                        u8"the {0} char8_t code unit encountered, represented "
                        u8"in hex as {1}, was found to be invalid under the "
                        u8"UTF8 standard. An attempt was made to recognise it "
                        u8"as a leading code unit as part of a unicode code "
                        u8"point, however this was not possible. "
                        u8"To be a leading code unit, a code unit "
                        u8"must be inclusively between either {2} and "
                        u8"{3}, {4} and {5}, {6} and {7}, or "
                        u8"{8} and {9}. {1} did not meet "
                        u8"this criteria.",
                        positive_integer_to_placement(
                            std::distance(_a_begin, _l_itt) + 1
                        ),
                        represent_char_as_hex_for_output(_l_byte_1),
                        represent_char_as_hex_for_output(char8_t(0x00)),
                        represent_char_as_hex_for_output(char8_t(0x7F)),
                        represent_char_as_hex_for_output(char8_t(0xC0)),
                        represent_char_as_hex_for_output(char8_t(0xDF)),
                        represent_char_as_hex_for_output(char8_t(0xE0)),
                        represent_char_as_hex_for_output(char8_t(0xEF)),
                        represent_char_as_hex_for_output(char8_t(0xF0)),
                        represent_char_as_hex_for_output(char8_t(0xF7))
                    ));
                }
                else
                {
                    return nullopt;
                }
            }
            char32_t _l_code_point{
                static_cast<char32_t>(_l_byte_1)
                & (_l_and_array[_l_code_point_size])
            };
            if (std::distance(_l_itt, _a_end) <= _l_code_point_size + 1)
            {
                if constexpr (Return_Reason)
                {
                    const std::size_t _l_code_points_remaining{
                        static_cast<size_t>(std::distance(_l_itt, _a_end))
                    };
                    return unexpected(fmt::format(
                        u8"the {0} char8_t code unit encountered, represented "
                        u8"in hex as {1}, was found to be invalid under the "
                        u8"UTF8 standard. It was processed as a leading code "
                        u8"unit, in an attempt to process a unicode code "
                        u8"point, however this was not possible. When "
                        u8"processed as a leading code unit, {1} should be "
                        u8"the first of {2} code units making up the code "
                        u8"point. "
                        u8"However the sequence of code units ends after {3} "
                        u8"code unit{4}, meaning that it is part of an invalid "
                        u8"code unit sequence.",
                        positive_integer_to_placement(
                            std::distance(_a_begin, _l_itt) + 1
                        ),
                        represent_char_as_hex_for_output(_l_byte_1),
                        _l_code_point_size + 2,
                        _l_code_points_remaining,
                        (_l_code_points_remaining > 1 ? u8"s" : u8"")
                    ));
                }
                else
                {
                    return nullopt;
                }
            }
            for (size_t _l_idx{0}; _l_idx <= _l_code_point_size; ++_l_idx)
            {
                ++_l_itt;
                const auto _l_byte_n{*_l_itt};
                if ((_l_byte_n & 0b1100'0000) != 0b1000'0000)
                {
                    if constexpr (Return_Reason)
                    {
                        auto _l_itt_limit{
                            _l_itt + (_l_code_point_size - _l_idx)
                        };
                        u8string _l_sequence_as_hex{u8"["};
                        for (auto _l_jtt{std::prev(_l_itt, _l_idx + 1)};
                             _l_jtt <= _l_itt_limit;
                             ++_l_jtt)
                        {
                            _l_sequence_as_hex.append(
                                represent_char_as_hex_for_output(*_l_jtt)
                            );
                            if (_l_jtt + 1 <= _l_itt_limit)
                            {
                                _l_sequence_as_hex.append(u8",");
                            }
                        }
                        _l_sequence_as_hex.append(u8"]");
                        return unexpected(fmt::format(
                            u8"the {0} char8_t code unit encountered, "
                            u8"represented in hex as {1}, was found to be "
                            u8"invalid under the UTF8 standard. It was "
                            u8"processed as a continuation code unit, as part "
                            u8"of an attempt to process the sub-sequence {4} "
                            u8"as a unicode code point, "
                            u8"however this was not possible. Continuation "
                            u8"code units must be inclusively between {2} and "
                            u8"{3}. {1} "
                            u8"is not, therefore meaning that it is part of an "
                            u8"invalid code unit sequence.",
                            positive_integer_to_placement(
                                std::distance(_a_begin, _l_itt) + 1
                            ),
                            represent_char_as_hex_for_output(_l_byte_n),
                            represent_char_as_hex_for_output(char8_t(0x80)),
                            represent_char_as_hex_for_output(char8_t(0xBF)),
                            _l_sequence_as_hex
                        ));
                    }
                    else
                    {
                        return nullopt;
                    }
                }
                // Shifts the bit into the code point. It shifts by
                // 6 due to each byte in a UTF8 string only using
                // the last 6 bits. The logical and isolates these 6
                // bits.
                _l_code_point
                    = (_l_code_point << 6) | (_l_byte_n & 0b0011'1111);
            }
            constexpr array<char32_t, 3> _l_code_point_limits{
                0b1000'0000, 0b1000'0000'0000, 0b0001'0000'0000'0000'0000
            };
            if (_l_code_point < _l_code_point_limits[_l_code_point_size])
            {
                if constexpr (Return_Reason)
                {
                    const u8string _l_smaller_char8_sequence{
                        unicode_char_to_u8string(_l_code_point)
                    };
                    return unexpected(fmt::format(
                        u8"when attempting to "
                        u8"process the {0} through to the {1} char8_t code "
                        u8"unit as a single unicode code point, an error was "
                        u8"detected under the UTF8 standard. This error was "
                        u8"found when processing the "
                        u8"code unit sub-sequence {2}. After "
                        u8"converting it to the single UTF32 code unit `{3}' "
                        u8"({4} in hex), it was found that `{3}' can be "
                        u8"represented using the smaller sub-sequence of "
                        u8"char8_t code units {5}. Therefore, this code "
                        u8"unit sub-sequence is invalid.",
                        positive_integer_to_placement(
                            std::distance(_a_begin, _a_itt) + 1
                        ),
                        positive_integer_to_placement(
                            std::distance(_a_begin, _l_itt) + 1
                        ),
                        detail::convert_string_view_to_hex_vector(
                            basic_string_view<typename T::value_type>(
                                _a_itt, _l_itt + 1
                            )
                        ),
                        _l_smaller_char8_sequence,
                        represent_char_as_hex_for_output(_l_code_point),
                        detail::convert_string_view_to_hex_vector(
                            _l_smaller_char8_sequence
                        )

                    ));
                }
                else
                {
                    return nullopt;
                }
            }
            if (auto _l_invalid_char_reason{
                    detail::if_invalid_char32_show_reason(_l_code_point)
                };
                _l_invalid_char_reason.has_value())
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(fmt::format(
                        u8"when attempting to process the {0} through to the "
                        u8"{1} char8_t code unit as a single unicode code "
                        u8"point, an error was detected under the UTF8 "
                        u8"standard. This error was found when processing the "
                        u8"code unit sub-sequence {2}. After "
                        u8"converting it to a single UTF32 code unit, it was "
                        u8"found to be invalid. {3} As {4} is not "
                        u8"within this range, this code unit sub-sequence is "
                        u8"invalid.",
                        positive_integer_to_placement(
                            std::distance(_a_begin, _a_itt) + 1
                        ),
                        positive_integer_to_placement(
                            std::distance(_a_begin, _l_itt) + 1
                        ),
                        detail::convert_string_view_to_hex_vector(
                            basic_string_view<typename T::value_type>(
                                _a_itt, _l_itt + 1
                            )
                        ),
                        _l_invalid_char_reason.value(),
                        represent_char_as_hex_for_output(_l_code_point)

                    ));
                }
                else
                {
                    return nullopt;
                }
            }
            else
            {
                return make_pair(_l_code_point, _l_code_point_size + 2);
            }
        }
        else
        {
            return make_pair(static_cast<char32_t>(_l_byte_1), 1);
        }
    };
    auto _l_next_from_u16_string
        = [](const T _a_begin, const T _a_itt, const T _a_end)
        -> std::conditional_t<
            Return_Reason,
            result_t<std::pair<char32_t, std::size_t>>,
            std::optional<std::pair<char32_t, std::size_t>>>
    {
        using CharT = T::value_type;
        auto           _l_itt{_a_itt};
        const char16_t _l_first{static_cast<char16_t>(*_l_itt)};
        if (detail::is_high_surrogate(_l_first))
        {
            if (std::distance(_l_itt, _a_end) <= 1)
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(
                        u8"Unpaired high surrogate at end of input"
                    );
                }
                else
                {
                    return nullopt;
                }
            }
            ++_l_itt;
            const auto _l_second{static_cast<char16_t>(*_l_itt)};
            if (not detail::is_low_surrogate(_l_second))
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(
                        u8"Invalid low surrogate following high surrogate"
                    );
                }
                else
                {
                    return nullopt;
                }
            }
            // Decode surrogate pair to code point
            char32_t _l_char
                = detail::char16_offset_for_char32_conversion<char32_t>();
            _l_char
                += ((static_cast<char32_t>(_l_first)
                     - high_surrogate_lower_value<char32_t>())
                    << 10);
            _l_char
                += (static_cast<char32_t>(_l_second)
                    - detail::low_surrogate_lower_value<char32_t>());
            return make_pair(_l_char, 2);
        }
        else if (detail::is_low_surrogate(_l_first))
        {
            // Unpaired low surrogate
            if constexpr (Return_Reason)
            {
                return unexpected(u8"Unpaired low surrogate");
            }
            else
            {
                return nullopt;
            }
        }
        else
        {
            return make_pair(static_cast<char32_t>(_l_first), 1);
        }
    };
    using CharT = typename T::value_type;
    if (_a_itt > _a_end)
    {
        if constexpr (Return_Reason)
        {
            return unexpected(u8"Unpaired low surrogate");
        }
        else
        {
            return nullopt;
        }
    }
    else if (_a_itt == _a_end)
    {
        if constexpr (Return_Reason)
        {
            return unexpected(u8"Unpaired low surrogate");
        }
        else
        {
            return nullopt;
        }
    }
    else
    {
        if constexpr (same_as<CharT, char8_t>)
        {
            return _l_next_from_u8string(_a_begin, _a_itt, _a_end);
        }
        else if constexpr (same_as<CharT, char16_t>
                           || is_wchar_and_16_bit_c<CharT>)
        {
            return _l_next_from_u16_string(_a_begin, _a_itt, _a_end);
        }
        else if constexpr (same_as<CharT, char32_t>
                           || is_wchar_and_32_bit_c<CharT>)
        {
            const CharT _l_char{*_a_itt};
            if (auto _l_next_char{detail::if_invalid_char32_show_reason(_l_char)
                };
                _l_next_char.has_value())
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(_l_next_char.value());
                }
                else
                {
                    return nullopt;
                }
            }
            else
            {
                return make_pair(_l_char, 1);
            }
        }
        else
        {
            __STATIC_ASSERT(T, "next_char32_t invalid for this character type");
        }
    }
}

template <typename CharT>
__constexpr std::u8string
            unicode_string_to_u8string(
                const std::basic_string_view<CharT> _a_str
            ) noexcept
{
    using namespace std;
    using arg_type_t = basic_string<CharT>;
    u8string _l_rv{};
    auto     _l_itt{std::begin(_a_str)};
    auto     _l_end{std::end(_a_str)};
    auto     _l_back_inserter{std::back_inserter(_l_rv)};
    while (_l_itt != _l_end)
    {
        const optional<char32_t> _l_char_opt{
            detail::next_char32_t_and_increment_iterator<false>(
                std::begin(_a_str), _l_itt, _l_end
            )
        };
        if (_l_char_opt.has_value())
        {
            detail::add_char32_to_u8string(
                _l_char_opt.value(), _l_back_inserter
            );
        }
        else
        {
            _l_rv.append(represent_char_as_hex_for_printing(*_l_itt));
            ++_l_itt;
        }
    }
    return _l_rv;
}

template <typename CharT>
__constexpr std::u8string
            unicode_string_to_u8string(
                const std::basic_string<CharT>& _a_str
            ) noexcept
{
    using namespace std;
    return unicode_string_to_u8string(basic_string_view<CharT>(_a_str));
}

template <typename CharT>
__constexpr_imp std::u8string
                unicode_char_to_u8string(
                    const CharT _a_char
                ) noexcept
{
    using namespace std;
    return unicode_string_to_u8string(basic_string<CharT>(1, _a_char));
}

template <typename T>
requires is_char_type_c<T>
         && (
             sizeof(T) >= 1
         )
__constexpr T ascii_limit() noexcept
{
    return T{0x7F};
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T single_char16_limit_and_three_char8_limit() noexcept
{
    return 0xFFFF;
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T two_char8_limit() noexcept
{
    return 0x7FF;
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 4
         )
__constexpr T char32_limit() noexcept
{
    return T{0x10'FFFF};
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T high_surrogate_lower_value() noexcept
{
    return T{0xD800};
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T low_surrogate_upper_value() noexcept
{
    return T{0xDFFF};
}

namespace detail
{
__constexpr std::optional<std::u8string>
            if_invalid_u32string_show_reason(
                const std::u32string_view _a_str
            ) noexcept
{
    using namespace std;
    for (const char32_t _l_char : _a_str)
    {
        if (auto _l_validated_char{if_invalid_char32_show_reason(_l_char)};
            _l_validated_char.has_value())
        {
            return _l_validated_char.value();
        }
    }
    return nullopt;
}

template <bool Return_u32string>
__constexpr
result_t<std::conditional_t<Return_u32string, std::u32string, std::monostate>>
    validate_u8string_and_convert_to_u32string(
        const std::u8string_view _a_str
    ) noexcept
{
    using namespace std;
    conditional_t<Return_u32string, u32string, monostate> _l_rv;
    auto _l_end{std::end(_a_str)};
    auto _l_begin{std::begin(_a_str)};
    for (auto _l_itt{_l_begin}; _l_itt != _l_end;)
    {
        auto _l_result{
            next_char32_t_and_increment_iterator<true>(_l_begin, _l_itt, _l_end)
        };
        if (_l_result.has_value())
        {
            if constexpr (Return_u32string)
            {
                _l_rv.push_back(_l_result.value());
            }
        }
        else
        {
            return unexpected(_l_result.error());
        }
    }
    return _l_rv;
}

template <bool Return_u32string>
__constexpr
result_t<std::conditional_t<Return_u32string, std::u32string, std::monostate>>
    validate_u16string_and_convert_to_u32string(
        const std::u16string_view _a_str
    ) noexcept
{
    using namespace std;
    conditional_t<Return_u32string, u32string, monostate> _l_rv;
    auto _l_end{std::end(_a_str)};
    auto _l_begin{std::begin(_a_str)};
    for (auto _l_itt{_l_begin}; _l_itt != _l_end;)
    {
        auto _l_result{
            next_char32_t_and_increment_iterator<true>(_l_begin, _l_itt, _l_end)
        };
        if (_l_result.has_value())
        {
            if constexpr (Return_u32string)
            {
                _l_rv.push_back(_l_result.value());
            }
        }
        else
        {
            return unexpected(_l_result.error());
        }
    }
    return _l_rv;
}

template <typename T>
requires is_wchar_and_32_bit_c<T> || std::same_as<T, char32_t>
__constexpr std::optional<std::u8string>
            if_invalid_char32_show_reason(
                const T _a_char
            ) noexcept
{
    using namespace std;
    if (is_surrogate<T>(_a_char) || _a_char > char32_limit<T>())
    {
        return fmt::format(
            u8"The UTF32 "
            u8"code unit in question is represented in hex as {0}, "
            u8"and is outside the unicode range. "
            u8"The unicode range is inclusively from "
            u8"{1} to {2}, and from {3} to {4}.",
            represent_char_as_hex_for_output(_a_char),
            represent_char_as_hex_for_output(static_cast<char32_t>(0x0)),
            represent_char_as_hex_for_output<T>(
                high_surrogate_lower_value<T>() - T(0x01)
            ),
            represent_char_as_hex_for_output<T>(
                low_surrogate_upper_value<T>() + T(0x01)
            ),
            represent_char_as_hex_for_output(char32_limit<T>())
        );
    }
    else
    {
        return nullopt;
    }
}

__constexpr void
    add_char32_to_u8string(
        const char32_t                                         _a_char,
        std::back_insert_iterator<std::basic_string<char8_t>>& _a_inserter
    ) noexcept
{
    constexpr char8_t _l_final_six_bits_set{0b0011'1111};
    constexpr char8_t _l_first_four_bits_set{0b1111'0000};
    constexpr char8_t _l_first_three_bits_set{0b1110'0000};
    constexpr char8_t _l_first_bit_set{0b1000'0000};
    if (is_valid_ascii_char(_a_char))
    {
        // 1-byte UTF-8
        _a_inserter = static_cast<char8_t>(_a_char);
    }
    else if (_a_char <= two_char8_limit<char32_t>())
    {
        // 2-byte UTF-8.
        // Isolate first 6 bits by rshift, then use binary or to set the other
        // bits.
        _a_inserter = static_cast<char8_t>(0b1100'0000 | (_a_char >> 6));
        // Isolate first 6 bits by binary and, then use binary or to set the
        // other bit.
        _a_inserter = static_cast<char8_t>(
            _l_first_bit_set | (_a_char & _l_final_six_bits_set)
        );
    }
    else if (_a_char <= single_char16_limit_and_three_char8_limit<char32_t>())
    {
        // 3-byte UTF-8
        _a_inserter
            = static_cast<char8_t>(_l_first_three_bits_set | (_a_char >> 12));
        _a_inserter = static_cast<char8_t>(
            _l_first_bit_set | ((_a_char >> 6) & _l_final_six_bits_set)
        );
        _a_inserter = static_cast<char8_t>(
            _l_first_bit_set | (_a_char & _l_final_six_bits_set)
        );
    }
    else if (_a_char <= char32_limit<char32_t>())
    {
        // 4-byte UTF-8
        _a_inserter
            = static_cast<char>(_l_first_four_bits_set | (_a_char >> 18));
        _a_inserter = static_cast<char>(
            _l_first_bit_set | ((_a_char >> 12) & _l_final_six_bits_set)
        );
        _a_inserter = static_cast<char>(
            _l_first_bit_set | ((_a_char >> 6) & _l_final_six_bits_set)
        );
        _a_inserter = static_cast<char>(
            _l_first_bit_set | (_a_char & _l_final_six_bits_set)
        );
    }
    else
    {
        std::unreachable();
    }
}

template <bool Return_Reason, typename T>
requires char_type_is_unicode_c<typename T::value_type>
__constexpr
std::conditional_t<Return_Reason, result_t<char32_t>, std::optional<char32_t>>
    next_char32_t_and_increment_iterator(
        const T& _a_begin,
        T&       _a_itt,
        const T  _a_end
    ) noexcept
{
    using namespace std;
    auto _l_next_char32_t_result{
        next_char32_t<Return_Reason>(_a_begin, _a_itt, _a_end)
    };
    // If it sa valid unicode character.
    if (_l_next_char32_t_result.has_value())
    {
        const auto& [_l_char, _l_size]{_l_next_char32_t_result.value()};
        std::advance(_a_itt, _l_size);
        return _l_char;
    }
    else
    {
        if constexpr (Return_Reason)
        {
            return unexpected(_l_next_char32_t_result.error());
        }
        else
        {
            return nullopt;
        }
    }
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 4
         )
__constexpr T char16_offset_for_char32_conversion() noexcept
{
    return T{0x1'0000};
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T high_surrogate_upper_value() noexcept
{
    return T{0xDBFF};
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T low_surrogate_lower_value() noexcept
{
    return T{0xDC00};
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr bool is_surrogate(
    const T _a_char
) noexcept
{
    return _a_char >= high_surrogate_lower_value<T>()
           && _a_char <= low_surrogate_upper_value<T>();
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr bool is_high_surrogate(
    const T _a_char
) noexcept
{
    return _a_char >= high_surrogate_lower_value<T>()
           && _a_char <= high_surrogate_upper_value<T>();
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr bool is_low_surrogate(
    const T _a_char
) noexcept
{
    return _a_char >= low_surrogate_lower_value<T>()
           && _a_char <= low_surrogate_upper_value<T>();
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr std::u8string
            convert_string_view_to_hex_vector(
                const std::basic_string<T>& _a_str
            ) noexcept
{
    using namespace std;
    return convert_string_view_to_hex_vector(basic_string_view<T>(_a_str));
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr std::u8string
            convert_string_view_to_hex_vector(
                const std::basic_string_view<T> _a_str
            ) noexcept
{
    using namespace std;
    u8string _l_rv{u8"["};
    auto     _l_itt{std::begin(_a_str)};
    auto     _l_size{std::size(_a_str)};
    for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
    {
        auto _l_char{*_l_itt};
        _l_rv.append(represent_char_as_hex_for_output(_l_char));
        if (_l_idx + 1 < _l_size)
        {
            _l_rv.append(u8",");
        }
        ++_l_itt;
    }
    _l_rv.append(u8"]");
    return _l_rv;
}
} // namespace detail

_END_ABC_NS