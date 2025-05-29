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
 *
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
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr std::basic_string<T>
    checkless_unicode_conversion(const std::basic_string_view<U> _a_str);
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr std::basic_string<T>
            checkless_unicode_conversion(const std::basic_string<U>& _a_str);
template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr std::basic_string<T>
            checkless_unicode_conversion(const U _a_char);
template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_unicode_string(const std::basic_string_view<T> _a_str) noexcept;
__constexpr bool
    is_valid_char(const char32_t _a_char) noexcept;
template <bool Return_Reason, typename T>
requires char_type_is_unicode_c<typename T::value_type>
__constexpr std::conditional_t<
    Return_Reason,
    result_t<std::pair<char32_t, std::size_t>>,
    std::optional<std::pair<char32_t, std::size_t>>>
    next_char32_t(const T _a_itt, const T _a_end) noexcept;
template <bool Return_Reason, typename T>
requires char_type_is_unicode_c<typename T::value_type>
__constexpr
std::conditional_t<Return_Reason, result_t<char32_t>, std::optional<char32_t>>
    next_char32_t_and_increment_iterator(T& _a_itt, const T _a_end) noexcept;
template <typename CharT>
__constexpr std::u8string
    unicode_string_to_u8string(const std::basic_string_view<CharT> _a_str
    ) noexcept;

template <typename CharT>
__constexpr std::u8string
    unicode_string_to_u8string(const std::basic_string<CharT>& _a_str) noexcept;

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
        const optional<char32_t> _l_char_opt{
            abc::next_char32_t_and_increment_iterator<false>(_l_itt, _l_end)
        };
        // If it sa valid unicode character.
        if (_l_char_opt.has_value())
        {
            const auto& _l_char{_l_char_opt.value()};
            // If doesn't fit within ASCII.
            if (not (_l_char <= 0x7F))
            {
                return unexpected(fmt::format(
                    u8"The function convert_unicode_to_ascii "
                    u8"could not encode the character '{0}' into basic "
                    u8"ASCII, which is the encoding the function uses to "
                    u8"encode std::string objects. This error occoured "
                    u8"when processing the {1} character in the unicode "
                    u8"string \"{2}\".",
                    unicode_string_to_u8string(u32string(1, _l_char)),
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
            using namespace std;
            const u8string _l_unicode{
                unicode_string_to_u8string(basic_string<CharT>(1, *_l_itt))
            };
            return unexpected(fmt::format(
                u8"The function convert_unicode_to_ascii "
                u8"encountered an invalid unicode character when processing "
                u8"its input. "
                u8"The {0} {1} character \"{2}\" was unable to be processed. "
                u8"\"{3}\" "
                u8"is the "
                u8"entirety of the remaining unicode string to process. In the "
                u8"unicode "
                u8"output shown in "
                u8"this message, bytes which do not represent specific unicode "
                u8"characters are shown in hex, and prepended by the string "
                u8"\"\\x\".",
                positive_integer_to_placement(
                    std::distance(_a_str.begin(), _l_itt) + 1
                ),
                type_id<CharT>(),
                abc::unicode_string_to_u8string(basic_string<CharT>(1, *_l_itt)
                ),
                abc::unicode_string_to_u8string(
                    basic_string<CharT>(_l_itt, _l_end)
                )
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
        if (_l_char < 0 || _l_char > 0x7F)
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
            if (_l_char <= 0xFFFF)
            {
                // BMP (Basic Multilingual Plane)
                _l_back_inserter = static_cast<char16_t>(_l_char);
            }
            else if (_l_char <= 0x10'FFFF)
            {
                // Supplementary Plane → surrogate pair
                const char32_t _l_char_cpy{_l_char - 0x1'0000};
                _l_back_inserter
                    = static_cast<char16_t>((_l_char_cpy >> 10) + 0xD800);
                _l_back_inserter
                    = static_cast<char16_t>((_l_char_cpy & 0x3FF) + 0xDC00);
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
                __STATIC_ASSERT(T, "Invalid wchar_t size.");
            }
        }
        else
        {
            __STATIC_ASSERT(
                T,
                "Invalid argument for "
                "_l_run_char16"
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
                __STATIC_ASSERT(T, "Invalid wchar_t size.");
            }
        }
        else
        {
            __STATIC_ASSERT(
                T,
                "Invalid argument for "
                "_l_run_char32"
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
                return _a_str;
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
                [](auto _a_str2)
                {
                    return _l_convert_u32string_to_u16string(_a_str2);
                }
            );
        }
        else if constexpr (same_as<char32_t, T>)
        {
            return _l_result;
        }
        else if constexpr (same_as<wstring, T>)
        {
            if constexpr (wchar_is_16_bit)
            {
                return _l_result
                    .transform(
                        [](auto _a_str2)
                        {
                            return _l_convert_u32string_to_u16string(_a_str2);
                        }
                    )
                    .transform(
                        [](auto _a_str2)
                        {
                            return pack_wstring(_a_str2);
                        }
                    );
            }
            else if constexpr (wchar_is_32_bit)
            {
                return _l_result.transform(
                    [](auto _a_str2)
                    {
                        return pack_wstring(_a_str2);
                    }
                );
            }
            else
            {
                __STATIC_ASSERT(T, "Invalid wchar_t size.");
            }
        }
        else
        {
            __STATIC_ASSERT(
                T,
                "Invalid argument for "
                "unicode_conversion"
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
                "Invalid argument for "
                "unicode_conversion"
            );
        }
    }
    else
    {
        __STATIC_ASSERT(
            T,
            "Invalid argument for "
            "unicode_conversion"
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
                checkless_unicode_conversion(
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
        using namespace std;
        u8string _l_str;
        ranges::for_each(
            _l_str,
            [&](auto _l_char)
            {
                _l_str.append(make_hex_from_char(_l_char));
            }
        );
        throw test_library_exception_t(fmt::format(
            u8"checkless_unicode_conversion failed. Input {0} "
            u8"argument \"_a_str\" contains invalid unicode "
            u8"character(s). In hex _a_str = \"{1}\".",
            type_id<basic_string<U>>(),
            _l_str
        ));
    }
}

template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr_imp std::basic_string<T>
                checkless_unicode_conversion(
                    const std::basic_string<U>& _a_str
                )
{
    using namespace std;
    return checkless_unicode_conversion<T>(basic_string_view<U>(_a_str));
}

template <typename T, typename U>
requires char_type_is_unicode_c<T> && char_type_is_unicode_c<U>
__constexpr_imp std::basic_string<T>
                checkless_unicode_conversion(
                    const U _a_char
                )
{
    using namespace std;
    return checkless_unicode_conversion<T>(basic_string<U>(1, _a_char));
}

template <typename T>
requires char_type_is_unicode_c<T>
__constexpr bool
    is_valid_unicode_string(
        const std::basic_string_view<T> _a_str
    ) noexcept
{
    using namespace std;
    if constexpr (same_as<T, char32_t>)
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
        __STATIC_ASSERT(T, "Doesn't work");
    }
}

__constexpr bool
    is_valid_char(
        const char32_t _a_char
    ) noexcept
{
    return not detail::if_invalid_char32_show_reason(_a_char).has_value();
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
    auto _l_next_from_u8string = [](const T _a_itt, const T _a_end)
        -> std::conditional_t<
            Return_Reason,
            result_t<std::pair<char32_t, std::size_t>>,
            std::optional<std::pair<char32_t, std::size_t>>>
    {
        auto       _l_itt{_a_itt};
        const auto _l_byte_1{*_l_itt};
        if (not (_l_byte_1 <= 0x7F))
        {
            constexpr array<char32_t, 3> _l_and_array{0x1F, 0x0F, 0x07};
            constexpr array<pair<char8_t, char8_t>, 3>
                _l_byte_1_and_equal_values{
                    {{0xE0, 0xC0}, {0xF0, 0xE0}, {0xF8, 0xF0}}
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
                    return unexpected(fmt::format(u8"couldnt'"));
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
            if (std::distance(_l_itt, _a_end) <= _l_code_point_size + 2)
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(fmt::format(
                        u8"UTF8 validation error. The code point \"{0}\" "
                        u8"(represented in hex)"
                        u8"signified that there should be {1} bytes "
                        u8"representing a unicide character. Instead, only {2} "
                        u8"bytes were remaining in the byte sequences.",
                        make_hex_from_char(_l_byte_1),
                        _l_code_point_size + 2,
                        std::distance(_l_itt, _a_end)
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
                const auto _l_byte{*_l_itt};
                if ((_l_byte & 0xC0) != 0x80)
                {
                    if constexpr (Return_Reason)
                    {
                        return unexpected(fmt::format(
                            u8"UTF8 validation error. Each byte set except the "
                            u8"leading byte, used to represent a unicode "
                            u8"character, "
                            "first two binary digits should be 10. Instead, "
                            "the first 2 binary digits of the {0} byte in the "
                            "sequence {1} are {3}.",
                            1,
                            1,
                            1
                        ));
                    }
                    else
                    {
                        return nullopt;
                    }
                }
                // Shifts the bit into the code point. It shifts by
                // 6 due to each byte in a UTF8 string only using
                // the last 6 bits. The & 0x3F isolates these 6
                // bits.
                _l_code_point = (_l_code_point << 6) | (_l_byte & 0x3F);
            }
            constexpr array<char32_t, 3> _l_code_point_limits{
                0x80, 0x800, 0x1'0000
            };
            if (_l_code_point < _l_code_point_limits[_l_code_point_size])
            {
                if constexpr (Return_Reason)
                {
                    return unexpected(fmt::format(
                        u8"UTF8 validation error. Created UTF32 unicode "
                        u8"character is not the shortest representation in "
                        u8"UTF8. "
                        u8"Specifically, {0} code points require the created "
                        u8"UTF32 character to be at least {1}. The created "
                        u8"UTF32 character has a value of {2}.",
                        _l_code_point_size,
                        make_hex_from_char(
                            _l_code_point_limits[_l_code_point_size - 1]
                        ),
                        make_hex_from_char(_l_code_point)

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
                        u8"UTF8 validation error found after conversion to "
                        u8"UTF32. {0}",
                        _l_invalid_char_reason.value()
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
    auto _l_next_from_u16_string = [](const T _a_itt, const T _a_end)
        -> std::conditional_t<
            Return_Reason,
            result_t<std::pair<char32_t, std::size_t>>,
            std::optional<std::pair<char32_t, std::size_t>>>
    {
        auto       _l_itt{_a_itt};
        const auto _l_first{*_l_itt};
        if (_l_first >= 0xD800 && _l_first <= 0xDBFF)
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
            const auto _l_second{*_l_itt};
            if (_l_second < 0xDC00 || _l_second > 0xDFFF)
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
            char32_t _l_char  = 0x1'0000;
            _l_char          += ((_l_first - 0xD800) << 10);
            _l_char          += (_l_second - 0xDC00);
            return make_pair(_l_char, 2);
        }
        else if (_l_first >= 0xDC00 && _l_first <= 0xDFFF)
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
    if constexpr (same_as<CharT, char8_t>)
    {
        return _l_next_from_u8string(_a_itt, _a_end);
    }
    else if constexpr (same_as<CharT, char16_t> || is_wchar_and_16_bit_c<CharT>)
    {
        return _l_next_from_u16_string(_a_itt, _a_end);
    }
    else if constexpr (same_as<CharT, char32_t> || is_wchar_and_32_bit_c<CharT>)
    {
        if (_a_itt == _a_end)
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
        else
        {
            const CharT _l_char{*_a_itt};
            if (auto _l_next_char{detail::if_invalid_char32_show_reason(_l_char)};
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
    }
    else
    {
        __STATIC_ASSERT(T, "Couldn't");
    }
}

template <bool Return_Reason, typename T>
requires char_type_is_unicode_c<typename T::value_type>
__constexpr
std::conditional_t<Return_Reason, result_t<char32_t>, std::optional<char32_t>>
    next_char32_t_and_increment_iterator(
        T&      _a_itt,
        const T _a_end
    ) noexcept
{
    using namespace std;
    auto _l_next_char32_t_result{
        abc::next_char32_t<Return_Reason>(_a_itt, _a_end)
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
            abc::next_char32_t_and_increment_iterator<false>(_l_itt, _l_end)
        };
        if (_l_char_opt.has_value())
        {
            detail::add_char32_to_u8string(_l_char_opt.value(), _l_back_inserter);
        }
        else
        {
            _l_rv.append(make_hex_from_char(*_l_itt));
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
    for (auto _l_itt{std::begin(_a_str)}; _l_itt != _l_end; ++_l_itt)
    {
        auto _l_result{
            next_char32_t_and_increment_iterator<true>(_l_itt, _l_end)
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
    for (auto _l_itt{std::begin(_a_str)}; _l_itt != _l_end; ++_l_itt)
    {
        auto _l_result{
            next_char32_t_and_increment_iterator<true>(_l_itt, _l_end)
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
    if (_a_char >= 0xD7FF && _a_char <= 0xE000)
    {
        return fmt::format(
            u8"UTF32 character found to be in the surrogate range. "
            u8"The surrogate range is beteween {0} and {1}. Character in "
            u8"question has a hex value value of {2}.",
            u8"0xD800",
            u8"0xDFFF",
            make_hex_from_char(_a_char)
        );
    }
    else if (_a_char > 0x10'FFFF)
    {
        return fmt::format(
            u8"UTF32 character found to outisde the valid UTF32 range - that "
            u8"is, greater than {0}. Character in question has a hex value "
            u8"value of {1}.",
            u8"0x10FFFF",
            make_hex_from_char(_a_char)
        );
    }
    return nullopt;
}

__constexpr void
    add_char32_to_u8string(
        const char32_t                                         _a_char,
        std::back_insert_iterator<std::basic_string<char8_t>>& _a_inserter
    ) noexcept
{
    if (_a_char <= 0x7F)
    {
        // 1-byte UTF-8
        _a_inserter = static_cast<char>(_a_char);
    }
    else if (_a_char <= 0x7FF)
    {
        // 2-byte UTF-8
        _a_inserter = static_cast<char>(0xC0 | (_a_char >> 6));
        _a_inserter = static_cast<char>(0x80 | (_a_char & 0x3F));
    }
    else if (_a_char <= 0xFFFF)
    {
        // 3-byte UTF-8
        _a_inserter = static_cast<char>(0xE0 | (_a_char >> 12));
        _a_inserter = static_cast<char>(0x80 | ((_a_char >> 6) & 0x3F));
        _a_inserter = static_cast<char>(0x80 | (_a_char & 0x3F));
    }
    else if (_a_char <= 0x10'FFFF)
    {
        // 4-byte UTF-8
        _a_inserter = static_cast<char>(0xF0 | (_a_char >> 18));
        _a_inserter = static_cast<char>(0x80 | ((_a_char >> 12) & 0x3F));
        _a_inserter = static_cast<char>(0x80 | ((_a_char >> 6) & 0x3F));
        _a_inserter = static_cast<char>(0x80 | (_a_char & 0x3F));
    }
    else
    {
        std::unreachable();
    }
}
} // namespace detail

_END_ABC_NS