#pragma once
#include "abc_test/internal/utility/internal/macros.h"
_BEGIN_ABC_REPORTERS_NS
enum class enum_finalised_test_set_data_fields_t
{
	TOTAL_TESTS_RAN,
	TOTAL_TESTS_PASSED,
	TOTAL_TESTS_FAILED,
	TOTAL_TESTS_FAILED_AND_TERMINATED,
	TOTAL_TESTS_FAILED_BUT_NOT_TERMINATED,
	TOTAL_TESTS_DESIGNATED_FAILURE_DUE_TO_EXCEPTION,
	TOTAL_ASSERTIONS_RAN,
	TOTAL_ASSERTIONS_PASSED,
	TOTAL_ASSERTIONS_FAILED,
	RNG_SEED,
	TDG_COLLECTION_STACK_TRIES_MAP
};
__constexpr
enum_print_pair_collection_t< enum_finalised_test_set_data_fields_t>
default_finalised_test_set_data_fields(
) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
enum_print_pair_collection_t< enum_finalised_test_set_data_fields_t>
default_finalised_test_set_data_fields(
) noexcept
{
	using enum enum_finalised_test_set_data_fields_t;
	using enum reporters::enum_print_data_t;
	return
	{
		{TOTAL_TESTS_RAN, PRINT},
		{TOTAL_TESTS_PASSED, PRINT},
		{TOTAL_TESTS_FAILED, PRINT},
		{TOTAL_TESTS_FAILED_AND_TERMINATED, PRINT},
		{TOTAL_TESTS_FAILED_BUT_NOT_TERMINATED, PRINT},
		{TOTAL_TESTS_DESIGNATED_FAILURE_DUE_TO_EXCEPTION, PRINT},
		{TOTAL_ASSERTIONS_RAN, PRINT},
		{TOTAL_ASSERTIONS_PASSED, PRINT},
		{TOTAL_ASSERTIONS_FAILED, PRINT},
		{RNG_SEED, PRINT},
		{TDG_COLLECTION_STACK_TRIES_MAP, PRINT},
	};
}
_END_ABC_REPORTERS_NS