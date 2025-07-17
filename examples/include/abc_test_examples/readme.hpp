#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

inline int
    fib(
        const int i
    )
{
    return (i <= 1) ? i : fib(i - 1) + fib(i - 2);
}

// <unit_test_example>
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
// </unit_test_example>

// <property_test_example>
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

// </property_test_example>

// <fuzzy_test_example>
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
// </fuzzy_test_example>
