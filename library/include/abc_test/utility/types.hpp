#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include <expected>
#include <string>
#include <vector>

_BEGIN_ABC_NS
template<typename T>
using result_t = std::expected<T, std::u8string>;

__constexpr std::string u8string_to_string(
    const std::u8string_view _a_str
) noexcept
{
    return std::string{};
}
__constexpr std::u8string char_star_to_u8string(
    const char* _a_cstar
) noexcept
{
    return std::u8string{};
}
__constexpr std::u8string string_view_to_u8string(
    const std::string_view _a_str
) noexcept
{
    return std::u8string{};
}
__constexpr std::vector<std::u8string> map_svs_to_u8strings(
    const std::vector<std::string>& _a_strs
) noexcept
{
    using namespace std;
    vector<u8string> _l_rv;
    for (auto& _l_str : _a_strs)
    {
        _l_rv.push_back(string_view_to_u8string(_l_str));
    }
    return _l_rv;
}
_END_ABC_NS