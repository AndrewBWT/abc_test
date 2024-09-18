#pragma once

#include "abc_test/matchers/generic_matcher.h"
#include "abc_test/core/errors/test_assertion_exception.h"

#include "abc_test/utility/str/string_utility.h"

//Macros

#define _INTERNAL_ASSERT(test_to_run, throw_exception, macro_name) abc::create_assertion(test_to_run, abc::utility::str::create_string({macro_name, "(", #test_to_run, ")"}), \
std::source_location::current(),abc::global::get_this_threads_test_runner_ref(), throw_exception)
#define _CHECK(test_to_run) _INTERNAL_ASSERT(test_to_run, false, "_CHECK")
#define _REQUIRE(test_to_run) _INTERNAL_ASSERT(test_to_run, true, "_REQUIRE")
#define _FAIL(string_to_print) abc::create_blank_assertion(abc::utility::str::create_string({"FAIL(\"",string_to_print, "\")"}), \
std::source_location::current(),abc::global::get_this_threads_test_runner_ref(),false)
#define _FAIL_AND_TERMINATE(string_to_print) abc::create_blank_assertion(abc::utility::str::create_string({"_FAIL_AND_TERMINATE(\"",string_to_print, "\")"}), \
std::source_location::current(),abc::global::get_this_threads_test_runner_ref(),true)

#define _MATCHER(Code) abc::matcher_t(Code, #Code)

//! Not macros
//#define _CHECK_NOT(test_to_run) _INTERNAL_ASSERT(test_to_run, false, "_CHECK_NOT")
//#define _REQUIRE_NOT(test_to_run) _INTERNAL_ASSERT(test_to_run, true, "_REQUIRE_NOT")

#define _MAKE_FUNC(Code) std::function<void()>_l_tut{[]() { Code;};};

_BEGIN_ABC_NS
/*!
* Function which runs the actual assertions.
* @param _a_matcher The matcher object being ran.
* @param _a_str_representation_of_line A literal expansion of the line, used in debug messages
* @param _a_test_runner The test_runner running the test.
* @param _a_terminate_on_failure Whether to throw an exception, or to log the error and continue.
* True acts like _REQUIRE, false like _CHECK.
	
*/
__constexpr
	bool
	create_assertion(
		generic_matcher_t& _a_matcher,
		const std::string_view _a_str_representation_of_line,
		const std::source_location& _a_source_location,
		test_runner_t& _a_test_runner,
		const bool _a_terminate_function_on_failure
	);
__constexpr
	bool
	create_assertion(
		generic_matcher_t&& _a_matcher,
		const std::string_view _a_str_representation_of_line,
		const std::source_location& _a_source_location,
		test_runner_t& _a_test_runner,
		const bool _a_terminate_function_on_failure
	);
__constexpr
	bool
	create_blank_assertion(
		const std::string_view _a_str_representation_of_line,
		const std::source_location& _a_source_location,
		test_runner_t& _a_test_runner,
		const bool _a_early_termination
	);
namespace
{
	template<
		typename T
	>
	__constexpr
		bool
		create_assertion_internal(
			T _a_matcher,
			const std::string_view _a_str_representation_of_line,
			const std::source_location& _a_source_location,
			test_runner_t& _a_test_runner,
			const bool _a_terminate_function_on_failure
		);
}
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	bool
	create_assertion(
		generic_matcher_t& _a_matcher,
		const std::string_view _a_str_representation_of_line,
		const std::source_location& _a_source_location,
		test_runner_t& _a_test_runner,
		const bool _a_terminate_function_on_failure
	)
{
	return create_assertion_internal<generic_matcher_t&>(_a_matcher,
		_a_str_representation_of_line, _a_source_location,_a_test_runner,
		_a_terminate_function_on_failure
	);
}
__constexpr_imp
	bool
	create_assertion(
		generic_matcher_t&& _a_matcher,
		const std::string_view _a_str_representation_of_line,
		const std::source_location& _a_source_location,
		test_runner_t& _a_test_runner,
		const bool _a_terminate_function_on_failure
	)
{
	return create_assertion_internal<generic_matcher_t&&>(std::move(_a_matcher),
		_a_str_representation_of_line, _a_source_location,_a_test_runner,
		_a_terminate_function_on_failure);
}
__constexpr_imp
	bool
	create_blank_assertion(
		const std::string_view _a_str_representation_of_line,
		const std::source_location& _a_source_location,
		test_runner_t& _a_test_runner,
		const bool _a_early_termination
	)
{
	using namespace std;
	using namespace errors;
	_a_test_runner.add_error(test_failure_info_t(
		_a_str_representation_of_line,
		_a_source_location,
		_a_test_runner.get_log_infos(false), _a_early_termination));
	return false;
}
namespace
{
	template<
		typename T
	>
	__constexpr_imp
		bool
		create_assertion_internal(
			T _a_matcher,
			const std::string_view _a_str_representation_of_line,
			const std::source_location& _a_source_location,
			test_runner_t& _a_test_runner,
			const bool _a_terminate_function_on_failure
		)
	{
		using namespace std;
		using namespace errors;
		const matcher_result_t& _l_mr{ _a_matcher.run_test(_a_test_runner) };
		_a_test_runner.register_tests_most_recent_source(_a_source_location);
		if (not _l_mr.passed())
		{
			_a_test_runner.add_error(test_failure_info_t(
				_a_str_representation_of_line,
				_a_source_location,
				&_a_matcher,
				_l_mr.str(),
				_a_test_runner.get_log_infos(false), _a_terminate_function_on_failure)
			);
			if (_a_terminate_function_on_failure)
			{
				throw test_assertion_exception_t();
			}
			return false;
		}
		else
		{
			return true;
		}
	}
}
_END_ABC_NS