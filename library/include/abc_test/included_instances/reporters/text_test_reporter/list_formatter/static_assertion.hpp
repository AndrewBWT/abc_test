#pragma once
#include "abc_test/core/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/core/test_reports/assertion_status/terminate.hpp"
#include "abc_test/core/test_reports/static_assertion.hpp"
_BEGIN_ABC_REPORTERS_NS
template<
	typename Assertion_Status
>
struct static_assertion_list_formatter_t
	: public list_formattable_t< reports::static_assertion_t<Assertion_Status>,
	combined_enum_static_assertion_fields_t, print_config_t>,
	public assertion_list_formatter_t<true, Assertion_Status>
{
public:
	__constexpr
		virtual
		bool
		check_data(
			const combined_enum_static_assertion_fields_t& _a_fid,
			const reports::static_assertion_t<Assertion_Status>& _a_element
		) const override;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_static_assertion_fields_t& _a_fid,
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
	const combined_enum_static_assertion_fields_t& _a_fid,
	const reports::static_assertion_t<Assertion_Status>& _a_element
) const
{
	using namespace std;
	if (auto _l_ptr{ get_if< enum_static_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		using enum enum_static_assertion_fields_t;
		switch (*_l_ptr)
		{
		case TEST_ANNOTATION:
			return _a_element.annotation().has_value();
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else if (auto _l_ptr{ get_if< combined_enum_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return assertion_list_formatter_t<true, Assertion_Status>::check_data(*_l_ptr, _a_element);
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
static_assertion_list_formatter_t<Assertion_Status>::get_data(
	const combined_enum_static_assertion_fields_t& _a_fid,
	const reports::static_assertion_t<Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{

	using namespace std;
	if (auto _l_ptr{ get_if< enum_static_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		using enum enum_static_assertion_fields_t;
		switch (*_l_ptr)
		{
		case TEST_ANNOTATION:
			return {
				_a_pc.colon(_a_pc.static_test_annotation_str()),
				_a_pc.indent(_a_pc.message_str(_a_element.annotation()))
			};
		default:
			throw errors::unaccounted_for_enum_exception(*_l_ptr);
		}
	}
	else if (auto _l_ptr{ get_if< combined_enum_assertion_fields_t>(&_a_fid) };
		_l_ptr != nullptr)
	{
		return assertion_list_formatter_t<true, Assertion_Status>::get_data(*_l_ptr, _a_element, _a_pc);
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
static_assertion_list_formatter_t<Assertion_Status>::get_str_representation(
	const reports::assertion_t<true, Assertion_Status>& _a_element,
	const print_config_t& _a_pc
) const
{
	return construct_str_representation(_a_element, "Static assertion");
}
_END_ABC_REPORTERS_NS