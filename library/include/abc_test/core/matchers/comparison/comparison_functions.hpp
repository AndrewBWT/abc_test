#pragma once

#include "abc_test/core/matchers/comparison/comparison_enum.hpp"
#include "abc_test/core/matchers/matcher_wrapper.hpp"
#include "abc_test/utility/printers/default_printer.hpp"

_BEGIN_ABC_MATCHER_NS

namespace
{
template <comparison_enum_t Cmp, typename T1, typename T2>
__constexpr matcher_result_t
    make_matcher_result(T1&& _a_t1, T2&& _a_t2) noexcept;
template <typename T>
__constexpr std::u8string
            format_str(const T& _a_element) noexcept;
template <typename T1, typename T2, comparison_enum_t Cmp_Enum>
__constexpr matcher_t
    make_cmp_matcher(T1&& _a_left_arg, T2&& _a_right_arg) noexcept;
} // namespace

_END_ABC_MATCHER_NS
_BEGIN_ABC_NS
/*!
 * @brief Factory function used to create equality matcher.
 * @tparam T1 The type of the first argument.
 * @tparam T2 Type of the second argument.
 * @param _a_left_arg The T1 arg.
 * @param _a_right_arg The T2 arg.
 * @return True if _a_left_arg == _a_right_arg, false otherwise.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    eq(T1&& _a_left_arg, T2&& _a_right_arg) noexcept;
/*!
 * @brief Factory function used to create inequality matcher.
 * @tparam T1 The type of the first argument.
 * @tparam T2 Type of the second argument.
 * @param _a_left_arg The T1 arg.
 * @param _a_right_arg The T2 arg.
 * @return True if _a_left_arg != _a_right_arg, false otherwise.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    neq(T1&& _a_left_arg, T2&& _a_right_arg) noexcept;
/*!
 * @brief Factory function used to create less-than matcher.
 * @tparam T1 The type of the first argument.
 * @tparam T2 Type of the second argument.
 * @param _a_left_arg The T1 arg.
 * @param _a_right_arg The T2 arg.
 * @return True if _a_left_arg < _a_right_arg, false otherwise.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    lt(T1&& _a_left_arg, T2&& _a_right_arg) noexcept;
/*!
 * @brief Factory function used to create less-than-or-equal-to matcher.
 * @tparam T1 The type of the first argument.
 * @tparam T2 Type of the second argument.
 * @param _a_left_arg The T1 arg.
 * @param _a_right_arg The T2 arg.
 * @return True if _a_left_arg <= _a_right_arg, false otherwise.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    leq(T1&& _a_left_arg, T2&& _a_right_arg) noexcept;
/*!
 * @brief Factory function used to create greater-than matcher.
 * @tparam T1 The type of the first argument.
 * @tparam T2 Type of the second argument.
 * @param _a_left_arg The T1 arg.
 * @param _a_right_arg The T2 arg.
 * @return True if _a_left_arg > _a_right_arg, false otherwise.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    gt(T1&& _a_left_arg, T2&& _a_right_arg) noexcept;
/*!
 * @brief Factory function used to create greater-than-or-equal-to matcher.
 * @tparam T1 The type of the first argument.
 * @tparam T2 Type of the second argument.
 * @param _a_left_arg The T1 arg.
 * @param _a_right_arg The T2 arg.
 * @return True if _a_left_arg >= _a_right_arg, false otherwise.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    geq(T1&& _a_left_arg, T2&& _a_right_arg) noexcept;

_END_ABC_NS

_BEGIN_ABC_MATCHER_NS

namespace
{
template <comparison_enum_t Cmp, typename T1, typename T2>
__constexpr matcher_result_t
    make_matcher_result(
        T1&& _a_t1,
        T2&& _a_t2
    ) noexcept
{
    using namespace std;
    u8string   _l_left_str{format_str<T1>(std::forward<T1>(_a_t1))};
    u8string   _l_right_str{format_str<T2>(std::forward<T2>(_a_t2))};
    const bool _l_result{
        cmp<T1, T2, Cmp>(std::forward<T1>(_a_t1), std::forward<T2>(_a_t2))
    };
    return matcher_result_t(
        _l_result,
        matcher_result_infos_t(fmt::format(
            u8"{0} {1} {2}",
            _l_left_str,
            _l_result ? cmp_str<Cmp>() : not_cmp_str<Cmp>(),
            _l_right_str
        ))
    );
}

template <comparison_enum_t Cmp, typename T1, typename T2>
struct matcher_default_comparable_t
{
public:
    static constexpr bool is_specialized{false};
    __constexpr virtual matcher_result_t
        run(const T1& _a_arg1, const T2& _a_arg2) const;
};

template <typename T1, typename T2, comparison_enum_t Cmp>
concept is_matcher_default_comparable_c
    = (matcher_default_comparable_t<Cmp, T1, T2>::is_specialized == true);

template <comparison_enum_t Cmp, typename T1, typename T2>
__constexpr matcher_result_t
    matcher_default_comparable_t<Cmp, T1, T2>::run(
        const T1& _a_arg1,
        const T2& _a_arg2
    ) const
{
    using namespace std;
    u8string   _l_left_str{format_str<T1>(_a_arg1)};
    u8string   _l_right_str{format_str<T2>(_a_arg2)};
    const bool _l_result{cmp<T1, T2, Cmp>(_a_arg1, _a_arg2)};
    return matcher_result_t(
        _l_result,
        matcher_result_infos_t(fmt::format(
            u8"{0} {1} {2}",
            _l_left_str,
            _l_result ? cmp_str<Cmp>() : not_cmp_str<Cmp>(),
            _l_right_str
        ))
    );
}

template <typename T>
__constexpr_imp std::u8string
                format_str(
                    const T& _a_element
                ) noexcept
{
    using namespace std;
    u8string _l_rv{u8"[?]"};
    using Stripped_Type = typename std::remove_cvref<T>::type;
    if constexpr (abc::utility::printer::default_printable<Stripped_Type>)
    {
        _l_rv = abc::utility::printer::default_printer<Stripped_Type>()
                    ->run_printer(_a_element);
    }
    else if constexpr (fmt::formattable<T>)
    {
        _l_rv = fmt::format(u8"{} formattable", _a_element);
    }
    return _l_rv;
}

template <typename T1, typename T2, comparison_enum_t Cmp_Enum>
__constexpr_imp matcher_t
    make_cmp_matcher(
        T1&& _a_left_arg,
        T2&& _a_right_arg
    ) noexcept
{
    using namespace std;
    matcher_result_t _l_mr;
    using Stripped_Type_T1 = typename std::remove_cvref<T1>::type;
    using Stripped_Type_T2 = typename std::remove_cvref<T1>::type;
    if constexpr (is_matcher_default_comparable_c<
                      Stripped_Type_T1,
                      Stripped_Type_T2,
                      Cmp_Enum>)
    {
        _l_mr = matcher_default_comparable_t<
                    Cmp_Enum,
                    Stripped_Type_T1,
                    Stripped_Type_T2>()
                    .run(
                        std::forward<T1>(_a_left_arg),
                        std::forward<T2>(_a_right_arg)
                    );
    }
    else
    {
        _l_mr = make_matcher_result<Cmp_Enum>(
            std::forward<T1>(_a_left_arg), std::forward<T2>(_a_right_arg)
        );
    }
    return mk_matcher_using_result_and_precedence(
        _l_mr, cmp_precedence<Cmp_Enum>()

    );
} // namespace
} // namespace

_END_ABC_MATCHER_NS
_BEGIN_ABC_NS

template <typename T1, typename T2>
__constexpr_imp matcher_t
    eq(
        T1&& _a_left_arg,
        T2&& _a_right_arg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using enum comparison_enum_t;
    return make_cmp_matcher<T1, T2, EQ>(
        std::forward<T1>(_a_left_arg), std::forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    neq(
        T1&& _a_left_arg,
        T2&& _a_right_arg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using enum comparison_enum_t;
    return make_cmp_matcher<T1, T2, NEQ>(
        std::forward<T1>(_a_left_arg), std::forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    lt(
        T1&& _a_left_arg,
        T2&& _a_right_arg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using enum comparison_enum_t;
    return make_cmp_matcher<T1, T2, LT>(
        std::forward<T1>(_a_left_arg), std::forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    leq(
        T1&& _a_left_arg,
        T2&& _a_right_arg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using enum comparison_enum_t;
    return make_cmp_matcher<T1, T2, LEQ>(
        std::forward<T1>(_a_left_arg), std::forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    gt(
        T1&& _a_left_arg,
        T2&& _a_right_arg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using enum comparison_enum_t;
    return make_cmp_matcher<T1, T2, GT>(
        std::forward<T1>(_a_left_arg), std::forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    geq(
        T1&& _a_left_arg,
        T2&& _a_right_arg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using enum comparison_enum_t;
    return make_cmp_matcher<T1, T2, GEQ>(
        std::forward<T1>(_a_left_arg), std::forward<T2>(_a_right_arg)
    );
}

_END_ABC_NS