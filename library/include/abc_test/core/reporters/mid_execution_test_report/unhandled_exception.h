
#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/mid_execution_test_report/unhandled_exception_not_derived_from_std_exception.h"
#include <string>
#include <vector>
#include <source_location>

_BEGIN_ABC_REPORTERS_NS
using str_collection_t = std::vector<std::string>;
class unhandled_exception_t : public unhandled_exception_not_derived_from_std_exception_t
{
public:
	__constexpr
		unhandled_exception_t(
			const std::source_location& _a_source_location,
			const std::string_view _a_error_msg,
			const str_collection_t& _a_log_info,
			const std::string_view _a_exception_type,
			const std::source_location& _a_last_source_location_logged
		) noexcept;
	__constexpr
		virtual
		std::string
		unformatted_string(
		) const noexcept override;
	__constexpr
		const std::string_view
		exception_type(
		) const noexcept;
	__constexpr
		const std::string_view
		exception_error_msg(
		) const noexcept;
private:
	std::string _m_exception_type;
	std::string _m_exception_error_msg;
	std::source_location _m_last_source_location_logged;
};
_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	unhandled_exception_t::unhandled_exception_t(
		const std::source_location& _a_source_location,
		const std::string_view _a_error_msg,
		const str_collection_t& _a_log_info,
		const std::string_view _a_exception_type,
		const std::source_location& _a_last_source_location_logged
	) noexcept
	: unhandled_exception_not_derived_from_std_exception_t(_a_source_location,_a_log_info, _a_last_source_location_logged)
	, _m_exception_type(_a_exception_type)
	, _m_exception_error_msg(_a_error_msg)
{

}
__constexpr_imp
	std::string
	unhandled_exception_t::unformatted_string(
	) const noexcept
{
	return fmt::format(
		"{0}{{{1},"
		"{2} = {3},"
		"{4} = {5}}}",
		typeid(*this),
		unformatted_base_string(), "_m_exception_type", _m_exception_type,
		"_m_exception_error_msg", _m_exception_error_msg
	);
}
__constexpr_imp
	const std::string_view
	unhandled_exception_t::exception_type(
	) const noexcept
{
	return _m_exception_type;
}
__constexpr_imp
	const std::string_view
	unhandled_exception_t::exception_error_msg(
	) const noexcept
{
	return _m_exception_error_msg;
}
_END_ABC_REPORTERS_NS