#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

_TEST_CASE("Basic logical operators", "examples::basic_examples::03_matchers_and_logical_operators")
{
	using namespace abc;
	/*!
	* The matcher_t object has functions written which allow it to be used with three 
	* logical operators; specifically &&, || and !.
	* 
	* Below are some examples.
	*/

}