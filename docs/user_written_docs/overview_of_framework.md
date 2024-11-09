# Overview of test framework{#overview_of_framework}

This file documents how the primary `abc` framework works. Specifically, it provides an overview of how the user creates tests, configures how the tests are to be ran, and runs the tests themselves. 

We also discuss the nomenclature used in `abc_test`; giving the user a basis for terms used throughout the rest of the documentation.

# Nomenclature

- We refer to `abc_test` as a *test library*. In this context, we consider `abc_test` to be a collection of C++ entities which a user can use as they see fit.
- A *test framework* is an entity (or set of entities) which can be used to build a program to run a set of user-defined tests. The test library `abc_test` includes a test framework - which is what this documentation file is about. We refer to this as the **`abc` test framework**.
- When a set of tests are grouped together and evaluated, we say that a *test suite* has been created and evaluated. Every time that the user declares a set of tests and compiles a program to run them, they are creating a test suite. When the user runs that test suite, we call it an **invoked test suite**. We refer to those test suites made using the `abc` test framework as **`abc` test suites**. `abc` test suites when ran are referred to as invoked `abc` test suites.

Generally we expect users of this library will use the `abc` test framework to create and run `abc` test suites. This document covers the following:

- How we expect the user to use the `abc` test framework to create `abc` test suites.
- An overview of how the `abc` test framework works.

# How to use the abc test framework

The `abc` test framework consists of three core components that the end-user should be aware of:

- `test_main_t`. This object contains the core logic which runs all tests. It has one constructor and one public function, as shown below.
```
class test_main_t
{
public:
    template<typename T>
    test_main_t(const validated_test_options_t<T>& _a_validated_test_options) noexcept;
    void run_tests() noexcept;
```
- `base_test_options_t`. This object acts as the "input" to the framework; it has a set of public fields which dictate how the framework processes tests. A detailed overview of `base_test_options_t` can be found in XXX.
- `validated_test_options_t`. The templated argument must be derived from `base_test_options_t`. Its constructor performs checks on the input parameter of type `base_test_options_t`. This way, checks have to be performed before it can be passed to `test_main_t`.

To use the `abc` test framework, the following code could be used:

```
void run_tests(const base_test_options_t& _a_test_options)
{
    auto _l_opts{validate_test_options(_a_test_options)};
    if (_l_opts.has_value())
    {
        //Options have been validated
        test_main_t _l_main(_l_opts.value());
        _l_main.run_tests();
    }
}
```
It is designed in this manner so that the user can use `base_test_options_t` to describe how they want `abc` test frameowrk to run.

As described in XXX, the core functionality of `abc_test` is split into two folders; `internal` and `included_instances`. Entities in `internal` are integral for `abc_test`, and those in `included_instances` can be considered extensions which are built on the primitaves provided in `internal`.

We feel that it is worth noting that there is a derived class of `base_test_options_t` called `included_instances_test_options_t` in `included_instances`. If not extending `abc_test`, we would advise the reader uses that entity when creating their test suites.

`included_instances` also includes a command line argument parser using the external library `CLI11`. It also includes the functionality to use configuration files. In this file is a function which also prints errors to the console if any are found when validating the test suite. 

```
#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
int main(int argc, char* argv[])
{
    return abc::run_test_suite_using_command_line_args(argc, argv);
}
```




The `abc` test framework is very simple to use it consists of the object `test_main_t`, the templated object `validated_test_options_t` and the extendable object `base_test_options_t`. These

In this subsection we show the steps the user should take when creating a test suite using the `abc` test framework

A broad overview of how we envisage the user will use the test framework is as follows:

1. The user uses the test framework to write their test suite. That is, set it up so that tests can be added to it.
1. The user then creates and registers a set of tests they wish to run as part of their test suite.
3. The user compiles the test suite program, runs the test suite program, and analysis the results from it.

In this document we discuss point 2.

The primary test framework's logic is encompassed in `test_main_t`. There is one public constructor associated with `test_main_t`, and one public function `run_test_suite()`.



1. The input components are prepared for `test_main_t` by the user.
2. The input components are then registered and validated by `test_main_t`.
3. The tests are ran and results reported by `test_main_t`.
4. Any feedback is reported, then the invokation of `test_main_t` terminates.

Below we provide an overview of each of these steps.

## Preparation

These can be thought of as the input to `test_main_t`. The most obvious of these are tests being declared and registered by the user, either with programmatical collections (e.g. `std::vector`), or with the GLOT.

There are other entities which need to be prepared and passed to `test_main_t`, such as the set of `test_reporter_t` elements, and the set of `error_reporter_t` elements. These tell `abc_test` how to report a tests result, and how to report errors respectively.

All of the input components are either controlled by or contained in the object `base_test_options_t` or a class derived from it. `test_main_t`'s only constructor takes a (validated) `base_test_options_t` object as its only parameter.

## Validation

At this point, the invokation of `test_main_t` can now proceed by checking the input it was given. 

## Running Tests

The tests are then ran. After each test is ran, the set of `test_reporter_t`'s associated with the `test_main_t` invokation are given the result of the test. Any errors are processed by `error_reporter_t`. 

# Termination

Finally, the `test_main_t` is terminated. 

# Running Tests

There are severl ways of running tests:

- A set of repetition data for several tests.
- A set of path used to determine which sets to run.
- An override for running all tests in path set even if theres no repetition data for it.
- If the input path is empty, then it becomes a singleton with "" in it.

- Take all tests which are filtered by the paths:
- (If the input path is empty, this is all tests)
    - If repetition data set has elements in
        - Override is off, then filter the set to only run those members which have repetition members.
        - Override is on, run all tests in the set. Those elemnets with repeittion data are ran with that repetition data.
    - If repetition data set has no elements in.
        - All tests in the set are ran.

- Default value 

the input paths are empty, and the test has no repetition data, then the test is ran normally.
- If the override is off, hte input paths are empty, and the test has repetition data with an entry for the test, then the test is ran with repetition data.
- If the override is off, the input paths are empty, and the test has repetition data but no entry for the test, then the test is not ran.
- If the override is off, the input paths are not empty and the test is in the set described by them, and the test has no repetition data, then the test is ran normally.
- If the override is off, the input paths are not empty and the test is in the set described by them, and the test has repetition data and the test is in that set, then the test is ran with repetition data.
- If the override is off, the input paths are not empty and the test is in the set described by them, and the test has repetition data but its not in the set, then the test is ran normally.

- If the override is off, the input paths are empty, and the test has repetition data, with an entry associated with this test, then the test is ran with the repetition data.
- If the override is off, the input paths are empty, and the test has repetition data but no entries associated with this test, then the test is not ran.
- If the override is off, the input paths are not-empty and one matches with this test, and the test has repetition data associatd with this test, then the test is ran with the repetition data.
- If the 

- If the override is on, and a test has repetition data, that test is ran with the repetition data.
- If the test override is on, the test has repetition data 

- If the override is off, and the test's path matches with the input path, and the test has repetition data, then the test is ran with the repetition data.
- If the override is on, the test's path does not match the repetition data, and the test has repetition data associated with it, then the test is not ran.
- 