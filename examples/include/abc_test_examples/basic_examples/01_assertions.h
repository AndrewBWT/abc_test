#pragma once
#include "abc_test/core.h"

#include "abc_test/included_instances.h"

#include <ranges>
#include <numeric>


/*!
 * Each test case we create is written using the abc::test_data_t object. It is
 * a synonym for the user_defined_test_data_t object. We use designiated initializers
 * to initialise the object.
 * 
 * Note the abnoraml syntax; we need an extra bracket to escape the macro separating
 * via the commas in the data structure. We could move the second bracket to before the
 * abc::test_data_t, as shown in the next example. 
 */
_TEST_CASE(abc::test_data_t({
	.name = "file_01_example_01",
	.description = "A simple test case with no assertions. It will alwyas pass",
	.path = "examples::basic_examples::assertions",
	.threads_required = 2
	}))
{
	
}

/*!
 * This function shows how to use static assertions; assertions which either pass, fail
 * or terminate the function. 
 */

_TEST_CASE((abc::test_data_t{
	.name = "file_01_example_02",
	.description = "Examples of basic static assertions",
	.path = "examples::basic_examples::assertions"
	}))
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

_TEST_CASE(abc::test_data_t({
	.name = "file_01_example_03",
	.description = "Examples of static assertions with messages",
	.path = "examples::basic_examples::assertions"
	}))
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

_TEST_CASE(abc::test_data_t({
	.name = "file_01_example_04",
	.description = "Examples showing how to get the result of a static assertion",
	.path = "examples::basic_examples::assertions"
	}))
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

_TEST_CASE(abc::test_data_t({
	.name = "file_01_example_05",
	.description = "Examples of matcher-based assertions",
	.path = "examples::basic_examples::assertions"
	}))
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

_TEST_CASE(abc::test_data_t({
	.name = "file_01_example_06",
	.description = "Examples showing the simulationg of logical operators",
	.path = "examples::basic_examples::assertions"
	}))
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