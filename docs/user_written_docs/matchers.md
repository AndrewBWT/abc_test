# Matchers

This page contains documentation concerning matchers in `abc_test`. It covers the following topics:

- Matcher Basics
- Included Matchers.
- Encoding Logic Into Matchers.
- User-defined Matchers.
- The Design of Matchers.

We saw [in this document]() how to write test assertions using `abc_test`. This section will assume familiarity with assertions in `abc_test`, and how matchers interact with them.

All of the functionality shown in this document  requires the following include directive.

```cpp
#include "abc_test/core.hpp"
```

Some functionality also requires the following include directive.

```cpp
#include "abc_test/included_instances.hpp"
```

We will make it clear to the reader when this is required.

# Matcher Basics
In this section we document the basic use of matchers in `abc_test`.

## What is a Matcher?

In `abc_test`, a matcher is an abstract object which provides a uniform way of encoding testing logic for use with assertions. To be clear to the reader, matchers only contain the **result** of some already evaluated test logic. `abc_test` contains a set of functions which allow the user to encode their tests, and also provides functionality to allow the user to write their own tests.

In `abc_test` the term "matcher" can refer to one of two objects; either the `matcher_t` object or the `annotated_matcher_t` object. Internally, both are type synonyms for the `matcher_wrapper_t` object, but with different template parameters. The only difference between the two objects is that the `annotated_matcher_t` object contains a user-provided annotation represented as a `std::string`. All assertion macros in `abc_test` are designed to accept either `matcher_t` objects or `annotated_matcher_t` objects.

Internally, a matcher object holds information about the test performed. The most important of these is the Boolean `result` variable, which holds the result of the performed test. 

Below we show some simple examples of matchers in `abc_test`.

```cpp
// matcher_t is the object which holds the result of a user-defined test. A default matcher has a result value of true.
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

## Bool Matchers

In the previous subsection we showed the reader some examples of how to create matchers. One of these examples created a simple matcher whose `result` value is equal to the argument provided to the function `bool_matcher`. This is called the Boolean matcher, and is one of two core matchers provided by `abc_test`. Boolean matchers can be created using the function `bool_matcher`, which has a type signature as follows:

```cpp
matcher_t bool_matcher(const bool arg);
```

An important note about the `bool_matcher` is that it is used in the default constructor for `matcher_t` (and `annotated_matcher_t`). It is written as follows:

```cpp
matcher_t() : matcher_t(bool_matcher(true))
{}
```

From this definition, any `matcher_t` object not assigned a value will automatically pass.

## Expression-Encoded Matchers

In [this subsection]() we showed the reader some examples of how to create matchers. One of these examples created a simple matcher whose `result` value is equal to the result of a comparison-based expression. This is called the expression-encoded matcher (EEM), and is one of two core matchers provided by `abc_test`. EEMs can be created using the function macro `_EXPR`. The macro can be thought of having a type signature as follows:

```cpp
matcher_t _EXPR(expression arg)
```

The `_EXPR` macro is designed to work with `==`, `!=`, `>`, `>=`, `<` and `<=`. Internally it works as follows:

- The `_EXPR(expression)` macro is expanded to become `abc::comparison_placeholder_t() < expression`.
- The object `abc::comparison_placeholder_t()` contains no member variables. It has an overloaded `<` operator in the form `template<typename T> abc::comparison_wrapper_t<T> operator<(const comparison_placeholder_t&, T&& _a_wrapper) noexcept`. This is designed to consume the first argument of `_EXPR`'s argument `expression`, creating an object of type `abc::comparison_wrapper_t<T>`.
    - `<` was used due to its low operator precedence.
- `abc::comparison_wrapper_t<T>` has a single member variable of type `T`, created from the `T` element consumed from `expression`. `abc::comparison_wrapper_t<T>` has operator overloads for `==`, `!=`, `>`, `>=`, `<` and `<=`, which take an argument in the form `T2 arg2` and return a `matcher_t` with the result of the comparison encoded in it. Internally, of the overlaoded operators for `abc::comparison_wrapper_t` call the "default" comparison expression using the internal argument of `abc::comparison_wrapper_t<T>` and `arg` as its arguments.

For example, `_EXPR(1==2)` is expanded in the following ways:

- `_EXPR(1==2)` is expanded to `abc::comparison_placeholder_t() < 1 == 2`.
- `abc::comparison_placeholder_t() < 1` becomes `abc::comparison_wrapper_t<T>(1)`, and the expression becomes `abc::comparison_wrapper_t<T>(1) == 2`.
- `abc::comparison_wrapper_t`'s `operator==` is called, which simply evaluates `1 == 2`. The returned `matcher_t` object encodes the result of the `1==2` expression.

## _MATCHER macro

In this subsection we introduce the `_MATCHER` macro to the reader. However before we do so, we will first provide some intuition as to why the reader may want to use it.

Consider the following code. For those unfamilar with assertions, the `_CHECK` macro simply reports the `matcher_t` test result to the `abc_test` framework. 

```cpp
_CHECK(1==2);
matcher_t t1 = _EXPR(2==3);
_CHECK(t1);
```

The following shows the output produced from the above code.

```cpp
 1)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:1
     Source code representation:
       "_CHECK(_EXPR(1 == 2))"
     Matcher failed with output:
       "1 != 2"
 2)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:3
     Source code representation:
       "_CHECK(t1)"
     Matcher failed with output:
       "2 != 3"
```

However, the user may notice that for `t1`, the source information is not complete. This is because the `matcher_t` element `t1` does not have all its source information associated with it. To this end, the `_MATCHER` macro encodes source information into a `matcher_t` (or `annotated_matcher_t`) entity.

```cpp
matcher_t t1 = _MATCHER(_EXPR(2==3));
_CHECK(t1);
```

```
 1)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:2
     Source code representation:
       "_CHECK(t1)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:1
       Source code representation:
         "_MATCHER(_EXPR(2 == 3))"
     Matcher failed with output:
       "2 != 3"
```

In [this section]() we will see how logical operators can be used to compose matchers together. The `_MATCHER` macro allows us to record source information when using logical operators.

The reader should note that code in this form.

```cpp
_CHECK(_MATCHER(_EXPR(1 == 2)));
```

Will not produce an additional source code representation attributed to the matcher.

## Annotating Matchers

As we have discussed previously, there are two types of matcher in `abc_test`; objects of type `matcher_t`, and objects of type `annotated_matcher_t`. 

`annotated_matcher_t` entities have an additional `std::string` associated with them. This allows the user to specify an additional anntoation to a matcher. However entites of type `annotated_matcher_t` are not as readily used as those of `matcher_t`. 

Specifically, all functions that encode a matcher return a `matcher_t` object. The functionality of matchers which allows logical operators to be used on them are only allowed on `matcher_t` objects.

The only publicly facing way to create an `annotated_matcher_t` entity is to use the two functions `abc::annotate`. They have the following type signatures.

```cpp
annotated_matcher_t
annotate(
    const matcher_t& _a_matcher,
    const std::string_view          _a_annotation
) noexcept;

annotated_matcher_t
annotate(
    const std::string_view          _a_annotation,
    const matcher_t& _a_matcher
) noexcept;
```

They are designed in this manner so that they are the "final" part added to a matcher before it is given to an assertion macro. An example of their use is shown below.

```cpp
_CHECK(annotate(_EXPR(i <= 7), "Checking the index is within range"));
```

This produces output such as the following.

However, there are other uses for `annotated_matcher_t` elements.

The following shows a way of checking an exception type using an `annotated_matcher_t` object.

# Encoding Logic Into Matchers

In this section we show the reader how logic can be encoded into the `matcher_t` object. This can be done in one of two ways; either using the logical operators, or using a set of specially designed macros.

## Logic Operators And the _MATCHER macro

`matcher_t` has `operator` definitions for `!`, `&&` and `||` encoded in it. The functions are defined as follows:

```cpp
matcher_t operator!() const noexcept;
matcher_t operator&&(const matcher_t& _a_matcher) const noexcept;
matcher_t operator||(const matcher_t& _a_matcher) const noexcept;
```

These allow the user to build up more checks on their written code. However, the reader should note that these operators are **not** evaluated lazily. By this we mean the expression `matcher_t t1 = t2 || t3` will evaluate both `t2` and `t3` even if `t2` passes. See [this section]() if the user requires functionality that emulates lazily evaluated operators.

Internally, these operators are relatively simple; the unary operator (`!`) works by "not"ing the result of the original `matcher_t`, and adding a "!" to its text output. The binary operators work in a similar way; the Boolean result of the two `matcher_t` arguments is calculated, and a text representation of the two outputs is created.

When using these matchers with assertions, we recomend using the `_MATCHER` macro so that source code representations are retained for the assertions. Below we show some examples.

```cpp
matcher_t m1 = _MATCHER(bool_matcher(true));
matcher_t m2 = _MATCHER(bool_matcher(false));
//Will evaluate to false. Text output will be "true && false".
matcher_t m3 = _MATCHER(m1 && m2);
_CHECK(m3);
//Will evaluate to true. Text output will be "!(true && false)"
matcher_t m4 = _MATCHER(!m3);
_REQUIRE(m4);
```

## Simulating Lazily Evaluated Logic Operators

The logical operators introduced above

# User-Defined Matchers

In this section we show the reader how they can design their own matchers to use with `abc_test`. 

Each `matcher_t` entity has an internal element of type `matcher_base_t`. It is this `matcher_base_t` entity which is used to write 

# Included Matchers

In this section we provide an overview of the matchers included with `abc_test`.



* 

# The Design of Matchers

We have not written this section yet.

