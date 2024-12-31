# Matchers

This page contains documentation concerning matchers in `abc_test`. It covers the following topics:

- Matcher Basics
- Encoding Logic Into Matchers.
- User-defined Matchers.
- Included Matchers.
- The Design of Matchers.

This document will assume the reader is familiar with the following documents:

- [Test Assertions]()

All of the functionality shown in this document  requires the following include directive.

```cpp
#include "abc_test/core.hpp"
```

Some functionality also requires the following include directive.

```cpp
#include "abc_test/included_instances.hpp"
```

We will make it clear to the reader when this is required.

At various points in this document we will show the reader some output from `abc_test`. In all instances the output we will show has been created using the `text_test_reporter_t` object, initialized using the object's default constructor. The test suite used to create the output has been configured to show both passed and failed assertions, and passed and failed matchers. We have also ammended the output slightly to make the source locations easier to read.

# Matcher Basics

In `abc_test` a matcher is an abstract entity that provides a standardized way of encoding test logic. A matcher entity contains the result of some already-evaluated test. They are used as arguments in assertion macros, which then report the result of the user-defined test to the overarching `abc_test` framework. `abc_test` contains a set of functions which allow the user to encode their own test logic into a matcher. `abc_test` also allows the user to write their own generalized functions for encoding test logic.

In `abc_test` the term "matcher" can refer to any object of either type `matcher_t` or type `annotated_matcher_t`. Internally, both types are type synonyms for the type `matcher_wrapper_t`, but use different template parameters. The core difference between the two types is that entities of type `annotated_matcher_t` contain an additional user-provided `std::string`, which represents a matcher's *annotation*. We discuss the other differences between the two types in [this subsection](). All assertion macros in `abc_test` are designed to accept objects of either type `matcher_t` or type `annotated_matcher_t`.

Internally, a matcher object holds information about the test performed. The data held by the matcher can be summarised as follows:

- `bool result`. The result of the test logic; `true` means the test passed, `false` means the test failed.
- `std::string result_str`. A text representation of the test performed.
- `std::string annotation`. The previously mentioned optional user-provided annotation. It can be used to provide clarification on what is being tested.

Matchers have an implicit bool operator associated with them which returns the `result` status of the matcher.

We conclude this section by showing the user some code examples of matchers in `abc_test`.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Matcher examples"})
)
{
    using namespace abc;
    // matcher_t is the object which holds the result of a user-defined test. A
    // default matcher has a result value of true, and an empty result_str value.
    matcher_t matcher_1;
    _CHECK(matcher_1);
    // The function bool_matcher creates a matcher_t whose result is the bool
    // argument given to bool_matcher. matcher_2 has a result value of true, and
    // a result_str value of "true".
    matcher_t matcher_2 = bool_matcher(true);
    // matcher_3 has a result value of false and a result_str value of "false".
    matcher_t matcher_3 = bool_matcher(false);
    // Below is an example showing how to annotate a matcher.
    annotated_matcher_t annotated_matcher_2(
        annotate(matcher_3, "Testing bool_matcher(false)")
    );
    _CHECK(matcher_2);
    _CHECK(matcher_3);
    _CHECK(annotated_matcher_2);
    int i               = 2;
    // Expressions can be encoded in matchers using the macro _EXPR. matcher_4
    // has a result value of true, as it checks if i==2 (which it does). Its
    // result_str value is "i == 2".
    matcher_t matcher_4 = _EXPR(i == 2);
    i                   = 8;
    // matcher_5 has a result value of false, as i<=7 is false. Its result_str
    // value is "i >= 7", showing the failure of the original expression.
    matcher_t matcher_5 = _EXPR(i <= 7);
    _CHECK(matcher_4);
    _CHECK(matcher_5);
    // Another example of annotating a matcher. Note the annotate function is
    // different to that used previously.
    annotated_matcher_t annotated_matcher_3(
        annotate(fmt::format("Checking i <= {}", 7), matcher_5)
    );
    _CHECK(annotated_matcher_3);
}
```

Below is the text output from the test case shown above.

```
 1)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:9
     Source code representation:
       "_CHECK(matcher_1)"
     Matcher passed with output:
       "true"
 2)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:23
     Source code representation:
       "_CHECK(matcher_2)"
     Matcher passed with output:
       "true"
 3)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:24
     Source code representation:
       "_CHECK(matcher_3)"
     Matcher failed with output:
       "false"
 4)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:26
     Source code representation:
       "_CHECK(annotated_matcher_2)"
     Matcher failed with output:
       "false"
     Matcher's annotation:
       "Testing bool_matcher(false)"
 5)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:36
     Source code representation:
       "_CHECK(matcher_4)"
     Matcher passed with output:
       "2 == 2"
 6)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:37
     Source code representation:
       "_CHECK(matcher_5)"
     Matcher failed with output:
       "8 > 7"
 7)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:43
     Source code representation:
       "_CHECK(annotated_matcher_3)"
     Matcher failed with output:
       "8 > 7"
     Matcher's annotation:
       "Checking i <= 7"
```

## Bool Matchers

In the previous section we provided some example code which showed the user how to create matchers in several different ways. In some of those examples a simple function called `bool_matcher` was used to construct a matcher whose `result` value was equal to the argument provided to `bool_matcher`. Matchers created in this way are called Boolean matchers. The Boolean matcher is one of two core matchers provided by `abc_test`. Boolean matchers can be created using the function `bool_matcher`, which has the following type signature.

```cpp
matcher_t bool_matcher(const bool arg);
```

A Boolean matcher's `result_str` value is always equal to `fmt::format("{}",arg)`. 

The reader should note that the Boolean matcher created from `bool_matcher(true)` is used to initialise the variables in default constructed `matcher_t`s and `annotated_matcher_t`s. From this, it should be clear to the reader that default constructed `matcher_t` and `annotated_matcher_t` objects will always pass.

## Expression-Encoded Matchers

In a [previous section]() we provided some example code which showed the user how to create matchers in several different ways. In some of those examples a macro called `_EXPR` was used to construct a matcher whose `result` value was equal to the result of the comparison-based expression used as the argument to `_EXPR`. Matchers created using this macro are called expression-encoded matchers (EEMs). The EEM is one of the two core matchers provided by `abc_test`. In `abc_test` EEMs are usually created using the macro `_EXPR`, which can be thought of as having the following type signature.

```cpp
matcher_t _EXPR(expression arg)
```

If the reader is unfamiliar with the `_CHECK_EXPR` and `_REQUIRE_EXPR`, two assertion macros closely related to the `_EXPR` macro, we would advise them to read [this subsection in the document about assertions]().

The `_EXPR` macro is designed around the assumption that its `arg` argument is in the form `a op b`, where `a` is of type `T1`, `b` is of type `T2`, and `op` is one of `==`, `!=`, `>`, `>=`, `<` or `<=`. For the `_EXPR` macro to work properly, the expression `a op b` must be valid. By this we mean, there must be an operator function in the form `bool operator op(const T1& a, const T2& b)` which can be used. Below we show some example code which adheres to this requirement.

```cpp
struct type_y
{
    std::vector<int> is;

    inline bool
        operator==(
            const type_x& rhs
        ) const
    {
        return is.size() == 2 && is[0] == rhs.i && is[1] == rhs.j;
    }

    inline bool
        operator==(
            const type_y& rhs
        ) const
    {
        return is == rhs.is;
    }
};

template <>
struct fmt::formatter<type_y> : formatter<string_view>
{
    inline auto
        format(
            type_y               arg_y,
            format_context& _a_ctx
        ) const -> format_context::iterator
    {
        using namespace std;
        const string _l_rv{fmt::format("y {{{0}}}", arg_y.is)};
        return formatter<string_view>::format(_l_rv, _a_ctx);
    }
};

_TEST_CASE(
    abc::test_case_t({.name = "X example"})
)
{
    type_x xi{1, 2};
    type_x xj{3, 4};
    _CHECK(_EXPR(xi == xj));
    type_y yi{
        {1, 2}
    };
    type_y yj{
        {4, 5, 6}
    };
    _CHECK(_EXPR(yi == yj));
    _CHECK(_EXPR(xi == yi));
}
```

In the `matcher_t` object created by the `_EXPR` macro, the value of the `result_str` variable is constructed from a textual representation of `a`, `b` and `op`. As `op` can only be a certain number of values, its textual representation is pre-determined by `abc_test`. The textual representation of either `a` or `b` is dependant upon whether `T1` or `T2` adheres to the `fmt::formattable` concept. For an arbitrary argument `arg` of type `T`, if `T` adheres to `fmt::formattable`, then `fmt::format("{}",arg)` is used as the textual representation. If `T` does not adhere to `fmt::formattable`, then a textual representation of `"[?]"` is used to represent `arg`. Below we show output from the test case shown above. This output illustrates to the reader how the `result_str` variable is constructed.

```
 1)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:43
     Source code representation:
       "_CHECK(_EXPR(xi == xj))"
     Matcher failed with output:
       "[?] != [?]"
 2)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:50
     Source code representation:
       "_CHECK(_EXPR(yi == yj))"
     Matcher failed with output:
       "y {[1, 2]} != y {[4, 5, 6]}"
 3)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:51
     Source code representation:
       "_CHECK(_EXPR(xi == yi))"
     Matcher passed with output:
       "[?] == y {[1, 2]}"
```

We finish this subsection by discussing explicitly how the `_EXPR` macro works. The macro is expanded and evaluated as follows:

- The `_EXPR(a op b)` macro is expanded to become `abc::comparison_placeholder_t() < a op b`.
- The object `abc::comparison_placeholder_t()` contains no member variables. It has an overloaded `<` operator in the form `template<typename T> abc::comparison_wrapper_t<T> operator<(const comparison_placeholder_t&, T&& _a_wrapper) noexcept`.
- `abc::comparison_wrapper_t` is a simple object with a single member variable of type `T`, and one constructor. `abc::comparison_placeholder_t()`'s `operator<` function calls this constructor, giving it `a` as its argument.
  - The operator `<` was chosen due to its low operator precedence.
- Using the `abc::comparison_placeholder_t()`'s `operator<` function, the expression is evaluated to become `abc::comparison_wrapper_t<T1>{a} op b`.
- `abc::comparison_wrapper_t` has operator overloads for `==`, `!=`, `>`, `>=`, `<` and `<=`. All of these operators take an argument `arg2` of type `T2`, and return a `matcher_t` object with the result of the binary expression encoded in it.
- `abc::comparison_wrapper_t<T1>{a}` consumes the `op` and `b` arguments, calling the relevant operator from `abc::comparison_wrapper_t`, and returns the created `matcher_t` object in the form `abc::matcher_t{.result = a op b, ...}`. 

As an example, `_EXPR(1==2)` is evaluated as follows:

- `_EXPR(1==2)` is expanded to become `abc::comparison_placeholder_t() < 1 == 2`.
- The subexpression `abc::comparison_placeholder_t() < 1` is evaluated to become `abc::comparison_wrapper_t<T>(1)`, and the whole expression becomes `abc::comparison_wrapper_t<T>(1) == 2`.
- `abc::comparison_wrapper_t`'s `operator==` is called, which constructs a `matcher_t` object. This `matcher_t` object's `result` value is `false` (as the expression is false), and its `result_str` value is  `"1 != 2"`. Thus, the entire expression becomes `matcher_t{.result = false, .result_str = "1 != 2"}`.


## _MATCHER macro

The `_MATCHER` macro can be thought of as a function with the following type signature.

```cpp
matcher_t _MATCHER(const matcher_t& matcher);
```

The `_MATCHER` returns a copy of the input argument `matcher`, except that this copy has additional source code information encoded within it. This source code information can be seen in the test results for the assertion the matcher is assigned to.

As an example, let us revisit the test case shown at the [beginning of this section](). We saw in its output that each `_CHECK` macro had a single source attributed to it. Below we show that test case re-written using the `_MATCHER` macro.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Matcher examples, revisited"})
)
{
    using namespace abc;
    matcher_t matcher_1 = _MATCHER(matcher_t());
    _CHECK(matcher_1);
    matcher_t matcher_2 = _MATCHER(bool_matcher(true));
    matcher_t matcher_3 = _MATCHER(bool_matcher(false));
    annotated_matcher_t annotated_matcher_2(_MATCHER(
        annotate(matcher_3, "Testing bool_matcher(false)")
    ));
    _CHECK(matcher_2);
    _CHECK(matcher_3);
    _CHECK(annotated_matcher_2);
    int i               = 2;
    matcher_t matcher_4 = _MATCHER(_EXPR(i == 2));
    i                   = 8;
    matcher_t matcher_5 = _MATCHER(_EXPR(i <= 7));
    _CHECK(matcher_4);
    _CHECK(matcher_5);
    annotated_matcher_t annotated_matcher_3(_MATCHER(
        annotate(fmt::format("Checking i <= {}", 7), matcher_5)
    ));
    _CHECK(annotated_matcher_3);
}
```

Below we show the output from this rewritten test case.

```cpp
 1)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:7
     Source code representation:
       "_CHECK(matcher_1)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:6
       Source code representation:
         "_MATCHER(matcher_t())"
     Matcher passed with output:
       "true"
 2)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:15
     Source code representation:
       "_CHECK(matcher_2)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:10
       Source code representation:
         "_MATCHER(bool_matcher(true))"
     Matcher passed with output:
       "true"
 3)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:16
     Source code representation:
       "_CHECK(matcher_3)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:11
       Source code representation:
         "_MATCHER(bool_matcher(false))"
     Matcher failed with output:
       "false"
 4)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:17
     Source code representation:
       "_CHECK(annotated_matcher_2)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:11
       Source code representation:
         "_MATCHER(bool_matcher(false))"
       Source location:
         ..\docs\assertion_examples.hpp:12
       Source code representation:
         "_MATCHER(annotate(matcher_3, "Testing bool_matcher(false)"))"
     Matcher failed with output:
       "false"
     Matcher's annotation:
       "Testing bool_matcher(false)"
 5)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:22
     Source code representation:
       "_CHECK(matcher_4)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:19
       Source code representation:
         "_MATCHER(_EXPR(i == 2))"
     Matcher passed with output:
       "2 == 2"
 6)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:23
     Source code representation:
       "_CHECK(matcher_5)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:21
       Source code representation:
         "_MATCHER(_EXPR(i <= 7))"
     Matcher failed with output:
       "8 > 7"
 7)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:27
     Source code representation:
       "_CHECK(annotated_matcher_3)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:21
       Source code representation:
         "_MATCHER(_EXPR(i <= 7))"
       Source location:
         ..\docs\assertion_examples.hpp:24
       Source code representation:
         "_MATCHER(annotate(fmt::format("Checking i <= {}", 7), matcher_5))"
     Matcher failed with output:
       "8 > 7"
     Matcher's annotation:
       "Checking i <= 7"
```

The reader can clearly see that by using the `_MATCHER` macro, the output shows all of each matcher's source code information. This information can be invaluable when trying to diagnose why a test case has failed, especially in cases where the control flow of a program is not immediately clear. We will also see in the [next section]() how the `_MATCHER` macro is useful when encoding Boolean logic into matchers.

As a final point, the reader should note that code in this form.

```cpp
_CHECK(_MATCHER(_EXPR(1 == 2)));
```

Will not produce an additional source code representation attributed to the matcher. The following output is instead produced.

```
 1)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:1
     Source code representation:
       "_CHECK(_MATCHER(_EXPR(1 == 2)))"
     Matcher failed with output:
       "1 != 2"
```

## Annotating Matchers

In this subsection we provide the complete documentation concerning the annotation of matchers. This includes documentation for the object `annotated_matcher_t` and the function `abc::annotate`. 

As we discussed [at the start of this section](), internally the matcher objects `matcher_t` and `annotated_matcher_t` are almost identical type synonyms for the same object. This object is `template<bool Annotated> abc::matcher_wrapper_t`. The template parameter `Annotated` is used to control whether instances of `matcher_wrapper_t` include an `std::string` member variable called `annotation`; if `true` they do, if `false` they do not. Below we show the type synonyms for `matcher_t` and `annotated_matcher_t`.

```cpp
using annotated_matcher_t = abc::matcher_wrapper_t<true>;
using matcher_t = abc::matcher_wrapper_t<false>;
```

Earlier we stated that the core difference between the two matcher types is that `annotated_matcher_t` has an additional member variable, as shown above. However this is not the only difference. `abc_test` constrains how the user is able to use `annotated_matcher_t` objects. The only way to create `annotated_matcher_t` objects is through using the two `abc::annotate` functions. These have the following type signatures. 

```cpp
annotated_matcher_t annotate(const matcher_t& _a_matcher, const std::string_view _a_annotation) noexcept;
annotated_matcher_t annotate(const std::string_view _a_annotation, const matcher_t& _a_matcher) noexcept;
```

An example of their use is shown in the test case below.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Annotation examples"})
)
{
    using namespace abc;
    matcher_t           m1  = _MATCHER(_EXPR(2 <= 3));
    annotated_matcher_t am1 = _MATCHER(annotate("Annotating 1", m1));
    annotated_matcher_t am2 = _MATCHER(annotate(m1, "Annotating 2"));
    _CHECK(am1);
    _CHECK(am2);
    _CHECK(annotate("Annotating a final time", _EXPR(2 <= 3)));
}
```

There are no constructors available for the `annotated_matcher_t` object - other than the default copy and move constructors - and no default constructor. 

In the [next section]() we will see how `matcher_t` objects can be composed together using the logical operators `operator&&`, `operator||` and `operator!`. `annotated_matcher_t` objects cannot be composed using these operators. The macros introduced in [this section]() - `_IF_OR_STATEMENT`, `_IF_AND_STATEMENT` and `_PROCESS` - allow the user to simulate lazily evaluated logical operators. These are not defined for `annotated_matcher_t` objects.

The reader may question why `abc_test` constrains the use of annotations so much. We wanted to encourage the user to think carefully about when to use an annotation. By having only one way of constructing an annotated matcher, it was felt that the user would have to be deliberate in creating such entities. In addition to this, we felt that adding annotations to groups of matchers - which would be required if we allowed `annotated_matcher_t`s to be used with the logical operators introduced in the next section - could become confusing for the user. For example, if we had a large number of annotated matchers composed together using logical operators, it may become difficult to discern which matcher an annotation was assigned to.

# Encoding Logic Into Matchers

In this section we show the reader how logic can be encoded into `matcher_t` objects using logical operators and specially designed macros. 

The reader should note that none of the functionality discussed in this section will work with objects of type `annotated_matcher_t`.

## Logical Operators

The `matcher_t` object has `operator` definitions for `!`, `&&` and `||`. The operators are defined as follows:

```cpp
matcher_t operator!() const noexcept;
matcher_t operator&&(const matcher_t& _a_matcher) const noexcept;
matcher_t operator||(const matcher_t& _a_matcher) const noexcept;
```

These operators can be used to encode verbose test logic into a single `matcher_t` object. The reader should note that the `matcher_t` objects created by the operators which take two arguments are not evaluated lazily. For example the expression `_EXPR(1 < 2) || _EXPR(2 < 3)` will always evaluate both subexpressions `_EXPR(1 < 2)` and `_EXPR(2 < 3)`. This contrasts with how the `||` operator behaves with `bool` values. In [this subsection]() we consider an alternative way of encoding logic into matchers, which allows us to simulate lazily evaluated logical operators.

To conclude this subsection, below we show an example test case which uses the matcher's logical operators. Through this example, and the output shown underneith, the reader should gain an understanding of how these operators can be used.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Logical operators"})
)
{
    using namespace abc;
    matcher_t m1 = _MATCHER(_EXPR(1 < 2));
    matcher_t m2 = _MATCHER(_EXPR(2 == 2));
    _CHECK(m1 && m2);
    _CHECK(m2 || m1);
    _CHECK(m1 || m2);
    matcher_t m3 = _MATCHER(m1 && m2);
    _CHECK(! m3);
    _CHECK(annotate(
        "Checking this vitally important thing",
        ! (_EXPR(1 < 2) && _EXPR(2 == 2))
    ));
}
```

Below we show the output for the above test case.

```
 1)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:8
     Source code representation:
       "_CHECK(m1 && m2)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:6
       Source code representation:
         "_MATCHER(_EXPR(1 < 2))"
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_MATCHER(_EXPR(2 == 2))"
     Matcher passed with output:
       "(1 < 2) && (2 == 2)"
 2)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:9
     Source code representation:
       "_CHECK(m2 || m1)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:6
       Source code representation:
         "_MATCHER(_EXPR(1 < 2))"
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_MATCHER(_EXPR(2 == 2))"
     Matcher passed with output:
       "(2 == 2) || (1 < 2)"
 3)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:10
     Source code representation:
       "_CHECK(m1 || m2)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:6
       Source code representation:
         "_MATCHER(_EXPR(1 < 2))"
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_MATCHER(_EXPR(2 == 2))"
     Matcher passed with output:
       "(1 < 2) || (2 == 2)"
 4)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:12
     Source code representation:
       "_CHECK(! m3)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:6
       Source code representation:
         "_MATCHER(_EXPR(1 < 2))"
       Source location:
         ..\docs\assertion_examples.hpp:7
       Source code representation:
         "_MATCHER(_EXPR(2 == 2))"
       Source location:
         ..\docs\assertion_examples.hpp:11
       Source code representation:
         "_MATCHER(m1 && m2)"
     Matcher failed with output:
       "!((1 < 2) && (2 == 2))"
 5)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:13
     Source code representation:
       "_CHECK(annotate( "Checking this vitally important thing", ! (_EXPR(1 < 2) && _EXPR(2 == 2)) ))"
     Matcher failed with output:
       "!((1 < 2) && (2 == 2))"
     Matcher's annotation:
       "Checking this vitally important thing"
```

## Simulating Lazily Evaluated Logic Operators

As discussed in the previous subsection, `matcher_t`'s logical operators which take two arguments - specifically `&&` and `||` - have no inbuilt lazy evaluation. For those unfamiliar with this type of behaviour when using logical operators with Booleans, consider the following functions.

```cpp
inline bool f1()
{
    return false;
}
inline bool f2()
{
    return true;
}
```

If we were to write the expression `f1() && f2()`, only `f1()` would be evaluated. This is because when `f1()` returns `false`, the `&&` operator understands that it is not possible for the function to return `true` no matter what the result of `f2()` is, and therefore does not evaluate it. However if we consider the expression `f2() && f1()`, after evaluating `f2()` the operator understands that it is still possible for the function to return `true`, and so `f1()` is evaluated.

`matcher_t`'s `&&` and `||` operators do not behave in this manner - in fact in C++ it is only `bool` whose logical operators exhibit this lazy behaviour. Instead, in a `matcher_t`-based expression such as `bool_matcher(f1()) && bool_matcher(f2())`, both subexpressions are always evaluated no matter what the result of the left or right subexpressions are.

However, lazy behaviour could be beneficial for matchers. For example, consider the following test case.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "A computationally expensive test"})
)
{
    using namespace abc;
    using namespace std;
    auto check_iota_results
        = [](const vector<int>&    vect,
             const pair<int, int>& begin_and_end_int)
    {
        matcher_t left_matcher
        {
            _EXPR(
                vect.size()
                == abs(begin_and_end_int.second - begin_and_end_int.first)
            )
        };
        matcher_t right_matcher{_EXPR(
            vect
            == ranges::to<vector<int>>(ranges::iota_view(begin_and_end_int.first, begin_and_end_int.second))
        )};
        _CHECK(left_matcher && right_matcher);
    };
    check_iota_results(
        ranges::to<vector<int>>(ranges::iota_view(1, 10'000)), { 1,9'999 }
    );
}
```

Here, `right_matcher` is much more computationally expensive to compute than `left_matcher`. The entire test case takes `3355 microseconds` to run on our machine. `abc_test` provides functionality which allows the user to write expressions that simulate lazily evaluated logical operators. As we will see later on, this will allow us to re-write this test case in a more efficient manner.

Unfortunately, due to how C++ works, it is not possible to use the logical operators `&&` and `||` to do this. Instead, `abc_test` provides this functionality through two additional types; the `simulated_or_expr_t` type and the `simulated_and_expr_t` type. Internally, they are both type synonyms for the type `template<abc::matcher::logic_enum_t> abc::matcher::simulated_logic_expr_t`, only differing by which `logic_enum_t` template parameter they use. `simulated_or_expr_t` is used to simulate lazily evaluated logical OR statements, while `simulated_and_expr_t` is used to simulate lazily evaluated logical AND statements. 

As both `simulated_or_expr_t` and `simulated_and_expr_t` are type synonyms for the `simulated_logic_expr_t` type, we will begin by explaining how the `simulated_logic_expr_t` type works. Internally, `simulated_logic_expr_t` contains two `std::optional` matchers, referred to as the left and right child. The user is able to set the left and right child using the functions `set_left_child` and `set_right_child` respectively. 

There is only one constructor for `simulated_logic_expr_t`. This is the default constructor, which sets both children to `std::nullopt` values.

`simulated_logic_expr_t` is designed to be used in the following way:
- First an instance of the object is created using the default constructor, and then one (usually the left) of the children is initialised. 
- `simulated_logic_expr_t` has an overloaded `bool operator` function, allowing an object instance to be used as an argument to an `if` statement. The `bool operator` function is defined in such a manner so that the `if` statement body will only be entered if the second child is required for the logical statement. 
- The user should then set the other (usually the right) child in the `if` statement's body. 
- After the `if` statement is concluded, the user is able to use the `simulated_logic_expr_t` object with an assertion macro. The assertion macro converts the `simulated_logic_expr_t` object to a `matcher_t` object, and the test is reported to the `abc_test` framework.

Below we show a test case which shows examples of how the `simulated_or_expr_t` and `simulated_and_expr_t` types, both type synonyms for `simulated_logic_expr_t`, should be used.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Simple simulation of logical operators"})
)
{
    using namespace abc;
    using namespace std;
    simulated_or_expr_t se_1;
    se_1.set_left_child(_MATCHER(_EXPR(1 == 1)));
    // This simulates a logical OR expression. As the only set child is
    // true, the if statemenet's body is not entered.
    if (se_1)
    {
        se_1.set_right_child(_MATCHER(_EXPR(2 == 2)));
    }
    _CHECK(se_1);

    simulated_and_expr_t se_2;
    se_2.set_left_child(_MATCHER(_EXPR(1 == 2)));
    // This simulates a logical AND expression. As the only set child is false,
    // the if statement's body is not entered.
    if (se_2)
    {
        se_2.set_right_child(_MATCHER(_EXPR(2 == 2)));
    }
    _CHECK(se_2);
}
```

Below we show the output from the test case above.

```
 1)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:15
     Source code representation:
       "_CHECK(se_1)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:8
       Source code representation:
         "_MATCHER(_EXPR(1 == 1))"
     Matcher passed with output:
       "(1 == 1) || (<unevaluated>)"
 2)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:25
     Source code representation:
       "_CHECK(se_2)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:18
       Source code representation:
         "_MATCHER(_EXPR(1 == 2))"
     Matcher failed with output:
       "(1 != 2) && (<unevaluated>)"
```

We can see from the output above that both `simulated_logic_expr_t` objects did not enter their associated `if` statements. Both objects determined that they did not need to do so due to the result of their already set left children. The unevaluated right children are represented by the `"(<unevaluated>)"` strings shown in the output. 

The reader should note that one disadvantage of creating an assertion using a `simulated_logic_expr_t` object is that it is possible for there to be "missing" source code representations in its test output. For example, in the above test case both `simulated_logic_expr_t` objects do not enter their associated `if` statements. This causes the lines which call the function `set_right_child` to not be evaluated. In turn, these lines are not shown as part of either assertion's `"Matcher's other sources"`. This has the potential to create some difficulties in understanding the control flow behind a failing test case.

Returning to `simulated_logic_expr_t`'s `bool operator` function, it is defined as follows:
- If neither of `simulated_logic_expr_t`'s children are set to `std::nullopt`, or both are set to `nullopt`, then the `bool operator` returns `false`.
- In all other cases, only one child is set to `std::nullopt`. We refer to the non-`std::nullopt` child as `child`. If the `simulated_logic_expr_t`'s `logic_enum_t` template value equals `logic_enum_t::OR` then the `bool operator` returns the result of `! child.result`. If the `simulated_logic_expr_t`'s `logic_enum_value_t` value equals `logic_enum_t::AND`, then the `bool operator` returns `child.result`. Otherwise, `bool operator` throws a runtime error.

The assertion macros `_CHECK`, `_REQUIRE`, `_BLOCK_CHECK`, `_BLOCK_REQUIRE` (and any source modifying BBA macros) can take an element of type `simulated_logic_expr_t` as an argument. They convert this argument to a `matcher_t` object, and then register this created `matcher_t` object with the `abc_test` framework. 

The reader should also note that the `_MATCHER` macro can also accept a `simulated_logic_expr_t` object as an argument, converting it to a `matcher_t` object. This allows the user to set the left and right children of a `simulated_logic_expr_t` object using `simulated_logic_expr_t` objects.

Below we show a test case which contains several simple examples created using the `simulated_logic_expr_t` type. This test case shows the user less commonly used functionality from the `simulated_logic_expr_t` type. We have also included comments describing the behaviour.

```cpp
_TEST_CASE(
    abc::test_case_t(
        {.name = "Several assertions created using simulated_logic_expr_t"}
    )
)
{
    using namespace abc;
    // A default constructed simulated_logic_expr_t has no left or right element
    // set.
    simulated_and_expr_t m1;
    // The if statement's body will not be entered.
    if (m1)
    {
        _TERMINATE_WITH_MSG("Never entered");
    }
    // This assertion will fail. All simulated_logic_expr_t entities with no
    // elements set automatically fail.
    _CHECK(m1);

    simulated_or_expr_t m2;
    m2.set_left_child(_MATCHER(_EXPR(1 == 1)));
    m2.set_right_child(_MATCHER(_EXPR(1 == 1)));
    // As both children are set (it doesn't matter if they are true or false)
    // this if statement will not be entered.
    if (m2)
    {
        _TERMINATE_WITH_MSG("Never entered");
    }
    // This assertion will pass. In the results the reader will see that both
    // children are evaluated.
    _CHECK(m2);

    // This resets both elements to std::nullopt.
    m2 = simulated_or_expr_t();

    // We can choose to set either the left or the right child. As long as only
    // one child is set, then the if statement may be entered.
    m2.set_right_child(_MATCHER(_EXPR(1 == 2)));
    if (m2)
    {
        m2.set_left_child(_MATCHER(_EXPR(1 == 1)));
    }
    // This assertion will pass.
    _CHECK(m2);

    simulated_and_expr_t m3;
    // The two set_right_child functions below set the children of m3 using
    // m2 and m1, both simulated_logic_expr_t types.
    // We can do this with or without the _MATCHER macro. Using it adds
    // source information to the test result.
    m3.set_left_child(m2);
    if (m3)
    {
        m3.set_right_child(_MATCHER(m1));
    }
    _CHECK(m3);
}
```

Below we show the output from this test case.

```
 1)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:17
     Source code representation:
       "_CHECK(m1)"
     Matcher failed with output:
       "(<unevaluated>) && (<unevaluated>)"
 2)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:30
     Source code representation:
       "_CHECK(m2)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:20
       Source code representation:
         "_MATCHER(_EXPR(1 == 1))"
       Source location:
         ..\docs\assertion_examples.hpp:21
       Source code representation:
         "_MATCHER(_EXPR(1 == 1))"
     Matcher passed with output:
       "(1 == 1) || (1 == 1)"
 3)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:43
     Source code representation:
       "_CHECK(m2)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:37
       Source code representation:
         "_MATCHER(_EXPR(1 == 2))"
       Source location:
         ..\docs\assertion_examples.hpp:40
       Source code representation:
         "_MATCHER(_EXPR(1 == 1))"
     Matcher passed with output:
       "(1 == 1) || (1 != 2)"
 4)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:55
     Source code representation:
       "_CHECK(m3)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:37
       Source code representation:
         "_MATCHER(_EXPR(1 == 2))"
       Source location:
         ..\docs\assertion_examples.hpp:40
       Source code representation:
         "_MATCHER(_EXPR(1 == 1))"
       Source location:
         ..\docs\assertion_examples.hpp:53
       Source code representation:
         "_MATCHER(m1)"
     Matcher failed with output:
       "((1 == 1) || (1 != 2)) && ((<unevaluated>) && (<unevaluated>))"
```

Below we show the first test case shown in this subsection re-written using the `simulated_and_expr_t` type.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "A computationally expensive test, revisited"})
)
{
    using namespace abc;
    using namespace std;
    auto check_iota_results
        = [](const vector<int>& vect, const pair<int, int>& begin_and_end_ints)
    {
        simulated_and_expr_t simulated_expr;
        simulated_expr.set_left_child(_MATCHER(_EXPR(
            vect.size()
            == std::abs(begin_and_end_ints.second - begin_and_end_ints.first)
        )));
        if (simulated_expr)
        {
            simulated_expr.set_right_child(_MATCHER(_EXPR(
                vect
                == ranges::to<vector<int>>(ranges::iota_view(
                    begin_and_end_ints.first, begin_and_end_ints.second
                ))
            )));
        }
        _CHECK(simulated_expr);
    };
    check_iota_results(
        ranges::to<vector<int>>(ranges::iota_view(1, 10'000)), {1, 9'999}
    );
}
```

This test case only takes `160 microseconds` to run on our machine, a significant speedup compared to the original test case. The following output is created from the test case above.

```
 1)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:24
     Source code representation:
       "_CHECK(simulated_expr)"
     Matcher's other sources:
       Source location:
         ..\docs\assertion_examples.hpp:11
       Source code representation:
         "_MATCHER(_EXPR( vect.size() == std::abs(begin_and_end_ints.second - begin_and_end_ints.first) ))"
     Matcher failed with output:
       "(9999 != 9998) && (<unevaluated>)"
```

The reader may question why `simulated_logic_expr_t` has been included with `abc_test`, as the same behaviour could be produced using a `matcher_t` object and its `bool operator` function. There are two core reasons behind this; firstly, by including the type `simulated_logic_expr_t`, the end-user saves time by not having to write their own logic around the `matcher_t` type to mimic `simulated_logic_expr_t`'s behaviour. Secondly, the output produced from the `simulated_logic_expr_t` type clearly shows the intention behind the created assertion through the use of the `(<unevaluated>)` string to represent unevaluated parts of a logical expression. Producing similar output from a `matcher_t` object would not be possible.

# User-Defined Matchers

In this section we show the reader how they can create their own matchers to use with `abc_test`. 

A `matcher_t` entity has no public constructors (except for the default constructor). Instead, we would direct the user to the function `mk_matcher_using_result` to create `matcher_t` objects. It is defined as follows:

```cpp
matcher_t mk_matcher_using_result(const matcher_result_t&)
```

A `matcher_result_t` object has two constructors; a default constructor and a constructor with the following type signature.

```cpp
matcher_result_t(const bool passed, const std::string_view str) noexcept;
```

The `passed` variable denotes whether the matcher passes, and the `str` variable holds a text representation of the matcher's result, which is used when producing output for the user.

Below we show an example of a function to create a `matcher_t` object. It is designed to check two `std::array` objects for equality, and if they are not equal it produces output which tells the user where the arrays diverge.

```cpp
template <typename T, std::size_t N>
inline abc::matcher_t
    arrays_equal(
        const std::array<T, N>& arr_l,
        const std::array<T, N>& arr_r
    )
{
    using namespace abc::matcher;
    using namespace std;
    optional<size_t> diverging_idx_opt;
    for (size_t idx{0}; idx < N; ++idx)
    {
        if (arr_l[idx] != arr_r[idx])
        {
            diverging_idx_opt = idx;
            break;
        }
    }
    if (diverging_idx_opt.has_value())
    {
        size_t diverging_idx{diverging_idx_opt.value()};
        return mk_matcher_using_result(matcher_result_t(
            false,
            fmt::format(
                "{0} != {1}. Arrays diverge at index {2}. Left array at index "
                "{2} = {3}, while right array at index {2} = {4}.",
                arr_l,
                arr_r,
                diverging_idx,
                arr_l[diverging_idx],
                arr_r[diverging_idx]
            )
        ));
    }
    else
    {
        return mk_matcher_using_result(
            matcher_result_t(true, fmt::format("{0} == {1}", arr_l, arr_r))
        );
    }
}

_TEST_CASE(
    abc::test_case_t({.name = "User-defined matcher"})
)
{
    using namespace std;
    _CHECK(arrays_equal(array<int, 3>{1, 2, 3}, array<int, 3>{1, 2, 3}));
    _CHECK(arrays_equal(array<int, 3>{1, 2, 3}, array<int, 3>{1, 2, 4}));
}
```

Below we show the output from this test.

```
 1)  Single-line assertion passed.
     Source location:
       ..\docs\assertion_examples.hpp:47
     Source code representation:
       "_CHECK(arrays_equal(array<int, 3>{1, 2, 3}, array<int, 3>{1, 2, 3}))"
     Matcher passed with output:
       "[1, 2, 3] == [1, 2, 3]"
 2)  Single-line assertion failed.
     Source location:
       ..\docs\assertion_examples.hpp:48
     Source code representation:
       "_CHECK(arrays_equal(array<int, 3>{1, 2, 3}, array<int, 3>{1, 2, 4}))"
     Matcher failed with output:
       "[1, 2, 3] != [1, 2, 4]. Arrays diverge at index 2. Left array at index 2 = 3, while right array at index 2 = 4."
```

# Included Matchers

`abc_test` comes with a set of functions to create matchers, which are designed to help the user when writing their own tests. All of these matchers require the following include directive.

```cpp
#include "abc_test/included_instances.hpp"
```

## Ranges

This subsection describes the set of matchers which are based around ranges.

```cpp
template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
constexpr matcher_t contains(R&& _a_range, const T& _a_value) noexcept;
```

This function creates a matcher which checks if `_a_range` contains `_a_value`.

# The Design of Matchers

We have not written this section yet.