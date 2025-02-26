#pragma once

#include "abc_test/internal/ds/test_data/test_set_data.hpp"
#include "abc_test/internal/ds/test_data/pre_test_run_report.hpp"
#include "abc_test/internal/ds/test_data/finalised_test_set_data.hpp"

_BEGIN_ABC_REPORTERS_NS

/*!
 * @brief An abstract object used when creating objects to process test results.
 */
struct test_reporter_t
{
public:
    /*!
     * @brief Abstract function used to process a test after it has finished running.
     * @param _a_itd The test to process.
     */
    __constexpr virtual void
        report_test(
            const ds::invoked_test_data_t& _a_itd
        ) const
        = 0;
    /*!
     * @brief Abstract function used to process the final report.
     *
     * @param _a_test_set_data The report concerning how all the tests have
     * performed.
     */
    __constexpr virtual void
        final_report(ds::finalised_test_set_data_t& _a_test_set_data) const noexcept
        = 0;
    __constexpr virtual void
        pre_test_run_report(ds::pre_test_run_report_t& _a_pre_test_run_report) const noexcept
        = 0;
};

/*!
 * @brief Type synonym for a collection of test_reporter_t objects; either raw
 * pointers or shared pointers.
 */
using test_reporters_t
    = std::vector<std::reference_wrapper<const test_reporter_t>>;
_END_ABC_REPORTERS_NS