#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/assertion_status_base.h"

_BEGIN_ABC_REPORTS_NS
struct static_status_t : assertion_status_base_t
{
protected:
	__constexpr
		static_status_t(
		) noexcept;
};
_END_ABC_REPORTS_NS
_BEGIN_ABC_REPORTS_NS
__constexpr_imp
static_status_t::static_status_t(
) noexcept
{

}
_END_ABC_REPORTS_NS