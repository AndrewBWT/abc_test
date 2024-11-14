#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/matcher_based_assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp"
_BEGIN_ABC_REPORTERS_NS
__constexpr enum_print_pair_collection_t<combined_enum_matcher_based_assertion_fields_t>
            default_static_fields() noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp enum_print_pair_collection_t<combined_enum_matcher_based_assertion_fields_t>
                default_static_fields() noexcept
{
    using enum enum_assertion_fields_t;
    using enum enum_matcher_based_assertion_fields_t;
    using enum enum_user_initialised_report_fields_t;
    using enum enum_matcher_based_assertion_single_line_fields_t;
    using enum reporters::enum_print_data_t;
    return {
        {STR_REPRESENTATION, CONTEXTUAL_PRINT},
        {TEST_DESCRIPTION,   CONTEXTUAL_PRINT},
        {SOURCE,             CONTEXTUAL_PRINT},
        {LOG_INFOS,          CONTEXTUAL_PRINT},
        {STATUS,             DONT_PRINT      },
    };
}

_END_ABC_REPORTERS_NS