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

If the user runs the `abc_test_examples` executable from the root directory with the command line options `--root_path examples/data/test_data --test_paths_to_run readme::fib` then the following output is produced.

<!-- run:build/examples/Debug/abc_test_examples.exe --help:sh -->

Then the following output is produced.

In the second set of output, the only assertions which are ran are those which failed the first test. More specifically, the data generator only produced values which failed an assertion. By providing the executable with the `repetition_config` given as output from the first test, we essentially encoded instructions into the executable to only have the data generator produce those values which previously produced failed assertions. 

This is one of the core features in `abc_test`. It allows the user to hone in on failing test assertions, skipping generated values which only produced passed assertions. To be clear, all data generators in `abc_test` have this functionality.

This functionality works by tracking information about what data generators are currently being iterated through when an assertion fails, and that information is retained. When the test has finished running, a `repetition_config` is produced, which can then be fed back to the test suite when it is re-ran.

### Property Testing Example ###

Property testing and fuzzy testing allow the user to either test some property about their code, or to test their code with many different values. `abc_test`'s generators can be used to add such functionality to a user's test suite.

Below we show an example which tests a user-defined function `users_mid_point`. It performs unit testing, fuzzy testing and property testing.

<!-- inject:examples/include/abc_test_examples/readme.hpp:property_test_example -->

### Fuzzy Testing Example ###

Fuzzy testing is used to test functions with many different values, in essence trying to find examples where the code "breaks". In C++, this behaviour is typically accompanied by an exception being thrown. Below we show an example which performs fuzzy testing on the `users_average` function.

<!-- inject:examples/include/abc_test_examples/readme.hpp:fuzzy_test_example -->

If the reader would like to see more examples of `abc_test`, we recommend reading the hand-written documentation and the documentation generated using Doxygen. 

## Installation ##

`abc_test` targets C++23. Currently `abc_test` requires the `fmt` library. `abc_test` is designed to be built using `cmake`. Below is an example `CMakeLists.txt` file, which builds `abc_test` as part of the project `test_library`. It assumes that `abc_test` has been downloaded and placed in the `abc_test_dir` directory.

```
cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 23)
project(test_library)
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.2.0
)
FetchContent_MakeAvailable(fmt)
add_subdirectory(abc_test_dir)

add_executable(test_library main.cpp)
target_link_libraries(test_library PRIVATE abc_test)
```

Then run the following commands in the same directory as the above `CMakeLists.txt`.

```
mkdir build && cd build
cmake ..
```

We have tested the above script in Windows using the MSVC and Clang compilers, built using Visual Studio 17.13.5. The Clang compiler version used was 18.1.8.

We have also tested it under Ubuntu 24.04 using the GNU and Clang compilers. The GNU compiler version used was 13.3.0, and the Clang compiler version used was 20.1.7. 

## Roadmap ##

As stated in the introduction, `abc_test` is currently a personal project. It was originally conceived from ideas in previous projects of mine, where I was wanting to have both fuzzy testing capabilities alongside unit testing capabilities. It grew significantly, and is now in a state where I want to share it and see what others think, and whether there is any interest in developing it further.

Some of the code is quite "hacky", and could do with being re-written. I am currently in the process of writing tests for `abc_test` in `abc_test`, which has spurred on adding new features, while also allowing me to fix bugs that I find.

## Licence ##

`abc_test` is distributed with the MIT licence. In the `licences` subfolder, we include licences shipped with other source code we have included and/or modified.

Specifically:

- `abc_test` uses the biginteger library [found here](https://github.com/benlau/biginteger/tree/master). We have augmented the code somewhat by adding constructors for the `long long` and `unsigned long long` types. We have also changed the names of the headers and cpp files, to better suit our naming convention.
- `abc_test` includes a version of the `DoNotOptimize` function, included in Google Benchmark library [found here](https://github.com/google/benchmark/tree/main). We have augmented the code for our needs, however the original code was copied from their work.