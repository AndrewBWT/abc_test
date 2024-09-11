#pragma once
#include <optional>

#include <fmt/std.h>
#include <source_location>

#include "abc_test/utility/internal/macros.h"

#include <string>

#include "fmt/base.h"

#include "fmt/format.h"

/*! Inner macro which declares a test, registers a test, and then provides that test's
* definition.
@param Description The test description
@param Path An identifier for the test, used for organisation.
@param Function_Name The name of the function. When used with _TEST_CASE, this will have been auto-generated.
*/
#define _TEST_CASE_1(Description,Path,Function_Name,Threads_Required)\
static void Function_Name(); \
abc::ds::registered_test_data_t NAMED_COUNTER(Establish)(Function_Name, \
Description,Path,std::source_location::current(),Threads_Required);\
static void Function_Name()

#define _TEST_CASE(Description, Path) _TEST_CASE_1(Description,Path,NAMED_COUNTER(Test_Case),1)
#define _TEST_CASE_THREADED(Description, Path, Threads) _TEST_CASE_1(Description,Path,NAMED_COUNTER(Test_Case),Threads)

_BEGIN_ABC_DS_NS
	using test_function_t = void (*)();
	using test_description_t = std::string;
	using test_description_ref_t = std::string_view;
	using test_path_t = std::string;
	using test_path_ref_t = std::string_view;
	using opt_source_loc_t = std::optional<std::source_location>;
	//	using test_function_t = void(*)(ab::test::test_runner_t&);
		/*!
		* The object used to store the data required for a test.
		* This object is designed to be initialised by a macro, not a human.
		*/
	struct registered_test_data_t
	{
	public:

		/*!
		* Generic default constructor, shouldn't really be used.
		*/
		__constexpr
			registered_test_data_t(
			) noexcept;
		/*!
		* Constructor generally used. Should be called by a macro, to ensure these
		* fields are written correclty.
		*/
		__constexpr
			registered_test_data_t(
				const test_function_t _a_test_function,
				const test_description_ref_t _a_description,
				const test_path_ref_t _a_test_path,
				const opt_source_loc_t& _a_source_location,
				const size_t _a_thread_resourses_required
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
		/*!
		* The description of the test. This is also its name.
		*
		*/
		test_description_t _m_description;
		/*!
		* The test path of the test. In the form "aa::bb::cc" (where :: is a delimiter chosen by the user)
		*/
		test_path_t _m_test_path;
		/*!
		* The location of the test. This is an optional field.
		*/
		opt_source_loc_t _m_source_location;
		/*!
		* The number of thread resourses required by the test.
		*/
		size_t _m_thread_resourses_required;
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
	__constexpr_imp
		registered_test_data_t::registered_test_data_t(
		) noexcept
		: registered_test_data_t(test_function_t(), "", "", opt_source_loc_t{}, 1)
	{

	}
	__constexpr_imp
		registered_test_data_t::registered_test_data_t(
			const test_function_t _a_test_function,
			const test_description_ref_t _a_description,
			const test_path_ref_t _a_test_path,
			const opt_source_loc_t& _a_source_location,
			const size_t _a_thread_resourses_required
		) noexcept
		: _m_description(_a_description)
		, _m_test_path(_a_test_path)
		, _m_source_location(_a_source_location)
		, _m_test_function(_a_test_function)
		, _m_thread_resourses_required(_a_thread_resourses_required)
	{

	}
	__constexpr_imp
		bool
		registered_test_data_t::operator==(
			const registered_test_data_t& _a_rhs
			) const noexcept
	{
		__cmp_test(_m_test_function);
		__cmp_test(_m_description);
		__cmp_test(_m_test_path);
		__cmp_test(_m_thread_resourses_required);
		__cmp_test(_m_source_location.has_value());
		if (_m_source_location.has_value())
		{
			__cmp_test(_m_source_location.value().column());
			__cmp_test(_m_source_location.value().file_name());
			__cmp_test(_m_source_location.value().function_name());
			__cmp_test(_m_source_location.value().line());
		}
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
	size_t _m_thread_resourses_required;
	using namespace std;
	string _l_rv{ fmt::format(
		"registered_test_data_t {{"
		"test_function_t = {0}, "
		"_m_description = \"{1}\", "
		"_m_test_path = \"{2}\", "
		"_m_source_location = {3}, "
		"_m_thread_resourses_required = {4}"
		"}}",
		_a_rtd._m_test_function != nullptr ? fmt::format("{0}", static_cast<void*>(_a_rtd._m_test_function)) : "nullptr",
		_a_rtd._m_description,
		_a_rtd._m_test_path,
		_a_rtd._m_source_location,
		_a_rtd._m_thread_resourses_required
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}