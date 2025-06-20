#pragma once
#include "abc_test/external/utfcpp/utf8.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/string_cast.hpp"

#include <fmt/xchar.h>
#include <string>

_BEGIN_ABC_UTILITY_STR_NS
template <typename T>
__constexpr std::u8string
            type_id() noexcept;
template <typename T>
requires std::unsigned_integral<T>
__no_constexpr std::u8string
               positive_integer_to_placement(const T _a_number) noexcept;
template <std::size_t I>
__no_constexpr std::u8string
positive_integer_to_placement() noexcept;

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

template <typename T>
__constexpr std::u8string
            represent_char_as_hex_for_printing(
                const T _a_char
            )
{
    return make_hex_from_char_with_prefix(_a_char, u8"\\x");
}

template <typename T>
__constexpr std::u8string
            represent_char_as_hex_for_output(
                const T _a_char
            )
{
    return make_hex_from_char_with_prefix(_a_char, u8"0x");
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
        T _l_normalised{_a_number % 100};
        if (_l_normalised > 10 && _l_normalised < 13)
        {
            return u8"th";
        }
        else
        {
            switch (_a_number % T(10))
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
_END_ABC_UTILITY_STR_NS