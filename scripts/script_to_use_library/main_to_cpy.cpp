// <initial_example>
// Only these two includes are needed.
#include <abc_test/core.hpp>
#include <abc_test/included_instances.hpp>

int
    main(
        int   argc,
        char* argv[]
    )
{
    // This code will process the arguments, and set up the testing enviornment.
    return abc::run_test_suite_using_command_line_args(argc, argv);
}

// This is the function which will be tested.
inline int
    fib(
        const int i
    )
{
    return (i <= 1) ? i : fib(i - 1) + fib(i - 2);
}

// The macro _TEST_CASE declares a test, and registers it with the underlying
// test framework. A test's name can be used to provide a description of the
// test, while a test's path provides a way of organizing tests.
_TEST_CASE(
    // Note the extra bracket after "abc::test_case_t". This ensures the commas
    // used to separate test_case_t's member variables are processed correctly.
    abc::test_case_t(
        {.name = "Testing Fibonacci function", .path = "tests::fib"}
    )
)
{
    // This test will pass.
    _CHECK_EXPR(fib(6) == 8);
    // This test will fail.
    _CHECK_EXPR(fib(7) == 14);
}

// </initial_example>