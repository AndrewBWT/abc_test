#pragma once
#include "abc_test/core/test_reports/assertion_status/dynamic_status.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Structure used to represent an assertion which can either pass or
 * terminate.
 */
struct pass_or_terminate_t : public dynamic_status_t
{
public:
    __constexpr
    pass_or_terminate_t() noexcept
        = delete;
    /*!
     * @brief Constructor
     * @param _a_pass The boolean denoting whether this assertion passes or
     * terminates.
     */
    __constexpr
    pass_or_terminate_t(const bool _a_pass) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
    pass_or_terminate_t::pass_or_terminate_t(
        const bool _a_pass
    ) noexcept
    : dynamic_status_t(_a_pass)
{}

_END_ABC_REPORTS_NS