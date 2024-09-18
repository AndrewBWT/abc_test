#pragma once

#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_NS
enum class comparison_enum_t
{
	EQ,
	NEQ,
	LEQ,
	LT,
	GEQ,
	GT
};
/*!
* Finds the precedence of the templated comparision_enum_t object.
*/
template<
	comparison_enum_t Comparison_Enum
>
__constexpr
	precedence_t
	cmp_precedence(
	) noexcept;
/*!
* Given two arguments of type T1 and T2, based on the given enum template,
* calls the appropriate comparison function.
*/
template<
	typename T1,
	typename T2,
	comparison_enum_t Comparison_Enum
>
__constexpr
	bool
	cmp(
		T1&& _a_l,
		T2&& _a_r
	) noexcept;
/*!
* Given the templated comparison_enum_t type, returns a string representation of that templated type.
*/
template<
	comparison_enum_t Comparison_Enum
>
__constexpr
	const char*
	cmp_str(
	) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
template<
	comparison_enum_t Comparison_Enum
>
__constexpr_imp
	precedence_t
	cmp_precedence(
	) noexcept
{
	using enum comparison_enum_t;
	if constexpr (
		Comparison_Enum == EQ ||
		Comparison_Enum == NEQ
		)
	{
		return 10;
	}
	else if constexpr (
		Comparison_Enum == LEQ ||
		Comparison_Enum == LT ||
		Comparison_Enum == GEQ ||
		Comparison_Enum == GT
		)
	{
		return 9;
	}
	else
	{
		static_assert(false, "precedence function does not have value for given template type.");
		return 0;
	}
}
template<
	typename T1,
	typename T2,
	comparison_enum_t Comparison_Enum
>
__constexpr_imp
	bool
	cmp(
		T1&& _a_l,
		T2&& _a_r
	) noexcept
{
	using enum comparison_enum_t;
	if constexpr (Comparison_Enum == EQ)
	{
		return _a_l == _a_r;
	}
	else if constexpr (Comparison_Enum == NEQ)
	{
		return _a_l != _a_r;
	}
	else if constexpr (Comparison_Enum == LT)
	{
		return _a_l < _a_r;
	}
	else if constexpr (Comparison_Enum == LEQ)
	{
		return _a_l <= _a_r;
	}
	else if constexpr (Comparison_Enum == GT)
	{
		return _a_l > _a_r;
	}
	else if constexpr (Comparison_Enum == GEQ)
	{
		return _a_l >= _a_r;
	}
	else
	{
		static_assert(false, "cmp function does not have value for given template type.");
		return true;
	}
}
template<
	comparison_enum_t Comparison_Enum
>
__constexpr_imp
	const char*
	cmp_str(
	) noexcept
{
	using enum comparison_enum_t;
	if constexpr (Comparison_Enum == EQ)
	{
		return "==";
	}
	else if constexpr (Comparison_Enum == NEQ)
	{
		return "!=";
	}
	else if constexpr (Comparison_Enum == LT)
	{
		return "<";
	}
	else if constexpr (Comparison_Enum == LEQ)
	{
		return "<=";
	}
	else if constexpr (Comparison_Enum == GT)
	{
		return ">";
	}
	else if constexpr (Comparison_Enum == GEQ)
	{
		return ">=";
	}
	else
	{
		static_assert(false, "str function does not have value for given template type.");
		return "";
	}
}
_END_ABC_NS