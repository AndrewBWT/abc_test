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

<!-- inject:scripts/script_to_use_library/main_to_cpy.cpp:initial_example -->

### Unit Testing Example ###

The example above could be considered a unit test for the `fib` function. However, writing the test case in this way has one major shortcoming; it relies on the user re-writing the same statement each time they want to test a new value with the `fib` function. In turn, this requires re-compiling the test executable.

`abc_test` contains many different types of data generators which can be used to iterate over data. One of these data generators is the file-based data generator. It can be used to write unit tests, where the values to test are taken from a file. Below we show an example of how it can be used to write a unit test. Adding new values to the test only requires adding values to the file - no re-compilation is necessary.

<!-- inject:examples/include/abc_test_examples/readme.hpp:unit_test_example -->

If the user runs the `abc_test_examples` executable from the project's root directory with the options `--root_path examples/data/test_data --test_paths_to_run readme::fib`, then the following is an example of the output which could be produced.

<!-- run:build/examples/Debug/abc_test_examples.exe --root_path examples/data/test_data --test_paths_to_run readme::fib:sh -->

If the user were to then run the same executable from the same location with the options `--root_path ../../examples/data/test_data --test_paths_to_run readme::fib --repetition_config 54657374696e67204669626f6e616363692066756e6374696f6e207573696e6720646174612066726f6d20612066696c653a3a726561646d653a3a666962:5b5b28302c302c2228302c203329222c5b5d295d5d --write_data_to_files false`, then the following is an example of the output which could be produced.

<!-- run:build/examples/Debug/abc_test_examples.exe --root_path examples/data/test_data --test_paths_to_run readme::fib --repetition_config 54657374696e67204669626f6e616363692066756e6374696f6e207573696e6720646174612066726f6d20612066696c653a3a726561646d653a3a666962:5b5b28302c302c2228302c203329222c5b5d295d5d --write_data_to_files false:sh -->

In the second run, the only assertions which are evaluated are those which failed during the first run. `abc_test` configured the data generator so that it only produced values which had previously failed an assertion. By providing the executable with the `repetition_config` given as output from the first run, we encoded instructions into the executable to behave in this manner.

This is one of the core features in `abc_test`. It allows the user to hone in on failing test assertions, skipping generated values which did not produce failed assertions. To be clear, all data generators in `abc_test` have this functionality.

This feature works by tracking information about what data generators are currently being iterated through when an assertion fails, and that information is memoized. When the test suite has finished running, a string of hex digits called a `repetition_config` is produced, which represents this information for each test that encountered at least one failed assertion. This `repetition_config` can then be given back to the test suite when it is re-ran, ensuring that the data generator's only produce those values which triggered a failed test assertion.

### Property Testing Example ###

Property testing allows the user to test some property about their code. In this context a "property" is a logical expression about some function or sequence of functions. Properties are usually tested with some randomly generated data, or data which comes from a user-defined range of values. `abc_test` includes data generators which make writing property tests simple.

Below we show an example which tests a user-defined function `users_mid_point`. The test case checks two properties; the first is that the result of `users_mid_point` matches `std::midpoint`. The second is that changing the order of the arguments given to `users_mid_point` doesn't change the result.

<!-- inject:examples/include/abc_test_examples/readme.hpp:property_test_example -->

### Fuzzy Testing Example ###

Fuzzy testing allows the user to stress-test their code; typically this is by using random or obscure data on it to see if it "breaks". in C++, "breaking" usually means the code throws an exception or crashes. Below we show an example of `abc_test` being used to perform fuzzy testing on the function `users_average`.

<!-- inject:examples/include/abc_test_examples/readme.hpp:fuzzy_test_example -->

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