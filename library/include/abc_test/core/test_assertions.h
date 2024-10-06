#pragma once

#include "abc_test/matchers/matcher.h"
#include "abc_test/core/errors/test_assertion_exception.h"

#include "abc_test/utility/str/string_utility.h"

#include "abc_test/core/test_reports/mid_test_invokation_report/static_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/generic_matcher_based_assertion.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass_or_fail.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass_or_terminate.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/terminate.h"

#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/pass.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/assertion_status/fail.h"
#include <concepts>

#include "abc_test/core/test_assertions/test_block.h"
#include "abc_test/core/test_reports/mid_test_invokation_report/matcher_based_assertion_block.h"

#include "abc_test/matchers/annotation.h"

#define _INTERNAL_CREATE_ASSERTION(_a_matcher, _a_assertion_type,\
	_a_str_representation_of_line)\
	abc::create_assertion<_a_assertion_type>(\
		_a_matcher,\
		abc::reports::single_source_t(_a_str_representation_of_line, std::source_location::current()),\
		abc::global::get_this_threads_test_runner_ref())
#define _CHECK(_a_matcher) \
	_INTERNAL_CREATE_ASSERTION(\
		_a_matcher, \
		abc::reports::pass_or_fail_t,\
		abc::utility::str::create_string({"_CHECK(", #_a_matcher, ")"})\
	)
#define _CHECK_WITH_MSG(_a_matcher, _a_msg_on_failure) \
	_INTERNAL_CREATE_ASSERTION(\
		_a_matcher, \
		abc::reports::pass_or_fail_t,\
		abc::utility::str::create_string({ "_CHECK_WITH_MSG(", #_a_matcher, ",", #_a_msg_on_failure,")" })\
	)
#define _REQUIRE(_a_matcher) \
	_INTERNAL_CREATE_ASSERTION(\
		_a_matcher, \
		abc::reports::pass_or_terminate_t,\
		abc::utility::str::create_string({"_REQUIRE(", #_a_matcher, ")"})\
	)
#define _REQUIRE_WITH_MSG(_a_matcher, _a_msg_on_failure) \
	_INTERNAL_CREATE_ASSERTION(\
		_a_matcher, \
		abc::reports::pass_or_terminate_t,\
		abc::utility::str::create_string({ "_REQUIRE_WITH_MSG(", #_a_matcher, ",", #_a_msg_on_failure,")" })\
	)
#define _INTERNAL_STATIC_MATCHER(_a_assertion_type, _a_msg,\
		_a_str_representation)\
	create_static_assertion<_a_assertion_type>(\
		_a_msg,\
		abc::reports::single_source_t(_a_str_representation, std::source_location::current()),\
		abc::global::get_this_threads_test_runner_ref())
#define _FAIL()\
	_INTERNAL_STATIC_MATCHER(\
		abc::reports::fail_t,\
		std::optional<std::string_view>(),\
		"FAIL()"\
	)
#define _TERMINATE()\
	_INTERNAL_STATIC_MATCHER(\
		abc::reports::terminate_t,\
		std::optional<std::string_view>(),\
		"_TERMINATE()"\
	)
#define _SUCCEED()\
	_INTERNAL_STATIC_MATCHER(\
		abc::reports::pass_t,\
		std::optional<std::string_view>(),\
		"_SUCCEED()"\
	)
#define _FAIL_WITH_MSG(_a_msg)\
	_INTERNAL_STATIC_MATCHER(\
		abc::reports::fail_t,\
		std::optional<std::string_view>(_a_msg),\
		abc::utility::str::create_string({ "_FAIL_WITH_MSG(", #_a_msg,")" })\
	)
#define _TERMINATE_WITH_MSG(_a_msg)\
	_INTERNAL_STATIC_MATCHER(\
		abc::reports::terminate_t,\
		std::optional<std::string_view>(_a_msg),\
		abc::utility::str::create_string({ "_TERMINATE_WITH_MSG(", #_a_msg,")" })\
	)
#define _SUCCEED_WITH_MSG(_a_msg)\
	_INTERNAL_STATIC_MATCHER(\
		abc::reports::pass_t,\
		std::optional<std::string_view>(_a_msg),\
		abc::utility::str::create_string({ "_SUCCEED_WITH_MSG(", #_a_msg,")" })\
	)

#define _SOURCE(_a_line) \
	abc::global::get_this_threads_test_runner_ref().register_source(std::source_location::current(), #_a_line,#_a_line);\
	_a_line

#define _MATCHER(_a_matcher)\
	abc::matcher_t(_a_matcher,\
		abc::reports::single_source_t(\
			abc::utility::str::create_string({"_MATCHER(",#_a_matcher,")"}),\
			std::source_location::current()))
		
#define _IF_OR_STATEMENT(_a_matcher)\
	if (_a_matcher.or_statement(\
	abc::reports::single_source_t(\
		abc::utility::str::create_string({"_IF_OR_STATEMENT(",#_a_matcher,")"}),\
		std::source_location::current())))

#define _IF_AND_STATEMENT(_a_matcher)\
	if (_a_matcher.and_statement(\
	abc::reports::single_source_t(\
		abc::utility::str::create_string({"_IF_OR_STATEMENT(",#_a_matcher,")"}),\
		std::source_location::current())))

#define _PROCESS(_a_matcher,_a_matcher_to_add_as_expression)\
	_a_matcher.process(_a_matcher_to_add_as_expression,\
		abc::reports::single_source_t(\
		abc::utility::str::create_string({ "_PROCESS(",#_a_matcher,",",#_a_matcher_to_add_as_expression,")" }), \
		std::source_location::current()));

#define _INNER_BEGIN_BLOCK(_a_name,_a_description,_a_assertion_type, _a_str_representation) {\
	abc::test_block_t<_a_assertion_type> _a_name(_a_description,\
		reports::single_source_t(_a_str_representation,std::source_location::current()));

#define _INNER_END_BLOCK(_a_name, _a_str_representation)\
	_a_name.register_end(\
		abc::reports::single_source_t(_a_str_representation,\
		std::source_location::current()));\
	abc::create_assertion_block(_a_name, \
		abc::global::get_this_threads_test_runner_ref());\
	}

#define _BEGIN_CHECK_ASSERTION_BLOCK(_a_name, _a_description)\
_INNER_BEGIN_BLOCK(_a_name, _a_description, abc::reports::pass_or_fail_t, \
	abc::utility::str::create_string({ \
	"_BEGIN_CHECK_ASSERTION_BLOCK(",#_a_name,")" }));

#define _END_CHECK_ASSERTION_BLOCK(_a_name)\
	_INNER_END_BLOCK(_a_name,\
		abc::utility::str::create_string(\
			{"_END_CHECK_ASSERTION_BLOCK(", #_a_name,")"}))

#define _BEGIN_REQUIRE_ASSERTION_BLOCK(_a_name, _a_description)\
_INNER_BEGIN_BLOCK(_a_name, _a_description, abc::matcher_t, abc::reports::pass_or_terminate_t, \
	abc::utility::str::create_string({ \
	"_BEGIN_CHECK_ASSERTION_BLOCK(",#_a_name,")" }));

#define _END_REQUIRE_ASSERTION_BLOCK(_a_name)\
	_INNER_END_BLOCK(_a_name,\
		abc::utility::str::create_string(\
			{"_END_REQUIRE_ASSERTION_BLOCK(", #_a_name,")"}))

_BEGIN_ABC_NS
template<
	typename T
>
	requires std::derived_from<T, reports::dynamic_status_t>
__constexpr
bool
create_assertion(
	const matcher_t & _a_matcher,
	const reports::single_source_t & _a_source,
	test_runner_t & _a_test_runner
) noexcept(std::same_as<T, reports::pass_or_fail_t>);
template<
	typename T
>
	requires std::derived_from<T, reports::dynamic_status_t>
__constexpr
bool
create_assertion(
	const matcher_with_annotation_t& _a_matcher,
	const reports::single_source_t & _a_source,
	test_runner_t & _a_test_runner
) noexcept(std::same_as<T, reports::pass_or_fail_t>);
template<
	typename T
>
requires std::derived_from<T,reports::static_status_t>
__constexpr
bool
create_static_assertion(
	const std::optional<std::string_view>& _a_str_to_print,
	const reports::single_source_t& _a_source,
	test_runner_t& _a_test_runner
) noexcept(not std::same_as<T, reports::terminate_t>);
template<
	typename T
>
	requires std::derived_from<T, reports::dynamic_status_t>
__constexpr
void
create_assertion_block(
	const test_block_t<T>&_a_test_block,
	test_runner_t & _a_test_runner
) noexcept(std::same_as<T, reports::pass_or_fail_t>);
namespace
{
	template<
		typename T
	>
	__constexpr
		bool
		return_result(
			const bool _a_pass
		) noexcept(std::same_as<T,reports::pass_or_fail_t>);
	template<
		typename T
	>
		requires std::derived_from<T, reports::dynamic_status_t>
	__constexpr
		bool
		create_assertion(
			const matcher_t& _a_matcher,
			const std::optional<std::string_view>& _a_str_to_print,
			const reports::single_source_t& _a_source,
			test_runner_t& _a_test_runner
		) noexcept(std::same_as<T, reports::pass_or_fail_t>);
}
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename T
>
	requires std::derived_from<T, reports::dynamic_status_t>
__constexpr_imp
bool
create_assertion(
	const matcher_t& _a_matcher,
	const reports::single_source_t& _a_source,
	test_runner_t& _a_test_runner
) noexcept(std::same_as<T, reports::pass_or_fail_t>)
{
	using namespace std;
	return create_assertion<T>(_a_matcher, optional<string>{}, _a_source, _a_test_runner);
}
template<
	typename T
>
	requires std::derived_from<T, reports::dynamic_status_t>
__constexpr_imp
bool
create_assertion(
	const matcher_with_annotation_t& _a_matcher,
	const reports::single_source_t& _a_source,
	test_runner_t& _a_test_runner
) noexcept(std::same_as<T, reports::pass_or_fail_t>)
{
	using namespace std;
	return create_assertion<T>(_a_matcher.matcher(), 
		optional<string>{_a_matcher.annotation()}, _a_source, _a_test_runner);
}
template<
	typename T
>
	requires std::derived_from<T, reports::static_status_t>
__constexpr_imp
bool
create_static_assertion(
	const std::optional<std::string_view>& _a_str_to_print,
	const reports::single_source_t& _a_source,
	test_runner_t& _a_test_runner
) noexcept(not std::same_as<T, reports::terminate_t>)
{
	using namespace std;
	using namespace reports;
	using namespace errors;
	const generic_assertion_t<true, T>* _l_gur{
		new static_assertion_t<T>(_a_source,
			_a_test_runner.get_log_infos(false),
			_a_str_to_print
			)
	};
	_a_test_runner.add_assertion(_l_gur);
	if constexpr (same_as < T, pass_t>)
	{
		return true;
	}
	else if constexpr (same_as < T, fail_t>)
	{
		return false;
	}
	else if constexpr (same_as<T, terminate_t>)
	{
		throw test_assertion_exception_t{};
	}
	else
	{
		static_assert(false);
	}
}
template<
	typename T
>
	requires std::derived_from<T, reports::dynamic_status_t>
__constexpr_imp
void
create_assertion_block(
	const test_block_t<T>& _a_test_block,
	test_runner_t& _a_test_runner
) noexcept(std::same_as<T, reports::pass_or_fail_t>)
{
	using namespace reports;
	const generic_assertion_t<false, T>* _l_gur;
	bool _l_passed{ true };
	if (_a_test_block.matcher().internal_matcher() == nullptr)
	{
		_l_gur = new matcher_based_assertion_block_t<T>(_a_test_block.source(),
			_a_test_runner.get_log_infos(false),
			_a_test_block.test_annotation(),
			matcher_result_t(),
			matcher_source_map_t(),
			_a_test_block.matcher_annotation()
		);
		_a_test_runner.add_assertion_and_warning(_l_gur,
			"Matcher_t object has not been initialised. Assertion is set to true");
	}
	else
	{
		matcher_result_t _l_mr{ _a_test_block.matcher().internal_matcher()->run_test(_a_test_runner) };
		matcher_source_map_t _l_msm;
		_a_test_block.matcher().internal_matcher()->gather_map_source(_l_msm);
		_l_passed = _l_mr.passed();
		_l_gur = new matcher_based_assertion_block_t<T>(_a_test_block.source(),
			_a_test_runner.get_log_infos(false),
			_a_test_block.test_annotation(),
			_l_mr,
			_l_msm,
			_a_test_block.matcher_annotation()
		);
		_a_test_runner.add_assertion(_l_gur);
	}
	return_result<T>(_l_passed);
}
template<
	typename T
>
	requires std::derived_from<T, reports::dynamic_status_t>
__constexpr_imp
void
matcher_based_assertion_block(
	const matcher_t& _a_matcher,
	const std::optional<std::string_view>& _a_str_to_print,
	const reports::single_source_t& _a_source,
	test_runner_t& _a_test_runner
) noexcept(std::same_as<T, reports::pass_or_fail_t>)
{
	using namespace reports;
	const generic_assertion_t<true, T>* _l_gur;
	bool _l_passed{ true };
	if (_a_matcher.internal_matcher() == nullptr)
	{
		_l_gur = new matcher_based_assertion_block_t<T>(_a_source,
			_a_test_runner.get_log_infos(false),
			_a_str_to_print,
			matcher_result_t(),
			matcher_source_map_t()
		);
		_a_test_runner.add_assertion_and_warning(_l_gur,
			"Matcher_t object has not been initialised. Assertion is set to true");
	}
	else
	{
		matcher_result_t _l_mr{ _a_matcher.internal_matcher()->run_test(_a_test_runner) };
		matcher_source_map_t _l_msm;
		_a_matcher.internal_matcher()->gather_map_source(_l_msm);
		_l_passed = _l_mr.passed();
		_l_gur = new matcher_based_assertion_t<T>(_a_source,
			_a_test_runner.get_log_infos(false),
			_a_str_to_print,
			_l_mr,
			_l_msm
		);
		_a_test_runner.add_assertion(_l_gur);
	}
	return return_result<T>(_l_passed);
}
namespace
{
	template<
		typename T
	>
	__constexpr_imp
		bool
		return_result(
			const bool _a_pass
		) noexcept(std::same_as<T,reports::pass_or_fail_t>)
	{
		using namespace std;
		using namespace errors;
		using namespace reports;
		if constexpr (same_as < T, pass_or_terminate_t>)
		{
			if (not _a_pass)
			{
				throw test_assertion_exception_t{};
			}
			return _a_pass;
		}
		else if constexpr (same_as < T, pass_or_fail_t>)
		{
			return _a_pass;
		}
		else
		{
			static_assert(false);
		}
	}
	template<
		typename T
	>
		requires std::derived_from<T, reports::dynamic_status_t>
	__constexpr_imp
	bool
		create_assertion(
			const matcher_t& _a_matcher,
			const std::optional<std::string_view>& _a_matcher_label,
			const reports::single_source_t& _a_source,
			test_runner_t& _a_test_runner
		) noexcept(std::same_as<T, reports::pass_or_fail_t>)
	{
		using namespace reports;
		const generic_assertion_t<true, T>* _l_gur;
		bool _l_passed{ true };
		if (_a_matcher.internal_matcher() == nullptr)
		{
			_l_gur = new matcher_based_assertion_t<T>(_a_source,
				_a_test_runner.get_log_infos(false),
				matcher_result_t(),
				matcher_source_map_t(),
				_a_matcher_label
			);
			_a_test_runner.add_assertion_and_warning(_l_gur,
				"Matcher_t object has not been initialised. Assertion is set to true");
		}
		else
		{
			matcher_result_t _l_mr{ _a_matcher.internal_matcher()->run_test(_a_test_runner) };
			matcher_source_map_t _l_msm;
			_a_matcher.internal_matcher()->gather_map_source(_l_msm);
			_l_passed = _l_mr.passed();
			_l_gur = new matcher_based_assertion_t<T>(_a_source,
				_a_test_runner.get_log_infos(false),
				_l_mr,
				_l_msm,
				_a_matcher_label
			);
			_a_test_runner.add_assertion(_l_gur);
		}
		return return_result<T>(_l_passed);
	}
}
/*template<
	typename T
>
	requires std::derived_from<T, reporters::dynamic_control_flow_disruption_t>
__constexpr_imp
	bool
	create_assertion(
		const std::string_view _a_str_representation_of_line,
		const std::source_location& _a_source_location,
		const matcher_t& _a_matcher,
		test_runner_t& _a_test_runner
	)
{
	using namespace std;
	using namespace reporters;
	using namespace errors;
	generic_matcher_t* _l_ptr{ _a_matcher.internal_matcher().get() };
	matcher_source_map_t _l_msm;
	matcher_result_t _l_mr;
	if (_l_ptr != nullptr)
	{
		_l_mr =  _l_ptr->run_test(_a_test_runner);
		_l_ptr->gather_map_source(_l_msm);
	}
	else
	{
		_a_test_runner.add_mid_execution_test_warning(
			new malformed_matcher_warning_t(
				_a_str_representation_of_line,
				_a_source_location
			));
	}
	_a_test_runner.add_function_report(
		new assertion_t<T>(single_source_t(_a_source_location,
			_a_str_representation_of_line),
			_l_msm, _l_mr));
	if constexpr (same_as<T, pass_or_fail_and_terminate_t>)
	{
		if (_l_mr.passed())
		{
			return true;
		}
		else
		{
			throw test_assertion_exception_t();
		}
	}
	else
	{
		return _l_mr.passed();
	}
}

__constexpr_imp
	void
	register_begin_block(
		const std::source_location& _a_source_location,
		const std::string_view _a_str_representation,
		test_runner_t& _a_test_runner
	) noexcept
{
	_a_test_runner.register_block_begin(
		reporters::block_begin_t(_a_source_location,_a_str_representation));
}*/
/*namespace
{
	template<
		typename T,
		bool Contains_Message
	>
		requires std::derived_from<T, reporters::dynamic_control_flow_disruption_t>
	__constexpr_imp
		bool
		create_blank_assertion(
			const reporters::manual_report_t<T, Contains_Message>& _a_ma,
			test_runner_t& _a_test_runner
		) noexcept(std::same_as<reporters::pass_or_fail_t>)
	{
		using namespace std;
		using namespace reporters;
		using namespace errors;
		_a_test_runner.add_function_report(_a_ma);
		if constexpr (same_as<T, terminate_t>)
		{
			throw test_assertion_exception_t();
		}
		else if constexpr (same_as<T, pass_t>)
		{
			return true;
		}
		else if constexpr (same_as<T, fail_t>)
		{
			return false;
		}
		else
		{
			static_assert(false);
		}
	}
	__constexpr_imp
		std::pair<matcher_result_t, matcher_source_map_t>
		get_matcher_result_and_map(
			test_runner_t& _a_test_runner,
			const matcher_t& _a_matcher
		) noexcept
	{
		generic_matcher_t* _l_ptr{ _a_matcher.internal_matcher().get() };
		matcher_source_map_t _l_msm;
		matcher_result_t _l_mr;
		if (_l_ptr != nullptr)
		{
			_l_mr = _l_ptr->run_test(_a_test_runner);
			_l_ptr->gather_map_source(_l_msm);
		}
		else
		{
			std::cout << "exiting" << std::endl;
			//std::exit(-1854);
			//_a_test_runner.add_mid_execution_test_warning(
			//	new malformed_matcher_warning_t(
			//		_a_str_representation_of_line,
			//		_a_source_location
			//	));
		}
		return { _l_mr,_l_msm };
	}
	template<
		typename T
	>
		requires std::derived_from<T, reporters::dynamic_control_flow_disruption_t>
	__constexpr_imp
		bool
		process_termination(
			const std::source_location& _a_sl,
			test_runner_t& _a_test_runner,
			const matcher_t& _a_matcher,
			reporters::function_report_t* _a_metr
		) noexcept(std::same_as<T, reporters::pass_or_fail_t>)
	{
		using namespace std;
		using namespace reporters;
		using namespace errors;
		_a_test_runner.register_tests_most_recent_source(_a_sl);
		_a_test_runner.add_function_report(_a_metr);
		const bool _l_res{ 
			_a_matcher.internal_matcher().get() == nullptr ? false :
			_a_matcher.internal_matcher().get()->matcher_result().passed() };
		if constexpr (same_as<T, pass_or_fail_t>)
		{
			return _l_res;
		}
		else if constexpr (same_as<T, pass_or_fail_and_terminate_t>)
		{
			if (not _l_res)
			{
				throw test_assertion_exception_t();
			}
			else
			{
				return true;
			}
		}
		else
		{
			static_assert(false);
		}
	}
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
		using namespace reporters;
		generic_matcher_t* _l_ptr{ _a_matcher.internal_matcher().get()};
		matcher_source_map_t _l_msm;
		if (_l_ptr != nullptr)
		{
			const matcher_result_t& _l_mr{ _l_ptr->run_test(_a_test_runner) };
			_l_ptr->gather_map_source(_l_msm);
		}
		else
		{
		//	_a_test_runner.add_mid_execution_test_warning(
		//		new malformed_matcher_warning_t(
		//			_a_str_representation_of_line,
		//			_a_source_location
		//		));
		}
		const bool _l_passed{ _l_ptr != nullptr ? _l_ptr->matcher_result().passed() : false };
		const optional<string_view> _l_matcher_str{ _l_ptr != nullptr ? 
			optional<string_view>(_l_ptr->matcher_result().str()) : 
			optional<string_view>()};

		//Has to be like this otherwise we have to remove the constness from the string_view.
		_a_test_runner.register_tests_most_recent_source(_a_source_location);
		_a_test_runner.add_mid_execution_test_report(
			new user_defined_assertion_t(
				_l_passed,
				_a_str_representation_of_line,
				_a_source_location,
				_l_msm,
				_a_test_runner.get_log_infos(false),
				_a_terminate_function_on_failure,
				_l_matcher_str
			)
		);
		if (not _l_passed && _a_terminate_function_on_failure)
		{
			throw test_assertion_exception_t();
		}
		return _l_passed;
	}
}*/
_END_ABC_NS