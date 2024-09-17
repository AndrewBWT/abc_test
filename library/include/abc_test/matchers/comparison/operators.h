#pragma once
#include "abc_test/matchers/comparison/constructors.h"
#include "abc_test/matchers/comparison/placeholder.h"

_BEGIN_ABC_NS
	/*!
	* Operator overload for equality comparison of a wrapper of type T1 and an element of T2.
	*/
	template<
		typename T1,
		typename T2
	>
	__constexpr
		matcher_t
		operator==(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept;
	/*!
	* Operator overload for inequality comparison of a wrapper of type T1 and an element of T2.
	*/
	template<
		typename T1,
		typename T2
	>
	__constexpr
		matcher_t
		operator!=(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept;
	/*!
	* Operator overload for less-than comparison of a wrapper of type T1 and an element of T2.
	*/
	template<
		typename T1,
		typename T2
	>
	__constexpr
		matcher_t
		operator<(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept;
	/*!
	* Operator overload for less-than-or-equal-to comparison of a wrapper of type T1 and an element of T2.
	*/
	template<
		typename T1,
		typename T2
	>
	__constexpr
		matcher_t
		operator<=(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept;
	/*!
	* Operator overload for greater-than comparison of a wrapper of type T1 and an element of T2.
	*/
	template<
		typename T1,
		typename T2
	>
	__constexpr
		matcher_t
		operator>(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept;
	/*!
	* Operator overload for greater-than-or-equal-to comparison of a wrapper of type T1 and an element of T2.
	*/
	template<
		typename T1,
		typename T2
	>
	__constexpr
		matcher_t
		operator>=(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept;
	/*!
	* Operator overload for less-than comparison of a dummy placeholder type and a type of T.
	* 
	* This comparison operator creates a wrapper type. It is used with severl macros to provide an apparent
	* native comparison operator within testing code.
	*/
	template<
		typename T
	>
	__constexpr
		wrapper<T>
		operator<(
			const placeholder_t&,
			T&& _a_wrapper
			) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
	template<
		typename T1,
		typename T2
	>
	__constexpr_imp
		matcher_t
		operator==(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept
	{
		using namespace std;
		return eq<T1, T2>(forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg));
	}
	template<
		typename T1,
		typename T2
	>
	__constexpr_imp
		matcher_t
		operator!=(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept
	{
		using namespace std;
		return neq<T1, T2>(forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg));
	}
	template<
		typename T1,
		typename T2
	>
	__constexpr_imp
		matcher_t
		operator<(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept
	{
		using namespace std;
		return lt<T1, T2>(forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg));
	}
	template<
		typename T1,
		typename T2
	>
	__constexpr_imp
		matcher_t
		operator<=(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept
	{
		using namespace std;
		return leq<T1, T2>(forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg));
	}
	template<
		typename T1,
		typename T2
	>
	__constexpr_imp
		matcher_t
		operator>(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept
	{
		using namespace std;
		return gt<T1, T2>(forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg));
	}
	template<
		typename T1,
		typename T2
	>
	__constexpr_imp
		matcher_t
		operator>=(
			wrapper<T1>&& _a_left_arg,
			T2&& _a_right_arg
			) noexcept
	{
		using namespace std;
		return geq<T1, T2>(forward<T1>(_a_left_arg.object()), forward<T2>(_a_right_arg));
	}
	template<
		typename T
	>
	__constexpr_imp
		wrapper<T>
		operator<(
			const placeholder_t& _a_pc,
			T&& _a_t
			) noexcept
	{
		using namespace std;
		return wrapper<T>(forward<T>(_a_t));
	}
_END_ABC_NS