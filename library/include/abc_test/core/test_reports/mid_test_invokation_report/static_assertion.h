#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/generic_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/static_status.h"
_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, static_status_t>
struct static_assertion_t
	: public generic_assertion_t<true, Assertion_Status>
{
public:
	__constexpr
		static_assertion_t(
			const user_initialised_report_t<true>::source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const std::optional<std::string_view>& _a_fail_msg,
			const std::optional<std::string_view>& _a_pass_msg
		) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, static_status_t>
__constexpr_imp
static_assertion_t<Assertion_Status>::static_assertion_t(
	const user_initialised_report_t<true>::source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const std::optional<std::string_view>& _a_fail_msg,
	const std::optional<std::string_view>& _a_pass_msg
) noexcept
	: generic_assertion_t<true, Assertion_Status>(Assertion_Status(), _a_source,
		_a_log_infos, _a_fail_msg, _a_pass_msg)
{

}
_END_ABC_REPORTS_NS
