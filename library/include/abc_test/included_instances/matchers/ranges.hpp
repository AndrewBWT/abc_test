#pragma once


#include <ranges>

_BEGIN_ABC_NS

template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr matcher_t
    contains(R&& _a_range, const T& _a_value) noexcept;

template <typename R1, typename R2>
requires std::
    same_as<std::ranges::range_value_t<R1>, std::ranges::range_value_t<R2>>
    __constexpr matcher_t
    ranges_equal(
        R1&& _a_range_1,
        R2&& _a_range_2
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using T = std::ranges::range_value_t<R1>;
    size_t                      _l_idx{0};
    typename R1::const_iterator _l_itt_1{std::begin(_a_range_1)};
    typename R1::const_iterator _l_itt_1_end{std::end(_a_range_2)};
    typename R2::const_iterator _l_itt_2{std::begin(_a_range_2)};
    typename R2::const_iterator _l_itt_2_end{std::end(_a_range_2)};
    optional<size_t>            _l_unequal_index;
    size_t                      _l_idx{1};
    string                      _l_explanation_str;
    while (_l_itt_1 != _l_itt_1_end && _l_itt_2 != _l_itt_2_end)
    {
        const T& _l_element_1{*_l_itt_1};
        const T& _l_element_2{*_l_itt_2};
        if (_l_element_1 != _l_element_2)
        {
            _l_unequal_index = _l_idx;
            break;
        }
        ++_l_itt_1;
        ++_l_itt_2;
        ++_l_idx;
    }
    if (not _l_unequal_index.has_value())
    {
        const bool _l_left_smaller{_l_itt_1 != _l_itt_1_end};
        if (_l_left_smaller || _l_itt_2 != _l_itt_2_end)
        {
            _l_explanation_str = fmt::format(
                "{0} argument (size {1} smaller than {2} argument ({3})",
                _l_left_smaller ? "left" : "right",
                _l_idx - 1,
                _l_left_smaller ? "right" : "left",
                (ranges::sized_range<R2>
                     ? fmt::format(
                           "{}",
                           ranges::size(
                               _l_left_smaller ? _a_range_2 : _a_range_1
                           )
                       )
                     : "unknown size")
            );
        }
        else
        {
            return matcher_t(matcher_result_t(
                true, fmt::format("{0} == {1}", _a_range_1, _a_range_2)
            ));
        }
    }
    else
    {
        _l_explanation_str = fmt::format(
            "Ranges diverge at index {0}. Left argument's index {0} = {1}, and "
            "right argument's index {0} = {2}",
            _l_idx,
            *_l_itt_1,
            *_l_itt_2
        );
    }
    return matcher_t(matcher_result_t(
        false,
        fmt::format(
            "{0} != {1}. {2}.", _a_range_1, _a_range_2, _l_explanation_str
        )
    ));
}

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