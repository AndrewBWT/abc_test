#pragma once
#include "abc_test/internal/test_reports/assertion.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class used to describe matcher-based assertion blocks.
 * @tparam Assertion_Status The status of the assertion block - must be derived
 * from dynamic_status_t.
 */
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
struct matcher_based_assertion_block_t
    : public assertion_t<false, Assertion_Status>
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
        const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_matcher_info,
        const opt_description_t<false>&              _a_opt_description
    ) noexcept;
    __constexpr
        const _ABC_NS_MATCHER::bba_inner_assertion_type_t&
        get_matcher() const noexcept;
private:
    _ABC_NS_MATCHER::bba_inner_assertion_type_t _m_matcher_info;
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
            const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_matcher_info,
            const opt_description_t<false>&              _a_opt_description
        ) noexcept
    : assertion_t<false, Assertion_Status>(
          _a_status,
          _a_source,
          _a_log_infos,
          _a_opt_description
      )
    , _m_matcher_info(_a_matcher_info)
{}
template <typename Assertion_Status>
    requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
const _ABC_NS_MATCHER::bba_inner_assertion_type_t&
matcher_based_assertion_block_t<Assertion_Status>::get_matcher() const noexcept
{
    return _m_matcher_info;
}

_END_ABC_REPORTS_NS