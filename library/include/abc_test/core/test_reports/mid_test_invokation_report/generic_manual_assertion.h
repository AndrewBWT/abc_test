#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/generic_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/dynamic_status.h"

_BEGIN_ABC_REPORTS_NS
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
struct generic_manual_assertion_t
	: public generic_assertion_t<Single_Source, Assertion_Status>
{
public:
	__constexpr
		generic_manual_assertion_t(
			const bool _a_pass,
			const user_initialised_report_t<Single_Source>::source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const std::optional<std::string_view>& _a_fail_msg,
			const std::optional<std::string_view>& _a_pass_msg
		) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
generic_manual_assertion_t<Single_Source,Assertion_Status>::generic_manual_assertion_t(
	const bool _a_pass,
	const user_initialised_report_t<Single_Source>::source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const std::optional<std::string_view>& _a_fail_msg,
	const std::optional<std::string_view>& _a_pass_msg
) noexcept
	: generic_assertion_t<Single_Source, Assertion_Status>(Assertion_Status(_a_pass), _a_source,
		_a_log_infos, _a_fail_msg,_a_pass_msg)
{

}
_END_ABC_REPORTS_NS
