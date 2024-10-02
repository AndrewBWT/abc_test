#pragma once
#include "abc_test/core/reporters/text_test_reporter/enum_fields/after_execution_test_report.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter.h"
#include "abc_test/core/reporters/after_execution_test_report.h"
_BEGIN_ABC_REPORTERS_NS
struct test_report_list_formatter
	: public list_formattable_t< reporters::after_execution_test_report_t, enum_after_execution_test_report_fields_t, print_config_t>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const enum_after_execution_test_report_fields_t& _a_fid,
			const reporters::after_execution_test_report_t& _a_element
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_after_execution_test_report_fields_t& _a_fid,
			const reporters::after_execution_test_report_t& _a_element,
			const print_config_t& _a_pc
		) const;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
bool
test_report_list_formatter::check_data(
	const enum_after_execution_test_report_fields_t& _a_fid,
	const reporters::after_execution_test_report_t& _a_element
) const
{
	using enum enum_after_execution_test_report_fields_t;
	switch (_a_fid)
	{
	case STR_STATUS:
	case PASSED:
	case NAME:
	case SOURCE_LOCATION:
	case SOURCE_REPRESENTATION:
	case TEST_PATH:
	case SEED_USED:
	case SEED_USED_HEX:
	case SEED_TO_USE_TO_RE_RUN_TEST:
	case SEED_TO_USE_TO_RE_RUN_TEST_IN_HEX:
	case TEST_WARNINGS_RECIEVED:
	case ASSERTIONS_RECIEVED:
	case ASSERTIONS_PASSED:
	case ASSERTIONS_FAILED:
	case COMPACT_ASSERTION_DATA:
	case TERMINATION_STATUS:
		return true;
	default:
		throw errors::unaccounted_for_enum_exception(_a_fid);
	}
}
__constexpr_imp
std::vector<std::string>
test_report_list_formatter::get_data(
	const enum_after_execution_test_report_fields_t& _a_fid,
	const reporters::after_execution_test_report_t& _a_element,
	const print_config_t& _a_pc
) const
{
	using enum enum_after_execution_test_report_fields_t;
	switch (_a_fid)
	{
	case STR_STATUS:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.str_status_str())),
			_a_pc.str_status(_a_element.passed())
		};
	case PASSED:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.passed_str())),
			_a_pc.passed(_a_element.passed())
		};
	case NAME:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.name_str())),
			_a_pc.name(_a_element.name())
		};
	case SOURCE_LOCATION:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.source_location_str())),
			_a_pc.source_location(_a_element.location().source_location())
		};
	case SOURCE_REPRESENTATION:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.source_code_str())),
			_a_pc.source_representation(_a_element.location().str())
		};
	case TEST_PATH:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.test_path_str())),
			_a_pc.test_path(_a_element.test_path())
		};
	case SEED_USED:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.seed_used_str())),
			_a_pc.seed_used(_a_element.seed_used())
		};
	case SEED_USED_HEX:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.seed_used_in_hex_str())),
			_a_pc.seed_used_in_hex(_a_element.seed_used_in_hex())
		};
	case SEED_TO_USE_TO_RE_RUN_TEST:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.seed_to_use_to_re_run_test_str())),
			_a_pc.seed_to_use_to_re_run_test(_a_element.seed_to_use_to_re_run_text())
		};
	case SEED_TO_USE_TO_RE_RUN_TEST_IN_HEX:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.seed_to_use_to_re_run_test_in_hex_str())),
			_a_pc.seed_to_use_to_re_run_test_in_hex(_a_element.seed_to_use_to_re_run_text_in_hex())
		};
	case TEST_WARNINGS_RECIEVED:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.test_warnings_recieved_str())),
			_a_pc.test_warnings_recieved(_a_element.test_warnings_recieved())
		};
	case ASSERTIONS_RECIEVED:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.assertions_recieved_str())),
			_a_pc.assertions_recieved(_a_element.assertions_recieved())
		};
	case ASSERTIONS_PASSED:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.assertions_passed_str())),
			_a_pc.assertions_passed(_a_element.assertions_passed())
		};
	case ASSERTIONS_FAILED:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.assertions_failed_str())),
			_a_pc.assertions_failed(_a_element.assertions_failed())
		};
	case COMPACT_ASSERTION_DATA:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.compat_assertion_data_str())),
			_a_pc.compat_assertion_data(_a_element.assertions_passed(),
				_a_element.assertions_recieved(), _a_element.assertions_failed())
		};
	case TERMINATION_STATUS:
		return
		{
			_a_pc.space(_a_pc.colon(_a_pc.termination_status_str())),
			_a_pc.termination_status(_a_element.terminated_type())
		};
	default:
		throw errors::unaccounted_for_enum_exception(_a_fid);
	}
}
_END_ABC_REPORTERS_NS