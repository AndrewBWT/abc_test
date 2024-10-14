#pragma once

#include "abc_test/core/test_reports/unexpected_report.h"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Class used to model entities thrown from a test, which are not derived
 * from std::exception.
 */
class unexpected_thrown_not_derived_from_std_exception_t
    : public unexpected_report_t<true>
{
public:
    /*!
     * @brief Constructor
     * @param _a_source The closest known source of the exception.
     * @return 
     */
    __constexpr
    unexpected_thrown_not_derived_from_std_exception_t(
        const ds::single_source_t& _a_source
    ) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
    unexpected_thrown_not_derived_from_std_exception_t::
        unexpected_thrown_not_derived_from_std_exception_t(
            const ds::single_source_t& _a_source
        ) noexcept
    : unexpected_report_t<true>(_a_source, false)
{}

_END_ABC_REPORTS_NS