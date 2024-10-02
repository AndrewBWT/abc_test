#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/source_pair.h"

_BEGIN_ABC_REPORTS_NS
class block_assertion_container_t
{
public:
	__constexpr
		void
		add_source(
			const reports::single_source_t& _a_single_source
		) noexcept;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
void
block_assertion_container_t::add_source(
	const reports::single_source_t& _a_single_source
) noexcept
{

}
_END_ABC_REPORTS_NS