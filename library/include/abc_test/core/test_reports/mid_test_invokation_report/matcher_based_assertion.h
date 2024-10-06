#pragma once
#include "abc_test/core/test_reports/mid_test_invokation_report/generic_matcher_based_assertion.h"

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
struct matcher_based_assertion_t
	: public generic_matcher_based_assertion_t<true, Assertion_Status>
{
public:
	__constexpr
		matcher_based_assertion_t(
			const user_initialised_report_t<true>::source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const matcher_result_t& _a_matcher_result,
			const matcher_source_map_t& _a_matcher_source_map,
			const std::optional<std::string_view>& _a_annotation
		) noexcept;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
matcher_based_assertion_t<Assertion_Status>::matcher_based_assertion_t(
	const user_initialised_report_t<true>::source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const matcher_result_t& _a_matcher_result,
	const matcher_source_map_t& _a_matcher_source_map,
	const std::optional<std::string_view>& _a_annotation
) noexcept
	: generic_matcher_based_assertion_t<true, Assertion_Status>(
		_a_source, _a_log_infos, std::optional<std::string>{},
		_a_matcher_result,_a_matcher_source_map,_a_annotation
	)
{

}
_END_ABC_REPORTS_NS