#pragma once
#include "abc_test/internal/setup_tests_log_reporter.h"
#include <iostream>

namespace ab::test
{
	struct pc_tests_log_reporter : public setup_tests_log_reporter_t
	{
	public:
		virtual
			constexpr
			bool
			report_error(
				const setup_test_error_t& _a_setup_test_error
			) noexcept
		{
			_m_setup_test_errors.push_back(_a_setup_test_error);
			return false;
		}
		constexpr
			std::vector<setup_test_error_t>&
			setup_test_errors(
			) noexcept
		{
			return _m_setup_test_errors;
		}
		inline
			void
			exit(
			) noexcept
		{
			std::cout << "Exiting..." << std::endl;
		}
	private:
		std::vector<setup_test_error_t> _m_setup_test_errors;
	};
}