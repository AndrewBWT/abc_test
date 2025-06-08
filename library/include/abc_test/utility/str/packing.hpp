#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/concepts.hpp"

#include <string>

_BEGIN_ABC_NS
__constexpr std::string
            pack_u8string_into_string(const std::u8string_view _a_str) noexcept;
__constexpr std::u8string
            unpack_string_to_u8string(const std::string_view _a_str) noexcept;
__constexpr auto
    pack_wstring(const std::wstring_view _a_str) noexcept;
template <typename T>
requires (wchar_is_16_bit && std::same_as<typename T::value_type, char16_t>)
         || ( wchar_is_32_bit
              && std::same_as<typename T::value_type, char32_t> )
__constexpr std::wstring unpack_wstring(const T _a_str_view);
_END_ABC_NS

_BEGIN_ABC_NS
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

__constexpr auto
    pack_wstring(
        const std::wstring_view _a_str
    ) noexcept
{
    using namespace std;
    if constexpr (wchar_is_16_bit)
    {
        u16string _l_rv{};
        _l_rv.resize(_a_str.size());
        ranges::copy(_a_str, _l_rv.begin());
        return _l_rv;
    }
    else if constexpr (wchar_is_32_bit)
    {
        u32string _l_rv{};
        _l_rv.resize(_a_str.size());
        ranges::copy(_a_str, _l_rv.begin());
        return _l_rv;
    }
    else
    {
    }
}

template <typename T>
requires (wchar_is_16_bit && std::same_as<typename T::value_type,char16_t>)
         || (wchar_is_32_bit && std::same_as<typename T::value_type, char32_t>)
__constexpr_imp std::wstring
                unpack_wstring(
                    const T _a_str_view
                )
{
    using namespace std;
    return wstring(_a_str_view.begin(), _a_str_view.end());
}

_END_ABC_NS