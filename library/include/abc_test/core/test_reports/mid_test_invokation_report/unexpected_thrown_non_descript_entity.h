#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/unexpected_report.h"

_BEGIN_ABC_REPORTS_NS

class unexpected_thrown_non_descript_entity_t :
	public unexpected_report_t<true>
{
public:
	__constexpr
		unexpected_thrown_non_descript_entity_t(
			const single_source_t& _a_source
		) noexcept;
};

_END_ABC_REPORTS_NS
_BEGIN_ABC_REPORTS_NS
__constexpr_imp
unexpected_thrown_non_descript_entity_t::unexpected_thrown_non_descript_entity_t(
	const single_source_t& _a_source
) noexcept
	: unexpected_report_t<true>(_a_source, false)
{

}
_END_ABC_REPORTS_NS