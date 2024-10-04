#pragma once
#include "abc_test/core/test_reports/mid_test_invokation_report/generic_matcher_based_assertion.h"

_BEGIN_ABC_REPORTS_NS
template<
	typename Assertion_Status
>
	requires std::derived_from<Assertion_Status, dynamic_status_t>
struct matcher_based_assertion_t
	: public generic_matcher_based_assertion_t<true, Assertion_Status>
{
public:
	using generic_matcher_based_assertion_t<true, Assertion_Status>::generic_matcher_based_assertion_t;
};
_END_ABC_REPORTS_NS