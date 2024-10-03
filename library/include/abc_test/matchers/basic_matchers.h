#pragma once
#include "abc_test/matchers/generic_matcher.h"

_BEGIN_ABC_NS
struct true_matcher_t
	: public generic_matcher_t
{
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t& _a_test_runner
		);
};
struct false_matcher_t
	: public generic_matcher_t
{
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t& _a_test_runner
		);
};
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
matcher_result_t
true_matcher_t::run(
	test_runner_t& _a_test_runner
)
{
	return matcher_result_t(true, true, "true");
}
__constexpr_imp
matcher_result_t
false_matcher_t::run(
	test_runner_t& _a_test_runner
)
{
	return matcher_result_t(true, false, "false");
}
_END_ABC_NS