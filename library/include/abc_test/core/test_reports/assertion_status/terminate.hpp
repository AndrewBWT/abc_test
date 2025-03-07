#pragma once

#include "abc_test/core/test_reports/assertion_status/static_status.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class which represents a static assertion failure, which also
 * terminates the test.
 */
struct terminate_t : static_status_t
{};

_END_ABC_REPORTS_NS