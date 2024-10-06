#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

_TEST_CASE("A simple test case with no assertions. This will alwyas pass", "examples::basic_examples::assertions")
{

}

_TEST_CASE("Basic static assertions", "examples::basic_assertions")
{
	/*!
	* Static assertions are the simplest types of assertions in the library. They will alwways do one thing;
	* either succeed, fail, or terminate. Below we show these three basic assertions.
	* 
	* To be very clear, all assertions in this library require a semi-colon after them. 
	* 
	* The macros do not include semi-colons, as they are designed to be able to be used in if-statements.
	*/
	_SUCCEED();
	_FAIL();
	_TERMINATE();
}

_TEST_CASE("Static assertions with messages", "examples::basic_examples::assertions")
{
	/*!
	* Like all assertions in this library, each static assertion can have an assocaited
	* annotation with it. The user can use these to describe what is being tested.
	* 
	* Static assertions have their own macros which annotate the assetions.
	*/
	_SUCCEED_WITH_MSG("Testing success");
	_FAIL_WITH_MSG("Testing failure");
	_TERMINATE_WITH_MSG("Testing termination");
}

_TEST_CASE("Getting the result of a static assertion", "examples::basic_examples::assertions")
{
	/*!
	* All macros built around assertions in this library are, at a very basic level,
	* built around functions which return booleans. The booleans signify whether
	* the function passes or not. 
	*/
	const bool _l_b1 = _SUCCEED_WITH_MSG("This message will never be shown!");
	bool b2{ _FAIL() };
	/*!
	* You can even get the bool result from a termination instruction.
	*/
	bool b3 = _TERMINATE_WITH_MSG("This message will kill the test!");
}

_TEST_CASE("Matcher-based assertion", "examples::basic_examples::assertions")
{
	/*!
	* These are what we believe the majority of users will use when writing testing code.
	* 
	* There are two core macros; _CHECK and _REQUIRE. _CHECK will fail with a failed assertion,
	* _REQUIRE will fail and terminate the function. 
	* 
	* Each takes a "matcher" as an argument. For now, assume that the expression eq(a,b) is a matcher
	* which compares the entities a and b. 
	* 
	* 
	*/
	using namespace abc;
	/*!
	* These will pass.
	*/
	_CHECK(abc::eq(1, 1));
	_REQUIRE(eq(1, 1));
	/*!
	* This will fail.
	*/
	_CHECK(abc::eq(1, 2));
	/*!
	* To annotate matchers, there is a special function.
	* 
	* There are two implementations, so either of these is fine.
	*/
	_CHECK(annotate("Checking equality",
		eq(1, 2)));
	_CHECK(annotate(eq(1, 2), "Checking for equality"));
	/*!
	* We can also get the results of checks and requires.
	*/
	_REQUIRE(eq(1, 1));
	bool b1 = _CHECK(annotate(eq(1, 1), "The numbers aren't the same"));
	_REQUIRE(annotate(eq(1, 1), "The numbers aren't the same"));
}

_TEST_CASE("Example of simulating logical operators", "examples::basic_examples::assertions")
{
	using namespace abc;
	using namespace std;
	/*!
	* For this example, assume that the function ab::contains(R,i) checks if i is in R.
	* 
	* By using the results of a _CHECK, we can ensure that a computationally expensive
	* _CHECK is skipped if the first argument is true.
	* 
	* This is done to simulate an OR statement.
	*/
	vector<int> _l_vect(1000);
	std::ranges::iota(_l_vect, 0);
	bool _l_result{ false };
	if (_l_result |= _CHECK(eq(1, 2)))
	{
		/*!
		* This code is more computationally expensive to run, and should never be hit.
		* 
		* We use the function abc::matchers::contains from matchers/ranges to run this code.
		* 
		*/
		_l_result |= _CHECK(abc::contains(_l_vect, 1000));
	}
}