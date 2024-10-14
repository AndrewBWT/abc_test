#pragma once
#include "abc_test/core/ds/source/single_source.h"
#include "abc_test/core/reporters/test_reporter_controller.h"
#include "abc_test/core/test_order_enum.h"
#include "abc_test/core/test_reports/basic_text_warning.h"
#include "abc_test/core/test_reports/assertion.h"

#include <memory>


_BEGIN_ABC_NS
struct log_test_msg_t;
// Forward declarations
struct ds::registered_test_data_t;

/*!
 * Object which controls the running of tests.
 */
struct test_runner_t
{
public:
    /*!
     * Main constructor used; initialises the test_reporter controller and
     * global test_options
     */
    __no_constexpr
        test_runner_t(
            reporters::test_reporter_controller_t& _a_trc,
            const test_options_t&                  _a_test_options
        ) noexcept;
    /*!
     * Registers a source with the object, used when showing information
     * pertaining to errors to the user.
     */
    __constexpr void
        register_tests_most_recent_source(
            const ds::single_source_t& _a_source_location
        ) noexcept;
    __constexpr void
        register_tests_most_recent_source(
            const ds::source_pair_t& _a_source_locations
        ) noexcept;
    __no_constexpr std::list<const log_test_msg_t*>::iterator
                   add_error_info(const log_test_msg_t* _a_log) noexcept;
    /*!
     * Removes an element from the current log list.
     */
    __no_constexpr_or_inline void
        remove_error_info(std::list<const log_test_msg_t*>::iterator _a_itt
        ) noexcept;

    constexpr void
        add_cached_exception(
            const std::string_view _a_str
        ) noexcept
    {
        using namespace std;
        _m_cached_log_msgs.push_back(string(_a_str));
    }

    /*!
     * Gets text representation of all the logs.
     */
    __no_constexpr_or_inline std::vector<std::string>
        get_log_infos(const bool _a_get_cached_results) noexcept;
    // Can't be constexpr as body must be in cpp file
    /*!
     * Runs a test.
     *
     * This is the 'hook' from the 'other side'; that is to say, this can be
     * considered the "main" when testing a function.
     */
    __no_constexpr_or_inline void
        run_test(const ds::post_setup_test_data_t& _a_post_setup_test_data);
    /*!
     * Tells the user how many tests have been ran by this test_runner.
     *
     * Remember that, due to multi-threading, this is only this instance of
     * test_runner.
     */
    __constexpr std::size_t
                tests_ran() const noexcept;
    /*!
     * Returns to the user the current invoked test for this object instance.
     */
    __no_constexpr ds::invoked_test_data_t&
                   current_test() noexcept;
    /*!
     * Generates a random seed.
     */
    __constexpr       utility::seed_t
                      generate_random_seeds() noexcept;
    __constexpr const ds::single_source_t&
                      most_recent_source() const noexcept;
    template <bool Single_Source, typename Assertion_Status>
    __constexpr void
        add_assertion(
            reports::assertion_ptr_t<Single_Source, Assertion_Status>&
                _a_ptr
        ) noexcept;
    template <bool Single_Source, typename Assertion_Status>
    __constexpr void
        add_assertion_and_warning(
            reports::assertion_ptr_t<Single_Source, Assertion_Status>&
                                   _a_ptr,
            const std::string_view _a_warning
        ) noexcept;

    /*__constexpr
        void
        register_source(
            const std::source_location& _a_sl,
            const std::string_view _a_str_representation,
            const std::string_view _a_inner_str_representation
        ) noexcept;
    __constexpr
        std::optional<std::string>
        get_registered_source(
            const std::source_location& _a_sl
        ) noexcept;
    __constexpr
        bool
        record_sources(
        ) const noexcept;*/
private:
    std::list<const log_test_msg_t*> _m_current_error_log_msgs;
    std::vector<std::string>         _m_cached_log_msgs;
    // reporters::after_execution_test_report_t _m_after_execution_test_report;
    // errors::test_failures_info_t _m_error_infos;
    reporters::test_reporter_controller_t*   _m_trc;
    std::size_t                              _m_tests_ran;
    std::unique_ptr<ds::invoked_test_data_t> _m_current_test;
    utility::rng                             _m_random_generator;
    test_order_enum_t                        _m_test_order;
    const test_options_t&                    _m_test_options;
    ds::single_source_t                      _m_tests_most_recent_source;
    //	std::optional<reports::single_source_t> _m_registered_source;
    template <bool Single_Source, typename Assertion_Status>
    __constexpr void
        add_assertion_and_optional_warning(
            reports::assertion_ptr_t<Single_Source, Assertion_Status>&
                                                   _a_ptr,
            const std::optional<std::string_view>& _a_optional_warning
        ) noexcept;
};

_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp
    test_runner_t::test_runner_t(
        reporters::test_reporter_controller_t& _a_trc,
        const test_options_t&                  _a_test_options
    ) noexcept
    //: _m_after_execution_test_report(reporters::after_execution_test_report_t{})
    : _m_trc(&_a_trc)
    , _m_tests_ran{0}
    , _m_current_test(nullptr)
    , _m_random_generator(utility::rng(_a_test_options._m_seed_values))
    , _m_test_order(test_order_enum_t::IN_ORDER)
    , _m_test_options(_a_test_options)
    , _m_tests_most_recent_source(ds::single_source_t())
{}

__constexpr_imp void
    test_runner_t::register_tests_most_recent_source(
        const ds::single_source_t& _a_source_location
    ) noexcept
{
    _m_tests_most_recent_source = _a_source_location;
}

__constexpr_imp void
    test_runner_t::register_tests_most_recent_source(
        const ds::source_pair_t& _a_source_locations
    ) noexcept
{
    using namespace ds;
    using namespace std;
    const optional<single_source_t>& _l_end_source{
        _a_source_locations.end_source()
    };
    _m_tests_most_recent_source = _l_end_source.has_value()
                                      ? _l_end_source.value()
                                      : _a_source_locations.begin_source();
}

__no_constexpr_imp std::list<const log_test_msg_t*>::iterator
                   test_runner_t::add_error_info(
        const log_test_msg_t* _a_log
    ) noexcept
{
    using namespace std;
    _m_current_error_log_msgs.push_back(_a_log);
    return --_m_current_error_log_msgs.end();
}

__constexpr_imp size_t
    test_runner_t::tests_ran() const noexcept
{
    return _m_tests_ran;
}

__no_constexpr_imp ds::invoked_test_data_t&
                   test_runner_t::current_test() noexcept
{
    return *_m_current_test;
}

__constexpr_imp utility::seed_t
                test_runner_t::generate_random_seeds() noexcept
{
    using namespace utility;
    const size_t _l_n_elements_used_to_seed_random_generators{
        _m_test_options._m_numb_elements_used_to_seed_random_generators
    };
    seed_t _l_seed(_l_n_elements_used_to_seed_random_generators);
    for (size_t _l_idx{0};
         _l_idx < _l_n_elements_used_to_seed_random_generators;
         ++_l_idx)
    {
        _l_seed[_l_idx] = _m_random_generator();
    }
    return _l_seed;
}

__constexpr_imp const ds::single_source_t&
                      test_runner_t::most_recent_source() const noexcept
{
    return _m_tests_most_recent_source;
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    test_runner_t::add_assertion(
        reports::assertion_ptr_t<Single_Source, Assertion_Status>&
            _a_ptr
    ) noexcept
{
    using namespace std;
    add_assertion_and_optional_warning(_a_ptr, optional<string_view>{});
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    test_runner_t::add_assertion_and_warning(
        reports::assertion_ptr_t<Single_Source, Assertion_Status>&
                               _a_ptr,
        const std::string_view _a_warning
    ) noexcept
{
    using namespace std;
    add_assertion_and_optional_warning(
        _a_ptr, optional<string_view>{_a_warning}
    );
}

/*__constexpr_imp
void
test_runner_t::register_source(
    const std::source_location& _a_sl,
    const std::string_view _a_str_representation,
    const std::string_view _a_inner_str_representation
) noexcept
{
    using namespace reports;
    using namespace std;
    register_tests_most_recent_source(single_source_t(_a_str_representation,
_a_sl)); if (_m_registered_source.has_value())
    {
        //std::exit(-1999);
    }
    //else
    {
        _m_registered_source =
optional<single_source_t>(single_source_t(_a_inner_str_representation, _a_sl));
    }
}
__constexpr_imp
std::optional<std::string>
test_runner_t::get_registered_source(
    const std::source_location& _a_sl
) noexcept
{
    using namespace std;
    using namespace reports;
    if (_m_registered_source.has_value())
    {
        if
(utility::equal_file_name_function_name_and_line(_m_registered_source.value().source_location(),_a_sl))
        {
            string _l_str{ _m_registered_source.value().str() };
            //_m_registered_source = optional<single_source_t>();
            return _l_str;
        }
    }
    return optional<string>{};
}
__constexpr_imp
bool
test_runner_t::record_sources(
) const noexcept
{
    return _m_record_successes;
}*/
template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    test_runner_t::add_assertion_and_optional_warning(
        reports::assertion_ptr_t<Single_Source, Assertion_Status>&
                                               _a_ptr,
        const std::optional<std::string_view>& _a_optional_warning
    ) noexcept
{
    using namespace reports;
    using namespace std;
    // reports::unexpected_non_terminating_report_ptr_t& _a_warning
    register_tests_most_recent_source(_a_ptr->source());
    if (_a_optional_warning.has_value())
    {
        _m_current_test->add_warning(
            make_unique<const unexpected_report_t<false>>(basic_text_warning_t(
                _a_ptr->last_source(), true, _a_optional_warning.value()
            ))
        );
    }
    _m_current_test->add_current_for_loop_stack_to_trie();
    _m_current_test->add_assertion(_a_ptr);
}

_END_ABC_NS