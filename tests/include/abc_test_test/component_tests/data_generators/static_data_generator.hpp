#pragma once

#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Basic component test for static data generator",
         .path = "abc_test_test::component_tests::data_generators::static_data_"
                 "generator",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    auto _l_unit_tests = _MULTI_MATCHER("Unit tests for static data generator");
    for (auto&& _l_values : read_data_from_file<vector<int>>("unit_test"))
    {
        vector<int> _l_result;
        for (auto&& _l_element : iterate_over<int>(_l_values))
        {
            _l_result.push_back(_l_element);
        }
        _l_unit_tests << _CHECK_EXPR(_l_result == _l_values);
    }
    _CHECK(_l_unit_tests);
    tdg_collection_stack_trie_t          _l_stack;
    ds::idgc_memoized_element_sequence_t _l_element_sequence;
    _l_element_sequence.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"4"}}
    });
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence);
    vector<int>      _l_read_results;
    function<void()> _l_test_func = [&]()
    {
        vector<int> _l_values = {1, 2, 3, 4, 5};
        for (auto&& _l_element : iterate_over<int>(_l_values))
        {
            _l_read_results.push_back(_l_element);
        }
    };
    std::vector<
        std::tuple<std::function<void()>, std::string, std::string,
        abc::ds::tdg_collection_stack_trie_t>> _l_funcs_to_run;
    _l_funcs_to_run.push_back(make_tuple(_l_test_func, "hello", "hi", _l_stack));
    abc_test::utility::abc_test_tests_internal_test_runner(1, _l_funcs_to_run);
    matcher_t _l_matcher{ false_matcher(u8"_l_vector is empty") };
    if (_l_read_results.size() > 0)
    {
        _l_matcher = _EXPR(_l_read_results[0] == 5);
    }
    _l_unit_tests << _CHECK(_l_matcher);
    _CHECK(_l_unit_tests);
}
