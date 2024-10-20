#pragma once

#include "abc_test/internal/test_reports/assertion.h"
#include "abc_test/internal/test_reports/assertion_status/dynamic_status.h"
#include "abc_test/internal/matchers/matcher_result.h"
#include "abc_test/internal/matchers/source_map.h"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Abstract class describing a matcher-based assertion.
 * @tparam Assertion_Status The status of the assertion.
 * @tparam Single_Source Bool if the assertion is from a single source. false
 * otherwise.
 */
template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
struct matcher_based_assertion_t
    : public assertion_t<Single_Source, Assertion_Status>
{
public:
    /*!
     * @brief Returns the assertion's matcher_result_t to the caller.
     * @return The assertion's matcher_result_t.
     */
    __constexpr const _ABC_NS_MATCHER::matcher_result_t&
        matcher_result() const noexcept;
    /*!
     * @brief Returns the assertion's matcher_source_map_t to the caller.
     * @return The assertion's matcher_source_map_t.
     */
    __constexpr const _ABC_NS_MATCHER::matcher_source_map_t&
        source_map() const noexcept;
    /*!
     * @brief Returns the matcher's annotation to the caller.
     * @return A cref to the matcher's annotation.
     */
    __constexpr const std::optional<std::string>&
                      annotation() const noexcept;
protected:
    _ABC_NS_MATCHER::matcher_result_t           _m_matcher_result;
    _ABC_NS_MATCHER::matcher_source_map_t       _m_matcher_map;
    std::optional<std::string> _m_annotation;
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
    matcher_based_assertion_t(
        typename const user_initialised_report_t<Single_Source>::source_t&
                                               _a_source,
        const ds::log_infos_t&                 _a_log_infos,
        const std::optional<std::string_view>& _a_test_annotation,
        const _ABC_NS_MATCHER::matcher_result_t&                _a_matcher_result,
        const _ABC_NS_MATCHER::matcher_source_map_t&            _a_matcher_source_map,
        const std::optional<std::string_view>& _a_annotation
    ) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp const _ABC_NS_MATCHER::matcher_result_t&
    matcher_based_assertion_t<Single_Source, Assertion_Status>::matcher_result(
    ) const noexcept
{
    return _m_matcher_result;
}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp const _ABC_NS_MATCHER::matcher_source_map_t&
    matcher_based_assertion_t<Single_Source, Assertion_Status>::source_map(
    ) const noexcept
{
    return _m_matcher_map;
}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp const std::optional<std::string>&
    matcher_based_assertion_t<Single_Source, Assertion_Status>::annotation(
    ) const noexcept
{
    return _m_annotation;
}
template <bool Single_Source, typename Assertion_Status>
    requires std::derived_from<Assertion_Status, dynamic_status_t>
__constexpr_imp
matcher_based_assertion_t<Single_Source, Assertion_Status>::
matcher_based_assertion_t(
    typename const user_initialised_report_t<Single_Source>::source_t&
    _a_source,
    const ds::log_infos_t& _a_log_infos,
    const std::optional<std::string_view>& _a_test_annotation,
    const _ABC_NS_MATCHER::matcher_result_t& _a_matcher_result,
    const _ABC_NS_MATCHER::matcher_source_map_t& _a_matcher_source_map,
    const std::optional<std::string_view>& _a_annotation
) noexcept
    : assertion_t<Single_Source, Assertion_Status>(
        Assertion_Status(_a_matcher_result.passed()),
        _a_source,
        _a_log_infos,
        _a_test_annotation
    )
    , _m_matcher_result(_a_matcher_result)
    , _m_matcher_map(_a_matcher_source_map)
    , _m_annotation(_a_annotation)
{}
_END_ABC_REPORTS_NS
