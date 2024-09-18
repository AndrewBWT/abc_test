#pragma once

#include <functional>
#include "abc_test/matchers/generic_matcher.h"

_BEGIN_ABC_NS
using function_wrapper_internal_t = std::function<matcher_result_t()>;
struct function_wrapper_matcher_t : public generic_matcher_t
{
public:
	__constexpr
		function_wrapper_matcher_t(
		) noexcept = delete;
	__constexpr
		function_wrapper_matcher_t(
			function_wrapper_internal_t _a_function
		) noexcept;
private:
	function_wrapper_internal_t _m_function;
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t&
		) override final;
};
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	function_wrapper_matcher_t::function_wrapper_matcher_t(
		function_wrapper_internal_t _a_function
	) noexcept
	: _m_function(_a_function)
{

}
__constexpr_imp
	matcher_result_t
	function_wrapper_matcher_t::run(
		test_runner_t&
)
{
	return _m_function();
}
_END_ABC_NS