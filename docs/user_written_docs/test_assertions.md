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

In `abc_test` assertions are entities provided to the user which interact with the `abc_test` framework. Every assertion provided by `abc_test` is a macro.

At runtime, an assertion registers its result with the overarching `abc` test framework. The result is either the assertion passes or the assertion fails. The user can define an assertion in such a manner so that, if it fails, it can terminate the test function early, moving to the next function in the test-suite.

# Static Assertions
Static assertions are the simplest assertions in `abc_test`. They do no runtime checks; instead they only pass, fail or fail and terminate the test. 

An overview of the three basic static assertion macros is shown below.

| Macro | Description |
|--|--|
| `_FAIL()` | Represents failure. It will register as an assertion failing in the test it is contained in. The overarching test will continue running. |
| `_TERMINATE()`| Represents failure and termination. It will register as an assertion failing in the test it is contained in. The overarching test will terminate. |
| `_SUCCEED()` | Represents success. This will register as an assertion passing in the test it is contained in.  The overarching test will continue running. |

Here is a simple example test using these assertion macros.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Simple assertions"}))
{
    //Will register a success.
    _SUCCEED();
    //Will register a failure
    _FAIL();
    //Will terminate the test.
    _TERMINATE();
    //Will never be reached.
    _SUCCEED();
}
```

We include three additional macros which take a single `string_view` argument. These act as messages, which are contained in the output from the test. The three macros are `_SUCCEED_WITH_MSG`, `_FAIL_WITH_MSG` and `_TERMINATE_WITH_MSG`. An example of their use is shown below.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Simple assertions with output"}))
{
    //Will register a success.
    _SUCCEED_WITH_MSG("This message probably won't be seen");
    //Will register a failure
    _FAIL_WITH_MSG("This one will though");
    //Will terminate the test.
    _TERMINATE_WITH_MSG("This one definitely will");
    //Will never be reached.
    _SUCCEED_WITH_MSG("This one won't");
}
```

# A Brief Introduction To Matchers
Every other assertion macro included with `abc_test` uses matchers in their construction. This section serves as a brief introduction for the user, allowing them to gain enough information to understand the examples shown in this document.

Matchers are objects in `abc_test`. 

# Single-line, matcher-based assertions
Every other matcher included in `abc_test` is a matcher based assertion

## Matchers

Matchers are closely related to assertions. A matcher is an object which performs some generic logic. Many assertions take matchers as arguments. The assertion entities then run the matcher's logic, allowing the result to be reported to the `abc` test framework.

In [this document]() we provide the user with documentation concerning matchers. For now, we will introduce a simple set of matchers, which will allow the user to understand the examples in this document.

Objects of type `matcher_t` are the types of arguments used by assertions in `abc_test`. Below we show two examples of `matcher_t` objects which will evaluate to `true` and `false` respectively.

```cpp
matcher_t true_matcher = bool_matcher(true);
matcher_t false_matcher = bool_matcher(false);
```

The following code represents more complicated `matcher_t` objects, whose results are based on how `i` compares to `j`.

```cpp
int i = 1;
int j = 2;
matcher_t m1 = _EXPR(i==j);
matcher_t m2 = _EXPR(i!=j);
matcher_t m3 = _EXPR(i < j);
matcher_t m4 = _EXPR(i >= j);
```

# Single-line, matcher-based assertions

There are two other single-line matchers in `abc_test`. One is called `_CHECK` and the other is called `_REQUIRE`. Both take a matcher as their argument. If `_CHECK`'s matcher fails, then a failed assertion is registered with the test, otherwise a passed assertion is registered with the test. If `_REQUIRE`'s matcher passes, then a passed assertion is registerd with the test. Otherwise, a failed assertion is raised, and the test function terminates. 

Here are some examples.

# Multi-line, matcher based assertions

We also include what are best described as multi-line assertions in `abc_test`. These are control structures which create some controlling object which matchers can be registered to. 

We include three types of multi-line matchers:

- single_write_multi_line_matcher. This matcher has an initial value, and can only be written to once after that. The assertion can register its matcher either when its set, or on the objects destruction. This can effect when a matcher terminates a function.
- multi_write_multi_line_matcher. This matcher has an initial value, and can be re-written to multiple times. It can again, eitehr be set to register its matcher when its set, or when the object is destroyed.
- multi_write_multi_line_counter_matcher. This matcher does the same as the above, but also keeps track of all its results. So at the end it can write things like "97/98 test assertions passed. Last assertion terminated the test." or "95/100 test assertions passed. 5 failed."

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