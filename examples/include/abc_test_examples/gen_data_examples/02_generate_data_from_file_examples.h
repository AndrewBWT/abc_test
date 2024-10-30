#pragma once
#include "abc_test/core.h"
#include "abc_test/included_instances/data_generator/file.h"
#include "abc_test/included_instances/matchers/ranges.h"
#include "abc_test/internal/data_generator/data_generator_collection_iterator.h"

#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>

_TEST_CASE(
    abc::test_data_t(
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
             general_data_file("file1"),
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
             general_data_file("file1"),
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
