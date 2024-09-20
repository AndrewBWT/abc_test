#pragma once

#include <stdexcept>
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_ERRORS_NS
/*!
* Dummy exception object used when an assertion fails and the entire function should terminate;
* this is thrown to be caught by the
*/
class test_assertion_exception_t
{
};
_END_ABC_ERRORS_NS
