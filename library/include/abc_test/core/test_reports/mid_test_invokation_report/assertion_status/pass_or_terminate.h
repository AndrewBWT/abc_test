#pragma once
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/dynamic_status.h"

_BEGIN_ABC_REPORTS_NS
struct pass_or_terminate_t : public dynamic_status_t
{
public:
	__constexpr
		pass_or_terminate_t(
		) noexcept = delete;
	__constexpr
		pass_or_terminate_t(
			const bool _a_pass
		) noexcept;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
pass_or_terminate_t::pass_or_terminate_t(
	const bool _a_pass
) noexcept
	: dynamic_status_t(_a_pass)
{

}
_END_ABC_REPORTS_NS