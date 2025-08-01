#pragma once
#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_DS_NS
// pass and no termination -> ok
// pass and terminate early -> ok
// pasa and unexpected termination -> fail
// fail and no termination -> fail
// fail and terminate early -> fail
// fail and unexpected termination -> fail

/*!
 * @brief Enum type which describes a test's status.
 */
enum class enum_test_status_t
{
    /*!
     * @brief The "pass" status; no errors have occoured, and the test is
     * successful.
     */
    NO_TERMINATION_TEST_PASSED,
    /*!
     * @brief The test has failed, however it has not been forced to terminate -
     * either it is still currently running, or it terminated normally.
     */
    NO_TERMINATION_TEST_FAILED,
    /*!
     * @brief The test has failed due to an assertion which instructed the
     * overarching test to terminate.
     */
    TERMINATION_OCCOURED_TEST_FAILED,
    /*!
     * @brief The test has failed due to an uncaught exception being thrown.
     */
    TERMINATION_OCCOURED_UNEXPECTED_THROW,
    /*!
    * @brief the test has failed due to a abc_test_exception_t being thrown.
    */
    termination_occoured_abc_text_exception_thrown
};
/*!
 * @brief Test which returns a bool signifying if the enum_test_status_t
 * argument is considered a passing value.
 * @param _a_test_status The enum to test
 * @return True if _a_test_status is considered a pass; false if a failure.
 */
__constexpr bool
    test_passed(const enum_test_status_t _a_test_status,
        const std::size_t _a_assertions_recieved);
/*!
 * @brief Function which returns a bool signifying if the enum_test_status_t
 * argument signifies a termination.
 * @param _a_test_status The enum_test_status_t to test.
 * @return True if _a_test_status is considered a termination; false if not.
 */
__constexpr bool
    terminated(const enum_test_status_t _a_test_status);
_END_ABC_DS_NS

/*!
 * formatter for post_setup_test_ata object.
 */
template <>
struct fmt::formatter<_ABC_NS_DS::enum_test_status_t> : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    __no_constexpr auto
        format(_ABC_NS_DS::enum_test_status_t _a_iti, format_context& _a_cxt)
            const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp bool
    test_passed(
        const enum_test_status_t _a_test_status,
        const std::size_t _a_assertions_recieved
    )
{
    using enum enum_test_status_t;
    switch (_a_test_status)
    {
    case NO_TERMINATION_TEST_PASSED:
        return (_a_assertions_recieved > 0);
    case NO_TERMINATION_TEST_FAILED:
    case TERMINATION_OCCOURED_TEST_FAILED:
    case TERMINATION_OCCOURED_UNEXPECTED_THROW:
    case termination_occoured_abc_text_exception_thrown:
        return false;
    default:
        throw errors::unaccounted_for_enum_exception(_a_test_status);
    }
}

__constexpr_imp bool
    terminated(
        const enum_test_status_t _a_test_status
    )
{
    using enum enum_test_status_t;
    switch (_a_test_status)
    {
    case NO_TERMINATION_TEST_PASSED:
    case NO_TERMINATION_TEST_FAILED:
        return false;
    case TERMINATION_OCCOURED_TEST_FAILED:
    case TERMINATION_OCCOURED_UNEXPECTED_THROW:
    case termination_occoured_abc_text_exception_thrown:
        return true;
    default:
        throw errors::unaccounted_for_enum_exception(_a_test_status);
    }
}

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::enum_test_status_t>::format(
        _ABC_NS_DS::enum_test_status_t _a_iti,
        format_context&                _a_ctx
    ) const -> format_context::iterator
{
    using enum _ABC_NS_DS::enum_test_status_t;
    using namespace std;
    string _l_rv{};
    switch (_a_iti)
    {
    case NO_TERMINATION_TEST_PASSED:
        _l_rv = "NO_TERMINATION_TEST_PASSED";
        break;
    case NO_TERMINATION_TEST_FAILED:
        _l_rv = "NO_TERMINATION_TEST_FAILED";
        break;
    case TERMINATION_OCCOURED_TEST_FAILED:
        _l_rv = "TERMINATION_OCCOURED_TEST_FAILED";
        break;
    case TERMINATION_OCCOURED_UNEXPECTED_THROW:
        _l_rv = "TERMINATION_OCCOURED_UNEXPECTED_THROW";
        break;
    case termination_occoured_abc_text_exception_thrown:
        _l_rv = "termination_occoured_abc_text_exception_thrown";
        break;
    default:
        throw _ABC_NS_ERRORS::unaccounted_for_enum_exception(_a_iti);
    }
    return formatter<string_view>::format(_l_rv, _a_ctx);
}