#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test_test/testing_utilities/bootstrapped_testing_utilities.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name
         = "Checking file creation for data generator's with file support",
         .path = "abc_test_test::component_tests::data_generators::data_"
                 "generator_with_file_support",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace abc_test::utility;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    auto _l_run_func
        = [&]<typename T, size_t N>(
              const std::function<data_generator_collection_t<
                  T>(const std::array<file_name_t<T>, N>&)>& _a_run_element,
              const std::array<std::pair<file_name_info_t<T>, std::string>, N>&
                  _a_files
          )
    {
        // Create the file paths
        if constexpr (N == 0)
        {
            return;
        }
        vector<pair<filesystem::path, string>> _l_paths;
        auto                                   ki
            = std::filesystem::current_path().string().append("\\paths\\hello");
        for (const auto& [_l_file, _l_comment] : _a_files)
        {
            _l_paths.push_back(
                make_pair(make_path(_l_file, u8".gd", u8".td", ki), _l_comment)
            );
        }
        // Make sure the files don't exist
        for (const auto& [_l_path, _l_comment] : _l_paths)
        {
            if (std::filesystem::exists(_l_path))
            {
                std::filesystem::remove(_l_path);
            }
        }
        auto wrapped = [](auto&& x)
        {
            auto ki = x.first;
            return make_file_name(std::forward<decltype(ki)>(ki));
        };
        // Actual test function.
        function<void()> _l_test_func_1 = [&]()
        {
            // This has to be done in here to get the correct file names.
            std::array<file_name_t<T>, N> arr2
                = abc_test::utility::transform_array(_a_files, wrapped);
            auto _l_generator{_a_run_element(arr2)};
            for (auto&& _l_element : _l_generator)
            {
            }
        };
        // Run the function
        std::vector<std::tuple<
            std::function<void()>,
            std::string,
            std::string,
            abc::ds::tdg_collection_stack_trie_t>>
                                    _l_funcs_to_run;
        tdg_collection_stack_trie_t _l_stack;
        string                      _l_name_1 = "hello";
        string                      _l_path   = "paths";
        _l_funcs_to_run.push_back(
            make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
        );
        abc_test::utility::abc_test_tests_internal_test_runner(
            1, true, _l_funcs_to_run
        );
        // Check the function creates the files.
        auto unit_tests = _MULTI_MATCHER("Check files exist");
        for (auto& [_l_path, _l_comment] : _l_paths)
        {
            unit_tests << _CHECK(_EXPR(filesystem::exists(_l_path) == true));
            auto _l_contents{get_file_contents(_l_path)};
            unit_tests << _CHECK(
                _EXPR(_l_contents.size() == 1)
                && _EXPR(_l_contents[0] == fmt::format("#{0}", _l_comment))
            );
        }
        _CHECK(unit_tests);
    };
    auto _l_func_maker = [&]<typename T, size_t N>()
        -> std::function<data_generator_collection_t<
            T>(const std::array<file_name_t<T>, N>&)>
    {
        return [](const std::array<file_name_t<T>, N>& _a_array)
        {
            auto _l_applied_func = std::apply(
                [](auto&&... args)
                {
                    return generate_data_randomly<T>(
                        std::forward<decltype(args)>(args)...
                    );
                },
                _a_array
            );
            return _l_applied_func;
        };
    };
    std::array<pair<file_name_info_t<int>, string>, 0> _l_arr = {};
    _l_run_func.operator()<int, 0>(_l_func_maker.operator()<int, 0>(), _l_arr);

    std::array<pair<file_name_info_t<int>, string>, 1> _l_arr_1 = {make_pair(
        file_name_info_t<int>(
            std::in_place_index<0>, file_name_basic_info_t(string("hello"))
        ),
        string("int")
    )};
    _l_run_func.operator(
    )<int, 1>(_l_func_maker.operator()<int, 1>(), _l_arr_1);
    std::array<pair<file_name_info_t<int>, string>, 3> _l_arr2 = {
        make_pair(
            file_name_info_t<int>(
                std::in_place_index<0>, file_name_basic_info_t(string("hello"))
            ),
            string("int")
        ),
        make_pair(
            file_name_info_t<int>(
                std::in_place_index<1>, file_name_basic_info_t(string("hello2"))
            ),
            "class std::tuple<unsigned __int64,unsigned __int64,unsigned "
            "__int64>"
        ),
        make_pair(
            file_name_info_t<int>(
                std::in_place_index<2>,
                make_pair(
                    file_name_basic_info_t(string("hello3")), rw_info_t<int>{}
                )
            ),
            "int"
        )
    };
    _l_run_func.operator()<int, 3>(_l_func_maker.operator()<int, 3>(), _l_arr2);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Check reading and writing of files for data generator with "
                 "file support",
         .path = "abc_test_test::component_tests::data_generators::data_"
                 "generator_with_file_support",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace _ABC_NS_UTILITY_IO;
    using namespace abc_test::utility;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace _ABC_NS_DS;
    using namespace _ABC_NS_UTILITY_PRINTER;
    auto _l_run_func
        = [&]<typename T, typename U, size_t N>(
              const std::function<data_generator_collection_t<
                  T>(const std::array<file_name_t<T>, N>&)>& _a_run_element,
              const std::array<
            std::tuple<
                      file_name_info_t<T>,
                      std::string,
            std::vector<std::variant<T, U>>,
            std::vector<std::variant<T, U>>>,
                  N>&          _a_files,
              const vector<T>& _a_elements
          )
    {
        // Create the file paths
        std::vector<std::tuple<
            filesystem::path,
            std::string,
            std::vector<std::variant<T, U>>,
            std::vector<std::variant<T, U>>>>
             _l_paths;
        auto ki = filesystem::current_path().string().append("\\paths\\hello");
        std::vector<std::vector<std::string>> _l_expected_files;
        for (const auto& [_l_file, _l_comment, _l_already_contained_elements, _l_file_contents_after] :
             _a_files)
        {
            _l_paths.push_back(make_tuple(
                make_path(_l_file, u8".gd", u8".td", ki),
                _l_comment,
                _l_already_contained_elements,
                _l_file_contents_after
            ));
        }
        // Make sure the files don't exist
        for (const auto& [_l_path, _l_comment, _l_already_contained_elements, _l_new_elements] :
             _l_paths)
        {
            vector<u8string> _l_expected_file;
            if (filesystem::exists(_l_path))
            {
                filesystem::remove(_l_path);
            }
            _l_expected_file.push_back(
                fmt::format(u8"#{0}", cast_string_to_u8string(_l_comment))
            );
            for (auto& _l_element : _l_already_contained_elements)
            {
                switch (_l_element.index())
                {
                case 0:
                    _l_expected_file.push_back(
                        default_printer_t<T>{}.run_printer(get<0>(_l_element))
                    );
                    break;
                case 1:
                    _l_expected_file.push_back(
                        default_printer_t<U>{}.run_printer(get<1>(_l_element))
                    );
                    break;
                }
            }
            ofstream _l_output(_l_path);
            for (auto& _l_line : _l_expected_file)
            {
                _l_output << string(_l_line.begin(), _l_line.end()) << endl;
            }
            _l_output.close();
            for (auto& _l_element : _l_new_elements)
            {
                switch (_l_element.index())
                {
                case 0:
                    _l_expected_file.push_back(
                        default_printer_t<T>{}.run_printer(get<0>(_l_element))
                    );
                    break;
                case 1:
                    _l_expected_file.push_back(
                        default_printer_t<U>{}.run_printer(get<1>(_l_element))
                    );
                    break;
                }
            }
            vector<string> _l_expected_file_2;
            for (auto& _l_line : _l_expected_file)
            {
                _l_expected_file_2.push_back(
                    string(_l_line.begin(), _l_line.end())
                );
            }
            _l_expected_files.push_back(_l_expected_file_2);
        }
        auto wrapped = [](auto&& x)
        {
            auto ki = get<0>(x);
            return make_file_name(std::forward<decltype(ki)>(ki));
        };
        // Actual test function.
        vector<T>        _l_elements_from_set;
        function<void()> _l_test_func_1 = [&]()
        {
            // This has to be done in here to get the correct file names.
            array<file_name_t<T>, N> arr2
                = abc_test::utility::transform_array(_a_files, wrapped);
            auto _l_generator{_a_run_element(arr2)};
            for (auto&& _l_element : _l_generator)
            {
                _CHECK_EXPR(_l_element == 0);
                _l_elements_from_set.push_back(_l_element);
            }
        };
        // Run the function
        vector<tuple<
            function<void()>,
            string,
            string,
            abc::ds::tdg_collection_stack_trie_t>>
                                    _l_funcs_to_run;
        tdg_collection_stack_trie_t _l_stack;
        string                      _l_name_1 = "hello";
        string                      _l_path   = "paths";
        _l_funcs_to_run.push_back(
            make_tuple(_l_test_func_1, _l_name_1, _l_path, _l_stack)
        );
        abc_test::utility::abc_test_tests_internal_test_runner(
            1, true, _l_funcs_to_run
        );
        // Check the function creates the files.
        auto unit_tests = _MULTI_MATCHER("Check files exist");
        unit_tests << _CHECK_EXPR(_a_elements == _l_elements_from_set);
        for (size_t _l_idx{0};
             auto& [_l_path, _l_comment, _l_three, _l_four] : _l_paths)
        {
            unit_tests << _CHECK(_EXPR(filesystem::exists(_l_path) == true));
            auto _l_contents{get_file_contents(_l_path)};
            unit_tests << _CHECK_EXPR(
                _l_contents == _l_expected_files[_l_idx++]
            );
        }
        _CHECK(unit_tests);
    };
    auto _l_func_maker = [&]<typename T, size_t N>()
        -> function<data_generator_collection_t<
            T>(const std::array<file_name_t<T>, N>&)>
    {
        return [](const std::array<file_name_t<T>, N>& _a_array)
        {
            auto _l_applied_func = std::apply(
                [](auto&&... args)
                {
                    return enumerate_data<T>(
                        enumerate_list<T>({1, 2, 3}),
                        std::forward<decltype(args)>(args)...
                    );
                },
                _a_array
            );
            return _l_applied_func;
        };
    };
    using u_type = size_t;
    array<
        tuple<
            file_name_info_t<int>,
            string,
            vector<variant<int, u_type>>,
            vector<variant<int, u_type>>>,
        0>
                _l_arr      = {};
    vector<int> _l_elements = {1, 2, 3};
    _l_run_func.operator(
    )<int, u_type, 0>(_l_func_maker.operator()<int, 0>(), _l_arr, _l_elements);

    array<
        tuple<
            file_name_info_t<int>,
            string,
            vector<variant<int, u_type>>,
            vector<variant<int, u_type>>>,
        1>
                _l_arr_1      = {make_tuple(
            file_name_info_t<int>(
                std::in_place_index<0>, file_name_basic_info_t(string("hello"))
            ),
            string("int"),
            vector<variant<int, u_type>>({4, 5, 6}),
            vector<variant<int, u_type>>({1, 2, 3})
        )};
    vector<int> _l_elements_1 = {4, 5, 6, 1, 2, 3};
    _l_run_func.operator()<int, u_type, 1>(
        _l_func_maker.operator()<int, 1>(), _l_arr_1, _l_elements_1
    );
    array<
        tuple<
            file_name_info_t<int>,
            string,
            vector<variant<int, u_type>>,
            vector<variant<int, u_type>>>,
        3>
        _l_arr_2
        = {make_tuple(
               file_name_info_t<int>(
                   std::in_place_index<0>,
                   file_name_basic_info_t(string("hello"))
               ),
               string("int"),
               vector<variant<int, u_type>>({4, 5, 6}),
               vector<variant<int, u_type>>({1, 2, 3})
           ),
           make_tuple(
               file_name_info_t<int>(
                   std::in_place_index<1>,
                   file_name_basic_info_t(string("hello2"))
               ),
               "unsigned __int64",
               vector<variant<int, u_type>>({0, 1, 2}),
               vector<variant<int, u_type>>({0, 1, 2})
           ),
           make_tuple(
               file_name_info_t<int>(
                   std::in_place_index<2>,
                   make_pair(
                       file_name_basic_info_t(string("hello3")),
                       rw_info_t<int>{}
                   )
               ),
               "int",
               vector<variant<int, u_type>>({25, 23, 27}),
               vector<variant<int, u_type>>({1, 2, 3})
           )};
    vector<int> _l_elements_2 = {4, 5, 6, 1, 2, 3};
    _l_run_func.operator()<int, u_type, 3>(
        _l_func_maker.operator()<int, 3>(), _l_arr_2, _l_elements_2
    );
}