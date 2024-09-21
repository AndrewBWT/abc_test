#pragma once

#include "abc_test/core/reporters/test_reporter.h"
#include "abc_test/core/reporters/after_execution_test_report.h"

_BEGIN_ABC_REPORTERS_NS
	/*!
	* Object which controls a set of test_reporters_t
	*
	* This object is designed to work with multiple threads trying to report
	* testing information
	*/
	struct test_reporter_controller_t
	{
	public:
		/*!
		* Basic constructor
		*/
		__no_constexpr
			test_reporter_controller_t(
				const test_options_t& _a_test_options
			) noexcept;
		/*!
		* Function to call finalise_reports on all members
		*/
		__no_constexpr
			void
			finalise_reports(
			) noexcept;
		/*!
		* Function which reports a single test to each reporter
		*/
		__constexpr
			void
			report_test(
				const reporters::after_execution_test_report_t& _a_aetr
			) noexcept;
		/*__no_constexpr
			void
			report_test(
				const ds::invoked_test_info_t& _a_iti,
				const reporters::test_failures_info_t& _a_error_info
			) noexcept;*/
		/*!
		* Adds a set of reporters to this object
		*/
		__constexpr
			void
			add_reporters(
				const test_reporters_t& _a_new_test_reporters
			) noexcept;
	private:
		test_reporters_t _m_reporters;
		std::mutex _m_reporters_mutex;
		const test_options_t& _m_test_options;
	};
_END_ABC_REPORTERS_NS


_BEGIN_ABC_REPORTERS_NS
	__no_constexpr_imp
		test_reporter_controller_t::test_reporter_controller_t(
			const test_options_t& _a_test_options
		) noexcept
		: _m_test_options(_a_test_options)
	{

	}
	__no_constexpr_imp
		void
		test_reporter_controller_t::finalise_reports(
		) noexcept
	{
		for (utility::shared_and_raw_ptr<test_reporter_t>& _l_reporter : _m_reporters)
		{
			test_reporter_t* _l_reporter_ptr{ utility::get_ptr(_l_reporter) };
			_l_reporter_ptr->final_report();
		}
	}
	__constexpr_imp
		void
		test_reporter_controller_t::report_test(
			const reporters::after_execution_test_report_t& _a_aetr
		) noexcept
	{
		std::unique_lock _l_report_test_unique_lokc(_m_reporters_mutex);
		for (utility::shared_and_raw_ptr<test_reporter_t>& _l_reporter : _m_reporters)
		{
			test_reporter_t* _l_reporter_ptr{ utility::get_ptr(_l_reporter) };
			_l_reporter_ptr->report_test(_a_aetr,_m_test_options);
		}
	}
	/*__no_constexpr_imp
		void
		test_reporter_controller_t::report_test(
			const ds::invoked_test_info_t& _a_iti,
			const errors::test_failures_info_t& _a_error_info
		) noexcept
	{
		std::unique_lock _l_report_test_unique_lokc(_m_reporters_mutex);
		for (utility::shared_and_raw_ptr<test_reporter_t>& _l_reporter : _m_reporters)
		{
			test_reporter_t* _l_reporter_ptr{ utility::get_ptr(_l_reporter) };
			_l_reporter_ptr->report_test(_a_iti, _a_error_info, _m_test_options);
		}
	}*/
	__constexpr_imp
		void
		test_reporter_controller_t::add_reporters(
			const test_reporters_t& _a_new_test_reporters
		) noexcept
	{
		_m_reporters.insert(_m_reporters.end(), _a_new_test_reporters.begin(), _a_new_test_reporters.end());
	}
	_END_ABC_REPORTERS_NS