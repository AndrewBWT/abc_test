# `abc_test` #

`abc_test` is a C++ testing library built for C++23. 

This is version 0.0.1 of `abc_test`. It is still in its initial development phase. As such, please be aware that the library could change from version to version - though we do not envisage any drastic changes to it at this point. If you are interested in the future plans for `abc_test`, please see [this section](#roadmap).

## Features ##
- `abc_test` is designed to be easy to set up, and allows the user to quickly begin writing tests. 
- `abc_test` does not force any one test methodology on the user. Instead, it provides the user with tools that can be used to write different types of tests - such as unit tests, fuzzy tests and property tests. These tools also allow the user to interleave different test types in the same test case - therefore, if the user desires to, they can keep all the testing logic for a single component in a single test case.
- `abc_test` is designed with concurrency in mind. Tests are run in parallel by default.
- `abc_test` keeps track of where test assertions fail. A test executable can be configured to skip previously passed test assertions, and only re-run the failed ones. This allows the user to spend less time configuring their test environment, or setting breakpoints, and more time understanding the underlying issues behind their failed test cases.
- `abc_test`'s assertions are designed to be human-readable and easy to understand, while also being highly configurable. This means the user is able to quickly discern what is being tested when looking at a test case, without necessarily requiring intricate knowledge of the `abc_test` library.

If any of these features have piqued your interest, please take a look at the examples below.

## Navigation ##
- [Examples](#examples)
- [Installation](#installation)
- [Roadmap](#roadmap)
- [License](#license)

## Examples ##

All of the examples we show in this section are taken from the `examples/include/abc_test_examples/readme.hpp` [file](examples/include/abc_test_examples/readme.hpp), unless stated otherwise. 

### Minimal Example ###

Below we show a simple example of how to use `abc_test`.

Unlike all the other examples in this section, this code comes from the `scripts/script_to_use_library/main_to_cpy.cpp` [file](scripts/script_to_use_library/main_to_cpy.cpp).

```cpp
// Only these two includes are needed when working with abc_test.
#include <abc_test/core.hpp>
#include <abc_test/included_instances.hpp>

int main(int argc,char* argv[])
{
    // This line will process the command line arguments, and set up the testing environment.
    return abc::run_test_suite_using_command_line_args(argc, argv);
}

// This is the function we will be testing.
inline int fib( const int i)
{
    return (i <= 1) ? i : fib(i - 1) + fib(i - 2);
}

// The macro _TEST_CASE declares a test, and registers it with the underlying
// test framework. A test's name can be used to provide an identifier for the 
// test, while a test's path provides a way of organizing the test.
_TEST_CASE(
    // Note the extra bracket after "abc::test_case_t". This ensures the commas
    // used to separate test_case_t's member variables are not consumed by the macro.
    abc::test_case_t(
        {.name = "Testing Fibonacci function", .path = "readme::fib"}
    )
)
{
    // This assertion will check if fib(6) == 8. It does, so the assertion will pass.
    _CHECK_EXPR(fib(6) == 8);
    // As fib(7) != 14, this assertion will fail.
    _CHECK_EXPR(fib(7) == 14);
}
```

### Unit Testing Example ###

The example above could be considered a unit test for the `fib` function. However, writing the test case in this way has one major shortcoming; it relies on the user re-writing the same statement each time they want to test a new value with the `fib` function. In turn, this requires re-compiling the test executable.

`abc_test` contains many different types of data generators which can be used to iterate over data. One of these data generators is the file-based data generator. It can be used to write unit tests, where the values to test are taken from a file. Below we show an example of how it can be used to write a unit test. Adding new values to the test only requires adding values to the file - no re-compilation is necessary.

```cpp
_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing Fibonacci function using data from a file",
         .path = "readme::fib"}
    )
)
{
    // This namespace is needed for some of the more complicated
    // functionality abc_test can provide.
    using namespace abc;
    // The macro _MULTI_MATCHER creates an object called a multi_matcher_t. A
    // multi_matcher_t can store a group of assertions. The macro's string
    // argument represents an annotation for the multi_matcher_t.
    auto unit_tester = _MULTI_MATCHER("Unit tests for Fibonacci function");
    // This for loop iterates over the values taken from the file.
    // The type "pair<int,int>" identifies what types of values are being read
    // from the file. The string argument represents the name of the file where
    // the values are read from. The file is located in the folder
    // tests/fib/Testing_Fibonacci_function_using_data_from_a_file, relative to
    // the user-set root folder.
    for (auto& [input, expected_output] :
         read_data_from_file<std::pair<int, int>>("unit_tests"))
    {
        // The values input and expected_output are taken from each line of the
        // file. The result of the assertion in the macro _CHECK_EXPR is sent to
        // unit_tester.
        unit_tester << _CHECK_EXPR(fib(input) == expected_output);
    }
    // unit_tester is checked to see whether it passed, and its
    // result is sent to the test framework to be processed. A multi_matcher_t
    // will only pass if all of the assertions it contains also pass.
    _CHECK(unit_tester);
}

// readme/fib/Testing_Fibonacci_function_using_data_from_a_file/unit_tests.gd
// (0, 0)
// (6, 8)
// (7, 14)
```

If the user runs the `abc_test_examples` executable from the project's root directory with the options `--root_path examples/data/test_data --test_paths_to_run readme::fib`, then the following is an example of the output which could be produced.

```sh
==========================================================================================
TEST SUITE CONFIGURATION
==========================================================================================
Global test list used?              Yes
Write data to files:                Yes
Path delimiter used:                "::"
Root path used:                     examples/data/test_data
Threads used:                       24
Comment used:                       "#"
general data extension used:        "gd"
Number of values used to seed RNGs: 100
Global seed:                        Not set by user. Global seed has been set randomly by system to the integer 1753490037
Force run all tests used?           No
test paths to run:                  readme::fib.
==========================================================================================
TEST INFO
==========================================================================================
Test result:                TEST FAILED
Test passed:                No
Test name:                  "Testing Fibonacci function using data from a file"
Source location:            G:\MyProjects\cpp\git_projects\abc_test\examples\include\abc_test_examples/readme.hpp:13
Source code representation: "_TEST_CASE(abc::test_case_t( {.name = "Testing Fibonacci function using data from a file", .path = "readme::fib"} ))"
Test Path:                  "readme::fib"
Seed used:                  <no seed used>
Seed used (hex):            <no seed used>
Seed to re-run test:        [[(0,0,"(0, 3)",[])]]
Seed to re-run test (hex):  "5b5b28302c302c2228302c203329222c5b5d295d5d"
Warnings:                   0
Assertions evaluated:       1
Assertions passed:          0
Assertions failed:          1
Assertion information:      0 / 1 assertions passed. All assertion failed.
Termination status:         Function exited normally with a failed status.
Time taken:                 5212 microseconds
==========================================================================================
ASSERTION INFO
==========================================================================================
  1)  Multi-element block-based assertion failed. 2/3 assertions passed. Assertion terminated function.
      Assertion's annotation:  "Unit tests for Fibonacci function"
      Source block begin:
        Source location:
          G:\MyProjects\cpp\git_projects\abc_test\examples\include\abc_test_examples/readme.hpp:26
        Source code representation:
          "_MULTI_MATCHER("Unit tests for Fibonacci function")"
        Source location:
          G:\MyProjects\cpp\git_projects\abc_test\examples\include\abc_test_examples/readme.hpp:44
        Source code representation:
          "_CHECK"
      Assertion data (grouped by source):
        The following 3 matcher-based assertions have the same source data, which is as follows:
        The data of the 3 matcher-based assertions:
        1)  Matcher-based assertion passed with output:
              Primary output from matcher:
                0 == 0
            Logged information:
              data generator's value = (0, 0)
        2)  Matcher-based assertion passed with output:
              Primary output from matcher:
                8 == 8
            Logged information:
              data generator's value = (6, 8)
        3)  Matcher-based assertion failed with output:
              Primary output from matcher:
                13 != 14
            Logged information:
              data generator's value = (7, 14)
==========================================================================================
==========================================================================================
TEST SUITE RESULTS
==========================================================================================
Total tests ran:                              1
Total tests passed:                           0
Total tests failed:                           1
Total tests failed and terminated:            0
Total tests failed (but not terminated):      1
Total tests which threw unexpected exception: 0
Total assertions ran:                         1
Total assertions passed:                      0
Total assertions failed:                      1
Global random seed:                           "1753490037"
Repetition loop stack seed:                   "54657374696e67204669626f6e616363692066756e6374696f6e207573696e6720646174612066726f6d20612066696c653a3a726561646d653a3a666962:5b5b28302c302c2228302c203329222c5b5d295d5d"
==========================================================================================
```

If the user were to then run the same executable from the same location with the options `--root_path ../../examples/data/test_data --test_paths_to_run readme::fib --repetition_config 54657374696e67204669626f6e616363692066756e6374696f6e207573696e6720646174612066726f6d20612066696c653a3a726561646d653a3a666962:5b5b28302c302c2228302c203329222c5b5d295d5d --write_data_to_files false`, then the following is an example of the output which could be produced.

```sh
==========================================================================================
TEST SUITE CONFIGURATION
==========================================================================================
Global test list used?              Yes
Write data to files:                No
Path delimiter used:                "::"
Root path used:                     examples/data/test_data
Threads used:                       24
Comment used:                       "#"
general data extension used:        "gd"
Number of values used to seed RNGs: 100
Global seed:                        Not set by user. Global seed has been set randomly by system to the integer 1753490037
Force run all tests used?           No
test paths to run:                  readme::fib.
==========================================================================================
TEST INFO
==========================================================================================
Test result:                TEST FAILED
Test passed:                No
Test name:                  "Testing Fibonacci function using data from a file"
Source location:            G:\MyProjects\cpp\git_projects\abc_test\examples\include\abc_test_examples/readme.hpp:13
Source code representation: "_TEST_CASE(abc::test_case_t( {.name = "Testing Fibonacci function using data from a file", .path = "readme::fib"} ))"
Test Path:                  "readme::fib"
Seed used:                  [[(0,0,"(0, 3)",[])]]
Seed used (hex):            "5b5b28302c302c2228302c203329222c5b5d295d5d"
Seed to re-run test:        [[(0,0,"(0, 3)",[])]]
Seed to re-run test (hex):  "5b5b28302c302c2228302c203329222c5b5d295d5d"
Warnings:                   0
Assertions evaluated:       1
Assertions passed:          0
Assertions failed:          1
Assertion information:      0 / 1 assertions passed. All assertion failed.
Termination status:         Function exited normally with a failed status.
Time taken:                 354 microseconds
==========================================================================================
ASSERTION INFO
==========================================================================================
  1)  Multi-element block-based assertion failed. 0/1 assertions passed. Assertion terminated function.
      Assertion's annotation:  "Unit tests for Fibonacci function"
      Source block begin:
        Source location:
          G:\MyProjects\cpp\git_projects\abc_test\examples\include\abc_test_examples/readme.hpp:26
        Source code representation:
          "_MULTI_MATCHER("Unit tests for Fibonacci function")"
        Source location:
          G:\MyProjects\cpp\git_projects\abc_test\examples\include\abc_test_examples/readme.hpp:44
        Source code representation:
          "_CHECK"
      Assertion data (grouped by source):
        1)  Matcher-based assertion failed with output:
              Primary output from matcher:
                13 != 14
            Logged information:
              data generator's value = (7, 14)
==========================================================================================
==========================================================================================
TEST SUITE RESULTS
==========================================================================================
Total tests ran:                              1
Total tests passed:                           0
Total tests failed:                           1
Total tests failed and terminated:            0
Total tests failed (but not terminated):      1
Total tests which threw unexpected exception: 0
Total assertions ran:                         1
Total assertions passed:                      0
Total assertions failed:                      1
Global random seed:                           "1753490037"
Repetition loop stack seed:                   "54657374696e67204669626f6e616363692066756e6374696f6e207573696e6720646174612066726f6d20612066696c653a3a726561646d653a3a666962:5b5b28302c302c2228302c203329222c5b5d295d5d"
==========================================================================================
```

In the second run, the only assertions which are evaluated are those which failed during the first run. `abc_test` configured the data generator so that it only produced values which had previously failed an assertion. By providing the executable with the `repetition_config` given as output from the first run, we encoded instructions into the executable to behave in this manner.

This is one of the core features in `abc_test`. It allows the user to hone in on failing test assertions, skipping generated values which did not produce failed assertions. To be clear, all data generators in `abc_test` have this functionality.

This feature works by tracking information about what data generators are currently being iterated through when an assertion fails, and that information is memoized. When the test suite has finished running, a string of hex digits called a `repetition_config` is produced, which represents this information for each test that encountered at least one failed assertion. This `repetition_config` can then be given back to the test suite when it is re-ran, ensuring that the data generator's only produce those values which triggered a failed test assertion.

### Property Testing Example ###

Property testing allows the user to test some property about their code. In this context a "property" is a logical expression about some function or sequence of functions. Properties are usually tested with some randomly generated data, or data which comes from a user-defined range of values. `abc_test` includes data generators which make writing property tests simple.

Below we show an example which tests a user-defined function `users_mid_point`. The test case checks two properties; the first is that the result of `users_mid_point` matches `std::midpoint`. The second is that changing the order of the arguments given to `users_mid_point` doesn't change the result.

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
        {.name = "Testing users_midpoint function", .path = "readme::midpoint"}
    )
)
{
    using namespace abc;
    auto property_tests
        = _MULTI_MATCHER("Property tests for users_midpoint function");
    // This for loop uses data two generators. They are chained together using
    // the & operator. The first generator creates random data. By default it
    // creates 100 random values of the type specified. The second generator
    // enumerates over all values from (-2,-2) to (2,2).
    for (auto& [arg1, arg2] :
         generate_data_randomly<std::pair<int, int>>()
             & enumerate_data(
                 from_m_to_n(std::make_pair(-2, -2), std::make_pair(2, 2))
             ))
    {
        auto result = users_midpoint(arg1, arg2);
        // In abc_test, a matcher_t object represents an assertion. The _EXPR
        // macro allows a matcher_t object to be created using a comparison
        // operator.
        //
        // The _CHECK macro is used to register a matcher_t object with the
        // testing framework. matcher_t objects can also be combined using the
        // logic operators &&, || and ! to express relationships between
        // assertions  - as seen below.
        property_tests << _CHECK(
            _EXPR(result == std::midpoint(arg1, arg2))
            && _EXPR(result == users_midpoint(arg2, arg1))
        );
    }
    _CHECK(property_tests);
}

```

### Fuzzy Testing Example ###

Fuzzy testing allows the user to stress-test their code; typically this is by using random or obscure data on it to see if it "breaks". in C++, "breaking" usually means the code throws an exception or crashes. Below we show an example of `abc_test` being used to perform fuzzy testing on the function `users_average`.

```cpp
inline float
    users_average(
        const std::vector<int>& elements
    )
{
    int sum{0};
    for (auto&& element : elements)
    {
        sum += element;
    }
    return static_cast<float>(sum) / static_cast<float>(elements.size());
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing users_average function", .path = "readme::average"}
    )
)
{
    using namespace abc;
    auto fuzzy_tests = _MULTI_MATCHER("Fuzzy tests for users_average function");

    for (auto& vect :
         // This version of generate_data_randomly works slightly differently to
         // the version seen previously. It takes an object called a gdf as an
         // argument, which represents a filename - in this case "random_data".
         // This version of generate_data_randomly will send any data which
         // triggers a failed assertion to be sent to the file "random_data".
         // Any time the data generator is ran, it will stream all the values
         // from "random_data" before generating random values.

         // This allows problematic test values to be retained easily, and helps
         // the user quickly identify whether there has been a test regression.
         generate_data_randomly<std::vector<int>>(gdf("random_data")))
    {
        // As discussed in the previous example, the matcher_t object represents
        // an assertion. It is only when it is used as an argument to the _CHECK
        // or _REQUIRE macros that the test framework will process the matcher_t
        // object - or when it is used as part of a multi_matcher_t object.

        // Previously we have seen the macro _EXPR be used to create a
        // matcher_t object using a comparison operator, however abc_test also
        // comes with functions that can be used to build matcher_t objects.

        // true_matcher encodes a matcher which always passes. Its string
        // argument can be used to encode a message associated with the matcher.
        matcher_t exception_matcher = true_matcher(u8"No exception was thrown");
        try
        {
            // The do_not_optimise function ensures that its argument is not
            // optimized away. It is borrowed from Google Benchmark.
            do_not_optimise(users_average(vect));
        }
        catch (const std::exception& exception)
        {
            // The false_matcher function is similar to true_matcher, except it
            // always fails.
            exception_matcher
                = false_matcher(u8"An unexpected exception was thrown");
        }
        fuzzy_tests << _CHECK(exception_matcher);
        // abc_test includes a set of macros which check for exceptions.
        // Using two of these, the above code can be written in a much shorter
        // way, as shown below.
        matcher_t exception_matcher_2;
        _BEGIN_NO_THROW_MATCHER(exception_matcher_2);
        do_not_optimise(users_average(vect));
        _END_NO_THROW_MATCHER(exception_matcher_2);
        fuzzy_tests << _CHECK(exception_matcher_2);
    }
    _CHECK(fuzzy_tests);
}
```

The folders `examples` and `tests` contain many more examples of code written using `abc_test`. The [hand-written documentation](docs/user_written_docs) is out of date, and is most likely not going to be particularly helpful. However, it is mostly only names of functions and macros that have changed, instead of core functionality - so it may provide some clarity if you're really struggling. The hand-written documentation is due to be re-written for version 0.0.2 of `abc_test`.

Alternatively, you could raise an issue through GitHub Issues if you require some help.

As this is a very early version of the library, please be aware that there could be catastrophic bugs in it. If you do find any bugs, please report them via GitHub Issues.

## Installation ##

To use `abc_test`, the user requires:

- C++23.
- The `fmt` [library](https://github.com/fmtlib/fmt)
- `cmake`

Below is an example `CMakeLists.txt` file that builds an executable called `test_exec`, which utilizes the `abc_test` library. It assumes that `abc_test` has been downloaded and placed in the `abc_test_dir` directory.

```
cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
project(test_exec)
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.2.0
)
FetchContent_MakeAvailable(fmt)
add_subdirectory(abc_test_dir)

add_executable(test_exec main.cpp)
target_link_libraries(test_exec PRIVATE abc_test)
```

Then run the following commands in the same directory as the above `CMakeLists.txt`.

```
mkdir build && cd build
cmake ..
```

We have tested the above script in Windows using the MSVC and Clang compilers, built using Visual Studio 17.13.5. The Clang compiler version used was 18.1.8.

We have also tested it under Ubuntu 24.04 using the GNU and Clang compilers. The GNU compiler version used was 13.3.0, and the Clang compiler version used was 20.1.7. 

## Roadmap ##

This is the initial release of `abc_test`, version 0.0.1.

While a lot of the core functionality is in-place, there is still a lot of work to do before `abc_test`'s full release. These are the core issues we want to address for version 0.0.2.

- Testing. A lot of the core features in `abc_test` lack testing. Without tests, code changes can (and have) broken features - it is in essence a case-study as to why testing is necessary. We are adding tests to `abc_test`, the tests themselves written using `abc_test` - in effect, test bootstrapping. However, to write tests for every part of `abc_test` will take some time. For the next release, we want to have in place some basic tests for data generators, repetition configurations, assertions, macros and logging. These tests will provide us with a safety-net when making further changes to the code.
- Lack of documentation. We have some Doxygen generated documentation and some user-written documentation. However the Doxygen documentation is incomplete for many functions, and the user-written documentation is for an older version of `abc_test`. We have kept it as we believe it will be helpful when writing the new documentation, and could help users wanting to learn more about `abc_test`. For version 0.0.2, we would like the core documentation regarding test declarations, assertions and data generators to be written to a high quality.
- Code clean-up. While we are happy with a lot of the code, there are some portions which are of a lower quality, written with little regard for maintainability or readability. We make no promises for version 0.0.2 regarding how this will be improved, however it will be something we will make an attempt to tackle.
- `abc_test` makes heavy use of generics and template specializations - for example, `default_printer_t` and `default_parser_t` specializations are necessary when working with the file-reading data generator, `default_enumeration_t` specializations are necessary when working with the enumeration data generator, and `default_random_generator_t` specializations are necessary when working with the random data generator. In version 0.0.1, many of these specializations are inconsistent, missing, or defined incorrectly. For version 0.0.2 we want to have addressed this, and hopefully have specializations defined for all of the core types in the C++ standard library.
- The default console and file output requires some improvement. Specifically, some output is poorly labelled, some fields are not being printed correctly, and some fields are missing. For version 0.0.2 we want to have much clearer default output for `abc_test`.
- In `abc_test` many of the functions which work with strings use a mixture of `std::string` and `std::u8string`. For version 0.0.2 we want to provide the user with overloads for each function and type of string.

## License ##

`abc_test` is distributed with the MIT licence. `abc_test` makes use of some other project's source code, which we have modified for our own needs, and included as part of the library's source code. In the `licenses` subfolder, we have included the licenses from those projects.

A complete list of other project's source code included with `abc_test` is as follows:
- `abc_test` uses the biginteger library [found here](https://github.com/benlau/biginteger/tree/master). We have augmented the code by adding constructors to `BigUnsigned` for the `long long` and `unsigned long long` types. We have also changed the names of the headers and cpp files, to better suit the style of `abc_test`.
- `abc_test` includes a version of the `DoNotOptimize` function, included in the Google Benchmark library [found here](https://github.com/google/benchmark/tree/main). We have augmented the code for our needs, however the original code was copied from that project.