#pragma once

#include "abc_test/core/test_options.h"

#include "abc_test/core/reporters/test_reporter_controller.h"

#include "abc_test/core/test_runner.h"

/*!
* This file contains all of the functions which access and modify global variables in the
* abc_test framework.
* 
* Unless you are building ontop of abc_test, it is any users of abc_test will need any
* of the functions in this file.
* 
* In regards to why we use global variables, there are two distinct "barriers" in this
* abc_test; the test code written by users appears to not need any references to the
* testing library. In actual fact, for many of the features to work, it needs extensive 
* access to the mechanisms in abc_test. The functions in this file are used to access
* parts of the testing library that would otherwise be inaccessable.
* 
*/
//Definitions
_BEGIN_ABC_GLOBAL_NS
/*!
* Sets up the global test options (GTO). It also links the 
* global error_reporter_controller_t to the GTO. 
* 
*/
__constexpr
	test_options_t&
	setup_global_variables(
		const test_options_t& _a_options
	) noexcept;
/*!
* Gets a reference to the global test_reporter_controller_t object.
*
*/
__constexpr
	reporters::test_reporter_controller_t&
	get_global_test_reporter_controller(
	) noexcept;
/*!
* Gets a reference to this threads test_runner_t object. 
*
*/
__constexpr
	test_runner_t&
	get_this_threads_test_runner_ref(
	) noexcept;
/*!
* Gets a const reference to the global test_options_t object.
*
*/
__constexpr
	const test_options_t&
	get_global_test_options(
	) noexcept;
/*!
* Gets a pointer to this threads test_runner_t object.
* 
* This can be useful when initialising a pointer which is part of an object. The default
* constructor could then use nullptr, whereas if it was a reference this wouldn't be
* possible.
*
*/
__constexpr
	test_runner_t*
	get_this_threads_test_runner_ptr(
	) noexcept;
/*!
* Gets a reference to this threads current invoked_test_info. That is, the current
* test that is running.
* 
* TODO: Currently this thread (assuming its not a nullptr) has a current element which
* is set at the default constructor. If we want to remove the default constructor, and
* only use elements created on the heap, this function would need to be changed. 
*
*/
__constexpr
	ds::invoked_test_info_t&
	get_this_threads_current_test(
	);
__constexpr
	const test_options_t&
	get_global_test_options(
	) noexcept;
__constexpr
	const test_options_t*
	get_global_test_options_ptr(
	) noexcept;
__constexpr
	reporters::error_reporter_controller_t&
	get_global_error_reporter_controller(
	) noexcept;
namespace
{
	__constexpr
		test_options_t&
		get_inner_global_test_options(
		) noexcept;
	__constexpr
		reporters::error_reporter_controller_t&
		get_inner_global_error_reporter_controller(
		) noexcept;
}
_END_ABC_GLOBAL_NS

//Implementation
_BEGIN_ABC_GLOBAL_NS
__constexpr_imp
	test_options_t&
	setup_global_variables(
		const test_options_t& _a_options
	) noexcept
{
	using namespace reporters;
	test_options_t& _l_to{ get_inner_global_test_options() };
	_l_to = _a_options;
	error_reporter_controller_t& _l_erc{ get_inner_global_error_reporter_controller() };
	_l_erc.set_test_options(_l_to);
	return _l_to;
}
__constexpr_imp
	reporters::test_reporter_controller_t&
	get_global_test_reporter_controller(
	) noexcept
{
	using namespace reporters;
	static test_reporter_controller_t _s_gtrc(get_global_test_options());
	return _s_gtrc;
}
__constexpr_imp
	test_runner_t&
	get_this_threads_test_runner_ref(
	) noexcept
{
	thread_local test_runner_t _tl_tr(get_global_test_reporter_controller(), get_global_test_options());
	return _tl_tr;
}
__constexpr_imp
	const test_options_t&
	get_global_test_options(
	) noexcept
{
	return get_inner_global_test_options();
}
__constexpr_imp
	test_runner_t*
	get_this_threads_test_runner_ptr(
	) noexcept
{
	return &get_this_threads_test_runner_ref();
}
__constexpr_imp
	ds::invoked_test_info_t&
	get_this_threads_current_test(
	)
{
	using namespace errors;
	test_runner_t* _l_tr{ get_this_threads_test_runner_ptr()};
	if (_l_tr == nullptr)
	{
		throw test_library_exception_t("This threads test_runner_t is a nullptr");
	}
	else
	{
		return _l_tr->current_test();
	}
}
__constexpr_imp
	const test_options_t*
	get_global_test_options_ptr(
	) noexcept
{
	return &get_global_test_options();
}
__constexpr_imp
	reporters::error_reporter_controller_t&
	get_global_error_reporter_controller(
	) noexcept
{
	return get_inner_global_error_reporter_controller();
}
namespace
{
	__constexpr_imp
		test_options_t&
		get_inner_global_test_options(
		) noexcept
	{
		static test_options_t _s_to;
		return _s_to;
	}
	__constexpr_imp
		reporters::error_reporter_controller_t&
		get_inner_global_error_reporter_controller(
		) noexcept
	{
		using namespace reporters;
		static error_reporter_controller_t _s_erc;
		_s_erc.set_test_options(get_inner_global_test_options());
		return _s_erc;
	}
}
_END_ABC_GLOBAL_NS