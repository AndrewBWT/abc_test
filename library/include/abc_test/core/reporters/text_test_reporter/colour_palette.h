#pragma once

#include "abc_test/utility/internal/macros.h"
#include <fmt/color.h>

_BEGIN_ABC_REPORTERS_NS
struct colour_palette_t
{
public:
	__constexpr
		colour_palette_t(
		) noexcept;
	//! The failure style used when highlighting information in text output.
	fmt::text_style _m_failure_text_style;
	//! The style used when highlighting information in text output.
	fmt::text_style _m_highlighted_info_style;
	//! The (slight) style used when highlighting information in text output.
	fmt::text_style _m_slight_highlight_style;
	//! The style used when highlighting good information in the text output
	fmt::text_style _m_pass_text_style;
	__constexpr
		std::string
		style_pass(
			const std::string_view _a_str
		) const noexcept;
	__constexpr
		std::string
		style_failure(
			const std::string_view _a_str
		) const noexcept;
	__constexpr
		std::string
		style_highlight(
			const std::string_view _a_str
		) const noexcept;
	__constexpr
		std::string
		style_slight_highlight(
			const std::string_view _a_str
		) const noexcept;
private:
	bool _m_colours_enabled;
	__constexpr
		std::string
		style(
			const std::string_view _a_str,
			const fmt::text_style& _a_style
		) const noexcept;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS

__constexpr_imp
	colour_palette_t::colour_palette_t(
	) noexcept
	: _m_colours_enabled(true)
	, _m_pass_text_style(fmt::fg(fmt::color::green))
	, _m_failure_text_style(fmt::fg(fmt::color::red))
	, _m_highlighted_info_style(fmt::fg(fmt::color::teal))
	, _m_slight_highlight_style(fmt::fg(fmt::color::gray))

{
}
__constexpr_imp
	std::string
	colour_palette_t::style_pass(
		const std::string_view _a_str
	) const noexcept
{
	return style(_a_str, _m_pass_text_style);
}
__constexpr_imp
	std::string
	colour_palette_t::style_failure(
		const std::string_view _a_str
	) const noexcept
{
	return style(_a_str, _m_failure_text_style);
}
__constexpr_imp
	std::string
	colour_palette_t::style_highlight(
		const std::string_view _a_str
	) const noexcept
{
	return style(_a_str, _m_highlighted_info_style);
}
__constexpr_imp
	std::string
	colour_palette_t::style_slight_highlight(
		const std::string_view _a_str
	) const noexcept
{
	return style(_a_str, _m_slight_highlight_style);
}
__constexpr_imp
	std::string
	colour_palette_t::style(
		const std::string_view _a_str,
		const fmt::text_style& _a_style
	) const noexcept
{
	return _m_colours_enabled ?
		fmt::format("{0}", fmt::styled(_a_str, _a_style)) :
		fmt::format("{0}", _a_str);
}
_END_ABC_REPORTERS_NS