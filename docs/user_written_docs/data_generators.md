# Data Generators

This page contains documentation concerning data generators in `abc_test`. It covers the following topics:

- Data Generator Basics.
- Static Data Generator.
- File Data Generator.

This document will assume the reader is familiar with the following documents:

- [Test Assertions]()
- [Matchers]()

All of the functionality shown in this document requires the following two include directives.

```cpp
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
```

Some functionality only requires the following include directive.

```cpp
#include "abc_test/core.hpp"
```

We will make it clear to the reader when this is the case.

At various points in this document we will show the reader some output from `abc_test`. In all instances the output we will show has been created using the `text_test_reporter_t` object, initialized using the object's default constructor. The test suite used to create the output has been configured to show both passed and failed assertions, and passed and failed matchers. We have also ammended the output slightly to make the source locations easier to read.

# Data Generator Basics

A data generator in `abc_test` is an abstract object which produces some arbitrary, user-defined sequence of data values. The created data can then be used in assertions, matchers and test cases. There are several different types of data generator included with `abc_test`; for example, there is a data generator whose values are copied from some user-defined collection, a data generator whose values are read from some user-defined file, and a data generator whose values are produced by some user-defined random function. In this document we provide the documentation for all of the data generators included with `abc_test`.

Data generators are tightly integrated into `abc_test`. `abc_test`, like many other testing libraries, can be configured to only re-run previously failed test cases. Unlike other testing libraries, `abc_test` extends this functionality to include data generators. A test suite created using `abc_test` can be configured so that each data generator in each test case will only generate values which have previously triggered a failed assertion. This allows the user to skip parts of a test case which are known to succeed, and allows them to concentrate on where the test case failed. We provide a full guide to this functionality [in this document]().

Data generators are designed to be used with a ranged-based for loop. Below we show a simple example, which uses what we call a static data generator, a file data generator, and a random data generator. In all the examples in this document we use multi-element block-based assertions. This type of assertion was specifically designed to be used with the data generator object, and we would recommend that the reader consider using them in their own testing code whenever they are using a data generator.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Simple data generators"})
)
{
    using namespace abc;
    // This for loop will produce 3 int values; 1, 2 and 3. It does this using a
    // static data generator.
    _BEGIN_MULTI_ELEMENT_BBA(check_numbs, "Checks the data generator's output");
    for (const int& integer : iterate_over<int>({1, 2, 3}))
    {
        // The element produced by the range-based for loop is a cref. All of
        // the for loops in this test case produce int crefs.
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
    // This for loop will read int values from the file "integer_file". It does
    // this using a file data generator.
    _BEGIN_MULTI_ELEMENT_BBA(check_numbs, "Checks the data generator's output");
    for (auto& integer : read_data_from_file<int>("integer_file"))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
    // This for loop will produce random int values. It does this using a random
    // data generator.
    _BEGIN_MULTI_ELEMENT_BBA(check_numbs, "Checks the data generator's output");
    for (auto& integer : generate_data_randomly<int>())
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
}
```

In the remaining subsections in this section we discuss two core components concerning data generators; the data generator collection and abstract objects used to represent files for interacting with data generators.

## Data Generator Collections

Each data generator included with `abc_test` is defined in the namespace `abc::data_gen`. Each data generator also has a set of static functions associated with it, which can be used to construct an instance of that data generator. These static functions are all located in the `abc` namespace. `iterate_over`, `read_data_from_file` and `generate_data_randomly` are examples of these types of functions. Details regarding these functions, and the data generators they represent, can be found in latter sections in this document.

`template<typename T> abc::data_gen::data_generator_collection_t` is the primary data type used when working with data generators. It produces entities of type `T`, and is designed to hold multiple data generators. Each of the previously described static functions returns an object of type `data_generator_collection_t`, the returned object containing a single data generator. `data_generator_collection_t` is the object type which is used in the range-based for loops shown in the example above; it has a `begin()` function, an `end()` function, and an iterator which produces the for loop's values.

These static functions are not the only way to create `data_generator_collection_t` objects. There is also a static function in the `abc` namespace called `data_generator_collection`, which takes a variadic number of `data_generator_collection_t` arguments. All of the arguments must have the same template type. It returns a new `data_generator_collection_t` object, which contains all the data generators held in the function's arguments. Through this function, the user is able to group together data generators, allowing them to simplify their code. The `data_generator_collection_t` object also has an overloaded `& operator`, which can be used to combine two `data_generator_collection_t` objects. Below we show an example test case which utilises these two methods of combining `data_generator_collection_t` objects.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Simple data generators, part 2"})
)
{
    using namespace abc;
    // This for loop will navigate through the data generators in the order they
    // are given; firstly it will produce 1, 2 and 3, then it will produce
    // random values, finally it will read data from the file "int_data".
    _BEGIN_MULTI_ELEMENT_BBA(check_numbs, "Checks the data generator's output");
    for (auto& integer : data_generator_collection<int>(
             iterate_over<int>({1, 2, 3}),
             generate_data_randomly<int>(),
             read_data_from_file<int>("int_data")
         ))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
    // The data_generator_collection_t object created from the for loop below is
    // identical to the one created from the data_generator_collection function
    // above.
    _BEGIN_MULTI_ELEMENT_BBA(check_numbs, "Checks the data generator's output");
    for (auto& integer : iterate_over<int>({1, 2, 3})
                             & generate_data_randomly<int>()
                             & read_data_from_file<int>("int_data"))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
}
```

The reader should note that the object `data_generator_collection_t` (and its associated function `data_generator_collection`) only requires the include directive `#include "abc_test/core.hpp"`, while all of the data generators included with `abc_test` require both this include directive and `#include "abc_test/included_instances.hpp"`.

## Representing Files

Many of the data generators included with `abc_test` are able to interact with files. In this section we describe the data these files contain, what they are used for, the abstract objects in `abc_test` that represent them, and how to use these objects.

There are two types of data file in `abc_test` used with data generators. These are the general data file (GDF) and the tertiary data file (TDF).

### General Data Files

For a data generator `dg` whose return type is a value of type `T`, a GDF used with `dg` represents a collection of `T` values. `dg` has a general data file interface (GDFI) which describes how to transform each `std::string` extracted from the GDF into an element `T`. The GDFI also describes how to turn `T` elements into `std::string` entities. In essence, the GDF stores text which represents `T` values, the GDFI describes how to parse and print `T` values, and the data generator controls how and when the GDFI reads and writes to the GDF.

`abc_test` uses two objects, `abc::utility::io::general_data_t` and `template<typename T> abc::utility::io::general_data_with_rw_info_t`, to represent data for use with general data files. 

`abc_test` uses uniform terminology and a uniform interface for allowing a data generator to interact with a file. In this subsection we explain this interface in detail.

There are two core file interfaces associated with data generators in `abc_test`. The first is the general data file interface (GDFI). This type of data file interface, associated with a data generator which produces values of type `T`, can be abstractly represented by the triple `(file_name, parser, printer)`, where `file_name` is a file name, `parser` is a function in the form `std::optional<T> parser(const std::string_view)` and `printer` is a function in the form `std::string printer(const T&)`.

A GDFI allows a data generator to read and write values of type `T` to a plain text file. Perhaps the most obvious example we will see [in a future section]() is the file data generator. It can read and write `T` values to a file. This allows the user to write their testing data to a file, without having to re-compile their executable.

However other data generators included with `abc_test` also use a GDFI. For example, the random data generator, the enumeration data generator, and the combinatorics data generator can output failing test cases to a general data file. However these data generaotrs do not read the associated data file.

The second core file interface is the tertiary data file interface (TDFI). This type of file interface is associated with a data generator which produces values of type `T` and some secondary type `U`, can be abstractly represented by the quintuple `(file_name, parser, printer, t_to_u, u_to_t)`, where `file_name` is a file name, `parser` is a function in the form `std::optional<U> parser(const std::string_view)`,`printer` is a function in the form `std::string printer(const U&)`, `t_to_u` is a function in the form `U t_to_u(const T&)` and `u_to_t` is a function in the form `T u_to_t(const U&)`. 

A TDFI allows a data generator to read and write values of type `U` to a plain text file, and to convert them to the data generator's `T` type. This `U` type is specific to the data generaotr itself - for example, a random data generator uses seed values as their tertiary type, and an enumeration data generator and a combinatorics data generator use a `std::size_t` value as an index for their tertiary type.

These abstract types of data generator are 

# Static Data Generator

A static data generator is the simplest data generator included with `abc_test`. It is represented by the type `template <typename T> static_data_generator_t`, and requires the include directive `#include "abc_test/included_instances.hpp"` to use.

The `static_data_generator_t` object has a simple design. It contains a set of objects of type `T` which are iterated over to produce the data generator's values. It has one constructor, which takes any range-like object `range` and copies `range`s elements to its internal memory. 

We recommend that the user create instances of the static data generator using the static function `iterate_over`. It is located in the `abc` namespace, and has the following type signature.

```cpp
template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
data_generator_collection_t<T> iterate_over(R&& range);
```

`iterate_over` will create a `data_generator_collection_t` object containing a single `static_data_generator_t` object we will call `dg`. `dg` is created using `static_data_generator_t`'s range-based constructor, called using the `range` argument given to `iterate_over`.

A static data generator is designed to allow the user to encode any range-like collection into a data generator object. As its constructor copies its range argument into its internal memory, its use can potentially be a disadvantage when considering efficiency. In addition to this, if the range needs to be changed by the programmer, then the entire executable will have to be re-compiled, which the user may not find desirable.

Some use-cases are a good fit for the static data generator. For example, if we wanted a data generator which contained Fibonacci numbers, it would be much easier and less error-prone to write an algorithm to compute them when compared to calculating them by hand. 

Below we show a test case which contains a good and a bad example use of the static data generator. In the good example, a static data generator is populated with Fibonacci numbers. The for loop's body tests each value to ensure it is greater than the previous one - effectively the assertion is performing a rudamentory integer overflow test.

The bad example shows a static data generator which contains hand-written values. If the user wishes to add to this set of values, or to change them, then a re-compilation of the executable is required. In the next section we show the reader the file data generator, which we believe is much better suited to handling hand-written data.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Static generator examples"})
)
{
    using namespace abc;
    using namespace std;
    auto fib_func = [](const size_t max_index)
    {
        vector<uint64_t> rv;
        for (size_t idx{0}; idx <= max_index; ++idx)
        {
            if (idx == 0)
            {
                rv.push_back(0);
            }
            else if (idx == 1)
            {
                rv.push_back(1);
            }
            else
            {
                auto itt_end{rv.end() - 1};
                rv.push_back(*itt_end + *(itt_end - 1));
            }
        }
        return rv;
    };
    optional<uint64_t> last;
    _BEGIN_MULTI_ELEMENT_BBA(
        fibonacci, "Test for overflow in uint64_t-based fibonacci function"
    );
    // This data generator stores the first 100 fibonacci numbers. The test
    // checks for overflow.
    for (auto& integer : iterate_over<uint64_t>(fib_func(100)))
    {
        if (last.has_value())
        {
            fibonacci += _BLOCK_CHECK(_EXPR(last.value() <= integer));
        }
        last = integer;
    }
    _END_BBA_CHECK(fibonacci);

    _BEGIN_MULTI_ELEMENT_BBA(to_string_check, "Test std::to_string function");
    // This data generator should probably not be encoded using a static data
    // generator. If the user wants to add more test data, then they will need
    // to re-compile the executable.
    using test_data_1 = pair<int, string>;
    for (auto& [integer, str] : iterate_over<test_data_1>({
             {1,   "1"  },
             {2,   "2"  },
             {100, "102"}
    }))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(std::to_string(integer) == str));
    }
    _END_BBA_CHECK(to_string_check);
}
```

# File Data Generator

A file data generator is a data generator used when wanting to use hand-written values with a data generator. It is represented by the type `template <typename T> file_data_generator_t`, and requires the include directive `#include "abc_test/included_instances.hpp"` to use.

A `file_data_generator_t` object, as the name suggests, is able to read (and write) data to an associated file. The object has several configrable components, described as follows:

- A `abc::utility::str::parser<T>` component called the `parser`. It contains a function for reading `std::string`s and converting them into `T` objects.
- A `abc::utility::str::printer<T>` component called the `printer`. It contains a function for turning an object of type `T` into a `std::string` object.
- A `std::filesystem::path` component called the `file_location`. This is a path where the test case's files are located. 
- A `std::filesystem::path` component called the `file_name`. This is the specific file which contains this data generator's test data in the path `file_location`.

All of these components, apart from `file_name`, can have default values. These are described as follows:

- The `parser`'s default value is created using the expression `scn::scan<T>(str, "{}"))`. If it will not compile (because `T` does not have a `scan` function) then a compile-time error is thrown.
- The `printer`'s default value is created using the expression `fmt::format("{}",obj)`. If it will not compile (because `T` has no `format` function) then a compile-time error is thrown.
- The `file_location` is set to the test case's `path` value. The user is able to set the root of the tests data using command line arguments (see this document), allowing it to be set correctly.