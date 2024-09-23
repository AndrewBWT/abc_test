#pragma once
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/text_test_reporter/enum_manual_failure_data.h"
#include "abc_test/core/reporters/text_test_reporter/enum_user_defined_assertion_print_fields.h"
#include <vector>
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/reporters/text_test_reporter/colour_palette.h"
#include "abc_test/core/reporters/mid_execution_test_report/test_assertion_result.h"
#include <variant>

#include "abc_test/core/reporters/text_test_reporter/manual_assertion_printer_config.h"

_BEGIN_ABC_REPORTERS_NS
using combined_enum_user_defined_assertion_t =
std::variant<combined_enum_manual_assertion_t, enum_user_defined_assertion_print_fields_t>;
using user_defined_assertion_element_t = std::pair<combined_enum_user_defined_assertion_t, enum_print_data_t>;
using user_defined_assertion_info_list_t = std::vector<user_defined_assertion_element_t>;
class user_defined_printer_config_t : public manual_assertion_printer_config_t
{
public:
	__constexpr
		std::vector<std::string>
		get_all_data(
			const user_defined_assertion_info_list_t& _a_enum,
			const colour_palette_t& _a_colours,
			const user_defined_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const;
	__constexpr
		virtual
		bool
		check_data(
			const user_defined_assertion_element_t& _a_pair,
			const user_defined_assertion_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const combined_enum_user_defined_assertion_t _a_enum,
			const colour_palette_t& _a_colours,
			const user_defined_assertion_t& _a_report,
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
			const enum_user_defined_assertion_print_fields_t _a_enum,
			const user_defined_assertion_t& _a_report
		) const;
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const enum_user_defined_assertion_print_fields_t _a_enum,
			const colour_palette_t& _a_colours,
			const user_defined_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const;
	__constexpr
		virtual
		bool
		check_matcher_source_map(
			const user_defined_assertion_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		bool
		check_matcher_string(
			const user_defined_assertion_t& _a_report
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		matcher_source_map_str(
			const colour_palette_t& _a_colours,
			const user_defined_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
	__constexpr
		virtual
		std::vector<std::string>
		matcher_string_str(
			const colour_palette_t& _a_colours,
			const user_defined_assertion_t& _a_report,
			const std::size_t _a_indent_size
		) const noexcept;
};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
std::vector<std::string>
user_defined_printer_config_t::get_all_data(
	const user_defined_assertion_info_list_t& _a_enums,
	const colour_palette_t& _a_colours,
	const user_defined_assertion_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using namespace std;
	vector<string> _l_rv;
	for (const user_defined_assertion_element_t& _l_element : _a_enums)
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
user_defined_printer_config_t::check_data(
	const user_defined_assertion_element_t& _a_pair,
	const user_defined_assertion_t& _a_report
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
		if (holds_alternative<combined_enum_manual_assertion_t>(_a_pair.first))
		{
			const combined_enum_manual_assertion_t _l_enum(get< combined_enum_manual_assertion_t>(_a_pair.first));
			const manual_assertion_t* _l_ptr{ static_cast<const manual_assertion_t*>(&_a_report) };
			if (_l_ptr == nullptr)
			{
				throw errors::unaccounted_for_nullptr(_l_ptr);
			}
			else
			{
				return manual_assertion_printer_config_t::check_data(_l_enum, *_l_ptr);
			}
		}
		else if (holds_alternative<enum_user_defined_assertion_print_fields_t>(_a_pair.first))
		{
			const enum_user_defined_assertion_print_fields_t _l_enum(get< enum_user_defined_assertion_print_fields_t>(_a_pair.first));
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
user_defined_printer_config_t::get_data(
	const combined_enum_user_defined_assertion_t _a_enum,
	const colour_palette_t& _a_colours,
	const user_defined_assertion_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using namespace std;
	if (holds_alternative<combined_enum_manual_assertion_t>(_a_enum))
	{
		const combined_enum_manual_assertion_t _l_enum(get< combined_enum_manual_assertion_t>(_a_enum));
		const manual_assertion_t* _l_ptr{ static_cast<const manual_assertion_t*>(&_a_report) };
		if (_l_ptr == nullptr)
		{
			throw errors::unaccounted_for_nullptr(_l_ptr);
		}
		else
		{
			return manual_assertion_printer_config_t::get_data(_l_enum, _a_colours, *_l_ptr, _a_indent_size);
		}
	}
	else if (holds_alternative<enum_user_defined_assertion_print_fields_t>(_a_enum))
	{
		const enum_user_defined_assertion_print_fields_t _l_enum(get< enum_user_defined_assertion_print_fields_t>(_a_enum));
		return get_data(_l_enum, _a_colours, _a_report, _a_indent_size);
	}
	else
	{
		throw errors::unaccounted_for_variant_exception(_a_enum);
	}
}
__constexpr
std::vector<std::string>
user_defined_printer_config_t::str_rep_data(
	const colour_palette_t& _a_colours,
	const mid_execution_test_report_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	return
	{
		(_a_report.passed() ?
		_a_colours.style_pass("Assertion passed.") :
		_a_colours.style_failure(
			fmt::format("Assertion failed{0}.",
				_a_report.early_termination() ? ", function terminated early" : ""))
		)
	};
}
__constexpr_imp
bool
user_defined_printer_config_t::check_contextual_data(
	const enum_user_defined_assertion_print_fields_t _a_enum,
	const user_defined_assertion_t& _a_report
) const
{
	using enum enum_user_defined_assertion_print_fields_t;
	switch (_a_enum)
	{
	case MATCHER_SOURCE_MAP:
		return check_matcher_source_map(_a_report);
	case MATCHER_STRING:
		return check_matcher_string(_a_report);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
std::vector<std::string>
user_defined_printer_config_t::get_data(
	const enum_user_defined_assertion_print_fields_t _a_enum,
	const colour_palette_t& _a_colours,
	const user_defined_assertion_t& _a_report,
	const std::size_t _a_indent_size
) const
{
	using enum enum_user_defined_assertion_print_fields_t;
	switch (_a_enum)
	{
	case MATCHER_SOURCE_MAP:
		return matcher_source_map_str(_a_colours,_a_report, _a_indent_size);
	case MATCHER_STRING:
		return matcher_string_str(_a_colours, _a_report, _a_indent_size);
	default:
		throw errors::unaccounted_for_enum_exception(_a_enum);
	}
}
__constexpr_imp
bool
user_defined_printer_config_t::check_matcher_source_map(
	const user_defined_assertion_t& _a_report
) const noexcept
{
	return _a_report.matcher_source_map().size() > 0;
}
__constexpr_imp
bool
user_defined_printer_config_t::check_matcher_string(
	const user_defined_assertion_t& _a_report
) const noexcept
{
	return true;
}
__constexpr_imp
std::vector<std::string>
user_defined_printer_config_t::matcher_source_map_str(
	const colour_palette_t& _a_colours,
	const user_defined_assertion_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	using namespace utility::str;
	using namespace std;
	vector<string> _l_rv{ "Additional relevant source code representations:" };
	for (const std::pair<std::string, std::vector<std::string>>& _l_pair : _a_report.matcher_source_map().map())
	{
		_l_rv.push_back(indent(_a_indent_size,"Location:"));
		_l_rv.push_back(indent(_a_indent_size * 2, 
			_a_colours.style_highlight(_l_pair.first)));
		if (_l_pair.second.size() > 0)
		{
			_l_rv.push_back(indent(_a_indent_size, _l_pair.second.size() == 1 ? "Source:" : "Sources:"));
			for (const string_view _l_str : _l_pair.second)
			{
				_l_rv.push_back(indent(_a_indent_size * 2, 
					_a_colours.style_slight_highlight(_l_str)));
			}
		}
	}
	return _l_rv;
}
__constexpr_imp
std::vector<std::string>
user_defined_printer_config_t::matcher_string_str(
	const colour_palette_t& _a_colours,
	const user_defined_assertion_t& _a_report,
	const std::size_t _a_indent_size
) const noexcept
{
	using namespace utility::str;
	return
	{
		"Matcher output:",
		_a_colours.style_slight_highlight(indent(_a_indent_size,
			_a_report.matcher_str().has_value() ? _a_report.matcher_str().value() :
			"<no matcher output>"))
	};
}
_END_ABC_REPORTERS_NS