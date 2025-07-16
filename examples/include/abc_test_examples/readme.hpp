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

// </property_test_example>

// <fuzzy_test_example>
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
         generate_data_randomly<std::vector<float>>()
             & enumerate_data(from_m_to_n(
                 std::vector<float>{}, std::vector<float>(1, 100.0f)
             )))
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
        fuzzy_tests << _CHECK(exception_matcher);
    }
    _CHECK(fuzzy_tests);
}

// </property_test_example>
