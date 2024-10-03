#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

_TEST_CASE("Matchers, explained", "examples::basic_examples::02_basic_matchers")
{
	using namespace abc;
	/*!
	* In 01_assertions.h, we showed the user the core assertions in abc_test. 
	* 
	* Two of these were _CHECK and _REQUIRE (and their MSG variants).
	* 
	* Both of these macros require a matcher as their argument. A matcher is an abstract
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
	_l_1 = matcher_t(matcher_internal_ptr_t(new true_matcher_t()));
	_l_1 = matcher_t(matcher_internal_ptr_t(new false_matcher_t()));
	_REQUIRE(_l_1);
	/*!
	* Finally, the reader should note that the default matcher_t, used at the start of this
	* test function, defaults to containing a true_matcher_t object.
	*/

}

_TEST_CASE("The _SOURCE macro", "examples::basic_examples::02_basic_matchers")
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
	* However, the source location is still retained. This allows the user to see
	* the other relevant lines. 
	
	* Internally, the constructors for matcher_t check to see whether there is a 
	* source code representation associated with the same line. If there is, the matcher_t
	* is linked to that source code line. 
	*
	* The macro _SOURCE(Code) takes Code and turns it into a string, registers
	* the source line with that string, then runs Code. Due to how macros work,
	* the string is registered with the same line as Code.
	* 
	* So now, if we do this:
	*/
	_SOURCE(matcher_t _l_2;);
	_CHECK(_l_2);
	/*!
	* We can see from the output that the source is attached to the _CHECK.
	* 
	* Specifically, in the "Matcher's other sources" subheading.
	* 
	* Please note that the _SOURCE does not include any semicolons after the line.
	* 
	* We always write semicolons after our macros in this library. In this instance
	* what is in the _SOURCE macro is a line of code itself, hence we feel it 
	* is appropriate to include it. It is of course not actually required.
	* 
	* Naturally, this can be abused. An example is through the following:
	*/

	_SOURCE(matcher_t _l_3; matcher_t _l_4);
	_CHECK(_l_3);
	_CHECK(_l_4);
	/*!
	* We can see that the source registers both statements to a single line. 
	* 
	* We hope in the future it is possible to get a stringified verison of a source
	* code line without using macros. It would allow us to not rely on the user
	* remembering to use _SOURCE to tag lines.
	*/


}