#pragma once

#include "abc_test/utility/internal/macros.hpp"

#include <stacktrace>
#include <string>
#include <vector>
#include <ranges>

_BEGIN_ABC_ERRORS_NS

struct abc_test_error_t
{
public:
    __constexpr
    abc_test_error_t(
        const std::vector<std::u8string>& _a_error_strings,
        const bool                        _a_error_terminates_test_framework,
        const std::stacktrace& _a_stacktrace = std::stacktrace::current()
    ) noexcept;
    __constexpr std::u8string
                make_single_str() const noexcept;
    __constexpr bool
                      terminates_test_framework() const noexcept;
    __constexpr const std::vector<std::u8string>&
                      errors() const noexcept;
private:
    std::vector<std::u8string> _m_error_strings;
    bool                       _m_error_terminates_test_framework;
    std::stacktrace            _m_stacktrace;
};

using opt_abc_test_error_t = std::optional<_ABC_NS_ERRORS::abc_test_error_t>;
_END_ABC_ERRORS_NS

_BEGIN_ABC_ERRORS_NS __constexpr_imp
    abc_test_error_t::abc_test_error_t(
        const std::vector<std::u8string>& _a_error_strings,
        const bool                        _a_error_terminates_test_framework,
        const std::stacktrace&            _a_stacktrace
    ) noexcept
    : _m_error_strings(_a_error_strings)
    , _m_error_terminates_test_framework(_a_error_terminates_test_framework)
    , _m_stacktrace(_a_stacktrace)
{}

__constexpr_imp std::u8string
                abc_test_error_t::make_single_str() const noexcept
{
    using namespace std;
    u8string _l_rv{};
    const size_t _l_size{ _m_error_strings.size() };
    for (size_t _l_idx{ 0 }; auto && _l_str : _m_error_strings)
    {
        _l_rv.append(_l_str);
        if (_l_idx + 1 < _l_size)
        {
            _l_rv.append(u8"\n");
        }
        ++_l_idx;
    }
    return _l_rv;
}

__constexpr bool
    abc_test_error_t::terminates_test_framework() const noexcept
{
    return _m_error_terminates_test_framework;
}

__constexpr const std::vector<std::u8string>&
                  abc_test_error_t::errors() const noexcept
{
    return _m_error_strings;
}

_END_ABC_ERRORS_NS