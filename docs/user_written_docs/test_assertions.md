# Test Assertions
This page contains documentation concerning assertions in `abc_test`. It covers the following topics:

- What are assertions?
- Static Assertions.
- A Brief introduction to matchers.
- Single-line, matcher-based assertions.
- Assertion blocks.
- Exception handling.
- How the Assertion Macros Work.

All of the functionality shown in this document  requires the following include directive.

```cpp
#include "abc_test/core.h"
```

# What are Assertions?

In `abc_test`, assertions are statements which check whether some user-defined condition is true or false. We say that if the condition is true, then the assertion passes, and if the condition is false, then the assertion fails. Below we show an example of an assertion which checks that the variable `i` is equal to `2`.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Assertion example"}))
{
    _CHECK(_EXPR(i == 2));
}
```

In `abc_test` assertions are created using a set of entities provided to the user. This document contains the documentation for these entities. 

There are two basic types of assertion in `abc_test`; *check-based* assertions and *termination-based* assertions. The only difference between the two assertion types is that termination-based assertions will terminate the test function they are being ran in if they fail. The previous example showed a check-based assertion. Below we show an example of a termination-based assertion.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Terminating test"}))
{
    int i = 2;

    //This assertion will fail and the test will terminate.
    _REQUIRE(_EXPR(i==1));

    //This assertion will never be ran.
    _CHECK(_EXPR(i==2));
}
```

# Static Assertions
Static assertions are the simplest assertions in `abc_test`. They do no runtime checks; instead they only pass, fail or fail and terminate the test. 

An overview of the three basic static assertion macros is shown below.

| Macro | Check-based or Termination-based | Description |
|--|--|--| 
| `_FAIL()` | Check-based. | Represents failure. It will register as an assertion failing in the test it is contained in. The overarching test will continue running. |
| `_TERMINATE()`| Termination-based. | Represents failure and termination. It will register as an assertion failing in the test it is contained in. The overarching test will terminate. |
| `_SUCCEED()` | Check-based. | Represents success. This will register as an assertion passing in the test it is contained in.  The overarching test will continue running. |

Here is a simple example test using these assertion macros.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Simple assertions"}))
{
    //Will register a success.
    _SUCCEED();
    //Will register a failure.
    _FAIL();
    //Will terminate the test.
    _TERMINATE();
    //Will never be reached.
    _SUCCEED();
}
```

We include three additional macros which take a single `string_view` argument. These act as messages, which are contained in the output from the test. The three macros are `_SUCCEED_WITH_MSG`, `_FAIL_WITH_MSG` and `_TERMINATE_WITH_MSG`. `_SUCCEED_WITH_MSG` and `_FAIL_WITH_MSG` are check-based assertions, while `_TERMINATE_WITH_MSG` is a termination-based assertion.

An example of their use is shown below.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Simple assertions with output"}))
{
    //Will register a success.
    _SUCCEED_WITH_MSG("This message will be seen");
    //Will register a failure.
    _FAIL_WITH_MSG("As will this one");
    //Will terminate the test.
    _TERMINATE_WITH_MSG("This one will too!");
    //Will never be reached.
    _SUCCEED_WITH_MSG("This one won't");
}
```

# A Brief Introduction To Matchers

Apart from those static assertions described in the previous section, every other assertion entity included with `abc_test` uses *matchers* in their construction. In `abc_test`, a matcher is a user-defined entity which performs some runtime check. It usually contains some generic logic, which allows it to be re-used across the user's test suite. For example, a matcher could be defined that checks if a `string` is empty. This matcher could then be re-used by the user whenever this check needs to be performed.

Assertions use the result of a matcher to determine whether the assertion should pass or fail. 

To understand the logic pertaining to the assertion entities described in the rest of this document, it is beneficial for the reader to have a basic understanding of what a matcher is.

Below we show some basic matcher examples.

```cpp
// matcher_t is the object which holds the matcher entity itself. All matchers have a result. A default matcher has a result of true.
matcher_t matcher_1;
// matcher_2 is a basic matcher whose result is true.
matcher_t matcher_2 = bool_matcher(true);
// matcher_3 has a result of false.
matcher_t matcher_3 = bool_matcher(false);
int i = 2;
// Expressions can be encoded in matchers using the macro _EXPR. matcher_4 matcher will return true, as it checks if i==2 (which it does).
matcher_t matcher_4 = _EXPR(i==2);
i = 8;
// matcher_5 will return false, as i <=7 is false.
matcher_t matcher_5 = _EXPR(i <= 7);
```

The code above introduces two core matcher mechanics; the `bool_matcher` and the expression-encoded matcher. The `bool_matcher` matcher object has a result that is equal to its `bool` argument. The expression-encoded matcher has a result that is equal to the comparison-based expression used as its argument. In [this document]() we provide more detailed documentation for both of these constructs.

# Single-line, matcher-based assertions

In the previous section we gave the reader a brief introduction to the matcher. In this section we introduce the two most commonly used assertion macros; the `_CHECK` assertion and the `_REQUIRE` assertion.

Below 
| Macro | Check-based or Termination-based | Description |
|--|--|--| 
| `_CHECK(matcher)` | Check-based. | If the result of the matcher is `true` the assertion passes. If false it fails. |
| `_REQUIRE(matcher)`| Termination-based. | If the result of the matcher is `true` the assertion passes. If false it fails, and the overarching test will terminate. |

Below we show some examples of the use of these macros.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Testing assertions"}))
{
    // This will register as a passed assertion.
    _CHECK(bool_matcher(true));
    // This will register as a failed assertion.
    _CHECK(bool_matcher(false));
    int i = 2;
    // This will register as a passed assertion.
    _CHECK(_EXPR(i==2));
    // This will register as a failed assertion.
    _CHECK(_EXPR(i==3));
    // This will register as a passed assertion.
    _REQUIRE(_EXPR(i<3));
    // This will register as fa failed assertion, and the test will terminate.
    _REQUIRE(_EXPR(i>=3));
    // This test won't be ran.
    _CHECK(_EXPR(i==2));
}
```

We include two other, closely related, assertions called `_CHECK_EXPR` and `_REQUIRE_EXPR`. These macros can be defined as follows:

```cpp
_CHECK_EXPR(expression) = _CHECK(_EXPR(expression));
_REQUIRE_EXPR(expression) = _REQUIRE(_EXPR(expression));
```

These are essentially used when the user wants to write expressions. The upside is that the code is clearer; the downside is that the user cannot do anything to the internal matchers.

Below is an example.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Testing assertions again"}))
{
    int i = 2;
    _CHECK_EXPR(i==2);
    _CHECK_EXPR(i==3);
    _REQUIRE_EXPR(i<3);
    _REQUIRE_EXPR(i>=3);
    _CHECK_EXPR(i==2);
}
```

# Multi-line, matcher based assertions

A multi-line assertion is a pair of macros which declare an object. This object can then have matchers attached to it.

They can have two modes:
- Pass/fail mode. The assertion block continues to take in elements. When it is destroyed, everything is sent to the controller.
- Pass/fail and terminate mode. Same as above, except it terminates at the end.

We also include what are best described as multi-line assertions in `abc_test`. These are control structures which create some controlling object which matchers can be registered to. 

We include three types of multi-line matchers:

- single_write_multi_line_matcher. This matcher has an initial value, and can only be written to once after that. The assertion can register its matcher either when its set, or on the objects destruction. This can effect when a matcher terminates a function.
- multi_write_multi_line_matcher. This matcher has an initial value, and can be re-written to multiple times. It can again, eitehr be set to register its matcher when its set, or when the object is destroyed.
- multi_write_multi_line_counter_matcher. This matcher does the same as the above, but also keeps track of all its results. So at the end it can write things like "97/98 test assertions passed. Last assertion terminated the test." or "95/100 test assertions passed. 5 failed."

```cpp
{
    _TERMINATE_BLOCK(matcher_block);
    for (i=0; i < 3; ++i)
    {
        matcher_block = _EXPR(i==2);
    }
    //Evaluated and runs on description.
}
```

# Exception handling

Unlike other testing frameworks, we have included everything required to test for assertions here.

However, this may not be obvious so we have included some macros to help the user. However, these can all be written by the user using hte tools above.

# more
In [this document]() we provde documentation about the `abc` test framework. It includes a [section]() about what thread-local and global variables should be setup for test functions written using the `abc_test` library to work correctly.

If the assertion macros are called outside of an enviornment where variables are not setup correctly, then the macro will generally have no effect. The only exception to this is if the macro wans to terminate the test function; in these circumstances, a `test_assertion_exception_t` exception will be thrown.

A concrete example of this would be:

```cpp
void main()
{
    //This code will throw an exception.
    TERMINATE();
}
```