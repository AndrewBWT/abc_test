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
    auto _l_unit_tests = _MULTI_MATCHER("Unit tests for Fibonacci function");
    // This for loop gathers the values from the file.
    // The type identifies what is being read from the file. The string
    // represents the name of the file. The file is in the folder tests/fib,
    // with the root file being set by either the system or the user.
    for (auto& [_l_input, _l_expected_output] :
         read_data_from_file<std::pair<int, int>>("unit_tests"))
    {
        // At this point, each line is read from the file, and its contents are
        // available to the user.
        // The result of the assertion is streamed to the multi matcher.
        _l_unit_tests << _CHECK_EXPR(fib(_l_input) == _l_expected_output);
    }
    // The multi matcher is then checked. It is this line which sneds the
    // information to the testing framework.
    _CHECK(_l_unit_tests);
}

// tests/fib/unit_tests.gd
// (0, 0)
// (6, 8)
// (7, 14)

// </unit_test_example>