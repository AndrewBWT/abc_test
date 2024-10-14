#pragma once

#include "abc_test/core/test_reports/assertion_base.h"
#include "abc_test/core/test_reports/assertion_status/assertion_status_base.h"
#include "abc_test/core/test_reports/assertion_status/fail.h"
#include "abc_test/core/test_reports/assertion_status/pass.h"
#include "abc_test/core/test_reports/assertion_status/pass_or_fail.h"
#include "abc_test/core/test_reports/assertion_status/pass_or_terminate.h"
#include "abc_test/core/test_reports/assertion_status/terminate.h"
#include "abc_test/core/test_reports/user_initialised_report.h"

#include <concepts>
#include <optional>
#include <string>

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Abstract class describing a generic assertion type.
 * @tparam Assertion_Status The type parameter of the assertion type. Must be
 * derived from assertion_status_base_t.
 * @tparam Single_Source The bool denoting whether this assertion uses a single
 * source or a pair of sourses.
 */
template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
class assertion_t
    : public user_initialised_report_t<Single_Source>
    , public assertion_base_t
{
public:
    /*!
     * @brief Returns a cref to the assertion's description.
     * @return A cref to the assertion's description.
     */
    __constexpr const std::optional<std::string>&
                      test_description() const noexcept;
    /*!
     * @brief Returns the assertion's status to the caller.
     * @return The assertion's status.
     */
    __constexpr Assertion_Status
        status() const noexcept;
    /*!
     * @brief Tells the caller whether the assertion passed or failed.
     * @return True if the assertino passed, false otherwise.
     */
    __constexpr bool
        get_pass_status() const noexcept;
    /*!
     * @brief Tells the caller whether the assertion terminates the test
     * function.
     * @return True if the assertion terminates the test function, false
     * otherwise.
     */
    __constexpr bool
        terminated() const noexcept;
protected:
    __constexpr
    assertion_t() noexcept
        = delete;
    __constexpr
    assertion_t(
        const Assertion_Status&                                   _a_status,
        const user_initialised_report_t<Single_Source>::source_t& _a_source,
        const ds::log_infos_t&                                    _a_log_infos
    ) noexcept;
    __constexpr
    assertion_t(
        const Assertion_Status&                                   _a_status,
        const user_initialised_report_t<Single_Source>::source_t& _a_source,
        const ds::log_infos_t&                                    _a_log_infos,
        const std::string_view                                    _a_annotation
    ) noexcept;
    __constexpr
    assertion_t(
        const Assertion_Status&                                   _a_status,
        const user_initialised_report_t<Single_Source>::source_t& _a_source,
        const ds::log_infos_t&                                    _a_log_infos,
        const std::optional<std::string_view>& _a_test_annotation
    ) noexcept;
    Assertion_Status           _m_status;
    std::optional<std::string> _m_test_description;
    // std::optional<std::string> _m_fail_message;
    // std::optional<std::string> _m_pass_message;
private:
};

template <bool Single_Source, typename Assertion_Status>
using assertion_ptr_t = std::unique_ptr<
    const reports::assertion_t<Single_Source, Assertion_Status>>;

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS

/*template<
    bool Single_Source,
    typename Assertion_Status
>
    requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
const std::optional<std::string>&
generic_assertion_t<Single_Source, Assertion_Status>::fail_message(
) const noexcept
{
    return _m_fail_message;
}
template<
    bool Single_Source,
    typename Assertion_Status
>
    requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
const std::optional<std::string>&
generic_assertion_t<Single_Source, Assertion_Status>::pass_message(
) const noexcept
{
    return _m_pass_message;
}*/
template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp const std::optional<std::string>&
    assertion_t<Single_Source, Assertion_Status>::test_description(
    ) const noexcept
{
    return _m_test_description;
}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp Assertion_Status
    assertion_t<Single_Source, Assertion_Status>::status() const noexcept
{
    return _m_status;
}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp bool
    assertion_t<Single_Source, Assertion_Status>::get_pass_status(
    ) const noexcept
{
    using namespace std;
    if constexpr (same_as<Assertion_Status, pass_t>)
    {
        return true;
    }
    else if constexpr (same_as<Assertion_Status, fail_t>
                       || same_as<Assertion_Status, terminate_t>)
    {
        return false;
    }
    else if constexpr (same_as<Assertion_Status, pass_or_fail_t>
                       || same_as<Assertion_Status, pass_or_terminate_t>)
    {
        return _m_status.pass();
    }
    else
    {
        __STATIC_ASSERT(Assertion_Status, "Cannot instantiate");
        // static_assert(false, "Cannot instantiate");
    }
}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp bool
    assertion_t<Single_Source, Assertion_Status>::terminated() const noexcept
{
    using namespace std;
    if constexpr (same_as<Assertion_Status, terminate_t>)
    {
        return true;
    }
    else if constexpr (same_as<Assertion_Status, pass_or_terminate_t>)
    {
        return not _m_status.pass();
    }
    else
    {
        return false;
    }
}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
    assertion_t<Single_Source, Assertion_Status>::assertion_t(
        const Assertion_Status&                                   _a_status,
        const user_initialised_report_t<Single_Source>::source_t& _a_source,
        const ds::log_infos_t&                                    _a_log_infos
    ) noexcept
    : assertion_t<Single_Source, Assertion_Status>(
          _a_status,
          _a_source,
          _a_log_infos,
          std::optional<std::string_view>{}
      )
{}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
    assertion_t<Single_Source, Assertion_Status>::assertion_t(
        const Assertion_Status&                                   _a_status,
        const user_initialised_report_t<Single_Source>::source_t& _a_source,
        const ds::log_infos_t&                                    _a_log_infos,
        const std::string_view _a_test_annotation
    ) noexcept
    : assertion_t<Single_Source, Assertion_Status>(
          _a_status,
          _a_source,
          _a_log_infos,
          std::optional<std::string_view>{_a_test_annotation}
      )
{}

template <bool Single_Source, typename Assertion_Status>
requires std::derived_from<Assertion_Status, assertion_status_base_t>
__constexpr_imp
    assertion_t<Single_Source, Assertion_Status>::assertion_t(
        const Assertion_Status&                                   _a_status,
        const user_initialised_report_t<Single_Source>::source_t& _a_source,
        const ds::log_infos_t&                                    _a_log_infos,
        const std::optional<std::string_view>& _a_test_annotation
    ) noexcept
    : user_initialised_report_t<Single_Source>(_a_source, _a_log_infos)
    , _m_status(_a_status)
    , _m_test_description(_a_test_annotation)
{}

_END_ABC_REPORTS_NS
