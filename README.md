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

```cpp
// Only these two includes are needed.
#include <abc_test/core.hpp>
#include <abc_test/included_instances.hpp>

int
    main(
        int   argc,
        char* argv[]
    )
{
    // This code will process the command line arguments, and set up the testing enviornment.
    return abc::run_test_suite_using_command_line_args(argc, argv);
}

// This is the function which will be tested.
inline int
    fib(
        const int i
    )
{
    return (i <= 1) ? i : fib(i - 1) + fib(i - 2);
}

// The macro _TEST_CASE declares a test, and registers it with the underlying
// test framework. A test's name can be used to provide a description of the
// test, while a test's path provides a way of organizing tests.
_TEST_CASE(
    // Note the extra bracket after "abc::test_case_t". This ensures the commas
    // used to separate test_case_t's member variables are not consumed by the macro.
    abc::test_case_t(
        {.name = "Testing Fibonacci function", .path = "tests::fib"}
    )
)
{
    // This test will pass.
    _CHECK_EXPR(fib(6) == 8);
    // This test will fail.
    _CHECK_EXPR(fib(7) == 14);
}

```

[Here]() the user can see the output from running this code.

### Unit Testing Example ###

Through the example above is valid testing code in `abc_test`, it relies on the user re-writing the same statement each time they want to test a new value, which in turn would require re-compilation.

`abc_test` contains many different types of data generators which can be used to loop through data. One of these data generators is the file-based data generator. It can be used to write unit tests, where the values to test are taken from a file. Below we show an example of this.

```cpp
_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing Fibonacci function using data from a file",
         .path = "tests::fib"}
    )
)
{
    // This namespace is needed for some of the more complicated abc test
    // functionality.
    using namespace abc;
    // This macro creates an object which can have assertions passed into it.
    // The string argument represents an annotation.
    auto unit_tester = _MULTI_MATCHER("Unit tests for Fibonacci function");
    // This for loop iterates over the values taken from the file.
    // The type "pair<int,int>" identifies what values being read from the file,
    // and the string argument represents the name of the file. The file is in
    // the folder tests/fib, with the root file being set by either the system
    // or the user.
    for (auto& [input, expected_output] :
         read_data_from_file<std::pair<int, int>>("unit_tests"))
    {
        // The values input and expected_output are taken from each lne of the
        // file. The result of the assertion _CHECK_EXPR is
        //  streamed to the multi matcher.
        unit_tester << _CHECK_EXPR(fib(input) == expected_output);
    }
    // The multi matcher is then checked. It only passes if all the assertions
    // are true. It is this line which sends the information to the testing
    // framework.
    _CHECK(unit_tester);
}

// tests/fib/unit_tests.gd
// (0, 0)
// (6, 8)
// (7, 14)
```

If the user runs the following command (replace `executable.exe` with the path to the executable of `abc_test_examples`).

```
data
```

The following output is produced.

```
hi
```

Here, the only assertions which are re-ran are those which fail the test. `abc_test` tracks information about what data generators are currently being iterated through and which assertions failed, and can produce a "seed" which can be used to re-run the test, but with the data generators only re-producing those values which caused an assertion to fail. In the command line arguments shown above, this "seed" is the long string of hex digits.

All data generators in `abc_test` have this functionality. It can allow the user to configure their test executable in such a manner so that only values which fail an assertion are re-generated from a data generator.

### Property Testing Example ###

Property testing and fuzzy testing allow the user to either test some property about their code, or to test their code with many different values. `abc_test`'s generators can be used to add such functionality to a user's test suite.

Below we show an example which tests a user-defined function `users_mid_point`. It performs unit testing, fuzzy testing and property testing.

```cpp
inline int
    users_midpoint(
        const int arg1,
        const int arg2
    )
{
    return (arg1 + arg2) / 2;
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing users_midpoint function", .path = "tests::midpoint"}
    )
)
{
    using namespace abc;
    auto property_tests
        = _MULTI_MATCHER("Property tests for users_midpoint function");
    // We are testing that users_midpoint returns the same result as
    // std::midpoint, and that reversing the arguments to users_midpoint
    // produces the same result.

    // This for loop uses two generators. They are chained together using the &
    // operator. The first generator creates random data. By default it creates
    // 100 random values of the type specified. The second generator enumerates
    // over all values from (-2,-2) to (2,2).
    for (auto& [arg1, arg2] :
         generate_data_randomly<std::pair<int, int>>()
             & enumerate_data(
                 from_m_to_n(std::make_pair(-2, -2), std::make_pair(2, 2))
             ))
    {
        auto result = users_midpoint(arg1, arg2);
        // In abc_test, a matcher represents an assertion, and the _CHECK macro
        // is used to register it with the testing framework.
        // The _EXPR macro allows comparison operators to be used to construct a
        // matcher. Matchers can also be used with the boolean operators &&, ||
        // and ! to express logical relationships between assertions.
        property_tests << _CHECK(
            _EXPR(result == std::midpoint(arg1, arg2))
            && _EXPR(result == users_midpoint(arg2, arg1))
        );
    }
    _CHECK(property_tests);
}

```

### Fuzzy Testing Example ###

Fuzzy testing is used to test functions with many different values, in essence trying to find examples where the code "breaks". In C++, this behaviour is typically accompanied by an exception being thrown. Below we show an example which performs fuzzy testing on the `users_average` function.

```cpp
inline float
    users_average(
        const std::vector<float>& elements
    )
{
    float sum{0.0};
    for (auto&& element : elements)
    {
        sum += element;
    }
    return sum / static_cast<float>(elements.size());
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing users_average function", .path = "tests::average"}
    )
)
{
    using namespace abc;
    auto fuzzy_tests = _MULTI_MATCHER("Fuzzy tests for users_average function");
    // We are testing that users_average works with a range of values.

    for (auto& vect :
         // Here, generate_data_randomly takes an argument called a
         // "general_data_file", signified by the function "gdf". Using this
         // GDF, values which trigger a test assertion failure are written to
         // the file "random_data". Every time the test is re-ran, the values
         // from this file are tested first. This allows problematic test values
         // to be retained easily, and helps the user quickly identify whether
         // there has been a test regression.
         generate_data_randomly<std::vector<float>>(gdf("random_data"))
             & enumerate_data(from_min_to_val(std::vector<float>(1, 100.0f))))
    {
        // matcher_t is the object which contains an assertion. Until it is put
        // into the _CHECK or _REQUIRE macro, the test framework will not
        // process it.

        // Prevoiusly we have used the macro _EXPR to encode a matcher using a
        // comparison operator, however abc_test also comes with functions that
        // can be used to build matchers.

        // true_matcher encodes an assertion which passes. Its string argument
        // can be used to encode a message associated with the matcher.
        matcher_t exception_matcher = true_matcher(u8"No exception was thrown");
        try
        {
            do_not_optimise(users_average(vect));
        }
        catch (const std::exception& exception)
        {
            // The false_matcher function is similar to the true_matcher
            // exception, except it encodes a false assertion.
            exception_matcher
                = false_matcher(u8"An unexpected exception was thrown");
        }
        // abc_test includes macros which can reduce the need for boiler-place
        // code to to check exceptions.
        matcher_t exception_matcher_2;
        _BEGIN_NO_THROW_MATCHER(exception_matcher_2);
        do_not_optimise(users_average(vect));
        _END_NO_THROW_MATCHER(exception_matcher_2);
        fuzzy_tests << _CHECK(exception_matcher && exception_matcher_2);
    }
    _CHECK(fuzzy_tests);
}

```

If the reader would like to see more examples of `abc_test`, we recommend reading the hand-written documentation and the documentation generated using Doxygen. 

## Installation ##

`abc_test` targets C++23. Currently `abc_test` requires the `fmt` library. `abc_test` is designed to be built using `cmake`. Below is an example `CMakeLists.txt` file, which builds `abc_test` as part of a user's project.

```
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 23)
project(my_executable)
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.2.0
)
FetchContent_MakeAvailable(fmt)
FetchContent_Declare(
  abc_test
  GIT_REPOSITORY https://github.com/abc_test/abc_test.git
  GIT_TAG 0.0.1
)
FetchContent_MakeAvailable(abc_test)

add_executable(my_executable main.cpp)
target_link_libraries(my_executable PRIVATE abc_test)
```

Then, navigating to the root directory of `my_executable`, run the following commands.

```
mkdir build && cd build
cmake ..
```

We have tested the above script in Windows using the MSVC and Clang compilers, built using Visual Studio 17.13.5. The Clang compiler version used was 18.1.8. So it should be buildable, and atleast the minimal example should compile and run.

We have also tested it under Ubuntu 24.04 using the GNU and Clang compilers. The GNU compiler version used was 13.3.0, and the Clang compiler version used was 20.1.7. 

## Roadmap ##

As stated in the introduction, `abc_test` is currently a personal project. It was originally conceived from ideas in previous projects of mine, where I was wanting to have both fuzzy testing capabilities alongside unit testing capabilities. It grew significantly, and is now in a state where I want to share it and see what others think, and whether there is any interest in developing it further.

Some of the code is quite "hacky", and could do with being re-written. I am currently in the process of writing tests for `abc_test` in `abc_test`, which has spurred on adding new features, while also allowing me to fix bugs that I find.

## Licence ##

`abc_test` is distributed with the MIT licence. In the `licences` subfolder, we include licences shipped with other source code we have included and/or modified.

Specifically:

- `abc_test` uses the biginteger library [found here](https://github.com/benlau/biginteger/tree/master). We have augmented the code somewhat by adding constructors for the `long long` and `unsigned long long` types. We have also changed the names of the headers and cpp files, to better suit our naming convention.
- `abc_test` includes a version of the `DoNotOptimize` function, included in Google Benchmark library [found here](https://github.com/google/benchmark/tree/main). We have augmented the code for our needs, however the original code was copied from their work.