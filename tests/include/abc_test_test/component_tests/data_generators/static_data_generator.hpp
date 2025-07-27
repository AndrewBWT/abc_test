#pragma once

#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"

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
    std::shared_ptr<_ABC_NS_DS::test_list_t> _l_tests_to_run
        = make_shared<_ABC_NS_DS::test_list_t>();
    test_options_base_t _l_running_options;
    _l_running_options.group_test_options.threads = 1;
    _l_running_options.group_test_options.test_lists.push_back(_l_tests_to_run);
    _l_running_options.group_test_options.error_reporters.push_back(
        make_shared<reporters::memoized_error_reporter_t>()
    );
    _l_running_options.group_test_options.test_reporters.push_back(
        make_shared<reporters::memoized_test_reporter_t>()
    );
    tdg_collection_stack_trie_t          _l_stack;
    ds::idgc_memoized_element_sequence_t _l_element_sequence;
    _l_element_sequence.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"4"}}
    });
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence);
    _l_running_options.group_test_options
        .map_of_unique_ids_and_for_loop_stack_tries.insert(
            u8"hi::hello", _l_stack
        );
    _l_running_options.individual_io_based_test_options.write_data_to_files = false;
    vector<int>      _l_read_results;
    function<void()> _l_test_func = [&]()
    {
        vector<int> _l_values = {1, 2, 3, 4, 5};
        for (auto&& _l_element : iterate_over<int>(_l_values))
        {
            _l_read_results.push_back(_l_element);
        }
    };
    test_function_t ki = std::make_shared<
        test_function_callable_object_t<decltype(_l_test_func)>>(_l_test_func);
    abc::ds::registered_test_data_t _l_rtd(
        ki,
        abc::ds::checked_user_defined_test_data_t::test_data(
            abc::ds::user_defined_test_data_t{.name = "hello", .path = "hi"}
        )
    );
    _l_tests_to_run->push_back(_l_rtd);
    simple_console_reporter_t _l_sco;
    abc::run_tests<test_options_base_t>(_l_running_options, _l_sco);
    //for (auto& _l_k : _l_sco.error_lines())
    //{
   //     std::cout << _l_k << std::endl;
   // }
    _CHECK(_EXPR(_l_read_results.size() > 0) && _EXPR(_l_read_results[0] == 5));
}
