# Test Assertions
This page contains documentation concerning assertions in `abc_test`. It covers the following topics:

- What are Assertions?
- Static Assertions.
- A Brief Introduction to Matchers.
- Single-line Assertions.
- Block-based Assertions.
- Exception Handling Using Assertions.
- How the Assertion Macros Work.

All of the functionality shown in this document  requires the following include directive.

```cpp
#include "abc_test/core.hpp"
```

Some functionality requires the following include directive.

```cpp
#include "abc_test/included_instances.hpp"
```

We will make it clear to the reader when this is required.

# What are Assertions?

In `abc_test`, assertions are entities which check whether some user-defined condition is true or false. We say that if the condition is true, then the assertion passes, and if the condition is false, then the assertion fails. Below we show an example of an assertion which checks that the variable `i` is equal to `2`.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Assertion example"}))
{
    int i = 2;
    _CHECK(_EXPR(i == 2));
}
```

In `abc_test` assertions are created using a set of macros provided to the user. This document contains the documentation for these macros. 

There are two basic types of assertion in `abc_test`; *check-based* assertions and *termination-based* assertions. The only difference between the two assertion types is that termination-based assertions, if they fail, will immediately terminate the test function they are being ran in. The previous example showed a check-based assertion. Below we show an example of a termination-based assertion.

```cpp
_TEST_CASE(abc::test_case_t({.name = "Terminating test"}))
{
    int i = 2;

    //This assertion will fail and the test will terminate.
    _REQUIRE(_EXPR(i==1));

    //This assertion will never be ran. If it were to be ran, it would pass.
    _CHECK(_EXPR(i==2));
}
```

# Static Assertions
Static assertions are the simplest assertions in `abc_test`. Unlike all other assertions in `abc_test`, they process no user-defined data; instead they only pass, fail or fail and terminate the test. 

Below we show an overview of the three basic static assertion macros.

| Macro | Check-based or Termination-based? | Description |
|--|--|--| 
| `_FAIL()` | Check. | Represents failure. It will register as an assertion failing in the test it is contained in. The overarching test will continue running. |
| `_TERMINATE()`| Termination. | Represents failure and termination. It will register as an assertion failing in the test it is contained in. The overarching test will terminate. |
| `_SUCCEED()` | Check. | Represents success. This will register as an assertion passing in the test it is contained in.  The overarching test will continue running. |

Below is a simple example test using these assertion macros.

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

There are an additional three static assertion macros included with `abc_test`. They are identical to the three static assertion macros introduced previously, except they take a `string_view` argument. This argument acts as a message which will be displayed in the output for the assertion. The three macros are `_SUCCEED_WITH_MSG`, `_FAIL_WITH_MSG` and `_TERMINATE_WITH_MSG`. They mirror  `_SUCCEED`, `_FAIL` and `_TERMINATE` respectively in their behaviour.

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

Apart from the static assertion macros described in the previous section, every other assertion macro included with `abc_test` encodes their test logic in *matchers*. In `abc_test`, a matcher is a user-defined object which performs some runtime check. It usually contains some generic logic, which allows it to be re-used across the user's test suite. For example, a matcher could be defined that checks if a `string` is empty. This matcher could then be re-used by the user whenever this check needs to be performed.

Assertion macros use the result of a matcher to determine whether the assertion should pass or fail. 

To understand the logic pertaining to the assertion macros described in the rest of this document, it is beneficial for the reader to have a basic understanding of what a matcher is.

Below we show some basic examples.

```cpp
// matcher_t is the object which holds the matcher entity itself. All matchers have a boolean result value, denoting whether they pass or fail. A default matcher has a result value of true.
matcher_t matcher_1;
// The function bool_matcher creates a matcher_t whose result is the bool argument given to bool_matcher. matcher_2 has a result value of true.
matcher_t matcher_2 = bool_matcher(true);
// matcher_3 has a result value of false.
matcher_t matcher_3 = bool_matcher(false);
int i = 2;
// Expressions can be encoded in matchers using the macro _EXPR. matcher_4 has a result value of true, as it checks if i==2 (which it does).
matcher_t matcher_4 = _EXPR(i==2);
i = 8;
// matcher_5 has a result value of false, as i <=7 is false.
matcher_t matcher_5 = _EXPR(i <= 7);
```

The code above introduces two core matchers to the reader; the `bool_matcher` and the expression-encoded matcher (EEM). The `bool_matcher` has a result that is equal to its `bool` argument. The EEM has a result that is equal to the comparison-based expression used as its argument. In [this document]() we provide detailed documentation for both of these constructs.

We will use these matchers throughout the rest of this document to aid the reader's understanding of the introduced concepts.

# Single-line Assertions

In the previous section we provided the reader with a brief introduction to matchers in `abc_test`. In this section we introduce single-line assertions (SLAs) to the reader. There are two core SLA macros included with `abc_test`; the `_CHECK` macro and the `_REQUIRE` macro. Each take a matcher as an argument. The `_CHECK` macro represents a check-based assertion, while the `_REQUIRE` macro represents a termination-based assertion. An overview of these macros is shown below.

| Macro | Check-based or Termination-based? | Description |
|--|--|--| 
| `_CHECK(matcher)` | Check. | If the result of the matcher is `true` the assertion passes. If `false` it fails. |
| `_REQUIRE(matcher)`| Termination. | If the result of the matcher is `true` the assertion passes. If `false` it fails, and the overarching test will terminate. |

Below we show an example test case which uses both of these SLA macros.

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
    // This will register as a failed assertion, and the test will terminate.
    _REQUIRE(_EXPR(i>=3));
    // This test won't be ran.
    _CHECK(_EXPR(i==2));
}
```

`abc_test` includes two other SLA macros which capture a common use-case. These are the `_CHECK_EXPR` and `_REQUIRE_EXPR` macros. They both take comparison-based expressions as their only argument. They are defined as follows:

```cpp
_CHECK_EXPR(expression) = _CHECK(_EXPR(expression));
_REQUIRE_EXPR(expression) = _REQUIRE(_EXPR(expression));
```

These macros are used as shorthand for when the user wants to write an SLA which uses an EEM in its construction. However if using them, the user is unable to access the internal `matcher_t` object.

Below we show an example test case using these macros.

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

# Block-based assertions

A block-based assertion (BBA) is a specific type of assertion in `abc_test`. The core difference when comparing BBAs to SLAs is that they allow the user to associate multiple matchers to a single assertion.

Programmatically, each BBA included with `abc_test` is represented by an object. The user first creates an instance of a BBA object, then associates matchers with that object. When the user has finished encoding their test into the BBA, they signal to the `abc` test framework that they wish to evaluate the BBA object. It is only at this point that it is determined whether the BBA passes or not. 

BBAs are designed to allow the user to encode more verbose checks into a test than a SLA allows.

## Simple Example

In this subsection we familiarise the reader with BBAs through an example; we explain how the example works,  and highlight the common components that are found in each BBA included with `abc_test`.

Below is the example we will discuss in this subsection.

```cpp
_BEGIN_SINGLE_ELEMENT_BBA(block_example, "A descriptive description");
block_example = _BLOCK_CHECK(_EXPR(1==2));
block_example = _BLOCK_CHECK(_EXPR(2==2));
_END_BBA_CHECK(block_example);
```

In the previous section we stated that, programmatically, BBAs are objects. While the user can use these objects themselves when declaring BBAs, we have also provided a set of macros which hide much of the internal logic for setting up BBAs correctly. The documentation in this section is for these macros, and we encourage the reader to use them.

Each BBA in `abc_test` contains three core components; a begin macro, an object type (usually unseen by the user), and a method of assigning matchers to the BBA. In the example above, the begin macro is `_BEGIN_SINGLE_ELEMENT_BBA`, the object type is `single_element_test_block_t`, and the method of assigning macros is through the use of the `=` operator. Each BBA also has a name; the example above shows the single-element (SE)-BBA.

BBAs also have end macros, which tell the `abc` framework to process the BBA. These have different effects on how the BBA is processed. The one used in our example, `_END_BBA_CHECK`, has the `abc` test framework analyse the BBA, and it either passes or fails. There is another end BBA macro which will either pass or fail and terminate. 

Every BBA's core begin macro takes two arguments. The first is the name of the created object which represents the BBA, and the second is an `std::string_view` entity, which represents a user-defined description for the BBA. In the example above, the name of the created `single_element_test_block_t` object is `block_example`, and the description is `"A descriptive description"`. The begin macro creates the BBA's internal object.

Each end macro takes the name of the created object as its argument. After the use of an end macro, the named object cannot be used again - internally, a closing brace is used so it goes out of scope.

A SE-BBA is designed to hold a single matcher. The user can override that matcher as many times as they want, with all previous matchers lost. When an end macro is used on a SE-BBA entity, the `abc` test framework uses the result of the SE-BBA's current matcher as the SE-BBA's result. In the case of the example above, the matcher is checked and either a pass or a fail is registered with the `abc` test framework.

There are a special set of macros available which allow a matcher to be assigned to a BBA. These macros are called BBA-matcher-elevation (BBAME) macros. The `_BLOCK_CHECK` macro, used in the example above, is one such BBAME macro.

To be clear to the reader, when a matcher is used in a BBAME macro, that matcher is evaluated at that point. The evaluation is not delayed in any form. The BBA only stores data from the matcher for future use.

In the next subsection we show the full set of BBAME macros in `abc_test`. We also encourage the reader to read the [section showing motivational examples](#motivational-examples), which contains some real-world use-cases for BBAs.

## Overview of Included Macros

In this subsection we provide documentation for all of the core BBA-related macros available in `abc_test`. The remaining non-core macros are almost identical to those introduced  in this section, except that they allow the user to modify the internal source location and representation used by the BBAs. We discuss these in greater detail in the [next subsection]().

The following table contains details about BBAME macros available in `abc_test`. Each macro takes a single argument of type `matcher_t`.

|Macro | Check-based or Termination-based? |Description |
|--|--|--|
`_BLOCK_CHECK` | Check. | Encodes a matcher argument for use with a BBA. The matcher is evaluated by this macro. |
`_BLOCK_REQUIRE`| Termination. | Encodes a matcher arguemnt for use with a BBA. The matcher argument is evaluated by this macro. If the matcher returns false, then this macro will raise a pre-emptive termination in the associated BBA. See [this subsection](#the-_block_require-macros) for more information regarding this. |

The table below shows an overview of each BBA provided by `abc_test`. It also includes each BBA's three core components; its begin macro, its object type and its method of assigning matchers to it. Each BBA begin macro requires two arguments; the first is `name`, the name of the internal BBA object to be created, and the second is `description`, an `std::string_view` object representing a user-defined description of the BBA. 

| Name |  Begin Macro | Internal Object | Assignment Method | Description |
|--|--|--|--|--|
Single-assertion BBA (SA-BBA) | `_BEGIN_SINGLE_` `ELEMENT_BBA` | `single_element_` `test_block_t` | `=` operator | Begins a BBA that can have a single matcher assigned to it. When the end macro is called on it, it will pass if its assigned matcher's result is true. It will fail otherwise. |
Multi-assertion BBA (MA-BBA) | `_BEGIN_MULTI_` `ELEMENT_BBA` | `multi_element_` `test_block_t` | `+=` operator | Begins a BBA that holds multiple matcher elements. When the end macro is called on it, it will pass if all of its assigned matcher's results are true. It will fail otherwise. |

Finally, below we show an overview of the end BBA macros. They all require a single argument; the name of the object which stores the BBA. 

| Name | Check-based or Termination-based? | Description |
|--|--|--|
`_END_BBA_CHECK` | Check. | Ends a BBA. If the BBA's internal rules state that the BBA fails, then a failure is registered with the `abc` test framework. Otherwise a pass is registered with the `abc` test framework. The object used as the argument is destroyed after this macro is used. |
`_END_BBA_REQUIRE` | Termination. | Ends a BBA. If the BBA's internal rules state that the BBA fails, then a failure is registered with the `abc` test framework and the test terminates. Otherwise a pass is registered with the `abc` test framework. The object used as the argument is destroyed after this macro is used. |

### The _BLOCK_REQUIRE Macros

The previous section contained a table showing the various BBAME macros included with `abc_test`. While we feel that the semantics regarding how the check-based macros - those with a prefix of `_BLOCK_CHECK` - interact with the BBAs has been made clear, the semantics of how the termination-based macros - those with a prefix of `_BLOCK_REQUIRE` - interact with the BBAs has not. To that end, in this subsection we explain the precise semantics of how the termination-based BBAME macros work.


When a termination-based BBAME macro is used with **any** BBA, the following occours; if the macro's matcher argument's result is true, then that result is assigned to the BBA, and the program continues normally. However, if the macro's matcher argument's result is false, then what occours is an "early termination" of the BBA; the failing matcher will be assigned to the BBA, the BBA will be registered with the `abc_test` framework immediately, and the test function will terminate.

The termination-based BBAME macros are designed to be used at points where it may not be desirable or possible for the test function to continue, even as far as to finish evaluating the BBA.

Below are some examples which show termination-based BBAME macros being used with BBAs. These examples should aid the reader in understanding their semantics.

```cpp
_BEGIN_SINGLE_ELEMENT_BBA(test1);
//This matcher will be assigned to the BBA.
test1 = _BLOCK_CHECK(_EXPR(1==2));
//But it is overridden by this matcher. After this line the test function will terminate.
test1 = _BLOCK_REQUIRE(_EXPR(2==3));
//And this matcher is never assigned to the BBA.
test1 = _BLOCK_CHECK(_EXPR(3==4));
_END_BBA_CHECK(test1);

_BEGIN_MULTI_ELEMENT_BBA(test2);
//This matcher will be assigned to the BBA.
test2 += _BLOCK_CHECK(_EXPR(1==1));
//This matcher is also assigned to the BBA. However as its result is false, the test function is immediatley terminated. In the test results, the BBA will only have 2 matchers assigned to it.
test2 += _BLOCK_REQUIRE(_EXPR(2==3));
//And this matcher is never assigned to the BBA.
test2 += _BLOCK_CHECK(_EXPR(3==4));
_END_BBA_CHECK(test2);
```

## Source-Modification BBA-Based Macros

There are a set of BBA-related macros included with `abc_test` which can be used to provide custom source code information for a BBA. In this subsection we will introduce them. However, before we do so, we will illustrate the motivation behind their inclusion in `abc_test`. Consider the following example.

```cpp
_BEGIN_SINGLE_ELEMENT_BBA(test1, "test1 description");
test1 = _BLOCK_CHECK(abc::bool_matcher(false));
_END_BBA_CHECK(test1);
```

Below is some possible output this BBA could produce. This output was created using the `text_test_reporter` with default parameters. Note that we have changed the output slightly to make it easier to read. All the output shown in this section is created in this way.

```
 1)  Single-element block-based assertion failed.
     Assertion description:
       "test1 description"
     Source block begin:
       Source location:
         ..\docs\assertion_examples.hpp:1
       Source code representation:
         "_BEGIN_SINGLE_ELEMENT_BBA(test1)"
     Source block end:
       Source location:
         ..\docs\assertion_examples.hpp:3
       Source code representation:
         "_END_BBA_CHECK(test1)"
     Matcher failed with output:
       "false"
     Source code representation:
       "_BLOCK_CHECK(bool_matcher(false))"
     Source location:
       ..\docs\assertion_examples.hpp:2
```

Let us now see what would happen if we create our own macros which internally use a BBA, as shown below.

```cpp
#define __USER_DEFINED_BBA_1_BEGIN(name)                                   \
    _BEGIN_SINGLE_ELEMENT_BBA(name, fmt::format("{} description", #name)); \
    name = _BLOCK_CHECK(abc::bool_matcher(false));

#define __USER_DEFINED_BBA_1_END(name) _END_BBA_CHECK(name)
```

If we were to use it in the following way.

```cpp
__USER_DEFINED_BBA_1_BEGIN(test1);
__USER_DEFINED_BBA_1_END(test1);
```

Then the following output could be produced.

```
 1)  Single-element block-based assertion failed.
     Assertion description:
       "test1 description"
     Source block begin:
       Source location:
         ..\docs\assertion_examples.hpp:1
       Source code representation:
         "_BEGIN_SINGLE_ELEMENT_BBA(test1)"
     Source block end:
       Source location:
         ..\docs\assertion_examples.hpp:2
       Source code representation:
         "_END_BBA_CHECK(test1)"
     Matcher failed with output:
       "false"
     Source code representation:
       "_BLOCK_CHECK(abc::bool_matcher(false))"
     Source location:
       ..\docs\assertion_examples.hpp:1
```

When reading this output, the user may be confused, as the source code representations and locations do not match the original source code's. To resolve these issues, `abc_test` includes a set of macros which allow the user to change the source code representation's associated with a BBA.

Each of these macros mirrors a "core" BBA macro introduced in [the previous section](#overview-of-included-macros). To begin, below we show a set of BBAME macros which, when they associate their matcher with a BBA, do not write any information about that matcher's source location to the BBA.

|Macro | Description |
|--|--|
`_BLOCK_CHECK_NO_SOURCE` |  This macro is identical to `_BLOCK_CHECK`, except that no source information about the matcher is written to the BBA. |
`_BLOCK_REQUIRE_NO_SOURCE`| This macro is identical to `_BLOCK_REQUIRE`, except that no source information about the matcher is written to the BBA. |

Below we show a set of BBA begin macros which allow the user to provide their own source representation. Each macro takes three arguments; a `name` which is the identifier used for the internal BBA object, an `std::string_view` of the BBA's description, and an `std::string_view` of a source representation. The source code representation given by the user replaces the one that would be generated by the original BBA begin macro.

|Macro | Description |
|--|--|
`_BEGIN_SINGLE_` `ELEMENT_BBA_CUSTOM_SOURCE` |  This macro is identical to `_BEGIN_SINGLE_ELEMENT_BBA`, except it takes an additional argument of a user-provided source representation. |
`_BEGIN_MULTI_` `ELEMENT_BBA_CUSTOM_SOURCE` |  This macro is identical to `_BEGIN_MULTI_ELEMENT_BBA`, except it takes an additional argument of a user-provided source representation. |

Below is the set of end BBA macros that allow the user to edit the representation a BBA uses for its end macro. `_END_BBA_CHECK_CUSTOM_SOURCE` and `_END_BBA_REQUIRE_CUSTOM_SOURCE` take two arguments; the first is the name of the BBA object, and the second is a `std::string_view` variable representing the source representation that the BBA will use to represent the end macro. `_END_BBA_CHECK_NO_SOURCE` and `_END_BBA_REQUIRE_NO_SOURCE` take a single argument like `_END_BBA_CHECK`; the name of the BBA object.

|Macro | Description |
|--|--|
`_END_BBA_CHECK_CUSTOM_SOURCE` |  This macro is identical to `_END_BBA_CHECK`, except it takes an additional argument of type `std::string_view`, which the BBA uses to represent its end macro. |
`_END_BBA_REQUIRE_CUSTOM_SOURCE` |  This macro is identical to `_END_BBA_REQUIRE`, except it takes an additional argument of type `std::string_view`, which the BBA uses to represent its end macro. |
`_END_BBA_CHECK_NO_SOURCE` |  This macro is identical to `_END_BBA_CHECK`, except it registers no source representation to the BBA. It is useful when writing a macro which contains both a begin and end BBA macro. |
`_END_BBA_REQUIRE_NO_SOURCE` |  This macro is identical to `_END_BBA_REQUIRE`, except it registers no source representation to the BBA. It is useful when writing a macro which contains both a begin and end BBA macro. |

Below we show `__USER_DEFINED_BBA_1_BEGIN` and `__USER_DEFINED_BBA_1_END` rewritten using the macros described above. The function `abc::utility::str::create_string` takes a list of `string`s and concatenates them together. 

```cpp
#define __USER_DEFINED_BBA_1_BEGIN(name)                \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(            \
        name,                                           \
        fmt::format("{} description", #name),           \
        abc::utility::str::create_string(               \
            {"__USER_DEFINED_BBA_1_BEGIN(", #name, ")"} \
        )                                               \
    )                                                   \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false));

#define __USER_DEFINED_BBA_1_END(name)                \
    _END_BBA_CHECK_CUSTOM_SOURCE(                     \
        name,                                         \
        abc::utility::str::create_string(             \
            {"__USER_DEFINED_BBA_1_END(", #name, ")"} \
        )                                             \
    )
```

The output from using these macros with the argument `test1` is as follows.

```
 1)  Single-element block-based assertion failed.
     Assertion description:
       "test1 description"
     Source block begin:
       Source location:
         ..\docs\assertion_examples.hpp:1
       Source code representation:
         "__USER_DEFINED_BBA_1_BEGIN(test1)"
     Source block end:
       Source location:
         ..\docs\assertion_examples.hpp:2
       Source code representation:
         "__USER_DEFINED_BBA_1_END(test1)"
     Matcher failed with output:
       "false"
```

This output now directly mirrors the code the user wrote. Below we show a macro which contains both begin and end BBA macros.

```cpp
#define __USER_DEFINED_BBA_2(name)                                             \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                                   \
        name,                                                                  \
        fmt::format("{} description", #name),                                  \
        abc::utility::str::create_string({"__USER_DEFINED_BBA_2(", #name, ")"} \
        )                                                                      \
    )                                                                          \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false));                   \
    _END_BBA_CHECK_NO_SOURCE(name)
``` 

If we call it using the following code.

```cpp
__USER_DEFINED_BBA_2(test1);
```

Then the following is an example of the output which could be produced. Note that the internal `abc` test framework is aware that there is no end source for this assertion.

```cpp
 1)  Single-element block-based assertion failed.
     Assertion description:
       "test1 description"
     Source location:
       ..\docs\assertion_examples.hpp:1
     Source code representation:
       "__USER_DEFINED_BBA_2(test1)"
     Matcher failed with output:
       "false"
```

These examples illustrate the purpose of these macros; they give the user fine-grained control over the output generated from user-created macros. The macros introduced in this subsection were used to create the included [exception handling macros](#exception-handling-using-assertions), whose documentation can be found in the next section.

## Motivational Examples

In this final subsection concerning BBAs, we show the reader useful, real-world examples of how they can be used in testing code.

Below we show some code which tests if an `std::string` can be parsed by the function `std::stoi`. The test is able to check for different types of exception, and that the parsed value `i` is `> 0`. Note that in this example we use the function `abc::annotate`, which is introduced in [the documentation for matchers](). For now, the reader can assume that `annotate` simply adds an annotation to the output.

```cpp
void
    test(
        const std::string str
    )
{
    using namespace abc;
    _BEGIN_CHECK_SINGLE_ELEMENT_BBA(
        exception_test, "Testing std::stoi function"
    )
    try
    {
        int i          = std::stoi(str);
        exception_test = _BLOCK_CHECK(annotate("Checking i > 0", _EXPR(i > 0)));
    }
    catch (std::invalid_argument const & ex)
    {
        exception_test = _BLOCK_CHECK(
            annotate("Invalid argument exception thrown", bool_matcher(false))
        );
    }
    catch (std::out_of_range const & ex)
    {
        exception_test = _BLOCK_CHECK(
            annotate("Integer out of range exception", bool_matcher(false))
        );
    }
    _END_BBA(exception_test);
}

// This serves as the testing code.
_TEST_CASE(
    abc::test_case_t({.name = "Testing stoi"})
)
{
    test("1");
    test("-1");
    test("hello");
    test("9999999999999999999999999999999999999");
}
```

The output from this code is shown below.


```
 1)  Block-based assertion passed.
     Test description:
       "Testing std::stoi function using "1""
     Source block begin:
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_BEGIN_SINGLE_ELEMENT_BBA(exception_test)"
     Source block end:
       Source location:
         ..\docs\assertion_examples.hpp:18
       Source code representation:
         "_END_BBA_CHECK(exception_test)"
     Matcher's annotation:
       "Checking i > 0"
     Matcher passed with output:
       "1 > 0"
     Source code representation:
       "_BLOCK_CHECK(annotate("Checking i > 0", _EXPR(i > 0)))"
     Source location:
       ..\docs\assertion_examples.hpp:4
 2)  Block-based assertion failed.
     Test description:
       "Testing std::stoi function using "-1""
     Source block begin:
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_BEGIN_SINGLE_ELEMENT_BBA(exception_test)"
     Source block end:
       Source location:
         ..\docs\assertion_examples.hpp:18
       Source code representation:
         "_END_BBA_CHECK(exception_test)"
     Matcher's annotation:
       "Checking i > 0"
     Matcher failed with output:
       "-1 <= 0"
     Source code representation:
       "_BLOCK_CHECK(annotate("Checking i > 0", _EXPR(i > 0)))"
     Source location:
       ..\docs\assertion_examples.hpp:4
 3)  Block-based assertion failed.
     Test description:
       "Testing std::stoi function using "hello""
     Source block begin:
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_BEGIN_SINGLE_ELEMENT_BBA(exception_test)"
     Source block end:
       Source location:
         ..\docs\assertion_examples.hpp:18
       Source code representation:
         "_END_BBA_CHECK(exception_test)"
     Matcher's annotation:
       "Invalid argument exception thrown"
     Matcher failed with output:
       "false"
     Source code representation:
       "_BLOCK_CHECK(annotate("Invalid argument exception thrown", bool_matcher(false)))"
     Source location:
       ..\docs\assertion_examples.hpp:8
 4)  Block-based assertion failed.
     Test description:
       "Testing std::stoi function using "9999999999999999999999999999999999999""
     Source block begin:
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_BEGIN_SINGLE_ELEMENT_BBA(exception_test)"
     Source block end:
       Source location:
         ..\docs\assertion_examples.hpp:18
       Source code representation:
         "_END_BBA_CHECK(exception_test)"
     Matcher's annotation:
       "Integer out of range exception"
     Matcher failed with output:
       "false"
     Source code representation:
       "_BLOCK_CHECK(annotate("Integer out of range exception", bool_matcher(false)))"
     Source location:
       ..\docs\assertion_examples.hpp:14
```



Another motivational example is shown below. In this code, we are testing our own midpoint function (represented by `f`) against `std::midpoint`. A for-loop is used to iterate over testing values, and the results are collected in a BBA.

```cpp
function<int(int,int)> f = [](int x, int y) {
    return (x+y) / 2;
}
_BEGIN_CHECK_MULTI_ELEMENT_BBA(mid_point_test, "Testing mid point function")
for (auto&& [integer1, integer2] : 
    {
        {0,1},
        {100,200},
        {500,200},
        {700,1234},
        {9223372036854775807 ,1}}
{
    mid_point_test += _BLOCK_CHECK(_EXPR(f(integer1,integer2) == std::midpoint(integer1,integer2)));
}
_END_BBA(mid_point_test);
```

In [this document about data generators]() we will show the reader many examples which use BBAs in this manner.

# Exception Handling Using Assertions

`abc_test` is designed in such a manner that there is no need to have specialised functions or macros for testing code which can throw an exception. Instead, using the BBAs introduced in the previous section, the user should be able to write their own code to do so.

However, we recognise that there are several commonly seen patterns found when testing for exceptions. To that end, we have included a set of macros which perform these common tests. They have all been created using BBAs. 

The user must use the following include directive to have access to these macros.

```cpp
#include "abc_test/included_instances.hpp"
```

In the set of exception-handling macros included with `abc_test`, they can be grouped together in three's; a check-based begin macro, a termination-based begin macro, and a single end macro.

Below we show all of the exception-handling macros included with `abc_test`. 

Every begin macro takes a single argument; a `name`, which indicates the name of the internal object which holds the logic for the test.

| Check-based Begin Macro, Termination-based Begin Macro & End Macro | End Macro Arguments | Description | 
|--|--|--|
`_BEGIN_CHECK_NO_THROW`, `_BEGIN_REQUIRE_NO_THROW`, `_END_NO_THROW` | `name`. The name of the object which holds the logic for the test. | These macros are used to check that code does not throw an exception. If it does, then the macros either fail or fail and terminate. Otherwise they pass.
`_BEGIN_CHECK_THROW_ANY`, `_BEGIN_REQUIRE_THROW_ANY`, `_END_THROW_ANY` | `name`. The name of the object which holds the logic for the test. | These macros are used to check that the code does throw an exception. The exception does not need to be derived from `std::exception`. If no exception is thrown then the macros either fail or fail and terminate. Otherwise they pass. |
`_BEGIN_CHECK_EXCEPTION_TYPE`,  `_BEGIN_REQUIRE_EXCEPTION_TYPE`, `_END_EXCEPTION_TYPE` | `name`. The name of the object which holds the logic for the test. `exception_type` The type of exception to check for. | These macros are used to check that the code throws an exception of a specific type. The exception does not need to be derived from `std::exception`. If no exception is thrown, or the exception thrown is not of the correct type (or derived from the correct type) then the macros either fail or fail and terminate. Otherwise they pass. |
`_BEGIN_CHECK_EXCEPTION_MSG`, `_BEGIN_REQUIRE_EXCEPTION_MSG`, `_END_EXCEPTION_MSG` | `name`. The name of the object which holds the logic for the test. `str` The `std::string` to check the exception's `what()` function against. | These macros are used to check that the code throws an exception which has a `what()` message that contains a specific `std::string`. The exception must be derived from `std::exception`. If no exception is thrown, or the exception's `what()` message does not contain the argument string, then the macros either fail or fail and terminate. Otherwise they pass. |
`_BEGIN_CHECK_EXCEPTION_TYPE_AND_MSG`, `_BEGIN_REQUIRE_EXCEPTION_TYPE_AND_MSG`, `_END_EXCEPTION_TYPE_AND_MSG` | `name`. The name of the object which holds the logic for the test. `exception_type` The type of exception to check for. `str` The `std::string` to check the exception's `what()` function against. | These macros are used to check that the code throws an exception which has a `what()` message that contains a specific `std::string`. The exception must be derived from `std::exception`. If no exception is thrown, or the exception's `what()` message does not contain the argument string, then the macros either fail or fail and terminate. Otherwise they pass. |



Below we show some example uses of each of the sets of macros shown above.


```cpp
//Checks that std::stoi throws no exception.
_BEGIN_CHECK_NO_THROW(t1)
int i = stoi("3");
_END_NO_THROW(t1);

//Checks that stoi does throw an exception
_BEGIN_CHECK_THROW_ANY(t2)
int i = stoi("hi");
_END_THROW_ANY(t2);

_BEGIN_CHECK_EXCEPTION_TYPE(t3, std::invalid_argument)
int i = stoi("hi");
_END_EXCEPTION_TYPE(t3);

//Checks the what() function return value for a string. Note it is only that it contains the string, not matches the string.
_BEGIN_CHECK_EXCEPTION_MSG(t4, std::invalid_argument, "sto")
int i = stoi("hi");
_END_EXCEPTION_MSG(t4);

//Checks the what() function return value for a string. Note it is only that it contains the string, not matches the string.
_BEGIN_CHECK_EXCEPTION_TYPE_AND_MSG(t5, "sto")
int i = stoi("hi");
_END_EXCEPTION_TYPE_AND_MSG(t5);
```



# How the Assertion Macros Work

Currently not written.