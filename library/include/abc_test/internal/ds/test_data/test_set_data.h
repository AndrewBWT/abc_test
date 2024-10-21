#pragma once
#include "abc_test/internal/ds/test_data/invoked_test_data.h"

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
    __constexpr
    test_set_data_t()
        = default;
    __constexpr
        void
        process_final_report(
            const test_set_data_t& _a_test_set_data
        ) noexcept;
    /*!
     * @brief This function processes a single invoked_test_data_t object,
     * updating its internal variabels based on how the test ran.
     * @param _a_invoked_test The invoked_test_data_t entity used to update this
     * object.
     */
    __constexpr void
        process_invoked_test(ds::invoked_test_data_t& _a_invoked_test) noexcept;
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__constexpr_imp
void
    test_set_data_t::process_final_report(
        const test_set_data_t& _a_test_set_data
    ) noexcept
{}

__constexpr_imp void
    test_set_data_t::process_invoked_test(
        ds::invoked_test_data_t& _a_invoked_test
    ) noexcept
{}

_END_ABC_DS_NS