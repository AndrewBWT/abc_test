#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <fmt/xchar.h>
#include <string>

_BEGIN_ABC_NS
template <typename T>
__constexpr    std::u8string
               type_id() noexcept;
__no_constexpr std::u8string
               positive_integer_to_placement(const std::size_t _a_number);

template <typename Type_To_Cast_To, typename T>
__constexpr std::u8string
            make_hex_from_char(
                const T _a_char
            ) noexcept
{
    Type_To_Cast_To _l_char_as_uint{static_cast<Type_To_Cast_To>(_a_char)};
    return fmt::format(u8"\\x{:x}", _l_char_as_uint);
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
    return checkless_convert_ascii_to_unicode_string<u8string>(typeid(T).name()
    );
    // return checkless_convert_string_to_u8string(typeid(T).name());
}

_END_ABC_NS