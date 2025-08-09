#pragma once
#include "abc_test/included_instances/test_harness/simple_reporter.hpp"

_BEGIN_ABC_NS
class simple_text_reporter_t : public simple_reporter_t
{
private:
    std::vector<std::u8string> _m_text;
    std::vector<std::u8string> _m_errors;
public:
    __no_constexpr_imp const std::vector<std::u8string>& text() const noexcept
    {
        return _m_text;
    }
    __no_constexpr_imp const std::vector<std::u8string>& errors() const noexcept
    {
        return _m_errors;
    }
    __no_constexpr_imp virtual void
        write_line(const std::u8string_view _a_str) noexcept override
    {
        using namespace std;
        _m_text.push_back(u8string(_a_str));
    }
    __no_constexpr_imp virtual void
        write_error_line(const std::u8string_view _a_str) noexcept override
    {
        using namespace std;
        _m_errors.push_back(u8string(_a_str));
    }
};
_END_ABC_NS