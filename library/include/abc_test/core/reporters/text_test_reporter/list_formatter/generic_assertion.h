#pragma once
#include "abc_test/core/reporters/text_test_reporter/enum_fields/generic_assertion.h"
#include "abc_test/core/reporters/text_test_reporter/list_formatter/user_initialised_report.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/generic_assertion.h"
_BEGIN_ABC_REPORTERS_NS
template<
	bool Single_Source,
	typename Assertion_Status
>
struct generic_assertion_list_formatter_t
	: public list_formattable_t< reports::generic_assertion_t<Single_Source, Assertion_Status>,
	combined_enum_generic_assertion_fields_t, print_config_t>,
	public user_initialised_report_list_formatter_t<Single_Source>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const combined_enum_generic_assertion_fields_t& _a_fid,
			const reports::generic_assertion_t<Single_Source, Assertion_Status>& _a_element
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_generic_assertion_fields_t& _a_fid,
			const reports::generic_assertion_t<Single_Source, Assertion_Status>& _a_element,
			const print_config_t& _a_pc
		) const;
	__constexpr
		virtual
		std::string
		get_str_representation(
			const reports::generic_assertion_t<Single_Source, Assertion_Status>& _a_element,
			const print_config_t& _a_pc
		) const = 0;
};
template<
	bool Single_Source,
	typename Assertion_Status
>
__constexpr
std::string
construct_str_representation(
	const reports::generic_assertion_t<Single_Source, Assertion_Status>& _a_element
) noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template<
	bool Single_Source,
	typename Assertion_Status
>
__constexpr_imp
bool
generic_assertion_list_formatter_t<Single_Source, Assertion_Status>::check_data(
	const combined_enum_generic_assertion_fields_t& _a_fid,
	const reports::generic_assertion_t<Single_Source, Assertion_Status>& _a_element
) const
{
	using namespace std;
	if (auto _l_ptr{ get_if< enum_user_initialised_report_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return user_initialised_report_list_formatter_t<Single_Source>::check_data(*_l_ptr, _a_element);
	}
	else if (auto _l_ptr{ get_if< enum_generic_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		using enum enum_generic_assertion_fields_t;
		switch (*_l_ptr)
		{
		case STATUS:
		case STR_REPRESENTATION:
			return true;
		case TEST_DESCRIPTION:
			return _a_element.test_description().has_value();
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_fid);
	}
}
template<
	bool Single_Source,
	typename Assertion_Status
>
__constexpr_imp
std::vector<std::string>
generic_assertion_list_formatter_t<Single_Source, Assertion_Status>::get_data(
	const combined_enum_generic_assertion_fields_t& _a_fid,
	const reports::generic_assertion_t<Single_Source, Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	using namespace std;
	if (auto _l_ptr{ get_if< enum_user_initialised_report_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return user_initialised_report_list_formatter_t<Single_Source>::get_data(*_l_ptr, _a_element, _a_pc);
	}
	else if (auto _l_ptr{ get_if< enum_generic_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		using enum enum_generic_assertion_fields_t;
		switch (*_l_ptr)
		{
		case STATUS:
			return
			{
				_a_pc.colon(_a_pc.status_str()),
				_a_pc.indent(_a_pc.status(_a_element.status()))
			};
		case TEST_DESCRIPTION:
			return
			{
				_a_pc.colon(_a_pc.test_description_str()),
				_a_pc.indent(_a_pc.message_str(_a_element.test_description()))
			};
		case STR_REPRESENTATION:
			return
			{
				_a_pc.highlight_pass_or_fail(
					_a_element.get_pass_status(),
					get_str_representation(_a_element,_a_pc))
			};
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_fid);
	}
}
template<
	bool Single_Source,
	typename Assertion_Status
>
__constexpr_imp
std::string
construct_str_representation(
	const reports::generic_assertion_t<Single_Source, Assertion_Status>& _a_element,
	const std::string_view _a_str
) noexcept
{
	using namespace std;
	using namespace reports;
	const bool _l_passed{ _a_element.get_pass_status() };
	const string _l_terminate_function_str{ (not _l_passed &&
			same_as<Assertion_Status, pass_or_terminate_t> ||
			same_as<Assertion_Status,terminate_t>) ?
		" Assertion terminated function." : "" };
	const string _l_status_str{ _l_passed ? "passed" : "failed" };
	return fmt::format("{0} {1}.{2}",
		_a_str,
		_l_status_str,
		_l_terminate_function_str);
}
_END_ABC_REPORTERS_NS