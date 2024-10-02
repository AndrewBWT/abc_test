#pragma once
#include "abc_test/core/reporters/text_test_reporter/enum_fields/unexpected_report.h"
#include <variant>
_BEGIN_ABC_REPORTERS_NS
enum class enum_unexpected_thrown_non_descript_entity_fields_t
{
};
using combined_enum_unexpected_thrown_non_descript_entity_fields_t =
std::variant< enum_unexpected_thrown_non_descript_entity_fields_t, enum_unexpected_report_fields_t>;
__constexpr
enum_print_pair_collection_t< combined_enum_unexpected_thrown_non_descript_entity_fields_t>
default_unexpected_thrown_non_descript_entity_fields(
) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
enum_print_pair_collection_t< combined_enum_unexpected_thrown_non_descript_entity_fields_t>
default_unexpected_thrown_non_descript_entity_fields(
) noexcept
{
	using enum enum_unexpected_report_fields_t;
	using enum enum_print_data_t;
	using enum enum_unexpected_thrown_non_descript_entity_fields_t;
	return
	{
		{STR_REPRESENTATION,PRINT},
		{SOURCE,PRINT},
	};
}
_END_ABC_REPORTERS_NS