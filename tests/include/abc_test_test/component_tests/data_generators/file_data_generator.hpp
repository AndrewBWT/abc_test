#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Checking file creation for file data generator",
         .path = "abc_test_test::component_tests::data_generators::file_data_"
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
    string           _l_name_1{"name"};
    string           _l_path{"path"};
    string           _l_file_name{"test_file_1"};
    filesystem::path _l_path_1{string{std::filesystem::current_path().string()}
                                   .append("\\")
                                   .append(_l_path)
                                   .append("\\")
                                   .append(_l_name_1)
                                   .append("\\")
                                   .append(_l_file_name)
                                   .append(".gd")};
    if (std::filesystem::exists(_l_path_1))
    {
        std::filesystem::remove(_l_path_1);
    }
    function<void()> _l_test_func_1 = [&]()
    {
        for (auto&& _l_element : read_data_from_file<int>(_l_file_name))
        {
        }
    };
    std::vector<std::tuple<
        std::function<void()>,
        std::string,
        std::string,
        abc::ds::tdg_collection_stack_trie_t>>
                                _l_funcs_to_run;
    tdg_collection_stack_trie_t _l_stack;
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    abc_test::utility::abc_test_tests_internal_test_runner(1, true, _l_funcs_to_run);
    // File should exist.
    _CHECK(annotate(
        u8"Checks file has been created after test ran",
        _EXPR(std::filesystem::exists(_l_path_1) == true)
    ));
    // Check contents of file. It should be empty essentially, except for a
    // comment line.
    vector<string> _l_file_conents;
    string         _l_line;
    ifstream       _l_input_file(_l_path_1);
    while (std::getline(_l_input_file, _l_line))
    {
        _l_file_conents.push_back(_l_line);
    }
    _REQUIRE(annotate(
        u8"Checks there is exactly one line in the file",
        _EXPR(_l_file_conents.size() == 1)
    ));
    _CHECK(annotate(
        u8"Checks there is exactly one line in the file",
        _EXPR(_l_file_conents[0] == "#int")
    ));
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Checking file reading for file data generator",
         .path = "abc_test_test::component_tests::data_generators::file_data_"
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
    string           _l_name_1{"name"};
    string           _l_path{"path"};
    string           _l_file_name{"test_file_1"};
    filesystem::path _l_path_1{ string{std::filesystem::current_path().string()}
                                   .append("\\")
                                   .append(_l_path)
                                   .append("\\")
                                   .append(_l_name_1)
                                   .append("\\")
                                   .append(_l_file_name)
                                   .append(".gd") };
    if (std::filesystem::exists(_l_path_1))
    {
        std::filesystem::remove(_l_path_1);
    }
    ofstream    _l_out(_l_path_1);
    vector<int> _l_elements = {1, 2, 3, 54, 100};
    for (auto& _l_element : _l_elements)
    {
        _l_out << abc::utility::str::cast_u8string_to_string(
            abc::utility::printer::default_printer_t<int>{}.run_printer(
                _l_element
            )
        ) << std::endl;
    }
    _l_out.close();
    vector<int>      _l_values;
    function<void()> _l_test_func_1 = [&]()
    {
        for (auto&& _l_element : read_data_from_file<int>(_l_file_name))
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
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    abc_test::utility::abc_test_tests_internal_test_runner(1, true, _l_funcs_to_run);
    // File has the correct values.
    _CHECK(
        annotate(u8"Checks values are correct", _EXPR(_l_values == _l_elements))
    );
    _l_values.clear();
    // Test again, but it should only generate the 1st, 3rd and 5th values.
    _l_funcs_to_run.clear();
    ds::idgc_memoized_element_sequence_t _l_element_sequence_1, _l_element_sequence_2, _l_element_sequence_3;
    _l_element_sequence_1.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"(0, 0)"}}
    });
    _l_element_sequence_2.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"(0, 2)"}}
    });
    _l_element_sequence_3.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"(0, 4)"}}
    });
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_1);
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_2);
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_3);
    vector<int> _l_expected_elements = {1, 3, 100};
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    abc_test::utility::abc_test_tests_internal_test_runner(1, false, _l_funcs_to_run);
    _CHECK(annotate(
        u8"Checks values are correct", _EXPR(_l_values == _l_expected_elements)
    ));
}

_TEST_CASE(
    abc::test_case_t(
        { .name = "Checking file failed file reading is dealt with correctly",
         .path = "abc_test_test::component_tests::data_generators::file_data_"
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
    string           _l_name_1{ "name" };
    string           _l_path{ "path" };
    string           _l_file_name{ "test_file_1" };
    filesystem::path _l_path_1{ string{std::filesystem::current_path().string()}
                                   .append("\\")
                                   .append(_l_path)
                                   .append("\\")
                                   .append(_l_name_1)
                                   .append("\\")
                                   .append(_l_file_name)
                                   .append(".gd") };
    if (std::filesystem::exists(_l_path_1))
    {
        std::filesystem::remove(_l_path_1);
    }
    ofstream    _l_out(_l_path_1);
    vector<int> _l_elements = { 1, 2, 3, 54, 100 };
    for (auto& _l_element : _l_elements)
    {
        _l_out << abc::utility::str::cast_u8string_to_string(
            abc::utility::printer::default_printer_t<int>{}.run_printer(
                _l_element
            )
        ) << std::endl;
    }
    _l_out.close();
    vector<int>      _l_values;
    function<void()> _l_test_func_1 = [&]()
        {
            for (auto&& _l_element : read_data_from_file<bool>(_l_file_name))
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
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    auto _l_result = abc_test::utility::abc_test_tests_internal_test_runner(1, true, _l_funcs_to_run);
    // File has the correct values.
    _CHECK(
        _EXPR(_l_result.memoized_error_repoter.errors().at(0).errors().at(0) == u8"huh")
    );
}
