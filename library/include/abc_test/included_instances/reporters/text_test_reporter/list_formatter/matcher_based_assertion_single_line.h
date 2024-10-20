#pragma once
#include "abc_test/internal/test_reports/matcher_based_assertion_single_line.h"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion_single_line.h"
_BEGIN_ABC_REPORTERS_NS
template<
	typename Assertion_Status
>
struct matcher_based_assertion_single_line_list_formatter_t
	: public list_formattable_t< reports::matcher_based_assertion_single_line_t<Assertion_Status>,
	combined_enum_matcher_based_assertion_single_line_fields_t, print_config_t>,
	public matcher_based_assertion_list_formatter_t<true, Assertion_Status>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const combined_enum_matcher_based_assertion_single_line_fields_t& _a_fid,
			const reports::matcher_based_assertion_single_line_t<Assertion_Status>& _a_element
		) const override;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_matcher_based_assertion_single_line_fields_t& _a_fid,
			const reports::matcher_based_assertion_single_line_t<Assertion_Status>& _a_element,
			const print_config_t& _a_pc
		) const override;
protected:
	__constexpr
		virtual
		std::string
		get_str_representation(
			const reports::assertion_t<true, Assertion_Status>& _a_element,
			const print_config_t& _a_pc
		) const override;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template<
	typename Assertion_Status
>
__constexpr_imp
bool
matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::check_data(
	const combined_enum_matcher_based_assertion_single_line_fields_t& _a_fid,
	const reports::matcher_based_assertion_single_line_t<Assertion_Status>& _a_element
) const
{
	using namespace std;
	if (auto _l_ptr{ get_if< enum_matcher_based_assertion_single_line_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		switch (*_l_ptr)
		{
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else if (auto _l_ptr{ get_if< combined_enum_matcher_based_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return matcher_based_assertion_list_formatter_t<true, Assertion_Status>::check_data(*_l_ptr, _a_element);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_fid);
	}
}
template<
	typename Assertion_Status
>
__constexpr_imp
std::vector<std::string>
matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::get_data(
	const combined_enum_matcher_based_assertion_single_line_fields_t& _a_fid,
	const reports::matcher_based_assertion_single_line_t<Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	using namespace std;
	if (auto _l_ptr{ get_if< enum_matcher_based_assertion_single_line_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		switch (*_l_ptr)
		{
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else if (auto _l_ptr{ get_if< combined_enum_matcher_based_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return matcher_based_assertion_list_formatter_t<true, Assertion_Status>::get_data(*_l_ptr, _a_element, _a_pc);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_fid);
	}
}
template<
	typename Assertion_Status
>
__constexpr_imp
std::string
matcher_based_assertion_single_line_list_formatter_t<Assertion_Status>::get_str_representation(
	const reports::assertion_t<true, Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	return construct_str_representation(_a_element, "Matcher-based assertion");
}
_END_ABC_REPORTERS_NS