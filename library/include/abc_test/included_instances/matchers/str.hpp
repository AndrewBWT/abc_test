#pragma once


#include "abc_test/core/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_NS

template <typename T, typename R>
    requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr matcher_t
contains(R&& _a_range, const T& _a_value) noexcept;

__no_constexpr matcher_t
c_str_equal(
    const char* _a_str1,
    const std::string_view _a_str2
) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS

__no_constexpr_imp matcher_t
c_str_equal(
    const char* _a_str1,
    const std::string_view _a_str2
) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    const bool _l_equal{ std::strcmp(_a_str1, _a_str2.data()) == 0 };
    return mk_matcher_using_result(matcher_result_t(
        _l_equal,
        fmt::format(
            "{0} {1} {2}", string(_a_str1), _l_equal ? "==" : "!=", _a_str2
        )
    ));
}

_END_ABC_NS