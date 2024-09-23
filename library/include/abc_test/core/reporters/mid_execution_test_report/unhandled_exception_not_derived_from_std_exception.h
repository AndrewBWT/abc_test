
#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/mid_execution_test_report.h"
#include <string>
#include <vector>
#include <source_location>

_BEGIN_ABC_REPORTERS_NS
using str_collection_t = std::vector<std::string>;
class unhandled_exception_not_derived_from_std_exception_t : 
	public mid_execution_test_report_t
{
public:
	__constexpr
		unhandled_exception_not_derived_from_std_exception_t(
			const std::source_location& _a_source_location,
			const str_collection_t& _a_log_info,
			const std::source_location& _m_last_source_location_logged
		) noexcept;
	__constexpr
		virtual
		std::string
		unformatted_string(
		) const noexcept override;
	__constexpr
		const std::source_location&
		last_source_location_logged(
		) const noexcept;
private:
	std::source_location _m_last_source_location_logged;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	unhandled_exception_not_derived_from_std_exception_t::
	unhandled_exception_not_derived_from_std_exception_t(
		const std::source_location& _a_source_location,
		const str_collection_t& _a_log_info,
		const std::source_location& _m_last_source_location_logged
	) noexcept
	: mid_execution_test_report_t(false, _a_source_location, true,_a_log_info)
	, _m_last_source_location_logged(_m_last_source_location_logged)
{

}
__constexpr_imp
	std::string
	unhandled_exception_not_derived_from_std_exception_t::unformatted_string(
) const noexcept
{
	return fmt::format(
		"{0}{{{1}}}",
		typeid(*this),
		unformatted_base_string()
	);
}
__constexpr_imp
	const std::source_location&
	unhandled_exception_not_derived_from_std_exception_t::last_source_location_logged(
	) const noexcept
{
	return _m_last_source_location_logged;
}
_END_ABC_REPORTERS_NS
