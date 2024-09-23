#pragma once

#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_REPORTERS_NS
enum class enum_print_data_t
{
	PRINT,
	CONTEXTUAL_PRINT,
	DONT_PRINT
};

__constexpr
	bool
	always_print(
		const enum_print_data_t _a_enum_print_data
	) noexcept;
__constexpr
	bool
	constexual_print(
		const enum_print_data_t _a_enum_print_data
	) noexcept;
__constexpr
	bool	
	dont_print(
		const enum_print_data_t _a_enum_print_data
	) noexcept;

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS

__constexpr_imp
	bool
	always_print(
	const enum_print_data_t _a_enum_print_data
	) noexcept
{
	using enum enum_print_data_t;
	return _a_enum_print_data == PRINT;
}
__constexpr_imp
	bool
	constexual_print(
		const enum_print_data_t _a_enum_print_data
	) noexcept
{
	using enum enum_print_data_t;
	return _a_enum_print_data == CONTEXTUAL_PRINT;
}
__constexpr_imp
	bool
	dont_print(
		const enum_print_data_t _a_enum_print_data
	) noexcept
{
	using enum enum_print_data_t;
	return _a_enum_print_data == DONT_PRINT;
}
_END_ABC_REPORTERS_NS