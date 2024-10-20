#pragma once

#include "abc_test/internal/test_reports/assertion_status/static_status.h"

_BEGIN_ABC_REPORTS_NS
/*!
 * @brief Concrete class which represents a static assertion failure.
 */
struct fail_t : static_status_t
{
};
_END_ABC_REPORTS_NS
