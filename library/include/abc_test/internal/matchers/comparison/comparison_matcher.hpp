#pragma once

#include "abc_test/internal/matchers/comparison/comparison_enum.hpp"
#include "abc_test/internal/matchers/operator_based_matcher.hpp"
#include "abc_test/internal/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief Generic object used when creating comparison-based matchers.
 *
 * This class itself is not usually used; instead the macro _EXPR and the
 * factory methods in constructor.h are used.
 *
 * @tparam T1 The parameter of the first argument.
 * @tparam T2 The parameter of the second argument.
 * @tparam Cmp The enum used to compare T1 and T2.
 */
template <typename T1, typename T2, comparison_enum_t Cmp>
struct comparison_matcher_t : public operator_based_matcher_t
{
public:
    /*!
     * @brief Constructor.
     * @param _a_l The T1 entity.
     * @param _a_r The T2 entity.
     */
    __constexpr
    comparison_matcher_t(T1&& _a_l, T2&& _a_r) noexcept;
    /*!
     * @brief Returns the precedence_t of the tempalted comparison type.
     * @return precedence_t representing the tempalted comparison_enum's
     * precedence.
     */
    __constexpr precedence_t
        get_precedence() const noexcept;
private:
    T1 _m_l;
    T2 _m_r;
    __constexpr virtual matcher_result_t
        run(test_runner_t&) noexcept;
};
namespace
{
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
template <typename T1, typename T2, comparison_enum_t Cmp>
__constexpr_imp
    comparison_matcher_t<T1, T2, Cmp>::comparison_matcher_t(
        T1&& _a_l,
        T2&& _a_r
    ) noexcept
    : _m_l(_a_l), _m_r(_a_r)
{}

template <typename T1, typename T2, comparison_enum_t Cmp>
__constexpr_imp precedence_t
    comparison_matcher_t<T1, T2, Cmp>::get_precedence() const noexcept
{
    return cmp_precedence<Cmp>();
}

template<
	typename T1,
	typename T2,
	comparison_enum_t Cmp
>
__constexpr_imp
	matcher_result_t
	comparison_matcher_t<T1, T2, Cmp>::run(
		test_runner_t&
	) noexcept
{
    using namespace std;
    string _l_left_str{format_str<T1>(_m_l)};
    string _l_right_str{format_str<T1>(_m_r)};
    return matcher_result_t(
        true,
        cmp<T1, T2, Cmp>(forward<T1>(_m_l), forward<T2>(_m_r)),
        fmt::format("{0} {1} {2}", _l_left_str, cmp_str<Cmp>(), _l_right_str)
    );
}
namespace
{
    template <typename T>
    __constexpr_imp std::string
        format_str(
            const T& _a_element
        ) noexcept
    {
        using namespace std;
        string _l_rv{ "[?]" };
        if constexpr (fmt::formattable<T>)
        {
            if constexpr (same_as<T, string>)
            {
                _l_rv = fmt::format("\"{}\"", _a_element);
            }
            else
            {
                _l_rv = fmt::format("{}", _a_element);
            }
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
        return make_matcher(new comparison_matcher_t<T1, T2, Cmp_Enum>(
            forward<T1>(_a_left_arg), forward<T2>(_a_right_arg)
        ));
    }
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