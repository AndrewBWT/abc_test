#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <vector>
#include <functional>
#include <string>
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/included_instances/reporters/memoized_error_reporter.hpp"
#include "abc_test/included_instances/reporters/memoized_test_reporter.hpp"
namespace abc_test::utility
{
    __no_constexpr_imp void
        abc_test_tests_internal_test_runner(
            const std::size_t _a_n_threads,
            const bool _a_write_data_to_files,
            const std::vector<
            std::tuple<std::function<void()>, std::string, std::string,
            abc::ds::tdg_collection_stack_trie_t>>&_a_functions_to_run
        )
    {
        using namespace abc;
        using namespace abc::data_gen;
        using namespace std;
        using namespace utility;
        using namespace abc::utility::io;
        using namespace _ABC_NS_UTILITY_STR;
        using namespace _ABC_NS_DS;
        shared_ptr<_ABC_NS_DS::test_list_t> _l_tests_to_run
            = make_shared<_ABC_NS_DS::test_list_t>();
        test_options_base_t _l_running_options;
        _l_running_options.group_test_options.threads = _a_n_threads;
        _l_running_options.group_test_options.test_lists.push_back(_l_tests_to_run);
        _l_running_options.group_test_options.error_reporters.push_back(
            make_shared<reporters::memoized_error_reporter_t>()
        );
        _l_running_options.group_test_options.test_reporters.push_back(
            make_shared<reporters::memoized_test_reporter_t>()
        );
        _l_running_options.individual_io_based_test_options.write_data_to_files
            = _a_write_data_to_files;

        for (auto&& [_l_function, _l_name, _l_path, _l_repetition_info] : _a_functions_to_run)
        {
            test_function_t ki = std::make_shared<
                test_function_callable_object_t<decltype(_l_function)>>(_l_function
                    );
            abc::ds::registered_test_data_t _l_rtd(
                ki,
                abc::ds::checked_user_defined_test_data_t::test_data(
                    abc::ds::user_defined_test_data_t{
                        .name = _l_name, .path = _l_path
                    }
                )
            );
            _l_tests_to_run->push_back(_l_rtd);
            u8string _l_uuid;

            _l_uuid.append(cast_string_to_u8string(_l_path))
                .append(_l_running_options.group_test_options.path_delimiter)
                .append(cast_string_to_u8string(_l_name));
            if (not _a_write_data_to_files)
            {
                _l_running_options.group_test_options
                    .map_of_unique_ids_and_for_loop_stack_tries.insert(
                        _l_uuid, _l_repetition_info
                    );
            }
        }
        simple_console_reporter_t _l_sco;
        abc::run_tests<test_options_base_t>(_l_running_options, _l_sco);
    }
} // namespace abc_test::utility