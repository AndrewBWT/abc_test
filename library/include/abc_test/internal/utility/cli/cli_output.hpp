#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"
#include <vector>
#include "abc_test/internal/utility/str/string_table.hpp"

_BEGIN_ABC_NS
class cli_output_t
{
public:
    __constexpr bool
        terminate_early() const noexcept
    {
        return _m_terminate_early;
    }

    __constexpr bool
        has_errors() const noexcept
    {
        return _m_errors.size() > 0;
    }

    __constexpr bool
        has_warnings() const noexcept
    {
        return false;
    }

    __constexpr bool
        show_log() const noexcept
    {
        return false;
    }

    __constexpr bool
        can_continue() const noexcept
    {
        return (not _m_terminate_early);
    }

    __constexpr bool
        has_text_output() const noexcept
    {
        return _m_output.size() > 0;
    }

    __constexpr void
        add_table_output(
            abc::utility::str::string_table_t&& _a_table
        )
    {
        _m_output.push_back(_a_table());
    }

    __constexpr const std::vector<std::string>&
        text_output() const noexcept
    {
        return _m_output;
    }

    __constexpr void
        set_terminate_early(
            const bool _a_terminate_early
        )
    {
        _m_terminate_early = _a_terminate_early;
    }

    __constexpr void
        add_error(
            const std::string_view _a_str
        ) noexcept
    {
        using namespace std;
        _m_errors.push_back(string(_a_str));
        _m_terminate_early = true;
    }

    __constexpr std::string
        errors() const noexcept
    {
        using namespace std;
        string _l_rv{};
        for (const string_view _a_str : _m_errors)
        {
            _l_rv.append(_a_str);
            _l_rv.append("\n");
        }
        return _l_rv;
    }

    std::vector<std::string> _m_output;
    bool                     _m_terminate_early = false;
    std::vector<std::string> _m_errors;
};

_END_ABC_NS