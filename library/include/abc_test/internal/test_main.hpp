#pragma once

#include "abc_test/included_instances/reporters/text_error_reporter.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter.hpp"
#include "abc_test/internal/ds/test_collections/test_collection.hpp"
#include "abc_test/internal/global.hpp"
#include "abc_test/internal/options/validated_test_options.hpp"
#include "abc_test/internal/reporters/test_reporter.hpp"
#include "abc_test/internal/reporters/test_reporter_controller.hpp"
#include "abc_test/internal/test_runner.hpp"

#include <memory>
#include <set>
#include <syncstream>
#include <thread>

_BEGIN_ABC_NS

/*!
 * @brief This class controls the core logic of the test suite.
 */
struct test_main_t
{
public:
    __constexpr
    test_main_t()
        = delete;
    /*!
     * @brief Constructor
     * @param _a_validated_test_options The validated test options used for
     * running the tests.
     */
    template<typename T>
    __no_constexpr
        test_main_t(const validated_test_options_t<T>& _a_validated_test_options
        ) noexcept;
    /*!
     * @brief This function runs the tests.
     *
     * We would not recomend running other code in a multi-threaded enviornment
     * which accesses the global variables described in global.h. This class
     * assumes it is the only entity (as well as those threads it spawns) which
     * can access those variables.
     */
    __no_constexpr void
        run_tests(ds::pre_test_run_report_t& _a_test_set_data) noexcept;
private:
    _ABC_NS_DS::test_lists_t             _m_test_list_collection;
    test_options_base_t                  _m_options;
    _ABC_NS_REPORTERS::test_reporters_t  _m_test_reporters;
    _ABC_NS_REPORTERS::error_reporters_t _m_error_reporters;
    std::mutex                           _m_mutex;
    size_t                               _m_thread_pool;
    std::mutex                           _m_thread_pool_mutex;
    std::condition_variable              _m_cv;
    size_t                               _m_current_thread_pool;
    std::mutex                           _m_threads_mutex;
    std::vector<std::jthread>            _m_threads;
    // std::vector<_ABC_NS_DS::test_set_data_t> _m_test_set_data;
    std::vector<test_runner_t> _m_test_runners;
    std::set<std::size_t>      _m_threads_free;
    /*!
     * @brief Inner constructor. Protected so it can't be used outside of this
     * class.
     * @param _a_test_opts The test_options_base_t object.
     */
    __no_constexpr
        test_main_t(const test_options_base_t& _a_test_opts) noexcept;
    /*!
     * @brief Runs an individual test in an individual thread.
     * @param _a_prtd The post_setup_test_data_t to run.
     * @param _a_thread_idx The thread index.
     * @param _a_test_set_data The test_set_data_t element to write data to.
     */
    __no_constexpr void
        run_individual_test(
            const _ABC_NS_DS::post_setup_test_data_t& _a_prtd,
            const size_t                              _a_thread_idx,
            test_runner_t&                            _a_test_runner,
            const std::size_t _a_order_ran_id
            //_ABC_NS_DS::test_set_data_t&              _a_test_set_data
        );
};

namespace
{
__constexpr _ABC_NS_DS::test_lists_t
            make_test_list_collection(
                const std::vector<std::shared_ptr<_ABC_NS_DS::test_list_t>>&
                           _a_ptrs_of_test_list,
                const bool _a_use_global_test_list
            ) noexcept;
template <typename T>
__constexpr std::vector<std::reference_wrapper<const T>>
    make_ref_collection(std::vector<std::shared_ptr<T>> _a_ptrs) noexcept;
template <typename T>
__constexpr std::set<T>
            set_from_min_to_n(const T _a_max) noexcept;
} // namespace

_END_ABC_NS

_BEGIN_ABC_NS
template<typename T>
__no_constexpr_imp
    test_main_t::test_main_t(
        const validated_test_options_t<T>& _a_validated_test_options
    ) noexcept
    : test_main_t(_a_validated_test_options.get_options())
{}

__no_constexpr_imp
    test_main_t::test_main_t(
        const test_options_base_t& _a_to
    ) noexcept
    : _m_test_list_collection(make_test_list_collection(
          _a_to.test_lists,
          _a_to.use_global_test_list
      ))
    , _m_options(_a_to)
    , _m_test_reporters(make_ref_collection(_a_to.test_reporters))
    , _m_error_reporters(make_ref_collection(_a_to.error_reporters))
    , _m_thread_pool(_a_to.threads)
    , _m_current_thread_pool(_a_to.threads)
    , _m_threads(std::vector<std::jthread>(_a_to.threads))
    , _m_threads_free(set_from_min_to_n(_a_to.threads))
//, _m_test_set_data(std::vector<_ABC_NS_DS::test_set_data_t>(_a_to.threads))
{}

__no_constexpr_imp void
    test_main_t::run_tests(ds::pre_test_run_report_t& _a_test_set_data) noexcept
{
    using namespace std;
    using namespace _ABC_NS_DS;
    using namespace _ABC_NS_REPORTERS;
    using namespace _ABC_NS_GLOBAL;
    using enum _ABC_NS_UTILITY::internal::internal_log_enum_t;
    _LIBRARY_LOG(
        MAIN_INFO, "run_tests() beginning.\nSetting up global test options..."
    );

    const test_options_base_t& _l_global_test_options{get_global_test_options()
    };
    setup_global_variables(_m_options);
    set_global_seed();
    _LIBRARY_LOG(MAIN_INFO, "Setting up global error reporter controller...");
    error_reporter_controller_t& _l_erc{get_global_error_reporter_controller()};
    _l_erc.add_reporters(_m_error_reporters);

    _LIBRARY_LOG(MAIN_INFO, "Setting up global test reporter controller...");
    test_reporter_controller_t& _l_trc{get_global_test_reporter_controller()};
    _l_trc.add_reporters(_m_test_reporters);
    _LIBRARY_LOG(MAIN_INFO, "Adding test sets to local test_collection_t...");
    test_collection_t _l_tc(_l_global_test_options, _l_erc);
    _l_tc.add_tests(_m_test_list_collection);
    if (_l_erc.soft_exit())
    {
        _LIBRARY_LOG(
            MAIN_INFO,
            "Exiting due to error when adding tests to test_collection_t"
            "object."
        );
        return;
    }
    _LIBRARY_LOG(MAIN_INFO, "Getting final set of tests in execution order");
    const post_setup_test_list_t _l_pstd{
        _l_tc.make_finalied_post_setup_test_list_in_run_order()
    };
    post_setup_test_list_itt_t       _l_pstd_itt{_l_pstd.begin()};
    const post_setup_test_list_itt_t _l_pstd_end{_l_pstd.end()};

    _m_test_runners = vector<test_runner_t>(
        _l_global_test_options.threads,
        test_runner_t(_l_trc, _l_global_test_options)
    );
    size_t _l_order_ran_id_counter{ 0 };
    _l_trc.report_pre_test_data(_a_test_set_data);
    _LIBRARY_LOG(MAIN_INFO, "Beginning running of tests...");
    while (_l_pstd_itt != _l_pstd_end && _l_erc.should_exit() == false)
    {
        // Get the current element in the list.
        const post_setup_test_data_t& _l_test{(*_l_pstd_itt++).get()};
        const size_t _l_order_ran_id{ _l_order_ran_id_counter++ };
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
        // Check we actually have the resourses... should we lock it here before
        // doing this?
        if (_l_erc.should_exit() == false && _m_threads_free.size() > 0)
        {
            // Otherwise, get a lock for the threads, remove the
            // resousrses...
            unique_lock  _l_thread_lock(_m_threads_mutex);
            const size_t _l_thread_idx{*_m_threads_free.begin()};
            _m_threads_free.erase(_l_thread_idx);
            _m_current_thread_pool -= _l_test.thread_resourses_required();
            _l_thread_lock.unlock();
            // Then run the thread
            _LIBRARY_LOG(MAIN_INFO, "Starting new thread to run test.");
            _m_threads[_l_thread_idx] = jthread(
                &test_main_t::run_individual_test,
                this,
                _l_test,
                _l_thread_idx,
                std::ref(_m_test_runners[_l_thread_idx]),
                _l_order_ran_id
                // std::ref(_m_test_set_data[_l_thread_idx])
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
    _LIBRARY_LOG(MAIN_INFO, "Finalising reports.");
    finalised_test_set_data_t _l_final_report;
    for (auto& _l_test_runner : _m_test_runners)
    {
        _l_final_report.process_final_report(_l_test_runner.test_set_data());
    }
    _l_trc.finalise_reports(_l_final_report);
}

__no_constexpr_imp void
    test_main_t::run_individual_test(
        const _ABC_NS_DS::post_setup_test_data_t& _a_prtd,
        const size_t                              _a_thread_idx,
        test_runner_t&                            _a_test_runner,
        const std::size_t _a_order_ran_id
        // _ABC_NS_DS::test_set_data_t&              _a_test_set_data
    )
{
    using namespace std;
    using namespace _ABC_NS_ERRORS;
    using namespace _ABC_NS_GLOBAL;
    using namespace _ABC_NS_REPORTERS;
    using enum _ABC_NS_UTILITY::internal::internal_log_enum_t;
    // Get the thread runner
    set_this_threads_test_runner(&_a_test_runner);
    test_runner_t& _l_threads_runner{get_this_threads_test_runner_ref()};
    // run in try
    try
    {
        _LIBRARY_LOG(MAIN_INFO, "Running test.");
        _l_threads_runner.run_test(_a_prtd, _a_order_ran_id);
    }
    // Catch if its a library error.
    catch (test_library_exception_t& _l_the)
    {
        _LIBRARY_LOG(MAIN_INFO, "Exception encountered when running test.");
        error_reporter_controller_t& _l_erc{
            get_global_error_reporter_controller()
        };
        _l_erc.report_error(
            setup_error_t(_l_the.error(), true, _l_the.stacktrace())
        );
    }
    catch (...)
    {
        _LIBRARY_LOG(MAIN_INFO, "Unknown exception caught when running test.");
        error_reporter_controller_t& _l_erc{
            get_global_error_reporter_controller()
        };
        _l_erc.report_error(
            setup_error_t("Unknown exception thrown from test_runner_t.", true)
        );
    }
    _LIBRARY_LOG(MAIN_INFO, "Test finished.");
    _LIBRARY_LOG(MAIN_INFO, "Freeing thread resourses.");
    unique_lock _l_thread_lock(_m_threads_mutex);
    _m_threads_free.insert(_a_thread_idx);
    unique_lock _l_thread_pool_lock(_m_thread_pool_mutex);
    _m_current_thread_pool += _a_prtd.thread_resourses_required();
    _m_cv.notify_one();
    _l_threads_runner.set_data_process_test();
    return;
}

namespace
{
__constexpr_imp _ABC_NS_DS::test_lists_t
                make_test_list_collection(
                    const std::vector<std::shared_ptr<_ABC_NS_DS::test_list_t>>&
                               _a_ptrs_of_test_list,
                    const bool _a_use_global_test_list
                ) noexcept
{
    using namespace _ABC_NS_DS;
    using namespace _ABC_NS_GLOBAL;
    using namespace std;
    test_lists_t _l_rv{};
    if (_a_use_global_test_list)
    {
        _l_rv.push_back(std::ref(get_global_test_list()));
    }
    for (const shared_ptr<test_list_t>& _l_ptr : _a_ptrs_of_test_list)
    {
        _l_rv.push_back(std::ref(*_l_ptr));
    }
    return _l_rv;
}

template <typename T>
__constexpr_imp std::vector<std::reference_wrapper<const T>>
                make_ref_collection(
                    std::vector<std::shared_ptr<T>> _a_ptrs
                ) noexcept
{
    using namespace std;
    std::vector<std::reference_wrapper<const T>> _l_rv{};
    for (const shared_ptr<T>& _l_ptr : _a_ptrs)
    {
        _l_rv.push_back(std::ref(*_l_ptr));
    }
    return _l_rv;
}

template <typename T>
__constexpr_imp std::set<T>
                set_from_min_to_n(
                    const T _a_max
                ) noexcept
{
    using namespace std;
    set<T> _l_set{};
    for (T _l_idx{numeric_limits<T>::min()}; _l_idx < _a_max; ++_l_idx)
    {
        _l_set.insert(_l_idx);
    }
    return _l_set;
}
} // namespace

_END_ABC_NS
