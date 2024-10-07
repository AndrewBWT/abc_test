#pragma once
#include "abc_test/utility/internal/macros.h"
#include <string>
#include <optional>
#include <fmt/base.h>
#include <typeinfo>

_BEGIN_ABC_DS_NS
/*!
 * @brief Contains user-defined information for a test function.
 * 
 * All the fields are public. This object is designed to be declared as part of a test
 * function. 
 */
class user_defined_test_data_t
{
public:
	/*!
	 * @brief Equality operator.
	 * @param _a_rhs 
	 */
	__constexpr
		bool
		operator==(
			const user_defined_test_data_t& _a_rhs
			) const noexcept;
	//! The name of the test. This should never be equal to "".
	std::string_view name;
	/*!
	* The description of the test. This is an optional field. 
	*/
	std::optional<std::string_view> description;
	/*!
	* The test path of the test. Should be in the form "aa::bb::cc" (where :: is a delimiter chosen by the user)
	*/
	std::string_view path;
	/*!
	* The number of thread resourses required by the test. Default of 1. Should never be zero.
	*/
	std::size_t threads_required = 1;
};
_END_ABC_DS_NS

_BEGIN_ABC_NS
//! A type synonym which makes the users life easier.
using test_data_t = ds::user_defined_test_data_t;
_END_ABC_NS

template
<
>
struct fmt::formatter<abc::ds::user_defined_test_data_t> : formatter<string_view> {
	// parse is inherited from formatter<string_view>.
	//Can't be cosntepxr due to use of fmt::format.
	/*!
	* @brief Formats a user_defined_test_data_t object.
	* @param _a_rtd The user_defined_test_data_t object.
	* @param _a_ctx The format_context.
	* @return The formatted string.
	*/
	__no_constexpr
	auto
		format(
			abc::ds::user_defined_test_data_t _a_rtd,
			format_context& _a_ctx
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp
bool
user_defined_test_data_t::operator==(
	const user_defined_test_data_t& _a_rhs
	) const noexcept
{
	__cmp_test(name);
	__cmp_test(description);
	__cmp_test(path);
	__cmp_test(threads_required);
	return true;
}
_END_ABC_DS_NS

__no_constexpr_imp
auto
fmt::formatter<abc::ds::user_defined_test_data_t>::format(
	abc::ds::user_defined_test_data_t _a_rtd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_rv{ fmt::format(
		"{0}{{"
		"{1} = {2},"
		"{3} = {4},"
		"{5} = {6},"
		"{7} = {8}"
		"}}",
		typeid(_a_rtd).name(),
		"name", _a_rtd.name,
		"description", _a_rtd.description,
		"path", _a_rtd.path,
		"threads_required", _a_rtd.threads_required
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}