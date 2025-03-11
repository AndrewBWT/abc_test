#pragma once
#include "abc_test/core/ds/source/single_source.hpp"
#include "abc_test/core/ds/test_data/test_set_data.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/core/reporters/test_reporter_controller.hpp"
#include "abc_test/core/test_order_enum.hpp"
#include "abc_test/core/test_reports/assertion.hpp"
#include "abc_test/core/test_reports/basic_text_warning.hpp"

#include <memory>


_BEGIN_ABC_LOGGING_NS
struct log_msg_t;
_END_ABC_LOGGING_NS
_BEGIN_ABC_DS_NS
struct registered_test_data_t;
_END_ABC_DS_NS
_BEGIN_ABC_NS

// Forward declarations

/*!
 * @brief Object which serves as the entry point for both running tests, and
 * storing test data. It is this structure which is accessed from within tests
 * when doing things such as logging messages, or writing assertion data to the
 * correct data structures.
 */
struct test_runner_t
{
public:
    __constexpr
    test_runner_t()
        = delete;
    /*!
     * @brief Constructor.
     * @param _a_trc The test_reporter_controller_t uased with this
     * test_runner_t.
     * @param _a_test_options The test_options_base_t used with this object.
     */
    __no_constexpr
        test_runner_t(
            _ABC_NS_REPORTERS::test_reporter_controller_t& _a_trc,
            const test_options_base_t&                     _a_test_options,
            const utility::rng& _a_rng
        ) noexcept;
    /*!
     * @brief Adds a log message to the current set of log messages.
     *
     * The _a_log reference is not owned by this class. The log_msg_t destructur
     * will ensure that this reference is deleted when the object is deleted.
     *
     * @param _a_log A reference to the log_msg_t.
     * @param _a_source The source of the log message.
     * @return An iterator to where the reference to this log message is stored.
     */
    __no_constexpr _ABC_NS_LOGGING::log_msg_itt_t
                   add_log_msg(
                       _ABC_NS_LOGGING::log_msg_ref_t     _a_log,
                       const _ABC_NS_DS::single_source_t& _a_source
                   ) noexcept;
    /*!
     * @brief Removes the log message from the test_runner_t's internal
     * structure.
     * @param _a_itt An iterator to where the log message reference is stored.
     */
    __no_constexpr_or_inline void
        remove_log_msg(_ABC_NS_LOGGING::log_msg_itt_t _a_itt) noexcept;
    /*!
     * @brief Logs a warning with the current test.
     * @param _a_str The string representing the warning.
     */
    __constexpr void
        add_text_warning(const std::string_view _a_str) noexcept;
    /*!
     * @brief Adds a text-based warning to the internal set of warnings.
     * @param _a_sr The string text which describes the warning.
     * @param _a_source The exact source associated with the warning.
     * @return
     */
    __constexpr void
        add_test_warning_with_source(
            const std::string_view             _a_sr,
            const _ABC_NS_DS::single_source_t& _a_source
        ) noexcept;
    /*!
     * @brief Adds a log message to the object's cached log messages.
     *
     * This is used when an unexpected exception is raised in some testing code.
     * Log messages will be deleted. By caching these, we can provide logging
     * information to the user.
     *
     * @param _a_str The string to log.
     */
    __constexpr void
        add_cached_log_msg(const std::string_view _a_str) noexcept;

    /*!
     * @brief Gets the set of log messages for the caller.
     * @param _a_get_cached_results Whether to include cached results in the
     * returned set of log messages.
     * @return A set of log messages.
     */
    __no_constexpr_or_inline _ABC_NS_DS::log_infos_t
        get_log_infos(const bool _a_get_cached_results) noexcept;
    /*!
     * @brief Function which runs a test.
     *
     * The core logic concerning the running of tests is contained in this
     * function.
     *
     * @param _a_post_setup_test_data The test to run.
     */
    __no_constexpr_or_inline void
        run_test(
            const _ABC_NS_DS::post_setup_test_data_t& _a_post_setup_test_data,
            const std::size_t                         _a_order_ran_id
        );
    /*!
     * @brief Tells the caller how many tests this test_runner_t has run.
     * @return The number of tests this test_runner_t has ran.
     */
    __constexpr const ds::test_set_data_t&
                      test_set_data() const noexcept;
    /*!
     * @brief Returns the current test to the caller.
     * @return The current invoked_test_data_t object being ran.
     */
    __no_constexpr _ABC_NS_DS::invoked_test_data_t&
                   current_test() noexcept;
    /*!
     * @brief Returns to the caller a random seed.
     * @return A random seed generated from this test_runner_t's random
     * generator.
     */
    __no_constexpr _ABC_NS_UTILITY::rng
        generate_random_seeds(const std::size_t _a_order_ran_id) noexcept;
    /*!
     * @brief Returns to the caller this test_runner_t's most recent source.
     * @return The object's most recent source.
     */
    __constexpr const std::optional<_ABC_NS_DS::single_source_t>&
                      most_recent_source() const noexcept;
    /*!
     * @brief Adds an assertion to the underlying test_runner_t.
     * @tparam Single_Source The type of source which the assertion is raised
     * from.
     * @tparam Assertion_Status The status of the assertion.
     * @param _a_ptr A pointer to the assertion.
     */
    template <bool Single_Source, typename Assertion_Status>
    __constexpr void
        add_assertion(
            _ABC_NS_REPORTS::assertion_ptr_t<Single_Source, Assertion_Status>&
                _a_ptr
        ) noexcept;
    /*!
     * @brief Adds an assertion and a text warning to the underlying
     * test_runner_t.
     * @tparam Single_Source The type of source which the assertion is raised
     * from.
     * @tparam Assertion_Status The status of the assertion.
     * @param _a_ptr A pointer to the assertion.
     * @param _a_warning The text of the given warning. This warning assumes
     * that the source is the assertion itself.
     */
    template <bool Single_Source, typename Assertion_Status>
    __constexpr void
        add_assertion_and_warning(
            _ABC_NS_REPORTS::assertion_ptr_t<Single_Source, Assertion_Status>&
                                   _a_ptr,
            const std::string_view _a_warning
        ) noexcept;
    __no_constexpr void
        set_data_process_test() noexcept;
private:
    _ABC_NS_LOGGING::log_msg_ptrs_t _m_current_log_msgs;
    std::vector<std::string>        _m_cached_log_msgs;
    std::reference_wrapper<_ABC_NS_REPORTERS::test_reporter_controller_t>
        _m_trc;
    // std::size_t                                      _m_tests_ran;
    std::shared_ptr<_ABC_NS_DS::invoked_test_data_t> _m_current_test;
    _ABC_NS_UTILITY::rng                             _m_random_generator;
    // const test_options_base_t&                       _m_test_options;
    _ABC_NS_DS::test_set_data_t _m_test_data;
    std::optional<_ABC_NS_DS::single_source_t> _m_tests_most_recent_source;
    template <bool Single_Source, typename Assertion_Status>
    __constexpr void
        add_assertion_and_optional_warning(
            _ABC_NS_REPORTS::assertion_ptr_t<Single_Source, Assertion_Status>&
                                                   _a_ptr,
            const std::optional<std::string_view>& _a_optional_warning
        ) noexcept;
    __constexpr void
        register_tests_most_recent_source(
            const _ABC_NS_DS::single_source_t& _a_source_location
        ) noexcept;
    __constexpr void
        register_tests_most_recent_source(
            const _ABC_NS_DS::source_pair_t& _a_source_locations
        ) noexcept;
};

_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp
    test_runner_t::test_runner_t(
        _ABC_NS_REPORTERS::test_reporter_controller_t& _a_trc,
        const test_options_base_t&                     _a_test_options,
        const utility::rng& _a_rng
    ) noexcept
    : _m_trc(_a_trc)
    , _m_current_test(nullptr)
    , _m_random_generator(_a_rng)
    // , _m_test_options(_a_test_options)
    , _m_tests_most_recent_source(_ABC_NS_DS::single_source_t())
{}

__constexpr_imp void
    test_runner_t::register_tests_most_recent_source(
        const _ABC_NS_DS::single_source_t& _a_source_location
    ) noexcept
{
    _m_tests_most_recent_source = _a_source_location;
}

__constexpr_imp void
    test_runner_t::register_tests_most_recent_source(
        const _ABC_NS_DS::source_pair_t& _a_source_locations
    ) noexcept
{
    using namespace _ABC_NS_DS;
    using namespace std;
    const optional<single_source_t>& _l_end_source{
        _a_source_locations.end_source()
    };
    _m_tests_most_recent_source = _l_end_source.has_value()
                                      ? _l_end_source.value()
                                      : _a_source_locations.begin_source();
}

__no_constexpr_imp _ABC_NS_LOGGING::log_msg_itt_t
                   test_runner_t::add_log_msg(
        _ABC_NS_LOGGING::log_msg_ref_t     _a_log,
        const _ABC_NS_DS::single_source_t& _a_source
    ) noexcept
{
    using namespace std;
    register_tests_most_recent_source(_a_source);
    _m_current_log_msgs.push_back(_a_log);
    return --_m_current_log_msgs.end();
}

__constexpr_imp const ds::test_set_data_t&
                      test_runner_t::test_set_data() const noexcept
{
    return _m_test_data;
}

__no_constexpr_imp _ABC_NS_DS::invoked_test_data_t&
                   test_runner_t::current_test() noexcept
{
    return *_m_current_test;
}

__no_constexpr_imp _ABC_NS_UTILITY::rng
                   test_runner_t::generate_random_seeds(
        const std::size_t _a_order_ran_id
    ) noexcept
{
    using namespace _ABC_NS_UTILITY;
    const size_t _l_n_elements_used_to_seed_random_generators{
        global::get_global_test_options()
            .number_of_integers_used_to_seed_random_generators
    };
    _m_random_generator.progress(
        _a_order_ran_id * _l_n_elements_used_to_seed_random_generators
    );
    return _m_random_generator.make_rng(_l_n_elements_used_to_seed_random_generators);
}

__constexpr_imp const std::optional<_ABC_NS_DS::single_source_t>&
                      test_runner_t::most_recent_source() const noexcept
{
    return _m_tests_most_recent_source;
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    test_runner_t::add_assertion(
        _ABC_NS_REPORTS::assertion_ptr_t<Single_Source, Assertion_Status>&
            _a_ptr
    ) noexcept
{
    using namespace std;
    add_assertion_and_optional_warning(_a_ptr, optional<string_view>{});
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    test_runner_t::add_assertion_and_warning(
        _ABC_NS_REPORTS::assertion_ptr_t<Single_Source, Assertion_Status>&
                               _a_ptr,
        const std::string_view _a_warning
    ) noexcept
{
    using namespace std;
    add_assertion_and_optional_warning(
        _a_ptr, optional<string_view>{_a_warning}
    );
}

__no_constexpr_imp void
    test_runner_t::set_data_process_test() noexcept
{
    _m_test_data.process_invoked_test(*_m_current_test);
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    test_runner_t::add_assertion_and_optional_warning(
        _ABC_NS_REPORTS::assertion_ptr_t<Single_Source, Assertion_Status>&
                                               _a_ptr,
        const std::optional<std::string_view>& _a_optional_warning
    ) noexcept
{
    using namespace _ABC_NS_REPORTS;
    using namespace std;
    register_tests_most_recent_source(_a_ptr->source());
    if (_a_optional_warning.has_value())
    {
        add_test_warning_with_source(
            _a_optional_warning.value(), _a_ptr->last_source()
        );
    }
    _m_current_test->add_current_for_loop_stack_to_trie();
    _m_current_test->add_assertion(_a_ptr);
}

__constexpr_imp void
    test_runner_t::add_text_warning(
        const std::string_view _a_str
    ) noexcept
{
    using namespace _ABC_NS_REPORTS;
    using namespace std;
    _m_current_test->add_warning(
        make_unique<basic_text_warning_t>(most_recent_source(), false, _a_str)
    );
}

__constexpr_imp void
    test_runner_t::add_test_warning_with_source(
        const std::string_view             _a_str,
        const _ABC_NS_DS::single_source_t& _a_source
    ) noexcept
{
    using namespace _ABC_NS_REPORTS;
    using namespace std;
    _m_current_test->add_warning(
        make_unique<basic_text_warning_t>(_a_source, true, _a_str)
    );
}

__constexpr_imp void
    test_runner_t::add_cached_log_msg(
        const std::string_view _a_str
    ) noexcept
{
    using namespace std;
    _m_cached_log_msgs.push_back(string(_a_str));
}

_END_ABC_NS