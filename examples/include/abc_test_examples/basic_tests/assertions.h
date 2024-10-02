#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

_TEST_CASE("Simple test case ,no assertions", "examples::basic_assertions")
{

}

_TEST_CASE("Simple test case ,no assertions", "examples::basic_assertions")
{
	using namespace abc;
	_CHECK(abc::eq(1, 1));
}

// This is the core macro used to define tests.
/*!
* Internall the macro declares a uniquely named test T, then created an object of type 
* abc::registered_test_data_t, with all the pertinant information such as name, source code
* location, test path and with T as an argument. When this object is created, it is
* added to a global list of tests, which are then processed by the testing suite.
* 
* Finally, then the macro begins the implementaiton of T, which is where the user provided
* code is imputted.
* 
*/
_TEST_CASE("Examples of the two core assertions", "examples::basic_assertions")
{
	using namespace abc;
	/* _CHECK and _REQUIRE are the core assertions used in test code. 
		_CHECK checks some assertion contained within, and an error is logged if it fails. 
		_REQUIRE also checks its assertion, but terminates the function if it fails.
		Both macros take an element which is of type \"generic_matcher_t\".
		matchers/comparison/constructors.h contains a function we can use for testing, \"abc::eq\".
		It tests if two elements are equal. e.g. eq(1,1) will pass, while eq(1,2) will fail.*/
	//This test will pass.
	_CHECK(abc::eq(1, 1));
	// This test will fail.
	_CHECK(abc::eq(1, 2));
	//This test will pass.
	_REQUIRE(abc::eq(1, 1));
	/*!
	* When using _CHECK or _REQUIRE, we can also get the bool result from the test.
	* 
	* As this test fails, it will still log the error. It allows us to perform more complicated
	* testing logic, if required. See the test "Simulating logical operators" for examples of
	* the use of this.
	*/
	const bool _l_result{ _CHECK(abc::eq(1,2)) };

	// This test will fail, and the test will terminate early. 
	// The following print statement will not be hit
	_REQUIRE(abc::eq(1, 2));
	fmt::println("This line will never be hit.");
}

_TEST_CASE("Simulating logical operators", "examples::basic_assertions")
{
	using namespace abc;
	using namespace std;
	//Simulating an and block.
	/*
	* This is quite simple to run, and should be pretty intuitive.
	*/
	_CHECK(eq(1, 1));
	_CHECK(eq(1, 2));
	// Simulating an OR block
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
		* See abc_test_examples/basic_tests/logical_operators.h for examples using the built-in
		* machinery for using boolean logic with the the testing framework.
		*/
		_l_result |= _CHECK(abc::contains(_l_vect, 1000));
	}
	/*!
	* Simulating a not statement.
	*/
	_CHECK(abc::contains(_l_vect, 1001));
}

_TEST_CASE("Some less often used, but useful assertions", "examples::basic_assertions")
{
	using namespace abc;
	/* 
	* _FAIL is used to log a failed assertion. This can be used in code where there is no 
	* appropriate matcher. 
	
	*/
	_FAIL_WITH_MSG("The message associated with the failure");

	/*!
	* There is also _FAIL_TERMINATE, which terminates the test if it is hit.
	*/
	_TERMINATE_WITH_MSG("If this line is hit, then the program will terminate.");
}