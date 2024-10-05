#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/comparison.h"

#include "abc_test/core/log_test_msg.h"
#include <vector>
#include "abc_test/matchers/function_wrapper.h"

_TEST_CASE("Assertion block", "examples::basic_examples::06_manual_block_assertion")
{
	using namespace abc;
	/*!
	* A manual assertion block can be a useful tool when dealing with control flow which
	* may not be sequential; for example, when there are gotos in the code, exceptions
	* being thrown or switches which fall into different cases.
	* 
	* We include several different types of assertion blocks in this library. In 
	* this section we will concentrate on one of them to show what it can do.
	* 
	* This is the MANUAL_CHECK_ASSERTION_BLOCK. Unlike the other assertions we have
	* introduced, this has a beginning and an end using the macros 
	* _BEGIN_MANUAL_CHECK_ASSERTION_BLOCK and _END_MANUAL_CHECK_ASSERTION_BLOCK.
	* 
	* Each of these take an argument. The object which underpins these blocks is
	* given this name. Below we show its basic use
	*/
	_BEGIN_MANUAL_CHECK_ASSERTION_BLOCK(_l_mn);
	int x = 5;
	_END_MANUAL_CHECK_ASSERTION_BLOCK(_l_mn);
	/*!
	* Internally, these macros create and close a new scope. The name element (in this
	* case, _l_mn) can be accessed and edited. When the closing macro is called (
	* in this case, _END_MANUAL_CHECK_ASSERTION_BLOCK), any checks are performed, 
	* and output written to the test reproters.
	* 
	* The MANUAL_CHECK_ASSERTION_BLOCK variabee can have booleans written to it. 
	* It is initially set at true. When the _END_MANUAL_CHECK_ASSERTION_BLOCK is
	* called, if the variable is true, then the assertion passes. If false, it
	* fails. 
	* 
	* The example above succeeds. The example below fails.
	*/
	_BEGIN_MANUAL_CHECK_ASSERTION_BLOCK(_l_mn);
	_l_mn = false;
	_END_MANUAL_CHECK_ASSERTION_BLOCK(_l_mn);
	/*!
	* We can also set the message associated with the assertion block, using the function
	* "set_assertion", as shown below.
	*/
	_BEGIN_MANUAL_CHECK_ASSERTION_BLOCK(_l_mn);
	_l_mn.set_message("This is a set message");
	_l_mn = false;
	_END_MANUAL_CHECK_ASSERTION_BLOCK(_l_mn);
	/* 
	* Essentially this is all there is to assertion blocks. They are essentially named
	* blocks, which can be accessed and edited. 
	* 
	* We include four distinct assertion blocks with this library (8 macros, as
	* each has a begninning and end). These are as follows:
	* 
	* _BEGIN_MANUAL_CHECK_ASSERTION_BLOCK/_END_MANUAL_CHECK_ASSERTION_BLOCK
	* - Assertions which you can write a bool to. They either pass or fail.
	* _BEGIN_MANUAL_REQUIRE_ASSERTION_BLOCK/_END_MANUAL_REQUIRE_ASSERTION_BLOCK
	* - Assertions which you can write a bool to. They either pass or terminate.
	* _BEGIN_CHECK_ASSERTION_BLOCK/_END_CHECK_ASSERTION_BLOCK
	* - Assertions which you can write a matcher to. They either pass or fail.
	* _BEGIN_REQUIRE_ASSERTION_BLOCK/_END_REQUIRE_ASSERTION_BLOCK
	* - Assertions which you can write a matcher to. They either pass or terminate.
	* 
	* Below we show some examples of the use of these.
	*/ 
	_BEGIN_MANUAL_REQUIRE_ASSERTION_BLOCK(_l_mn);
	_l_mn.set_message("This could fail spectacularly");
	_l_mn = true;
	_END_MANUAL_REQUIRE_ASSERTION_BLOCK(_l_mn);

	_BEGIN_CHECK_ASSERTION_BLOCK(_l_mn);
	_l_mn.set_message("This will fail spectacularly");
	_l_mn = matcher_t(false_matcher());
	_END_CHECK_ASSERTION_BLOCK(_l_mn);
	/*!
	* Another example showing what happens when we use the _MATCHER macro
	*/
	_BEGIN_CHECK_ASSERTION_BLOCK(_l_mn);
	_l_mn.set_message("This will fail spectacularly");
	_l_mn = _MATCHER(matcher_t(false_matcher()));
	_END_CHECK_ASSERTION_BLOCK(_l_mn);
}

namespace testing
{
	constexpr
		int
		test_function(
			const int _l_id
		)
	{
		if (_l_id == 0)
		{
			throw std::runtime_error("A runtime errorr");
		}
		else if (_l_id == 1)
		{
			throw std::exception("The exception type");
		}
		else if (_l_id == 2)
		{
			throw int(_l_id);
		}
		else
		{
			return 0;
		}
	}
	inline
		void
		test_handling_code(
			const int _l_id
		)
	{
		using namespace abc;
		using namespace std;
		_BEGIN_CHECK_ASSERTION_BLOCK(_l_test_exception_code);
		try
		{
			int _l_rv = test_function(_l_id);
			_l_test_exception_code = _MATCHER(_EXPR(0 == _l_rv));
			_l_test_exception_code.set_message("No exception thrown");
		}
		catch (const std::runtime_error& _l_re)
		{
			_l_test_exception_code.set_message("Runtime error thrown");
			_l_test_exception_code = _MATCHER(
				(eq<string, string>(_l_re.what(),
				"The runtime error")));
		}
		catch (const std::exception& _l_e)
		{
			_l_test_exception_code.set_message("std::exception type thrown");
			_l_test_exception_code = _MATCHER(
				(eq<string, string>(_l_e.what(),
					"The exceptioniano type")));
		}
		catch (...)
		{
			_l_test_exception_code.set_message("Unkonwn exception type thrown");
			_l_test_exception_code = _MATCHER(false_matcher());
		}
		_END_CHECK_ASSERTION_BLOCK(_l_test_exception_code);
	}
}

_TEST_CASE("Example Using assertion blocks to deal with excetpions", 
	"examples::basic_examples::06_manual_block_assertion")
{
	using namespace abc;
	using namespace std;
	using namespace testing;
	/*!
	* The examples above are very simple, and the user may be wondering why
	* these are included. In the function above we show a more complicated
	* example of what assertion blocks can do, and how they can be used to
	* define fine-grained control over what outputs are given from portions of code
	* whose control flow may not be usual.
	* 
	* Below we call that function with several arguments so the user can see
	* what output is reported.
	*/
	test_handling_code(0);
	test_handling_code(1);
	test_handling_code(2);
	test_handling_code(3);
}