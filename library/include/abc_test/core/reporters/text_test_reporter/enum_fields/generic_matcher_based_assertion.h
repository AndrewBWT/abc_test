#pragma once
#include "abc_test/core/reporters/text_test_reporter/enum_fields/generic_assertion.h"
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
_BEGIN_ABC_REPORTERS_NS

enum class enum_generic_matcher_based_assertion_fields_t
{
	MATCHER_SOURCE_MAP,
	MATCHER_OUTPUT
};
using combined_enum_generic_matcher_based_assertion_fields_t =
std::variant< enum_generic_matcher_based_assertion_fields_t, combined_enum_generic_assertion_fields_t>;
/*__constexpr
enum_print_pair_collection_t< combined_enum_assertion_fields_t>
default_assertion_fields(
) noexcept;*/
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
/*__constexpr_imp
enum_print_pair_collection_t< combined_enum_assertion_fields_t>
default_assertion_fields(
) noexcept
{
	using enum enum_assertion_fields_t;
	using enum enum_generic_assertion_fields_t;
	using enum enum_user_initialised_report_fields_t;
	using enum reporters::enum_print_data_t;
	return
	{
		{STR_REPRESENTATION,CONTEXTUAL_PRINT},
		{MESSAGE, CONTEXTUAL_PRINT},
		{SOURCE,CONTEXTUAL_PRINT},
		{LOG_INFOS, CONTEXTUAL_PRINT},
		{STATUS,DONT_PRINT},
		{MATCHER_SOURCE_MAP, CONTEXTUAL_PRINT},
		{MATCHER_OUTPUT,CONTEXTUAL_PRINT},
	};
}*/
_END_ABC_REPORTERS_NS