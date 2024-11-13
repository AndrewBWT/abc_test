#pragma once
#include "abc_test/internal/test_reports/matcher_based_assertion.hpp"

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
    /*!
     * @brief Constructor.
     * @param _a_source The source of the assertion.
     * @param _a_log_infos The log messages associated with the assertion.
     * @param _a_test_annotation The user-defined annotation associated with the
     * assertion.
     * @param _a_matcher_result The result from the assertion's matcher.
     * @param _a_matcher_source_map The source map from the assertion's matcher.
     * @param _a_annotation The user-defined annotation associated with the
     * assertion's matcher.
     *
     * To be clar, _a_test_annotation and _a_annotation are very different. One
     * is associated with the matcher, the other associated with the assertion
     * itself. In single-line assertions, only the matcher's annotation is used.
     * However in matcher-based assertion blocks, both are used.
     */
    __constexpr
    matcher_based_assertion_block_t(
        const status_t<Assertion_Status>&                          _a_status,
        typename const user_initialised_report_t<false>::source_t& _a_source,
        const ds::log_infos_t&                                     _a_log_infos,
        typename const matcher_based_assertion_t<false, Assertion_Status>::
            matcher_based_assertion_matchers_t& _a_matchers_and_annotations,
        const _ABC_NS_MATCHER::matcher_source_map_t& _a_matcher_source_map,
        const opt_description_t<false>&              _a_opt_description
    ) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
    matcher_based_assertion_block_t<Assertion_Status>::
        matcher_based_assertion_block_t(
            const status_t<Assertion_Status>& _a_status,
            typename const user_initialised_report_t<false>::source_t&
                                   _a_source,
            const ds::log_infos_t& _a_log_infos,
            typename const matcher_based_assertion_t<false, Assertion_Status>::
                matcher_based_assertion_matchers_t& _a_matchers_and_annotations,
            const _ABC_NS_MATCHER::matcher_source_map_t& _a_matcher_source_map,
            const opt_description_t<false>&              _a_opt_description
        ) noexcept
    : matcher_based_assertion_t<false, Assertion_Status>(
          _a_status,
          _a_source,
          _a_log_infos,
          _a_matchers_and_annotations,
          _a_matcher_source_map,
          _a_opt_description
      )
{}

_END_ABC_REPORTS_NS