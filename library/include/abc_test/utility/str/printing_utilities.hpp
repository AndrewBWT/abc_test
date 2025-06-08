#pragma once
#include "abc_test/external/utfcpp/utf8.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <fmt/xchar.h>
#include <string>

_BEGIN_ABC_NS
template <typename T>
__constexpr    std::u8string
               type_id() noexcept;
__no_constexpr std::u8string
               positive_integer_to_placement(const std::size_t _a_number);

/*template <typename T>
__constexpr_imp std::optional<std::pair<char32_t, std::size_t>>
                next_char32_t(
                    const T _a_itt,
                    const T _a_end
                )
{
    using namespace std;
    using VT = typename T::value_type;
    auto _l_itt_cpy{_a_itt};
    if constexpr (same_as<VT, char8_t>)
    {
        try
        {
            char32_t _l_char{utf8::next(_l_itt_cpy, _a_end)};
            return make_pair(_l_char, distance(_a_itt, _l_itt_cpy));
        }
        catch (const std::exception& _a_exception)
        {
            return std::nullopt;
        }
    }
    else if constexpr (same_as<VT, char16_t>
                       || (same_as<VT, wchar_t> && sizeof(wchar_t) == 2))
    {
        using namespace utf8;
        utfchar32_t               _l_char = 0;
        utf8::internal::utf_error _l_error_code
            = utf8::internal::validate_next16(_l_itt_cpy, _a_end, _l_char);
        if (_l_error_code != utf8::internal::UTF8_OK)
        {
            return std::nullopt;
        }
        else
        {
            return make_pair(_l_char, std::distance(_a_itt, _l_itt_cpy));
        }
    }
    else if constexpr (same_as<VT, char32_t>
                       || (same_as<VT, wchar_t> && sizeof(wchar_t) == 4))
    {
        if (_a_itt == _a_end)
        {
            return std::nullopt;
        }
        else
        {
            char32_t _l_char{*_a_itt};
            if ((_l_char <= 0xD7FF)
                || (_l_char >= 0xE000 && _l_char <= 0x10'FFFF))
            {
                return make_pair(_l_char, size_t(1));
            }
            else
            {
                return std::nullopt;
            }
        }
    }
    else
    {
        __STATIC_ASSERT(T, "next_char32_t not valid for this type");
    }
}*/

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

template <typename T>
__constexpr std::u8string
            make_hex_from_char(
                const T _a_char
            ) noexcept
{
    using namespace std;
    using Type_To_Cast_To = char_underlying_type_t<T>;
    const u8string _l_char_as_hex{
        fmt::format(u8"{:x}", static_cast<Type_To_Cast_To>(_a_char))
    };
    return fmt::format(
        u8"\\x{0}{1}",
        u8string((sizeof(Type_To_Cast_To) * 2) - _l_char_as_hex.size(), u8'0'),
        _l_char_as_hex
    );
}

template <typename T>
__constexpr std::basic_string<T>
            make_focused_string(
                const std::basic_string_view<T> _a_sv,
                const std::size_t               _a_idx,
                const std::size_t               _a_limit = 10
            )
{
    using namespace std;
    const size_t _l_offset{_a_limit + 1};
    const bool   _l_enough_elements_before{_a_idx >= _l_offset};
    const bool   _l_engouh_elements_after{_a_idx + _l_offset <= _a_sv.size()};
    const size_t _l_after_offset{_a_idx + _l_offset};
    const size_t _l_before_offset{_a_idx - _l_offset};
    return basic_string<T>{
        _l_enough_elements_before
            ? (_l_engouh_elements_after
                   ? _a_sv.substr(_l_before_offset, _l_after_offset)
                   : _a_sv.substr(_l_before_offset))
            : (_l_engouh_elements_after ? _a_sv.substr(0, _l_after_offset)
                                        : _a_sv)
    };
}

template <typename T>
__constexpr std::basic_string<T>
            make_focused_string(
                const std::basic_string<T>& _a_sv,
                const std::size_t           _a_idx,
                const std::size_t           _a_limit = 10
            )
{
    using namespace std;
    return make_focused_string(basic_string_view<T>(_a_sv), _a_idx, _a_limit);
}

_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp std::u8string
                   positive_integer_to_placement(
                       const std::size_t _a_number
                   )
{
    switch (_a_number)
    {
    case 1:
        return u8"1st";
    case 2:
        return u8"2nd";
    case 3:
        return u8"3rd";
    default:
        return fmt::format(u8"{0}th", _a_number);
    }
}

template <typename T>
__constexpr_imp std::u8string
                type_id() noexcept
{
    using namespace std;
    string _l_str{typeid(T).name()};
    return u8string(_l_str.begin(), _l_str.end());
}

_END_ABC_NS