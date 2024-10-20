#pragma once

#include "abc_test/included_instances/matchers/function_wrapper.h"

#include <ranges>

_BEGIN_ABC_NS

template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr matcher_t
    contains(R&& _a_range, const T& _a_value) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS

template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr matcher_t
    contains(
        R&&      _a_range,
        const T& _a_value
    ) noexcept
{
    using namespace std;
    return matcher_t(matcher_base_ptr_t(new function_wrapper_matcher_t(
        [&]()
        {
            return matcher_result_t(
                true,
                (std::ranges::find(_a_range, _a_value) != _a_range.end()),
                fmt::format("{0} does not contain {1}", _a_range, _a_value)
            );
        }
    )));
}

_END_ABC_NS