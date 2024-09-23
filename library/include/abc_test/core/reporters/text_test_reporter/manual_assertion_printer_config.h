#pragma once
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/text_test_reporter/enum_manual_failure_data.h"
#include "abc_test/core/reporters/text_test_reporter/enum_test_report_data.h"
#include <vector>
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/reporters/text_test_reporter/colour_palette.h"
#include "abc_test/core/reporters/mid_execution_test_report/manual_failure.h"
#include <variant>

#include "abc_test/core/reporters/text_test_reporter/test_report_printer_config.h"

_BEGIN_ABC_REPORTERS_NS
using combined_enum_manual_assertion_t =
std::variant<enum_test_report_data_t, enum_manual_assertion_print_fields_t>;
using manual_assertion_element_t = std::pair<combined_enum_manual_assertion_t, enum_print_data_t>;
using manual_assertion_info_list_t = std::vector<manual_assertion_element_t>;
class manual_assertion_printer_config_t : public test_report_data_config_t
{
public:
	__constexpr
		std::vector<std::string>
		get_all_data(
			const manual_assertion_info_list_t& _a_enum,
			const colour_palette_t& _a_colours,
			const manual_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const;
	__constexpr
		virtual
		bool
		check_data(
			const combined_enum_manual_assertion_t _a_pair,
			const manual_assertion_t& _a_report
		) const;
	__constexpr
		virtual
		bool
		check_data(
			const manual_assertion_element_t& _a_pair,
			const manual_assertion_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_manual_assertion_t _a_enum,
			const colour_palette_t& _a_colours,
			const manual_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const;
private:
	__constexpr
		virtual
		std::vector<std::string>
		str_rep_data(
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
protected:
	__constexpr
		virtual
		bool
		check_contextual_data(
			const enum_manual_assertion_print_fields_t _a_enum,
			const manual_assertion_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_manual_assertion_print_fields_t _a_enum,
			const colour_palette_t& _a_colours,
			const manual_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const;
	__constexpr
		virtual
		bool
		check_source_code_representation(
			const manual_assertion_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		source_code_rep_data(
			const colour_palette_t& _a_colours,
			const manual_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	std::vector<std::string>
	manual_assertion_printer_config_t::get_all_data(
		const manual_assertion_info_list_t& _a_enums,
		const colour_palette_t& _a_colours,
		const manual_assertion_t& _a_report,
		const std::size_t _a_indent_size
	) const
{
	using namespace std;
	vector<string> _l_rv;
	for (const manual_assertion_element_t& _l_element : _a_enums)
	{
		if (check_data(_l_element, _a_report))
		{
			_l_rv.append_range(get_data(_l_element.first, _a_colours, _a_report, _a_indent_size));
		}
	}
	return _l_rv;
}
__constexpr_imp
	bool
	manual_assertion_printer_config_t::check_data(
		const combined_enum_manual_assertion_t _a_enum,
		const manual_assertion_t& _a_report
	) const
{
	using namespace std;
	if (const enum_test_report_data_t * _l_enum_ptr{ get_if< enum_test_report_data_t>(&_a_enum) })
	{
		const mid_execution_test_report_t* _l_ptr{ static_cast<const mid_execution_test_report_t*>(&_a_report) };
		if (_l_ptr == nullptr)
		{
			throw errors::unaccounted_for_nullptr(_l_ptr);
		}
		else
		{
			return test_report_data_config_t::check_contextual_data(*_l_enum_ptr, *_l_ptr);
		}
	}
	else if (const enum_manual_assertion_print_fields_t * _l_enum_ptr{ get_if< enum_manual_assertion_print_fields_t>(&_a_enum) })
	{
		return check_contextual_data(*_l_enum_ptr, _a_report);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_enum);
	}
}
__constexpr_imp
bool
	manual_assertion_printer_config_t::check_data(
	const manual_assertion_element_t& _a_pair,
	const manual_assertion_t& _a_report
) const
{
	if (always_print(_a_pair.second))
	{
		return true;
	}
	else if (dont_print(_a_pair.second))
	{
		return false;
	}
	else
	{
		return check_data(_a_pair.first, _a_report);
	}
}
__constexpr_imp
	std::vector<std::string>
	manual_assertion_printer_config_t::get_data(
		const combined_enum_manual_assertion_t _a_enum,
		const colour_palette_t& _a_colours,
		const manual_assertion_t& _a_report,
		const std::size_t _a_indent_size
	) const
{
	using namespace std;
	if (holds_alternative<enum_test_report_data_t>(_a_enum))
	{
		const enum_test_report_data_t _l_enum(get< enum_test_report_data_t>(_a_enum));
		const mid_execution_test_report_t* _l_ptr{ static_cast<const mid_execution_test_report_t*>(&_a_report) };
		if (_l_ptr == nullptr)
		{
			throw errors::unaccounted_for_nullptr(_l_ptr);
		}
		else
		{
			return test_report_data_config_t::get_data(_l_enum, _a_colours,*_l_ptr,_a_indent_size);
		}
	}
	else if (holds_alternative<enum_manual_assertion_print_fields_t>(_a_enum))
	{
		const enum_manual_assertion_print_fields_t _l_enum(get< enum_manual_assertion_print_fields_t>(_a_enum));
		return get_data(_l_enum, _a_colours, _a_report, _a_indent_size);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_enum);
	}
}
__constexpr
	std::vector<std::string>
	manual_assertion_printer_config_t::str_rep_data(
		const colour_palette_t& _a_colours,
		const mid_execution_test_report_t& _a_report,
		const std::size_t _a_indent_size
		) const noexcept
{
	return 
	{
		(_a_report.passed() ?
		_a_colours.style_pass("Manual Assertion passed.") :
		_a_colours.style_failure(
			fmt::format("Manual assertion failed{0}.",
				_a_report.early_termination() ? "function terminated early" : ""))
		)
	};
}
__constexpr_imp
	bool
	manual_assertion_printer_config_t::check_contextual_data(
		const enum_manual_assertion_print_fields_t _a_enum,
		const manual_assertion_t & _a_report
	) const
{
	using enum enum_manual_assertion_print_fields_t;
	switch (_a_enum)
	{
	case SOURCE_CODE_REPRESENTATION:
		return check_source_code_representation(_a_report);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
std::vector<std::string>
	manual_assertion_printer_config_t::get_data(
		const enum_manual_assertion_print_fields_t _a_enum,
		const colour_palette_t& _a_colours,
		const manual_assertion_t& _a_report,
		const std::size_t _a_indent_size
	) const
{
	using enum enum_manual_assertion_print_fields_t;
	switch (_a_enum)
	{
	case SOURCE_CODE_REPRESENTATION:
		return source_code_rep_data(_a_colours, _a_report, _a_indent_size);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
	bool
	manual_assertion_printer_config_t::check_source_code_representation(
		const manual_assertion_t& _a_report
	) const noexcept
{
	return true;
}
__constexpr_imp
	std::vector<std::string>
	manual_assertion_printer_config_t::source_code_rep_data(
		const colour_palette_t& _a_colours,
		const manual_assertion_t& _a_report,
		const std::size_t _a_indent_size
	) const noexcept
{
	using namespace utility::str;
	return
	{
		"Source code representation:",
		_a_colours.style_slight_highlight(indent(_a_indent_size,
			_a_report.source_code_representation()))
	};
}
_END_ABC_REPORTERS_NS