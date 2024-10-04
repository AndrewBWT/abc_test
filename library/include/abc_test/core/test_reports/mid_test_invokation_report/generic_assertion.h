#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/user_initialised_report.h"
#include <concepts>
#include <optional>
#include <string>
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/assertion_status_base.h"

#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/fail.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/terminate.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass_or_terminate.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass_or_fail.h"

_BEGIN_ABC_REPORTS_NS
class generic_assertion_type_t
{
public:
	__constexpr_imp
		virtual
		~generic_assertion_type_t(
		) noexcept
	{

	}
};
using generic_user_report_ptr_t = std::shared_ptr<const generic_assertion_type_t>;
using generic_user_report_collection_t = std::vector<generic_user_report_ptr_t>;
template<
	bool Single_Source,
	typename Assertion_Status
>
requires std::derived_from<Assertion_Status,assertion_status_base_t>
class generic_assertion_t : 
	public user_initialised_report_t<Single_Source>,
	public generic_assertion_type_t
{
public:
	__constexpr
		const std::optional<std::string>&
		message(
		) const noexcept;
	__constexpr
		Assertion_Status
		status(
		) const noexcept;
	__constexpr
		bool
		get_pass_status(
		) const noexcept;
	__constexpr
		bool
		terminated(
		) const noexcept;
protected:
	__constexpr
		generic_assertion_t(
		) noexcept = delete;
	__constexpr
		generic_assertion_t(
			const Assertion_Status& _a_status,
			const user_initialised_report_t<Single_Source>::source_t& _a_source,
			const log_infos_t& _a_log_infos
		) noexcept;
	__constexpr
		generic_assertion_t(
			const Assertion_Status& _a_status,
			const user_initialised_report_t<Single_Source>::source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const std::string_view _a_msg
		) noexcept;
	__constexpr
		generic_assertion_t(
			const Assertion_Status& _a_status,
			const user_initialised_report_t<Single_Source>::source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const std::optional<std::string_view>& _a_msg
		) noexcept;
	Assertion_Status _m_status;
	std::optional<std::string> _m_msg;
private:
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
const std::optional<std::string>&
generic_assertion_t<Single_Source, Assertion_Status>::message(
) const noexcept
{
	return _m_msg;
}
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
Assertion_Status
generic_assertion_t<Single_Source, Assertion_Status>::status(
) const noexcept
{
	return _m_status;
}
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
bool
generic_assertion_t<Single_Source, Assertion_Status>::get_pass_status(
) const noexcept
{
	using namespace std;
	if constexpr (same_as<Assertion_Status, pass_t>)
	{
		return true;
	}
	else if constexpr (same_as<Assertion_Status, fail_t> || 
		same_as<Assertion_Status,terminate_t>)
	{
		return false;
	}
	else if constexpr (same_as<Assertion_Status, pass_or_fail_t> ||
		same_as<Assertion_Status, pass_or_terminate_t>)
	{
		return _m_status.pass();
	}
	else
	{
		static_assert(false, "Cannot instantiate");
	}
}
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
bool
generic_assertion_t<Single_Source, Assertion_Status>::terminated(
) const noexcept
{
	using namespace std;
	if constexpr (same_as<Assertion_Status, terminate_t>)
	{
		return true;
	}
	else if constexpr (same_as<Assertion_Status, pass_or_terminate_t>)
	{
		return not _m_status.pass();
	}
	else
	{
		return false;
	}
}
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
generic_assertion_t<Single_Source, Assertion_Status>::generic_assertion_t(
	const Assertion_Status& _a_status,
	const user_initialised_report_t<Single_Source>::source_t& _a_source,
	const log_infos_t& _a_log_infos
) noexcept
	: generic_assertion_t<Single_Source,Assertion_Status>(
		_a_status, _a_source, _a_log_infos, std::optional<std::string_view>{})
{

}
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
generic_assertion_t<Single_Source, Assertion_Status>::generic_assertion_t(
	const Assertion_Status& _a_status,
	const user_initialised_report_t<Single_Source>::source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const std::string_view _a_str
) noexcept
	: generic_assertion_t<Single_Source, Assertion_Status>(
		_a_status, _a_source, _a_log_infos, std::optional<std::string_view>{_a_str})
{

}
template<
	bool Single_Source,
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
generic_assertion_t<Single_Source, Assertion_Status>::generic_assertion_t(
	const Assertion_Status& _a_status,
	const user_initialised_report_t<Single_Source>::source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const std::optional<std::string_view>& _a_msg
) noexcept
	: user_initialised_report_t<Single_Source>(_a_source,_a_log_infos)
	, _m_status(_a_status)
	, _m_msg(_a_msg)
{

}
_END_ABC_REPORTS_NS
