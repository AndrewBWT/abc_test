#pragma once
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/text_test_reporter/enum_non_derived_unhandled_exception_print_fields.h"
#include "abc_test/core/reporters/text_test_reporter/enum_test_report_data.h"
#include <vector>
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/reporters/text_test_reporter/colour_palette.h"
#include "abc_test/core/reporters/mid_execution_test_report/manual_failure.h"
#include <variant>

#include "abc_test/core/reporters/text_test_reporter/test_report_printer_config.h"
#include <abc_test/core/reporters/mid_execution_test_report/unhandled_exception_not_derived_from_std_exception.h>

_BEGIN_ABC_REPORTERS_NS
using combined_non_derived_unhandled_exception_print_fields_t =
std::variant<enum_test_report_data_t, enum_non_derived_unhandled_exception_print_fields_t>;
using non_derived_exception_element_t = std::pair<combined_non_derived_unhandled_exception_print_fields_t, enum_print_data_t>;
using non_derived_exception_info_list_t = std::vector<non_derived_exception_element_t>;
class non_derived_unhandlded_exception_printer_config_t : public test_report_data_config_t
{
public:
	__constexpr
		std::vector<std::string>
		get_all_data(
			const non_derived_exception_info_list_t& _a_enum,
			const colour_palette_t& _a_colours,
			const unhandled_exception_not_derived_from_std_exception_t& _a_report,
			const std::size_t _a_indent_size
		) const;
	__constexpr
		virtual
		bool
		check_data(
			const combined_non_derived_unhandled_exception_print_fields_t _a_enum,
			const unhandled_exception_not_derived_from_std_exception_t& _a_report
		) const;
	__constexpr
		virtual
		bool
		check_data(
			const non_derived_exception_element_t& _a_pair,
			const unhandled_exception_not_derived_from_std_exception_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_non_derived_unhandled_exception_print_fields_t _a_enum,
			const colour_palette_t& _a_colours,
			const unhandled_exception_not_derived_from_std_exception_t& _a_report,
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
	__constexpr
		virtual
		std::vector<std::string>
		source_location_data(
			const colour_palette_t& _a_colours,
			const mid_execution_test_report_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
protected:
	__constexpr
		virtual
		bool
		check_contextual_data(
			const enum_non_derived_unhandled_exception_print_fields_t _a_enum,
			const unhandled_exception_not_derived_from_std_exception_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_non_derived_unhandled_exception_print_fields_t _a_enum,
			const colour_palette_t& _a_colours,
			const unhandled_exception_not_derived_from_std_exception_t& _a_report,
			const std::size_t _a_indent_size
		) const;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
std::vector<std::string>
non_derived_unhandlded_exception_printer_config_t::get_all_data(
	const non_derived_exception_info_list_t& _a_enums,
	const colour_palette_t& _a_colours,
	const unhandled_exception_not_derived_from_std_exception_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using namespace std;
	vector<string> _l_rv;
	for (const non_derived_exception_element_t& _l_element : _a_enums)
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
	non_derived_unhandlded_exception_printer_config_t::check_data(
		const combined_non_derived_unhandled_exception_print_fields_t _a_enum,
		const unhandled_exception_not_derived_from_std_exception_t& _a_report
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
			return test_report_data_config_t::check_contextual_data(_l_enum, *_l_ptr);
		}
	}
	else if (holds_alternative<enum_non_derived_unhandled_exception_print_fields_t>(_a_enum))
	{
		const enum_non_derived_unhandled_exception_print_fields_t _l_enum(get<enum_non_derived_unhandled_exception_print_fields_t>(_a_enum));
		return check_contextual_data(_l_enum, _a_report);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_enum);
	}
}
__constexpr_imp
bool
non_derived_unhandlded_exception_printer_config_t::check_data(
	const non_derived_exception_element_t& _a_pair,
	const unhandled_exception_not_derived_from_std_exception_t& _a_report
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
non_derived_unhandlded_exception_printer_config_t::get_data(
	const combined_non_derived_unhandled_exception_print_fields_t _a_enum,
	const colour_palette_t& _a_colours,
	const unhandled_exception_not_derived_from_std_exception_t& _a_report,
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
			return test_report_data_config_t::get_data(_l_enum, _a_colours, *_l_ptr, _a_indent_size);
		}
	}
	else if (holds_alternative<enum_non_derived_unhandled_exception_print_fields_t>(_a_enum))
	{
		const enum_non_derived_unhandled_exception_print_fields_t _l_enum(get< enum_non_derived_unhandled_exception_print_fields_t>(_a_enum));
		return get_data(_l_enum, _a_colours, _a_report, _a_indent_size);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_enum);
	}
}
__constexpr
std::vector<std::string>
non_derived_unhandlded_exception_printer_config_t::str_rep_data(
	const colour_palette_t& _a_colours,
	const mid_execution_test_report_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	using namespace std;
	const string _l_str{ fmt::format("Unexpected exception type, not derived from {0}, "
				"encountered.{1}",
		typeid(std::exception).name()," Terminating function.") };
	return
	{
		(_a_report.passed() ?
		_a_colours.style_pass(_l_str) :
		_a_colours.style_failure(_l_str))
	};
}
__constexpr_imp
std::vector<std::string>
non_derived_unhandlded_exception_printer_config_t::source_location_data(
	const colour_palette_t& _a_colours,
	const mid_execution_test_report_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	using namespace utility::str;
	return
	{
		"We do not know where the exception was thrown from. "
		"However, the last source location logged by the testing library before this exception is:",
		_a_colours.style_highlight(indent(_a_indent_size,
			location_string(_a_report.source_location())))
	};
}
__constexpr_imp
bool
non_derived_unhandlded_exception_printer_config_t::check_contextual_data(
	const enum_non_derived_unhandled_exception_print_fields_t _a_enum,
	const unhandled_exception_not_derived_from_std_exception_t& _a_report
) const
{
	using enum enum_non_derived_unhandled_exception_print_fields_t;

	switch (_a_enum)
	{
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
std::vector<std::string>
non_derived_unhandlded_exception_printer_config_t::get_data(
	const enum_non_derived_unhandled_exception_print_fields_t _a_enum,
	const colour_palette_t& _a_colours,
	const unhandled_exception_not_derived_from_std_exception_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using enum enum_non_derived_unhandled_exception_print_fields_t;
	switch (_a_enum)
	{
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
_END_ABC_REPORTERS_NS