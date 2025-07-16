#pragma once


#include <ranges>

_BEGIN_ABC_NS

template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr matcher_t
    contains(R&& _a_range, const T& _a_value) noexcept;
template <typename R>
requires std::ranges::range<R>
__constexpr matcher_t
    all_equal(R&& _a_range) noexcept;

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
    return mk_matcher_using_result(matcher_result_t(
        _l_contains,
        matcher_result_infos_t(fmt::format(
            u8"{0} {1} {2}",
            _a_range,
            _l_contains ? u8"contains" : u8"does not contain",
            _a_value
        ))
    ));
}

template <typename R>
requires std::ranges::range<R>
__constexpr matcher_t
    all_equal(
        R&& _a_range
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using T          = std::ranges::range_value_t<R>;
    using Range_Type = std::remove_cvref_t<R>;
    optional<T> _l_value_to_check_against;
    for (auto&& _l_element : _a_range)
    {
        if (_l_value_to_check_against.has_value())
        {
            auto _l_result{
                abc::matcher::matcher_default_comparable_t<
                    T,
                    T,
                    abc::matcher::comparison_enum_t::EQ>{}
                    .run(_l_value_to_check_against.value(), _l_element)
            };
            if (not _l_result.passed())
            {
                return mk_matcher_using_result(matcher_result_t(
                    false,
                    matcher_result_infos_t(
                        fmt::format(
                            u8"All elements in {0} are not equal.",
                            abc::utility::printer::default_printer_t<
                                Range_Type>{}
                                .run_printer(_a_range)
                        ),
                        {u8"Element at index {0} not equal to the previous "
                         u8"elements.",
                         u8"Below we show the result of the matcher checking "
                         u8"equality between the first element and the element "
                         u8"in question."},
                        {_l_result.str().get_tree()}
                    )
                ));
            }
        }
    }
    return mk_matcher_using_result(matcher_result_t(
        true,
        matcher_result_infos_t(fmt::format(
            u8"All elements in {0} are equal.",
            abc::utility::printer::default_printer_t<Range_Type>{}
                .run_printer(_a_range)
        ))
    ));
}

_END_ABC_NS