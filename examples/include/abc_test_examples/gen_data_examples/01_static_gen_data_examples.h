#pragma once
#include "abc_test/internal/data_generator/data_generator_collection_iterator.h"
#include "abc_test/internal/ds/test_data/macros.h"
#include "abc_test/internal/test_assertions/macros.h"
#include "abc_test/included_instances/gen_data/enumerable/specializations/enum.h"
#include "abc_test/included_instances/gen_data/static_data.h"
#include "abc_test/internal/matchers/comparison.h"
#include "abc_test/internal/matchers/matcher_wrapper.h"
#include "abc_test/included_instances/matchers/ranges.h"

#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>

_TEST_CASE(
    abc::test_data_t(
        {.name             = "file_01_example_01",
         .description      = "Simpe data generator example using static_data.",
         .path             = "examples::gen_data_examples::static_gen_data",
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
    for (auto&& [_l_input, _l_output] : iterate_over<test_data_t>({
             {0, 0},
             {1, 2},
             {2, 4},
             {4, 7}
    }))
    {
        /*
         * This loop will report an errror on its fourth iteration.
         */
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
}

_TEST_CASE(
    abc::test_data_t(
        {.name        = "file_01_example_02",
         .description = "Showing how to define collections of data generators.",
         .path        = "examples::gen_data_examples::static_gen_data",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    /*
     * In this example we use the same test case, just to show some examples
     */
    function<int(int)> _l_f = [](int _a_input)
    {
        return _a_input * 2;
    };
    using test_data_t = pair<int, int>;
    /*!
     * This is a collection of data generators. They will be processed one after
     * another.
     */
    for (auto&& [_l_input, _l_output] : data_generator_collection<test_data_t>(
             iterate_over<test_data_t>({
                 {0, 0},
                 {1, 2},
    }),
             iterate_over<test_data_t>({})
         ))
    {
        /*
         * This loop won't fail. Its just to show collections off.
         */
        _CHECK(annotate(
            _EXPR(_l_f(_l_input) == _l_output),
            fmt::format("Testing _l_f({0}) == {1}", _l_input, _l_output)
        ));
    }
}