#pragma once
#include "abc_test/internal/ds/test_data/invoked_test_data.h"
#include "abc_test/internal/global.h"
#include "abc_test/internal/ds/data_generator_memoization/map_unique_id_to_tdg_collection_stack_trie.h"

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure containing data from running a set of tests.
 *
 */
struct test_set_data_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __no_constexpr
    test_set_data_t()
        = default;
    /*!
     * @brief This function processes a single invoked_test_data_t object,
     * updating its internal variabels based on how the test ran.
     * @param _a_invoked_test The invoked_test_data_t entity used to update this
     * object.
     */
    __no_constexpr void
        process_invoked_test(ds::invoked_test_data_t& _a_invoked_test) noexcept;
private:
    abc::ds::map_unique_id_to_tdg_collection_stack_trie_t
        _m_map_ids_to_tdg_collection_stack_tries;
    /*!
     * @brief The total number of tests ran.
     */
    std::size_t _m_total_tests_ran{0};
    /*!
     * @brief The total number of tests which ran and passed - no exceptions
     * thrown.
     */
    std::size_t _m_total_tests_passed{0};
    /*!
     * @brief Total number of tests which ran and failed - including those whose
     * invokation was terminated due to an uncaught exception.
     *
     * _m_total_tests_ran = _m_total_tests_passed +
     * _m_total_tests_failed_and_terminated;
     */
    std::size_t _m_total_tests_failed{0};
    /*!
     * @brief Total number of tests which ran and failed an assertion, and the
     * assertion terminated the function.
     */
    std::size_t _m_total_tests_failed_and_terminated{0};
    /*!
     * @brief Total number of tests which were ran and failed, but did not
     * terminate due to any sort of exception, or an assertion which caused the
     * function terminate early.
     */
    std::size_t _m_total_tests_failed_but_not_terminated{0};
    /*!
     * @brief Total number of tests which were ran but failed due to an uncaught
     * exception.
     */
    std::size_t _m_total_tests_designated_failure_due_to_exception{0};
    std::size_t _m_total_assertions_ran{0};
    std::size_t _m_total_assertions_passed{0};
    std::size_t _m_total_assertions_failed{0};
    friend class finalised_test_set_data_t;
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS

__no_constexpr_imp void
    test_set_data_t::process_invoked_test(
        ds::invoked_test_data_t& _a_invoked_test
    ) noexcept
{
    using enum enum_test_status_t;
    _m_map_ids_to_tdg_collection_stack_tries.insert(
        _a_invoked_test.post_setup_test_data().discovery_id(),
        _a_invoked_test.for_loop_stack_trie()
    );
    const enum_test_status_t _l_test_status{_a_invoked_test.test_status()};
    const bool               _l_test_passed{test_passed(_l_test_status)};
    _m_total_tests_ran    += 1;
    _m_total_tests_passed += _l_test_passed ? 1 : 0;
    _m_total_tests_failed += (not _l_test_passed) ? 1 : 0;
    _m_total_tests_failed_and_terminated
        += (_l_test_status == TERMINATION_OCCOURED_TEST_FAILED) ? 1 : 0;
    _m_total_tests_failed_but_not_terminated
        += (_l_test_status == NO_TERMINATION_TEST_FAILED) ? 1 : 0;
    _m_total_tests_designated_failure_due_to_exception
        += (_l_test_status == TERMINATION_OCCOURED_UNEXPECTED_THROW) ? 1 : 0;
    _m_total_assertions_ran    += _a_invoked_test.assertions_recieved();
    _m_total_assertions_passed += _a_invoked_test.assertions_passed();
    _m_total_assertions_failed += _a_invoked_test.assertions_failed();
}
_END_ABC_DS_NS