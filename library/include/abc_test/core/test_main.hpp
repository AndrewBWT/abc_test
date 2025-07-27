#pragma once

#include "abc_test/core/ds/test_collections/test_collection.hpp"
#include "abc_test/core/global.hpp"
#include "abc_test/core/global/test_framework_global_variable_set.hpp"
#include "abc_test/core/reporters/test_reporter.hpp"
#include "abc_test/core/reporters/test_reporter_controller.hpp"
#include "abc_test/core/test_evaluator.hpp"
#include "abc_test/included_instances/reporters/text_error_reporter.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter.hpp"
#include "abc_test/included_instances/test_harness/simple_reporter.hpp"
#include "abc_test/utility/cli.hpp"

#include <condition_variable>
#include <memory>
#include <set>
#include <syncstream>
#include <thread>


_BEGIN_ABC_NS

/*!
 * @brief This function runs the tests.
 *
 * On the use of the GLOT.
 *
 * Most use cases of this function assume it is only called once to create a
 * test suite.
 *
 * However, abc_test bootstraps its testing aparatus, using it to test itself.
 * We use it internally to run tests in a isolated enviornment.
 *
 * Using the GLOT in such an enviornment would create an infinite loop of test
 * harnesses.
 *
 * The bool tempalte argument dictates whether running run_tests can access the
 * GLOT.
 */
template <typename T, bool GLOT_Available = false>
__no_constexpr int
    run_tests(
        T&                                            _a_options,
        simple_reporter_t&                            _a_simple_reporter,
        const std::optional<_ABC_NS_UTILITY_CLI::cli_t<T>>& _a_cli = std::optional<_ABC_NS_UTILITY_CLI::cli_t<T>>{},
        const ds::memoized_cli_history_t&             _a_cli_history
        = ds::memoized_cli_history_t()
    ) noexcept;

_END_ABC_NS

_BEGIN_ABC_NS

template <typename T, bool GLOT_Available>
__no_constexpr_imp int
    run_tests(
        T&                                                  _a_options,
        simple_reporter_t&                                  _a_simple_reporter,
        const std::optional<_ABC_NS_UTILITY_CLI::cli_t<T>>& _a_opt_cli,
        const ds::memoized_cli_history_t&                   _a_cli_history
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_DS;
    using namespace _ABC_NS_REPORTERS;
    using namespace _ABC_NS_GLOBAL;
    using namespace _ABC_NS_ERRORS;
    using enum _ABC_NS_UTILITY::internal::internal_log_enum_t;
    // The function which runs an individual test.
    mutex                    _l_thread_pool_mutex;
    mutex                    _l_threads_mutex;
    condition_variable       _l_cv;
    vector<jthread>          _l_threads;
    vector<test_evaluator_t> _l_test_runners;
    set<size_t>              _l_threads_free;
    _a_options.pre_validation_process();
    if (auto _l_validation_errors{_a_options.validate()};
        _l_validation_errors.has_value())
    {
        _a_simple_reporter.write_error_lines(_l_validation_errors.value());
        return -1;
    }
    else
    {
        _a_options.post_validation_process();
        for (size_t _l_idx{numeric_limits<size_t>::min()};
             _l_idx < _a_options.group_test_options.threads;
             ++_l_idx)
        {
            _l_threads_free.insert(_l_idx);
        }
        _l_threads.resize(_a_options.group_test_options.threads);
        size_t _l_current_thread_pool{_a_options.group_test_options.threads};
        auto   _l_run_individual_test
            = [&](const _ABC_NS_DS::post_setup_test_data_t& _a_prtd,
                  const size_t                              _a_thread_idx,
                  test_evaluator_t&                         _a_test_runner,
                  const std::size_t                         _a_order_ran_id,
                test_framework_global_variable_set_t* _a_global_var_set_to_use)
        {
            // Get the thread runner
            push_this_threads_test_runner_and_global_var_set(&_a_test_runner, _a_global_var_set_to_use);
            test_evaluator_t& _l_threads_runner{
                get_this_threads_test_evaluator_ref()
            };
            // run in try
            try
            {
                _LIBRARY_LOG(MAIN_INFO, u8"Running test.");
                _l_threads_runner.run_test(_a_prtd, _a_order_ran_id);
            }
            // Catch if its a library error.
            catch (test_library_exception_t& _l_the)
            {
                _LIBRARY_LOG(
                    MAIN_INFO, u8"Exception encountered when running test."
                );
                error_reporter_controller_t& _l_erc{
                    get_this_threads_error_reporter_controller()
                };
                const string_view _l_error{_l_the.what()};
                _l_erc.report_error(setup_error_t(
                    u8string(_l_error.begin(), _l_error.end()),
                    true,
                    _l_the.stacktrace()
                ));
            }
            catch (...)
            {
                _LIBRARY_LOG(
                    MAIN_INFO, "Unknown exception caught when running test."
                );
                error_reporter_controller_t& _l_erc{
                    get_this_threads_error_reporter_controller()
                };
                _l_erc.report_error(setup_error_t(
                    u8"Unknown exception thrown from test_runner_t.", true
                ));
            }
            _LIBRARY_LOG(MAIN_INFO, "Test finished.");
            _LIBRARY_LOG(MAIN_INFO, "Freeing thread resourses.");
            unique_lock _l_thread_lock(_l_threads_mutex);
            _l_threads_free.insert(_a_thread_idx);
            unique_lock _l_thread_pool_lock(_l_thread_pool_mutex);
            _l_current_thread_pool += _a_prtd.thread_resourses_required();
            _l_cv.notify_one();
            _l_threads_runner.set_data_process_test();
            pop_this_threads_test_runner_and_global_var_set();
        };
        _LIBRARY_LOG(
            MAIN_INFO,
            "run_tests() beginning.\nSetting up global and thread local "
            "variables..."
        );
        auto _l_make_ref_collection
            = []<typename T>(const std::vector<std::shared_ptr<T>>& _a_ptrs)
        {
            vector<reference_wrapper<const T>> _l_rv{};
            for (const shared_ptr<T>& _l_ptr : _a_ptrs)
            {
                _l_rv.push_back(std::ref(*_l_ptr));
            }
            return _l_rv;
        };
        test_framework_global_variable_set_t _l_local_lfgvs(
            &_a_options,
            _l_make_ref_collection(_a_options.group_test_options.error_reporters
            ),
            _l_make_ref_collection(_a_options.group_test_options.test_reporters)
        );
        const test_framework_global_variable_set_t& _l_tfgvs{
            push_this_threads_variable_set(&_l_local_lfgvs)
        };
        const T& _l_global_test_options{
            *(static_cast<const T*>(_l_tfgvs.test_options()))
        };
        error_reporter_controller_t& _l_erc{
            get_this_threads_error_reporter_controller()
        };
        test_reporter_controller_t& _l_trc{get_this_threads_test_reporter_controller()
        };
        _LIBRARY_LOG(
            MAIN_INFO, "Adding test sets to local test_collection_t..."
        );
        test_collection_t _l_tc;
        _l_tc.add_tests(
            [&]()
            {
                test_lists_t _l_rv{};
                if constexpr (GLOT_Available)
                {
                    if (_l_global_test_options.glot_aware_test_options
                            .use_global_test_list)
                    {
                        _l_rv.push_back(std::ref(get_global_test_list()));
                    }
                }
                for (const shared_ptr<test_list_t>& _l_ptr :
                     _l_global_test_options.group_test_options.test_lists)
                {
                    _l_rv.push_back(std::ref(*_l_ptr));
                }
                return _l_rv;
            }()
        );
        if (_l_erc.soft_exit())
        {
            _LIBRARY_LOG(
                MAIN_INFO,
                "Exiting due to error when adding tests to test_collection_t"
                "object."
            );
            return -1;
        }
        _LIBRARY_LOG(
            MAIN_INFO, "Getting final set of tests in execution order"
        );
        const post_setup_test_list_t _l_pstd{
            _l_tc.make_finalied_post_setup_test_list_in_run_order()
        };
        ds::pre_test_run_report_t _l_pre_test_run_report(
            _a_cli_history, &_l_global_test_options
        );
        _l_pre_test_run_report.report_all_tests(_l_pstd.size());
        post_setup_test_list_itt_t       _l_pstd_itt{_l_pstd.begin()};
        const post_setup_test_list_itt_t _l_pstd_end{_l_pstd.end()};
        _l_test_runners = vector<test_evaluator_t>(
            _l_global_test_options.group_test_options.threads,
            test_evaluator_t()
        );
        size_t _l_order_ran_id_counter{0};
        _l_trc.report_pre_test_data(_l_pre_test_run_report);
        _LIBRARY_LOG(MAIN_INFO, "Beginning running of tests...");
        while (_l_pstd_itt != _l_pstd_end && _l_erc.should_exit() == false)
        {
            // Get the current element in the list.
            const post_setup_test_data_t& _l_test{(*_l_pstd_itt++).get()};
            const size_t _l_order_ran_id{_l_order_ran_id_counter++};
            _LIBRARY_LOG(MAIN_INFO, fmt::format("Loaded test {0}.", _l_test));
            // Find out the resourses required for this test;
            const size_t _l_next_thread_size{_l_test.thread_resourses_required()
            };
            // If we are unable to allocate the required resourses
            if (_l_current_thread_pool < _l_next_thread_size)
            {
                _LIBRARY_LOG(
                    MAIN_INFO,
                    fmt::format(
                        "Waiting as _l_current_thread_pool ({0}) < "
                        "_l_thread_next_size ({1}).",
                        _l_current_thread_pool,
                        _l_next_thread_size
                    )
                );
                // Wait until we do have the resourses.
                unique_lock _l_thread_pool_lock(_l_thread_pool_mutex);
                _l_cv.wait(
                    _l_thread_pool_lock,
                    [&]
                    {
                        return _l_current_thread_pool >= _l_next_thread_size;
                    }
                );
            }
            // Check we actually have the resourses... should we lock it here
            // before doing this?
            if (_l_erc.should_exit() == false && _l_threads_free.size() > 0)
            {
                // Otherwise, get a lock for the threads, remove the
                // resousrses...
                unique_lock  _l_thread_lock(_l_threads_mutex);
                const size_t _l_thread_idx{*_l_threads_free.begin()};
                _l_threads_free.erase(_l_thread_idx);
                _l_current_thread_pool -= _l_test.thread_resourses_required();
                _l_thread_lock.unlock();
                // Then run the thread
                _LIBRARY_LOG(MAIN_INFO, "Starting new thread to run test.");
                _l_threads[_l_thread_idx] = jthread(
                    _l_run_individual_test,
                    _l_test,
                    _l_thread_idx,
                    std::ref(_l_test_runners[_l_thread_idx]),
                    _l_order_ran_id,
                    &_l_local_lfgvs
                );
            }
            else
            {
                _LIBRARY_LOG(
                    MAIN_INFO, "_l_erc == false, we are terminating this loop."
                );
            }
        }
        // Wait for all the threads to finish
        _LIBRARY_LOG(MAIN_INFO, "Waiting for all threads to finish.");
        for (jthread& _l_thread : _l_threads)
        {
            if (_l_thread.joinable())
            {
                _l_thread.join();
            }
        }
        _LIBRARY_LOG(MAIN_INFO, "All threads finished.");
        if (_l_erc.should_exit())
        {
            _LIBRARY_LOG(
                MAIN_INFO, "Catastrophic error found when running tests."
            );
            _l_erc.report_information(fmt::format(
                "One (or more) of thre tests reported a catastrophic error "
                "which "
                "could not be recovered from. "
                "The entire testing harness was terminated. There were {0} "
                "tests "
                "which were not run at all, and "
                "{1} terminated due to catastrophic error(s). "
                "We would highly suggest either checking your program, or "
                "contacting the developer of this library.",
                std::distance(_l_pstd_itt, _l_pstd_end),
                _l_erc.catastrophic_errors()
            ));
            _l_erc.hard_exit();
        }
        _LIBRARY_LOG(MAIN_INFO, "Setting up auto configuration");
        finalised_test_set_data_t _l_final_report;
        for (auto& _l_test_runner : _l_test_runners)
        {
            _l_final_report.process_final_report(_l_test_runner.test_set_data()
            );
        }
        if (_a_opt_cli.has_value())
        {
            auto _l_cli{_a_opt_cli.value()};
            if (_l_cli.auto_configuration().has_value())
            {
                _l_cli.setup_next_file(
                    _a_options.cli_test_options.autofile_name,
                    _a_options.cli_test_options.autofile_size,
                    _l_final_report.get_re_run_test_options<T>(),
                    _l_final_report.total_tests_failed() == 0
                );
            }
        }
        _LIBRARY_LOG(MAIN_INFO, "Finalising reports.");
        _l_trc.finalise_reports(_l_final_report);
        global::pop_this_threads_global_variable_set();
        return 0;
    }
}

_END_ABC_NS
