# Test Assertions

In this file we describe every way of writing test assertions.

Files Recommended to have read before this file:
Declaring Tests - understand the syntax we use to declare tests.

# Overview

Abstractly speaking, assertions in `abc_test` are pieces of code that are able to break the control flow of the current function. They interact with the overarching `abc_test` framework to do this in a safe manner, and to allow the overarching program to continue.

We would describe `abc_test` as a matcher-based testing library. Not every assertion is a matcher, however they all *process* matchers; matchers act as arguments to every assertion function or structure in `abc_test`. Therefore, we will begin by describing what we mean by a matcher.

A matcher, in `abc_test` is a structure which performs some pre-defined logic test on any input received. In essence, it returns `false` or `true`. We use them to standardise frequently used assertions; for example if a list is sorted, or a pointer `==nullptr`.

We use macros to process and pass the results of matchers to the `abc_test` framework. For example:

```
_CHECK(abc::eq(1==2))
```

`abc::eq(1,2)` creates a matcher which checks for equality between `1` and `2`. `_CHECK` is a macro which takes this matcher, runs it, and passes its result to the `abc_test` framework.