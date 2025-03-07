#pragma once

#include "abc_test/utility/internal/macros.hpp"

#include <stdexcept>

_BEGIN_ABC_ERRORS_NS

/*!
 * @brief Dummy object used to signal to the system that an assetion has failed,
 * and the user requires the entire test to terminate early. This object is
 * thrown by the system, then caught and handled appropriately.
 * 
 * It contains no data.
 */
class test_assertion_exception_t
{};

_END_ABC_ERRORS_NS
