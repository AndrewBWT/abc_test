#pragma once

#include "abc_test/utility/internal/macros.h"
#include <fmt/color.h>
#include "abc_test/core/errors/test_library_exception.h"
#include "abc_test/core/reporters/text_test_reporter/enum_test_overview_data.h"
#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/reporters/text_test_reporter/colour_palette.h"
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"

#include <iomanip>

_BEGIN_ABC_REPORTERS_NS
struct test_overview_printer_config_t
{
public:
	__constexpr
		virtual
		const char*
		info_id(
			const enum_test_overview_data_t _a_enum
		) const;
	__constexpr
		virtual
		bool
		check_data(
			const std::pair<enum_test_overview_data_t, enum_print_data_t>& _a_pair,
			const after_execution_test_report_t& _a_report
		) const;
	__constexpr
		virtual
		bool
		check_contextual_data(
			const enum_test_overview_data_t _a_enum,
			const after_execution_test_report_t& _a_report
		) const;
	__constexpr
		virtual
		std::string
		get_data(
			const enum_test_overview_data_t _a_enum,
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const;
private:
	__constexpr
		virtual
		const char*
		title_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		name_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		location_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		test_path_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		seed_used_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		seed_used_in_hex_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		seed_to_use_to_re_run_test_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		seed_to_use_to_re_run_test_in_hex_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		total_assertions_ran_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		total_assertions_passed_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		total_assertions_failed_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		condesned_assertions_info_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		total_warnings_id(
		) const noexcept;
	__constexpr
		virtual
		const char*
		terminated_early_id(
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_title(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_test_path(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_seed_used(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_seed_used_hex(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_seed_to_use_to_re_run_test(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_seed_to_use_to_re_run_test_in_hex(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_total_assertions_ran(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_total_assertions_passed(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_total_assertions_failed(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_condensed_assertions_info(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_total_warnings(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		bool
		contextual_terminated_early(
			const after_execution_test_report_t& _a_aetr
		) const noexcept;
	__constexpr
		virtual
		std::string
		location_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		name_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		title_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		test_path_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		seed_used_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		seed_used_in_hex_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		seed_used_to_re_run_test_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		seed_used_to_re_run_test_in_hex_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		total_assertions_ran_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		total_assertions_passed_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		total_assertions_failed_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		condensed_assertions_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		total_warnings_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::string
		terminated_early_info(
			const colour_palette_t& _a_colours,
			const after_execution_test_report_t& _a_report
		) const noexcept;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS

__constexpr_imp
	const char*
	test_overview_printer_config_t::info_id(
		const enum_test_overview_data_t _a_enum
	) const
{
	using enum enum_test_overview_data_t;
	switch (_a_enum)
	{
	case LOCATION:
		return location_id();
	case NAME:
		return name_id();
	case TITLE:
		return title_id();
	case TEST_PATH:
		return test_path_id();
	case SEED_USED:
		return seed_used_id();
	case SEED_USED_HEX:
		return seed_used_in_hex_id();
	case SEED_TO_USE_TO_RE_RUN_TEST:
		return seed_to_use_to_re_run_test_id();
	case SEED_TO_USE_TO_RE_RUN_TEST_HEX:
		return seed_to_use_to_re_run_test_in_hex_id();
	case TOTAL_ASSERTIONS_RAN:
		return total_assertions_ran_id();
	case TOTAL_ASSERTIONS_PASSED:
		return total_assertions_passed_id();
	case TOTAL_ASSERTIONS_FAILED:
		return total_assertions_failed_id();
	case CONDENSED_ASSERTIONS_INFO:
		return condesned_assertions_info_id();
	case TOTAL_WARNINGS:
		return total_warnings_id();
	case TERMINATED_EARLY:
		return terminated_early_id();
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
	bool
	test_overview_printer_config_t::check_data(
		const std::pair<enum_test_overview_data_t, enum_print_data_t>& _a_pair,
		const after_execution_test_report_t& _a_report
	) const
{
	return (always_print(_a_pair.second) ||
		constexual_print(_a_pair.second) && check_contextual_data(_a_pair.first, _a_report));
}
__constexpr_imp
	bool
	test_overview_printer_config_t::check_contextual_data(
		const enum_test_overview_data_t _a_enum,
		const after_execution_test_report_t& _a_aetr
	) const
{
	using enum enum_test_overview_data_t;
	switch (_a_enum)
	{
	case TITLE:
		return contextual_title(_a_aetr);
	case TEST_PATH:
		return contextual_test_path(_a_aetr);
	case SEED_USED:
		return contextual_seed_used(_a_aetr);
	case SEED_USED_HEX:
		return contextual_seed_used_hex(_a_aetr);
	case SEED_TO_USE_TO_RE_RUN_TEST:
		return contextual_seed_to_use_to_re_run_test(_a_aetr);
	case SEED_TO_USE_TO_RE_RUN_TEST_HEX:
		return contextual_seed_to_use_to_re_run_test_in_hex(_a_aetr);
	case TOTAL_ASSERTIONS_RAN:
		return contextual_total_assertions_ran(_a_aetr);
	case TOTAL_ASSERTIONS_PASSED:
		return contextual_total_assertions_passed(_a_aetr);
	case TOTAL_ASSERTIONS_FAILED:
		return contextual_total_assertions_failed(_a_aetr);
	case CONDENSED_ASSERTIONS_INFO:
		return contextual_condensed_assertions_info(_a_aetr);
	case TOTAL_WARNINGS:
		return contextual_total_warnings(_a_aetr);
	case TERMINATED_EARLY:
		return contextual_terminated_early(_a_aetr);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::get_data(
		const enum_test_overview_data_t _a_enum,
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const
{
	using enum enum_test_overview_data_t;
	switch (_a_enum)
	{
	case LOCATION:
		return location_info(_a_colours, _a_report);
	case NAME:
		return name_info(_a_colours, _a_report);
	case TITLE:
		return title_info(_a_colours,_a_report);
	case TEST_PATH:
		return test_path_info(_a_colours, _a_report);
	case SEED_USED:
		return seed_used_info(_a_colours, _a_report);
	case SEED_USED_HEX:
		return seed_used_in_hex_info(_a_colours, _a_report);
	case SEED_TO_USE_TO_RE_RUN_TEST:
		return seed_used_to_re_run_test_info(_a_colours, _a_report);
	case SEED_TO_USE_TO_RE_RUN_TEST_HEX:
		return seed_used_to_re_run_test_in_hex_info(_a_colours, _a_report);
	case TOTAL_ASSERTIONS_RAN:
		return total_assertions_ran_info(_a_colours, _a_report);
	case TOTAL_ASSERTIONS_PASSED:
		return total_assertions_passed_info(_a_colours, _a_report);
	case TOTAL_ASSERTIONS_FAILED:
		return total_assertions_failed_info(_a_colours, _a_report);
	case CONDENSED_ASSERTIONS_INFO:
		return condensed_assertions_info(_a_colours, _a_report);
	case TOTAL_WARNINGS:
		return total_warnings_info(_a_colours, _a_report);
	case TERMINATED_EARLY:
		return terminated_early_info(_a_colours, _a_report);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::title_id(
	) const noexcept
{
	return "TEST_INFO";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::name_id(
	) const noexcept
{
	return "Test description";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::location_id(
	) const noexcept
{
	return "Location";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::test_path_id(
	) const noexcept
{
	return "Test path";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::seed_used_id(
	) const noexcept
{
	return "Seed used";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::seed_used_in_hex_id(
	) const noexcept
{
	return "Seed used (in hex)";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::seed_to_use_to_re_run_test_id(
	) const noexcept
{
	return "Seed to use to re-run test";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::seed_to_use_to_re_run_test_in_hex_id(
	) const noexcept
{
	return "Seed to use to re-run test (in hex)";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::total_assertions_ran_id(
	) const noexcept
{
	return "Total assertions ran";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::total_assertions_passed_id(
	) const noexcept
{
	return "Total assertions passed";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::total_assertions_failed_id(
	) const noexcept
{
	return "Total assertions failed";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::condesned_assertions_info_id(
	) const noexcept
{
	return "Assertions info";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::total_warnings_id(
	) const noexcept
{
	return "Total warnings recieved";
}
__constexpr_imp
	const char*
	test_overview_printer_config_t::terminated_early_id(
	) const noexcept
{
	return "Early termination";
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_title(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_test_path(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_seed_used(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return _a_aetr.seed_used() != "";
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_seed_used_hex(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return _a_aetr.seed_used_in_hex() != "";
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_seed_to_use_to_re_run_test(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return _a_aetr.seed_to_use_to_re_run_text() != "";
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_seed_to_use_to_re_run_test_in_hex(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return _a_aetr.seed_to_use_to_re_run_text_in_hex() != "";
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_total_assertions_ran(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_total_assertions_passed(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_total_assertions_failed(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_condensed_assertions_info(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return true;
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_total_warnings(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return _a_aetr.test_warnings_recieved();
}
__constexpr_imp
	bool
	test_overview_printer_config_t::contextual_terminated_early(
		const after_execution_test_report_t& _a_aetr
	) const noexcept
{
	return _a_aetr.terminated_early();
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::location_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace utility::str;
	return _a_report.location().has_value() ?
		_a_colours.style_highlight(location_string(_a_report.location().value())) :
		_a_colours.style_failure("<no location>");
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::name_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	return _a_report.name().has_value() ?
		_a_colours.style_highlight(_a_report.name().value()) :
		_a_colours.style_failure("<no name>");
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::title_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	return _a_report.passed() ?
		_a_colours.style_pass("TEST PASSED") :
		_a_colours.style_failure("TEST FAILED");
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::test_path_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace utility::str;
	return _a_colours.style_highlight(quote(_a_report.test_path()));
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::seed_used_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	return _a_colours.style_slight_highlight(_a_report.seed_used());
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::seed_used_in_hex_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace utility::str;
	return _a_colours.style_slight_highlight(quote(_a_report.seed_used_in_hex()));
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::seed_used_to_re_run_test_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	return _a_colours.style_slight_highlight(_a_report.seed_to_use_to_re_run_text());
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::seed_used_to_re_run_test_in_hex_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace utility::str;
	return _a_colours.style_slight_highlight(
		quote(_a_report.seed_to_use_to_re_run_text_in_hex()));
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::total_assertions_ran_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace std;
	return to_string(_a_report.test_reports_recieved());
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::total_assertions_passed_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace std;
	return to_string(_a_report.test_reports_passed());
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::total_assertions_failed_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace std;
	size_t _l_tr_passed{ _a_report.test_reports_passed() };
	const string _l_tr_str{ fmt::format("{0}",_l_tr_passed)};
	return _l_tr_passed == 0 ?
		_a_colours.style_pass(_l_tr_str) :
		_a_colours.style_failure(_l_tr_str);
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::condensed_assertions_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace std;
	const string _l_init_str{ fmt::format("{0} / {1}",
			_a_report.test_reports_passed(), _a_report.test_reports_recieved()) };
	return _a_report.passed() ?
		_a_report.test_reports_recieved() == 0 ? 
		_a_colours.style_pass(fmt::format("No assertions ran.")) : 
		_a_colours.style_pass(fmt::format("{0}. All assertions passed.", _l_init_str)) :
		_a_colours.style_failure(fmt::format("{0}. {1} assertions failed.", _l_init_str,
			_a_report.test_reports_failed()));
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::total_warnings_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	using namespace std;
	size_t _l_warnings{ _a_report.test_warnings_recieved() };
	string _l_str{ fmt::format("{0}",_l_warnings) };
	return _l_warnings == 0 ?
		_a_colours.style_pass(_l_str) :
		_a_colours.style_failure(_l_str);
}
__constexpr_imp
	std::string
	test_overview_printer_config_t::terminated_early_info(
		const colour_palette_t& _a_colours,
		const after_execution_test_report_t& _a_report
	) const noexcept
{
	return _a_report.terminated_early() ?
		_a_colours.style_failure("Yes") :
		_a_colours.style_pass("No");
}
_END_ABC_REPORTERS_NS