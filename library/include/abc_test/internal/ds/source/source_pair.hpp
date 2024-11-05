#pragma once
#include "abc_test/internal/ds/source/single_source.hpp"

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure representing a pair of single_source_t objects.
 *
 * It is used to describe "blocks" of code, allowing the system to gain a notion
 * of a beginning and end.
 *
 * The end source is optional.
 */
struct source_pair_t
{
public:
    /*!
     * @brief Default constructor.
     *
     * Generally should not be used.
     */
    __constexpr
    source_pair_t()
        = default;
    /*!
     * @brief Constructor with a single_source_t.
     * @param _a_begin_source The beginning source.
     *
     * There is no end source set in this source_pair_t object.
     */
    __constexpr
    source_pair_t(const single_source_t& _a_begin_source) noexcept;
    /*!
     * @brief Constructor with a pair of single_source_t objects.
     * @param _a_begin_source The beginning source.
     * @param _a_end_source The end source.
     * @return
     */
    __constexpr
    source_pair_t(
        const single_source_t& _a_begin_source,
        const single_source_t& _a_end_source
    ) noexcept;
    /*!
     * @brief Returns the beginning source to the caller.
     * @return The beginning source.
     */
    __constexpr const single_source_t&
        begin_source() const noexcept;
    /*!
     * @brief Returns the (optional) end source to the caller.
     * @return The optional<single_source_t> representing the end source.
     */
    __constexpr const std::optional<single_source_t>&
                      end_source() const noexcept;
    /*!
     * @brief Gets the last source from a source_pair_t object.
     * @return If there is an end source, returns it, otherwise returns the
     * beginning source.
     */
    __constexpr const single_source_t&
        last_source() const noexcept;
    /*!
     * @brief Sets the end source in a soruce_pair_t object.
     * @param _a_end_source The end source to set.
     *
     * Note this can override previously set end sources.
     */
    __constexpr void
        set_end_source(const single_source_t& _a_end_source) noexcept;
private:
    single_source_t                _m_begin_source;
    std::optional<single_source_t> _m_end_source;
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__constexpr_imp
    source_pair_t::source_pair_t(
        const single_source_t& _a_begin_source
    ) noexcept
    : _m_begin_source(_a_begin_source)
    , _m_end_source(std::optional<single_source_t>())
{}

__constexpr_imp
    source_pair_t::source_pair_t(
        const single_source_t& _a_begin_source,
        const single_source_t& _a_end_source
    ) noexcept
    : _m_begin_source(_a_begin_source)
    , _m_end_source(std::optional<single_source_t>(_a_end_source))
{}

const __constexpr_imp single_source_t&
    source_pair_t::begin_source() const noexcept
{
    return _m_begin_source;
}

const __constexpr_imp std::optional<single_source_t>&
                      source_pair_t::end_source() const noexcept
{
    return _m_end_source;
}

const __constexpr_imp single_source_t&
    source_pair_t::last_source() const noexcept
{
    if (_m_end_source.has_value())
    {
        return _m_end_source.value();
    }
    else
    {
        return _m_begin_source;
    }
}

__constexpr void
    source_pair_t::set_end_source(
        const single_source_t& _a_end_source
    ) noexcept
{
    _m_end_source = _a_end_source;
}

_END_ABC_DS_NS