#pragma once
#include "abc_test/utility/internal/macros.h"
#include <variant>
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/user_initialised_report.h"
_BEGIN_ABC_REPORTERS_NS
enum class enum_generic_assertion_fields_t
{
	STATUS,
	TEST_DESCRIPTION,
	STR_REPRESENTATION
};
using combined_enum_generic_assertion_fields_t =
std::variant< enum_generic_assertion_fields_t, enum_user_initialised_report_fields_t>;

_END_ABC_REPORTERS_NS