#pragma once
#include "abc_test/core/errors/test_library_setup_error.h"

_BEGIN_ABC_REPORTERS_NS

/*!
 * @brief Abstract object which processes errors taken as the test framework is
 * running.
 */
struct error_reporter_t
{
    /*!
     * @brief Function to manage a setup_error_t being reported.
     * @param _a_error The error being reported.
     */
    __constexpr virtual void
        report_error(const errors::setup_error_t& _a_error) const noexcept
        = 0;
    /*!
     * @brief Function to manage reporting information. This may be in the form
     * of a warning, or just something to log to the user.
     * @param _a_str The string to be reported.
     */
    __constexpr virtual void
        report_information(const std::string_view _a_str) const noexcept
        = 0;
    /*!
     * @brief The function to be called if termination occours. This function is
     * meant to manage any freeing of resourses.
     */
    __constexpr virtual void
        exit() const noexcept
        = 0;
};
/*!
 * @brief Type synonym for a collection of either shared or raw pointers of type
 * error_reproter_t.
 */
using error_reporters_t
= std::vector<std::reference_wrapper<const error_reporter_t>>;
_END_ABC_REPORTERS_NS