#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/concepts.hpp"

#include <string>

_BEGIN_ABC_UTILITY_STR_NS
__constexpr std::string
            cast_u8string_to_string(const std::u8string_view _a_str) noexcept;
__constexpr std::u8string
            cast_string_to_u8string(const std::string_view _a_str) noexcept;
__constexpr auto
    cast_wstring_to_unicode_string(const std::wstring_view _a_str) noexcept;
template <typename T>
//requires (wchar_is_16_bit && std::same_as<typename T::value_type, char16_t>)
//         || ( wchar_is_32_bit
//              && std::same_as<typename T::value_type, char32_t> )
__constexpr std::wstring cast_unicode_string_to_wstring(const T _a_str_view);
_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
__constexpr_imp std::string
                cast_u8string_to_string(
                    const std::u8string_view _a_str
                ) noexcept
{
    using namespace std;
    return string(_a_str.begin(), _a_str.end());
}

__constexpr_imp std::u8string
                cast_string_to_u8string(
                    const std::string_view _a_str
                ) noexcept
{
    using namespace std;
    return u8string(_a_str.begin(), _a_str.end());
}

__constexpr auto
    cast_wstring_to_unicode_string(
        const std::wstring_view _a_str
    ) noexcept
{
    using namespace std;
    if constexpr (wchar_is_16_bit)
    {
        return u16string(_a_str.begin(), _a_str.end());
    }
    else if constexpr (wchar_is_32_bit)
    {
        return u32string(_a_str.begin(), _a_str.end());
    }
    else
    {
       // __STATIC_ASSERT(
       //     wchar_t,
        //    "cast_wstring_to_unicode_string not defined for wchar_t of this "
       //     "size"
      //  );
    }
}

template <typename T>
//requires (wchar_is_16_bit && std::same_as<typename T::value_type,char16_t>)
//         || (wchar_is_32_bit && std::same_as<typename T::value_type, char32_t>)
__constexpr_imp std::wstring
                cast_unicode_string_to_wstring(
                    const T _a_str_view
                )
{
    using namespace std;
    if constexpr (sizeof(typename T::value_type) == sizeof(wchar_t))
    {
        return wstring(_a_str_view.begin(), _a_str_view.end());
    }
    else
    {
        return wstring{};
    }
}

_END_ABC_UTILITY_STR_NS
