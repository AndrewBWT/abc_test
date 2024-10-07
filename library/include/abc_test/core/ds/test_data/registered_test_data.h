#pragma once
#include <optional>

#include <fmt/std.h>
#include <source_location>

#include "abc_test/utility/internal/macros.h"

#include <string>

#include "fmt/base.h"

#include "fmt/format.h"

#include "abc_test/core/ds/test_collections/test_list.h"
#include "abc_test/core/ds/source/single_source.h"

#include "abc_test/core/ds/test_data/user_defined_test_data.h"

_BEGIN_ABC_DS_NS
//! The type of test functions; simply a reference to a function with no arguments and
//! which returns nothing.
using test_function_t = void (&)();
using test_description_t = std::string;
using test_description_ref_t = std::string_view;
using test_path_t = std::string;
using test_path_ref_t = std::string_view;
/*!
* The object used to store the data required for a test.
* This object is designed to be initialised by a macro, not a human.
*/
struct registered_test_data_t
{
public:

		/*!
		* Default constructor deleted.
		*/
		__constexpr
			registered_test_data_t(
			) noexcept = delete;
		/*!
		* Constructor generally used. Should be called by a macro, to ensure these
		* fields are written correclty.
		*/
		__constexpr
			registered_test_data_t(
				const test_function_t _a_test_function,
				const user_defined_test_data_t _a_user_data,
				//	const test_description_ref_t _a_description,
				//	const test_path_ref_t _a_test_path,
				const reports::single_source_t& _a_source
				//	const std::size_t _a_thread_resourses_required
			) noexcept;
		/*!
		* Equals operator for checking equality.
		*/
		__constexpr
			bool
			operator==(
				const registered_test_data_t& _a_rhs
				) const noexcept;
		/*!
		* The location of the function for this test.
		*/
		test_function_t _m_test_function;
		user_defined_test_data_t _m_user_data;
		/*!
		* The description of the test. This is also its name.
		*
		*/
		//test_description_t _m_description;
		/*!
		* The test path of the test. In the form "aa::bb::cc" (where :: is a delimiter chosen by the user)
		*/
		//test_path_t _m_test_path;
		/*!
		* The location of the test. This is an optional field.
		*/
		reports::single_source_t _m_source;
		//opt_source_loc_t _m_source_location;
		/*!
		* The number of thread resourses required by the test.
		*/
		//std::size_t _m_thread_resourses_required;
	};
_END_ABC_DS_NS

/*!
* formatter for registered_test_data_t
*/
template
<
>
__constexpr
struct fmt::formatter<abc::ds::registered_test_data_t> : formatter<string_view> {
	// parse is inherited from formatter<string_view>.

	auto 
		format(
			abc::ds::registered_test_data_t _a_rtd, 
			format_context& _a_ctx
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
	/*__constexpr_imp
		registered_test_data_t::registered_test_data_t(
		) noexcept
		: registered_test_data_t(test_function_t(), "", "", reports::single_source_t{}, 1)
	{

	}*/
	__constexpr_imp
		registered_test_data_t::registered_test_data_t(
			const test_function_t _a_test_function,
			const user_defined_test_data_t _a_user_data,
		//	const test_description_ref_t _a_description,
		//	const test_path_ref_t _a_test_path,
			const reports::single_source_t& _a_source
		//	const std::size_t _a_thread_resourses_required
		) noexcept
		: _m_user_data(_a_user_data)
		//: _m_description(_a_description)
		//, _m_test_path(_a_test_path)
		, _m_source(_a_source)
		, _m_test_function(_a_test_function)
		//, _m_thread_resourses_required(_a_thread_resourses_required)
	{
		abc::ds::add_test(*this);
	}
	__constexpr_imp
		bool
		registered_test_data_t::operator==(
			const registered_test_data_t& _a_rhs
			) const noexcept
	{
		__cmp_test(_m_test_function);
		__cmp_test(_m_user_data);
		//__cmp_test(_m_description);
		//__cmp_test(_m_test_path);
		//__cmp_test(_m_thread_resourses_required);
		__cmp_test(_m_source);
		return true;
	}
	_END_ABC_DS_NS

__constexpr_imp
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