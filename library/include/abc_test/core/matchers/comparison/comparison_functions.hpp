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
__constexpr std::string
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
    string     _l_left_str{format_str<T1>(forward<T1>(_a_t1))};
    string     _l_right_str{format_str<T2>(forward<T2>(_a_t2))};
    const bool _l_result{
        cmp<T1, T2, Cmp>(forward<T1>(_a_t1), forward<T2>(_a_t2))
    };
    return matcher_result_t(
        _l_result,
        fmt::format(
            "{0} {1} {2}",
            _l_left_str,
            (_l_result ? cmp_str<Cmp>() : not_cmp_str<Cmp>()),
            _l_right_str
        )
    );
}

template <typename T>
__constexpr_imp std::string
                format_str(
                    const T& _a_element
                ) noexcept
{
    using namespace std;
    string _l_rv{"[?]"};
    if constexpr (abc::utility::printer::default_printable<
                      typename std::remove_cvref<T>::type>)
    {
        _l_rv = abc::utility::printer::default_printer<
                    typename std::remove_cvref<T>::type>()
                    ->run_printer(_a_element);
    }
    else if constexpr (fmt::formattable<T>)
    {
        _l_rv = fmt::format("{}", _a_element);
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
    return mk_matcher_using_result_and_precedence(
        make_matcher_result<Cmp_Enum>(
            forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
        ),
        cmp_precedence<Cmp_Enum>()

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
        forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
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
        forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
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
        forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
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
        forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
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
        forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
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
        forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
    );
}

_END_ABC_NS