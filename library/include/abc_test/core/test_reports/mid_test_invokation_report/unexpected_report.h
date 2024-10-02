#pragma once

#include "abc_test/utility/internal/macros.h"

#include <type_traits>
#include <concepts>

//#include "abc_test/core/log_test_msg.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/single_source.h"

_BEGIN_ABC_REPORTS_NS
template<
	bool Terminate
>
class unexpected_report_t
{
public:
	__constexpr
		unexpected_report_t(
		) noexcept = delete;
	template<typename = typename std::enable_if<Terminate>::type>
	__constexpr
		unexpected_report_t(
			const single_source_t& _a_source,
			const log_infos_t& _a_log_infos,
			const bool _a_exact_source
		) noexcept;
	template<typename = typename std::enable_if<not Terminate>::type>
	__constexpr
		unexpected_report_t(
			const single_source_t& _a_source,
			const bool _a_exact_source
		) noexcept;
	single_source_t _m_last_source;
	bool _m_exact_source;
	std::conditional_t<Terminate,
		log_infos_t,
		std::monostate> _m_log_infos;
};
using opt_unexpected_report_t = std::optional< unexpected_report_t<true>>;
using unexpected_non_terminating_report_ptr_t = std::shared_ptr<const unexpected_report_t<false>>;
using unexpected_non_terminating_report_collection_t = std::vector<unexpected_non_terminating_report_ptr_t>;
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	bool Terminate
>
template<typename>
__constexpr_imp
unexpected_report_t<Terminate>::unexpected_report_t(
	const single_source_t& _a_source,
	const log_infos_t& _a_log_infos,
	const bool _a_exact_source
) noexcept
	: _m_last_source(_a_source)
	, _m_log_infos(_a_log_infos)
	, _m_exact_source(_a_exact_source)
{

}
template<
	bool Terminate
>
template<typename>
__constexpr_imp
unexpected_report_t<Terminate>::unexpected_report_t(
	const single_source_t& _a_source,
	const bool _a_exact_source
) noexcept
	: _m_last_source(_a_source)
	, _m_log_infos(std::monostate())
	, _m_exact_source(_a_exact_source)
{

}
_END_ABC_REPORTS_NS