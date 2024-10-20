#pragma once
#include "abc_test/internal/utility/internal/macros.h"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/user_initialised_report.h"
#include "abc_test/internal/test_reports/user_initialised_report.h"
#include "abc_test/included_instances/reporters/text_test_reporter/print_config.h"
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter.h"
_BEGIN_ABC_REPORTERS_NS
template<
	bool Single_Source
>
struct user_initialised_report_list_formatter_t
	: public list_formattable_t< reports::user_initialised_report_t<Single_Source>, enum_user_initialised_report_fields_t, print_config_t>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const enum_user_initialised_report_fields_t& _a_fid,
			const reports::user_initialised_report_t<Single_Source>& _a_element
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_user_initialised_report_fields_t& _a_fid,
			const reports::user_initialised_report_t<Single_Source>& _a_element,
			const print_config_t& _a_pc
		) const;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template<
	bool Single_Source
>
__constexpr_imp
bool
user_initialised_report_list_formatter_t<Single_Source>::check_data(
	const enum_user_initialised_report_fields_t& _a_fid,
	const reports::user_initialised_report_t<Single_Source>& _a_element
) const
{
	using namespace errors;
	using enum enum_user_initialised_report_fields_t;
	switch (_a_fid)
	{
	case SOURCE:
	//case STR_REPRESENTATION:
		return true;
	case LOG_INFOS:
		return _a_element.log_infos().size() > 0;
	default:
		throw unaccounted_for_enum_exception(_a_fid);
	}
}
template<
	bool Single_Source
>
__constexpr_imp
std::vector<std::string>
user_initialised_report_list_formatter_t<Single_Source>::get_data(
	const enum_user_initialised_report_fields_t& _a_fid,
	const reports::user_initialised_report_t<Single_Source>& _a_element,
	const print_config_t& _a_pc
) const
{
	using namespace std;
	using namespace errors;
	using enum enum_user_initialised_report_fields_t;
	switch (_a_fid)
	{
	//case STR_REPRESENTATION:
	//	return
	//	{
	//		str_representation()
	//	};
	case SOURCE:
		if constexpr (Single_Source)
		{
			return
			{
				_a_pc.colon(_a_pc.source_location_str()),
				_a_pc.indent(_a_pc.source_location(_a_element.source().source_location())),
				_a_pc.colon(_a_pc.source_code_str()),
				_a_pc.indent(_a_pc.source_representation(_a_element.source().source_code_representation()))
			};
		}
		else
		{
			vector<string> _l_rv =
			{
				_a_pc.colon(_a_pc.source_location_pair_begin_str()),
				_a_pc.indent(_a_pc.colon(_a_pc.source_location_str())),
				_a_pc.indent(_a_pc.source_location(_a_element.source().begin_source().source_location()),2),
				_a_pc.indent(_a_pc.colon(_a_pc.source_code_str())),
				_a_pc.indent(_a_pc.source_representation(_a_element.source().begin_source().source_code_representation()),2),
				_a_pc.colon(_a_pc.source_location_pair_end_str()),
			};
			if (not _a_element.source().end_source().has_value())
			{
				_l_rv.push_back(_a_pc.indent(_a_pc.highlight_fail(std::string(_a_pc.no_end_source()))));
			}
			else
			{
				const ds::single_source_t _l_end{ _a_element.source().end_source().value()};
				_l_rv.append_range(
					std::vector<std::string>({
					_a_pc.indent(_a_pc.colon(_a_pc.source_location_str())),
					_a_pc.indent(_a_pc.source_location(_l_end.source_location()), 2),
					_a_pc.indent(_a_pc.colon(_a_pc.source_code_str())),
					_a_pc.indent(_a_pc.source_representation(_l_end.source_code_representation()), 2)
					}));
			}
			return _l_rv;
		}

	case LOG_INFOS:
	{
		vector<string> _l_rv{ _a_pc.colon(_a_pc.log_info_str()) };
		for (const string_view& _l_str : _a_element.log_infos())
		{
			_l_rv.push_back(_a_pc.indent(_a_pc.log_info(_l_str)));
		}
		return _l_rv;
	}
	default:
		throw unaccounted_for_enum_exception(_a_fid);
	}
}
_END_ABC_REPORTERS_NS