#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp"
#include <variant>
_BEGIN_ABC_REPORTERS_NS
enum class enum_matcher_based_assertion_block_fields_t
{
	SHOW_ALL_ASSERTIONS
};
enum class enum_matcher_assertion_block_assertion_fields_t
{
	MATCHER_ANNOTATION,
	MATCHER_RESULT_STRING,
	MATCHER_SOURCE_MAP
};
using combined_enum_matcher_based_assertion_block_fields_t =
std::variant< enum_matcher_based_assertion_block_fields_t, combined_enum_assertion_fields_t>;
__constexpr
enum_print_pair_collection_t< combined_enum_matcher_based_assertion_block_fields_t>
default_matcher_based_assertion_block_fields(
) noexcept;
__constexpr
enum_print_pair_collection_t< enum_matcher_assertion_block_assertion_fields_t>
default_matcher_based_assertion_block_assertion_fields(
) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
enum_print_pair_collection_t< combined_enum_matcher_based_assertion_block_fields_t>
default_matcher_based_assertion_block_fields(
) noexcept
{
	using enum enum_assertion_fields_t;
	using enum enum_user_initialised_report_fields_t;
	using enum reporters::enum_print_data_t;
	using enum enum_matcher_based_assertion_block_fields_t;
	return
	{
		{STR_REPRESENTATION,CONTEXTUAL_PRINT},
		{TEST_DESCRIPTION,CONTEXTUAL_PRINT},
		{SOURCE,CONTEXTUAL_PRINT},
		{LOG_INFOS, CONTEXTUAL_PRINT},
		{STATUS,DONT_PRINT},
		{SHOW_ALL_ASSERTIONS, CONTEXTUAL_PRINT},
	};
}
__constexpr_imp
enum_print_pair_collection_t< enum_matcher_assertion_block_assertion_fields_t>
default_matcher_based_assertion_block_assertion_fields(
) noexcept
{
	using enum enum_matcher_assertion_block_assertion_fields_t;
	using enum reporters::enum_print_data_t;
	return
	{
		{MATCHER_ANNOTATION,CONTEXTUAL_PRINT},
		{MATCHER_RESULT_STRING,CONTEXTUAL_PRINT},
		{MATCHER_SOURCE_MAP,CONTEXTUAL_PRINT},
	};
}
_END_ABC_REPORTERS_NS