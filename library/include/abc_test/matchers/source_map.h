#pragma once

#include <map>

#include <fmt/std.h>
#include <fmt/ranges.h>
#include <source_location>

#include "abc_test/utility/internal/macros.h"

#include "abc_test/utility/str/string_utility.h"

#include "abc_test/core/ds/source/single_source.h"

_BEGIN_ABC_NS
struct source_location_lt_t
{
	__constexpr
		bool
		operator()(
			const std::source_location& _a_lhs,
			const std::source_location& _a_rhs
			) const noexcept;
};
class matcher_source_map_t
{
public:
	//Can't be constexpr as calls insert_source.
	__no_constexpr
		void
		insert(
			const ds::single_source_t& _a_source
		);
	__constexpr
		const std::map<std::source_location, std::vector<std::string>, source_location_lt_t>&
		map(
		) const noexcept;
	__constexpr
		bool
		has_elements(
		) const noexcept;
	__no_constexpr_or_inline
		std::string
		gather_list_of_sources_and_representations(
		) const noexcept;
private:
	bool _m_has_elements = false;
	std::map<std::source_location, std::vector<std::string>, source_location_lt_t> _m_internal_map;
	//Can't be constexpr as changes map.
	__no_constexpr
		void
		insert_source(
			const std::source_location& _a_sl
		) noexcept;
};

_END_ABC_NS

/*!
* formatter for post_setup_test_ata object.
*/
template
<
>
struct fmt::formatter<abc::matcher_source_map_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	//Can't be constexpr due to fmt.
	__no_constexpr
		auto
		format(
			abc::matcher_source_map_t _a_iti,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_NS

__constexpr_imp
bool
source_location_lt_t::operator()(
	const std::source_location& _a_lhs,
	const std::source_location& _a_rhs
	) const noexcept
{
	if (_a_lhs.file_name() < _a_rhs.file_name())
	{
		return true;
	}
	else if (_a_lhs.file_name() > _a_rhs.file_name())
	{
		return false;
	}
	else
	{
		if (_a_lhs.function_name() < _a_rhs.function_name())
		{
			return true;
		}
		else if (_a_lhs.function_name() > _a_rhs.function_name())
		{
			return false;
		}
		else
		{
			if (_a_lhs.line() < _a_rhs.line())
			{
				return true;
			}
			else if (_a_lhs.line() > _a_rhs.line())
			{
				return false;
			}
			else
			{
				return _a_lhs.column() < _a_rhs.column();
			}
		}
	}
}
__no_constexpr_imp
	void
	matcher_source_map_t::insert(
		const ds::single_source_t& _a_source
	)
{
	using namespace std;
	using namespace ds;
	_m_has_elements = true;
	insert_source(_a_source.source_location());
	_m_internal_map.at(_a_source.source_location()).push_back(string(_a_source.source_code_representation()));
}
__constexpr_imp
	const std::map<std::source_location, std::vector<std::string>, source_location_lt_t>&
	matcher_source_map_t::map(
	) const noexcept
{
	return _m_internal_map;
}
__constexpr_imp
	bool
	matcher_source_map_t::has_elements(
	) const noexcept
{
	return _m_has_elements;
}
__no_constexpr_imp
void
matcher_source_map_t::insert_source(
	const std::source_location& _a_sl
) noexcept
{
	if (not _m_internal_map.contains(_a_sl))
	{
		_m_internal_map.insert({ _a_sl,{} });
	}
}
_END_ABC_NS

__no_constexpr_imp
auto
fmt::formatter<abc::matcher_source_map_t>::format(
	abc::matcher_source_map_t _a_rtd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	size_t _m_thread_resourses_required;
	using namespace std;
	string _l_rv{ fmt::format(
		"{0} {{"
		"{1} = {2}"
		"}}"
		, typeid(_a_rtd).name()
		, "_m_internal_map"
		, _a_rtd.map()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}