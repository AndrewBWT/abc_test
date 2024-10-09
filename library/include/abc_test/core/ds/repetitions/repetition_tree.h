#pragma once




#include "abc_test/core/ds/repetitions/for_loop_data.h"
#include "abc_test/core/errors/test_library_exception.h"
#include "fmt/base.h"

#include "fmt/ranges.h"
#include <charconv>
#include "abc_test/utility/str/parser_utility.h"

#include <exception>

_BEGIN_ABC_NS
class test_options_t;
_END_ABC_NS

_BEGIN_ABC_DS_NS
template<
	typename bool Is_Root
>
class repetition_tree_node_t
{
public:
	__constexpr
		repetition_tree_node_t(
		) noexcept = default;
	template < typename = typename std::enable_if<not Is_Root>::type >
	__constexpr
		repetition_tree_node_t(
			const for_loop_iteration_data_t& _a_flid
		) noexcept;
	/*!
* Prints the object in a format as described by the test_options.
*/
	__constexpr
		std::string
		print_repetition_tree_compressed(
		) const noexcept;
	/*!
	* Prints a repretition tree without a test options.
	*/
	__constexpr
		std::string
		print_repetition_tree(
		) const noexcept;
	/*!
* Incrmeents a given for loop;
*/
	__constexpr
		std::optional<repetition_data_t>
		increment_last_index(
			const repetition_data_sequence_t& _a_rds
		) const noexcept;
	/*!
	* Checks if the given for_loop_data_collection_t is contained in this repetition tree.
	*/
	__constexpr
		bool
		is_repetition_to_be_repeated(
			const repetition_data_sequence_t& _a_rds
		) const noexcept;
	/*!
	* Adds a repetition data sequence to the underlying structure.
	*/
	__constexpr
		bool
		add_repetition(
			const repetition_data_sequence_t& _a_rds
		) noexcept;
	__constexpr
		std::optional<repetition_data_t>
		find_next_for_loop(
			const repetition_data_sequence_t& _a_rds
		) const noexcept;
	friend class repetition_tree_node_t<not Is_Root>;
private:
	std::conditional_t<not Is_Root, for_loop_iteration_data_t, std::monostate> _m_for_loop_data;
	std::vector<std::vector<std::shared_ptr<repetition_tree_node_t<false>>>> _m_children;
	__constexpr
		std::optional<repetition_data_t>
		increment_last_index(
			const repetition_data_sequence_t& _a_rds,
			const std::size_t _a_idx
		) const noexcept;
	__constexpr
		std::optional<repetition_data_t>
		find_next_for_loop(
			const repetition_data_sequence_t& _a_rds,
			const std::size_t _a_idx
		) const noexcept;
	__constexpr
		bool
		is_repetition_to_be_repeated(
			const repetition_data_sequence_t& _a_rds,
			const std::size_t _a_idx
		) const noexcept;
	__constexpr
		bool
		add_repetition(
			const repetition_data_sequence_t& _a_rds,
			const std::size_t _a_idx
		) noexcept;
};
using repetition_tree_t = repetition_tree_node_t<true>;
__constexpr
std::expected<repetition_tree_t, std::string>
parse_repetition_tree(
	const std::string_view _a_str,
	const test_options_t& _a_options,
	const std::size_t _a_depth
) noexcept;
_END_ABC_DS_NS

template<
	typename bool Is_Root
>
struct fmt::formatter<abc::ds::repetition_tree_node_t<Is_Root>> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	//Can't be constexpr due to use of fmt
	__no_constexpr
		auto
		format(
			abc::ds::repetition_tree_node_t<Is_Root> _a_rt,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
template<
	typename bool Is_Root
>
template < typename>
__constexpr_imp
repetition_tree_node_t<Is_Root>::repetition_tree_node_t(
	const for_loop_iteration_data_t& _a_flid
) noexcept
	: _m_for_loop_data(_a_flid)
{

}
template<
	typename bool Is_Root
>
__constexpr_imp
std::string
repetition_tree_node_t<Is_Root>::print_repetition_tree_compressed(
) const noexcept
{
	using namespace std;
	string _l_rv{ print_repetition_tree() };
	vector<unsigned char> _l_chars;
	for (char _l_char : _l_rv)
	{
		_l_chars.push_back(static_cast<unsigned char>(_l_char));
	}
	_l_rv.clear();
	for (unsigned char _l_char : _l_chars)
	{
		_l_rv.append(fmt::format("{:x}", _l_char));
	}
	return _l_rv;
}
/*!
* Prints a repretition tree without a test options.
*/
template<
	typename bool Is_Root
>
__constexpr_imp
std::string
repetition_tree_node_t<Is_Root>::print_repetition_tree(
) const noexcept
{
	using namespace std;
	using namespace utility::str;
	string _l_children_str{ _c_l_square_bracket };
	for (size_t _l_idx{ 0 }; _l_idx < _m_children.size(); ++_l_idx)
	{
		vector<shared_ptr< repetition_tree_node_t<false>>> _l_for_loop_at_idx{ _m_children[_l_idx] };
		string _l_children_str{ _c_l_square_bracket };
		for (size_t _l_jdx{ 0 }; _l_jdx < _l_for_loop_at_idx.size(); ++_l_jdx)
		{
			_l_children_str.append(_l_for_loop_at_idx[_l_jdx]->print_repetition_tree());
			_l_children_str.append(_l_jdx + 1 < _m_children.size() ? _c_comma : "");
		}
		_l_children_str.append(_c_r_square_bracket);
		_l_children_str.append(_l_idx + 1 < _m_children.size() ? _c_comma : "");
	}
	_l_children_str.append(_c_r_square_bracket);
	if constexpr (Is_Root)
	{
		return _l_children_str;
	}
	else
	{
		return fmt::format("({0},{1},\"{2}\",{3}",
			_m_for_loop_data.generation_collection_index,
			_m_for_loop_data.mode,
			_m_for_loop_data.additional_data,
			_l_children_str);
	}
}
template<
	typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::increment_last_index(
	const repetition_data_sequence_t& _a_rds
) const noexcept
{
	return increment_last_index(_a_rds, 0);
}
/*template<
	typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::find_next_for_loop(
	const for_loop_data_collection_t& _a_for_loop_data_collection
) const noexcept
{
	using namespace std;
	using enum utility::internal::internal_log_enum_t;
	const repetition_data_sequence_t& _l_rds{ _a_for_loop_data_collection.repetition_data_sequence() };
	if (_l_rds.size() == 0)
	{
		if (_m_children.size() > 0 &&
			_m_children[0].size() > 0)
		{
			return repetition_data_t{ 0,_m_children[0][0].get()->for_loop_data() };
		}
		else
		{
			return optional<repetition_data_t>{};
		}
	}
	else
	{
		return find_next_for_loop(_l_rds, 0);
	}
}*/
/*!
* Incrmeents a given for loop;
*/
/*!
* Checks if the given for_loop_data_collection_t is contained in this repetition tree.
*/
template<
	typename bool Is_Root
>
__constexpr_imp
bool
repetition_tree_node_t<Is_Root>::is_repetition_to_be_repeated(
	const repetition_data_sequence_t& _a_rds
) const noexcept
{
	return is_repetition_to_be_repeated(_a_rds, 0);
}
/*!
* Adds a repetition data sequence to the underlying structure.
*/
template<
	typename bool Is_Root
>
__constexpr_imp
bool
repetition_tree_node_t<Is_Root>::add_repetition(
	const repetition_data_sequence_t& _a_rds
) noexcept
{
	return add_repetition(_a_rds, 0);
}
template<
	typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::find_next_for_loop(
	const repetition_data_sequence_t& _a_rds
) const noexcept
{
	return find_next_for_loop(_a_rds, 0);
}
template<
	typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::increment_last_index(
	const repetition_data_sequence_t& _a_rds,
	const std::size_t _a_idx
) const noexcept
{
	using namespace std;
	using enum utility::internal::internal_log_enum_t;
	if (_a_idx >= _a_rds.size())
	{
		return optional<repetition_data_t>{};
	}
	else if (_a_idx + 1 >= _a_rds.size())
	{
		const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
		vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
		size_t _l_idx{ 0 };
		for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
		{
			if (_l_rfl->_m_for_loop_data == _l_rd.for_loop_iteration_data &&
				_l_idx + 1 < _l_rtn.size())
			{
				return repetition_data_t{
					_l_rd.for_loop_index,_l_rtn[_l_idx + 1]->_m_for_loop_data };
			}
			_l_idx++;
		}
	}
	else
	{
		const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
		vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
		for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
		{
			if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
			{
				return _l_rfl->increment_last_index(_a_rds, _a_idx + 1);
			}
		}
		return std::optional<repetition_data_t>{};
	}
	return std::optional<repetition_data_t>{};
}
template<
	typename bool Is_Root
>
__constexpr_imp
std::optional<repetition_data_t>
repetition_tree_node_t<Is_Root>::find_next_for_loop(
	const repetition_data_sequence_t& _a_rds,
	const std::size_t _a_idx
) const noexcept
{
	using namespace std;
	using enum utility::internal::internal_log_enum_t;
	if (_a_idx >= _a_rds.size())
	{
		if (_m_children.size() > 0 &&
			_m_children[0].size() > 0)
		{
			return repetition_data_t{ 0,_m_children[0][0].get()->_m_for_loop_data };
		}
		else
		{
			return optional<repetition_data_t>{};
		}
	}
	else
	{
		const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
		vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
		for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
		{
			if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
			{
				return _l_rfl->find_next_for_loop(_a_rds, _a_idx + 1);
			}
		}
		return std::optional<repetition_data_t>{};
	}
	return std::optional<repetition_data_t>{};
}
template<
	typename bool Is_Root
>
__constexpr_imp
bool
repetition_tree_node_t<Is_Root>::is_repetition_to_be_repeated(
	const repetition_data_sequence_t& _a_rds,
	const std::size_t _a_idx
) const noexcept
{
	using namespace std;
	using enum utility::internal::internal_log_enum_t;
	if (_a_idx >= _a_rds.size())
	{
		return false;
	}
	else if (_a_idx + 1 >= _a_rds.size())
	{
		const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
		size_t _l_idx{ 0 };
		vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
		for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
		{
			if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
			{
				return true;
			}
			_l_idx++;
		}
		return false;
	}
	else
	{
		const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
		vector < shared_ptr<repetition_tree_node_t<false>>> _l_rtn{ _m_children[_l_rd.for_loop_index] };
		for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
		{
			if (_l_rfl.get()->_m_for_loop_data == _l_rd.for_loop_iteration_data)
			{
				return _l_rfl->is_repetition_to_be_repeated(_a_rds, _a_idx + 1);
			}
		}
		return false;
	}
}
template<
	typename bool Is_Root
>
__constexpr_imp
bool
repetition_tree_node_t<Is_Root>::add_repetition(
	const repetition_data_sequence_t& _a_rds,
	const std::size_t _a_idx
) noexcept
{
	using namespace std;
	using enum utility::internal::internal_log_enum_t;
	_LIBRARY_LOG(REPETITION_INFO, fmt::format("Adding repetition sequence {0}", _a_rds));
	if (_a_idx >= _a_rds.size())
	{
		return true;
	}
	else
	{
		const repetition_data_t& _l_rd{ _a_rds[_a_idx] };
		if (_l_rd.for_loop_index >= _m_children.size())
		{
			_m_children.resize(_l_rd.for_loop_index + 1);
		}
		vector < shared_ptr<repetition_tree_node_t<false>>>& _l_rtn{ _m_children[_l_rd.for_loop_index] };
		for (const shared_ptr<repetition_tree_node_t<false>>& _l_rfl : _l_rtn)
		{
			if (_l_rfl->_m_for_loop_data == _l_rd.for_loop_iteration_data)
			{
				return _l_rfl->add_repetition(_a_rds, _a_idx + 1);
			}
		}
		shared_ptr < repetition_tree_node_t<false>> _l_new_child{
			shared_ptr<repetition_tree_node_t<false>>(new repetition_tree_node_t<false>(_l_rd.for_loop_iteration_data)) };
		vector < shared_ptr<repetition_tree_node_t<false>>>::iterator _l_x =
			upper_bound(_l_rtn.begin(), _l_rtn.end(), _l_new_child,
				[](const auto& _a_left, const auto& _a_right) {
					return _a_left->_m_for_loop_data < _a_right->_m_for_loop_data; });
		vector < shared_ptr<repetition_tree_node_t<false>>>::iterator _l_itt = _l_rtn.insert(
			_l_x,
			_l_new_child
		);
		return (*_l_itt)->add_repetition(_a_rds, _a_idx + 1);
	}
}
__constexpr_imp
std::expected<repetition_tree_t, std::string>
parse_repetition_tree(
	const std::string_view _a_str,
	const test_options_t& _a_options,
	const std::size_t _a_depth
) noexcept
{
	return std::unexpected("hello");
}
_END_ABC_DS_NS