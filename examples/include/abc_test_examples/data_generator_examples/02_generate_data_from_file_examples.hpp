#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>

_TEST_CASE(
    abc::test_case_t(
        {.name             = "file_02_example_01",
         .description      = "File data generator examples.",
         .path             = "examples::gen_data_examples::file_data",
         .threads_required = 2}
    )
)
{
    using namespace abc;
    using namespace std;
    /*
     * In this example ,we are testing a function. To keep evertyhing condensed,
     * we will write it here as a std::function
     */
    function<int(int)> _l_f = [](int _a_input)
    {
        return _a_input * 2;
    };
    // To test this function, we describe our test data as a pair.
    using test_data_t = pair<int, int>;
    /*
     * This is a data generator. It iterates over a set of data, given as a list
     * of test_data_t elements.
     */
    for (auto&& [_l_input, _l_output] : read_data_from_file<test_data_t>(
             gdf("file1"),
             {
                 {0, 0},
                 {1, 2},
                 {2, 4},
                 {4, 7},
    }
         ))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
    for (auto&& [_l_input, _l_output] : read_data_from_file<test_data_t>(
             gdf("file1"),
             {
                 {0, 0},
                 {1, 2},
                 {2, 4},
                 {4, 7}
    }
         ))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
}
