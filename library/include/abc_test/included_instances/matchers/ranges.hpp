#pragma once


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
    using namespace _ABC_NS_MATCHER;
    const bool _l_contains{ranges::find(_a_range, _a_value) != _a_range.end()};
    return matcher_t(matcher_result_t(
        _l_contains,
        fmt::format(
            "{0} {1} {2}",
            _a_range,
            _l_contains ? "contains" : "does not contain",
            _a_value
        )
    ));
}

_END_ABC_NS