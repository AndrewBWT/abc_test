#pragma once
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/text_test_reporter/enum_test_report_data.h"
#include <vector>
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/reporters/text_test_reporter/colour_palette.h"

_BEGIN_ABC_REPORTERS_NS
class test_report_data_config_t
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const std::pair<enum_test_report_data_t, enum_print_data_t>& _a_pair,
			const mid_execution_test_report_t& _a_report
		) const;
	__constexpr
		virtual
		bool
		check_contextual_data(
			const enum_test_report_data_t _a_enum,
			const mid_execution_test_report_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_test_report_data_t _a_enum,
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const;
private:
	__constexpr
		virtual
		bool
		check_passed_info(
			const mid_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		bool
		check_source_location(
			const mid_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		bool
		check_early_termination(
			const mid_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		bool
		check_log_infos(
			const mid_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		bool
		check_str_representation(
			const mid_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		passed_info_data(
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		source_location_data(
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		early_termination_data(
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		log_infos_data(
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		str_rep_data(
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept = 0;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
bool
test_report_data_config_t::check_data(
	const std::pair<enum_test_report_data_t, enum_print_data_t>& _a_pair,
	const mid_execution_test_report_t& _a_report
) const
{
	return (always_print(_a_pair.second) ||
		constexual_print(_a_pair.second) && check_contextual_data(_a_pair.first, _a_report));
}
__constexpr_imp
bool
test_report_data_config_t::check_contextual_data(
	const enum_test_report_data_t _a_enum,
	const mid_execution_test_report_t& _a_report
) const
{
	using enum enum_test_report_data_t;
	switch (_a_enum)
	{
	case PASSED:
		return check_passed_info(_a_report);
	case SOURCE_LOCATION:
		return check_source_location(_a_report);
	case EARLY_TERMINATION:
		return check_early_termination(_a_report);
	case LOG_INFOS:
		return check_log_infos(_a_report);
	case STR_REPRESENTATION:
		return check_str_representation(_a_report);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
	std::vector<std::string>
	test_report_data_config_t::get_data(
		const enum_test_report_data_t _a_enum,
		const colour_palette_t& _a_colours,
		const mid_execution_test_report_t& _a_report,
		const std::size_t _a_indent_size
	) const
{
	using enum enum_test_report_data_t;
	switch (_a_enum)
	{
	case PASSED:
		return passed_info_data(_a_colours,_a_report,_a_indent_size);
	case SOURCE_LOCATION:
		return source_location_data(_a_colours, _a_report, _a_indent_size);
	case EARLY_TERMINATION:
		return early_termination_data(_a_colours, _a_report, _a_indent_size);
	case LOG_INFOS:
		return log_infos_data(_a_colours, _a_report, _a_indent_size);
	case STR_REPRESENTATION:
		return str_rep_data(_a_colours, _a_report, _a_indent_size);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
	bool
	test_report_data_config_t::check_passed_info(
		const mid_execution_test_report_t& _a_report
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_report_data_config_t::check_source_location(
		const mid_execution_test_report_t& _a_report
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_report_data_config_t::check_early_termination(
		const mid_execution_test_report_t& _a_report
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_report_data_config_t::check_log_infos(
		const mid_execution_test_report_t& _a_report
	) const noexcept
{
	return (_a_report.log_infos().size() > 0);
}
__constexpr_imp
	bool
	test_report_data_config_t::check_str_representation(
		const mid_execution_test_report_t& _a_report
	) const noexcept
{
	return true;
}
__constexpr_imp
	std::vector<std::string>
	test_report_data_config_t::passed_info_data(
		const colour_palette_t& _a_colours,
		const mid_execution_test_report_t& _a_report,
		const std::size_t _a_indent_size
	) const noexcept
{
	return 
	{
		"Pass/Fail Status: " +
		(_a_report.passed() ?
		_a_colours.style_pass("Passed") :
		_a_colours.style_failure("Failed"))
	};
}
__constexpr_imp
	std::vector<std::string>
	test_report_data_config_t::source_location_data(
		const colour_palette_t& _a_colours,
		const mid_execution_test_report_t& _a_report,
		const std::size_t _a_indent_size
	) const noexcept
{
	using namespace utility::str;
	return
	{
		{"Location:"},
		indent(_a_indent_size,
			_a_colours.style_highlight(location_string(_a_report.source_location())))
	};
}
__constexpr_imp
	std::vector<std::string>
	test_report_data_config_t::early_termination_data(
		const colour_palette_t& _a_colours,
		const mid_execution_test_report_t& _a_report,
		const std::size_t _a_indent_size
	) const noexcept
{
	return
	{ 
		"Terminated Early: " +
		(_a_report.early_termination() ?
		_a_colours.style_failure("Yes") :
		_a_colours.style_pass("No"))
	};
}
__constexpr_imp
	std::vector<std::string>
	test_report_data_config_t::log_infos_data(
		const colour_palette_t& _a_colours,
		const mid_execution_test_report_t& _a_report,
		const std::size_t _a_indent_size
	) const noexcept
{
	using namespace std;
	using namespace utility::str;
	const vector<string>& _l_log_infos{ _a_report.log_infos() };
	vector<string> _l_rv{ "Log messages:" };
	for (const string_view _a_str : _l_log_infos)
	{
		_l_rv.push_back(indent(_a_indent_size,_a_colours.style_slight_highlight(_a_str)));
	}
	return _l_rv;
}
_END_ABC_REPORTERS_NS