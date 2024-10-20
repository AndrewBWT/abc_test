#pragma once

#include <functional>
#include "abc_test/core/matchers/matcher_base.h"

#include "abc_test/core/matchers/matcher_wrapper.h"

_BEGIN_ABC_NS
using function_wrapper_internal_t = std::function<matcher_result_t()>;
struct function_wrapper_matcher_t : public matcher_base_t
{
public:
	__constexpr
		function_wrapper_matcher_t(
		) noexcept = delete;
	__no_constexpr
		function_wrapper_matcher_t(
			function_wrapper_internal_t _a_function
		) noexcept;
private:
	function_wrapper_internal_t _m_function;
	__no_constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t&
		) override final;
};
__no_constexpr_imp
matcher_t
function_wrapper(
	const function_wrapper_internal_t& _a_function
) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp
	function_wrapper_matcher_t::function_wrapper_matcher_t(
		function_wrapper_internal_t _a_function
	) noexcept
	: _m_function(_a_function)
{

}
__no_constexpr_imp
	matcher_result_t
	function_wrapper_matcher_t::run(
		test_runner_t&
)
{
	return _m_function();
}
__no_constexpr_imp
matcher_t
function_wrapper(
	const function_wrapper_internal_t& _a_function
) noexcept
{
	return matcher(new function_wrapper_matcher_t(_a_function));
}
_END_ABC_NS