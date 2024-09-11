#pragma once
#include "abc_test/core/errors/test_library_setup_error.h"

_BEGIN_ABC_REPORTERS_NS
	/*!
	* The generic object which allows errors to be dealt with when the test suite is being ran.
	*/
	struct error_reporter_t
	{
		/*!
		* Describes how to deal with an error_t being reported.
		*/
		__constexpr
			virtual
			bool
			report_error(
				const errors::setup_error_t& _a_error
			) noexcept = 0;
		__constexpr
			virtual
			void
			report_information(
				const std::string_view _a_str
			) noexcept = 0;
		/*!
		* What to do if the test suite is terminated - e.g. any cleaning up
		*/
		__constexpr
			virtual
			void
			exit(
			) noexcept = 0;
	};
_END_ABC_NS
_END_NS