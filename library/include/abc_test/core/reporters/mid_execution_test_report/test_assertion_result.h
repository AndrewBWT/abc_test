
#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/mid_execution_test_report/manual_failure.h"
#include "abc_test/matchers/source_map.h"

_BEGIN_ABC_REPORTERS_NS
using str_collection_t = std::vector<std::string>;
class user_defined_assertion_t : public manual_assertion_t
{
public:
	__constexpr
		user_defined_assertion_t(
			const bool _a_passed,
			const std::string_view _a_root_source_code_representation,
			const std::source_location& _a_source_location,
			const matcher_source_map_t& _a_matcher_source_map,
			const str_collection_t& _a_log_info,
			const bool _a_early_termination,
			const std::optional<std::string_view>& _a_matcher_str
		) noexcept;
	__constexpr
		const std::optional<std::string>&
		matcher_str(
		) const noexcept;
	__constexpr
		virtual
		std::string
		unformatted_string(
		) const noexcept override;
	__constexpr
		const matcher_source_map_t&
		matcher_source_map(
		) const noexcept;
private:
	matcher_source_map_t _m_matcher_source_map;
	std::optional<std::string> _m_matcher_str;
};
_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	user_defined_assertion_t::user_defined_assertion_t(
		const bool _a_passed,
		const std::string_view _a_root_source_code_representation,
		const std::source_location& _a_source_location,
		const matcher_source_map_t& _a_matcher_source_map,
		const str_collection_t& _a_log_info,
		const bool _a_early_termination,
		const std::optional<std::string_view>& _a_matcher_str
	) noexcept
	: manual_assertion_t(_a_root_source_code_representation,_a_source_location,
		_a_log_info,_a_early_termination,_a_passed)
	, _m_matcher_source_map(_a_matcher_source_map)
	, _m_matcher_str(_a_matcher_str)
{
	/*
		const bool _a_passed
	*/
}
__constexpr_imp
	const std::optional<std::string>&
	user_defined_assertion_t::matcher_str(
) const noexcept
{
	return _m_matcher_str;
}
__constexpr_imp
	std::string
	user_defined_assertion_t::unformatted_string(
	) const noexcept
{
	return fmt::format(
		"{0}{{{1}"
		", {2} = {3}"
		", {4} = {5}"
		"}}",
		typeid(*this),
		unformatted_base_string(), "_m_matcher_source_map", _m_matcher_source_map,
		"_m_matcher_str", _m_matcher_str
	);
}
__constexpr_imp
	const matcher_source_map_t&
	user_defined_assertion_t::matcher_source_map(
	) const noexcept
{
	return _m_matcher_source_map;
}
_END_ABC_REPORTERS_NS