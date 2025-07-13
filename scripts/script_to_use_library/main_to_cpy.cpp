// <initial_example>
#include <abc_test/core.hpp>
#include <abc_test/included_instances.hpp>

int
    main(
        int   argc,
        char* argv[]
    )
{
     return abc::run_test_suite_using_command_line_args(argc, argv);
}

inline int
    fib(
        const int i
    )
{
    return (i <= 1) ? i : fib(i - 1) + fib(i - 2);
}

// <simple_example>
_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing Fibonacci function", .path = "tests::fib"}
    )
)
{
    _CHECK_EXPR(fib(6) == 8);
    _CHECK_EXPR(fib(7) == 14);
}
// </initial_example>