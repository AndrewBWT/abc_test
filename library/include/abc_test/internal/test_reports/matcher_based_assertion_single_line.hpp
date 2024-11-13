#pragma once
#include "abc_test/internal/test_reports/matcher_based_assertion.hpp"

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
        const typename matcher_based_assertion_t<true, Assertion_Status>::
            matcher_based_assertion_matchers_t&      _a_matcher_and_annotation
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
            const typename matcher_based_assertion_t<true, Assertion_Status>::
                matcher_based_assertion_matchers_t& _a_matcher_and_annotation
        ) noexcept

    : matcher_based_assertion_t<true, Assertion_Status>(
          get<0>(_a_matcher_and_annotation).passed(),
          _a_source,
          _a_log_infos,
          _a_matcher_and_annotation,
          std::monostate{}
      )
{}

_END_ABC_REPORTS_NS