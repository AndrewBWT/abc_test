
#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/mid_execution_test_report.h"
#include <source_location>
#include <string>
#include <vector>

_BEGIN_ABC_REPORTERS_NS
class manual_failure_t : public mid_execution_test_report_t
{
public:
	__constexpr
		manual_failure_t(
			const std::string_view _a_root_source_code_representation,
			const std::source_location& _a_source_location,
			const str_collection_t& _a_log_info,
			const bool _a_early_termination
		) noexcept;
	__constexpr
		const std::string_view
		source_code_representation(
		) const noexcept;
	__constexpr
		virtual
		std::string
		unformatted_string(
		) const noexcept override;
private:
	std::string _m_scr;
};
_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	manual_failure_t::manual_failure_t(
		const std::string_view _a_root_source_code_representation,
		const std::source_location& _a_source_location,
		const str_collection_t& _a_log_info,
		const bool _a_early_termination
	) noexcept
	: mid_execution_test_report_t(false, _a_source_location, _a_early_termination,
		_a_log_info)
	, _m_scr(_a_root_source_code_representation)
{

}
__constexpr_imp
	const std::string_view
	manual_failure_t::source_code_representation(
	) const noexcept
{
	return _m_scr;
}
__constexpr_imp
	std::string
	manual_failure_t::unformatted_string(
) const noexcept
{
	return fmt::format(
		"{0}{{{1},"
		"{2} = {3}}}",
		typeid(*this),
		unformatted_base_string(),"_m_scr",_m_scr
	);
}
_END_ABC_REPORTERS_NS