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
template <typename T>
__constexpr char32_t
    next_char32_t(T& _a_itt, T _a_end);
template <bool Perform_Checks, typename T>
__constexpr
    std::conditional_t<Perform_Checks, result_t<std::string>, std::string>
    internal_convert_unicode_string_to_ascii_string(const T _a_str) noexcept;
template <bool Perform_Checks, typename T>
__constexpr std::conditional_t<Perform_Checks, result_t<T>, T>
    internal_convert_ascii_to_unicode_string(const std::string_view _a_str
    ) noexcept;
template <bool Perform_Checks, bool Input_Sanitised, typename T, typename U>
__constexpr std::conditional_t<Perform_Checks, result_t<T>, T>
            internal_unicode_conversion(const U _a_str) noexcept;
} // namespace detail

_END_ABC_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp result_t<std::string>
                convert_unicode_string_to_ascii_string(
                    const T _a_str
                ) noexcept
{
    return detail::internal_convert_unicode_string_to_ascii_string<true>(_a_str
    );
}

template <typename T>
__constexpr_imp std::string
                checkless_convert_unicode_string_to_ascii_string(
                    const T _a_str
                ) noexcept
{
    return detail::internal_convert_unicode_string_to_ascii_string<false>(_a_str
    );
}

template <typename T>
__constexpr result_t<T>
            convert_ascii_to_unicode_string(
                const std::string_view _a_str
            ) noexcept
{
    return detail::internal_convert_ascii_to_unicode_string<true, T>(_a_str);
}

template <typename T>
__constexpr T
    checkless_convert_ascii_to_unicode_string(
        const std::string_view _a_str
    ) noexcept
{
    return detail::internal_convert_ascii_to_unicode_string<false, T>(_a_str);
}

template <typename T, typename U>
__constexpr T
    unicode_conversion(
        const U _a_str
    ) noexcept
{
    return detail::internal_unicode_conversion<false, false, T>(_a_str);
    /*    using namespace std;
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
                    _a_str.begin(), _a_str.end(),
       back_inserter(_l_intermediate_str)
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
                    _a_str.begin(), _a_str.end(),
       back_inserter(_l_intermediate_str)
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
        }*/
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

template <typename T>
__constexpr_imp char32_t
    next_char32_t(
        T& _a_itt,
        T  _a_end
    )
{
    using namespace std;
    using VT = typename T::value_type;
    if constexpr (same_as<VT, char8_t>)
    {
        return utf8::next(_a_itt, _a_end);
    }
    else if constexpr (same_as<VT, char16_t>
                       || (same_as<VT, wchar_t> && sizeof(wchar_t) == 2))
    {
        return utf8::next16(_a_itt, _a_end);
    }
    else if constexpr (same_as<VT, char32_t>
                       || (same_as<VT, wchar_t> && sizeof(wchar_t) == 4))
    {
        if (_a_itt == _a_end)
        {
            throw utf8::not_enough_room();
        }
        else
        {
            char32_t _l_char{*_a_itt++};
            if ((_l_char <= 0xD7FF)
                || (_l_char >= 0xE000 && _l_char <= 0x10'FFFF))
            {
                return _l_char;
            }
            else
            {
                throw utf8::invalid_code_point(_l_char);
            }
        }
    }
    else
    {
        __STATIC_ASSERT(T, "next_char32_t not valid for this type");
    }
}

template <bool Perform_Checks, typename T>
__constexpr_imp
    std::conditional_t<Perform_Checks, result_t<std::string>, std::string>
    internal_convert_unicode_string_to_ascii_string(
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
            _l_char = detail::next_char32_t(_l_itt, _l_end);
        }
        catch (const exception& _l_exception)
        {
            if constexpr (Perform_Checks)
            {
                return detail::make_unexpected_exception_msg(
                    type_id<decltype(_l_exception)>(),
                    _l_exception.what(),
                    _l_itt,
                    _l_end
                );
            }
            else
            {
                _l_char = static_cast<char32_t>(*_l_itt);
                ++_l_itt;
                continue;
            }
        }
     //   ++_l_itt;
        if constexpr (Perform_Checks)
        {
            if (not (_l_char <= 0x7F))
            {
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
                    detail::convert_char_to_string<char8_t>(_l_char),
                    positive_integer_to_placement(_l_idx),
                    unicode_conversion<u8string>(_a_str)
                ));
            }
        }
        _l_output.push_back(static_cast<char>(_l_char));
    }

    return _l_output;
}

template <bool Perform_Checks, typename T>
__constexpr_imp std::conditional_t<Perform_Checks, result_t<T>, T>
                internal_convert_ascii_to_unicode_string(
                    const std::string_view _a_str
                ) noexcept
{
    using namespace std;
    T                       _l_output;
    back_insert_iterator<T> _l_output_itt{back_inserter(_l_output)};
    // Encode each ASCII character as UTF-8 using utf8cpp
    for (char _l_char : _a_str)
    {
        if (static_cast<unsigned char>(_l_char) > 0x7F)
        {
            if constexpr (Perform_Checks)
            {
                return unexpected(fmt::format(
                    u8"Non-ASCII character encountered. Character = {0}",
                    _l_char
                ));
            }
            else
            {
                auto ki = static_cast<unsigned char>(_l_char);
                utf8::append(static_cast<char32_t>(ki), _l_output_itt);
                continue;
            }
        }
        else
        {
            utf8::append(static_cast<char32_t>(_l_char), _l_output_itt);
        }
    }
    return _l_output;
}

template <bool Perform_Checks, bool Input_Sanitised, typename T, typename U>
__constexpr_imp std::conditional_t<Perform_Checks, result_t<T>, T>
                internal_unicode_conversion(
                    const U _a_str
                ) noexcept
{
    using namespace std;
    using rv = conditional_t<Perform_Checks, result_t<T>, T>;
    using TV = typename T::value_type;
    using UV = typename U::value_type;

    if constexpr (same_as<TV, UV>)
    {
        if constexpr (same_as<TV, char8_t>)
        {
            u8string _l_validated_str;
            auto _l_itt{ _a_str.begin() };
            do
            {
                auto _l_itt2{ utf8::find_invalid(_l_itt,_a_str.end()) };
                if (_l_itt2 != _a_str.end())
                {
                    if (_l_itt2 != _a_str.begin())
                    {
                        _l_validated_str.append(_l_itt, _l_itt2);
                    }
                    _l_itt = _l_itt2 + 1;
                }
                else
                {
                    _l_validated_str.append(_l_itt, _l_itt2);
                    _l_itt = _a_str.end();
                }
            } while (_l_itt != _a_str.end());
            return T(_l_validated_str);
        }
        else
        {
            const conditional_t<Perform_Checks, result_t<u8string>, u8string>
                _l_result_1{
                    internal_unicode_conversion<Perform_Checks, false, T>(_a_str)
            };
            if constexpr (Perform_Checks)
            {
                if (_l_result_1.has_value())
                {
                    return internal_unicode_conversion<false, true, T>(_a_str);
                }
                else
                {
                    return unexpected(_l_result_1.error());
                }
            }
            else
            {
                return internal_unicode_conversion<false, true, T>(_a_str);
            }
        }
    }
    else if constexpr (same_as<UV, wchar_t>)
    {
        if constexpr (sizeof(wchar_t) == 2)
        {
            u16string _l_intermediate_str(_a_str.begin(), _a_str.end());
            return internal_unicode_conversion<
                Perform_Checks,
                Input_Sanitised,
                T>(_l_intermediate_str);
        }
        else if constexpr (sizeof(wchar_t) == 4)
        {
            u32string _l_intermediate_str(_a_str.begin(), _a_str.end());
            return internal_unicode_conversion<
                Perform_Checks,
                Input_Sanitised,
                T>(_l_intermediate_str);
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
        u8string                       _l_intermediate_str;
        back_insert_iterator<u8string> _l_inserter{
            back_inserter(_l_intermediate_str)
        };
        u8string _l_object_as_u8str;
        auto     _l_itt{_a_str.begin()};
        do
        {
            char32_t _l_char = utf8::internal::mask16(*_l_itt++);
            // Take care of surrogate pairs first
            if (utf8::internal::is_lead_surrogate(_l_char))
            {
                if (_l_itt != _a_str.end())
                {
                    const char32_t _l_trail_surrogate
                        = utf8::internal::mask16(*_l_itt++);
                    if (utf8::internal::is_trail_surrogate(_l_trail_surrogate))
                    {
                        _l_char = (_l_char << 10) + _l_trail_surrogate
                                  + utf8::internal::SURROGATE_OFFSET;
                    }
                    else
                    {
                        if constexpr (Perform_Checks)
                        {
                            return unexpected("hello");
                        }
                        else
                        {
                            continue;
                        }
                       // throw invalid_utf16(
                      //      static_cast<utfchar16_t>(_l_trail_surrogate)
                      //  );
                    }
                }
                else
                {
                    if constexpr (Perform_Checks)
                    {
                        return unexpected("hello");
                    }
                    else
                    {
                        continue;
                    }
                  //  throw invalid_utf16(static_cast<utfchar16_t>(_l_char));
                }
            }
            // Lone trail surrogate
            else if (utf8::internal::is_trail_surrogate(_l_char))
            {
                if constexpr (Perform_Checks)
                {
                    return unexpected("hello");
                }
                else
                {
                    continue;
                }
               // throw invalid_utf16(static_cast<utfchar16_t>(cp));
            }

            utf8::append(_l_char, _l_inserter);

            /*try
            {
                utf8::utf16to8(_l_itt, _a_str.end(), _l_inserter);
            }
            catch (const exception& _l_exception)
            {
                if constexpr (Perform_Checks)
                {
                    return unexpected("Couldn't work");
                }
                else
                {
                    ++_l_itt;
                    continue;
                }
            }*/
        }
        while (_l_itt != _a_str.end());
        if constexpr (same_as<T, basic_string<char8_t>>)
        {
            return _l_intermediate_str;
        }
        else
        {
            return internal_unicode_conversion<false, true, T>(
                _l_intermediate_str
            );
        }
    }
    // UV only char8_t, char32_t.
    else if constexpr (same_as<UV, char32_t>)
    {
        u8string _l_intermediate_str;
        auto     _l_itt = _a_str.begin();
        while (_l_itt != _a_str.end())
        {
            char32_t _l_char{*_l_itt++};
            if (not utf8::internal::is_code_point_valid(_l_char))
            {
                if constexpr (Perform_Checks)
                {
                    return unexpected("Couldn't work");
                }
                else
                {
                    continue;
                }
            }
            else
            {
                utf8::internal::append(
                    _l_char, std::back_inserter(_l_intermediate_str)
                );
            }
        }

        if constexpr (same_as<T, basic_string<char8_t>>)
        {
            return _l_intermediate_str;
        }
        else
        {
            return internal_unicode_conversion<false, true, T>(
                _l_intermediate_str
            );
        }
    }

    else if constexpr (same_as<char8_t, UV>)
    {
        u8string _l_input;
        if constexpr (not Input_Sanitised)
        {
            auto _l_itt{_a_str.begin()};
            do
            {
                try
                {
                    utf8::utf32to8(
                        _l_itt, _a_str.end(), back_inserter(_l_input)
                    );
                }
                catch (const exception& _l_exception)
                {
                    if constexpr (Perform_Checks)
                    {
                        return unexpected("Couldn't work");
                    }
                    else
                    {
                        ++_l_itt;
                        continue;
                    }
                }
            }
            while (_l_itt != _a_str.end());
        }
        else
        {
            _l_input = _a_str;
        }
        if constexpr (same_as<TV, char16_t>
                      || same_as<TV, wchar_t> && sizeof(wchar_t) == 2)
        {
            u16string _l_intermediate_str;
            utf8::utf8to16(
                _l_input.begin(),
                _l_input.end(),
                back_inserter(_l_intermediate_str)
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
                _l_input.begin(),
                _l_input.end(),
                back_inserter(_l_intermediate_str)
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

} // namespace detail

_END_ABC_NS