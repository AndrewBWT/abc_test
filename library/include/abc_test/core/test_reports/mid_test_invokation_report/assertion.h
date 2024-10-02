#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/generic_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/dynamic_status.h"
#include "abc_test/matchers/matcher_result.h"
#include "abc_test/matchers/source_map.h"

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
struct assertion_t
	: public generic_assertion_t<true, Assertion_Status>
{
public:
	__constexpr
		assertion_t(
			const user_initialised_report_t<true>::source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const std::optional<std::string_view>& _a_msg,
			const matcher_result_t& _a_matcher_result,
			const matcher_source_map_t& _a_matcher_source_map
		) noexcept;
	__constexpr
		const matcher_result_t&
		matcher_result(
		) const noexcept;
	__constexpr
		const matcher_source_map_t&
		source_map(
		) const noexcept;
protected:
	matcher_result_t _m_matcher_result;
	matcher_source_map_t _m_matcher_map;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
assertion_t<Assertion_Status>::assertion_t(
	const user_initialised_report_t<true>::source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const std::optional<std::string_view>& _a_msg,
	const matcher_result_t& _a_matcher_result,
	const matcher_source_map_t& _a_matcher_source_map
) noexcept
	: generic_assertion_t<true, Assertion_Status>(
		Assertion_Status(_a_matcher_result.passed()),
		_a_source,
		_a_log_infos, _a_msg)
	, _m_matcher_result(_a_matcher_result)
	, _m_matcher_map(_a_matcher_source_map)
{

}
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
const matcher_result_t&
assertion_t<Assertion_Status>::matcher_result(
) const noexcept
{
	return _m_matcher_result;
}
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
const matcher_source_map_t&
assertion_t<Assertion_Status>::source_map(
) const noexcept
{
	return _m_matcher_map;
}
_END_ABC_REPORTS_NS
