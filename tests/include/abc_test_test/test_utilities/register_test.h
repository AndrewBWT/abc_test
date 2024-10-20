#pragma once
#include "abc_test_test/test_utilities/test_info.h"
#include "abc_test/internal/register_test.h"

namespace ab::test
{
	void
		check_register_test(
			const registered_test_data_t& _a_register_test,
			const bool _a_check_null,
			const test_info_t& _a_test_info
		)
	{
		if (_a_check_null)
		{
			CHECK(_a_register_test._m_function == nullptr);
		}
		CHECK(_a_register_test._m_test_info == _a_test_info);
	}
}