#pragma once
#include "abc_test/internal/ds/test_data/test_set_data.h"
_BEGIN_ABC_DS_NS

/*!
 * @brief Structure containing data from running a set of tests.
 *
 */
struct finalised_test_set_data_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __no_constexpr
        finalised_test_set_data_t()
        = default;
    __no_constexpr void
        process_final_report(const test_set_data_t& _a_test_set_data) noexcept;
    __constexpr
        std::size_t
        total_tests_ran(
        ) const noexcept;
    __constexpr
        std::size_t
        total_tests_passed(
        ) const noexcept;
    __constexpr
        std::size_t
        total_tests_failed(
        ) const noexcept;
    __constexpr
        std::size_t
        total_tests_failed_and_terminated(
        ) const noexcept;
    __constexpr
        std::size_t
        total_tests_failed_but_not_terminated(
        ) const noexcept;
    __constexpr
        std::size_t
        total_tests_designated_failure_due_to_exception(
        ) const noexcept;
    __constexpr
        std::size_t
        total_assertions_ran(
        ) const noexcept;
    __constexpr
        std::size_t
        total_assertions_passed(
        ) const noexcept;
    __constexpr
        std::size_t
        total_assertions_failed(
        ) const noexcept;
    __constexpr
        const utility::complete_global_seed_t&
        seed() const noexcept;
    __constexpr
        const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t&
        map_ids_to_tdg_collection_stack_tries() const noexcept;
private:
    abc::ds::map_unique_id_to_tdg_collection_stack_trie_t
        _m_map_ids_to_tdg_collection_stack_tries;
    /*!
     * @brief The total number of tests ran.
     */
    std::size_t _m_total_tests_ran{ 0 };
    /*!
     * @brief The total number of tests which ran and passed - no exceptions
     * thrown.
     */
    std::size_t _m_total_tests_passed{ 0 };
    /*!
     * @brief Total number of tests which ran and failed - including those whose
     * invokation was terminated due to an uncaught exception.
     *
     * _m_total_tests_ran = _m_total_tests_passed +
     * _m_total_tests_failed_and_terminated;
     */
    std::size_t _m_total_tests_failed{ 0 };
    /*!
     * @brief Total number of tests which ran and failed an assertion, and the
     * assertion terminated the function.
     */
    std::size_t _m_total_tests_failed_and_terminated{ 0 };
    /*!
     * @brief Total number of tests which were ran and failed, but did not
     * terminate due to any sort of exception, or an assertion which caused the
     * function terminate early.
     */
    std::size_t _m_total_tests_failed_but_not_terminated{ 0 };
    /*!
     * @brief Total number of tests which were ran but failed due to an uncaught
     * exception.
     */
    std::size_t _m_total_tests_designated_failure_due_to_exception{ 0 };
    std::size_t _m_total_assertions_ran{ 0 };
    std::size_t _m_total_assertions_passed{ 0 };
    std::size_t _m_total_assertions_failed{ 0 };
    utility::complete_global_seed_t _m_seed{ global::get_global_seed() };
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp void
finalised_test_set_data_t::process_final_report(
    const test_set_data_t& _a_test_set_data
) noexcept
{
    using namespace std;
    for (const pair<key_t, tdg_collection_stack_trie_t>& _l_element :
        _a_test_set_data._m_map_ids_to_tdg_collection_stack_tries.map())
    {
        _m_map_ids_to_tdg_collection_stack_tries.insert(
            _l_element.first, _l_element.second
        );
    }
    _m_total_tests_ran += _a_test_set_data._m_total_tests_ran;
    _m_total_tests_passed += _a_test_set_data._m_total_tests_passed;
    _m_total_tests_failed += _a_test_set_data._m_total_tests_failed;
    _m_total_tests_failed_and_terminated
        += _a_test_set_data._m_total_tests_failed_and_terminated;
    _m_total_tests_failed_but_not_terminated
        += _a_test_set_data._m_total_tests_failed_but_not_terminated;
    _m_total_tests_designated_failure_due_to_exception
        += _a_test_set_data._m_total_tests_designated_failure_due_to_exception;
    _m_total_assertions_ran += _a_test_set_data._m_total_assertions_ran;
    _m_total_assertions_passed += _a_test_set_data._m_total_assertions_passed;
    _m_total_assertions_failed += _a_test_set_data._m_total_assertions_failed;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_tests_ran(
) const noexcept
{
    return _m_total_tests_ran;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_tests_passed(
) const noexcept
{
    return _m_total_tests_passed;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_tests_failed(
) const noexcept
{
    return _m_total_tests_failed;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_tests_failed_and_terminated(
) const noexcept
{
    return _m_total_tests_failed_and_terminated;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_tests_failed_but_not_terminated(
) const noexcept
{
    return _m_total_tests_failed_but_not_terminated;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_tests_designated_failure_due_to_exception(
) const noexcept
{
    return _m_total_tests_designated_failure_due_to_exception;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_assertions_ran(
) const noexcept
{
    return _m_total_assertions_ran;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_assertions_passed(
) const noexcept
{
    return _m_total_assertions_passed;
}
__constexpr_imp
std::size_t
finalised_test_set_data_t::total_assertions_failed(
) const noexcept
{
    return _m_total_assertions_failed;
}
__constexpr_imp
const utility::complete_global_seed_t&
finalised_test_set_data_t::seed() const noexcept
{
    return _m_seed;
}
__constexpr_imp
const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t&
finalised_test_set_data_t::map_ids_to_tdg_collection_stack_tries() const noexcept
{
    return _m_map_ids_to_tdg_collection_stack_tries;
}
_END_ABC_DS_NS