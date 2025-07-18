#pragma once

#include "abc_test/core/matchers/matcher_result.hpp"
#include "abc_test/core/test_reports/assertion.hpp"
#include "abc_test/core/test_reports/assertion_status/static_status.hpp"
_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Concrete class used to create static assertions in test code.
 * @tparam Assertion_Status The assertion status type parameter. Must be derived
 * from static_status_t.
 */
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
struct static_assertion_t
    : public assertion_t<true, Assertion_Status>
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
        const std::optional<std::string>&                _a_annotation,
        const std::size_t _a_index
    ) noexcept;
    __constexpr
    static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos,
        const std::size_t _a_index
    ) noexcept;
    __constexpr const std::optional<std::string>&
                annotation() const noexcept;
private:
    std::optional<std::string> _m_static_assertion_annotation;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
__constexpr_imp
    static_assertion_t<Assertion_Status>::static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos,
        const std::optional<std::string>&                _a_annotation,
        const std::size_t _a_index
    ) noexcept
    : assertion_t<true, Assertion_Status>(
          std::monostate{},
          _a_source,
          _a_log_infos,
          std::monostate{},
        _a_index
      )
    , _m_static_assertion_annotation(_a_annotation)
{}

template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
__constexpr_imp
    static_assertion_t<Assertion_Status>::static_assertion_t(
        const user_initialised_report_t<true>::source_t& _a_source,
        const ds::log_infos_t&                           _a_log_infos,
        const std::size_t _a_index
    ) noexcept
    : assertion_t<true, Assertion_Status>(
          std::monostate{},
          _a_source,
          _a_log_infos,
          std::monostate{},
        _a_index
      )
{}

template <typename Assertion_Status>
requires std::derived_from<Assertion_Status, static_status_t>
__constexpr_imp const std::optional<std::string>&
    static_assertion_t<Assertion_Status>::annotation() const noexcept
{
    return _m_static_assertion_annotation;
}

_END_ABC_REPORTS_NS
