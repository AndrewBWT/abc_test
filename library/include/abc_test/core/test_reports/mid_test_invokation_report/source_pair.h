#pragma once
#include "abc_test/core/test_reports/mid_test_invokation_report/single_source.h"

_BEGIN_ABC_REPORTS_NS

struct source_pair_t
{
public:
	__constexpr_imp
		source_pair_t(
			const single_source_t& _a_begin_source,
			const single_source_t& _a_end_source
		) noexcept
	{

	}
};
_END_ABC_REPORTS_NS