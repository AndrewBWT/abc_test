#pragma once

#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_NS
class matcher_result_t
{
public:
	__constexpr
		matcher_result_t(
		) noexcept;
	__constexpr
		matcher_result_t(
			const bool _a_ran,
			const bool _a_passed,
			const std::string_view _a_str
		) noexcept;
	__constexpr
		bool
		passed(
		) const noexcept;
	__constexpr
		const std::string_view
		str(
		) const noexcept;
private:
	bool _m_ran;
	bool _m_passed;
	std::string _m_str;
};

_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	matcher_result_t::matcher_result_t(
	) noexcept
	: matcher_result_t(false, false, std::string_view{})
{

}
__constexpr_imp
	matcher_result_t::matcher_result_t(
		const bool _a_ran,
		const bool _a_passed,
		const std::string_view _a_str
	) noexcept
	: _m_ran(_a_ran)
	, _m_passed(_a_passed)
	, _m_str(_a_str)
{

}
__constexpr_imp
	bool
	matcher_result_t::passed(
	) const noexcept
{
	return _m_passed;
}
__constexpr_imp
	const std::string_view
	matcher_result_t::str(
	) const noexcept
{
	return _m_str;
}
_END_ABC_NS