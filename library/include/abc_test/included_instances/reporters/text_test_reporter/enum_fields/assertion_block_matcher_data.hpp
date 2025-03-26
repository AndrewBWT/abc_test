#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp"
#include <variant>
_BEGIN_ABC_REPORTERS_NS
enum class enum_assertion_block_matcher_data_fields_t
{
	MATCHER_ANNOTATION,
	MATCHER_RESULT_STRING,
	MATCHER_SOURCE_MAP,
	MATCHER_MAIN_SOURCE_REP,
	MATCHER_MAIN_SOURCE_LOCATION,
	LOG_INFOS
};
_END_ABC_REPORTERS_NS