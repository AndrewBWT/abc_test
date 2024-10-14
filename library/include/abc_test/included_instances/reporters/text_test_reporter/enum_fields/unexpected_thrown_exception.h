#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_thrown_non_descript_entity.h"
_BEGIN_ABC_REPORTERS_NS
enum class enum_unexpected_exception_fields_t
{
	EXCEPTION_TYPE,
	EXCEPTION_MESSAGE
};
using combined_enum_unexpected_exception_fields_t =
std::variant< combined_enum_unexpected_thrown_non_descript_entity_fields_t, enum_unexpected_exception_fields_t>;
__constexpr
enum_print_pair_collection_t< combined_enum_unexpected_exception_fields_t>
default_unexpected_exception_fields(
) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
enum_print_pair_collection_t< combined_enum_unexpected_exception_fields_t>
default_unexpected_exception_fields(
) noexcept
{
	using enum enum_unexpected_report_fields_t;
	using enum enum_print_data_t;
	using enum enum_unexpected_exception_fields_t;
	return
	{
		{STR_REPRESENTATION,PRINT},
		{SOURCE,PRINT},
		{EXCEPTION_TYPE,PRINT},
		{EXCEPTION_MESSAGE,PRINT}
	};
}
_END_ABC_REPORTERS_NS