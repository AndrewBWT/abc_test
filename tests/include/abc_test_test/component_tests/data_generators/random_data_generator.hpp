#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

/*_TEST_CASE(
    abc::test_case_t(
        {.name = "Basic random generator testing values for int",
         .path = "abc_test_test::component_tests::data_generators::random_data"
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
    vector<pair<int, int>> _l_bounds = {
        {1,      1     },
        {1,      20    },
        {-5,     17    },
        {100,    600   },
        {-1'000, 12'345}
    };
    auto unit_tests = _MULTI_MATCHER("Checking randomly generated int values");
    for (auto& [_l_min, _l_max] : _l_bounds)
    {
        vector<int> _l_results;
        for (auto&& _l_element : generate_data_randomly<int>(
                 default_random_generator<int>(_l_min, _l_max)
             ))
        {
            _l_results.push_back(_l_element);
        }
        for (auto& _l_result : _l_results)
        {
            unit_tests << _CHECK(
                _EXPR(_l_result >= _l_min) && _EXPR(_l_result <= _l_max)
            );
        }
    }
    _CHECK(unit_tests);
}*/

_TEST_CASE(
    abc::test_case_t(
        { .name = "Checking random generator repetition config",
         .path = "abc_test_test::component_tests::data_generators::random_data"
                 "generator",
         .threads_required = 1 }
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
    vector<int> _l_values;
    function<void()> _l_test_func_1 = [&]()
        {
            for (auto&& _l_element :
                generate_data_randomly<int>(default_random_generator<int>(0, 100)))
            {
                _l_values.push_back(_l_element);
            }
        };
    std::vector<std::tuple<
        std::function<void()>,
        std::string,
        std::string,
        abc::ds::tdg_collection_stack_trie_t>>
        _l_funcs_to_run;
    tdg_collection_stack_trie_t _l_stack;
    _l_funcs_to_run.clear();
    string           _l_name_1{ "name" };
    string           _l_path{ "path" };
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    abc::utility::complete_global_seed_t _l_rng = static_cast<unsigned int>(100);
    auto _l_rv = abc_test::utility::abc_test_tests_internal_test_runner(
        1, true, _l_funcs_to_run, _l_rng
    );
    vector<int> _l_expected;
    _l_expected.push_back(_l_values[0]);
    _l_expected.push_back(_l_values[2]);
    _l_expected.push_back(_l_values[4]);
    _l_values.clear();
    _l_funcs_to_run.clear();
    u8string _l_uuid;

    _l_uuid.append(cast_string_to_u8string(_l_path))
        .append(u8"::")
        .append(cast_string_to_u8string(_l_name_1));
     ds::idgc_memoized_element_sequence_t _l_element_sequence_1,
        _l_element_sequence_2, _l_element_sequence_3;
    _l_element_sequence_1.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"(0,1,0)"}}
        });
    _l_element_sequence_2.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"(2,3,2)"}}
        });
    _l_element_sequence_3.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"(4,5,4)"}}
        });
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_1);
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_2);
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_3);
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    auto _l_rv2 = abc_test::utility::abc_test_tests_internal_test_runner(
        1, false, _l_funcs_to_run, _l_rng
    );
    _CHECK(annotate(
        u8"Checks values are correct", _EXPR(_l_expected == _l_values)
    ));
}