#pragma once

#include "abc_test/internal/test_reports/unexpected_thrown_non_descript_entity.hpp"

_BEGIN_ABC_REPORTS_NS

/*!
 * @brief Class used to model entities thrown from a test which are subclasses
 * of std::exception.
 */
class unexpected_thrown_exception_t
    : public unexpected_thrown_not_derived_from_std_exception_t
{
public:
    /*!
     * @brief Constructor.
     * @param _a_source The closest source of the exception.
     * @param _a_exception_name The exception name.
     * @param _a_exception_what_function The string from the exception's what() function.
     * @return 
     */
    __constexpr
    unexpected_thrown_exception_t(
        const std::optional<_ABC_NS_DS::single_source_t>& _a_source,
        const std::string_view     _a_exception_name,
        const std::string_view     _a_exception_what_function
    ) noexcept;
    /*!
     * @brief Returns the string representing the type of the exception.
     * @return A cref to a string representing the type of the exception.
     */
    __constexpr const std::string_view
                      exception_type() const noexcept;
    /*!
     * @brief REturns the string representing the exception's what() message.
     * @return A cref of a string representign the exception's what() message.
     */
    __constexpr const std::string_view
                      exception_message() const noexcept;
protected:
    std::string _m_exception_name;
    std::string _m_exception_what_function;
};

_END_ABC_REPORTS_NS

_BEGIN_ABC_REPORTS_NS
__constexpr_imp
    unexpected_thrown_exception_t::unexpected_thrown_exception_t(
        const std::optional<_ABC_NS_DS::single_source_t>& _a_source,
        const std::string_view     _a_exception_name,
        const std::string_view     _a_exception_what_function
    ) noexcept
    : unexpected_thrown_not_derived_from_std_exception_t(_a_source)
    , _m_exception_name(_a_exception_name)
    , _m_exception_what_function(_a_exception_what_function)
{}

__constexpr_imp const std::string_view
    unexpected_thrown_exception_t::exception_type() const noexcept
{
    return _m_exception_name;
}

__constexpr_imp const std::string_view
    unexpected_thrown_exception_t::exception_message() const noexcept
{
    return _m_exception_what_function;
}

_END_ABC_REPORTS_NS