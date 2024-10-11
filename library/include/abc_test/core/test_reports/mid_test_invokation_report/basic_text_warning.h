#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/unexpected_report.h"

_BEGIN_ABC_REPORTS_NS
class basic_text_warning_t
	: public unexpected_report_t<false>
{
	
public:
	__constexpr
		basic_text_warning_t(
			const ds::single_source_t& _a_source,
			const bool _a_exact_source,
			const std::string_view _a_str
		) noexcept;
private:
	std::string _m_str;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
basic_text_warning_t::basic_text_warning_t(
	const ds::single_source_t& _a_source,
	const bool _a_exact_source,
	const std::string_view _a_str
) noexcept
	: unexpected_report_t<false>(_a_source,_a_exact_source)
	, _m_str(_a_str)
{

}
_END_ABC_REPORTS_NS