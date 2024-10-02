#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/assertion_status_base.h"

_BEGIN_ABC_REPORTS_NS
struct dynamic_status_t : assertion_status_base_t
{
public:
	__constexpr
		bool
		pass(
		) const noexcept;
protected:
	__constexpr
		dynamic_status_t(
		) noexcept = delete;
	__constexpr
		dynamic_status_t(
			const bool _a_pass
		) noexcept;
	bool _m_pass;
};
_END_ABC_REPORTS_NS
_BEGIN_ABC_REPORTS_NS
__constexpr_imp
bool
dynamic_status_t::pass(
) const noexcept
{
	return _m_pass;
}
__constexpr_imp
dynamic_status_t::dynamic_status_t(
	const bool _a_pass
) noexcept
	: _m_pass(_a_pass)
{

}
_END_ABC_REPORTS_NS