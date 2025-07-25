#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include <string>
#include <vector>

_BEGIN_ABC_NS
class simple_reporter_t
{
public:
    __no_constexpr_imp virtual void
        write_line(const std::u8string_view _a_str) noexcept = 0;
    __no_constexpr_imp virtual void
        write_error_line(const std::u8string_view _a_str) noexcept = 0;
    __constexpr void
        write_lines(const std::vector<std::u8string>& _a_strs) noexcept
    {
        for (auto&& _l_str : _a_strs)
        {
            write_line(_l_str);
        }
    }
    __constexpr void
        write_error_lines(const std::vector<std::u8string>& _a_strs) noexcept
    {
        for (auto&& _l_str : _a_strs)
        {
            write_error_line(_l_str);
        }
    }
};
_END_ABC_NS

_BEGIN_ABC_NS

_END_ABC_NS