#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/matchers/operator_based_matcher.h"

_BEGIN_ABC_NS
enum logic_enum_t
{
	NOT,
	OR,
	AND
};
template<
	logic_enum_t Logic_Enum
>
__constexpr
	precedence_t
	logic_precedence(
) noexcept;

template<
	logic_enum_t Logic_Enum
>
__constexpr
	const char*
	logic_str(
	) noexcept;
template<
	logic_enum_t Logic_Enum
>
__constexpr
	bool
	compute_logic_result(
		const bool _a_left_result,
		const bool _a_right_result
	) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	precedence_t
	logic_precedence(
	) noexcept
{
	using enum logic_enum_t;
	if constexpr (Logic_Enum == NOT)
	{
		return 3;
	}
	else if constexpr (Logic_Enum == OR)
	{
		return 15;
	}
	else if constexpr (Logic_Enum == AND)
	{
		return 14;
	}
	else
	{
		static_assert(false, "precedence function does not have value for given template type.");
		return 0;
	}
}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	const char*
	logic_str(
	) noexcept
{
	using enum logic_enum_t;
	if constexpr (Logic_Enum == NOT)
	{
		return "!";
	}
	else if constexpr (Logic_Enum == OR)
	{
		return "||";
	}
	else if constexpr (Logic_Enum == AND)
	{
		return "&&";
	}
	else
	{
		static_assert(false, "str function does not have value for given template type.");
		return "";
	}
}
template<
	logic_enum_t Logic_Enum
>
__constexpr_imp
	bool
	compute_logic_result(
		const bool _a_left_result,
		const bool _a_right_result
	) noexcept
{
	using enum logic_enum_t;
	if constexpr (Logic_Enum == NOT)
	{
		return false;
	}
	else if constexpr (Logic_Enum == OR)
	{
		return (_a_left_result || _a_right_result);
	}
	else if constexpr (Logic_Enum == AND)
	{
		return (_a_left_result && _a_right_result);
	}
	else
	{
		static_assert(false, "compute_logic_result function does not have value for given "
			"template type.");
		return "";
	}
}
_END_ABC_NS