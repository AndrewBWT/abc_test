#pragma once

#include "abc_test/utility/str/concepts.hpp"
#include "abc_test/utility/str/printing_utilities.hpp"
#include "abc_test/utility/types.hpp"

#include <string>

_BEGIN_ABC_NS
template <typename T>
__constexpr result_t<std::string>
            convert_unicode_string_to_ascii_string(const T _a_str) noexcept;
template <typename T>
__constexpr std::string
    checkless_convert_unicode_string_to_ascii_string(const T _a_str) noexcept;
template <typename T>
__constexpr result_t<T>
    convert_ascii_to_unicode_string(const std::string_view _a_str) noexcept;
template <typename T>
__constexpr T
    checkless_convert_ascii_to_unicode_string(const std::string_view _a_str
    ) noexcept;
template <typename T, typename U>
__constexpr T
    unicode_conversion(const U _a_str) noexcept;
__constexpr std::string
            pack_u8string_into_string(const std::u8string_view _a_str) noexcept;
__constexpr std::u8string
            unpack_string_to_u8string(const std::string_view _a_str) noexcept;

namespace detail
{
template <typename T>
__constexpr result_t<std::string>
            make_unexpected_exception_msg(
                const std::u8string_view _a_exception_name,
                const std::string_view   _a_str,
                const T                  _a_begin_itt,
                const T                  _a_end_itt
            ) noexcept;

template <typename T>
requires char_type_is_unicode_c<T>
__no_constexpr_imp std::basic_string<T>
                   convert_char_to_string(const char32_t _a_char) noexcept;
} // namespace detail

_END_ABC_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp result_t<std::string>
                convert_unicode_string_to_ascii_string(
                    const T _a_str
                ) noexcept
{
    using namespace std;
    using CharT                       = T::value_type;
    // Use utf8cpp to iterate over code points
    typename T::const_iterator _l_itt = _a_str.begin();
    typename T::const_iterator _l_end = _a_str.end();
    string                     _l_output;
    for (size_t _l_idx{1}; _l_itt != _l_end; ++_l_idx)
    {
        char32_t _l_char;
        try
        {
            _l_char = utf8::next(_l_itt, _l_end);
        }
        catch (const utf8::invalid_utf8& _l_exception)
        {
            return detail::make_unexpected_exception_msg(
                u8"invalid_utf8", _l_exception.what(), _l_itt, _l_end
            );
        }
        catch (const utf8::invalid_code_point& _l_exception)
        {
            return detail::make_unexpected_exception_msg(
                u8"invalid_code_point", _l_exception.what(), _l_itt, _l_end
            );
        }
        if (not (_l_char <= 0x7F))
        {
            u8string _l_char_as_u8{
                detail::convert_char_to_string<char8_t>(_l_char)
            };
            return unexpected(fmt::format(
                u8"The function convert_unicode_string_to_ascii_string "
                u8"could not "
                u8"encode "
                u8"the character '{0}' into basic ASCII, which is the "
                u8"encoding "
                u8"used for the std::string type in abc_test. This error "
                u8"occoured when converting the {1} character in the "
                u8"string "
                u8"\"{2}\".",
                _l_char_as_u8,
                positive_integer_to_placement(_l_idx),
                unicode_conversion<u8string>(_a_str)
            ));
        }
        else
        {
            _l_output.push_back(static_cast<char>(_l_char));
        }
    }

    return _l_output;
}

template <typename T>
__constexpr_imp std::string
                checkless_convert_unicode_string_to_ascii_string(
                    const T _a_str
                ) noexcept
{
    using namespace std;
    using CharT                       = T::value_type;
    // Use utf8cpp to iterate over code points
    typename T::const_iterator _l_itt = _a_str.begin();
    typename T::const_iterator _l_end = _a_str.end();
    string                     _l_output;
    for (size_t _l_idx{1}; _l_itt != _l_end; ++_l_idx)
    {
        char32_t _l_char;
        try
        {
            _l_char = utf8::next(_l_itt, _l_end);
        }
        catch (const std::exception& _l_exception)
        {
            break;
        }
        _l_output.push_back(static_cast<char>(_l_char));
    }

    return _l_output;
}

template <typename T>
__constexpr result_t<T>
            convert_ascii_to_unicode_string(
                const std::string_view _a_str
            ) noexcept
{
    using namespace std;
    T _l_output;

    // Encode each ASCII character as UTF-8 using utf8cpp
    for (char _l_char : _a_str)
    {
        if (static_cast<unsigned char>(_l_char) > 0x7F)
        {
            return unexpected(fmt::format(
                u8"Non-ASCII character encountered. Character = {0}", _l_char
            ));
        }
        utf8::append(
            static_cast<char32_t>(_l_char), std::back_inserter(_l_output)
        );
    }
    return _l_output;
}

template <typename T>
__constexpr T
    checkless_convert_ascii_to_unicode_string(
        const std::string_view _a_str
    ) noexcept
{
    using namespace std;
    T _l_output;

    // Encode each ASCII character as UTF-8 using utf8cpp
    for (char _l_char : _a_str)
    {
        utf8::append(
            static_cast<char32_t>(_l_char), std::back_inserter(_l_output)
        );
    }
    return _l_output;
}

template <typename T, typename U>
__constexpr T
    unicode_conversion(
        const U _a_str
    ) noexcept
{
    using namespace std;
    using TV = typename T::value_type;
    using UV = typename U::value_type;
    if constexpr (same_as<TV, UV>)
    {
        return T(_a_str);
    }
    else if constexpr (same_as<UV, wchar_t>)
    {
        if constexpr (sizeof(wchar_t) == 2)
        {
            u16string _l_intermediate_str(_a_str.begin(), _a_str.end());
            if constexpr (same_as<T, basic_string<char16_t>>)
            {
                return _l_intermediate_str;
            }
            else
            {
                return unicode_conversion<T>(_l_intermediate_str);
            }
        }
        else if constexpr (sizeof(wchar_t) == 4)
        {
            u32string _l_intermediate_str(_a_str.begin(), _a_str.end());
            if constexpr (same_as<T, basic_string<char32_t>>)
            {
                return _l_intermediate_str;
            }
            else
            {
                return unicode_conversion<T>(_l_intermediate_str);
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion not defined for this wchar_t type"
            );
        }
    }
    // UV Only char8_t, char16_t, char32_t.
    else if constexpr (same_as<UV, char16_t>)
    {
        u8string _l_intermediate_str;
        utf8::utf16to8(
            _a_str.begin(), _a_str.end(), back_inserter(_l_intermediate_str)
        );
        if constexpr (same_as<T, basic_string<char8_t>>)
        {
            return _l_intermediate_str;
        }
        else
        {
            return unicode_conversion<T>(_l_intermediate_str);
        }
    }
    // UV only char8_t, char32_t.
    else if constexpr (same_as<UV, char32_t>)
    {
        u8string _l_intermediate_str;
        utf8::utf32to8(
            _a_str.begin(), _a_str.end(), back_inserter(_l_intermediate_str)
        );
        if constexpr (same_as<T, basic_string<char8_t>>)
        {
            return _l_intermediate_str;
        }
        else
        {
            return unicode_conversion<T>(_l_intermediate_str);
        }
    }
    else if constexpr (same_as<char8_t, UV>)
    {
        if constexpr (same_as<TV, char16_t>
                      || same_as<TV, wchar_t> && sizeof(wchar_t) == 2)
        {
            u16string _l_intermediate_str;
            utf8::utf8to16(
                _a_str.begin(), _a_str.end(), back_inserter(_l_intermediate_str)
            );
            if constexpr (same_as<TV, wchar_t> && sizeof(wchar_t) == 2)
            {
                return wstring(
                    _l_intermediate_str.begin(), _l_intermediate_str.end()
                );
            }
            else
            {
                return _l_intermediate_str;
            }
        }
        else if constexpr (same_as<TV, char32_t>
                           || same_as<TV, wchar_t> && sizeof(wchar_t) == 4)
        {
            u32string _l_intermediate_str;
            utf8::utf8to32(
                _a_str.begin(), _a_str.end(), back_inserter(_l_intermediate_str)
            );
            if constexpr (same_as<TV, wchar_t> && sizeof(wchar_t) == 4)
            {
                return wstring(
                    _l_intermediate_str.begin(), _l_intermediate_str.end()
                );
            }
            else
            {
                return _l_intermediate_str;
            }
        }
        else
        {
            __STATIC_ASSERT(
                T, "unicode_conversion not defined for this character type"
            );
        }
    }
    else
    {
        __STATIC_ASSERT(
            T, "unicode_conversion not defined for this character type"
        );
    }
}

__constexpr_imp std::string
                pack_u8string_into_string(
                    const std::u8string_view _a_str
                ) noexcept
{
    using namespace std;
    return string(_a_str.begin(), _a_str.end());
}

__constexpr_imp std::u8string
                unpack_string_to_u8string(
                    const std::string_view _a_str
                ) noexcept
{
    using namespace std;
    return u8string(_a_str.begin(), _a_str.end());
}

namespace detail
{
template <typename T>
__constexpr_imp result_t<std::string>
                make_unexpected_exception_msg(
                    const std::u8string_view _a_exception_name,
                    const std::string_view   _a_str,
                    const T                  _a_begin_itt,
                    const T                  _a_end_itt
                ) noexcept
{
    using namespace std;
    using CharT = T::value_type;
    return unexpected(fmt::format(
        u8"The function convert_unicode_string_to_ascii_string "
        u8"encountered an {0} exception when attempting "
        u8"to read the next char32_t entity from the substring {1}."
        u8"The exception's what() message was \"{2}\".",
        _a_exception_name,
        unicode_conversion<u8string>(
            basic_string<CharT>(_a_begin_itt, _a_end_itt)
        ),
        unpack_string_to_u8string(_a_str)
    ));
}

template <typename T>
requires char_type_is_unicode_c<T>
__no_constexpr std::basic_string<T>
               convert_char_to_string(
                   const char32_t _a_char
               ) noexcept
{
    using namespace std;
    if constexpr (is_wchar_and_32_bit_c<T>)
    {
        return wstring(1, static_cast<wchar_t>(_a_char));
    }
    else
    {
        u32string _l_str(1, _a_char);
        if constexpr (same_as<char32_t, T>)
        {
            return _l_str;
        }
        else if constexpr (is_wchar_and_16_bit_c<T>)
        {
            return unicode_conversion<wstring>(_l_str);
        }
        else if constexpr (same_as<char16_t, T>)
        {
            return unicode_conversion<u16string>(_l_str);
        }
        else if constexpr (same_as<char8_t, T>)
        {
            return unicode_conversion<u8string>(_l_str);
        }
        else
        {
            __STATIC_ASSERT(T, "convert_char_to_string not defined for type.");
        }
    }
}
} // namespace detail

_END_ABC_NS