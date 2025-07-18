#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"


#include <ranges>
#include <numeric>

_TEST_CASE(abc::test_case_t({
	.name = "file_02_example_01",
	.description = "Examples concerning matchers",
	.path = "examples::basic_examples::02_basic_matchers"
	}))
{
	using namespace abc;
	/*!
	* In 01_assertions.h, we showed the user the core assertions in abc_test. 
	* 
	* Two of these were _CHECK and _REQUIRE.
	* 
	* These macros call a function which takes either a matcher_t or an annotated_matcher_t. 
	* 
	* In this example we will concentrate on the matcher_t object.
	* 
	* A matcher is an abstract
	* object which holds an expression which is evaluated to true or false. The matcher
	* also prepares a pretty-printed string, and can store source code information
	* allowing for easy to understand printing.
	* 
	* The object which inhibits a matcher is matcher_t.
	*/
	matcher_t _l_1;
	_CHECK(_l_1);
	/*!
	* Internally, a matcher_t object holds a pointer to a generic_matcher object, which
	* is itself an abstract object. This allows the user to write their own matchers,
	* which work with the rest of the library.
	* 
	* We include several matchers with the library. Two examples of basic matchers are
	* true_matcher_t and false_matcher_t. These evaluate to true and false respectively.
	*/
	//_l_1 = matcher(new static_matcher_t<reports::pass_t>());
	//_l_1 = matcher(new static_matcher_t<reports::fail_t>());
	_REQUIRE(_l_1);
	/*!
	* For the in-built matcher types shipped with this library, we include named
	* constructors. For example we have
	*/
	_l_1 = true_matcher();
	_l_1 = false_matcher();
	_REQUIRE(_l_1);
	/*!
	* Finally, the reader should note that the default matcher_t object defaults to 
	* contianing a true_matcher_t object. It also has a constructor which
	* takes a boolean; true means initialsie with a true_matcher_t,
	* false with a false_matcher_t.
	*/
	_l_1 = matcher_t(true);
	_l_1 = matcher_t(false);
	_REQUIRE(_l_1);

}

_TEST_CASE(abc::test_case_t({
	.name = "file_02_example_02",
	.description = "Boolean-based matchers",
	.path = "examples::basic_examples::02_basic_matchers"
	}))
{
	using namespace abc;
	/*!
	* Assertions in abc_test use matchers predominantly. However, we recognise that the
	* user may not want to or need to make their own matchers. Instead, they may
	* just require an assertion which checks if a bool is true or false. To that end,
	* one of the core matchers included with abc_test is the bool_matcher_t.
	*/
	_CHECK(bool_matcher(false));
	_REQUIRE(bool_matcher(true));

	_CHECK(annotate(bool_matcher(false),"A fantastic message"));
	_CHECK(annotate(bool_matcher(true), "A second description"));

	bool b2 = true && false;
	std::string str = "This won't ever work!!";
	bool b1 = _CHECK(annotate(bool_matcher(b2), "Any bool really"));
	_REQUIRE(annotate(bool_matcher(b1), str));
}

_TEST_CASE(abc::test_case_t({
	.name = "file_02_example_03",
	.description = "Examples using the _MATCHER macro",
	.path = "examples::basic_examples::02_basic_matchers"
	}))
{
	using namespace abc;
	/*!
	* In the below test, the source code representation from the _CHECK macro is as 
	* follows:
	* \"_CHECK(_l_1);\"
	* 
	*/
	matcher_t _l_1;
	_CHECK(_l_1);
	/*!
	* 
	* One of the fields in an matcher-based assertion is called the matcher source map.
	* 
	* This is a map of all relevant sources recorded by the testing facility, pertaining
	* to the given matcher.
	* 
	* We point the user to a macro which records the source code, as well as the source
	* position, of matchers. When they are declared over multiple lines, this macro
	* can help in keeping track of all of the relevant lines of code, and points
	* the user to them in the event that an assertion fails.
	* 
	*/
	matcher_t l2 = _MATCHER(matcher_t());
	_CHECK(l2);
	/*!
	* We can also use it as follows
	*/
	l2 = _MATCHER(true_matcher());
	_CHECK(l2);
	/*!
	* While the following are allowed, they will issue warnings to the user.
	*/
	l2 = _MATCHER(_MATCHER(true_matcher()));
	_CHECK(l2);
	_CHECK(_MATCHER(l2));
}

_TEST_CASE(abc::test_case_t({
	.name = "file_02_example_04",
	.description = "Examples showing matcher's annotations",
	.path = "examples::basic_examples::02_basic_matchers"
	}))
{
	using namespace abc;
	/*!
	* Annotating an assertion can provide clarity to the reader.
	* 
	* In our library, it is designed so that the user uses the "annotate" function on
	* a matcher, before passing it to either _CHECK or _REQUIRE. 
	*/
	matcher_t _l_1;
	_CHECK(annotate(_l_1, "Some test annotation"));
	annotated_matcher_t _l_2(_l_1, "A very important test");
	_CHECK(_l_2);
	/*!
	*
	* It is our desire that those using this library use "annotate" before _CHECK and 
	* _REQUIRE. By doing this, the source code is captured correctly. In the code
	* directly above this, the line 
	* 
	* "
	matcher_with_annotation_t _l_2(_l_1, "A very important test");" is not captured.
	*
	* 
	* The _MATCHER macro can be used to alleviate this issue, in much the same way
	* as it can with a regular matcher_t. 
	*/
	matcher_t l2 = _MATCHER(matcher_t());
	_CHECK(l2);
	/*!
	* We can also use it as follows
	*/
	l2 = _MATCHER(true_matcher());
	_CHECK(l2);
	/*!
	* While the following are allowed, they will issue warnings to the user.
	*/
	l2 = _MATCHER(_MATCHER(true_matcher()));
	_CHECK(l2);
	_CHECK(_MATCHER(l2));
}