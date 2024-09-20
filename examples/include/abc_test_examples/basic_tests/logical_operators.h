#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

#include "abc_test/matchers/comparison.h"

_TEST_CASE("Examples using the logical operators.", "examples::basic_assertions")
{
	using namespace abc;
	//Obviously we have the basic type
	_CHECK(eq(1, 2));
	//We can also do this
	_CHECK(eq(1, 2) && eq(2, 3));
	//Note that both not both elements are necessarily ran; due to how the logical operators work,
	//the second argument is not ran. If you have a very compuataionally expensive matcher,
	// this could be beneficial.

	//Both elements are evaluated, as its an or.
	_CHECK(eq(1, 2) || EXPR(2 == 3));

	//We can even do stuff like this. 
	_CHECK(!(eq(1, 1) && EXPR(2 == 2)));

	//I would advise the user to be aware what they are doing - if you don't want to evaluate 
	// things, consider using the example shown in basic_tests/assertions.h. Or,
	// use the mechanisms shown below.
}

_TEST_CASE("A multi-line assertion", "examples::basic_assertions")
{
	using namespace abc;
	matcher_t _l_m1{ EXPR(1 == 2) };
	_CHECK(!_l_m1);
	/*!
	* The type returned from all matchers is matcher_t. We can use declare these on one line,
	* then use them in a test on another.
	* 
	* However, when reporting the results from an assertion, the reader may want to be aware 
	* of these - due to how the macros work, the above will only print information pertaining
	* to the code in the "_CHECK".
	* 
	* You can "register" the first line to the second, as shown below.
	*/

	matcher_t _l_m2{ _MATCHER(EXPR(1 == 2)) };
	_CHECK(!_l_m2);

	/*!
	* When using multi-line assertions, try to register each line that is part of the assertion.
	* 
	* The following pattern can be used when writing a test in which one of the tests in the OR
	* comparison requires some setup which is computationally expensive.
	* 
	* Please note that, upon "or_statemenet" being called, the matcher _l_1 WILl be evaluated.
	*/
	matcher_t _l_1{ _MATCHER(EXPR(1 == 1)) };
	if (_l_1.or_statement())
	{
		// Computationally expensive statement.
		_l_1.process(EXPR(2 == 3));
	}
	/*!
	* Note that _MATCHER isn't needed for process to store the nrelevant expression
	*/
	_CHECK(!_l_1);
	//This also works with ands.
	matcher_t _l_2{ _MATCHER(EXPR(1 == 1)) };
	if (_l_2.and_statement())
	{
		// Computationally expensive statement.
		_l_2.process(EXPR(2 == 3));
	}
	_CHECK(_l_2);
	//There is no function to prepare a matcher for a not function
	//, as there is no requirement to.

	//These also work with require statements.
	//_REQUIRE(_l_2);

	//Just as an aside, nothing different will happen when the following is done.
	_CHECK(_MATCHER(EXPR(1 == 2)));
}

_TEST_CASE("Ill-formed multi-line assertions", "examples::basic_assertions")
{
	using namespace abc;
	/*!
	* The multi-line assertion support can be very useful, but is very easy to forgot, or get
	* wrong. Here we discuss what happens when you get things wrong.
	*/

	/*!
	* Here is an or statement which doesn't have a right arm.
	*/
	matcher_t _l_1{ _MATCHER(EXPR(1 == 2)) };
	if (_l_1.or_statement())
	{

	}
	/*!
	* What happens is, when the code is ran, a warning is omitted to the user informing
	* them that a binary logical statmeent has been created, but it contains no right-child.
	* 
	* Note that if _l_1 was initiallys et to EXPR(1==1) (a true statement) this would not
	* throw an error, however a warning would still be omitted.	
	*/
	_CHECK(_l_1);

	/*!
	* The same error is reported if the following is done. It is essentially the same error.
	*/
	_CHECK(EXPR(1 == 2) || matcher_t());

	/*!
	* A different type of error is reported if the following is used. 
	*/
	_CHECK(matcher_t() || EXPR(1 == 2));

	/*!
	* Here are some other uses of logic operators which are ill-formed.
	*/
	_CHECK(! matcher_t());

	_CHECK(matcher_t());
	/*!
	* The same thing happens with an ill-formed and statement.
	*/

	matcher_t _l_2{ _MATCHER(EXPR(1 == 1)) };
	if (_l_2.and_statement())
	{
	}
	_CHECK(_l_2);
	/*!
	* What about something more complicated - but also wrong.
	*/
	_l_2 = matcher_t{ _MATCHER(EXPR(1 == 1)) };
	if (_l_2.and_statement())
	{
		matcher_t _l_3(_MATCHER(EXPR(2 == 3)));
		_l_2 = _l_2 && _l_3;
	}
	/*!
	* While it may be pretty obvious that the expected result is that the function becomes 
	* l_2 && _l_3, this will omit a similar error as before. 
	* 
	* If X is the placeholder expected branch from the "and_statement", the assertion becomes
	* 
	* (EXPR(1==2) && false) && EXPR(2==3).
	* 
	* Further more, it now becomes impossible to insert an element into the branch for l2. 
	* 
	* While the reader may think it would be easy to make it so that this expressionw would be 
	* accepted, we then get into the difficult issue of what happens if we tell the expression to
	* expect an AND, but an OR or a NOT is used instead. 
	While we could make a special case for when the operator matches the prepared
	* statement, we (currently) choose not to.
	* 
	* Like the previous examples, a warning is shown to the user.
	*/
	_CHECK(_l_2);

	/*!
	* Finally, what happens when a matcher tries to process another matcher when it has not been prepared?
	* 
	* Nothing!
	* 
	* The following code will have no effect on _l1.
	*/
	matcher_t _l_4{ _MATCHER(EXPR(1 == 2)) };
	_l_4.process(_MATCHER(EXPR(1 == 1)));

}