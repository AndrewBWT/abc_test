#pragma once

#include "abc_test/matchers/comparison/comparison_matcher.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/wrapper.h"

_BEGIN_ABC_NS
/*!
* Named constructor for making an equality matcher.
*/
template<
	typename T1,
	typename T2
>
__constexpr
	matcher_t
	eq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept;
/*!
* Named constructor for making an inequality matcher.
*/
template<
	typename T1,
	typename T2
>
__constexpr
	matcher_t
	neq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept;
/*!
* Named constructor for making a less-than matcher.
*/
template<
	typename T1,
	typename T2
>
__constexpr
	matcher_t
	lt(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept;
/*!
* Named constructor for making a less-than-or-equal-to matcher.
*/
template<
	typename T1,
	typename T2
>
__constexpr
	matcher_t
	leq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept;
/*!
* Named constructor for making a greater-than matcher.
*/
template<
	typename T1,
	typename T2
>
__constexpr
	matcher_t
	gt(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept;
/*!
* Named constructor for making a greater-than-or-equal-to matcher.
*/
template<
	typename T1,
	typename T2
>
__constexpr
	matcher_t
	geq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept;
namespace
{
	template<
		typename T1,
		typename T2,
		comparison_enum_t Cmp_Enum
	>
	__constexpr
		matcher_t
		make_matcher(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
		) noexcept;
}
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename T1,
	typename T2
>
__constexpr_imp
	matcher_t
	eq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept
{
	using namespace std;
	using enum comparison_enum_t;
	return make_matcher<T1, T2, EQ>(forward<T1>(_a_left_arg), forward<T2>(_a_right_arg));
}
template<
	typename T1,
	typename T2
>
__constexpr_imp
	matcher_t
	neq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept
{
	using namespace std;
	using enum comparison_enum_t;
	return make_matcher<T1, T2, NEQ>(forward<T1>(_a_left_arg), forward<T2>(_a_right_arg));
}
template<
	typename T1,
	typename T2
>
__constexpr_imp
	matcher_t
	lt(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept
{
	using namespace std;
	using enum comparison_enum_t;
	return make_matcher<T1, T2, LT>(forward<T1>(_a_left_arg), forward<T2>(_a_right_arg));
}
template<
	typename T1,
	typename T2
>
__constexpr_imp
	matcher_t
	leq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept
{
	using namespace std;
	using enum comparison_enum_t;
	return make_matcher<T1, T2, LEQ>(forward<T1>(_a_left_arg), forward<T2>(_a_right_arg));
}
template<
	typename T1,
	typename T2
>
__constexpr_imp
	matcher_t
	gt(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept
{
	using namespace std;
	using enum comparison_enum_t;
	return make_matcher<T1, T2, GT>(forward<T1>(_a_left_arg), forward<T2>(_a_right_arg));
}
template<
	typename T1,
	typename T2
>
__constexpr_imp
	matcher_t
	geq(
		T1&& _a_left_arg,
		T2&& _a_right_arg
	) noexcept
{
	using namespace std;
	using enum comparison_enum_t;
	return make_matcher<T1, T2, GEQ>(forward<T1>(_a_left_arg), forward<T2>(_a_right_arg));
}
namespace
{
	template<
		typename T1,
		typename T2,
		comparison_enum_t Cmp_Enum
	>
	__constexpr_imp
		matcher_t
		make_matcher(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
		) noexcept
	{
		using namespace std;
		return matcher_t(matcher_internal_ptr_t(new comparison_matcher_t<T1, T2, Cmp_Enum>(forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg))),
			std::optional<std::source_location>());
	}
}
_END_ABC_NS