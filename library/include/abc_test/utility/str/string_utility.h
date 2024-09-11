#pragma once

#include <string>
#include <vector>
#include "abc_test/utility/internal/macros.h"

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
	_END_ABC_UTILITY_STR_NS