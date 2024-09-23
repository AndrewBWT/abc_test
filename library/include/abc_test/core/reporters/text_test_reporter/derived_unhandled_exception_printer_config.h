#pragma once
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/text_test_reporter/enum_derived_unhandled_exception_print_fields.h"
#include "abc_test/core/reporters/text_test_reporter/enum_test_report_data.h"
#include <vector>
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/reporters/text_test_reporter/colour_palette.h"
#include "abc_test/core/reporters/mid_execution_test_report/manual_failure.h"
#include <variant>

#include "abc_test/core/reporters/text_test_reporter/test_report_printer_config.h"
#include <abc_test/core/reporters/mid_execution_test_report/unhandled_exception.h>
#include "abc_test/core/reporters/text_test_reporter/non_derived_unhandled_exception_printer_config.h"

_BEGIN_ABC_REPORTERS_NS
using combined_derived_unhandled_exception_print_fields_t =
std::variant<combined_non_derived_unhandled_exception_print_fields_t, enum_derived_unhandled_exception_print_fields_t>;
using derived_exception_element_t = std::pair<combined_derived_unhandled_exception_print_fields_t, enum_print_data_t>;
using derived_exception_info_list_t = std::vector<derived_exception_element_t>;
class derived_unhandled_exception_printer_config_t : public 
	non_derived_unhandlded_exception_printer_config_t
{
public:
	__constexpr
		std::vector<std::string>
		get_all_data(
			const derived_exception_info_list_t& _a_enum,
			const colour_palette_t& _a_colours,
			const unhandled_exception_t& _a_report,
			const std::size_t _a_indent_size
		) const;
	__constexpr
		virtual
		bool
		check_data(
			const derived_exception_element_t& _a_pair,
			const unhandled_exception_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_derived_unhandled_exception_print_fields_t _a_enum,
			const colour_palette_t& _a_colours,
			const unhandled_exception_t& _a_report,
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
		) const noexcept override;
protected:
	__constexpr
		virtual
		bool
		check_contextual_data(
			const enum_derived_unhandled_exception_print_fields_t _a_enum,
			const unhandled_exception_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_derived_unhandled_exception_print_fields_t _a_enum,
			const colour_palette_t& _a_colours,
			const unhandled_exception_t& _a_report,
			const std::size_t _a_indent_size
		) const;
	__constexpr
		virtual
		bool
		check_exception_type(
			const unhandled_exception_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		bool
		check_exception_error_msg(
			const unhandled_exception_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		exception_type_str(
			const colour_palette_t& _a_colours,
			const unhandled_exception_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		exception_error_str(
			const colour_palette_t& _a_colours,
			const unhandled_exception_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
std::vector<std::string>
derived_unhandled_exception_printer_config_t::get_all_data(
	const derived_exception_info_list_t& _a_enums,
	const colour_palette_t& _a_colours,
	const unhandled_exception_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using namespace std;
	vector<string> _l_rv;
	for (const derived_exception_element_t& _l_element : _a_enums)
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
derived_unhandled_exception_printer_config_t::check_data(
	const derived_exception_element_t& _a_pair,
	const unhandled_exception_t& _a_report
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
		using namespace std;
		if (holds_alternative<combined_non_derived_unhandled_exception_print_fields_t>(_a_pair.first))
		{
			const combined_non_derived_unhandled_exception_print_fields_t _l_enum(get< combined_non_derived_unhandled_exception_print_fields_t>(_a_pair.first));
			const unhandled_exception_not_derived_from_std_exception_t* _l_ptr{ static_cast<const unhandled_exception_not_derived_from_std_exception_t*>(&_a_report) };
			if (_l_ptr == nullptr)
			{
				throw errors::unaccounted_for_nullptr(_l_ptr);
			}
			else
			{
				return non_derived_unhandlded_exception_printer_config_t::check_data(_l_enum, *_l_ptr);
			}
		}
		else if (holds_alternative<enum_derived_unhandled_exception_print_fields_t>(_a_pair.first))
		{
			const enum_derived_unhandled_exception_print_fields_t _l_enum(get<enum_derived_unhandled_exception_print_fields_t>(_a_pair.first));
			return check_contextual_data(_l_enum, _a_report);
		}
		else
		{
			throw errors::unaccounted_for_variant_exception(_a_pair.first);
		}
	}
}
__constexpr_imp
std::vector<std::string>
derived_unhandled_exception_printer_config_t::get_data(
	const combined_derived_unhandled_exception_print_fields_t _a_enum,
	const colour_palette_t& _a_colours,
	const unhandled_exception_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using namespace std;
	if (holds_alternative<combined_non_derived_unhandled_exception_print_fields_t>(_a_enum))
	{
		const combined_non_derived_unhandled_exception_print_fields_t _l_enum(get< combined_non_derived_unhandled_exception_print_fields_t>(_a_enum));
		const unhandled_exception_not_derived_from_std_exception_t* _l_ptr{ static_cast<const unhandled_exception_not_derived_from_std_exception_t*>(&_a_report) };
		if (_l_ptr == nullptr)
		{
			throw errors::unaccounted_for_nullptr(_l_ptr);
		}
		else
		{
			return non_derived_unhandlded_exception_printer_config_t::get_data(_l_enum, _a_colours, *_l_ptr, _a_indent_size);
		}
	}
	else if (holds_alternative<enum_derived_unhandled_exception_print_fields_t>(_a_enum))
	{
		const enum_derived_unhandled_exception_print_fields_t _l_enum(get< enum_derived_unhandled_exception_print_fields_t>(_a_enum));
		return get_data(_l_enum, _a_colours, _a_report, _a_indent_size);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_enum);
	}
}
__constexpr
std::vector<std::string>
derived_unhandled_exception_printer_config_t::str_rep_data(
	const colour_palette_t& _a_colours,
	const mid_execution_test_report_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	using namespace std;
	const string _l_str{ fmt::format("Unexpected exception type.{0}",
		" Terminating function.") };
	return
	{
		(_a_report.passed() ?
		_a_colours.style_pass(_l_str) :
		_a_colours.style_failure(_l_str))
	};
}
__constexpr_imp
bool
derived_unhandled_exception_printer_config_t::check_contextual_data(
	const enum_derived_unhandled_exception_print_fields_t _a_enum,
	const unhandled_exception_t& _a_report
) const
{
	using enum enum_derived_unhandled_exception_print_fields_t;
	switch (_a_enum)
	{
	case EXCEPTION_TYPE:
		return check_exception_type(_a_report);
	case EXCEPTION_ERROR_MSG:
		return check_exception_error_msg(_a_report);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
std::vector<std::string>
derived_unhandled_exception_printer_config_t::get_data(
	const enum_derived_unhandled_exception_print_fields_t _a_enum,
	const colour_palette_t& _a_colours,
	const unhandled_exception_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using enum enum_derived_unhandled_exception_print_fields_t;
	switch (_a_enum)
	{
	case EXCEPTION_TYPE:
		return exception_type_str(_a_colours, _a_report, _a_indent_size);
	case EXCEPTION_ERROR_MSG:
		return exception_error_str(_a_colours, _a_report, _a_indent_size);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
bool
derived_unhandled_exception_printer_config_t::check_exception_type(
	const unhandled_exception_t& _a_report
) const noexcept
{
	return true;
}
__constexpr_imp
bool
derived_unhandled_exception_printer_config_t::check_exception_error_msg(
	const unhandled_exception_t& _a_report
) const noexcept
{
	return true;
}
__constexpr_imp
std::vector<std::string>
derived_unhandled_exception_printer_config_t::exception_type_str(
	const colour_palette_t& _a_colours,
	const unhandled_exception_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	using namespace utility::str;
	return
	{
		"Exception's type:",
		_a_colours.style_slight_highlight(indent(_a_indent_size,
			_a_report.exception_type()))
	};
}
__constexpr_imp
std::vector<std::string>
derived_unhandled_exception_printer_config_t::exception_error_str(
	const colour_palette_t& _a_colours,
	const unhandled_exception_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	using namespace utility::str;
	return
	{
		"Exception's what() function return value:",
		_a_colours.style_slight_highlight(indent(_a_indent_size,
			_a_report.exception_error_msg()))
	};
}
_END_ABC_REPORTERS_NS