#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/generic_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/dynamic_status.h"

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
requires std::derived_from<Assertion_Status, dynamic_status_t>
struct manual_assertion_t
	: public generic_assertion_t<true, Assertion_Status>
{
public:
	__constexpr
		manual_assertion_t(
			const bool _a_pass,
			const user_initialised_report_t<true>::source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const std::optional<std::string_view>& _a_msg
		) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
manual_assertion_t<Assertion_Status>::manual_assertion_t(
	const bool _a_pass,
	const user_initialised_report_t<true>::source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const std::optional<std::string_view>& _a_msg
) noexcept
	: generic_assertion_t<true,Assertion_Status>(Assertion_Status(_a_pass),_a_source,
		_a_log_infos,_a_msg)
{

}
_END_ABC_REPORTS_NS
