#pragma once
#include "abc_test/core/test_reports/mid_test_invokation_report/single_source.h"

_BEGIN_ABC_REPORTS_NS

struct source_pair_t
{
public:
	__constexpr
		source_pair_t(
			const single_source_t& _a_begin_source
		) noexcept;
	__constexpr
		source_pair_t(
			const single_source_t& _a_begin_source,
			const single_source_t& _a_end_source
		) noexcept;
	__constexpr
		const single_source_t&
		begin_source(
		) const noexcept;
	__constexpr
		const std::optional<single_source_t>&
		end_source(
		) const noexcept;
private:
	single_source_t _m_begin_source;
	std::optional<single_source_t> _m_end_source;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
source_pair_t::source_pair_t(
	const single_source_t& _a_begin_source
) noexcept
	: _m_begin_source(_a_begin_source)
	, _m_end_source(std::optional<single_source_t>())
{

}
__constexpr_imp
source_pair_t::source_pair_t(
	const single_source_t& _a_begin_source,
	const single_source_t& _a_end_source
) noexcept
	: _m_begin_source(_a_begin_source)
	, _m_end_source(std::optional<single_source_t>(_a_end_source))
{

}
__constexpr_imp
const single_source_t&
source_pair_t::begin_source(
) const noexcept
{
	return _m_begin_source;
}
__constexpr_imp
const std::optional<single_source_t>&
source_pair_t::end_source(
) const noexcept
{
	return _m_end_source;
}
_END_ABC_REPORTS_NS