#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/static_status.h"

_BEGIN_ABC_REPORTS_NS
struct terminate_t : static_status_t
{
public:
	__constexpr
		terminate_t(
		) noexcept;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
terminate_t::terminate_t(
) noexcept
{

}
_END_ABC_REPORTS_NS