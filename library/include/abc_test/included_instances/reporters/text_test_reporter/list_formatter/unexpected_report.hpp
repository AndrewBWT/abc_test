#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/unexpected_report.hpp"
#include "abc_test/internal/test_reports/unexpected_report.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.hpp"
_BEGIN_ABC_REPORTERS_NS
template<
	bool Terminating
>
struct unexpected_report_list_formatter_t
	: public list_formattable_t< reports::unexpected_report_t<Terminating>, enum_unexpected_report_fields_t, print_config_t>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const enum_unexpected_report_fields_t& _a_fid,
			const reports::unexpected_report_t<Terminating>& _a_element
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_unexpected_report_fields_t& _a_fid,
			const reports::unexpected_report_t<Terminating>& _a_element,
			const print_config_t& _a_pc
		) const;
protected:
	__constexpr
		virtual
		std::string
		get_str_representation(
			const reports::unexpected_report_t<Terminating>& _a_element,
			const print_config_t& _a_pc
		) const = 0;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template<
	bool Terminating
>
__constexpr_imp
bool
unexpected_report_list_formatter_t<Terminating>::check_data(
	const enum_unexpected_report_fields_t& _a_fid,
	const reports::unexpected_report_t<Terminating>& _a_element
) const
{
	using namespace errors;
	using enum enum_unexpected_report_fields_t;
	switch (_a_fid)
	{
	case SOURCE:
	case STR_REPRESENTATION:
		return true;
	default:
		throw unaccounted_for_enum_exception(_a_fid);
	}
}
template<
	bool Terminating
>
__constexpr_imp
std::vector<std::string>
unexpected_report_list_formatter_t<Terminating>::get_data(
	const enum_unexpected_report_fields_t& _a_fid,
	const reports::unexpected_report_t<Terminating>& _a_element,
	const print_config_t& _a_pc
) const
{
	/**/using namespace std;
	using namespace errors;
	using enum enum_unexpected_report_fields_t;
	switch (_a_fid)
	{
	case SOURCE:
		return
		{
			_a_pc.indent(_a_pc.colon(_a_pc.potential_source_str(_a_element.exact_source()))),
			_a_pc.indent(_a_pc.source_location(_a_element.source().source_location()),2),
			_a_pc.indent(_a_pc.colon(_a_pc.potential_code_str(_a_element.exact_source()))),
			_a_pc.indent(_a_pc.source_representation(_a_element.source().source_code_representation()),2)
		};
	case STR_REPRESENTATION:
		return
		{
			_a_pc.highlight_fail(
				get_str_representation(_a_element,_a_pc))
		};
	default:
		throw unaccounted_for_enum_exception(_a_fid);
	}
}
_END_ABC_REPORTERS_NS