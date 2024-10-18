#pragma once

#include "abc_test/core/reporters/test_reporter.h"

_BEGIN_ABC_REPORTERS_NS

/*!
 * @brief Structure used to hold a set of test_reporter_t objects. This allows
 * the testing suite to report information about tests to several different
 * places, decided by the user.
 */
struct test_reporter_controller_t
{
public:
    /*!
     * @brief Constructor.
     */
    __no_constexpr
        test_reporter_controller_t() noexcept;
    /*!
     * @brief Function to call to get all the test_reporters to process the
     * test_set_data_t final report.
     * @param _a_test_set_data The test_set_data_t object to report.
     * @return
     */
    __no_constexpr void
        finalise_reports(ds::test_set_data_t& _a_test_set_data) noexcept;
    /*!
     * @brief Function to call when reporting a test's results.
     * @param _a_aetr the invoked_test_data_t test to report.
     */
    __constexpr void
        report_test(const ds::invoked_test_data_t& _a_aetr) noexcept;
    /*!
     * @brief Adds a set of test_reporters_t to the object.
     * @param _a_new_test_reporters The test_reporters_t entites to add to the
     * object.
     */
    __constexpr void
        add_reporters(const test_reporters_t& _a_new_test_reporters) noexcept;
private:
    test_reporters_t _m_reporters;
    std::mutex       _m_reporters_mutex;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__no_constexpr_imp
    test_reporter_controller_t::test_reporter_controller_t() noexcept
{}

__no_constexpr_imp void
    test_reporter_controller_t::finalise_reports(
        ds::test_set_data_t& _a_test_set_data
    ) noexcept
{
    using namespace std;
    for (reference_wrapper<const test_reporter_t> _l_reporter : _m_reporters)
    {
        _l_reporter.get().final_report(_a_test_set_data);
    }
}

__constexpr_imp void
    test_reporter_controller_t::report_test(
        const ds::invoked_test_data_t& _a_itd
    ) noexcept
{
    using namespace std;
    unique_lock _l_report_test_unique_lokc(_m_reporters_mutex);
    for (reference_wrapper<const test_reporter_t> _l_reporter : _m_reporters)
    {
        _l_reporter.get().report_test(_a_itd);
    }
}

/*__no_constexpr_imp
    void
    test_reporter_controller_t::report_test(
        const ds::invoked_test_info_t& _a_iti,
        const errors::test_failures_info_t& _a_error_info
    ) noexcept
{
    std::unique_lock _l_report_test_unique_lokc(_m_reporters_mutex);
    for (utility::shared_and_raw_ptr<test_reporter_t>& _l_reporter :
_m_reporters)
    {
        test_reporter_t* _l_reporter_ptr{ utility::get_ptr(_l_reporter) };
        _l_reporter_ptr->report_test(_a_iti, _a_error_info, _m_test_options);
    }
}*/
__constexpr_imp void
    test_reporter_controller_t::add_reporters(
        const test_reporters_t& _a_new_test_reporters
    ) noexcept
{
    _m_reporters.insert(
        _m_reporters.end(),
        _a_new_test_reporters.begin(),
        _a_new_test_reporters.end()
    );
}

_END_ABC_REPORTERS_NS