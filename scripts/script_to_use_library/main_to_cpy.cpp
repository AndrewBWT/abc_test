// <initial_example>
// Only these two includes are needed when working with abc_test.
#include <abc_test/core.hpp>
#include <abc_test/included_instances.hpp>

int main(int argc,char* argv[])
{
    // This line will process the command line arguments, and set up the testing environment.
    return abc::run_test_suite_using_command_line_args(argc, argv);
}

// This is the function we will be testing.
inline int fib( const int i)
{
    return (i <= 1) ? i : fib(i - 1) + fib(i - 2);
}

// The macro _TEST_CASE declares a test, and registers it with the underlying
// test framework. A test's name can be used to provide an identifier for the 
// test, while a test's path provides a way of organizing the test.
_TEST_CASE(
    // Note the extra bracket after "abc::test_case_t". This ensures the commas
    // used to separate test_case_t's member variables are not consumed by the macro.
    abc::test_case_t(
        {.name = "Testing Fibonacci function", .path = "readme::fib"}
    )
)
{
    // This assertion will check if fib(6) == 8. It does, so the assertion will pass.
    _CHECK_EXPR(fib(6) == 8);
    // As fib(7) != 14, this assertion will fail.
    _CHECK_EXPR(fib(7) == 14);
}
// </initial_example>