#pragma once
#include "abc_test/core/reporters/text_test_reporter/list_formatter/generic_manual_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/matcher_based_assertion_block.h"
#include "abc_test/core/reporters/text_test_reporter/enum_fields/matcher_based_assertion_block.h"
_BEGIN_ABC_REPORTERS_NS
template<
	typename Assertion_Status
>
struct matcher_based_assertion_block_list_formatter_t
	: public list_formattable_t< reports::matcher_based_assertion_block_t<Assertion_Status>,
	combined_enum_matcher_based_assertion_block_fields_t, print_config_t>,
	public generic_matcher_based_assertion_list_formatter_t<false, Assertion_Status>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
			const reports::matcher_based_assertion_block_t<Assertion_Status>& _a_element
		) const override;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
			const reports::matcher_based_assertion_block_t<Assertion_Status>& _a_element,
			const print_config_t& _a_pc
		) const override;
protected:
	__constexpr
		virtual
		std::string
		get_str_representation(
			const reports::generic_assertion_t<false, Assertion_Status>& _a_element,
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
matcher_based_assertion_block_list_formatter_t<Assertion_Status>::check_data(
	const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
	const reports::matcher_based_assertion_block_t<Assertion_Status>& _a_element
) const
{
	using namespace std;
	if (auto _l_ptr{ get_if< enum_matcher_based_assertion_block_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		switch (*_l_ptr)
		{
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else if (auto _l_ptr{ get_if< combined_enum_generic_matcher_based_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return generic_matcher_based_assertion_list_formatter_t<false, Assertion_Status>::check_data(*_l_ptr, _a_element);
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
matcher_based_assertion_block_list_formatter_t<Assertion_Status>::get_data(
	const combined_enum_matcher_based_assertion_block_fields_t& _a_fid,
	const reports::matcher_based_assertion_block_t<Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	using namespace std;
	if (auto _l_ptr{ get_if< enum_matcher_based_assertion_block_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		switch (*_l_ptr)
		{
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else if (auto _l_ptr{ get_if< combined_enum_generic_matcher_based_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return generic_matcher_based_assertion_list_formatter_t<false, Assertion_Status>::get_data(*_l_ptr, _a_element, _a_pc);
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
matcher_based_assertion_block_list_formatter_t<Assertion_Status>::get_str_representation(
	const reports::generic_assertion_t<false, Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	using namespace std;
	using namespace reports;
	return fmt::format("Matcher-based assertion block {0}.{1}",
		_a_element.get_pass_status() ? "passed" : "failed",
		(not _a_element.get_pass_status() &&
			same_as<Assertion_Status, pass_or_terminate_t>) ?
		" Assertion terminated function." : "");
}
_END_ABC_REPORTERS_NS