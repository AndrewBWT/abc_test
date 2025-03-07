#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include <variant>
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/user_initialised_report.hpp"
_BEGIN_ABC_REPORTERS_NS
enum class enum_assertion_fields_t
{
	STATUS,
	TEST_DESCRIPTION,
	STR_REPRESENTATION
};
using combined_enum_assertion_fields_t =
std::variant< enum_assertion_fields_t, enum_user_initialised_report_fields_t>;

_END_ABC_REPORTERS_NS