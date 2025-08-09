#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Check parsing of a correcr repetition configuration",
         .path = "abc_test_test::component_tests::repetition_config",
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
    vector<u8string> _l_args
        = {u8"--write_data_to_files", u8"false", u8"--repetition_config", };
    map_unique_id_to_tdg_collection_stack_trie_t _l_mds;
    tdg_collection_stack_trie_t                  _l_trie_1;
    auto                                         _l_add_data
        = [](std::vector<indexed_data_generator_collection_memoized_element_t>&
                                                            _a_fds,
             const tuple<size_t, size_t, size_t, u8string>& _a_tuple)
    {
        auto& [_l_for_loop_index, _l_generation_data, _l_mode, _l_str]{_a_tuple
        };
        _a_fds.push_back(indexed_data_generator_collection_memoized_element_t{
            .for_loop_index = _l_for_loop_index,
            .for_loop_iteration_data
            = dgc_memoized_element_t{.generation_collection_index = _l_generation_data, .flied = dg_memoized_element_t{.mode = _l_mode, .additional_data = _l_str}}
        });
    };
    std::vector<indexed_data_generator_collection_memoized_element_t> _l_flds;
    _l_add_data(_l_flds, {0, 0, 0, u8"hi"});
    _l_trie_1.add_for_loop_creation_data_sequence(_l_flds);
    _l_mds.insert(u8"func", _l_trie_1);
    auto _l_str
        = print_map_of_unique_ids_to_tdg_collection_stack_tries<false>(_l_mds);
    _CHECK_EXPR(_l_str == u8"func:[[(0,0,\"hi\",[])]]");
    _l_args.push_back(
        print_map_of_unique_ids_to_tdg_collection_stack_tries(_l_mds)
    );
    auto _l_result = abc_test::utility::abc_test_tests_command_line_test_runner(
        1, _l_args, {}
    );
    vector<u8string> _l_expected;
    _CHECK_EXPR(_l_result.simple_text_reporter.errors() == _l_expected);
}
