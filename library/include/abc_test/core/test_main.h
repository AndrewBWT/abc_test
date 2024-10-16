#pragma once

#include "abc_test/core/ds/test_collections/test_collection.h"
#include "abc_test/core/reporters/test_reporter.h"
#include "abc_test/core/reporters/test_reporter_controller.h"
#include "abc_test/core/options/validated_test_options.h"
#include "abc_test/core/test_runner.h"
#include "abc_test/global.h"
#include "abc_test/included_instances/reporters/text_error_reporter.h"
#include "abc_test/included_instances/reporters/text_test_reporter.h"

#include <memory>
#include <set>
#include <syncstream>
#include <thread>

_BEGIN_ABC_NS

/*!
 * The main runner. This object holds the majority of logic concerning the
 * ordering of things within the system.
 */
struct test_main_t
{
public:
    __constexpr
        test_main_t() = delete;
    /*!
     * Default constructor. This should be generally used.
     */
    __no_constexpr
        test_main_t(
        const validated_test_options_t& _a_validated_test_options
        ) noexcept;
    /*!
     * Runs the test of the underlying object.
     */
    __no_constexpr void
        run_tests() noexcept;
private:
    ds::test_lists_t                 _m_test_list_collection;
    test_options_base_t                   _m_options;
    reporters::test_reporters_t      _m_test_reporters;
    reporters::error_reporters_t     _m_error_reporters;
    std::mutex                       _m_mutex;
    size_t                           _m_thread_pool;
    std::mutex                       _m_thread_pool_mutex;
    std::condition_variable          _m_cv;
    size_t                           _m_current_thread_pool;
    std::mutex                       _m_threads_mutex;
    std::vector<std::jthread>        _m_threads;
    std::vector<ds::test_set_data_t> _m_test_set_data;
    std::set<std::size_t>            _m_threads_free;
    __no_constexpr void
        run_individual_thread(
            const ds::post_setup_test_data_t& _a_prtd,
            const size_t                      _a_thread_idx,
            ds::test_set_data_t&              _a_test_set_data
        ) noexcept;
    __constexpr void
        validate_reporters(const test_options_base_t& _a_test_options) noexcept;
};

using tests_in_order_t = std::vector<int>;
_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp
    test_main_t::test_main_t(const validated_test_options_t& _a_validated_test_options) noexcept
    : _m_test_list_collection(ds::test_lists_t())
    , _m_options(_a_validated_test_options.get_options())
    , _m_test_reporters(reporters::test_reporters_t())
    , _m_error_reporters(reporters::error_reporters_t())
    , _m_mutex(std::mutex())
    , _m_thread_pool(0)
    , _m_thread_pool_mutex(std::mutex())
    , _m_cv(std::condition_variable())
    , _m_current_thread_pool(std::thread::hardware_concurrency())
    , _m_threads_mutex(std::mutex())
    , _m_threads(std::vector<std::jthread>(std::thread::hardware_concurrency()))
    , _m_threads_free(std::set<std::size_t>())
    , _m_test_set_data(std::vector<ds::test_set_data_t >(std::thread::hardware_concurrency()))
{
    for (size_t _l_idx{0}; _l_idx < _m_current_thread_pool; _l_idx++)
    {
        _m_threads_free.insert(_l_idx);
    }
}

__no_constexpr_imp void
    test_main_t::run_tests() noexcept
{
    using namespace std;
    using namespace ds;
    using namespace reporters;
    using enum utility::internal::internal_log_enum_t;
    // Setup of data before running
    _m_thread_pool = (_m_options.threads > thread::hardware_concurrency()
                      || _m_options.threads == 0)
                         ? thread::hardware_concurrency()
                         : _m_options.threads;
    _LIBRARY_LOG(
        MAIN_INFO,
        fmt::format(
            "_m_thread_pool = {0}. Due to "
            "_m_options._m_threads = {1} and "
            "thread::hardware_concurrency() = {2}",
            _m_thread_pool,
            _m_options.threads,
            thread::hardware_concurrency()
        )
    );
    _LIBRARY_LOG(MAIN_INFO, "Setting up global error reporters");

    error_reporter_controller_t& _l_erc{
        global::get_global_error_reporter_controller()
    };
    _LIBRARY_LOG(MAIN_INFO, "Validating reporters.");
    validate_reporters(_m_options);
    _l_erc.add_reporters(_m_error_reporters);
    _LIBRARY_LOG(MAIN_INFO, "Validating test_options_t.");
   // _m_options.validate_input(_l_erc);
    if (_l_erc.soft_exit())
    {
        _LIBRARY_LOG(MAIN_INFO, "Exiting due to invalid test_options_t.");
        return;
    }
    global::setup_global_variables(_m_options);
    const test_options_base_t& _l_global_test_options{ global::get_global_test_options() };
    test_collection_t _l_tc(_l_global_test_options, _l_erc);
    _LIBRARY_LOG(MAIN_INFO, "Adding tests to test_collection_t.");
    _l_tc.add_tests(_m_test_list_collection);
    if (_l_erc.soft_exit())
    {
        _LIBRARY_LOG(MAIN_INFO, "Exiting due to error when adding tests.");
        return;
    }
    _LIBRARY_LOG(
        MAIN_INFO, "Running make_finalied_post_setup_test_list_in_run_order"
    );
    const post_setup_test_list_t _l_pstd{
        _l_tc.make_finalied_post_setup_test_list_in_run_order()
    };
    post_setup_test_list_itt_t       _l_pstd_itt{_l_pstd.begin()};
    const post_setup_test_list_itt_t _l_pstd_end{_l_pstd.end()};
    test_reporter_controller_t& _l_global_trc{
    global::get_global_test_reporter_controller()
    };
    _l_global_trc.add_reporters(_m_test_reporters);
    // The actual logic for running the threads
    _LIBRARY_LOG(MAIN_INFO, "Beginning loop going through all tests.");
    while (_l_pstd_itt != _l_pstd_end && _l_erc.should_exit() == false)
    {
        // Get the current element in the list.
        const post_setup_test_data_t& _l_test{(*_l_pstd_itt++).get()};
        _LIBRARY_LOG(MAIN_INFO, fmt::format("Loaded test {0}.", _l_test));
        // Find out the resourses required for this test;
        const size_t _l_next_thread_size{_l_test.thread_resourses_required()};
        // If we are unable to allocate the required resourses
        if (_m_current_thread_pool < _l_next_thread_size)
        {
            _LIBRARY_LOG(
                MAIN_INFO,
                fmt::format(
                    "Waiting as _m_current_thread_pool ({0}) < "
                    "_l_thread_next_size ({1}).",
                    _m_current_thread_pool,
                    _l_next_thread_size
                )
            );
            // Wait until we do have the resourses.
            unique_lock _l_thread_pool_lock(_m_thread_pool_mutex);
            _m_cv.wait(
                _l_thread_pool_lock,
                [_l_next_thread_size, this]
                {
                    return _m_current_thread_pool >= _l_next_thread_size;
                }
            );
        }
        if (_l_erc.should_exit() == false && _m_threads_free.size() > 0)
        {
            // Otherwise, get a lock for the threads, remove the resousrses...
            unique_lock  _l_thread_lock(_m_threads_mutex);
            const size_t _l_thread_idx{*_m_threads_free.begin()};
            _m_threads_free.erase(_l_thread_idx);
            _l_thread_lock.unlock();
            // Then run the thread
            _LIBRARY_LOG(MAIN_INFO, "Starting new thread to run test.");
            _m_threads[_l_thread_idx] = jthread(
                &test_main_t::run_individual_thread,
                this,
                _l_test,
                _l_thread_idx,
                std::ref(_m_test_set_data[_l_thread_idx])
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
    for (jthread& _l_thread : _m_threads)
    {
        if (_l_thread.joinable())
        {
            _l_thread.join();
        }
    }
    _LIBRARY_LOG(MAIN_INFO, "All threads finished.");
    if (_l_erc.should_exit())
    {
        _LIBRARY_LOG(MAIN_INFO, "Catastrophic error found when running tests.");
        _l_erc.report_information(fmt::format(
            "One (or more) of thre tests reported a catastrophic error which "
            "could not be recovered from. "
            "The entire testing harness was terminated. There were {0} tests "
            "which were not run at all, and "
            "{1} terminated due to catastrophic error(s). "
            "We would highly suggest either checking your program, or "
            "contacting the developer of this library.",
            std::distance(_l_pstd_itt, _l_pstd_end),
            _l_erc.catastrophic_errors()
        ));
        _l_erc.hard_exit();
    }
    _LIBRARY_LOG(MAIN_INFO, "Finalising reports.");
    ds::test_set_data_t _l_final_report(_m_test_set_data);
    _l_global_trc.finalise_reports(_l_final_report);
}

__no_constexpr_imp void
    test_main_t::run_individual_thread(
        const ds::post_setup_test_data_t& _a_prtd,
        const size_t                      _a_thread_idx,
        ds::test_set_data_t&              _a_test_set_data
    ) noexcept
{
    using namespace std;
    using namespace errors;
    using enum utility::internal::internal_log_enum_t;
    unique_lock _l_thread_pool_lock(_m_thread_pool_mutex);
    _LIBRARY_LOG(MAIN_INFO, "Updating thread pool.");
    _m_current_thread_pool -= _a_prtd.thread_resourses_required();
    _l_thread_pool_lock.unlock();
    test_runner_t& _l_threads_runner{global::get_this_threads_test_runner_ref()
    };
    try
    {
        _LIBRARY_LOG(MAIN_INFO, "Running test.");
        _l_threads_runner.run_test(_a_prtd);
    }
    catch (test_library_exception_t& _l_the)
    {
        _LIBRARY_LOG(MAIN_INFO, "Exception encountered when running test.");
        reporters::error_reporter_controller_t& _l_erc{
            global::get_global_error_reporter_controller()
        };
        _l_erc.report_error(
            setup_error_t(_l_the.error(), true, _l_the.stacktrace())
        );
    }
    _LIBRARY_LOG(MAIN_INFO, "Test finished.");
    unique_lock _l_thread_lock(_m_threads_mutex);
    _LIBRARY_LOG(MAIN_INFO, "Freeing thread resourses.");
    _m_threads_free.insert(_a_thread_idx);
    _l_thread_pool_lock.lock();
    _m_current_thread_pool += _a_prtd.thread_resourses_required();
    _m_cv.notify_one();
    _a_test_set_data.process_invoked_test(_l_threads_runner.current_test());
    return;
}

__constexpr_imp void
    test_main_t::validate_reporters(
        const test_options_base_t& _a_test_options
    ) noexcept
{
    using namespace std;
    using namespace reporters;
    if (_m_test_reporters.size() == 0)
    {
       // if (_a_test_options._m_allow_empty_test_reporters_to_be_used)
        {
            cout << "WARNING: No test reporters have been registered to the "
                    "test_main_t object. "
                    "The test suite will be ran, however no results of any "
                    "kind will be reported. "
                    "If this circumstance is encountered again, and you wish "
                    "for a simple console test reporter "
                    "to be added, in the associated test_options_t object, set "
                    "\"_m_allow_empty_test_reporters_to_be_used\" to false."
                 << endl;
        }
     //   else
        {
            cout << "WARNING: No test reporters have been registered to the "
                    "test_main_t object. "
                    "A basic console output test reporter will be added, as we "
                    "assume this is in error. "
                    "If you wish to run the test_main_t object without any "
                    "test reporters, in the associated "
                    "test_options_t object, set "
                    "\"_m_allow_empty_test_reporters_to_be_used\" to true."
                 << endl;
            _m_test_reporters.push_back(
                shared_ptr<test_reporter_t>(new text_test_reporter_t())
            );
        }
    }
    if (_m_error_reporters.size() == 0)
    {
      //  if (_a_test_options._m_allow_empty_error_reporters_to_be_used)
        {
            cout << "WARNING: No error reporters have been registered to the "
                    "test_main_t object. "
                    "The test suite will be ran, however no internal logic "
                    "errors will be reported. "
                    "If this circumstance is encountered again, and you wish "
                    "for a simple console error reporter "
                    "to be added, in the associated test_options_t object, set "
                    "\"_m_allow_empty_error_reporters_to_be_used\" to false."
                 << endl;
        }
      //  else
        {
            cout << "WARNING: No error reporters have been registered to the "
                    "test_main_t object. "
                    "A basic console output error reporter will be added, as "
                    "we assume this is in error. "
                    "If you wish to run the test_main_t object without any "
                    "error reporters, in the associated "
                    "test_options_t object, set "
                    "\"_m_allow_empty_error_reporters_to_be_used\" to true."
                 << endl;
            _m_error_reporters.push_back(
                shared_ptr<error_reporter_t>(new text_error_reporter_t())
            );
        }
    }
}

_END_ABC_NS