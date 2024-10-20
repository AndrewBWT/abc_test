#pragma once

#include "abc_test/internal/test_reports/assertion_status/assertion_status_base.h"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Abstract class which represents a static assertion status.
 *
 * We use a protected default constructor to ensure elemnets of this class
 * cannot be created.
 */
struct static_status_t : assertion_status_base_t
{
protected:
    __constexpr
    static_status_t()
        = default;
};

_END_ABC_REPORTS_NS
_BEGIN_ABC_REPORTS_NS
_END_ABC_REPORTS_NS