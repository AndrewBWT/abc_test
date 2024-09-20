
#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/mid_execution_test_report.h"

_BEGIN_ABC_REPORTERS_NS
using str_collection_t = std::vector<std::string>;
class test_assertion_result_t : public mid_execution_test_report_t
{
public:
	__constexpr
		test_assertion_result_t(
			const bool _a_passed,
			const std::string_view _a_root_source_code_representation,
			const std::source_location& _a_source_location,
			const matcher_source_map_t& _a_matcher_source_map,
			const str_collection_t& _a_log_info,
			const bool _a_early_termination,
			const std::optional<std::string_view>& _a_matcher_str
		) noexcept;
	__constexpr
		const std::string_view
		source_code_representation(
		) const noexcept;
	__constexpr
		const std::optional<std::string>&
		matcher_str(
		) const noexcept;
	__constexpr
		virtual
		std::string
		unformatted_string(
		) const noexcept override;
private:
	matcher_source_map_t _m_matcher_source_map;
	std::string _m_scr;
	std::optional<std::string> _m_matcher_str;
};
_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	test_assertion_result_t::test_assertion_result_t(
		const bool _a_passed,
		const std::string_view _a_root_source_code_representation,
		const std::source_location& _a_source_location,
		const matcher_source_map_t& _a_matcher_source_map,
		const str_collection_t& _a_log_info,
		const bool _a_early_termination,
		const std::optional<std::string_view>& _a_matcher_str
	) noexcept
	: mid_execution_test_report_t(_a_passed,
		_a_source_location, _a_early_termination,
		_a_log_info)
	, _m_matcher_source_map(_a_matcher_source_map)
	, _m_scr(_a_root_source_code_representation)
	, _m_matcher_str(_a_matcher_str)
{

}
__constexpr_imp
	const std::string_view
	test_assertion_result_t::source_code_representation(
	) const noexcept
{
	return _m_scr;
}
__constexpr_imp
	const std::optional<std::string>&
	test_assertion_result_t::matcher_str(
) const noexcept
{
	return _m_matcher_str;
}
__constexpr_imp
	std::string
	test_assertion_result_t::unformatted_string(
	) const noexcept
{
	return fmt::format(
		"{0}{{{1},"
		"{2} = {3},"
		"{4} = {5},"
		"{6} = {7}}}",
		typeid(*this),
		unformatted_base_string(), "_m_matcher_source_map", _m_matcher_source_map,
		"_m_scr", _m_scr,
		"_m_matcher_str", _m_matcher_str
	);
}
_END_ABC_REPORTERS_NS