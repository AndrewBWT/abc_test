#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t({.name = "A simple test", .path = "simple_test_path"})
)
{
    // Testing code.
}

_TEST_CASE(
    abc::test_case_t({.name = "A simple test", .path = "simple_test"})
)
{}

_NAMED_TEST_CASE(
    nameless_function,
    abc::test_case_t({.name = "A named test"})
)
{}

_TEST_CASE(
    abc::test_case_t(
        {.name        = "A simple test",
         .description = "A brilliant description, describing wonderful things",
         .path        = "simple_test::simplest_test::so_simple",
         .threads_required = 4}
    )
)
{
    // Testing code.
}

inline void
    adding_test_at_runtime_1()
{
    using namespace abc;
    using namespace abc::ds;
    std::function<void()> _l_test_function = []()
    {
        // Test code
    };
    registered_test_data_t _l_test(
        mk_function(_l_test_function),
        checked_user_defined_test_data_t::test_data(
            test_case_t{.name = "Test added at runtime"}
        )
    );
    global::add_test(_l_test);
}

inline void
    adding_test_at_runtime_2()
{
    using namespace std;
    using namespace abc;
    using namespace abc::ds;
    vector<registered_test_data_t> tests;
    for (size_t idx{0}; idx < 100; ++idx)
    {
        function<void()> f = []()
        {
            // Test code
        };
        tests.push_back(registered_test_data_t(
            mk_function(f),
            checked_user_defined_test_data_t::test_data(test_case_t{
                .name = fmt::format("Test {} added at runtime", idx),
            })
        ));
    }
}