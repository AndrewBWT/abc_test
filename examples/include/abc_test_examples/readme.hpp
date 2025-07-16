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
    // The multi matcher is then checked. It is this line which sends the
    // information to the testing framework.
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
    using namespace std;
    auto property_tests
        = _MULTI_MATCHER("Property tests for users_midpoint function");
    // We are testing that our midpoint function returns the same result as
    // std::midpoint.

    // This for loop uses two generators. They are chained together using the &
    // operator. The first generator creates random data. The second generator
    // enumerates over all values from (-2,-2) to (2,2).
    for (auto& [arg1, arg2] :
         generate_data_randomly<std::pair<int, int>>()
             & enumerate_data(from_m_to_n(make_pair(-2, -2), make_pair(2, 2))))
    {
        // This streams the result of checking that the result of users_midpoint
        // is the same as std::midpoint.
        property_tests << _CHECK_EXPR(
            users_midpoint(arg1, arg2) == std::midpoint(arg1, arg2)
        );
    }
    // The multi matcher is then checked. It is this line which sends the
    // information to the testing framework.
    _CHECK(property_tests);
}
// </property_test_example>