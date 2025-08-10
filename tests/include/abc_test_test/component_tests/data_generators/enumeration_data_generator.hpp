#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Basic enumeration data generator example using all values",
         .path
         = "abc_test_test::component_tests::data_generators::enumeration_data_"
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
    vector<char> _l_results;
    for (auto&& _l_element : enumerate_data(all_values<char>()))
    {
        _l_results.push_back(_l_element);
    }
    vector<char> _l_to_check_against;
    for (char _l_element{std::numeric_limits<char>::min()};
         _l_element != std::numeric_limits<char>::max();
         ++_l_element)
    {
        _l_to_check_against.push_back(_l_element);
    }
    _l_to_check_against.push_back(std::numeric_limits<char>::max());
    _CHECK_EXPR(_l_to_check_against == _l_results);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Basic enumeration data generator example using from_m_to_n",
         .path
         = "abc_test_test::component_tests::data_generators::enumeration_data_"
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
    vector<char> _l_results;
    for (auto&& _l_element : enumerate_data(from_m_to_n<char>('a', 'z')))
    {
        _l_results.push_back(_l_element);
    }
    vector<char> _l_to_check_against;
    for (char _l_element{'a'}; _l_element != 'z'; ++_l_element)
    {
        _l_to_check_against.push_back(_l_element);
    }
    _l_to_check_against.push_back('z');
    _CHECK_EXPR(_l_to_check_against == _l_results);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Checking file creation for enum data generator",
         .path
         = "abc_test_test::component_tests::data_generators::enumeration_data_"
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
    string _l_name_1{"name"};
    string _l_path{"path"};
    auto   _l_make_file_path
        = [&](const std::string _a_file_name, const std::string _a_extension)
    {
        return filesystem::path{string{std::filesystem::current_path().string()}
                                    .append("\\")
                                    .append(_l_path)
                                    .append("\\")
                                    .append(_l_name_1)
                                    .append("\\")
                                    .append(_a_file_name)
                                    .append(_a_extension)};
    };
    string           _l_file_name{"test_file_1"};
    filesystem::path _l_path_1{_l_make_file_path(_l_file_name, ".gd")};
    filesystem::path _l_path_2{_l_make_file_path(_l_file_name, ".td")};
    if (std::filesystem::exists(_l_path_1))
    {
        std::filesystem::remove(_l_path_1);
    }
    if (std::filesystem::exists(_l_path_2))
    {
        std::filesystem::remove(_l_path_2);
    }
    function<void()> _l_test_func_1 = [&]()
    {
        for (auto&& _l_element :
             enumerate_data(from_m_to_n<char>('a', 'z'), gdf(_l_file_name)))
        {
        }
        for (auto&& _l_element :
             enumerate_data(from_m_to_n<char>('a', 'z'), tdf(_l_file_name)))
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
    abc_test::utility::abc_test_tests_internal_test_runner(
        1, true, _l_funcs_to_run
    );
    // File should exist.
    _CHECK(annotate(
        u8"Checks file has been created after test ran",
        _EXPR(std::filesystem::exists(_l_path_1) == true)
    ));
    _CHECK(annotate(
        u8"Checks file has been created after test ran",
        _EXPR(std::filesystem::exists(_l_path_2) == true)
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
        _EXPR(_l_file_conents[0] == "#char")
    ));
    _l_input_file = ifstream(_l_path_2);
    _l_file_conents.clear();
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
        _EXPR(_l_file_conents[0] == "#unsigned __int64")
    ));
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Checking repetition config for enum data generator",
         .path
         = "abc_test_test::component_tests::data_generators::enumeration_data_"
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
    vector<char> _l_values;
    function<void()> _l_test_func_1 = [&]()
    {
        for (auto&& _l_element :
             enumerate_data(from_m_to_n<char>('a', 'z')))
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
    ds::idgc_memoized_element_sequence_t _l_element_sequence_1,
        _l_element_sequence_2, _l_element_sequence_3;
    _l_element_sequence_1.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"0"}}
    });
    _l_element_sequence_2.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"2"}}
    });
    _l_element_sequence_3.push_back(indexed_data_generator_collection_memoized_element_t{
        .for_loop_index = 0,
        .for_loop_iteration_data
        = dgc_memoized_element_t{.generation_collection_index = 0, .flied = dg_memoized_element_t{.mode = 0, .additional_data = u8"4"}}
    });
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_1);
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_2);
    _l_stack.add_for_loop_creation_data_sequence(_l_element_sequence_3);
    vector<char> _l_expected_elements = {'a', 'c', 'e'};
    string           _l_name_1{ "name" };
    string           _l_path{ "path" };
    _l_funcs_to_run.push_back(
        make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
    );
    abc_test::utility::abc_test_tests_internal_test_runner(
        1, false, _l_funcs_to_run
    );
    _CHECK(annotate(
        u8"Checks values are correct", _EXPR(_l_values == _l_expected_elements)
    ));
}