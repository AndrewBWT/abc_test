#pragma once
#include "abc_test/core/test_reports/matcher_based_assertion.h"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class used to describe matcher-based assertion blocks.
 * @tparam Assertion_Status The status of the assertion block - must be derived
 * from dynamic_status_t.
 */
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
struct matcher_based_assertion_block_t
    : public matcher_based_assertion_t<false, Assertion_Status>
{
public:
    using matcher_based_assertion_t<false, Assertion_Status>::
        matcher_based_assertion_t;
};

_END_ABC_REPORTS_NS