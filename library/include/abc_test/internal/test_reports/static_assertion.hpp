#pragma once

#include "abc_test/internal/matchers/matcher_result.hpp"
#include "abc_test/internal/test_reports/assertion.hpp"
#include "abc_test/internal/test_reports/assertion_status/static_status.hpp"
_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class used to create static assertions in test code.
 * @tparam Assertion_Status The assertion status type parameter. Must be derived
 * from static_status_t.
 */
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
struct static_assertion_t
    : public matcher_based_assertion_t<true, Assertion_Status>
{
public:
    /*!
     * @brief Constructor.
     * @param _a_source The source of the assertion.
     * @param _a_log_infos The ds::log_infos_t of the assertion.
     * @param _a_annotation The user-written annotation provided with the
     * assertion.
     */
    __constexpr
    static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos,
        const std::optional<std::string>&                               _a_annotation
    ) noexcept;
    __constexpr
    static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos
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
        const std::optional<std::string>&                               _a_annotation
    ) noexcept
    : matcher_based_assertion_t<true, Assertion_Status>(
          std::monostate{},
          _a_source,
          _a_log_infos,
          make_pair(_ABC_NS_MATCHER::matcher_result_t(), _a_annotation),
          _ABC_NS_MATCHER::matcher_source_map_t{},
        std::monostate{}
      )
{}

template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
__constexpr_imp
    static_assertion_t<Assertion_Status>::static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos
    ) noexcept
    : matcher_based_assertion_t<true, Assertion_Status>(
          std::monostate{},
          _a_source,
          _a_log_infos,
          make_pair(
              _ABC_NS_MATCHER::matcher_result_t(),
              std::optional<std::string>{}
          ),
          _ABC_NS_MATCHER::matcher_source_map_t{},
          std::monostate{}
      )
{}

_END_ABC_REPORTS_NS
