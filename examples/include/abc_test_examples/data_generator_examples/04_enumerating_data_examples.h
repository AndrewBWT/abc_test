#pragma once
#include "abc_test/core.h"
#include "abc_test/included_instances.h"

_TEST_CASE(
    abc::test_data_t(
        {.name             = "file_04_example_01",
         .description      = "Enumerating data examples",
         .path             = "examples::gen_data_examples::enumerating_data",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using test_data_t                   = uint8_t;
    /*
     * In this example ,we are testing a function. To keep evertyhing condensed,
     * we will write it here as a std::function
     */
    function<uint8_t(test_data_t)> _l_f = [](test_data_t _a_input)
    {
        return _a_input * 2;
    };
    // To test this function, we describe our test data as a pair.
    uint8_t _l_output = 250;
    /*
     * This is a data generator. It iterates over a set of data, given as a list
     * of test_data_t elements.
     */
    for (auto&& _l_input : enumerate_data<test_data_t>())
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
    for (auto&& _l_input : enumerate_data<test_data_t>(all_values<test_data_t>()))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
    for (auto&& _l_input : enumerate_data<test_data_t>(all_values_moving_forward<test_data_t>()))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
    for (auto&& _l_input : enumerate_data<test_data_t>(all_values_moving_backward<test_data_t>()))
    {
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
}
