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
    next_char32_t(const T _a_itt, const T _a_end) noexcept;
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
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr result_t<std::basic_string<T>>
            unicode_conversion_internal(const std::basic_string_view<U> _a_str
            ) noexcept;
__constexpr std::optional<std::u8string>
    if_invalid_u32string_show_reason(const std::u32string_view _a_str) noexcept;

template <bool Return_u32string>
__constexpr
result_t<std::conditional_t<Return_u32string, std::u32string, std::monostate>>
    validate_u8string_and_convert_to_u32string(const std::u8string_view _a_str
    ) noexcept;

template <bool Return_u32string, typename Original_Type>
requires char_type_is_unicode_c<Original_Type>
__constexpr
result_t<std::conditional_t<Return_u32string, std::u32string, std::monostate>>
    validate_u16string_and_convert_to_u32string(const std::u16string_view _a_str
    ) noexcept;
template <typename T>
requires is_wchar_and_32_bit_c<T> || std::same_as<T, char32_t>
__constexpr bool
    is_invalid_char32(const T _a_char) noexcept;
template <typename T>
requires is_wchar_and_32_bit_c<T> || std::same_as<T, char32_t>
__constexpr std::optional<std::u8string>
            if_invalid_char32_show_reason(const T _a_char) noexcept;
__constexpr void
    add_char32_to_u8string(
        const char32_t                                         _a_char,
        std::back_insert_iterator<std::basic_string<char8_t>>& _a_inserter
    ) noexcept;
template <
    bool Return_Reason,
    bool Is_Substring,
    typename T,
    typename Original_Value_Type>
requires char_type_is_unicode_c<typename T::value_type>
         && char_type_is_unicode_c<Original_Value_Type>
__constexpr std::conditional_t<
    Return_Reason,
    result_t<std::pair<char32_t, std::size_t>>,
    std::optional<std::pair<char32_t, std::size_t>>>
    next_char32_t_internal(const T _a_begin, const T _a_itt, const T _a_end)
        noexcept;
template <bool Return_Reason, typename T, typename Original_Type>
requires char_type_is_unicode_c<typename T::value_type>
         && char_type_is_unicode_c<Original_Type>
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
requires char_type_is_unicode_c<T>
__constexpr T
    zero() noexcept;
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T one_below_high_surrogate_lower_value() noexcept;
template <typename T>
requires char_type_is_unicode_c<T> && (sizeof(T) >= 2)
__constexpr T one_above_low_surrogate_upper_value() noexcept;
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

template <typename T, bool Part_Of_Sequence>
requires char_type_is_unicode_c<T>
__constexpr std::u8string
            unicode_error_description() noexcept;

template <typename CharT, bool Is_Substring>
__constexpr std::u8string
            single_code_unit_error_description(
                const size_t             _a_index,
                const std::u8string_view _a_char
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
        const auto _l_char{*_l_itt};
        // Do this as quicker than finding error. Error handling runs through
        // slower next_char32_t_and_increment_iterator.
        if (is_valid_ascii_char(_l_char))
        {
            _l_back_inserter = _l_char;
            ++_l_itt;
        }
        else
        {
            // Gets the char32_t. There is some redundancy in using this
            // function (there are checks for ascii in next_char32_t) however as
            // this is not the "hot path" we don't think it really matters.
            const result_t<char32_t> _l_char_res{
                detail::next_char32_t_and_increment_iterator<true, itt, CharT>(
                    _a_str.begin(), _l_itt, _l_end
                )
            };
            if (_l_char_res.has_value())
            {
                const auto& _l_char32t{_l_char_res.value()};
                return unexpected(fmt::format(
                    u8"The function convert_unicode_to_ascii encountered an "
                    u8"error when processing the unicode input \"{0}\". The "
                    u8"error encountered is as follows; the {1} char32_t code "
                    u8"point extracted from the unicode input string, "
                    u8"representing the character `{2}' (hex {3}), "
                    u8"was found to be outside the basic ASCII range - that "
                    u8"is, it was not inclusively within the range {4} and "
                    u8"{5}.",
                    unicode_string_to_u8string(_a_str),
                    positive_integer_to_placement(_l_idx),
                    unicode_char_to_u8string(_l_char32t),
                    represent_char_as_hex_for_output(_l_char32t),
                    represent_char_as_hex_for_output(detail::zero<char32_t>()),
                    represent_char_as_hex_for_output(ascii_limit<char32_t>())
                ));
            }
            else
            {
                // Invalid unicode character.
                return unexpected(fmt::format(
                    u8"The function convert_unicode_to_ascii "
                    u8"encountered an error when processing "
                    u8"the unicode input \"{0}\". The error encountered is as "
                    u8"follows; {1}",
                    unicode_string_to_u8string(_a_str),
                    _l_char_res.error()
                ));
            }
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
            u8string _l_str_as_u8string{};
            ranges::for_each(
                _a_str,
                [&](const char _a_char)
                {
                    if (is_valid_ascii_char(_a_char))
                    {
                        _l_str_as_u8string.push_back(_a_char);
                    }
                    else
                    {
                        _l_str_as_u8string.append(
                            represent_char_as_hex_for_printing(_a_char)
                        );
                    }
                }
            );
            return unexpected(fmt::format(
                u8"The function convert_ascii_to_unicode "
                u8"encountered an error when processing "
                u8"the ASCII input \"{0}\". The error encountered is as "
                u8"follows; the {1} char type extracted from the unicode input "
                u8"string, "
                u8"represented in hex as {2}, lies outside the ASCII range. "
                u8"The ASCII range "
                u8"is inclusively between {3} and {4}; therefore, it was not "
                u8"possible to "
                u8"encode this char in unicode.",
                _l_str_as_u8string,
                positive_integer_to_placement(_l_idx),
                represent_char_as_hex_for_output(_l_char),
                represent_char_as_hex_for_output(detail::zero<char>()),
                represent_char_as_hex_for_output(char(ascii_limit<char>()))
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
    return detail::unicode_conversion_internal<T>(_a_str).or_else(
        [&](const u8string _a_error) -> result_t<basic_string<T>>
        {
            return unexpected(fmt::format(
                u8"The function unicode_conversion encountered an error when "
                u8"processing the unicode input \"{0}\". The error "
                u8"encountered is as follows; {1}",
                unicode_string_to_u8string(_a_str),
                _a_error
            ));
        }
    );
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
    if (auto _l_result{detail::unicode_conversion_internal<T>(_a_str)};
        _l_result.has_value())
    {
        return _l_result.value();
    }
    else
    {
        throw test_library_exception_t(fmt::format(
            u8"The function unicode_conversion_with_exception threw an "
            u8"exception due to an error encountered when processing the "
            u8"unicode input \"{0}\". The error encountered is as "
            u8"follows; {1}",
            unicode_string_to_u8string(_a_str),
            _l_result.error()
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
        return detail::validate_u16string_and_convert_to_u32string<false, T>(
                   _a_str
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
        return not detail::is_invalid_char32(_a_char);
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
        const T _a_itt,
        const T _a_end
    ) noexcept
{
    using namespace std;
    using CharT        = T::value_type;
    auto _l_error_func = [&]() -> u8string
    {
        return fmt::format(
            u8"The function next_char32_t was unable to get the next UTF32 "
            u8"code point from the unicide substring \"{0}\". This substring "
            u8"has "
            u8"been created using the iterator arguments for next_char32_t. "
            u8"next_char32_t was unable to get the next UF32 code point due to "
            u8"the following reason;",
            unicode_string_to_u8string(basic_string_view<CharT>(
                _a_itt > _a_end ? _a_end : _a_itt, _a_end
            ))
        );
    };
    if (_a_itt >= _a_end)
    {
        if constexpr (Return_Reason)
        {
            return unexpected(fmt::format(
                u8"{0} the current iterator is either equal to, or "
                u8"past, the end iterator. Therefore, no more code "
                u8"units can be processed.",
                _l_error_func()
            ));
        }
        else
        {
            return nullopt;
        }
    }
    else
    {
        auto _l_rv{
            detail::next_char32_t_internal<Return_Reason, true, T, CharT>(
                _a_itt, _a_itt, _a_end
            )
        };
        if constexpr (Return_Reason)
        {
            return _l_rv.or_else(
                [&](const u8string _a_str) -> result_t<pair<char32_t, size_t>>
                {
                    return unexpected(
                        fmt::format(u8"{0} {1}", _l_error_func(), _a_str)
                    );
                }
            );
        }
        else
        {
            return _l_rv;
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
            detail::next_char32_t_and_increment_iterator<
                false,
                decltype(_l_itt),
                CharT>(std::begin(_a_str), _l_itt, _l_end)
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
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr result_t<std::basic_string<T>>
            unicode_conversion_internal(
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
    // Runs the code for converting a UTF16 string to any other unicode
    // string type.
    auto _l_run_char16 = [&](const std::basic_string_view<char16_t> _a_str_2
                         ) -> result_t<std::basic_string<T>>
    {
        // If the output is UTF16, then only validates the input. Otherwise,
        // either converts and validates a UTF16 to UTF32, or reports the
        // error.
        auto _l_result{detail::validate_u16string_and_convert_to_u32string<
            not (same_as<char16_t, T>),
            U>(_a_str_2)};
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
                    "unicode_conversion_internal valid for wchar_t, however "
                    "Invalid "
                    "for "
                    "this "
                    "wchar_t size."
                );
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion_internal invalid for this character type"
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
                    "unicode_conversion_internal valid for wchar_t, however "
                    "Invalid "
                    "for "
                    "this "
                    "wchar_t size."
                );
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion_internal invalid for this character type"
            );
        }
    };
    if constexpr (same_as<char8_t, U>)
    {
        // Converts UTF8 to all other character types.
        auto _l_result{detail::validate_u8string_and_convert_to_u32string<
            not (same_as<char8_t, T>)>(_a_str)};
        // If invalid then report why, otherwise just returns original
        // string.
        if constexpr (same_as<char8_t, T>)
        {
            return _l_result.transform(
                [&](auto _a_monostate)
                {
                    return basic_string<T>(_a_str);
                }
            );
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
                    "unicode_conversion_internal valid for wchar_t, however "
                    "Invalid "
                    "for "
                    "this "
                    "wchar_t size."
                );
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion_internal invalid for this character type"
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
                "unicode_conversion_internal valid for wchar_t, however "
                "Invalid for "
                "this "
                "wchar_t size."
            );
        }
    }
    else
    {
        __STATIC_ASSERT(
            T, "unicode_conversion_internal invalid for this character type"
        );
    }
}

__constexpr std::optional<std::u8string>
            if_invalid_u32string_show_reason(
                const std::u32string_view _a_str
            ) noexcept
{
    using namespace std;
    for (size_t _l_idx{1}; const char32_t _l_char : _a_str)
    {
        if (auto _l_validated_char{if_invalid_char32_show_reason(_l_char)};
            _l_validated_char.has_value())
        {
            const u8string _l_char_as_str{
                represent_char_as_hex_for_output(_l_char)
            };
            return make_optional(fmt::format(
                u8"{0}. {1}",
                detail::single_code_unit_error_description<char32_t, false>(
                    _l_idx, _l_char_as_str
                ),
                _l_validated_char.value()
            ));
        }
        ++_l_idx;
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
        auto _l_result{next_char32_t_and_increment_iterator<
            true,
            decltype(_l_itt),
            char8_t>(_l_begin, _l_itt, _l_end)};
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

template <bool Return_u32string, typename Original_Type>
requires char_type_is_unicode_c<Original_Type>
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
        auto _l_result{next_char32_t_and_increment_iterator<
            true,
            decltype(_l_itt),
            Original_Type>(_l_begin, _l_itt, _l_end)};
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
__constexpr bool
    is_invalid_char32(
        const T _a_char
    ) noexcept
{
    return is_surrogate<T>(_a_char) || _a_char > char32_limit<T>();
}

template <typename T>
requires is_wchar_and_32_bit_c<T> || std::same_as<T, char32_t>
__constexpr std::optional<std::u8string>
            if_invalid_char32_show_reason(
                const T _a_char
            ) noexcept
{
    using namespace std;
    if (is_invalid_char32(_a_char))
    {
        return fmt::format(
            u8"UTF32 code units must be inclusively between either {0} "
            u8"and {1}, or {2} and {3}. Therefore, the "
            u8"code unit {4} is invalid.",
            represent_char_as_hex_for_output(detail::zero<char32_t>()),
            represent_char_as_hex_for_output<T>(
                detail::one_below_high_surrogate_lower_value<T>()
            ),
            represent_char_as_hex_for_output<T>(
                detail::one_above_low_surrogate_upper_value<T>()
            ),
            represent_char_as_hex_for_output(char32_limit<T>()),
            represent_char_as_hex_for_output(_a_char)
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
        // Isolate first 6 bits by rshift, then use binary or to set the
        // other bits.
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

template <
    bool Return_Reason,
    bool Is_Substring,
    typename T,
    typename Original_Value_Type>
requires char_type_is_unicode_c<typename T::value_type>
         && char_type_is_unicode_c<Original_Value_Type>
__constexpr std::conditional_t<
    Return_Reason,
    result_t<std::pair<char32_t, std::size_t>>,
    std::optional<std::pair<char32_t, std::size_t>>>
    next_char32_t_internal(
        const T _a_begin,
        const T _a_itt,
        const T _a_end
    ) noexcept
{
    using namespace std;
    using CharT = T::value_type;
    using rv_t  = conditional_t<
         Return_Reason,
         result_t<pair<char32_t, size_t>>,
         optional<pair<char32_t, size_t>>>;
    auto _l_multi_code_unit_description
        = [&]<bool Single_Element>(
              const size_t                   _a_begin_index,
              const basic_string_view<CharT> _a_code_units
          ) -> u8string
    {
        u8string _l_substring_str;
        if constexpr (Is_Substring)
        {
            _l_substring_str = u8"sub";
        }
        u8string     _l_names;
        const size_t _l_limit{_a_begin_index + _a_code_units.size()};
        for (size_t _l_idx{_a_begin_index}; _l_idx < _l_limit; ++_l_idx)
        {
            _l_names.append(positive_integer_to_placement(_l_idx));
            if (_l_idx + 2 < _l_limit)
            {
                _l_names.append(u8", ");
            }
            else if (_l_idx + 1 < _l_limit)
            {
                _l_names.append(u8" and ");
            }
        }
        u8string _l_code_units_as_str, _l_were_or_was_str;
        if constexpr (Single_Element)
        {
            _l_code_units_as_str
                = represent_char_as_hex_for_output(_a_code_units[0]);
            _l_were_or_was_str = u8"was";
        }
        else
        {
            _l_code_units_as_str
                = detail::convert_string_view_to_hex_vector(_a_code_units);
            _l_were_or_was_str = u8"were";
        }
        const u8string _l_units_str{_a_code_units.size() == 1 ? u8"" : u8"s"};
        return fmt::format(
            u8"the {0} {1} code unit{2} in the unicode {3}string, "
            u8"represented "
            u8"in hex as {4}, {5} {6}",
            _l_names,
            type_id<Original_Value_Type>(),
            _l_units_str,
            _l_substring_str,
            _l_code_units_as_str,
            _l_were_or_was_str,
            detail::unicode_error_description<CharT, true>()
        );
    };
    auto _l_next_from_u8string
        = [&](const T _a_begin, const T _a_itt, const T _a_end) -> rv_t
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
                    const u8string _l_char_as_str{
                        represent_char_as_hex_for_output(_l_byte_1)
                    };
                    return unexpected(fmt::format(
                        u8"{0}. An "
                        u8"attempt was made to recognise it as a leading "
                        u8"code "
                        u8"unit, as part of a unicode code point. However "
                        u8"it "
                        u8"did not meet the criteria. For a code unit to "
                        u8"be "
                        u8"recognised as a leading code unit, it must be "
                        u8"inclusively between either {1} and "
                        u8"{2}, {3} and {4}, {5} and {6}, or "
                        u8"{7} and {8}. As {9} did "
                        u8"not meet these criteria, it was recognised as "
                        u8"an "
                        u8"invalid code unit.",
                        detail::single_code_unit_error_description<
                            Original_Value_Type,
                            Is_Substring>(
                            std::distance(_a_begin, _l_itt) + 1, _l_char_as_str
                        ),
                        represent_char_as_hex_for_output(zero<char8_t>()),
                        represent_char_as_hex_for_output(ascii_limit<char8_t>()),
                        represent_char_as_hex_for_output(char8_t(0xC0)),
                        represent_char_as_hex_for_output(char8_t(0xDF)),
                        represent_char_as_hex_for_output(char8_t(0xE0)),
                        represent_char_as_hex_for_output(char8_t(0xEF)),
                        represent_char_as_hex_for_output(char8_t(0xF0)),
                        represent_char_as_hex_for_output(char8_t(0xF7)),
                        _l_char_as_str
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
            const size_t _l_code_units_processed{
                static_cast<size_t>(std::distance(_l_itt, _a_end))
            };
            if (_l_code_units_processed <= _l_code_point_size + 1)
            {
                if constexpr (Return_Reason)
                {
                    const std::size_t _l_code_points_remaining{
                        static_cast<size_t>(std::distance(_l_itt, _a_end))
                    };
                    const u8string _l_char_as_str{
                        represent_char_as_hex_for_output(_l_byte_1)
                    };
                    if (_l_code_units_processed == 1)
                    {
                        return unexpected(fmt::format(
                            u8"{0}. It was recognised as a leading code "
                            u8"unit, and should have been the 1st of {1} "
                            u8"code units, which together would have "
                            u8"represented a unicode code point. "
                            u8"However, the sequence of code units ends "
                            u8"after "
                            u8"the {2} code unit {3}, therefore meaning "
                            u8"this "
                            u8"code unit sequence is invalid.",
                            _l_multi_code_unit_description.operator()<true>(
                                std::distance(_a_begin, _l_itt) + 1,
                                basic_string_view<CharT>(_l_itt, _a_end)
                            ),
                            _l_code_point_size + 2,
                            positive_integer_to_placement(1),
                            _l_char_as_str
                        ));
                    }
                    else
                    {
                        return unexpected(fmt::format(
                            u8"{0}. {1} was recognised as a leading code "
                            u8"unit, and should have been the 1st of {2} "
                            u8"code units, which together would have "
                            u8"represented a unicode code point. "
                            u8"However, the sequence of code units ends "
                            u8"after "
                            u8"the {3} code unit {4}, therefore meaning "
                            u8"this "
                            u8"code unit sequence is invalid.",
                            _l_multi_code_unit_description.operator()<false>(
                                std::distance(_a_begin, _l_itt) + 1,
                                basic_string_view<CharT>(_l_itt, _a_end)
                            ),
                            _l_char_as_str,
                            _l_code_point_size + 2,
                            positive_integer_to_placement(
                                _l_code_points_remaining
                            ),
                            represent_char_as_hex_for_output(
                                *std::prev(_a_end, 1)
                            )
                        ));
                    }
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
                            u8"{0}. An attempt was made to recognise the "
                            u8"{1} "
                            u8"code unit in the sequence ({2}) as a "
                            u8"continuation code unit, as part of a "
                            u8"unicode "
                            u8"code point. However it did not meet the "
                            u8"criteria. For a code unit to be recognised "
                            u8"as a "
                            u8"continuation code unit, it must be "
                            u8"inclusively "
                            u8"between {3} and {4}. As {2} did not meet "
                            u8"this criteria, this code unit sequence is "
                            u8"invalid.",
                            _l_multi_code_unit_description.operator()<false>(
                                std::distance(_a_begin, _l_itt) - _l_idx,
                                basic_string_view<CharT>(
                                    std::prev(_l_itt, _l_idx + 1),
                                    std::prev(_l_itt, _l_idx + 1)
                                        + _l_code_point_size + 2
                                )
                            ),
                            positive_integer_to_placement(_l_idx + 2),
                            represent_char_as_hex_for_output(_l_byte_n),
                            represent_char_as_hex_for_output(char8_t(0x80)),
                            represent_char_as_hex_for_output(char8_t(0xBF))
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
                        u8"{0}. After converting the sequence to the "
                        u8"single "
                        u8"UTF32 "
                        u8"code unit `{1}' ({2} in hex), it was found "
                        u8"that `{1}' can be represented using the smaller "
                        u8"sequence of char8_t code units {3}. Part of the "
                        u8"unicode standard dicates that UTF8 sequence are "
                        u8"only valid if they encode unicode code points "
                        u8"using "
                        u8"the smallest possible char8_t representation. "
                        u8"Therefore, this code unit sequence is invalid.",
                        _l_multi_code_unit_description.operator()<false>(
                            std::distance(_a_begin, _l_itt)
                                - (_l_code_point_size),
                            basic_string_view<CharT>(
                                std::prev(_l_itt, _l_code_point_size + 1),
                                _l_itt + 1
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
                        u8"{0}. After converting the sequence to a single "
                        u8"UTF32 "
                        u8"code unit, it was found to be invalid. "
                        u8"The UTF32 "
                        u8"code unit in question is represented in hex as "
                        u8"{1}, "
                        u8"and is outside the unicode range. "
                        u8"The unicode range is inclusively from "
                        u8"{2} to {3}, and from {4} to {5}. "
                        u8"As {1} is not within this range, this code unit "
                        u8"sequence is invalid.",
                        _l_multi_code_unit_description.operator()<false>(
                            std::distance(_a_begin, _l_itt)
                                - (_l_code_point_size),
                            basic_string_view<CharT>(
                                std::prev(_l_itt, _l_code_point_size + 1),
                                _l_itt + 1
                            )
                        ),
                        represent_char_as_hex_for_output(_l_code_point),
                        represent_char_as_hex_for_output(
                            detail::zero<char32_t>()
                        ),
                        represent_char_as_hex_for_output<char32_t>(
                            detail::one_below_high_surrogate_lower_value<char32_t>()
                        ),
                        represent_char_as_hex_for_output<char32_t>(
                            detail::one_above_low_surrogate_upper_value<char32_t>()
                        ),
                        represent_char_as_hex_for_output(char32_limit<char32_t>(
                        ))
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
        = [&](const T _a_begin, const T _a_itt, const T _a_end) -> rv_t
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
                    const u8string _l_char_as_str{
                        represent_char_as_hex_for_output(_l_first)
                    };
                    return unexpected(fmt::format(
                        u8"{0}. It was processed as a high surrogate code "
                        u8"unit, meant to be the first part of a pair of "
                        u8"high "
                        u8"and low surrogate code units, which together "
                        u8"should "
                        u8"represent a single unicode code point. However, "
                        u8"as "
                        u8"it is the last code unit in the sequence of "
                        u8"code "
                        u8"units, it was not possible to process it as "
                        u8"part of "
                        u8"a pair, and it is therefore part of an invalid "
                        u8"code "
                        u8"unit sequence.",
                        detail::single_code_unit_error_description<
                            Original_Value_Type,
                            Is_Substring>(
                            std::distance(_a_begin, _l_itt) + 1, _l_char_as_str
                        )
                    ));
                }
                else
                {
                    return nullopt;
                }
            }
            ++_l_itt;
            const char16_t _l_second{static_cast<char16_t>(*_l_itt)};
            if (not detail::is_low_surrogate(_l_second))
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(fmt::format(
                        u8"{0}. The 1st code unit was processed as a high "
                        u8"surrogate code unit, meant to be the first part "
                        u8"of "
                        u8"a pair of high and low surrogate code units, "
                        u8"which "
                        u8"together should represent a single unicode code "
                        u8"point. However, the 2nd code unit was "
                        u8"identified as "
                        u8"a high surrogate code unit - that is, "
                        u8"inclusively "
                        u8"between {1} and {2}. It should have been a "
                        u8"low surrogate code unit - inclusively between "
                        u8"{3} and {4}. As such, the code unit sequence "
                        u8"is invalid.",
                        _l_multi_code_unit_description.operator()<false>(
                            std::distance(_a_begin, _l_itt),
                            basic_string_view<CharT>(
                                std::prev(_l_itt, 1), _l_itt + 1
                            )
                        ),
                        represent_char_as_hex_for_output(
                            high_surrogate_lower_value<char16_t>()
                        ),
                        represent_char_as_hex_for_output(
                            detail::high_surrogate_upper_value<char16_t>()
                        ),
                        represent_char_as_hex_for_output(
                            detail::low_surrogate_lower_value<char16_t>()
                        ),
                        represent_char_as_hex_for_output(
                            low_surrogate_upper_value<char16_t>()
                        )
                    ));
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
                const u8string _l_char_as_str{
                    represent_char_as_hex_for_output(_l_first)
                };
                return unexpected(fmt::format(
                    u8"{0}. It is a low surrogate code "
                    u8"unit - inclusively between the values {1} and {2}. "
                    u8"The UTF16 standard dicates that low surrogate code "
                    u8"units should always follow a high surrogate code "
                    u8"unit. "
                    u8"As such, {3} is part of an invalid code unit "
                    u8"sequence.",
                    detail::single_code_unit_error_description<
                        Original_Value_Type,
                        Is_Substring>(
                        std::distance(_a_begin, _l_itt) + 1, _l_char_as_str
                    ),
                    represent_char_as_hex_for_output(
                        detail::low_surrogate_lower_value<char16_t>()
                    ),
                    represent_char_as_hex_for_output(
                        low_surrogate_upper_value<char16_t>()
                    ),
                    _l_char_as_str
                ));
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
    if (_a_itt > _a_end)
    {
        if constexpr (Return_Reason)
        {
            return unexpected(fmt::format(
                u8"the supplied iterators were inconsistent. Specifically "
                u8"the "
                u8"current iterator is greater than the end iterator."
            ));
        }
        else
        {
            return nullopt;
        }
    }
    else if (_a_itt < _a_begin)
    {
        if constexpr (Return_Reason)
        {
            return unexpected(fmt::format(
                u8"the supplied iterators were inconsistent. Specifically "
                u8"the "
                u8"current iterator is less than the beginning iterator."
            ));
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
            return unexpected(fmt::format(
                u8"the supplied iterators were inconsistent. Specifically "
                u8"the "
                u8"current iterator is equal to the end iterator. "
                u8"Therefore, "
                u8"no characters are able to be extracted from it."
            ));
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
                    const u8string _l_char_as_str{
                        represent_char_as_hex_for_output(_l_char)
                    };
                    return unexpected(fmt::format(
                        u8"{0}. {1}",
                        detail::single_code_unit_error_description<
                            Original_Value_Type,
                            Is_Substring>(
                            std::distance(_a_begin, _a_itt) + 1, _l_char_as_str
                        ),
                        _l_next_char.value()
                    ));
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

template <bool Return_Reason, typename T, typename Original_Type>
requires char_type_is_unicode_c<typename T::value_type>
         && char_type_is_unicode_c<Original_Type>
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
        next_char32_t_internal<Return_Reason, false, T, Original_Type>(
            _a_begin, _a_itt, _a_end
        )
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
__constexpr T
    zero() noexcept
{
    return T(0x0);
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T one_below_high_surrogate_lower_value() noexcept
{
    return high_surrogate_lower_value<T>() - T(0x01);
}

template <typename T>
requires char_type_is_unicode_c<T>
         && (
             sizeof(T) >= 2
         )
__constexpr T one_above_low_surrogate_upper_value() noexcept
{
    return low_surrogate_upper_value<T>() + T(0x01);
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

template <typename T, bool Part_Of_Sequence>
requires char_type_is_unicode_c<T>
__constexpr std::u8string
            unicode_error_description() noexcept
{
    using namespace std;
    u8string _l_standard_name;
    if constexpr (same_as<T, char8_t>)
    {
        _l_standard_name = u8"UTF8";
    }
    else if constexpr (same_as<T, char16_t> || is_wchar_and_16_bit_c<T>)
    {
        _l_standard_name = u8"UTF16";
    }
    else if constexpr (same_as<T, char32_t> || is_wchar_and_32_bit_c<T>)
    {
        _l_standard_name = u8"UTF32";
    }
    else
    {
        __STATIC_ASSERT(T, "next_char32_t not defined for this character type");
    }
    constexpr u8string_view _l_part_of_sequence_prefix{
        u8"found to be part of an invalid "
        u8"code unit sequence"
    };
    constexpr u8string_view _l_not_part_of_sequence_prefix{
        u8"found to be invalid"
    };
    u8string_view _l_prefix;
    if constexpr (Part_Of_Sequence)
    {
        _l_prefix = _l_part_of_sequence_prefix;
    }
    else
    {
        _l_prefix = _l_not_part_of_sequence_prefix;
    }
    return fmt::format(
        u8"{0} under the {1} standard", _l_prefix, _l_standard_name
    );
}

template <typename CharT, bool Is_Substring>
__constexpr std::u8string
            single_code_unit_error_description(
                const size_t             _a_index,
                const std::u8string_view _a_char
            ) noexcept
{
    using namespace std;
    u8string _l_substring_str;
    if constexpr (Is_Substring)
    {
        _l_substring_str = u8"sub";
    }
    return fmt::format(
        u8"the {0} {1} code unit in the unicode {2}string, represented "
        u8"in hex as {3}, was {4}",
        positive_integer_to_placement(_a_index),
        type_id<CharT>(),
        _l_substring_str,
        _a_char,
        detail::unicode_error_description<CharT, false>()
    );
}
} // namespace detail

_END_ABC_NS