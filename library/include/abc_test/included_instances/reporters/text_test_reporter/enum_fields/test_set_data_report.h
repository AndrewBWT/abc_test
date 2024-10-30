#pragma once
#include "abc_test/internal/utility/internal/macros.h"
_BEGIN_ABC_REPORTERS_NS
enum class enum_test_set_data_fields_t
{
	TOTAL_TESTS_RAN
};
__constexpr
enum_print_pair_collection_t< enum_test_set_data_fields_t>
default_test_set_data_fields(
) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
enum_print_pair_collection_t< enum_test_set_data_fields_t>
default_test_set_data_fields(
) noexcept
{
	using enum enum_test_set_data_fields_t;
	using enum reporters::enum_print_data_t;
	return
	{
		{TOTAL_TESTS_RAN, PRINT}
	};
}
_END_ABC_REPORTERS_NS