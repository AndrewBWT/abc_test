#pragma once
#include "abc_test/core/reporters/text_test_reporter/enum_fields/generic_assertion.h"
_BEGIN_ABC_REPORTERS_NS
enum class enum_generic_manual_assertion_fields_t
{

};
using combined_enum_generic_manual_assertion_fields_t =
std::variant< enum_generic_manual_assertion_fields_t, combined_enum_generic_assertion_fields_t>;

_END_ABC_REPORTERS_NS