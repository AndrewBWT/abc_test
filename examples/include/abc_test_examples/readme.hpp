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
    // We are testing thatusers_midpoint returns the same result as
    // std::midpoint.

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
        // This streams the result of checking that the result of users_midpoint
        // is the same as std::midpoint.
        property_tests << _CHECK_EXPR(
            users_midpoint(arg1, arg2) == std::midpoint(arg1, arg2)
        );
    }
    _CHECK(property_tests);
}

// </property_test_example>
// <assertion_examples>
_TEST_CASE(
    abc::test_case_t({.name = "Assertion examples", .path = "tests::matchers"})
)
{
    using namespace abc;
    // Previously we used the macro _CHECK_EXPR to write assertions.
    _CHECK_EXPR(1 == 2);
    // This is shorthand, which combines the _CHECK and _EXPR macros. The above
    // could be written as
    _CHECK(_EXPR(1 == 2));
    // abc_test is a matcher-based testing library. Matchers on their own encode
    // an assertion, however they do not report their result to the testing
    // framework. This expression on its own wouldn't report anything.
    matcher_t matcher1 = _EXPR(2 == 2);
    // However, the user can then write the following expression so that the
    // matcher is processed by the testing framework.
    _CHECK(matcher1);
    // There is another macro, _REQUIRE, which will terminate the currently
    // running test case if it fails.
    _REQUIRE(matcher1);

    // The _EXPR macro allows the comparison operators to be used to write
    // assertions. It does this using some macro trickery, which we will not
    // describe here - though it is very similar to how the Catch2 testing
    // framework works.

    // The user can write their own matchers, or use the
    // matchers included with abc_test. For example, the matcher "contains"
    // checks if a range includes a specified element.
    std::vector<int> to_check = {1, 2, 3};
    _CHECK(contains(to_check, 4));

    // Matchers can also be annotated.
    _CHECK(annotate(contains(to_check, 4), u8"Testing that {1,2,3} contains 4")
    );

    // Matchers can also be used with the logic operators &&, || and ! to encode
    // more verbose tests into a single assertion. The binary logic operators &&
    // and || will always evaluate both subexpressions, so the user may find
    // their logic has to be split up in some circumstances.
    _CHECK(_EXPR(1 == 2) && _EXPR(2 == 3) || _EXPR(4 == 5) && ! _EXPR(1 == 2));

    // As matchers are objects which can be manipulated and reassigned before
    // being processed by the underlying test framework, the user is able to
    // write tests for segments of code where the control flow may not be
    // sequential - for example, when working with exceptions.

    // The functions true_matcher and false_matcher encode a matcher which
    // passes or fails respectively. A string can be provided as an argument,
    // serving as a message for the matcher's output.
    matcher_t        matcher3 = true_matcher(u8"No exception was thrown");
    std::vector<int> to_check_2;
    try
    {
        int x = to_check_2.at(0);
    }
    catch (const std::exception& exception)
    {
        matcher3 = false_matcher(u8"Exception encountered");
    }
    _CHECK(matcher3);
}
// </assertion_examples>