#pragma once
#include "abc_test/internal/utility/internal/macros.h"
#include <vector>
#include <string>

_BEGIN_ABC_UTILITY_STR_NS
class string_table_t
{
public:
	__no_constexpr
		string_table_t(
			const std::vector<std::size_t>& _a_columns_to_align = std::vector<std::size_t>()
		) noexcept;
	__constexpr
		std::string
		operator()() const noexcept;
	__no_constexpr
		void
		push_back(
			const std::string_view _a_str,
			const std::size_t _a_indent = 0
		) noexcept;
	__no_constexpr
		void
		push_empty(
		) noexcept;
	__constexpr
		void
		new_line(
		) noexcept;
private:
	std::vector<std::vector<std::string>> _m_str_table;
	std::map<std::size_t, std::size_t> _m_columns_to_largest_str;
	std::size_t _m_next_column;
	bool _m_new_line_directive_issued;
};
namespace 
{
	__no_constexpr
		std::map<std::size_t, std::size_t>
		make_columns_to_largest_str(
			const std::vector<std::size_t>& _a_columns_to_align
		) noexcept;
}
_END_ABC_UTILITY_STR_NS
_BEGIN_ABC_UTILITY_STR_NS
__no_constexpr_imp
	string_table_t::string_table_t(
		const std::vector<std::size_t>& _a_columns_to_align
	) noexcept
	: _m_str_table(std::vector<std::vector<std::string>>())
	, _m_columns_to_largest_str(make_columns_to_largest_str(_a_columns_to_align))
	, _m_next_column(0)
	, _m_new_line_directive_issued(true)
{

}
__constexpr_imp
	std::string
	string_table_t::operator()() const noexcept
{
	using namespace std;
	string _l_rv{};
	for (const std::vector<std::string>& _l_row : _m_str_table)
	{
		size_t _l_column{ 0 };
		for (const std::string& _l_table_element : _l_row)
		{
			_l_rv.append(_l_table_element);
			if (_m_columns_to_largest_str.contains(_l_column))
			{
				_l_rv.append(string(
					_m_columns_to_largest_str.at(_l_column) -
					_l_table_element.size(), ' '));
			}
			++_l_column;
		}
		_l_rv.append("\n");
	}
	return _l_rv;
}
__no_constexpr_imp
	void
	string_table_t::push_back(
		const std::string_view _a_str,
		const std::size_t _a_indent
	) noexcept
{
	using namespace std;
	if (_m_new_line_directive_issued)
	{
		_m_str_table.push_back(vector<string>());
		_m_new_line_directive_issued = false;
	}
	const string _l_to_insert{ string(_a_indent, ' ') + string(_a_str) };
	const size_t _l_to_insert_size{ _l_to_insert.size() };
	if (_m_columns_to_largest_str.contains(_m_next_column))
	{
		if (_m_columns_to_largest_str.at(_m_next_column) < _l_to_insert_size)
		{
			_m_columns_to_largest_str.at(_m_next_column) = _l_to_insert_size;
		}
	}
	++_m_next_column;
	_m_str_table.back().push_back(_l_to_insert);
}
__no_constexpr
	void
	string_table_t::push_empty(
	) noexcept
{
	using namespace std;
	push_back(string_view());
}
__constexpr_imp
	void
	string_table_t::new_line(
	) noexcept
{
	using namespace std;
	_m_new_line_directive_issued = true;
	_m_next_column = 0;
}
namespace
{
	__no_constexpr_imp
		std::map<std::size_t, std::size_t>
		make_columns_to_largest_str(
			const std::vector<std::size_t>& _a_columns_to_align
		) noexcept
	{
		using namespace std;
		map<size_t, size_t> _l_rv;
		for (const size_t _l_column : _a_columns_to_align)
		{
			_l_rv.insert({ _l_column,0 });
		}
		return _l_rv;
	}
}
_END_ABC_UTILITY_STR_NS