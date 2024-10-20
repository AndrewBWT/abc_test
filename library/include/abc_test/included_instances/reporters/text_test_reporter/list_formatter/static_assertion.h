#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/list_formatter/assertion.h"
#include "abc_test/internal/test_reports/assertion_status/pass_or_terminate.h"
#include "abc_test/internal/test_reports/assertion_status/terminate.h"
#include "abc_test/internal/test_reports/static_assertion.h"
_BEGIN_ABC_REPORTERS_NS
template<
	typename Assertion_Status
>
struct static_assertion_list_formatter_t
	: public list_formattable_t< reports::static_assertion_t<Assertion_Status>,
	combined_enum_assertion_fields_t, print_config_t>,
	public assertion_list_formatter_t<true, Assertion_Status>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const combined_enum_assertion_fields_t& _a_fid,
			const reports::static_assertion_t<Assertion_Status>& _a_element
		) const override;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_assertion_fields_t& _a_fid,
			const reports::static_assertion_t<Assertion_Status>& _a_element,
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
static_assertion_list_formatter_t<Assertion_Status>::check_data(
	const combined_enum_assertion_fields_t& _a_fid,
	const reports::static_assertion_t<Assertion_Status>& _a_element
) const
{
	return assertion_list_formatter_t<true, Assertion_Status>::check_data(_a_fid, _a_element);
}
template<
	typename Assertion_Status
>
__constexpr_imp
std::vector<std::string>
static_assertion_list_formatter_t<Assertion_Status>::get_data(
	const combined_enum_assertion_fields_t& _a_fid,
	const reports::static_assertion_t<Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	return assertion_list_formatter_t<true, Assertion_Status>::get_data(_a_fid, _a_element, _a_pc);
}
template<
	typename Assertion_Status
>
__constexpr_imp
std::string
static_assertion_list_formatter_t<Assertion_Status>::get_str_representation(
	const reports::assertion_t<true, Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	return construct_str_representation(_a_element, "Static assertion");
}
_END_ABC_REPORTERS_NS