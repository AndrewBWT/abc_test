#pragma once


#include "abc_test/core/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_NS
__no_constexpr matcher_t
stacktraces_equal(
    const std::stacktrace& _a_st1,
    const std::stacktrace& _a_st2
) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp matcher_t
stacktraces_equal(
    const std::stacktrace& _a_st1,
    const std::stacktrace& _a_st2
) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    bool _l_equal{ true };
    if (_a_st1.size() != _a_st2.size())
    {
        _l_equal = false;
    }
    else
    {
        const size_t _l_size{ _a_st1.size() };
        for (size_t _l_idx{ 0 }; _l_idx < _l_size; _l_idx++)
        {
            const stacktrace_entry& _l_se1{ _a_st1[_l_idx] };
            const stacktrace_entry& _l_se2{ _a_st2[_l_idx] };
            if (_l_se1 != _l_se2)
            {
                _l_equal = false;
            }
        }
    }
    return mk_matcher_using_result(matcher_result_t(
        _l_equal,
        fmt::format(
            u8"{0} {1} {2}",
            unpack_string_to_u8string(to_string(_a_st1)),
            _l_equal ? u8"==" : u8"!=",
            unpack_string_to_u8string(to_string(_a_st2))
        )
    ));
}
_END_ABC_NS