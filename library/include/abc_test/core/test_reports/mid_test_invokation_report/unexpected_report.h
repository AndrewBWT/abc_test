#pragma once

#include "abc_test/utility/internal/macros.h"

#include <type_traits>
#include <concepts>
#include "abc_test/core/test_reports/mid_test_invokation_report/single_source.h"

_BEGIN_ABC_REPORTS_NS
using log_infos_t = std::vector<std::string>;
template<
	bool Terminate
>
class unexpected_report_t
{
public:
	__constexpr
		unexpected_report_t(
		) noexcept = delete;
	__constexpr
		unexpected_report_t(
			const single_source_t& _a_source,
			const bool _a_exact_source
		) noexcept;
	__constexpr
		unexpected_report_t(
			const source_pair_t& _a_sources,
			const bool _a_exact_source
		) noexcept;
	__constexpr
		virtual
		~unexpected_report_t(
		) noexcept;
	__constexpr
		const single_source_t&
		source(
		) const noexcept;
	__constexpr
		bool
		exact_source(
		) const noexcept;
protected:
	single_source_t _m_last_source;
	bool _m_exact_source;
};
using opt_unexpected_report_t = std::optional< std::shared_ptr<const unexpected_report_t<true>>>;
using unexpected_non_terminating_report_ptr_t = std::shared_ptr<const unexpected_report_t<false>>;
using unexpected_non_terminating_report_collection_t = std::vector<unexpected_non_terminating_report_ptr_t>;
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template<
	bool Terminate
>
__constexpr_imp
unexpected_report_t<Terminate>::unexpected_report_t(
	const single_source_t& _a_source,
	const bool _a_exact_source
) noexcept
	: _m_last_source(_a_source)
	, _m_exact_source(_a_exact_source)
{

}
template<
	bool Terminate
>
__constexpr_imp
unexpected_report_t<Terminate>::unexpected_report_t(
	const source_pair_t& _a_sources,
	const bool _a_exact_source
) noexcept
	//: _m_last_source(_a_source)
	//, _m_exact_source(_a_exact_source)
{

}
template<
	bool Terminate
>
__constexpr_imp
unexpected_report_t<Terminate>::~unexpected_report_t(
) noexcept
{

}
template<
	bool Terminate
>
__constexpr_imp
const single_source_t&
unexpected_report_t<Terminate>::source(
) const noexcept
{
	return _m_last_source;
}
template<
	bool Terminate
>
__constexpr_imp
bool
unexpected_report_t<Terminate>::exact_source(
) const noexcept
{
	return _m_exact_source;
}
_END_ABC_REPORTS_NS