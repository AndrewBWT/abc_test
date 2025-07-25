#pragma once
#include "abc_test/included_instances/test_harness/simple_reporter.hpp"

_BEGIN_ABC_NS

class simple_console_reporter_t : public simple_reporter_t
{
public:
    __no_constexpr_imp void
        write_line(const std::u8string_view _a_str) noexcept override
    {
        using namespace std;
        cout << string(_a_str.begin(), _a_str.end()) << endl;
    }
    __no_constexpr_imp void
        write_error_line(const std::u8string_view _a_str) noexcept override
    {
        using namespace std;
        cerr << string(_a_str.begin(), _a_str.end()) << endl;
    }
};
_END_ABC_NS