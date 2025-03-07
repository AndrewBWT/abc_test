#pragma once

#include "abc_test/core/test_reports/assertion_status/assertion_status_base.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief An abstract class used to represent a dynamic test status; in that it
 * is not known at compile time whether this will pass or fail.
 *
 * Through the use of a protected constructor (and no default constructor),
 * instances of this class cannot be made.
 */
struct dynamic_status_t : assertion_status_base_t
{
    /*!
     * @brief Tells the caller whether this status passed or failed.
     * @return The pass or failure status of this object.
     */
    __constexpr bool
        pass() const noexcept;
protected:
    __constexpr
    dynamic_status_t() noexcept
        = delete;
    /*!
     * @brief Constructor.
     * @param _a_pass The pass value used.
     */
    __constexpr
    dynamic_status_t(const bool _a_pass) noexcept;
    /*!
     * @brief Internal pass value. This denotes whether the element passes or
     * fails. It is used in both of this class's derived classes.
     */
    bool _m_pass;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp bool
    dynamic_status_t::pass() const noexcept
{
    return _m_pass;
}

__constexpr_imp
    dynamic_status_t::dynamic_status_t(
        const bool _a_pass
    ) noexcept
    : _m_pass(_a_pass)
{}

_END_ABC_REPORTS_NS