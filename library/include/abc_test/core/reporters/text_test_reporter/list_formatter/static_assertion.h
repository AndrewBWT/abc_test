#pragma once
#include "abc_test/core/reporters/text_test_reporter/list_formatter/generic_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass_or_terminate.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/terminate.h"
_BEGIN_ABC_REPORTERS_NS
template<
	typename Assertion_Status
>
struct static_assertion_list_formatter_t
	: public list_formattable_t< reports::static_assertion_t<Assertion_Status>,
	combined_enum_generic_assertion_fields_t, print_config_t>,
	public generic_assertion_list_formatter_t<true, Assertion_Status>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const combined_enum_generic_assertion_fields_t& _a_fid,
			const reports::static_assertion_t<Assertion_Status>& _a_element
		) const override;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_generic_assertion_fields_t& _a_fid,
			const reports::static_assertion_t<Assertion_Status>& _a_element,
			const print_config_t& _a_pc
		) const override;
protected:
	__constexpr
		virtual
		std::string
		get_str_representation(
			const reports::generic_assertion_t<true, Assertion_Status>& _a_element,
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
	const combined_enum_generic_assertion_fields_t& _a_fid,
	const reports::static_assertion_t<Assertion_Status>& _a_element
) const
{
	return generic_assertion_list_formatter_t<true, Assertion_Status>::check_data(_a_fid, _a_element);
}
template<
	typename Assertion_Status
>
__constexpr_imp
std::vector<std::string>
static_assertion_list_formatter_t<Assertion_Status>::get_data(
	const combined_enum_generic_assertion_fields_t& _a_fid,
	const reports::static_assertion_t<Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	return generic_assertion_list_formatter_t<true, Assertion_Status>::get_data(_a_fid, _a_element, _a_pc);
}
template<
	typename Assertion_Status
>
__constexpr_imp
std::string
static_assertion_list_formatter_t<Assertion_Status>::get_str_representation(
	const reports::generic_assertion_t<true, Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	using namespace std;
	using namespace reports;
	return fmt::format("Static assertion {0}.{1}",
		_a_element.get_pass_status() ? "passed" : "failed",
		(same_as<Assertion_Status, terminate_t> ?
			"Assertion terminated function." : ""));
}
_END_ABC_REPORTERS_NS