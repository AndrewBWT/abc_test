#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions/macros.h"
#include "abc_test/core/ds/test_data/macros.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

_TEST_CASE(abc::test_data_t({
	.name = "file_03_example_01",
	.description = "Basic logical operators examples",
	.path = "examples::basic_examples::03_matchers_and_logical_operators"
	}))
{
	using namespace abc;
	/*!
	* The matcher_t object has functions written which allow it to be used with three 
	* logical operators; &&, || and !.
	* 
	* Below are some basic examples
	*/
	_CHECK(true_matcher() && false_matcher());
	_CHECK(false_matcher() || false_matcher());
	_CHECK(not true_matcher());
	/*!
	* The reader should note that, unlike the normal logical operators, both branches
	* of each binary logical oeprator are evaluated no matter what - there is no "short-
	* circuiting" to stop the second argument in an AND being evaluated if the first
	* was already false.

	* Naturall, these work with the _MATCHER macro as well.
	*/
	matcher_t _l_1(false);
	_l_1 = _MATCHER(true_matcher() && false_matcher());
	_CHECK(_l_1);
}

_TEST_CASE(abc::test_data_t({
	.name = "file_03_example_02",
	.description = "Short-circuiting logical operators",
	.path = "examples::basic_examples::03_matchers_and_logical_operators"
	}))
{
	using namespace abc;
	/*!
	* In 01_assertions.h, we showed an example of using the return value
	* from a _CHECK to initialise an if-statement. Using this, a computationally
	* expensive assertion could be avoided. 
	* 
	* While that is a valid way of doing so, we have included some additional
	* functionality to allow the user to achieve the same goal, but to have their
	* assertions show the logic statment that is being emulated.
	*
	* To this end, we now introduce two macros; these are IF_OR_STATEMENT and 
	* IF_AND_STATEMENT. Both operate on matchers, and work by calling
	* the functions matcher_t::or_statement and matcher_t::and_statement. We use
	* macros so as to reduce the possibility of writing an if statement in the form
	* if (false && _l_matcher.and_statement()).
	* 
	* When either of these macros are called, a "phony" binary logical operator is
	* created, with the unevaluated arm containing a dummy value. If a "short-circuit"
	* is available (e.g. IF_OR_STATEMENT is used on a matcher which already evaluates
	* to true) then what is in the if statement is skipped, and the resulting
	* matcher's output shows that it was an or statmenet.
	* 
	* We use the macro _PROCESS to add something to the right arm of a binary matcher
	* expression.
	*/
	matcher_t _l_1{ _MATCHER(true_matcher()) };
	_IF_OR_STATEMENT(_l_1)
	{
		//Computationaly expensive matcher which will never be evaluated.
		_PROCESS(_l_1, true_matcher() && false_matcher());
	}
	_CHECK(annotate(_l_1, "Unevaluated right branch"));
	_l_1 = _MATCHER(false_matcher());
	_IF_OR_STATEMENT(_l_1)
	{
		//Computationaly expensive matcher which will never be evaluated.
		_PROCESS(_l_1, true_matcher() && false_matcher());
	}
	_CHECK(annotate(_l_1, "Evaluated right branch"));
	_l_1 = _MATCHER(false_matcher());
	_IF_AND_STATEMENT(_l_1)
	{
		//Computationaly expensive matcher which will never be evaluated.
		_PROCESS(_l_1, true_matcher() && false_matcher());
	}
	_REQUIRE(annotate(_l_1, "This one will terminate!"));
}