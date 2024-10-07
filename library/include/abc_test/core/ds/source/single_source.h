#pragma once
#include "abc_test/utility/internal/macros.h"
#include <source_location>
#include <string>
#include <fmt/std.h>

_BEGIN_ABC_REPORTS_NS

class single_source_t
{
public:
	__constexpr
		single_source_t(
		) noexcept;
	__constexpr
		single_source_t(
			const std::string_view _a_str,
			const std::source_location& _a_sl
		) noexcept;
	__constexpr
		const std::string_view
		str(
		) const noexcept;
	__constexpr
		const std::source_location&
		source_location(
		) const noexcept;
	__constexpr
		bool
		operator==(
			const single_source_t& _a_rhs
			) const noexcept;
private:
	std::string _m_str;
	std::source_location _m_sl;
};

_END_ABC_REPORTS_NS

template
<
>
struct fmt::formatter<abc::reports::single_source_t> : formatter<string_view> {
	// parse is inherited from formatter<string_view>.
	//Can'tbe constexpr due to use of fmt::format
	__no_constexpr
		auto
		format(
			abc::reports::single_source_t _a_rtd,
			format_context& _a_ctx
		) const
		->format_context::iterator;
};

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
single_source_t::single_source_t(
) noexcept
{

}
__constexpr_imp
single_source_t::single_source_t(
	const std::string_view _a_str,
	const std::source_location& _a_sl
) noexcept
	: _m_str(_a_str)
	, _m_sl(_a_sl)
{

}
__constexpr_imp
const std::string_view
single_source_t::str(
) const noexcept
{
	return _m_str;
}
__constexpr_imp
const std::source_location&
single_source_t::source_location(
) const noexcept
{
	return _m_sl;
}
__constexpr_imp
bool
single_source_t::operator==(
	const single_source_t& _a_rhs
	) const noexcept
{
	__cmp_test(_m_str);
	__cmp_test(_m_sl.column());
	__cmp_test(_m_sl.file_name());
	__cmp_test(_m_sl.function_name());
	__cmp_test(_m_sl.line());
	return true;
}
_END_ABC_REPORTS_NS

__no_constexpr_imp
auto
fmt::formatter<abc::reports::single_source_t>::format(
	abc::reports::single_source_t _a_rtd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	const string _l_rv{ fmt::format(
		"{0} {{"
		"{1} = {2}, "
		"{3} = {4}"
		"}}",
		typeid(_a_rtd).name(),
		"_m_str", _a_rtd.str(),
		"_m_sl", _a_rtd.source_location()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}