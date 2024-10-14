#pragma once

#include "abc_test/core/test_reports/assertion.h"
#include "abc_test/core/test_reports/assertion_status/static_status.h"
_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class used to create static assertions in test code.
 * @tparam Assertion_Status The assertion status type parameter. Must be derived
 * from static_status_t.
 */
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
struct static_assertion_t : public assertion_t<true, Assertion_Status>
{
public:
    /*!
     * @brief Constructor.
     * @param _a_source The source of the assertion.
     * @param _a_log_infos The ds::log_infos_t of the assertion.
     * @param _a_annotation The user-written annotation provided with the assertion.
     */
    __constexpr
    static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos,
        const std::optional<std::string_view>&           _a_annotation
    ) noexcept;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
__constexpr_imp
    static_assertion_t<Assertion_Status>::static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos,
        const std::optional<std::string_view>&           _a_annotation
    ) noexcept
    : assertion_t<true, Assertion_Status>(
          Assertion_Status(),
          _a_source,
          _a_log_infos,
          _a_annotation
      )
{}

_END_ABC_REPORTS_NS
