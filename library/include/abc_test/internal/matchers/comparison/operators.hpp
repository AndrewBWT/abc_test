#pragma once
#include "abc_test/internal/matchers/comparison/comparison_functions.hpp"
#include "abc_test/internal/matchers/comparison/comparison_placeholder.hpp"
#include "abc_test/internal/matchers/comparison/comparison_wrapper.hpp"


_BEGIN_ABC_MATCHER_NS
/*!
 * @brief Operator used to create comparison-based matchers which model the
 * checking of equality between two elements.
 * @tparam T1 The paramter contained in the comparison_wrapper_t.
 * @tparam T2 The parameter of the second argument.
 * @param _a_left_arg A comparison_wrapper_t entity containing a T1 element.
 * @param _a_right_arg An element of type T2.
 * @return matcher_t modelling equality between the entity in _a_left_arg and
 * _a_right_arg.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    operator==(comparison_wrapper_t<T1>&& _a_left_arg, T2&& _a_right_arg)
        noexcept;
/*!
 * @brief Operator used to create comparison-based matchers which model the
 * checking of inequality between two elements.
 * @tparam T1 The paramter contained in the comparison_wrapper_t.
 * @tparam T2 The parameter of the second argument.
 * @param _a_left_arg A comparison_wrapper_t entity containing a T1 element.
 * @param _a_right_arg An element of type T2.
 * @return matcher_t modelling inequality between the entity in _a_left_arg and
 * _a_right_arg.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    operator!=(comparison_wrapper_t<T1>&& _a_left_arg, T2&& _a_right_arg)
        noexcept;
/*!
 * @brief Operator used to create comparison-based matchers which model the
 * checking of less-than between two elements.
 * @tparam T1 The paramter contained in the comparison_wrapper_t.
 * @tparam T2 The parameter of the second argument.
 * @param _a_left_arg A comparison_wrapper_t entity containing a T1 element.
 * @param _a_right_arg An element of type T2.
 * @return matcher_t modelling less-than between the entity in _a_left_arg and
 * _a_right_arg.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    operator<(comparison_wrapper_t<T1>&& _a_left_arg, T2&& _a_right_arg)
        noexcept;
/*!
 * @brief Operator used to create comparison-based matchers which model the
 * checking of less-than-or-equal-to between two elements.
 * @tparam T1 The paramter contained in the comparison_wrapper_t.
 * @tparam T2 The parameter of the second argument.
 * @param _a_left_arg A comparison_wrapper_t entity containing a T1 element.
 * @param _a_right_arg An element of type T2.
 * @return matcher_t modelling less-than-or-equal-to between the entity in
 * _a_left_arg and _a_right_arg.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    operator<=(comparison_wrapper_t<T1>&& _a_left_arg, T2&& _a_right_arg)
        noexcept;
/*!
 * @brief Operator used to create comparison-based matchers which model the
 * checking of greater-than between two elements.
 * @tparam T1 The paramter contained in the comparison_wrapper_t.
 * @tparam T2 The parameter of the second argument.
 * @param _a_left_arg A comparison_wrapper_t entity containing a T1 element.
 * @param _a_right_arg An element of type T2.
 * @return matcher_t modelling greater-than between the entity in
 * _a_left_arg and _a_right_arg.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    operator>(comparison_wrapper_t<T1>&& _a_left_arg, T2&& _a_right_arg)
        noexcept;
/*!
 * @brief Operator used to create comparison-based matchers which model the
 * checking of greater-than-or-equal-to between two elements.
 * @tparam T1 The paramter contained in the comparison_wrapper_t.
 * @tparam T2 The parameter of the second argument.
 * @param _a_left_arg A comparison_wrapper_t entity containing a T1 element.
 * @param _a_right_arg An element of type T2.
 * @return matcher_t modelling greater-than-or-equal-to between the entity in
 * _a_left_arg and _a_right_arg.
 */
template <typename T1, typename T2>
__constexpr matcher_t
    operator>=(comparison_wrapper_t<T1>&& _a_left_arg, T2&& _a_right_arg)
        noexcept;
/*!
 * Operator overload for less-than comparison of a dummy placeholder type and a
 * type of T.
 *
 * This comparison operator creates a wrapper type. It is used with severl
 * macros to provide an apparent native comparison operator within testing code.
 */

/*!
 * @brief Operator used to create entities of type comparison_wrapper_t.
 *
 * This function is designed to be used with the EXPR macro. By prepending any
 * equality statement with this operator, the equality statement can be turned
 * into a matcher-based statement.
 *
 * @tparam T The parameter of the argument.
 * @param The unanemd paramter; comparision_placeholder_t's is a dummy class.
 * @param _a_wrapper
 * @return
 */
template <typename T>
__constexpr comparison_wrapper_t<T>
            operator<(const comparison_placeholder_t&, T&& _a_wrapper) noexcept;
_END_ABC_MATCHER_NS

_BEGIN_ABC_MATCHER_NS
template <typename T1, typename T2>
__constexpr_imp matcher_t
    operator==(
        comparison_wrapper_t<T1>&& _a_left_arg,
        T2&&                       _a_right_arg
    ) noexcept
{
    using namespace std;
    return eq<T1, T2>(
        forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    operator!=(
        comparison_wrapper_t<T1>&& _a_left_arg,
        T2&&                       _a_right_arg
    ) noexcept
{
    using namespace std;
    return neq<T1, T2>(
        forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    operator<(
        comparison_wrapper_t<T1>&& _a_left_arg,
        T2&&                       _a_right_arg
    ) noexcept
{
    using namespace std;
    return lt<T1, T2>(
        forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    operator<=(
        comparison_wrapper_t<T1>&& _a_left_arg,
        T2&&                       _a_right_arg
    ) noexcept
{
    using namespace std;
    return leq<T1, T2>(
        forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    operator>(
        comparison_wrapper_t<T1>&& _a_left_arg,
        T2&&                       _a_right_arg
    ) noexcept
{
    using namespace std;
    return gt<T1, T2>(
        forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg)
    );
}

template <typename T1, typename T2>
__constexpr_imp matcher_t
    operator>=(
        comparison_wrapper_t<T1>&& _a_left_arg,
        T2&&                       _a_right_arg
    ) noexcept
{
    using namespace std;
    return geq<T1, T2>(
        forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg)
    );
}

template <typename T>
__constexpr_imp comparison_wrapper_t<T>
                operator<(
        const comparison_placeholder_t& _a_pc,
        T&&                             _a_t
    ) noexcept
{
    using namespace std;
    return comparison_wrapper_t<T>(forward<T>(_a_t));
}

_END_ABC_MATCHER_NS