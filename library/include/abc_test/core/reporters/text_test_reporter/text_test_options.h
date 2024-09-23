#pragma once

#include "abc_test/utility/internal/macros.h"
#include <fmt/color.h>
#include "abc_test/core/reporters/text_test_reporter/enum_test_overview_data.h"
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
#include "abc_test/core/reporters/text_test_reporter/manual_assertion_printer_config.h"
#include <vector>

_BEGIN_ABC_REPORTERS_NS
using test_info_element_t = std::pair<enum_test_overview_data_t, enum_print_data_t>;
using test_info_list_t = std::vector<test_info_element_t>;
struct text_test_options_t
{
public:
	__constexpr
		text_test_options_t(
		) noexcept;
	template<
		typename R
	>
	__constexpr
		text_test_options_t(
			R&& _a_test_options
		) noexcept;
	__constexpr
		const test_info_list_t&
		test_info_list(
		) const noexcept;
	__constexpr
		const manual_assertion_info_list_t&
		manual_assertion_info_list(
		) const noexcept;
	__constexpr
		const user_defined_assertion_info_list_t&
		user_defined_assertion_info_list(
		) const noexcept;
	__constexpr
		const non_derived_exception_info_list_t&
		non_derived_exception_info_list(
		) const noexcept;
	__constexpr
		const derived_exception_info_list_t&
		derived_exception_info_list(
		) const noexcept;
	__constexpr
		std::size_t
		indent(
		) const noexcept;
private:
	test_info_list_t _m_test_info_to_print;
	manual_assertion_info_list_t _m_manual_assertion_info_to_print;
	user_defined_assertion_info_list_t _m_user_defined_info_list;
	non_derived_exception_info_list_t _m_non_derived_exception_info_list;
	derived_exception_info_list_t _m_derived_exception_info_list;
	std::size_t _m_indent = 2;
};
namespace
{
	__constexpr
		test_info_list_t
		default_test_info(
		) noexcept;
	__constexpr
		manual_assertion_info_list_t
		default_manual_assertion_info(
		) noexcept;
	__constexpr
		user_defined_assertion_info_list_t
		default_user_defined_info(
		) noexcept;
	__constexpr
		non_derived_exception_info_list_t
		non_derived_exception_info(
		) noexcept;
	__constexpr
		derived_exception_info_list_t
		derived_exception_info(
		) noexcept;
}

_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS

__constexpr_imp
text_test_options_t::text_test_options_t(
) noexcept
	: _m_test_info_to_print(default_test_info())
	, _m_manual_assertion_info_to_print(default_manual_assertion_info())
	, _m_user_defined_info_list(default_user_defined_info())
	, _m_non_derived_exception_info_list(non_derived_exception_info())
	, _m_derived_exception_info_list(derived_exception_info())
{

}
template<
	typename R
>
__constexpr_imp
	text_test_options_t::text_test_options_t(
		R&& _a_test_options
	) noexcept
{
	using namespace std;
	for (auto&& _l_test_option : _a_test_options)
	{

	}
}
__constexpr_imp
	const test_info_list_t&
	text_test_options_t::test_info_list(
	) const noexcept
{
	return _m_test_info_to_print;
}
__constexpr_imp
	const manual_assertion_info_list_t&
	text_test_options_t::manual_assertion_info_list(
	) const noexcept
{
	return _m_manual_assertion_info_to_print;
}
__constexpr_imp
const user_defined_assertion_info_list_t&
text_test_options_t::user_defined_assertion_info_list(
) const noexcept
{
	return _m_user_defined_info_list;
}
__constexpr_imp
const non_derived_exception_info_list_t&
text_test_options_t::non_derived_exception_info_list(
) const noexcept
{
	return _m_non_derived_exception_info_list;
}
__constexpr_imp
const derived_exception_info_list_t&
text_test_options_t::derived_exception_info_list(
) const noexcept
{
	return _m_derived_exception_info_list;
}
__constexpr_imp
	std::size_t
	text_test_options_t::indent(
	) const noexcept
{
	return _m_indent;
}
namespace
{
	__constexpr_imp
		test_info_list_t
		default_test_info(
		) noexcept
	{
		using enum enum_test_overview_data_t;
		using enum enum_print_data_t;
		return
		{
			{TITLE,PRINT},
			{NAME,PRINT},
			{LOCATION,PRINT},
			{TEST_PATH,PRINT},
			{SEED_USED_HEX,CONTEXTUAL_PRINT},
			{SEED_TO_USE_TO_RE_RUN_TEST_HEX,CONTEXTUAL_PRINT},
			{CONDENSED_ASSERTIONS_INFO,CONTEXTUAL_PRINT},
			{TOTAL_WARNINGS,CONTEXTUAL_PRINT},
			{TERMINATED_EARLY,CONTEXTUAL_PRINT},
		};
	}
	__constexpr_imp
		manual_assertion_info_list_t
		default_manual_assertion_info(
		) noexcept
	{
		using enum enum_manual_assertion_print_fields_t;
		using enum enum_test_report_data_t;
		using enum enum_print_data_t;
		return
		{
			{STR_REPRESENTATION,PRINT},
			{SOURCE_CODE_REPRESENTATION,PRINT},
			{SOURCE_LOCATION,PRINT},
			{LOG_INFOS,CONTEXTUAL_PRINT},
		};
	}
	__constexpr_imp
		user_defined_assertion_info_list_t
		default_user_defined_info(
		) noexcept
	{
		using enum enum_manual_assertion_print_fields_t;
		using enum enum_test_report_data_t;
		using enum enum_user_defined_assertion_print_fields_t;
		using enum enum_print_data_t;
		return
		{
			{STR_REPRESENTATION,PRINT},
			{SOURCE_CODE_REPRESENTATION,PRINT},
			{SOURCE_LOCATION,PRINT},
			{MATCHER_STRING,PRINT},
			{MATCHER_SOURCE_MAP,CONTEXTUAL_PRINT},
			{LOG_INFOS,CONTEXTUAL_PRINT},
		};
	}
	__constexpr_imp
		non_derived_exception_info_list_t
		non_derived_exception_info(
		) noexcept
	{
		using enum enum_test_report_data_t;
		using enum enum_non_derived_unhandled_exception_print_fields_t;
		using enum enum_print_data_t;
		return
		{
			{STR_REPRESENTATION,PRINT},
			{SOURCE_LOCATION,PRINT},
			{LOG_INFOS,CONTEXTUAL_PRINT},
		};
	}
	__constexpr_imp
		derived_exception_info_list_t
		derived_exception_info(
		) noexcept
	{
		using enum enum_test_report_data_t;
		using enum enum_non_derived_unhandled_exception_print_fields_t;
		using enum enum_print_data_t;
		using enum enum_derived_unhandled_exception_print_fields_t;
		return
		{
			{STR_REPRESENTATION,PRINT},
			{SOURCE_LOCATION,PRINT},
			{EXCEPTION_TYPE,PRINT},
			{EXCEPTION_ERROR_MSG,CONTEXTUAL_PRINT},
			{LOG_INFOS,CONTEXTUAL_PRINT},
		};
	}
}
_END_ABC_REPORTERS_NS