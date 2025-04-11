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