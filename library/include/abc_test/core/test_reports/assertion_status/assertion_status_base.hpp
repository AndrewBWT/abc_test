#pragma once

#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Class used as a base class; it only has a protected default
 * constructor so instances of it cannot be made.
 *
 * It is used to define all of the different types of assertion status.
 */
struct assertion_status_base_t
{
protected:
    __constexpr
    assertion_status_base_t()
        = default;
};

_END_ABC_REPORTS_NS