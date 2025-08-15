#pragma once


#include "abc_test/core/matchers/matcher_wrapper.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
#include "abc_test/utility/str/string_utils.hpp"

_BEGIN_ABC_NS
__no_constexpr matcher_t
    c_str_equal(const char* _a_str1, const std::string_view _a_str2) noexcept;

template <matcher::comparison_enum_t Cmp, typename T1, typename T2>
__constexpr_imp std::u8string
                comparison_expr_to_string(
                    const T1&  _a_arg1,
                    const T2&  _a_arg2,
                    const bool _a_pass
                ) noexcept
{
    using namespace std;
    using namespace abc::utility::printer;
    const u8string _l_cmp_str{
        (_a_pass) ? matcher::cmp_str<Cmp>() : matcher::not_cmp_str<Cmp>()
    };
    return fmt::format(
        u8"{0} {1} {2}",
        default_printer_t<T1>{}.run_printer(_a_arg1),
        _l_cmp_str,
        default_printer_t<T2>{}.run_printer(_a_arg2)
    );
}

template <typename R1, typename R2, abc::matcher::comparison_enum_t Cmp>
__constexpr_imp std::pair<bool, std::vector<std::u8string>>
                compare_strings(
                    const R1& _a_str1,
                    const R2& _a_str2
                ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace abc::utility::printer;
    using T            = ranges::range_value_t<R1>;
    using R1_const_itt = decltype(std::begin(_a_str1));
    using R2_const_itt = decltype(std::begin(_a_str2));
    R1_const_itt                                _l_itt_1{std::begin(_a_str1)};
    R1_const_itt                                _l_itt_1_end{std::end(_a_str1)};
    R2_const_itt                                _l_itt_2{std::begin(_a_str2)};
    R2_const_itt                                _l_itt_2_end{std::end(_a_str2)};
    optional<tuple<size_t, u8string, u8string>> _l_diverging_index;
    size_t                                      _l_idx{1};
    vector<u8string>                            _l_explanation_strs;
    auto                                        _l_specific_character_print_func
        = [&](const T _a_char, const size_t _a_index, const size_t _a_char_idx
          ) -> u8string
    {
        optional<T> _l_arg_as_single_char;
        if constexpr (same_as<char, T>)
        {
            if (is_valid_ascii_char(_a_char))
            {
                _l_arg_as_single_char = _a_char;
            }
        }
        else
        {
            if (is_valid_char(_a_char))
            {
                _l_arg_as_single_char = _a_char;
            }
        }
        auto _l_single_char_to_unicode_str = [&](const T _a_single_char)
        {
            if constexpr (same_as<T, char>)
            {
                return cast_string_to_u8string(string(1, _a_single_char));
            }
            else
            {
                return unicode_string_to_u8string(
                    basic_string<T>(1, _a_single_char)
                );
            }
        };
        return fmt::format(
            u8"The {0} {1} element in the {2} argument {3}. "
            u8"In hex, this element is {4}.",
            positive_integer_to_placement(_a_char_idx),
            type_id<T>(),
            positive_integer_to_placement(_a_index),
            (_l_arg_as_single_char.has_value()
                 ? fmt::format(
                       u8"can be represented as the single character "
                       u8"'{0}'",
                       _l_single_char_to_unicode_str(
                           _l_arg_as_single_char.value()
                       )
                   )
                 : fmt::format(u8"cannot be represented as a single character")
            ),
            represent_char_as_hex_for_output(_a_char)
        );
    };
    auto _l_surrounding_str_print_func
        = [&](auto& _a_str, const size_t _a_idx, const size_t _a_argument_idx
          ) -> u8string
    {
        return fmt::format(
            u8"To aid in locating the error, we have constructed a sub-string "
            u8"from the {0} argument, concenrated around its {1} element. The "
            u8"sub-string is as follows: \"...{3}...\".",
            positive_integer_to_placement(_a_argument_idx),
            positive_integer_to_placement(_a_idx),
            _a_argument_idx,
            default_printer_t<basic_string<T>>{u8""}.run_printer(
                make_focused_string(_a_str, _a_idx)
            )
        );
    };
    auto _l_func_add_types_to_explanation_str = [&]()
    {
        const u8string _l_suffix{fmt::format(
            u8"Both argument's internal elements are of type \"{0}\".",
            type_id<T>()
        )};
        if constexpr (same_as<R1, R2>)
        {
            _l_explanation_strs.push_back(fmt::format(
                u8"Both the {0} and the {1} arguments are of type "
                u8"\"{2}\". {3}",
                positive_integer_to_placement<1>(),
                positive_integer_to_placement<2>(),
                type_id<R1>(),
                _l_suffix
            ));
        }
        else
        {
            _l_explanation_strs.push_back(fmt::format(
                u8"The {0} argument's type is \"{1}\", while the "
                u8"{2} argument's "
                u8"type is \"{3}\". {4}",
                positive_integer_to_placement<1>(),
                type_id<R1>(),
                positive_integer_to_placement<2>(),
                type_id<R2>(),
                _l_suffix
            ));
        }
    };
    while (_l_itt_1 != _l_itt_1_end && _l_itt_2 != _l_itt_2_end)
    {
        const T& _l_element_1{*_l_itt_1};
        const T& _l_element_2{*_l_itt_2};
        if constexpr (Cmp == matcher::comparison_enum_t::EQ)
        {
            if (_l_element_1 != _l_element_2)
            {
                _l_diverging_index = make_tuple(
                    _l_idx,
                    fmt::format(
                        u8"the {0} and the {1} argument are equal",
                        positive_integer_to_placement<1>(),
                        positive_integer_to_placement<2>()
                    ),
                    fmt::format(
                        u8"internal elements of the {0} and {1} argument are "
                        u8"not equal",
                        positive_integer_to_placement<1>(),
                        positive_integer_to_placement<2>()
                    )
                );
                break;
            }
        }
        else if constexpr (Cmp == matcher::comparison_enum_t::NEQ)
        {
            if (_l_element_1 == _l_element_2)
            {
                _l_diverging_index = make_tuple(
                    _l_idx,
                    fmt::format(
                        u8"the {0} and the {1} argument are not equal",
                        positive_integer_to_placement<1>(),
                        positive_integer_to_placement<2>()
                    ),
                    fmt::format(
                        u8"internal elements of the {0} and {1} argument are "
                        u8"equal",
                        positive_integer_to_placement<1>(),
                        positive_integer_to_placement<2>()
                    )
                );
                break;
            }
        }
        else if constexpr (Cmp == matcher::comparison_enum_t::LEQ)
        {
            if (_l_element_1 > _l_element_2)
            {
                _l_diverging_index = make_tuple(
                    _l_idx,
                    u8"arguments are equal",
                    u8"elements of the arguments are not equa"
                );
                break;
            }
        }
        else if constexpr (Cmp == matcher::comparison_enum_t::LT)
        {
            if (_l_element_1 >= _l_element_2)
            {
                _l_diverging_index = make_tuple(
                    _l_idx,
                    u8"arguments are equal",
                    u8"elements of the arguments are not equa"
                );
                break;
            }
        }
        else if constexpr (Cmp == matcher::comparison_enum_t::GEQ)
        {
            if (_l_element_1 < _l_element_2)
            {
                _l_diverging_index = make_tuple(
                    _l_idx,
                    u8"arguments are equal",
                    u8"elements of the arguments are not equa"
                );
                break;
            }
        }
        else if constexpr (Cmp == matcher::comparison_enum_t::GT)
        {
            if (_l_element_1 <= _l_element_2)
            {
                _l_diverging_index = make_tuple(
                    _l_idx,
                    u8"arguments are equal",
                    u8"elements of the arguments are not equa"
                );
                break;
            }
        }
        else
        {
            __STATIC_ASSERT_NO_TYPE("Couldn't work out");
        }
        ++_l_itt_1;
        ++_l_itt_2;
        ++_l_idx;
    }
    if (not _l_diverging_index.has_value())
    {
        // Atleast one of the strings must be at the end.
        const bool _l_left_at_end{_l_itt_1 == _l_itt_1_end};
        if (_l_left_at_end && (_l_itt_2 == _l_itt_2_end))
        {
            if constexpr (Cmp == matcher::comparison_enum_t::EQ
                          || Cmp == matcher::comparison_enum_t::LEQ
                          || Cmp == matcher::comparison_enum_t::GEQ)
            {
                return {true, {}};
            }
            else
            {
                _l_func_add_types_to_explanation_str();
                _l_explanation_strs.push_back(fmt::format(
                    u8"The matcher determined that the {0} and the "
                    u8"{1} argument are equal, which fails the "
                    u8"matcher's test.",
                    positive_integer_to_placement<1>(),
                    positive_integer_to_placement<2>()
                ));
                return {false, _l_explanation_strs};
            }
        }
        else if (_l_left_at_end)
        {
            if constexpr (Cmp == matcher::comparison_enum_t::LT
                          || Cmp == matcher::comparison_enum_t::LEQ
                          || Cmp == matcher::comparison_enum_t::NEQ)
            {
                return {true, _l_explanation_strs};
            }
            else
            {
                _l_func_add_types_to_explanation_str();
                _l_explanation_strs.push_back(fmt::format(
                    u8"TThe {0} argument is smaller in size than the "
                    u8"{1} argument, causing the matcher's test to fail.",
                    positive_integer_to_placement<1>(),
                    positive_integer_to_placement<2>()
                ));
                return {false, _l_explanation_strs};
            }
        }
        else
        {
            if constexpr (Cmp == matcher::comparison_enum_t::GT
                          || Cmp == matcher::comparison_enum_t::GEQ
                          || Cmp == matcher::comparison_enum_t::NEQ)
            {
                return {true, _l_explanation_strs};
            }
            else
            {
                _l_func_add_types_to_explanation_str();
                _l_explanation_strs.push_back(fmt::format(
                    u8"TThe {0} argument is bigger in size than the "
                    u8"{1} argument, causing the matcher's test to fail.",
                    positive_integer_to_placement<1>(),
                    positive_integer_to_placement<2>()
                ));
                return {false, _l_explanation_strs};
            }
        }
    }
    else
    {
        _l_func_add_types_to_explanation_str();
        optional<u8string> _l_char_in_hex;
        _l_explanation_strs.push_back(fmt::format(
            u8"The first failure point encountered in both "
            u8"arguments is at index {0}. The matcher is checking if "
            u8"{1}. At index {0} the "
            u8"{2}.",
            _l_idx,
            get<1>(_l_diverging_index.value()),
            get<2>(_l_diverging_index.value())
        ));
        _l_explanation_strs.push_back(
            _l_specific_character_print_func(*_l_itt_1, 1, _l_idx)
        );
        _l_explanation_strs.push_back(
            _l_specific_character_print_func(*_l_itt_2, 2, _l_idx)
        );
        _l_explanation_strs.push_back(
            _l_surrounding_str_print_func(_a_str1, _l_idx, 1)
        );
        _l_explanation_strs.push_back(
            _l_surrounding_str_print_func(_a_str2, _l_idx, 2)
        );
        return {false, _l_explanation_strs};
    }
}

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
        const bool _l_left_smaller{_l_itt_1 == _l_itt_1_end};
        if (not _l_left_smaller && (_l_itt_2 == _l_itt_2_end))
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
        if (not (_l_left_smaller || _l_itt_2 == _l_itt_2_end))
        {
            _l_explanation_str = fmt::format(
                u8"{0} argument (size {1}) smaller than {2} argument ({3})",
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
        _l_explanation_str = fmt::format(
            u8"Strings diverge at index {0}. Left argument's index {0} = {1}, "
            u8"and "
            u8"right argument's index {0} = {2}. The surrounding string of the "
            u8"left and right arguments are \"...{3}...\" and \"...{4}...\"",
            _l_idx,
            default_printer_t<T>{}.run_printer(*_l_itt_1),
            default_printer_t<T>{}.run_printer(*_l_itt_2),
            default_printer_t<R1_Type>{u8""}.run_printer(
                make_focused_string(_a_range_1, _l_idx)
            ),
            default_printer_t<R2_Type>{u8""}.run_printer(
                make_focused_string(_a_range_2, _l_idx)
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

_END_ABC_NS
_BEGIN_ABC_MATCHER_NS

template <comparison_enum_t Cmp, typename T>
struct matcher_default_comparable_t<
    Cmp,
    std::basic_string_view<T>,
    std::basic_string_view<T>>
{
public:
    static constexpr bool is_specialized{true};

    __constexpr virtual matcher_result_t
        run(
            const std::basic_string_view<T>& _a_arg1,
            const std::basic_string_view<T>& _a_arg2
        ) const
    {
        using namespace std;
        const auto& [_l_passed, _l_explanation_strs]{
            compare_strings<basic_string_view<T>, basic_string_view<T>, Cmp>(
                _a_arg1, _a_arg2
            )
        };
        return matcher_result_t(
            _l_passed,
            matcher_result_infos_t(
                comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, _l_passed),
                _l_explanation_strs
            )
        );
    }
};

template <comparison_enum_t Cmp, typename T>
struct matcher_default_comparable_t<
    Cmp,
    std::basic_string<T>,
    std::basic_string<T>>
{
public:
    static constexpr bool is_specialized{true};

    __constexpr virtual matcher_result_t
        run(
            const std::basic_string<T>& _a_arg1,
            const std::basic_string<T>& _a_arg2
        ) const
    {
        using namespace std;
        return matcher_default_comparable_t<
                   Cmp,
                   basic_string_view<T>,
                   basic_string_view<T>>{}
            .run(_a_arg1, _a_arg2);
    }
};

template <comparison_enum_t Cmp, typename T, typename U>
struct matcher_default_comparable_t<
    Cmp,
    std::expected<T, U>,
    std::expected<T, U>>
{
public:
    static constexpr bool is_specialized{true};

    __constexpr virtual matcher_result_t
        run(
            const std::expected<T, U>& _a_arg1,
            const std::expected<T, U>& _a_arg2
        ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        matcher_result_t _l_result;
        vector<u8string> _l_explanation_strs;
        auto _l_same_type_explanation_func = [&](const size_t _a_idx)
        {
            _l_explanation_strs.push_back(fmt::format(
                u8"Both arguments, of type \"{0}\", have a value inhabiting "
                u8"the "
                u8"{1} type \"{2}\". Due to the result from the sub-matcher "
                u8"comparing these values, this matcher fails.",
                type_id<decltype(_a_arg1)>(),
                positive_integer_to_placement(_a_arg1.has_value() ? 0u : 1u),
                _a_arg1.has_value() ? type_id<T>() : type_id<U>()
            ));
        };
        if (_a_arg1.has_value() && _a_arg2.has_value())
        {
            _l_result = matcher_default_comparable_t<Cmp, T, T>().run(
                _a_arg1.value(), _a_arg2.value()
            );
            _l_same_type_explanation_func(1);
        }
        else if (not (_a_arg1.has_value()) && not (_a_arg2.has_value()))
        {
            _l_result = matcher_default_comparable_t<Cmp, U, U>().run(
                _a_arg1.error(), _a_arg2.error()
            );
            _l_same_type_explanation_func(2);
        }
        else
        {
            using arg_type_t = decltype(_a_arg1);
            if (_a_arg1.has_value())
            {
                if constexpr (Cmp == matcher::comparison_enum_t::LT
                              || Cmp == matcher::comparison_enum_t::LEQ
                              || Cmp == matcher::comparison_enum_t::NEQ)
                {
                    return matcher_result_t(
                        true,
                        matcher_result_infos_t(comparison_expr_to_string<Cmp>(
                            _a_arg1, _a_arg2, true
                        ))
                    );
                }
            }
            if (_a_arg2.has_value())
            {
                if constexpr (Cmp == matcher::comparison_enum_t::GT
                              || Cmp == matcher::comparison_enum_t::GEQ
                              || Cmp == matcher::comparison_enum_t::NEQ)
                {
                    return matcher_result_t(
                        true,
                        matcher_result_infos_t(comparison_expr_to_string<Cmp>(
                            _a_arg1, _a_arg2, true
                        ))
                    );
                }
            }
            auto _l_add_reaosn_to_explanation
                = [&](const std::expected<T, U>& _a_arg,
                      const size_t               _a_expected_idx)
            {
                const size_t   _l_type_idx = _a_arg.has_value() ? 1 : 2;
                const u8string _l_type_id
                    = _a_arg.has_value() ? type_id<T>() : type_id<U>();
                _l_explanation_strs.push_back(fmt::format(
                    u8"The {0} argument has a value of type \"{1}\", "
                    u8"inhabiting "
                    u8"the {2} index in the ovearching type.",
                    positive_integer_to_placement(_a_expected_idx),
                    _l_type_id,
                    positive_integer_to_placement(_l_type_idx)
                ));
            };
            _l_explanation_strs.push_back(fmt::format(
                u8"Both arguments, of type \"{0}\", have values which "
                u8"inhabit "
                u8"different type indexes.",
                type_id<arg_type_t>()
            ));
            _l_add_reaosn_to_explanation(_a_arg1, 1);
            _l_add_reaosn_to_explanation(_a_arg2, 2);
            _l_explanation_strs.push_back(fmt::format(
                u8"When comparing the type indexes using the comparison "
                u8"operator \"{0}\", the matcher fails.",
                cmp_str<Cmp>()
            ));
            return matcher_result_t(
                false,
                matcher_result_infos_t(
                    comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, false),
                    _l_explanation_strs
                )
            );
        }
        if (_l_result.passed())
        {
            return matcher_result_t(
                true,
                matcher_result_infos_t(
                    comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, true)
                )
            );
        }
        else
        {
            return matcher_result_t(
                false,
                matcher_result_infos_t(
                    comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, true),
                    _l_explanation_strs,
                    vector<matcher_result_infos_t::tree_structure_t>(
                        1,
                        make_tuple(
                            u8"Child",
                            u8"C",
                            make_shared<matcher_result_infos_t>(_l_result.str())
                        )
                    )
                )
            );
        }
    }
};

template <comparison_enum_t Cmp, typename T>
struct matcher_default_comparable_t<Cmp, std::optional<T>, std::optional<T>>
{
public:
    static constexpr bool is_specialized{true};

    __constexpr virtual matcher_result_t
        run(
            const std::optional<T>& _a_arg1,
            const std::optional<T>& _a_arg2
        ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        matcher_result_t _l_result;
        vector<u8string> _l_explanation_strs;
        using arg_type_t                   = decltype(_a_arg1);
        auto _l_same_type_explanation_func = [&]()
        {
            _l_explanation_strs.push_back(fmt::format(
                u8"Both arguments, of type \"{0}\", have a value inhabiting "
                u8"the "
                u8"type \"{1}\". Due to the result from the sub-matcher "
                u8"comparing these values, this matcher fails.",
                type_id<decltype(_a_arg1)>(),
                type_id<T>()
            ));
        };
        if (_a_arg1.has_value() && _a_arg2.has_value())
        {
            _l_result = matcher_default_comparable_t<Cmp, T, T>().run(
                _a_arg1.value(), _a_arg2.value()
            );
            _l_same_type_explanation_func();
            return matcher_result_t(
                false,
                matcher_result_infos_t(
                    comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, true),
                    _l_explanation_strs,
                    vector<matcher_result_infos_t::tree_structure_t>(
                        1,
                        make_tuple(
                            u8"Child",
                            u8"C",
                            make_shared<matcher_result_infos_t>(_l_result.str())
                        )
                    )
                )
            );
        }
        else if (_a_arg1.has_value())
        {
            if constexpr (Cmp == matcher::comparison_enum_t::GT
                          || Cmp == matcher::comparison_enum_t::GEQ
                          || Cmp == matcher::comparison_enum_t::NEQ)
            {
                return matcher_result_t(
                    true,
                    matcher_result_infos_t(
                        comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, true)
                    )
                );
            }
            else
            {
                return matcher_result_t(
                    false,
                    matcher_result_infos_t(
                        comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, false)
                    )
                );
            }
        }
        else if (_a_arg2.has_value())
        {
            if constexpr (Cmp == matcher::comparison_enum_t::LT
                          || Cmp == matcher::comparison_enum_t::LEQ
                          || Cmp == matcher::comparison_enum_t::NEQ)
            {
                return matcher_result_t(
                    true,
                    matcher_result_infos_t(
                        comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, true)
                    )
                );
            }
            else
            {
                return matcher_result_t(
                    false,
                    matcher_result_infos_t(
                        comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, false)
                    )
                );
            }
        }
        else
        {
            if constexpr (Cmp == matcher::comparison_enum_t::LEQ
                || Cmp == matcher::comparison_enum_t::GEQ
                || Cmp == matcher::comparison_enum_t::EQ)
            {
                return matcher_result_t(
                    true,
                    matcher_result_infos_t(
                        comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, true)
                    )
                );
            }
            else
            {
                return matcher_result_t(
                    false,
                    matcher_result_infos_t(
                        comparison_expr_to_string<Cmp>(_a_arg1, _a_arg2, false)
                    )
                );
            }
        }
    }
};

_END_ABC_MATCHER_NS

_BEGIN_ABC_NS
/*template <bool Expected_Behaviour>
__no_constexpr_imp matcher_t
    no_exception_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    bool     _l_result{true};
    u8string _l_str;
    if constexpr (not Expected_Behaviour)
    {
        _l_result = false;
        _l_str    = u8"not ";
    }
    return mk_matcher_using_result(matcher_result_t(
        _l_result,
        matcher_result_infos_t(fmt::format(
            u8"No exception was thrown, which is {0}the expected behaviour",
            _l_str
        ))
    ));
}

template <bool Expected_Behaviour>
__no_constexpr_imp matcher_t
    exception_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    bool     _l_result{true};
    u8string _l_str;
    if constexpr (not Expected_Behaviour)
    {
        _l_result = false;
        _l_str    = u8"not ";
    }
    return mk_matcher_using_result(matcher_result_t(
        _l_result,
        matcher_result_infos_t(fmt::format(
            u8"An exception was thrown, which is {0}the expected "
            u8"behaviour. The exception is of an unknown type.",
            _l_str
        ))
    ));
}

template <bool Expected_Behaviour, typename T>
requires std::derived_from<T, std::exception>
__no_constexpr_imp matcher_t
    exception_thrown(
        const T& _a_exception
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    bool     _l_result{true};
    u8string _l_str;
    if constexpr (not Expected_Behaviour)
    {
        _l_result = false;
        _l_str    = u8"not ";
    }
    return mk_matcher_using_result(matcher_result_t(
        _l_result,
        matcher_result_infos_t(fmt::format(
            u8"An arbitrary exception was thrown, which is {0}the expected "
            u8"behaviour. "
            u8"TThe exception is of type \"{1}\". Its "
            u8"what() function return the string \"{2}\".",
            _l_str,
            type_id<T>(),
            _a_exception.what()
        ))
    ));
}

template <typename T>
__no_constexpr_imp matcher_t
    exception_thrown_with_expected_type() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    u8string _l_str;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"An exception of an unknown type was thrown, which is not the "
            u8"expected behaviour. An exception of type \"{0}\" was expected. ",
            _l_str
        ))
    ));
}

template <typename Expected_Base, typename U>
__no_constexpr_imp matcher_t
    exception_thrown_with_expected_type(
        const U& _a_exception
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    u8string _l_additional_string;
    if constexpr (derived_from<U, exception>)
    {
        _l_additional_string = fmt::format(
            u8" The thrown exception's what() function returned \"{0}\".",
            unpack_string_to_u8string(_a_exception.what())
        );
    }
    if constexpr (derived_from<U, Expected_Base>)
    {
        return mk_matcher_using_result(matcher_result_t(
            true,
            matcher_result_infos_t(fmt::format(
                u8"An exception of type \"{0}\" was thrown, which is the "
                u8"expected behaviour. This is because \"{0}\" is derived from "
                u8"the expected exception type \"{1}\".{2}",
                type_id<U>(),
                type_id<Expected_Base>(),
                _l_additional_string
            ))
        ));
    }
    else
    {
        return mk_matcher_using_result(matcher_result_t(
            false,
            matcher_result_infos_t(fmt::format(
                u8"An exception of type \"{0}\" was thrown, which is not the "
                u8"expected behaviour. This is because \"{0}\" is not derived "
                u8"from "
                u8"the expected exception type \"{1}\".{2}",
                type_id<U>(),
                type_id<Expected_Base>(),
                _l_additional_string
            ))
        ));
    }
}

template <typename U>
requires std::derived_from<U, std::exception>
__no_constexpr_imp matcher_t
    exception_thrown_with_expected_msg(
        const std::u8string_view _a_expected_msg,
        const U&                 _a_exception
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    u8string _l_exceptions_string, _l_additional_string;
    _l_exceptions_string = unpack_string_to_u8string(_a_exception.what());
    auto _l_str_matcher{matcher_default_comparable_t<
                            decltype(_l_exceptions_string),
                            decltype(_l_exceptions_string),
                            matcher::comparison_enum_t::EQ>(
                            _a_expected_msg, _l_exceptions_string
    )
                            .run()};
    if (_l_str_matcher.passed())
    {
        return mk_matcher_using_result(matcher_result_t(
            true,
            matcher_result_infos_t(fmt::format(
                u8"An exception was thrown whose what() function returned "
                u8"a string which matched the expected string. The "
                u8"thrown exception is of type \"{0}\". The exception's "
                u8"what() function returned \"{1}\".",
                type_id<U>(),
                _l_exceptions_string
            ))
        ));
    }
    else
    {
        return mk_matcher_using_result(matcher_result_t(
            false,
            matcher_result_infos_t(
                fmt::format(
                    u8"An exception was thrown whose what() function "
                    u8"returned a string which did not match the expected "
                    u8"string. The thrown exception is of type \"{0}\". "
                    u8"The information pertaining to the strings is shown "
                    u8"below",
                    type_id<U>()
                ),
                _l_str_matcher.str().get_vector()
            )
        ));
    }
}

__no_constexpr_imp matcher_t
exception_thrown_with_expected_msg(
    const std::u8string_view _a_str
) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"An exception was thrown which was not derived from "
            u8"\"{0}\", and was of an unknown type."
            u8"Therefore, we could not check if the exception's what() "
            u8"function returned the string \"{1}\" (as we didn't know if it "
            u8"had one). This was "
            u8"unexpected behaviour.",
            type_id<std::exception>(),
            _a_str
        ))
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
        matcher_result_infos_t(fmt::format(
            u8"An unexpected exception, of a type derived from {0} was thrown. "
            u8"The exception's what() function returned \"{1}\".",
            type_id<std::exception>(),
            unpack_string_to_u8string(_a_exception.what())
        ))
    ));
}

template <typename T>
__no_constexpr_imp matcher_t
    unexpected_exception_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"It was expected that an exception of type {0} was to be thrown. "
            u8"Instead, an unexpected exception of an unknown type was thrown.",
            type_id<T>()
        ))
    ));
}

__no_constexpr_imp matcher_t
    unexpected_exception_of_unknown_type_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"An unexpected exception of an unknown type was thrown."
        ))
    ));
}

template <typename R1, typename R2>
__constexpr_imp matcher_t
    check_exception_string(
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
    using R1_const_itt = decltype(std::begin(_a_range_1));
    using R2_const_itt = decltype(std::begin(_a_range_2));
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
        const bool _l_left_smaller{_l_itt_1 == _l_itt_1_end};
        if (not _l_left_smaller && (_l_itt_2 == _l_itt_2_end))
        {
            return mk_matcher_using_result(matcher_result_t(
                true,
                matcher_result_infos_t(fmt::format(
                    u8"Exception's what() message returned the expected "
                    u8"string. {0} == {1}",
                    default_printer_t<R1_Type>{}.run_printer(_a_range_1),
                    default_printer_t<R2_Type>{}.run_printer(_a_range_2)
                ))
            ));
        }
        if (not (_l_left_smaller || _l_itt_2 == _l_itt_2_end))
        {
            _l_explanation_str = fmt::format(
                u8"{0} argument (size {1}) smaller than {2} argument ({3})",
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
                matcher_result_infos_t(fmt::format(
                    u8"Exception's what() message returned the expected "
                    u8"string. {0} == {1}",
                    default_printer_t<R1_Type>{}.run_printer(_a_range_1),
                    default_printer_t<R2_Type>{}.run_printer(_a_range_2)
                ))
            ));
        }
    }
    else
    {
        _l_explanation_str = fmt::format(
            u8"Strings diverge at index {0}. Left argument's index {0} = {1}, "
            u8"and "
            u8"right argument's index {0} = {2}. The surrounding string of the "
            u8"left and right arguments are \"...{3}...\" and \"...{4}...\"",
            _l_idx,
            default_printer_t<T>{}.run_printer(*_l_itt_1),
            default_printer_t<T>{}.run_printer(*_l_itt_2),
            default_printer_t<R1_Type>{u8""}.run_printer(
                make_focused_string(_a_range_1, _l_idx)
            ),
            default_printer_t<R2_Type>{u8""}.run_printer(
                make_focused_string(_a_range_2, _l_idx)
            )

        );
    }
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"Exception's what() message did not return the expected string. "
            u8"The exception's what() message ({0}) != the expected message "
            u8"({1}). {2}.",
            default_printer_t<R1_Type>{}.run_printer(_a_range_1),
            default_printer_t<R2_Type>{}.run_printer(_a_range_2),
            _l_explanation_str
        ))
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
        matcher_result_infos_t(fmt::format(
            u8"{0} {1} {2}",
            unpack_string_to_u8string(_a_str1),
            _l_equal ? u8"==" : u8"!=",
            unpack_string_to_u8string(_a_str2)
        ))
    ));
}*/

_END_ABC_NS