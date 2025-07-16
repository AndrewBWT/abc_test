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

Nearly all of the examples we show are taken from the `examples/include/abc_test_examples/readme.hpp` [file](examples/include/abc_test_examples/readme.hpp), unless stated otherwise. 

### Minimal Example ###

Below we show the simplest example, which illustrates how easy it is to begin working with `abc_test`.

Unlike all the other examples in this section, this code comes from the `scripts/script_to_use_library/main_to_cpy.cpp` [file](scripts/script_to_use_library/main_to_cpy.cpp).

<!-- inject:scripts/script_to_use_library/main_to_cpy.cpp:initial_example -->

[Here]() the user can see the output from running this code.

### Unit Testing Example ###

Through the example above is valid testing code in `abc_test`, it relies on the user re-writing the same statement each time they want to test a new value, which in turn would require re-compilation.

`abc_test` contains many different types of data generators which can be used to loop through data. One of these data generators is the file-based data generator. It can be used to write unit tests, where the values to test are taken from a file. Below we show an example of this.

<!-- inject:examples/include/abc_test_examples/readme.hpp:unit_test_example -->

If re-running the test suite.

Below we show the output from running the `examples` test executable using the following command line arguments. In essence, it will only run the above test case.



The user can re-run the test using the following command line arguments. 

The repetition information is taken from the results above. By running the test suite like this, only the values which issued a test assertion failure are re-ran from the file "unit_tests". Below we show the output from this re-running of the test code.

All data generators in `abc_test` have this functionality. It can allow the user to configure their test executable in such a manner so that only values which fail an assertion are re-generated from a data generator.

### Property Testing Example ###

Property testing and fuzzy testing allow the user to either test some property about their code, or to test their code with many different values. `abc_test`'s generators can be used to add such functionality to a user's test suite.

Below we show an example which tests a user-defined function `users_mid_point`. It performs unit testing, fuzzy testing and property testing.

<!-- inject:examples/include/abc_test_examples/readme.hpp:property_test_example -->

### Detailed Assertion Examples ###

Data generators can be used to perform fuzzy testing. That it to say, a function can be tested to ensure that it doesn't throw any exceptions. 

<!-- inject:examples/include/abc_test_examples/readme.hpp:assertion_examples -->

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