#pragma once

#include "abc_test/core/test_reports/mid_test_invokation_report/unexpected_thrown_non_descript_entity.h"

_BEGIN_ABC_REPORTS_NS
class unexpected_thrown_exception_t :
	public unexpected_thrown_non_descript_entity_t
{
public:
	__constexpr
		unexpected_thrown_exception_t(
			const single_source_t& _a_source,
			const std::string_view _a_exception_name,
			const std::string_view _a_exception_what_function
		) noexcept;
	__constexpr
		const std::string_view
		exception_type(
		) const noexcept;
	__constexpr
		const std::string_view
		exception_message(
		) const noexcept;
protected:
	std::string _m_exception_name;
	std::string _m_exception_what_function;
};
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
unexpected_thrown_exception_t::unexpected_thrown_exception_t(
	const single_source_t& _a_source,
	const std::string_view _a_exception_name,
	const std::string_view _a_exception_what_function
) noexcept
	: unexpected_thrown_non_descript_entity_t(_a_source)
	, _m_exception_name(_a_exception_name)
	, _m_exception_what_function(_a_exception_what_function)
{

}
__constexpr_imp
const std::string_view
unexpected_thrown_exception_t::exception_type(
) const noexcept
{
	return _m_exception_name;
}
__constexpr_imp
const std::string_view
unexpected_thrown_exception_t::exception_message(
) const noexcept
{
	return _m_exception_what_function;
}
_END_ABC_REPORTS_NS