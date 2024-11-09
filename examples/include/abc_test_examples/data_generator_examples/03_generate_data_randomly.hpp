#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include <numeric>
#include <ranges>
#include <scn/ranges.h>
#include <tuple>
#include <vector>

_TEST_CASE(
    abc::test_case_t(
        {.name             = "file_03_example_01",
         .description      = "Random data generator examples.",
         .path             = "examples::gen_data_examples::random_data",
         .threads_required = 1}
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
    using test_data_t = int;
    int _l_output     = 250;
    /*
     * This is a data generator. It iterates over a set of data, given as a list
     * of test_data_t elements.
     */
    // for (auto&& _l_input : generate_data_randomly<test_data_t>())
    // {
    //    _CHECK(annotate(
    //       _EXPR(_l_f(_l_input) == _l_output),
    //       fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
    // ));
    // }

    for (auto&& _l_input :
         generate_data_randomly<test_data_t>(general_data_file("file1x")))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }

    for (auto&& _l_input :
         generate_data_randomly<test_data_t>(tertiary_data_file("file2")))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
    for (auto&& _l_input : generate_data_randomly<test_data_t>(
             general_data_file("file3"), tertiary_data_file("file4")
         ))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
    for (auto&& _l_input : generate_data_randomly<test_data_t>(
             tertiary_data_file("file5"), general_data_file("file6")
         ))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
}
