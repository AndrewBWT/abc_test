#pragma once

#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/ds/test_data/invoked_test_data.h"

_BEGIN_ABC_REPORTERS_NS
	//Forward declarations
	struct ds::invoked_test_info_t;
	/*!
	* An generic object used to report information regarding tests.
	*
	*/
	struct test_reporter_t
	{
	public:
		__constexpr
			virtual
			void
			report_test(
				const reporters::after_execution_test_report_t& _a_aetr,
				const test_options_t& _a_test_options
			) = 0;
		/*!
		* Used to report individual tests
		*/
		/*__constexpr
			virtual
			void
			report_test(
				const ds::invoked_test_info_t& _a_iti,
				const errors::test_failures_info_t& _a_error_info,
				const test_options_t& _a_test_options
			) noexcept = 0;*/
		/*!
		* Used to provide information when all tests have been run regarding information about the
		* entire test run.
		*/
		__constexpr
			virtual
			void
			final_report(
			) noexcept;
	};
	using test_reporters_t = std::vector<utility::shared_and_raw_ptr<test_reporter_t>>;
	_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
	__constexpr_imp
		void
		test_reporter_t::final_report(
		) noexcept
	{

	}
	_END_ABC_REPORTERS_NS