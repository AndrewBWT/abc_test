#pragma once
#include "abc_test/core/ds/test_data/invoked_test_data.h"

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure containing data from running a set of tests.
 *
 */
struct test_set_data_t
{
public:
    /*!
     * @brief Constructor.
     *
     * This constructor is combines a set of test_set_data_t objects. Internally
     * it is used to combine test_set_data_t objects from diferent threads.
     *
     * @param _a_test_set_datas The set of test_set_data_t objects to create
     * this object from.
     */
    __constexpr
    test_set_data_t(const std::vector<test_set_data_t>& _a_test_set_datas
    ) noexcept;
    /*!
     * @brief This function processes a single invoked_test_data_t object,
     * updating its internal variabels based on how the test ran.
     * @param _a_invoked_test The invoked_test_data_t entity used to update this object.
     */
    __constexpr void
        process_invoked_test(ds::invoked_test_data_t& _a_invoked_test) noexcept;
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__constexpr_imp
    test_set_data_t::test_set_data_t(
        const std::vector<test_set_data_t>& _a_test_set_datas
    ) noexcept
{}

__constexpr_imp void
    test_set_data_t::process_invoked_test(
        ds::invoked_test_data_t& _a_invoked_test
    ) noexcept
{}

_END_ABC_DS_NS