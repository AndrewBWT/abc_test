#pragma once
#include "abc_test/included_instances/reporters/text_test_reporter/enum_fields/assertion.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp"
_BEGIN_ABC_REPORTERS_NS
enum class enum_matcher_based_assertion_single_line_fields_t
{
    MATCHER_SOURCE_MAP,
    MATCHER_OUTPUT,
    MATCHER_ANNOTATION
};
using combined_enum_matcher_based_assertion_single_line_fields_t = std::variant<
    enum_matcher_based_assertion_single_line_fields_t,
    combined_enum_assertion_fields_t>;
__constexpr enum_print_pair_collection_t<
    combined_enum_matcher_based_assertion_single_line_fields_t>
    default_matcher_based_assertion_fields() noexcept;
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__constexpr_imp enum_print_pair_collection_t<
    combined_enum_matcher_based_assertion_single_line_fields_t>
    default_matcher_based_assertion_fields() noexcept
{
    using enum enum_assertion_fields_t;
    using enum enum_user_initialised_report_fields_t;
    using enum reporters::enum_print_data_t;
    using enum enum_matcher_based_assertion_single_line_fields_t;
    return {
        {MATCHER_OUTPUT,     PRINT           },
        {MATCHER_ANNOTATION, CONTEXTUAL_PRINT},
        {STR_REPRESENTATION, DONT_PRINT      },
        {TEST_DESCRIPTION,   CONTEXTUAL_PRINT},
        {SOURCE,             CONTEXTUAL_PRINT},
        {LOG_INFOS,          CONTEXTUAL_PRINT},
        {STATUS,             DONT_PRINT      },
        {MATCHER_SOURCE_MAP, CONTEXTUAL_PRINT},
    };
}

_END_ABC_REPORTERS_NS