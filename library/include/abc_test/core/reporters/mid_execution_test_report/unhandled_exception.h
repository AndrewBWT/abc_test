
#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/mid_execution_test_report.h"
#include <string>
#include <vector>
#include <source_location>

_BEGIN_ABC_REPORTERS_NS
using str_collection_t = std::vector<std::string>;
class unhandled_exception_t : public mid_execution_test_report_t
{
public:
	__constexpr
		unhandled_exception_t(
			const std::source_location& _a_source_location,
			const std::string_view _a_error_msg,
			const str_collection_t& _a_log_info,
			const std::string_view _a_exception_type
		) noexcept;
	__constexpr
		virtual
		std::string
		unformatted_string(
		) const noexcept override;
private:
	std::string _m_exception_type;
	std::string _m_exception_error_msg;
};
_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	unhandled_exception_t::unhandled_exception_t(
		const std::source_location& _a_source_location,
		const std::string_view _a_error_msg,
		const str_collection_t& _a_log_info,
		const std::string_view _a_exception_type
	) noexcept
	: mid_execution_test_report_t(false, _a_source_location, true,_a_log_info)
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
_END_ABC_REPORTERS_NS