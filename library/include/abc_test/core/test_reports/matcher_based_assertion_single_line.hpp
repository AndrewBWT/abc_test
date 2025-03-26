#pragma once
#include "abc_test/core/test_reports/assertion.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Class used to model matcher_based_assertions on a single line.
 * @tparam Assertion_Status The status of the assertion. Must be derived from
 * dynamic_status_t.
 */
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
struct matcher_based_assertion_single_line_t
    : public assertion_t<true, Assertion_Status>
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
        const user_initialised_report_t<true>::source_t&   _a_source,
        const ds::log_infos_t&                             _a_log_infos,
        const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_matcher_info,
        const std::size_t                                  _a_index
    ) noexcept;
    __constexpr const _ABC_NS_MATCHER::bba_inner_assertion_type_t&
                      matcher_info() const noexcept;
private:
    _ABC_NS_MATCHER::bba_inner_assertion_type_t _m_matcher_info;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
    matcher_based_assertion_single_line_t<Assertion_Status>::
        matcher_based_assertion_single_line_t(
            const user_initialised_report_t<true>::source_t&   _a_source,
            const ds::log_infos_t&                             _a_log_infos,
            const _ABC_NS_MATCHER::bba_inner_assertion_type_t& _a_matcher_info,
            const std::size_t                                  _a_index
        ) noexcept

    : assertion_t<true, Assertion_Status>(
          _a_matcher_info.matcher_result().passed(),
          _a_source,
          _a_log_infos,
          std::monostate{},
          _a_index
      )
    , _m_matcher_info(_a_matcher_info)
{}

template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp const _ABC_NS_MATCHER::bba_inner_assertion_type_t&
    matcher_based_assertion_single_line_t<Assertion_Status>::matcher_info(
    ) const noexcept
{
    return _m_matcher_info;
}

_END_ABC_REPORTS_NS