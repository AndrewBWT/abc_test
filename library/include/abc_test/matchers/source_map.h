#pragma once

#include <map>

#include <fmt/std.h>
#include <source_location>

#include "abc_test/utility/internal/macros.h"

#include "abc_test/utility/str/string_utility.h"

_BEGIN_ABC_NS

class matcher_source_map_t
{
public:
	__constexpr
		void
		insert(
			const std::optional<std::source_location>& _a_source_location,
			const std::optional<std::string>& _a_str
		) noexcept;
	__constexpr
		const std::map<std::string, std::vector<std::string>>&
		map(
		) const noexcept;
	__constexpr
		std::size_t
		size(
		) const noexcept;
private:
	std::map<std::string, std::vector<std::string>> _m_internal_map;
};

_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	void
	matcher_source_map_t::insert(
		const std::optional<std::source_location>& _a_source_location,
		const std::optional<std::string>& _a_str
	) noexcept
{
	using namespace std;
	if (_a_source_location.has_value())
	{
		const string& _l_sl{ abc::utility::str::location_string(_a_source_location.value()) };
		if (not _m_internal_map.contains(_l_sl))
		{
			_m_internal_map.insert({ _l_sl,{} });
		}
		if (_a_str.has_value())
		{
			_m_internal_map.at(_l_sl).push_back(_a_str.value());
		}
	}
}
__constexpr_imp
	const std::map<std::string, std::vector<std::string>>&
	matcher_source_map_t::map(
	) const noexcept
{
	return _m_internal_map;
}
__constexpr_imp
	std::size_t
	matcher_source_map_t::size(
	) const noexcept
{
	return _m_internal_map.size();
}
_END_ABC_NS