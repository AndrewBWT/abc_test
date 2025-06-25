#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Testing unicode_conversion function",
         .path = "tests::unicode_conversion"}
    )
)
{
    using namespace abc;
    _CHECK_EXPR(1 == 2);
    _REQUIRE_EXPR(2 == 2);

    matcher_t matcher_1 = _EXPR(1 == 2);
    _CHECK(matcher_1);
    _CHECK(_EXPR(1 == 2));
    _CHECK_EXPR(1 == 2);

    _CHECK(_EXPR(1 == 2) && _EXPR(2 == 3) || _EXPR(3 == 4));

    _BEGIN_MULTI_ELEMENT_BBA(
        _l_assertion_with_multiple_checks,
        "Contains many tests wrapped up in one assertion"
    );
    _l_assertion_with_multiple_checks += _BLOCK_CHECK(matcher_1);
    _l_assertion_with_multiple_checks
        += _BLOCK_CHECK(_EXPR(1 == 2) && _EXPR(2 == 3));
    _END_BBA_CHECK(_l_assertion_with_multiple_checks);
}