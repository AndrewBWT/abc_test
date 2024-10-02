#pragma once
#include <optional>

#include <fmt/std.h>
#include <source_location>

#include "abc_test/utility/internal/macros.h"

#include <string>

#include "fmt/base.h"

#include "fmt/format.h"

#include "abc_test/core/ds/test_collections/test_list.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/single_source.h"

/*! Inner macro which declares a test, registers a test, and then provides that test's
* definition.
@param Description The test description
@param Path An identifier for the test, used for organisation.
@param Function_Name The name of the function. When used with _TEST_CASE, this will have been auto-generated.
*/
#define FUNCTION_PREFIX _a_establish

#define _TEST_CASE_1(_a_description,_a_path,_a_function_name,_a_threads_required,\
	 _a_str_representation)\
	static void _a_function_name();\
	abc::ds::registered_test_data_t NAMED_COUNTER(_a_establish)(\
		_a_function_name,\
		_a_description,\
		_a_path,\
		abc::reports::single_source_t(_a_str_representation,std::source_location::current()),\
		_a_threads_required\
	);\
	static void _a_function_name()

#define TEST_CASE_STR _l_test_case


#define _TEST_CASE(_a_description, _a_path) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		NAMED_COUNTER(TEST_CASE_STR),\
		1,\
		abc::utility::str::create_string({"_TEST_CASE(",#_a_description,",",#_a_path,")"})\
	)


#define _TEST_CASE_THREADED(_a_description, _a_path, _a_threads) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		NAMED_COUNTER(TEST_CASE_STR),\
		_a_threads,\
		abc::utility::str::create_string({"_TEST_CASE_THREADED(",#_a_description,",",#_a_path,",",#_a_threads,")"})\
	)

#define _NAMED_TEST_CASE(_a_name, _a_description, _a_path) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		_a_name,\
		1,\
		abc::utility::str::create_string({"_NAMED_TEST_CASE(",#_a_name,",", #_a_description,",",#_a_path,")"})\
	)

#define _NAMED_TEST_CASE_THREADED(_a_name, _a_description, _a_path, _a_threads) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		_a_name,\
		_a_threads,\
		abc::utility::str::create_string({"_NAMED_TEST_CASE_THREADED(",#_a_name, ",",#_a_description,",",#_a_path,",",#_a_threads,")"})\
	)

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
				const reports::single_source_t& _a_source,
				const std::size_t _a_thread_resourses_required
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
		reports::single_source_t _m_source;
		//opt_source_loc_t _m_source_location;
		/*!
		* The number of thread resourses required by the test.
		*/
		std::size_t _m_thread_resourses_required;
	};
	__constexpr_imp
		void
		register_test(
			const registered_test_data_t& _a_rtd
		) noexcept
	{
		
	}
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
		: registered_test_data_t(test_function_t(), "", "", reports::single_source_t{}, 1)
	{

	}
	__constexpr_imp
		registered_test_data_t::registered_test_data_t(
			const test_function_t _a_test_function,
			const test_description_ref_t _a_description,
			const test_path_ref_t _a_test_path,
			const reports::single_source_t& _a_source,
			const std::size_t _a_thread_resourses_required
		) noexcept
		: _m_description(_a_description)
		, _m_test_path(_a_test_path)
		, _m_source(_a_source)
		, _m_test_function(_a_test_function)
		, _m_thread_resourses_required(_a_thread_resourses_required)
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
		__cmp_test(_m_description);
		__cmp_test(_m_test_path);
		__cmp_test(_m_thread_resourses_required);
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
	size_t _m_thread_resourses_required;
	using namespace std;
	string _l_rv{ fmt::format(
		"registered_test_data_t {{"
		"test_function_t = {0}, "
		"_m_description = \"{1}\", "
		"_m_test_path = \"{2}\", "
		"_m_source = {3}, "
		"_m_thread_resourses_required = {4}"
		"}}",
		_a_rtd._m_test_function != nullptr ? fmt::format("{0}", static_cast<void*>(_a_rtd._m_test_function)) : "nullptr",
		_a_rtd._m_description,
		_a_rtd._m_test_path,
		_a_rtd._m_source,
		_a_rtd._m_thread_resourses_required
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}