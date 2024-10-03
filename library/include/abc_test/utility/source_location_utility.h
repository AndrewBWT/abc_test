#pragma once
#include <source_location>
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_UTILITY_NS
__constexpr
bool
equal_file_name_function_name_and_line(
	const std::source_location& _a_lhs,
	const std::source_location& _a_rhs
) noexcept;
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
__constexpr_imp
bool
equal_file_name_function_name_and_line(
	const std::source_location& _a_lhs,
	const std::source_location& _a_rhs
) noexcept
{
	if (std::strcmp(_a_lhs.file_name(),_a_rhs.file_name())) {
		return false;
	}
	if (std::strcmp(_a_lhs.function_name(), _a_rhs.function_name())) {
		return false;
	}
	if (_a_lhs.line() != _a_rhs.line()) {
		return false;
	}
	return true;
}
_END_ABC_UTILITY_NS