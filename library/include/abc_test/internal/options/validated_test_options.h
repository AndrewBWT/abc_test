#pragma once
#include "abc_test/internal/options/test_options_base.h"

#include <expected>

_BEGIN_ABC_NS
template<typename T>
class validated_test_options_t;
/*!
 * @brief Type synonym for an expected type containing either a
 * validated_test_options_t type or an error represented by an std::string.
 */
template<typename T>
using validated_test_options_or_error_t
    = std::expected<validated_test_options_t<T>, std::vector<std::string>>;

/*!
 * @brief Class to hold a validated test_options_base_t variable.
 */
template<typename T>
class validated_test_options_t
{
public:
    const T& _m_options;
    /*!
     * @brief Gets a cref to the internal test_options_base_t object.
     * @return Cref to the object's test_options_base_t member variable.
     */
    __constexpr_imp const T&
        get_options() const noexcept;
    /*!
     * @brief Function which validates a test_options base, return a
     * validated_test_options_or_error_t type.
     * @param _a_test_options The variable to check.
     * @return A validated_test_options_or_error_t variable. It is either a
     * validated_test_options_t object, if validation succeeded. Otherwise, it
     * is a string describing the validation failure.
     */
    __no_constexpr static validated_test_options_or_error_t<T>
        validate_test_options(T& _a_test_options
        ) noexcept;
private:
    __no_constexpr
        validated_test_options_t(T& _a_test_options
        ) noexcept;
};

_END_ABC_NS

_BEGIN_ABC_NS
template<typename T>
__constexpr_imp const T&
    validated_test_options_t<T>::get_options() const noexcept
{
    return _m_options;
}

template<typename T>
__no_constexpr_imp validated_test_options_or_error_t<T>
    validated_test_options_t<T>::validate_test_options(
        T& _a_test_options
    ) noexcept
{
    using namespace std;
    if (optional<vector<string>> _l_error_msg{_a_test_options.validate_and_pre_process()};
        _l_error_msg.has_value())
    {
        return unexpected(_l_error_msg.value());
    }
    else
    {
        return validated_test_options_t(_a_test_options);
    }
}

template<typename T>
__no_constexpr_imp
    validated_test_options_t<T>::validated_test_options_t(
        T& _a_test_options
    ) noexcept
    : _m_options(_a_test_options)
{}

_END_ABC_NS