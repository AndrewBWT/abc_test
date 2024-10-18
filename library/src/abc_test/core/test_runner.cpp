#include "abc_test/core/test_runner.h"

#include "abc_test/core/errors/test_assertion_exception.h"
#include "abc_test/core/errors/test_library_exception.h"
#include "abc_test/core/logging/log_msg.h"
#include "abc_test/core/test_reports//unexpected_thrown_exception.h"
#include "abc_test/utility/internal/log.h"

_BEGIN_ABC_NS
__no_constexpr_or_inline_imp void
    test_runner_t::run_test(
        const ds::post_setup_test_data_t& _a_post_setup_test_data
    )
{
    using namespace std;
    using namespace errors;
    using namespace utility;
    using namespace ds;
    using namespace reporters;
    using namespace reports;
    using enum utility::internal::internal_log_enum_t;
    _LIBRARY_LOG(
        TEST_INFO,
        fmt::format(
            "About to run the following test: {0}", _a_post_setup_test_data
        )
    );
    _m_current_test = make_unique<invoked_test_data_t>(
        generate_random_seeds(),
        _a_post_setup_test_data,
        _m_tests_ran,
        _m_test_options.root_path
    );
    invoked_test_data_t&          _l_current_test{*_m_current_test};
    const registered_test_data_t& _l_rtd{
        _l_current_test.post_setup_test_data().registered_test_data()
    };
    _m_tests_most_recent_source = _l_rtd._m_source;
    try
    {
        (_l_rtd._m_test_function)();
    }
    catch (const test_assertion_exception_t& _l_assertion)
    {}
    catch (const test_library_exception_t& _l_internal_logic_error)
    {
        throw _l_internal_logic_error;
    }
    catch (const exception& _l_exception)
    {
        _l_current_test.set_unexpected_termination(
            make_unique<const unexpected_report_t<true>>(
                unexpected_thrown_exception_t(
                    most_recent_source(),
                    typeid(_l_exception).name(),
                    _l_exception.what()
                )
            )
        );
    }
    catch (...)
    {
        _l_current_test.set_unexpected_termination(
            make_unique<const unexpected_report_t<true>>(
                unexpected_thrown_not_derived_from_std_exception_t(
                    most_recent_source()
                )
            )
        );
    }
    _m_trc.get().report_test(_l_current_test);
    _m_current_log_msgs.clear();
    ++_m_tests_ran;
}

__no_constexpr_or_inline_imp void
    test_runner_t::remove_log_msg(
        log_msg_itt_t _a_itt
    ) noexcept
{
    using namespace std;
    _m_current_log_msgs.erase(_a_itt);
}

__no_constexpr_or_inline_imp ds::log_infos_t
                             test_runner_t::get_log_infos(
        const bool _a_get_cached_results
    ) noexcept
{
    using namespace std;
    using namespace ds;
    log_infos_t _l_rv;
    if (_a_get_cached_results)
    {
        return _m_cached_log_msgs;
    }
    else
    {
        log_msg_itt_t         _l_end{_m_current_log_msgs.end()};
        vector<log_msg_itt_t> _l_to_deletes;
        for (log_msg_itt_t _l_itt{_m_current_log_msgs.begin()};
             _l_itt != _l_end;
             ++_l_itt)
        {
            const log_msg_t& _l_elem{(*_l_itt).get()};
            _l_rv.push_back(string(_l_elem.str()));
            if (_l_elem.delete_after_use())
            {
                _l_to_deletes.push_back(_l_itt);
            }
        }
        for (log_msg_itt_t _l_to_delete : _l_to_deletes)
        {
            _m_current_log_msgs.erase(_l_to_delete);
        }
    }
    return _l_rv;
}

_END_ABC_NS