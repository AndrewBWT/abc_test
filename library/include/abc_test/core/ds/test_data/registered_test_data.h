#pragma once
#include <optional>

#include <fmt/std.h>
#include <source_location>

#include "abc_test/utility/internal/macros.h"

#include <string>

#include "fmt/base.h"

#include "fmt/format.h"

#include "abc_test/core/ds/source/single_source.h"

#include "abc_test/core/ds/test_data/checked_user_defined_test_data.h"
#include "abc_test/core/ds/type_synonyms.h"

_BEGIN_ABC_DS_NS
/*!
* The object used to store the data required for a test.
* This object is designed to be initialised by a macro, not a human.
*/
struct registered_test_data_t
{
public:
	// Default constructor deleted.
	__constexpr
		registered_test_data_t(
		) noexcept = delete;
	/*!
	 * @brief Only constructor for object.
	 * @param _a_test_function A reference to the test function.
	 * @param _a_user_data The user provided test data, already checked.
	 * @param _a_source Source information about the test.
	 * @return 
	 */
	__constexpr
		registered_test_data_t(
			const test_function_t _a_test_function,
			const checked_user_defined_test_data_t _a_user_data,
			const ds::single_source_t& _a_source
		) noexcept;
	/*!
	 * @brief User defined equality operator.
	 * @arg _a_rhs Argument to compare element to.
	 * @return bool True if equal, false if not.
	 */
	__constexpr
		bool
		operator==(
			const registered_test_data_t& _a_rhs
			) const noexcept = default;
	/*!
	* The location of the function for this test.
	*/
	test_function_t _m_test_function;
	/*! The user defined test data.
	 */
	user_defined_test_data_t _m_user_data;
	/*!
	* The location of the test. This is an optional field.
	*/
	ds::single_source_t _m_source;
};
_END_ABC_DS_NS

/*!
* formatter for registered_test_data_t
*/
template
<
>
struct fmt::formatter<abc::ds::registered_test_data_t> : formatter<string_view> {
	// parse is inherited from formatter<string_view>.
	//Can't be constexpr due to use of fmt::format.
	__no_constexpr
		auto 
		format(
			abc::ds::registered_test_data_t _a_rtd, 
			format_context& _a_ctx
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp
registered_test_data_t::registered_test_data_t(
	const test_function_t _a_test_function,
	const checked_user_defined_test_data_t _a_user_data,
	const ds::single_source_t& _a_source
) noexcept
	: _m_user_data(_a_user_data.user_defined_test_data())
	, _m_source(_a_source)
	, _m_test_function(_a_test_function)
{
}
_END_ABC_DS_NS

__no_constexpr_imp
auto 
fmt::formatter<abc::ds::registered_test_data_t>::format(
	abc::ds::registered_test_data_t _a_rtd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_rv{ fmt::format(
		"{0}{{"
		"{1} = {2},"
		"{3} = {4},"
		"{5} = {6}"
		"}}",
		typeid(_a_rtd).name(),
		"_m_test_function", fmt::format("{0}", static_cast<void*>(_a_rtd._m_test_function)),
		"_m_user_data", _a_rtd._m_user_data,
		"_m_source", _a_rtd._m_source
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}