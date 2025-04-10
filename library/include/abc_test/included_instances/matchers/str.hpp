#pragma once


#include "abc_test/core/matchers/matcher_wrapper.hpp"
#include "abc_test/utility/printers/default_printer.hpp"

_BEGIN_ABC_NS
__no_constexpr matcher_t
    c_str_equal(const char* _a_str1, const std::string_view _a_str2) noexcept;

template <typename R1, typename R2>
__constexpr_imp matcher_t
    strings_equal(
        R1&& _a_range_1,
        R2&& _a_range_2
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace abc::utility::printer;
    using T            = ranges::range_value_t<R1>;
    using R1_Type      = remove_cvref_t<R1>;
    using R2_Type      = remove_cvref_t<R2>;
    using R1_const_itt = R1_Type::const_iterator;
    using R2_const_itt = R1_Type::const_iterator;
    R1_const_itt     _l_itt_1{std::begin(_a_range_1)};
    R1_const_itt     _l_itt_1_end{std::end(_a_range_1)};
    R2_const_itt     _l_itt_2{std::begin(_a_range_2)};
    R2_const_itt     _l_itt_2_end{std::end(_a_range_2)};
    optional<size_t> _l_unequal_index;
    size_t           _l_idx{1};
    u8string         _l_explanation_str;
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
                u8"{0} argument (size {1} smaller than {2} argument ({3})",
                _l_left_smaller ? u8"left" : u8"right",
                _l_idx - 1,
                _l_left_smaller ? u8"right" : u8"left",
                (ranges::sized_range<R2>
                     ? fmt::format(
                           u8"{}",
                           ranges::size(
                               _l_left_smaller ? _a_range_2 : _a_range_1
                           )
                       )
                     : u8"unknown size")
            );
        }
        else
        {
            return mk_matcher_using_result(matcher_result_t(
                true,
                fmt::format(
                    u8"{0} == {1}",
                    default_printer_t<R1_Type>{}.run_printer(_a_range_1),
                    default_printer_t<R2_Type>{}.run_printer(_a_range_2)
                )
            ));
        }
    }
    else
    {
        size_t  _l_limit = 10;
        R1_Type _l_left_surrounding_str{
            _l_idx < _l_limit
                ? _a_range_1.substr(_l_idx + _l_limit)
                : _a_range_1.substr(_l_idx - _l_limit, _l_limit * 2)
        };
        R2_Type _l_right_surrounding_str{
            _l_idx < _l_limit
                ? _a_range_2.substr(_l_idx + _l_limit)
                : _a_range_2.substr(_l_idx - _l_limit, _l_limit * 2)
        };
        _l_explanation_str = fmt::format(
            u8"Strings diverge at index {0}. Left argument's index {0} = {1}, "
            u8"and "
            u8"right argument's index {0} = {2}. The surrounding string of the "
            u8"left and right arguments are \"...{3}...\" and \"...{4}...\"",
            _l_idx,
            default_printer_t<T>{}.run_printer(*_l_itt_1),
            default_printer_t<T>{}.run_printer(*_l_itt_2),
            default_printer_t<R1_Type>{u8""}.run_printer(_l_left_surrounding_str
            ),
            default_printer_t<R2_Type>{u8""}.run_printer(
                _l_right_surrounding_str
            )

        );
    }
    return mk_matcher_using_result(matcher_result_t(
        false,
        fmt::format(
            u8"{0} == {1}. {2}.",
            default_printer_t<R1_Type>{}.run_printer(_a_range_1),
            default_printer_t<R2_Type>{}.run_printer(_a_range_2),
            _l_explanation_str
        )
    ));
}

__no_constexpr_imp matcher_t
    expected_no_exception() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        true,
        fmt::format(u8"No exception thrown, which was the expected behaviour")
    ));
}

__no_constexpr_imp matcher_t
    unexpected_exception_thrown(
        const std::exception& _a_exception
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        fmt::format(
            u8"An unexpected exception, of a type derived from {0} was thrown. "
            u8"The exception's what() function returned \"{1}\".",
            type_id<std::exception>(),
            unpack_string_to_u8string(_a_exception.what())
        )
    ));
}

__no_constexpr_imp matcher_t
    unexpected_exception_of_unknown_type_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        fmt::format(u8"An unexpected exception of an unknown type was thrown.")
    ));
}

_END_ABC_NS

_BEGIN_ABC_NS

__no_constexpr_imp matcher_t
    c_str_equal(
        const char*            _a_str1,
        const std::string_view _a_str2
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    const bool _l_equal{std::strcmp(_a_str1, _a_str2.data()) == 0};
    return mk_matcher_using_result(matcher_result_t(
        _l_equal,
        fmt::format(
            u8"{0} {1} {2}",
            convert_string_to_u8string(_a_str1).value(),
            _l_equal ? u8"==" : u8"!=",
            convert_string_to_u8string(_a_str2).value()
        )
    ));
}

_END_ABC_NS