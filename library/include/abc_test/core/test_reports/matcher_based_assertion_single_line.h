#pragma once
#include "abc_test/core/test_reports/matcher_based_assertion.h"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Class used to model matcher_based_assertions on a single line.
 * @tparam Assertion_Status The status of the assertion. Must be derived from
 * dynamic_status_t.
 */
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
struct matcher_based_assertion_single_line_t
    : public matcher_based_assertion_t<true, Assertion_Status>
{
public:
    /*!
     * @brief Constructor.
     * @param _a_source The assertion's source.
     * @param _a_log_infos The assertion's log info.
     * @param _a_matcher_result The matcher's result.
     * @param _a_matcher_source_map The matcher's source map.
     * @param _a_annotation The matcher's annotation.
     * 
     * Note that the test annotation is not set by this function.
     */
    __constexpr
    matcher_based_assertion_single_line_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos,
        const matcher_result_t&                          _a_matcher_result,
        const matcher_source_map_t&                      _a_matcher_source_map,
        const std::optional<std::string_view>&           _a_annotation
    ) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
    matcher_based_assertion_single_line_t<Assertion_Status>::
        matcher_based_assertion_single_line_t(
            const user_initialised_report_t<true>::source_t& _a_source,
            const ds::log_infos_t&                           _a_log_infos,
            const matcher_result_t&                          _a_matcher_result,
            const matcher_source_map_t&            _a_matcher_source_map,
            const std::optional<std::string_view>& _a_annotation
        ) noexcept
    : matcher_based_assertion_t<true, Assertion_Status>(
          _a_source,
          _a_log_infos,
          std::optional<std::string>{},
          _a_matcher_result,
          _a_matcher_source_map,
          _a_annotation
      )
{}

_END_ABC_REPORTS_NS