#pragma once

#include "abc_test/core/test_options.h"

#include "abc_test/core/reporters/test_reporter_controller.h"

#include "abc_test/core/test_runner.h"

//#include "abc_test/utility/internal/logging.h"




_BEGIN_ABC_GLOBAL_NS
	__constexpr
		test_options_t&
		setup_global_variables(
			const test_options_t& _a_options
		) noexcept;
	__constexpr
		reporters::test_reporter_controller_t&
		get_global_test_reporter_controller(
		) noexcept;
	__constexpr
		test_runner_t&
		get_this_threads_test_runner_ref(
		) noexcept;
	__constexpr
		const test_options_t&
		get_global_test_options(
		) noexcept;
	__constexpr
		test_runner_t*
		get_this_threads_test_runner_ptr(
		) noexcept;
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
_END_ABC_NS
_END_NS

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
_END_ABC_NS
_END_NS