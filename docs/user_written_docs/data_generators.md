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

Data generators are tightly integrated into `abc_test`. `abc_test`, like many other testing libraries, can be configured to only re-run previously failed test cases. Unlike other testing libraries, `abc_test` extends this functionality to include data generators. A test suite created using `abc_test` can be configured so that each data generator in each test case will only generate values which have previously triggered a failed assertion. When re-running a test case, this allows the user to skip parts which are known to not raise any failed assertions, and instead concentrate on parts which are known to have raised failed assertions. We provide a full guide to this functionality [in this document]().

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
    // This for loop will read int values from the file "integer_file". It does
    // this using a file data generator.
    for (auto& integer : read_data_from_file<int>("integer_file"))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    // This for loop will produce random int values. It does this using a random
    // data generator.
    for (auto& integer : generate_data_randomly<int>())
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
}
```

In the remaining subsections in this introductionary section we discuss three core components concerning data generators; the data generator collection, printers and parsers for data generators, and file interfaces for data generators.

## Data Generator Collections

Each data generator class included with `abc_test` is defined in the namespace `abc::data_gen`. Each data generator object also has a set of factory methods associated with it, which are used to construct an instance of that data generator. These factory methods are all located in the `abc` namespace. `iterate_over`, `read_data_from_file` and `generate_data_randomly` are examples of these factory methods. Details regarding these functions, and the data generators they represent, can be found in latter sections in this document.

`template<typename T> abc::data_gen::data_generator_collection_t` is the primary data type used when working with data generators. It produces entities of type `T`, and is designed to hold multiple data generators. Each of the previously described factory methods returns an object of type `data_generator_collection_t`, the returned object containing a single data generator. `data_generator_collection_t` is the object type which is used in the range-based for loops shown in the previous example; it has a `begin()` function, an `end()` function, and an iterator which produces the for loop's values.

These factory methods are not the only way to create `data_generator_collection_t` objects. There is another factory method in the `abc` namespace called `data_generator_collection`, which takes a variadic number of `data_generator_collection_t` arguments. All of the arguments must have the same template type. It returns a new `data_generator_collection_t` object, which contains all the data generators held in the function's arguments. Through this function, the user is able to group together data generators, allowing them to simplify their code. The `data_generator_collection_t` object also has an overloaded `& operator`, which can be used to combine two `data_generator_collection_t` objects. Below we show an example test case which utilises these two methods of combining `data_generator_collection_t` objects.

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
    // The data_generator_collection_t object created from the for loop below is
    // identical to the one created from the data_generator_collection function
    // above.
    for (auto& integer : iterate_over<int>({1, 2, 3})
                             & generate_data_randomly<int>()
                             & read_data_from_file<int>("int_data"))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
}
```

The reader should note that the object `data_generator_collection_t` (and its associated function `data_generator_collection`) only requires the include directive `#include "abc_test/core.hpp"`. The other factory methods (`iterate_over`, `generate_data_randomly` and `read_data_from_file`) require both this include directive and `#include "abc_test/included_instances.hpp"`.

## Printers & Parsers

Many of the data generators included with `abc_test` have functionality which allows them to interact with files. Part of this functionality relies on there being a printer and parser for the generator's data type `T`. As well as providing printers and parsers for many types included with C++, `abc_test` also allows the user to write their own printers and parsers. In this subsection we provide the documentation for the objects which represent these abstract ideas.

### Printer

Printers in `abc_test` are, in their most basic form, represented by the abstract class `template<typename T> abc::utility::printer::base_printer_t`. It contains one virtual function `run_printer`, with the following type signature. 

```cpp
std::string run_printer(const T& object) const;
```

As `base_printer_t` cannot be instantiated, derived classes must be used to create instances of `base_printer_t`. `abc_test` uses the `template<typename T> printer_t` type, which is a synonym for `std::shared_ptr<base_printer_t>` to refer to an "arbitrary" printer.

There is a single function in the `abc` namespace which can be used to invoke a `printer_base_t` object, defined as follows.

```cpp
template <typename T>
std::string
print(
    const T& object,
    const utility::printer::printer_t<T>& printer = utility::printer::default_printer<T>()
) noexcept;
```

`print` calls `printer`'s `run_printer` function on `object`. We can see that `printer` has a default value, which is created from the function `default_printer`. `default_printer` creates a `printer_t` instance using a subclass of `printer_base_t` included with `abc_test` called `default_printer_t`.

`default_printer_t` is perhaps the most commonly used printer in `abc_test`. It has no default implementation, instead it can only be instantiated using specializations. We include many specializations with `abc_test `for types in the `stl`, as well as auxillery functions that can be used to create `default_printer_t` specializations for objects and enums.

`default_printer` wraps the `default_printer_t` object for `T` in a `printer_t` object. 

There is one other subclass of `base_printer_t` included with `abc_test` called `function_printer_t`. Instances of it can only be created by providing a callable object which takes an object of type `T` and returns an `std::string`. It is designed to allow the user to use an arbitrary function to create their own `printer_t` type. 

Below we show a test case which shows the things above. 

### Parser

`template<typename T> abc::utility::str::parser<T>` is a utility class designed to provide a uniform interface to user-defined parsing functions in `abc_test`. After initialisation, a `parser_t` instance is able to parse text representations to objects of type `T`. `parser_t`'s core "parsing function" is a member function called `run_parser`, which has the following type signature.

```cpp
std::expected<T,std::string> run_parser(const std::string_view str) const noexcept;
```

If the parse of `str` is successful, then `run_parser` returns a `T` entity. If it is unsuccessful, then `run_parser` returns an `std::string` object.

`parser_t` has a private member variable called `parse_function`, whose type is analoguous to a callable function with a type signature identical to `run_parser`'s. Internally, `run_parser` calls `parse_function` using `arg` as its argument.

`parser_t` has an additional member function called `run_parser_with_exception`, which is also used for parsing. It performs identically to `run_parser`, except that it throws an exception if its `std::string_view` argument cannot be parsed. It has the following type signature.

```cpp
T run_parser_with_exception(const std::string_view str) const;
```

`parser_t` instances can be created using one of two factory methods. Both of these are in the `abc` namespace, and are declared as follows.

```cpp
template<typenmae T, typename F>
requires std::invocable<F, const std::string_view>
abc::utility::str::parser<T> parser(F function) noexcept;

template<typename T>
requires abc::utility::str::scannable<T>
abc::utility::str::parser<T> default_parser() noexcept;
```

`parser` takes any function-like object `function` as an argument and creates a `parser_t` object using it. It does this by initializing `parse_function` with `function`. `function` must be a callable object which takes a single argument of type `const std::string_view`, and returns an `std::expected<T,std::string>` object. 

`default_parser` constructs a `parser_t` object only if `T` adheres to the concept `abc::utility::str::scannable`. Assuming it does, `default_parser` creates its own `parser_t` instance using the `scn` library. It does this by initializing `parse_function` using an `std::function` wrapped around the expression `scn::scan(str, "{}")`, where `str` is a `const std::string_view` variable. 

`scannable` is a simple concept which ensures that the object `T` has a `scn::scanner` class specialisation defined.

Below we show some example uses of the `parser_t` object using the functions introduced above.

```cpp
constexpr bool
    operator==(
        const S& lhs,
        const S& rhs
    )
{
    return std::make_pair(lhs.int_1, lhs.int_2)
           == std::make_pair(rhs.int_1, rhs.int_2);
}

template <>
struct scn::scanner<S>
{
    template <typename ParseContext>
    auto
        parse(
            ParseContext& pctx
        ) -> scan_expected<typename ParseContext::iterator>
    {
        return pctx.begin();
    }

    template <typename Context>
    auto
        scan(
            S&       val,
            Context& ctx
        ) -> scan_expected<typename Context::iterator>
    {
        // constexpr std::string _l_str;
        auto result = scn::scan<std::pair<int, int>>(ctx.range(), "{}");
        if (! result)
        {
            return unexpected(result.error());
        }
        else
        {
            val.int_1 = result->value().first;
            val.int_2 = result->value().second;
            return result->begin();
        }
    }
};

_TEST_CASE(
    abc::test_case_t({.name = "Parser object"})
)
{
    using namespace abc;
    using namespace std;
    // int_printer_1 is created using a bespoke function to print int objects.
    auto int_parser_func = [](const std::string_view str)
    {
        int result{};
        auto [ptr, ec]
            = from_chars(str.data(), str.data() + str.size(), result);

        if (ec == std::errc())
        {
            return expected<int, string>(result);
        }
        else
        {
            return expected<int, string>(
                unexpected(fmt::format("Could not parse \"{0}\"", str))
            );
        }
    };
    abc::utility::str::parser_t<int> int_parser_1
        = parser<int>(int_parser_func);
    // int_printer_2 is created using int's fmt::format definition.
    abc::utility::str::parser_t<int> int_parser_2 = default_parser<int>();

    _CHECK_EXPR(int_parser_1.run_parser_with_exception("123") == 123);
    _CHECK_EXPR(int_parser_2.run_parser_with_exception("123") == 123);

    // s_printer_1 is created using a bespoke function to print S objects.
    auto s_parser_func = [](const std::string_view str)
    {
        auto res = scn::scan<pair<int, int>>(string(str), "S {{{}}}");
        if (res.has_value())
        {
            return expected<S, string>(
                S{res->value().first, res->value().second}
            );
        }
        else
        {
            return expected<S, string>(
                unexpected(fmt::format("Could not parse \"{0}\"", str))
            );
        }
    };
    abc::utility::str::parser_t<S> s_parser_1 = parser<S>(s_parser_func);
    // s_printer_2 is created using S's fmt::format definition.
    abc::utility::str::parser_t<S> s_parser_2 = default_parser<S>();

    _CHECK_EXPR(s_parser_1.run_parser_with_exception("S {1, 2}") == (S{1, 2}));
    _CHECK_EXPR(s_parser_2.run_parser_with_exception("{1, 2}") == (S{1, 2}));
}
```

### Strings

We advise the user to be careful when using `std::string` (or any other type which is an alias for `std::basic_string`) with default `printer_t` or `parser_t` objects, as the behaviour may not be what the user expects.

Later on in this document we will show how some data generators in `abc_test` can read and write data to files. They do this using the `printer_t` and `parser_t` objects described in this section. As dicsused previously, `parser_t` and `printer_t` use the libraries `scn` and `fmt` respectively to write their default functions. However, this causes issues for string-like entities such as `std::string`.

To understand why this is, we must first consider how `fmt` and `scn` process `std::string`s by default. If considering a `std::string` object on its own, `fmt` will print it without quotation marks. `scn` behaves similarly; when parsing it will assume there are no quotation marks around the `std::string`.

The default behaviour of the two libraries diverges when `std::string` entities are used with range-based formatters and parsers. Consider the `std::pair<std::string,std::string>` element `{"123", "456}`. `fmt` will print this element in the form `("123", "456")`. `scn` is unable to parse this back to a `std::pair<std::string,std::string>` entity.

To that end `abc_test` provides an alternate to `std::string` called `abc::str_t`. Internally it contains a `std::string` entity. Below we show its two constructors. Both initialise `str_t`'s internal `std::string` entity using the argument.

```cpp
str_t(const char* val);
str_t(const std::string_view _a_str);
```

`str_t`'s `default_printer` will always print out its string using quotation marks. Its `default_parser` will firstly check for quotation marks to parse, before then trying to parse a quoteless `std::string`, terminating on the first blank space.

When working with files, we advise the user to use `str_t` instead of `std::string`. While we know that this is not deal, we believe that, without us providing our own `fmt` and `scn`-like functionality, it is the best solution.


Below we show two test cases which illustrate the aformentioned beheaviour concerning `std::string`s, as well as how to use `str_t`. This example shows how we could write a printer and parser for a bespoke datatype which internally contains `std::string`s. The reader will see from this example that using `str_t`'s for parsing bespoke data structures which contain `std::string` entities can be laborious. 

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Strings and str_t's"})
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    // This pair of printer/parser is able to parse the argument correctly.
    _CHECK_EXPR(
        default_parser<string>().run_parser_with_exception(
            default_printer<string>().run_printer("123")
        )
        == "123"
    );
    auto       pair_parser_1  = default_parser<pair<string, string>>();
    auto       pair_printer_1 = default_printer<pair<string, string>>();
    const auto arg_1          = make_pair("123"s, "456"s);
    // This printer/parser pair is not able to parse the argument correctly.
    // This is because the parser will terminate either at the first whitespace
    // character, or at the end of the string. The printed pair becomes "{123,
    // 456}". It is parsed as {"{123," "456}"}.
    _CHECK_EXPR(
        pair_parser_1.run_parser_with_exception(pair_printer_1.run_printer(arg_1
        ))
        != arg_1
    );
    const auto arg_2 = make_pair("\"123\""s, "\"456\""s);
    // The user could surround the strings by quotes, and assume that the parser
    // would recognise them as escape characters. However this requires
    // scn::scan to be formatted correctly, which isn't how the default parser
    // is defined. So the following won't work.
    _CHECK_EXPR(
        pair_parser_1.run_parser_with_exception(pair_printer_1.run_printer(arg_2
        ))
        != arg_2
    );
    // The user may question why not specialise std::string's default_parser
    // implementation, however this would not fix the issue. If we had a
    // pair<string,string> elements, it would not call the default_parser, it
    // would call the default scn::scan implementation.

    auto       pair_parser_2  = default_parser<pair<str_t, str_t>>();
    auto       pair_printer_2 = default_printer<pair<str_t, str_t>>();
    const auto arg_3          = make_pair(str_t("\"123\""s), str_t("\"456\""s));
    // However this will work. str_t always requires delimited strings -
    // beginning and ending with ".
    _CHECK_EXPR(
        pair_parser_2.run_parser_with_exception(pair_printer_2.run_printer(arg_3
        ))
        == arg_3
    );

    // This will also parse correctly.
    _CHECK_EXPR(
        default_parser<str_t>().run_parser_with_exception(
            default_printer<str_t>().run_printer("\"123\"")
        )
        == "\"123\""
    );
    // However this will not. There MUST be a " to begin a str_t.
    _CHECK_EXPR(
        default_parser<str_t>().run_parser_with_exception(
            default_printer<str_t>().run_printer("123")
        )
        == "123"
    );
}
```

Unfortunately.

### Reader/Writer Information


### Reader/Writer Interface

In `abc_test` there is an object `template<typename T> abc::utility::str::rw_info_t` that provides functionality to print and parse objects between the `std::string` type and the `T` type. By creating instances of this class, the user is able to write their own printers and parsers for either their own type, or currently existing types. 

The type itself consists of two internal objects; an instance of `template<typename T> abc::utility::str::printer` and an instance of `template<typename T> abc::utility::str::parser`. These objects can be created using a callable object signature. 

## Data Generators & Files

In this section we describe the data these files contain, what they are used for, the abstract objects in `abc_test` that represent them, and how to use these objects.

There are two types of data file in `abc_test` used with data generators. These are the general data file (GDF) and the tertiary data file (TDF).

### General Data Files

For a data generator `dg` whose return type is a value of type `T`, a GDF used with `dg` represents a collection of `T` values. `dg` has a general data file interface (GDFI) which describes how to transform each `std::string` extracted from the GDF into an element `T`. The GDFI also describes how to turn `T` elements into `std::string` entities. In essence, the GDF stores text which represents `T` values, the GDFI describes how to parse and print `T` values, and the data generator controls how and when the GDFI reads and writes to the GDF.

`abc_test` uses two objects, `abc::utility::io::general_data_t` and `template<typename T> abc::utility::io::general_data_with_rw_info_t`, to represent data for use with general data files. The object `general_data_t` represents a GDF with a user-provided file name and a default parser and printer, while a `general_data_with_rw_info_t` object represents a GDF with a user-provided file name and a user-provided `rw_info_t` object.

There are three factory methods in the `abc` namespace which can be used to construct `general_data_t` objects. They are defined as follows:

- `abc::utility::io::general_data_t general_data(const std::string_view file_name) noexcept`. This function creates a `general_data_t` object whose internal file name is built using `abc_test` framework's `file_location`, the current test case's `path` and the user-provided `file_name` variable.
- `abc::utility::io::general_data_t general_data(const std::string_view file_name, const std::string_view folder) noexcept`. This function creates a `general_data_t` object whose internal file name is built using `abc_test` framework's `file_location`, the user-provided `folder` variable and the user-provided `file_name` variable.
- `abc::utility::io::general_data_t general_data(const std::filesystem::path path) noexcept`. This function creates a `general_data_t` object whose internal file name is equal to the user-provided `path` variable.

There is one factory method in the `abc` namespace which can be used to construct `general_data_with_rw_info_t` objecst. It is defined as follows:

- `abc::utility::io::general_data_with_rw_info_t<T> gdf_w_rw_info(const abc::utility::io::general_data_t& general_data, const rw_info_t<T>& rw_info) noexcept;`. It constructs a `general_data_with_rw_info_t<T>` object which uses the `general_data_t`'s file entity as its path, and the `rw_info` as its `rw_info_t` element.

Below we show a test case which shows some example uses of these factory methods.

```cpp
_TEST_CASE(
    abc::test_case_t(
        {.name = "General data examples", .path = "abc::general_data"}
    )
)
{
    // Assume the root path is set at "root_path".
    // Assume the default general data file extension "gd" has been used
    using namespace std::filesystem;
    using namespace abc;
    using namespace std;
    // s1 = "root_path/abc/general_data/general\ data\ examples/hello.gd
    string s1 = gdf("hello").path().string();
    // s2 = "root_path/abc/special_folder/hello2.gd.
    string s2 = gdf("hello", "abc/special_folder").path().string();
    // s3 = "special_root/hello/three/hello3.gdf
    string s3
        = gdf_from_path("special_root/hello/three/hello3.gdf").path().string();
    auto printer = printer({}[](const int& i) {return std::to_chars(i);};);
    auto gdf_with_rw_info_1 = gdf_w_rw_info<int>(gdf("file1"),rw_info<int>(printer,default_parser<int>()));
}
```

### Tertiary Data Files

`abc_test` uses uniform terminology and a uniform interface for allowing a data generator to interact with a file. In this subsection we explain this interface in detail.

There are two core file interfaces associated with data generators in `abc_test`. The first is the general data file interface (GDFI). This type of data file interface, associated with a data generator which produces values of type `T`, can be abstractly represented by the triple `(file_name, parser, printer)`, where `file_name` is a file name, `parser` is a function in the form `std::optional<T> parser(const std::string_view)` and `printer` is a function in the form `std::string printer(const T&)`.

A GDFI allows a data generator to read and write values of type `T` to a plain text file. Perhaps the most obvious example we will see [in a future section]() is the file data generator. It can read and write `T` values to a file. This allows the user to write their testing data to a file, without having to re-compile their executable.

However other data generators included with `abc_test` also use a GDFI. For example, the random data generator, the enumeration data generator, and the combinatorics data generator can output failing test cases to a general data file. However these data generaotrs do not read the associated data file.

The second core file interface is the tertiary data file interface (TDFI). This type of file interface is associated with a data generator which produces values of type `T` and some secondary type `U`, can be abstractly represented by the quintuple `(file_name, parser, printer, t_to_u, u_to_t)`, where `file_name` is a file name, `parser` is a function in the form `std::optional<U> parser(const std::string_view)`,`printer` is a function in the form `std::string printer(const U&)`, `t_to_u` is a function in the form `U t_to_u(const T&)` and `u_to_t` is a function in the form `T u_to_t(const U&)`. 

A TDFI allows a data generator to read and write values of type `U` to a plain text file, and to convert them to the data generator's `T` type. This `U` type is specific to the data generaotr itself - for example, a random data generator uses seed values as their tertiary type, and an enumeration data generator and a combinatorics data generator use a `std::size_t` value as an index for their tertiary type.

These abstract types of data generator are 

## Strings

We would not recomend using `std::string` to read or write data to files in `abc_test` - unless using a bespoke `rw_info_t` entity. This is because the default parser (based on `scn`) does not escape `"` characters. Instead we have provided a placeholder called `abc::str`, which includes correct `fmt` and `scn` implementations.

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