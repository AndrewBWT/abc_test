# `abc_test` #

`abc_test` is a C++ testing library, targeting C++23. Currently `abc_test` is a personal project. Please be aware of this when using it, as its components, function definitions and macros are liable to change. If you are interested in when `abc_test` may be nearing release, please see [here](#roadmap).

## Features ##
- `abc_test` is designed to be easy to set up, and allows the user to quickly begin writing tests. 
- `abc_test`'s design does not guide the user to any one test methodology. Instead, it provides the tools to write different types of tests such as unit tests, fuzzy tests and property tests, to name but a few examples. The user is able to interleave different types of tests in individual test cases, with the design goal being that the user is then able to contain all the testing logic for a single component in a single place.
- `abc_test` is designed with concurrency in mind. Tests are run in parallel by default.
- `abc_test` tracks where test assertions fail, and for which values. A test executable can be configured to be re-ran, and skip previously passed test assertions. This allows the user spends less time configuring their test environment, or setting breakpoints, and more time understanding the underlying issues.
- `abc_test'`s test assertions are designed to be human-readable and easy to understand, while also being highly configurable. The design goal behind this was to allow the user to be able to quickly discern what is being tested, without requiring intricate knowledge of the `abc_test` framework.

If any of these features have piqued your interest, `abc_test` may be of interest to you.

## Navigation ##
- [Examples](#examples)
- [Installation](#installation)
- [Roadmap](#roadmap)
- [License](#license)

## Examples ##

We will begin with a minimal example, which shows how easy it is to begin working with `abc_test`.

<!-- inject:scripts/script_to_use_library/main_to_cpy.cpp:initial_example -->

This next example shows the user how to write assertions.

This example shows how to use data generators to write a test case which performs unit tests, fuzzy tests, property tests and regression tests.

## Installation ##

`abc_test` targets C++23. It has been built using `cmake` and, for now at-least, we recommend using `cmake` to install it.

Given a 

See above for the environments it has been tested in. To install it run the following from the main folder.

```
mkdir build && cd build
cmake ..
```

`abc_test` has been compiled under Windows using the MSVC and Clang compilers. The MSVC compiler in question is that which ships with 17.13.5. The Clang compiler used is version 18.1.8.

`abc_test` has also been built under Linux, specifically Ubuntu 24. It was compiled using the GNU compiler on that platform.


## Roadmap ##

As stated in the introduction, `abc_test` is currently a personal project. It was originally conceived from ideas in previous projects of mine, where I was wanting to have both fuzzy testing capabilities alongside unit testing capabilities. It grew significantly, and is now in a state where I want to share it and see what others think, and whether there is any interest in developing it further.

Some of the code is quite "hacky", and could do with being re-written. I am currently in the process of writing tests for `abc_test` in `abc_test`, which has spurred on adding new features, while also allowing me to fix bugs that I find.

## Licence ##