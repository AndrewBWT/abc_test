#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"

#include <fmt/std.h>
#include <source_location>
#include <string>

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure used to hold a source location together with a string
 * representation of some source code.
 */
class single_source_t
{
public:
    /*!
     * @brief Default constructor.
     *
     * Usually isn't used, but is required as it is used in some stl containers.
     *
     */
    __constexpr
    single_source_t() noexcept;
    /*!
     * @brief Constructor for single_source_t. This is the constructor which is
     * usually used.
     * @param _a_str String representation of the source code location.
     * @param _a_sl The source code location
     */
    __constexpr
    single_source_t(
        const std::string_view      _a_str,
        const std::source_location& _a_sl
    ) noexcept;
    /*!
     * @brief Returns the object's source code representation.
     * @return The objects source code representation.
     */
    const __constexpr std::string_view
                      source_code_representation() const noexcept;
    /*!
     * @brief Returns the object's source code location.
     * @return The object's source code location.
     */
    const __constexpr std::source_location&
                      source_location() const noexcept;
    /*!
     * @brief Equality operator for the single_source_t object.
     * @param _a_rhs The single_source_t object to comapre to.
     * @return True if this is equal to _a_rhs, false otherwise.
     */
    __constexpr bool
        operator==(const single_source_t& _a_rhs) const noexcept;
private:
    std::string          _m_str;
    std::source_location _m_sl;
};

_END_ABC_DS_NS

template <>
struct fmt::formatter<_ABC_NS_DS::single_source_t> : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can'tbe constexpr due to use of fmt::format
    __no_constexpr auto
        format(_ABC_NS_DS::single_source_t _a_rtd, format_context& _a_ctx) const
        -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp
    single_source_t::single_source_t() noexcept
{}

__constexpr_imp
    single_source_t::single_source_t(
        const std::string_view      _a_str,
        const std::source_location& _a_sl
    ) noexcept
    : _m_str(_a_str), _m_sl(_a_sl)
{}

const __constexpr_imp std::string_view
                      single_source_t::source_code_representation() const noexcept
{
    return _m_str;
}

const __constexpr_imp std::source_location&
                      single_source_t::source_location() const noexcept
{
    return _m_sl;
}

__constexpr_imp bool
    single_source_t::operator==(
        const single_source_t& _a_rhs
    ) const noexcept
{
    __cmp_test(_m_str);
    __cmp_test(_m_sl.column());
    __cmp_test(_m_sl.file_name());
    __cmp_test(_m_sl.function_name());
    __cmp_test(_m_sl.line());
    return true;
}

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::single_source_t>::format(
        _ABC_NS_DS::single_source_t _a_rtd,
        format_context&          _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0} {{"
        "{1} = {2}, "
        "{3} = {4}"
        "}}",
        typeid(_a_rtd).name(),
        "_m_str",
        _a_rtd.source_code_representation(),
        "_m_sl",
        _a_rtd.source_location()
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}