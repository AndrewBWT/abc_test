#pragma once
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_ERRORS_NS
	/*!
	* Enum used to differentiate between different types of error encountered in testing code.
	*/
	enum class test_failure_info_enum_t
	{
		TEST_FAILURE,
		MANUAL_FAILURE,
		UNHANDLED_EXCEPTION,
		UNHADLED_EXCEPTION_NOT_DERIVED_FROM_EXCEPTION
	};
_END_ABC_ERRORS_NS