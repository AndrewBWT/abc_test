#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

// <initial_example>

inline int
fib(
    const int i
)
{
    return (i <= 1) ? i : fib(i - 1) + fib(i - 2);
}

_TEST_CASE(
    abc::test_case_t(
        { .name = "Testing Fibonacci function using data from a file", .path = "tests::fib" }
    )
)
{
    _BEGIN_MULTI_ELEMENT_BBA(_l_unit_tests, "Unit testing Fibonacci function");

    _END_BBA_CHECK(_l_unit_tests);
}

// </initial_example>