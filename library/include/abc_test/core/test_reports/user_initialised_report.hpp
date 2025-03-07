#pragma once

#include "abc_test/core/ds/source/source_pair.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <type_traits>

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Abstract class, designed to model reports created from a test
 * function, declared by the user. The most obvious example is an assertion.
 * @tparam Single_Source If true the source value is a single_source_t,
 * otherwise its a pair_source_t.
 * 
 */
template <bool Single_Source>
class user_initialised_report_t
{
public:
    /*!
     * @brief Type synonym for the source_t type of the class.
     */
    using source_t = std::
        conditional_t<Single_Source, ds::single_source_t, ds::source_pair_t>;
    /*!
     * @brief Returns the source_t to the caller.
     * @return The object's source_t element.
     */
    __constexpr const source_t&
                      source() const noexcept;
    /*!
     * @brief Returns the ds::log_infos_t to the caller.
     * @return The object's ds::log_infos_t element.
     */
    __constexpr const ds::log_infos_t&
                      log_infos() const noexcept;
    /*!
     * @brief Returns the last source from this object to the caller.
     * @return The last registered source from this object's source_t element.
     */
    __constexpr const ds::single_source_t&
                      last_source() const noexcept;
protected:
    __constexpr
    user_initialised_report_t() noexcept
        = delete;
    /*!
     * @brief Protected constructor so that elements of this type can't be created.
     */
    __constexpr
    user_initialised_report_t(
        const source_t&        _a_source,
        const ds::log_infos_t& _a_log_infos
    ) noexcept;
    source_t        _m_source;
    ds::log_infos_t _m_log_infos;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS

template <bool Single_Source>
__constexpr_imp const user_initialised_report_t<Single_Source>::source_t&
    user_initialised_report_t<Single_Source>::source() const noexcept
{
    return _m_source;
}

template <bool Single_Source>
__constexpr_imp const ds::log_infos_t&
    user_initialised_report_t<Single_Source>::log_infos() const noexcept
{
    return _m_log_infos;
}

template <bool Single_Source>
__constexpr_imp const ds::single_source_t&
    user_initialised_report_t<Single_Source>::last_source() const noexcept
{
    if constexpr (Single_Source)
    {
        return _m_source;
    }
    else
    {
        return _m_source.last_source();
    }
}

template <bool Single_Source>
__constexpr_imp
    user_initialised_report_t<Single_Source>::user_initialised_report_t(
        const source_t&        _a_source,
        const ds::log_infos_t& _a_log_infos
    ) noexcept
    : _m_source(_a_source), _m_log_infos(_a_log_infos)
{}

_END_ABC_REPORTS_NS