#pragma once
#include "abc_test/core/test_reports/assertion.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class used to describe matcher-based assertion blocks.
 * @tparam Assertion_Status The status of the assertion block - must be derived
 * from dynamic_status_t.
 */
    template <typename Assertion_Status>
    requires std::derived_from<Assertion_Status, assertion_status_base_t>
struct multi_element_assertion_block_t
    : public assertion_t<false, Assertion_Status>

{
public:
    __constexpr
        multi_element_assertion_block_t(
            const status_t<Assertion_Status>& _a_status,
            typename const user_initialised_report_t<false>::source_t& _a_source,
            const ds::log_infos_t& _a_log_infos,
            const _ABC_NS_MATCHER::matcher_res_infos_t& _a_matcher_info,
            const opt_description_t<false>& _a_opt_description
        ) noexcept;
    __constexpr
        const _ABC_NS_MATCHER::matcher_res_infos_t&
        get_matcher() const noexcept;
private:
    _ABC_NS_MATCHER::matcher_res_infos_t _m_matcher_info;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template <typename Assertion_Status>
    requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
multi_element_assertion_block_t<Assertion_Status>::
multi_element_assertion_block_t(
    const status_t<Assertion_Status>& _a_status,
    typename const user_initialised_report_t<false>::source_t&
    _a_source,
    const ds::log_infos_t& _a_log_infos,
    const _ABC_NS_MATCHER::matcher_res_infos_t& _a_matcher_info,
    const opt_description_t<false>& _a_opt_description
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
    requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
const _ABC_NS_MATCHER::matcher_res_infos_t&
multi_element_assertion_block_t<Assertion_Status>::get_matcher() const noexcept
{
    return _m_matcher_info;
}

_END_ABC_REPORTS_NS