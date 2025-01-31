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

All of the classes and functions described in this subsection are contained in the namespace `abc::utility::printer`. 

Printers in `abc_test` are, in their most basic form, represented by the abstract class `template<typename T> printer_base_t`. `printer_base_t` objects are specifically designed to allow the user to print entities of type `T`. Below we show an overview of the class.

```cpp
template<typename T>
class printer_base_t
{
private:
    using value_type_t = T;
    std::string run_printer(const T& object) const = 0;
};
```


As the `printer_base_t` class cannot be instantiated, the user must use classes derived from it to create concrete objects, which can then be used to print text representations of objects of type `value_type_t`. `abc_test` contains the type synonym `template<typename T> printer_t` to represent `std::unique_ptr<base_printer_t>` objects. It uses this type synonym to represent concrete `printer_base_t` objects, allowing them to be passed around the `abc_test` framework.

There are two core functions which should be used when creating and running printers. They are outlined below.

```cpp
template <typename T>
std::string print(
    const T&                              object,
    const printer_t<T>&                   printer_obj
    = mk_printer(default_printer_t<T>())
) noexcept;

template <typename T>
printer_t<typename T::value_type>
            mk_printer(const T& _a_printer_base) noexcept;
```

`print` is the function designed to be used when printing data. As arguments it takes the entity `object` to be printed, and the `printer_t` object `printer_obj`. It runs the `object` through `printer_obj`, returning the `std::string` result from `printer_obj`. `printer_obj` is default initialized using the `default_printer_t` object and the function `mk_printer`. The `mk_printer` function is very simple; it converts an arbitrary `base_printer_t` object into a `printer_t` object. The `default_printer_t` object is outlined below.

There are two concrete classes derived from `base_printer_t` included with `abc_test`. The first of these is the `template<typename T> default_printer_t` object. It has no default implementation, and instead relies on there being a specialization for its `T` template parameter. `abc_test` provides implementations for many of the classes included with the `stl`. As seen previously, the function `print` uses a `default_printer_t` object if no `printer_t` argument is supplied.

The user is encouraged to create specializations for `default_printer_t` for their own classes. In the test case shown at the end of this section, we show an example of this.

The reader should note there is an auxillery function associated with `default_printer_t` called `default_printer`. It creates and wraps a `default_printer_t` in a `printer_t` object. `default_printer` takes an arbitrary number of arguments, all of which are passed to the constructor for `default_printer_t`. Its type signature is as follows.

```cpp
template<typename T, typename ... Ts>
 printer_t<T> default_printer(Ts ... elements) noexcept;
```

The other class derived from `base_printer_t` included with `abc_test` is the class `template<typename T> function_printer_t`. Below we show an overview of this class. We also show the reader how it is implemented, which should aid in their understanding.

```cpp
template<typename T, typename F>
requires std::is_invocable_r_v<std::string, F, const T&>
class function_printer_t
{
public:
    function_printer_t() = delete;
    function_printer_t(F _callable_object) noexcept : callable_object(_callable_object) {}
    std::string run_printer(const T& object) const
    {
        return std::invoke(callable_object, object);
    }
private:
    F callable_object;
};
```

This class is designed to allow the user to use their own callable object when interfacing with `abc_test`'s printer facilities, meaning they do not necessarily have to define their own subclass of `base_printer_t` when writing their own printer. In a similar vein to `default_printer`, there is an auxillery function associated with `function_printer_t` called `function_printer`. It creates and wraps a `function_printer` in a `printer_t` object. Its type signautre is as follows.

```cpp
template<typename T, typename F>
    requires std::is_invocable_r_v<std::string, F, const T&>
printer_t<T> function_printer(F _a_callable) noexcept;
```

Below we show a test case which contains several examples showing how printers work. This example also shows the user how to define their own `default_printer_t` instances, and their own classes derived from `base_printer_t` .

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Printers in abc_test"})
)
{
    using namespace abc;
    using namespace std;
    // The printer functionality is contained in the abc::utility::printer
    // namespace.
    using namespace abc::utility::printer;
    // As we cannot create instantiations of base_printer_t, we will show the
    // user the included subclasses which use it as their base class.

    // The first of these is default_printer_t.
    default_printer_t<int> printer_1;
    _CHECK_EXPR(printer_1.run_printer(123) == "123");

    // We recommend that the user use the function print when printing. It takes
    // two arguments; the first is the object to be printed, and the second is
    // the printer_t object. Any derived type from base_printer_t can be
    // elevated to this type using mk_printer.

    _CHECK_EXPR(print(123, mk_printer(printer_1)) == "123");

    // We can also use the specialised default_printer function, which will
    // create and wrap a default_printer_t in a printer_t object.
    _CHECK_EXPR(print(123, default_printer<int>()) == "123");

    // print has a default parameter for its second argument, which creates a
    // default_printer_t object. Therefore, to get the default behaviour, no
    // argument needs to be provided.
    _CHECK_EXPR(print<int>(123) == "123");

    // abc_test includes default_printer_t specializations for more complicated
    // types, such as std::tuple and std::vector.
    using tuple_type = tuple<int, string, vector<bool>>;
    tuple_type tuple_1{
        123, "h3llo"s, vector<bool>{true, false, true}
    };
    _CHECK_EXPR(print(tuple_1) == "(123, \"h3llo\", {true, false, true})");

    // A function_printer_t object allows the user to create a base_printer_t
    // object which can take any callable object which takes a const T& and
    // returns an std::string.

    // Here is a function which prints a bool.
    auto bool_printer = [](const bool& object) -> std::string
    {
        return object ? "1" : "0";
    };
    // We can now use it to create a function_printer_t object.
    function_printer_t<bool, decltype(bool_printer)> fp1(bool_printer);
    _CHECK_EXPR(print(true, mk_printer(fp1)) == "1");

    // We can use the auxillery function function_printer, which removes some of
    // the boilerplate code required.
    _CHECK_EXPR(print(true, function_printer<bool>(bool_printer)) == "1");

    // Using bool_printer, we can use the specialized default_printer_t
    // constructors for the types std::tuple and std::vector to
    // change how tuple_1 is printed. 
    _CHECK_EXPR(
        print(
            // Printing tuple_1
            tuple_1,
            // Make a printer using the default type of the tuple.
            default_printer<tuple_type>(
                // But initialized with our own printers for each of the
                // tuple's types.
                default_printer<int>(),
                default_printer<string>(),
                // The last type is changed. It uses the specialized
                // default_printer_t constructor for vector.
                default_printer<vector<bool>>(
                    // But it uses its own printer_t type for bool, created
                    // using function_printer.
                    function_printer<bool>(bool_printer)
                )
            )
        )
        == "(123, \"h3llo\", {1, 0, 1})"
    );
    // An easier way to get the same effect is as follows: firstly
    // create a default printer for tuple_type.
    default_printer_t<tuple_type> tuple_printer;
    // Then change the third field.
    std::get<2>(tuple_printer.get_printers_ref())
        = default_printer<vector<bool>>(function_printer<bool>(bool_printer));
    // Then run the printer using mk_printer to elevate tuple_printer to
    // a printer_t type.

    _CHECK_EXPR(
        print(tuple_1, mk_printer(tuple_printer))
        == "(123, \"h3llo\", {1, 0, 1})"
    );
}

// This is a user-defined object.
struct S
{
    int int_1;
    int int_2;
};

// This is how we create a default_printer_t instance for S.
template <>
struct abc::utility::printer::default_printer_t<S>
    : public abc::utility::printer::printer_base_t<S>
{
    inline std::string
        run_printer(
            const S& _a_object
        ) const noexcept
    {
        // We use the object object_printer_parser_t and the function
        // object_printer. We give it the default object_printer_parser_t
        // object, a string which is prepended to the beginning of the list of
        // data fields, the names of the data fields, and then the fields
        // themselves
        return object_printer_with_field_names(
            object_printer_parser_t{},
            "S",
            {"int_1", "int_2"},
            _a_object.int_1,
            _a_object.int_2
        );
    }
};

// This is a bespoke printer_base_t instance for int. It prints the data out in
// hex.
struct int_printer_t : abc::utility::printer::printer_base_t<int>
{
    inline std::string
        run_printer(
            const int& _a_object
        ) const noexcept
    {
        return fmt::format("{:x}", _a_object);
    }
};

// This is a bespoke printer for S.
struct s_printer : public abc::utility::printer::printer_base_t<S>
{
    abc::utility::printer::printer_t<int> m_int_printer
        = abc::utility::printer::default_printer<int>();
    s_printer() = default;

    inline s_printer(
        const abc::utility::printer::printer_t<int>& int_printer
    )
        : m_int_printer(int_printer)
    {}

    inline std::string
        run_printer(
            const S& _a_object
        ) const noexcept
    {
        // This printer uses the function object_printer_with_custom_printers
        // with a bespoke object_printer_parser_t object.
        // object_printer_with_custom_printers takes a tuple of bespoke printers
        // for each field it is provided.
        return object_printer_with_custom_printers(
            abc::utility::object_printer_parser_t{
                .begin_char = '(',
                .end_char   = ')',
            },
            "S",
            std::make_tuple(m_int_printer, m_int_printer),
            _a_object.int_1,
            _a_object.int_2
        );
    }
};

_TEST_CASE(
    abc::test_case_t({.name = "Custom printers"})
)
{
    using namespace abc;
    using namespace abc::utility::printer;
    S s1{10'000, 20'000};
    // This will call the default printer on S.
    _CHECK_EXPR(print(s1) == "S {int_1 = 10000, int_2 = 20000}");

    // This printer uses the bespoke printer s_printer.
    _CHECK_EXPR(print(s1, mk_printer(s_printer())) == "S (10000, 20000)");

    // This also uses the bespoke printer s_printer, but we use int_printer_t to
    // print S's integers.
    _CHECK_EXPR(
        print(s1, mk_printer(s_printer(mk_printer(int_printer_t()))))
        == "S (2710, 4e20)"
    );
}
```

### Parser

All of the classes and functions described in this subsection are contained in the namespace `abc::utility::parser`. 

Parsers in `abc_test` are, in their most basic form, represented by the abstract class `template<typename T> parser_base_t`. `parser_base_t` objects are specifically designed to allow the user to parse entities of type `T`. This functionality is realised using a class representing text `parser_input_t` and a type synonym representing a parser's result `template<typename T> parse_result<T>`.

Below we show the definition of `parser_result`, and an overview of the `parser_base_t` class.

```cpp
template <typename T>
using parse_result_t = std::expected<T, std::string>;

template <typename T>
struct parser_base_t
{
public:
    using value_type_t = T;
    virtual parse_result_t<T>
        run_parser(parser_input_t& _a_parse_input) const = 0;
};
```

The `parse_input_t` class can abstractly be thought of as a `std::string_view`. In reality, it uses a set of iterators, together with specialised functions for parsing text. When the reader is writing their own parsers using `abc_test`, we recomend them checking the class definition of `parse_input_t`. 


As the `parser_base_t` class cannot be instantiated, the user must use classes derived from it to create concrete objects, which can then be parse text representations of objects of type `value_type_t`. `abc_test` contains the type synonym `template<typename T> parser_t` to represent `std::unique_ptr<base_parser_t<T>>` objects. It uses this type synonym to represent concrete `parser_base_t` objects, allowing them to be passed around the `abc_test` framework.

There are three core functions which should be used when creating and running parsers. They are outlined below.

```cpp
template <typename T>
parse_result_t<T> parse(
    const std::string_view str,
    const parser_t<T>&     parser_obj = mk_parser(default_parser_t<T>())
) noexcept;
template <typename T>
T run_parser_with_exception(
    const std::string_view str,
    const parser_t<T>&     parser_obj = mk_parser(default_parser_t<T>())
);
template <typename T>
parser_t<typename T::value_type_t> mk_parser(
    T parser_base_obj
) noexcept;
```

`parse` and `run_parser_with_exception` are the functions designed to be used when parsing data. As arguments both take the `std::string_view` `str` to be parsed, and the `parser_t` object `parser_obj`. It runs `str` through `parser_obj`, returning either a `parse_result_t<T>` object (for `parse`), or a `T` object (from `run_parser_with_exception`). `run_parser_with_exception` will throw an exception if parsing fails. In both functions `parser_obj` is default initialized using the `default_parser_t` object and the function `mk_parser`. The `mk_parser` function is very simple; it converts an arbitrary `parser_base_t` object into a `parser_t` object. The `default_parser_t` object is outlined below.

There are two concrete classes derived from `parser_base_t` included with `abc_test`. The first of these is the `template<typename T> default_parser_t` object. It has no default implementation, and instead relies on there being a specialization for its `T` template parameter. `abc_test` provides implementations for many of the classes included with the `stl`. As seen previously, the functions `parse` and `run_parser_with_exception` use a `default_parser_t` object if no `parser_t` argument is supplied.

The user is encouraged to create specializations for `default_parser_t` for their own classes. In the test case shown at the end of this section, we show an example of this.

The reader should note there is an auxillery function associated with `default_parser_t` called `default_parser`. It creates and wraps a `default_parser_t` in a `parser_t` object. `default_parser` takes an arbitrary number of arguments, all of which are passed to the constructor for `default_parser_t`. Its type signature is as follows.

```cpp
template <typename T, typename ... Ts>
parser_t<T> default_parser(Ts ... elements) noexcept;
```

The other class derived from `parser_base_t` included with `abc_test` is the class `template<typename T> function_parser_t`. Below we show an overview of this class. We also show the reader how it is implemented, which should aid in their understanding.

```cpp
template <typename T, typename F>
    requires std::is_invocable_r_v<parse_result_t<T>, F, parser_input_t&>
struct function_parser_t : public parser_base_t<T>
{
public:
    function_parser_t() = delete;
    function_parser_t(F _callable) noexcept : callable(_callable) {}
    parse_result_t<T> run_parser(
        parser_input_t& parse_input) const override
    {
        return std::invoke_r(_callable,parse_input);
    }
private:
    F callable;
};
```

This class is designed to allow the user to use their own callable object when interfacing with `abc_test`'s parser facilities, meaning they do not necessarily have to define their own subclass of `base_parser_t` when writing their own parser. In a similar vein to `default_parser`, there is an auxillery function associated with `function_parser_t` called `function_parser`. It creates and wraps a `function_parser_t` in a `parser_t` object. Its type signautre is as follows.

```cpp
template <typename T, typename F>
    requires std::is_invocable_r_v<parse_result_t<T>, F, parser_input_t&>
__constexpr parser_t<T> function_parser(F _a_callable) noexcept;
```

Below we show a test case which contains several examples showing how parsers work. This example also shows the user how to define their own `default_parser_t` instances, and their own classes derived from `base_parser_t`. This example uses the struct `S` defined in the example shown in the previous subsection. The reader should note that there is an unseen equals operator defined for `S`, to ensure that the tests are able to work correctly.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "Parsers in abc_test"})
)
{
    using namespace abc;
    using namespace std;
    // The parser functionality is contained in the abc::utility::parser
    // namespace.
    using namespace abc::utility::parser;
    // As we cannot create instantiations of base_parser_t, we will show the
    // user the included subclasses which use it as their base class.

    // The first of these is default_printer_t.
    default_parser_t<int> parser_1;
    // We can elevate it to the parser_t type using mk_parser.
    _CHECK_EXPR(parse("123", mk_parser(parser_1)) == parse_result_t<int>(123));
    // For the rest of the assertions in this test case, we will use the
    // function parse_with_exception. This removes the need for the
    // parse_result_t type.
    _CHECK_EXPR(parse_with_exception<int>("123", mk_parser(parser_1)) == 123);

    // We can also use the default_parser function instead of using mk_parser.
    _CHECK_EXPR(parse_with_exception<int>("123", default_parser<int>()) == 123);

    // As parse_with_exception (and parse) have their second argument defaulted
    // to a default parser, we can just write.
    _CHECK_EXPR(parse_with_exception<int>("123") == 123);


    // abc_test includes default_parser_t specializations for more complicated
    // types, such as std::tuple and std::vector.
    using tuple_type = tuple<int, string, vector<bool>>;
    tuple_type tuple_1{
        123, "h3llo"s, vector<bool>{true, false, true}
    };
    _CHECK_EXPR(
        parse_with_exception<tuple_type>("(123, \"h3llo\", {true, false, true})"
        )
        == tuple_1
    );

    // A function_parser_t object allows the user to create a base_parser_t
    // object which can take any callable object which takes a parser_input_t&
    // and returns a parse_result<T>.

    // Here is a function which parses a bool represented as a 0 or 1.
    auto bool_parser = [](parser_input_t& input) -> parse_result_t<bool>
    {
        if (*input == '1')
        {
            input.advance(1);
            return parse_result_t<bool>(true);
        }
        else if (*input == '0')
        {
            input.advance(1);
            return parse_result_t<bool>(false);
        }
        else
        {
            return parse_error<bool>(
                "Could not recognise character. Should be 0 or 1"
            );
        }
    };
    // We can now use it to create a function_parser_t object.
    function_parser_t<bool, decltype(bool_parser)> fp1(bool_parser);
    _CHECK_EXPR(parse_with_exception<bool>("1", mk_parser(fp1)) == true);

    // We can use the auxillery function function_parser, which removes some of
    // the boilerplate code required.
    _CHECK_EXPR(
        parse_with_exception<bool>("1", function_parser<bool>(bool_parser))
        == true
    );

    // Using bool_parser, we can use the specialized default_parser_t
    // constructors for the types std::tuple and std::vector to
    // change how tuple_1 is printed.
    _CHECK_EXPR(
        parse_with_exception(
            // Printing the following string
            "(123, \"h3llo\", {1, 0, 1})",
            // Make a parser using the default type of the tuple.
            default_parser<tuple_type>(
                // But initialized with our own parsers for each of the
                // tuple's types.
                default_parser<int>(),
                default_parser<string>(),
                // The last type is changed. It uses the specialized
                // default_parser_t constructor for vector.
                default_parser<vector<bool>>(
                    // But it uses its own parser_t type for bool, created
                    // using function_parser.
                    function_parser<bool>(bool_parser)
                )
            )
        )
        == tuple_1
    );
    // An easier way to get the same effect is as follows: firstly
    // create a default printer for tuple_type.
    default_parser_t<tuple_type> tuple_parser;
    // Then change the third field.
    std::get<2>(tuple_parser.get_parsers_ref())
        = default_parser<vector<bool>>(function_parser<bool>(bool_parser));
    // Then run the printer using mk_printer to elevate tuple_printer to
    // a printer_t type.

    _CHECK_EXPR(
        parse_with_exception(
            "(123, \"h3llo\", {1, 0, 1})", mk_parser(tuple_parser)
        )
        == tuple_1
    );
}

// This is how we create a default_printer_t instance for S.
template <>
struct abc::utility::parser::default_parser_t<S>
    : public abc::utility::parser::parser_base_t<S>
{
    inline parse_result_t<S>
        run_parser(
            parser_input_t& _a_input
        ) const noexcept
    {
        // We use the object object_printer_parser_t and the function
        // object_printer. We give it the default object_printer_parser_t
        // object, a string which is prepended to the beginning of the list of
        // data fields, the names of the data fields, and then the fields
        // themselves
        return abc::utility::parser::
            object_parser_with_field_names<S, int, int>(
                object_printer_parser_t{}, "S", {"int_1", "int_2"}, _a_input
            );
    }
};

// This is a bespoke printer_base_t instance for int. It prints the data out in
// hex.
struct int_parser_t : abc::utility::parser::parser_base_t<int>
{
    inline abc::utility::parser::parse_result_t<int>
        run_parser(
            abc::utility::parser::parser_input_t& _a_input
        ) const noexcept
    {
        using namespace std;
        using namespace abc::utility::parser;
        string_view sv{_a_input.sv()};
        size_t      pos{sv.find_first_not_of("0123456789abcdef")};
        if (pos == string::npos)
        {
            return parse_error<int>("Could not parse");
        }
        else
        {
            string sv2{sv.substr(0, pos)};
            int    n = stoul(sv2.c_str(), nullptr, 16);
            _a_input.advance(pos);
            return parse_result_t<int>(n);
        }
    }
};

// This is a bespoke printer for S.
struct s_parser : public abc::utility::parser::parser_base_t<S>
{
    abc::utility::parser::parser_t<int> m_int_parser
        = abc::utility::parser::default_parser<int>();
    s_parser() = default;

    inline s_parser(
        const abc::utility::parser::parser_t<int>& int_parser
    )
        : m_int_parser(int_parser)
    {}

    inline abc::utility::parser::parse_result_t<S>
        run_parser(
            abc::utility::parser::parser_input_t& _a_input
        ) const
    {
        // This printer uses the function object_printer_with_custom_printers
        // with a bespoke object_printer_parser_t object.
        // object_printer_with_custom_printers takes a tuple of bespoke printers
        // for each field it is provided.
        return abc::utility::parser::
            object_printer_with_custom_parsers<S, int, int>(
                abc::utility::object_printer_parser_t{
                    .begin_char = '(',
                    .end_char   = ')',
                },
                "S",
                std::make_tuple(m_int_parser, m_int_parser),
                _a_input
            );
    }
};

_TEST_CASE(
    abc::test_case_t({.name = "Custom parsers"})
)
{
    using namespace abc;
    using namespace abc::utility::parser;
    S s1{10'000, 20'000};
    // This will call the default printer on S.
    _CHECK_EXPR(
        parse_with_exception<S>("S {int_1 = 10000, int_2 = 20000}") == s1
    );

    // This printer uses the bespoke printer s_printer.
    _CHECK_EXPR(
        parse_with_exception<S>("S (10000, 20000)", mk_parser(s_parser())) == s1
    );

    // This also uses the bespoke printer s_printer, but we use int_printer_t to
    // print S's integers.
    _CHECK_EXPR(
        parse_with_exception<S>(
            "S (2710, 4e20)", mk_parser(s_parser(mk_parser(int_parser_t())))
        )
        == s1
    );
}
```


### Reader/Writer Interface

The parsers and printers we described above are used in some data generators in `abc_test`. We pair together parsers and printers in the auxillery struct `template<typename T> abc::utility::str::rw_info_t`. It is a simple class, and is defined as follows.

```cpp
template <typename T>
struct rw_info_t
{
    parser::parser_t<T>   internal_parser  = parser::default_parser<T>();
    printer::printer_t<T> internal_printer = printer::mk_printer(printer::default_printer_t<T>()
};
```

Naturally, the user is able to set the `internal_parser` and `internal_printer` to suit their needs, if they require different parsing or printing functionality.

## Data Generators & Files

In this section we describe the data these files contain, what they are used for, the abstract objects in `abc_test` that represent them, and how to use these objects.

There are two types of data file in `abc_test` used with data generators. These are the general data file (GDF) and the tertiary data file (TDF).

These data files can be used to write data which has failed a test to a file, which can then be re-used in future tests. This can be particularly useful for randomly generated data, to ensure there is no regression when code is updated.

### General Data Files

For a data generator class `dg` whose return type is a value of type `T`, a GDF used with `dg` contains a collection of `T` values. `dg` has a general data file interface (GDFI) which describes how to transform each `std::string` extracted from the GDF into an element `T`. The GDFI also describes how to turn `T` elements into `std::string` entities. In essence, the GDF stores text which represents `T` values, the GDFI describes how to parse and print `T` values, and the data generator controls how and when the GDFI reads and writes to the GDF. `abc_test` uses the `rw_info_t` object to represent the GDFI. 

`abc_test` uses two objects, `abc::utility::io::general_data_t` and `template<typename T> abc::utility::io::general_data_with_rw_info_t`, to allow the user to dictate how a data generator interacts with a GDF. The object `general_data_t` represents a GDF with a user-provided file name and a default parser and printer, while a `general_data_with_rw_info_t` object represents a GDF with a user-provided file name and a user-provided `rw_info_t` object.

There are three factory methods in the `abc` namespace which can be used to construct `general_data_t` objects. Their type signature is shown below.

```cpp
abc::utility::io::general_data_t
               gdf(const std::string_view _a_str) noexcept;
abc::utility::io::general_data_t
               gdf(const std::string_view _a_folder,
                   const std::string_view _a_str) noexcept;
abc::utility::io::general_data_t
               gdf_from_path(const std::filesystem::path& _a_path) noexcept;
```

The first `gdf` function creates a `general_data_t` object whose internal file name is built using `abc_test` framework's `file_location`, the current test case's `path` and the user-provided `file_name` variable. If the internal file does not exist, then it is created. This includes any parent folders which do not exist (up to the `abc_test` framework's `file_location`).

The second `gdf` function creates a `general_data_t` object whose internal file name is built using `abc_test` framework's `file_location`, the user-provided `folder` variable and the user-provided `file_name` variable. If the internal file does not exist, then it is created. However if the user-provided `folder` does not exist, then an exception will be thrown and the testing framework will terminate. 

 Finally, `gdf_from_path` creates a `general_data_t` object whose internal file name is equal to the user-provided `path` variable. The entire folder subdirectory must exist otherwise an exception will be thrown and the testing framework will terminate. 

There is one factory method in the `abc` namespace which can be used to construct `general_data_with_rw_info_t` objecst. It is defined as follows.

```cpp
template <typename T>
__no_constexpr abc::utility::io::general_data_with_rw_info_t<T>
general_data_file(
    const abc::utility::io::general_data_t                 _a_gdf,
    const abc::utility::str::rw_info_t<T>& _a_rw_info
) noexcept;
```

This function constructs a `general_data_with_rw_info_t<T>` object which uses the `general_data_t`'s file entity as its path, and the `rw_info` as its `rw_info_t` element.

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

A TDF is very different to a GDF; each data generator class has a tertiary data type associated with it. This data dictates how the data generator can "re-generate" that data. For example, in a static data generator, this is an integer index. In a file data generator, this is the line in the file the data was drawn from, and in a random data generator, it is state information pertaining to the random number generator.

A TDF stores this data for a specific data generator class. The end-user has no control over how this data is recorded, it can simply be used by them to set the data generator itself. A TDF can be initialised using a `general_data_t` object, using the auxillery functions described in the previous subsection. 

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

A file data generator is a data generator best-used when wanting to use hand-written values with a data generator. It is represented by the type `template <typename T> file_data_generator_t`, and requires the include directive `#include "abc_test/included_instances.hpp"` to use.

A `file_data_generator_t` object, as the name suggests, is able to read (and write) data to an associated file. There are several factory methods in the `abc` namespace which can be used to construct file data generators, outlined as follows.

```cpp
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const std::string_view& _a_data_file_name,
        R&&                     _a_init_list = R{}
    );
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const utility::io::general_data_t& _a_general_data_file,
        R&&                                _a_init_list = R{}
    );
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>>
    read_data_from_file(
        const utility::io::general_data_with_rw_info_t<T>& _a_general_data_file,
        R&&                                                _a_init_list = R{}
    );
```

Each `read_data_from_file` takes two argumnets; the first pertains to file information for the class. The second argument is an optional list of values to seed the file with when it is created.

The first `read_data_from_file` function creates its GDF using the `std::string_view` provided, calling the first `gdf` function which takes a `std::string_view` as an argument. The second takes a generic `general_data_t` object to define its file. The third takes a `general_data_with_rw_info_t` to define its file type. This includes a `rw_info` which is used to read and write data to and from the file, instead of the default values.

Below we show some example uses of the file data generator.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "File data generator examples"})
)
{
    using namespace abc;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        to_string_check, "Testing std::to_string function"
    );
    // The type we will use in this test case.
    using test_type_1 = pair<int, string>;
    // The file "integer_file" contains the following data:
    // (1, "1")
    // (2, "2")
    // (100, "102")
    for (auto& [integer, str] :
         read_data_from_file<test_type_1>("integer_file"))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // This is another way of accessing "integer_file".
    for (auto& [integer, str] :
         read_data_from_file<test_type_1>(gdf("integer_file")))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // This accesses a differnet file, in special_folder. special_folder must
    // exist before this data generator is ran. Otherwise, the test framework
    // will throw an exception and terminate.
    // The file, if it doesn't already exist, is initialised using the data
    // shown.
    for (auto& [integer, str] : read_data_from_file<test_type_1>(
             gdf("special_folder", "integer_file"),
             {
                 {10,  "10" },
                 {20,  "20" },
                 {300, "301"}
    }
         ))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // Another example. This one has an exact file where the data is located.
    for (auto& [integer, str] : read_data_from_file<test_type_1>(
             gdf_from_path(
                 "../../examples/data/test_data/unique/path/integer_file.gd"
             ),
             {
                 {1'000,  "1000" },
                 {2'000,  "2000" },
                 {30'000, "30100"}
    }
         ))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // This one uses a custom printer and parser, which reads the int data in
    // hex.
    for (auto& [integer, str] :
         read_data_from_file<test_type_1>(general_data_file(
             gdf("integer_file"),
             utility::str::rw_info_t<test_type_1>{
                 .internal_parser
                 = utility::parser::default_parser<test_type_1>(
                     utility::parser::mk_parser(int_parser_t()),
                     utility::parser::default_parser<string>()
                 ),
                 .internal_printer
                 = utility::printer::default_printer<test_type_1>(
                     utility::printer::mk_printer(int_printer_t()),
                     utility::printer::default_printer<string>()
                 )
             }
         )))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    _END_BBA_CHECK(to_string_check);
}
```

# Random Data Generator

A random data generator is used when wanting to test with randomly generated values. It also includes extensive support for reading and writing values fo files. It is represented by the type `template <typename T> random_data_generator_t`, and requires the include directive `#include "abc_test/included_instances.hpp"` to use.

# Basic Random Generator

`abc_test` comes with its own random generator which is used when making random data generators. This is the `abc::utility::rng` object. If the user is writing their own random generator object, an understanding of how this object works is beneficial.

There are components of the `rng` object which are not required by the user. Below we provide an overview of the pertinant class information.

```cpp
class rng
{
public:
    using result_type = std::mt19937_64::result_type;
    inline result_type
    operator()()
    {
        return inner_rng();
    }
private:
    std::mt19937_64 inner_rng;
}
```

The user can see that `rng` is essentially a wrapper around an `std::mt19937_64` object. Random data values can be extracted using the `operator()` function.

## Random Generator

`random_data_generator_t` objects are able to use a user-defined random generator object. These are all derived from the abstract class `template<typename T> random_generator_base_t`, which we outline below.

```cpp
template <typename T>
struct random_generator_base_t
{
public:
    using value_type_t = T;
    virtual T
        operator()(utility::rng& rnd_generator, const std::size_t index) = 0;
};
```

The `random_generator_base_t` class has a single function, `operator()`. It takes a random generator `utility::rng` and the element being generated represented as an `std::size_t`, `index`. This is the function which creates the random elements. The `index` variable can be used to "limit" the created values; for example, if creating a random `std::vector`, this can be used to control the size of the created vector.

`abc_test` also comes with a type synonym `random_generator_t` used to define instances of `random_generator_base_t`, as well as a function for elevating arbitrary classes which extend `random_generator_base_t` to `random_generator_t` entities. These are outlined below.

```cpp
template <typename T>
using random_generator_t = std::shared_ptr<random_generator_base_t<T>>;
template <typename T>
__constexpr data_gen::random_generator_t<typename T::value_type_t>
            mk_random_generator(T&& _a_random_generator_base) noexcept;
```

The user is able to define concrete classes by extending `random_generator_base_t`. Below we show an example.

```cpp
struct int_generator_t : public abc::data_gen::random_generator_base_t<int>
{
    int_generator_t(int multiplier = 10)
        : m_multiplier(multiplier)
    { }
    inline int
        operator()(abc::utility::rng& _a_rnd_generator, const std::size_t _a_index)
    {
        using namespace std;
        size_t limit{ _a_index * m_multiplier };
        return (_a_rnd_generator() % limit);
    }
private:
    int m_multiplier;
};
```

`abc_test` comes with three concrete instances of `random_generator_base_t`. The first of these is `random_generator_function_t`, which is outlined below.

```cpp
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng&, const std::size_t>
struct random_generator_function_t : public random_generator_base_t<T>
{
private:
    F _m_rnd_gen;
public:
    random_generator_function_t(F _a_rnd_func) noexcept;
    virtual T
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
};
```

This object allows the user to wrap a callable entity into a `random_generator_base_t` object, without having to define their own class derived from `random_generator_base_t`. 

`random_generator_base_t` also has an associated class in the `abc` namespace, which can be used to quickly construct an entity of type `random_generator_function_t` wrapped in a `random_generator_t` entity. It is defined as follows.

```cpp
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng&, const std::size_t>
data_gen::random_generator_t<typename T::value_type_t>
using_function(F func) noexcept;
```

The second of these is the `default_random_generator_t`. Its `operator()` function has no concrete implementation, instead relying on there being specializations defined for this class.

Below we show an example of a specialization defined for the type `S`, introduced earlier in this document.

```cpp
template <>
struct abc::data_gen::default_random_generator_t<S>
    : public abc::data_gen::random_generator_base_t<S>
{
    inline default_random_generator_t(
        abc::data_gen::random_generator_t<int> int_generator
    )
        : m_int_generator(int_generator)
    {}

    inline S
        operator()(
            abc::utility::rng& _a_rnd_generator,
            const std::size_t  _a_index
        )
    {
        using namespace std;
        return make_random_object<S>(
            _a_rnd_generator,
            _a_index,
            make_tuple(m_int_generator, m_int_generator)
        );
    }

    abc::data_gen::random_generator_t<int> m_int_generator
        = default_random_generator<int>();
};
```

The final of these concrete subclasses of `random_generator_base_t` is slightly more complicated. It uses the enumeration data generator described in the previous section as a basis for generating random values. 

## Creating Random Data Generators


There are several factory methods in the `abc` namespace which can be used to construct random data generators, outlined as follows.

```cpp
template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly(
                    const data_gen::random_generator_t<T>& _a_rnd,
                    Args... _a_file_reader_writers
                );
template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly(Args... _a_file_reader_writers);

template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
    generate_data_randomly(const _ABC_NS_DG::random_generator_t<T>& _a_rnd);
template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly();
```

The first of these `generate_data_randomly` takes a `data_gen::random_data_generator_t<T>` object, and a list of additional file arguments. These file arguments can either be `tertiay_data_file_t`, `general_data_file_t` or `template<typename T> general_data_with_rw_info_t` objects. However it is only the first in the set of argumnets which the data generator will read elements from. The other files are useful if the user wants to use failing values for other data generators.

