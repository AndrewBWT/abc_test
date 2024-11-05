#pragma once

#include "abc_test/internal/ds/source/source_pair.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"

#include <concepts>
#include <type_traits>

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Base-class designed to model reports from test functions. This class
 * specifically models unexpected reports.
 *
 * The class cannot be insantiated due to not having any public constructors.
 *
 * @tparam Terminate Bool determining whether the testing frameowork should
 * terminate this function when it processes this object.
 */
template <bool Terminate>
class unexpected_report_t
{
public:
    /*!
     * @brief Virtual destructor.
     *
     * Defined so that collections of unexpected_report_t elemnets can be
     * created.
     *
     */
    __constexpr virtual ~unexpected_report_t() noexcept;
    /*!
     * @brief Returns the source of the object to the caller.
     * @return A cref to the single_source_t component of the object.
     */
    __constexpr const ds::single_source_t&
                      source() const noexcept;
    /*!
     * @brief Bool denoting whether this report has an exact location for its
     * source.
     *
     * For example, if there was a function thrown an exception, then test suite
     * could not know where the test was thrown from. Instead, the most recent
     * source logged with the framework is used as a guide. This bool denotes
     * whether this source is exact, or an estimation.
     *
     * @return True if the source is exact, false otherwise.
     */
    __constexpr bool
        exact_source() const noexcept;
protected:
    __constexpr
    unexpected_report_t() noexcept
        = delete;
    /*!
     * @brief Constructor
     * @param _a_source The object's source.
     * @param _a_exact_source Whether its an exact source.
     * @return 
     */
    __constexpr
    unexpected_report_t(
        const ds::single_source_t& _a_source,
        const bool                 _a_exact_source
    ) noexcept;
    /*!
     * @brief Constructor
     * @param _a_sources If the source is a source pair, this is used.
     * @param _a_exact_source 
     * @return 
     */
    __constexpr
    unexpected_report_t(
        const ds::source_pair_t& _a_sources,
        const bool               _a_exact_source
    ) noexcept;
protected:
    ds::single_source_t _m_last_source;
    bool                _m_exact_source;
};
/*!
 * @brief Type synonym for a pointer to an unexepcted report which terminates.
 */
using opt_unexpected_report_t
    = std::unique_ptr<const unexpected_report_t<true>>;
/*!
 * @brief Type synonym to a pointer to an unexepcted report which does not terminate.
 */
using unexpected_non_terminating_report_ptr_t
    = std::unique_ptr<const unexpected_report_t<false>>;
/*!
 * @brief Type synonym to a collection of non-terminating test reports.
 */
using unexpected_non_terminating_report_collection_t
    = std::vector<unexpected_non_terminating_report_ptr_t>;
_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
template <bool Terminate>
__constexpr_imp
    unexpected_report_t<Terminate>::unexpected_report_t(
        const ds::single_source_t& _a_source,
        const bool                 _a_exact_source
    ) noexcept
    : _m_last_source(_a_source), _m_exact_source(_a_exact_source)
{}

template <bool Terminate>
__constexpr_imp
    unexpected_report_t<Terminate>::unexpected_report_t(
        const ds::source_pair_t& _a_sources,
        const bool               _a_exact_source
    ) noexcept
    : _m_last_source(_a_sources.begin_source())
    , _m_exact_source(_a_exact_source)
{}

template <bool Terminate>
__constexpr_imp unexpected_report_t<Terminate>::~unexpected_report_t() noexcept
{}

template <bool Terminate>
__constexpr_imp const ds::single_source_t&
                      unexpected_report_t<Terminate>::source() const noexcept
{
    return _m_last_source;
}

template <bool Terminate>
__constexpr_imp bool
    unexpected_report_t<Terminate>::exact_source() const noexcept
{
    return _m_exact_source;
}

_END_ABC_REPORTS_NS