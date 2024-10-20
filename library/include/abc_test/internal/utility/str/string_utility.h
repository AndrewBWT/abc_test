#pragma once

#include <string>
#include <vector>
#include "abc_test/internal/utility/internal/macros.h"
#include <fmt/base.h>

_BEGIN_ABC_UTILITY_STR_NS
/*!
* Appends together a set of strings into a single output string.
*/
__constexpr
	std::string
	create_string(
		const std::vector<std::string_view>& _a_strs
	) noexcept;
/*!
* Splits a string using the given delimiter
*/
__constexpr
	std::vector<std::string>
	split_string(
		const std::string_view _a_str,
		const std::string_view delimiter
	) noexcept;
__no_constexpr
	std::string
	location_string(
		const std::source_location& _a_sl
	) noexcept;
__constexpr
	std::string
	quote(
		const std::string_view _a_str
	) noexcept;
__constexpr
	std::string
	indent(
		const std::size_t _a_size,
		const std::string_view _a_str
	) noexcept;
__constexpr
const std::string_view
return_str_if_next_index_in_bound(
	const std::size_t _a_index_to_check,
	const std::size_t _a_max_index,
	const std::string_view _a_str
) noexcept;
_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
__constexpr_imp
	std::string
	create_string(
		const std::vector<std::string_view>& _a_strs
	) noexcept
{
	using namespace std;
	string _l_rv{};
	for (auto&& _l_s : _a_strs)
	{
		_l_rv.append(_l_s);
	}
	return _l_rv;
}
__constexpr_imp
	std::vector<std::string>
	split_string(
		const std::string_view _a_str,
		const std::string_view _a_delimiter
	) noexcept
{
	using namespace std;
	if (_a_str == "")
	{
		return {};
	}
	size_t pos_start = 0, pos_end, delim_len = _a_delimiter.size();
	string token;
	vector<string> res;

	while ((pos_end = _a_str.find(_a_delimiter, pos_start)) != string::npos) {
		token = _a_str.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}
	res.push_back(string(_a_str.substr(pos_start)));
	return res;
}
__no_constexpr_imp
	std::string
	location_string(
		const std::source_location& _a_sl
	) noexcept
{
	using namespace std;
	//A formalised way of showing a location
	return string{ fmt::format(
		"{0}:{1}",
		_a_sl.file_name(),
		_a_sl.line()
	) };
}
__constexpr_imp
	std::string
	quote(
		const std::string_view _a_str
	) noexcept
{
	using namespace std;
	return string("\"").append(_a_str).append("\"");
}
__constexpr_imp
	std::string
	indent(
		const std::size_t _a_size,
		const std::string_view _a_str
	) noexcept
{
	using namespace std;
	return string(_a_size, ' ').append(_a_str);
}
__constexpr_imp
const std::string_view
return_str_if_next_index_in_bound(
	const std::size_t _a_index_to_check,
	const std::size_t _a_max_index,
	const std::string_view _a_str
) noexcept
{
	return _a_index_to_check + 1 < _a_max_index ? _a_str : "";
}
_END_ABC_UTILITY_STR_NS