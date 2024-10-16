#pragma once
#include "abc_test/core/options/test_options_base.h"

#include <expected>

_BEGIN_ABC_NS
class validated_test_options_t;
using validated_test_options_or_error_t
    = std::expected<validated_test_options_t, std::string>;

/*!
 * @brief Class to hold a validated test_options_base_t variable.
 */
class validated_test_options_t
{
public:
    test_options_base_t _m_options;
    /*!
     * @brief Gets a cref to the internal test_options_base_t object.
     * @return Cref to the object's test_options_base_t member variable.
     */
    __constexpr_imp const test_options_base_t&
        get_options() const noexcept;
    /*!
     * @brief Function which validates a test_options base, return a
     * validated_test_options_or_error_t type.
     * @param _a_test_options The variable to check.
     * @return A validated_test_options_or_error_t variable. It is either a
     * validated_test_options_t object, if validation succeeded. Otherwise, it
     * is a string describing the validation failure.
     */
    __no_constexpr static validated_test_options_or_error_t
        validate_test_options(const test_options_base_t& _a_test_options
        ) noexcept;
private:
    __no_constexpr
        validated_test_options_t(const test_options_base_t& _a_test_options
        ) noexcept;
};

_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp const test_options_base_t&
    validated_test_options_t::get_options() const noexcept
{
    return _m_options;
}

__no_constexpr_imp validated_test_options_or_error_t
    validated_test_options_t::validate_test_options(
        const test_options_base_t& _a_test_options
    ) noexcept
{
    using namespace std;
    if (optional<string> _l_error_msg{_a_test_options.validate()};
        _l_error_msg.has_value())
    {
        return unexpected(_l_error_msg.value());
    }
    else
    {
        return validated_test_options_t(_a_test_options);
    }
}

__no_constexpr_imp
    validated_test_options_t::validated_test_options_t(
        const test_options_base_t& _a_test_options
    ) noexcept
    : _m_options(_a_test_options)
{}

_END_ABC_NS