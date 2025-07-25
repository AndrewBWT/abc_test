#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/core/matchers/logic_enum.hpp"

_BEGIN_ABC_MATCHER_NS
/*!
 * @brief Enum used to represent the different comparison operators.
 */
enum class comparison_enum_t
{
    /*!
     * @brief Enum representing equality.
     */
    EQ,
    /*!
     * @brief Enum representing non-equality.
     */
    NEQ,
    /*!
     * @brief Enum representing less than or equal to.
     */
    LEQ,
    /*!
     * @brief Enum representing less than.
     */
    LT,
    /*!
     * @brief Enum representing greater than or equal to.
     */
    GEQ,
    /*!
     * @brief Enum representing greater than.
     */
    GT
};
/*!
 * @brief Function which gets the precedence number of the template argument.
 * @tparam Comparison_Enum The template argument.
 * @return A precedence_t element representing the operators precedence. This is
 * represented as an integer.
 */
template <comparison_enum_t Comparison_Enum>
__constexpr precedence_t
    cmp_precedence() noexcept;
/*!
 * @brief Function which calls the comparison operator depending on the
 * template.
 * @tparam T1 The type of the first argument.
 * @tparam T2 The type of the second argument.
 * @tparam Comparison_Enum An enum representing the type of comparison to perform.
 * @param _a_l The first T1 argument.
 * @param _a_r The second argument.
 * @return Performs the enum's comparison on the arguments, returning the bool result.
 */
template <typename T1, typename T2, comparison_enum_t Comparison_Enum>
__constexpr bool
    cmp(const T1& _a_l, const T2& _a_r) noexcept;
/*!
 * @brief Gets a string representation of the enum argument.
 * 
 * @tparam Comparison_Enum The comparison enum template argument.
 * @return A const char* containing a string representation of the argument.
 */
template <comparison_enum_t Comparison_Enum>
__constexpr std::u8string_view
    cmp_str() noexcept;
template<comparison_enum_t Comparison_Enum>
__constexpr std::u8string_view
not_cmp_str() noexcept;
_END_ABC_MATCHER_NS

_BEGIN_ABC_MATCHER_NS
template <comparison_enum_t Comparison_Enum>
__constexpr_imp precedence_t
    cmp_precedence() noexcept
{
    using enum comparison_enum_t;
    if constexpr (Comparison_Enum == EQ || Comparison_Enum == NEQ)
    {
        return 10;
    }
    else if constexpr (Comparison_Enum == LEQ || Comparison_Enum == LT
                       || Comparison_Enum == GEQ || Comparison_Enum == GT)
    {
        return 9;
    }
    else
    {
        __STATIC_ASSERT_NO_TYPE(
            "precedence function does not have value for given template type."
        );
        return 0;
    }
}

template <typename T1, typename T2, comparison_enum_t Comparison_Enum>
__constexpr_imp bool
    cmp(
        const T1& _a_l,
        const T2& _a_r
    ) noexcept
{
    using enum comparison_enum_t;
    if constexpr (Comparison_Enum == EQ)
    {
        return _a_l == _a_r;
    }
    else if constexpr (Comparison_Enum == NEQ)
    {
        return _a_l != _a_r;
    }
    else if constexpr (Comparison_Enum == LT)
    {
        return _a_l < _a_r;
    }
    else if constexpr (Comparison_Enum == LEQ)
    {
        return _a_l <= _a_r;
    }
    else if constexpr (Comparison_Enum == GT)
    {
        return _a_l > _a_r;
    }
    else if constexpr (Comparison_Enum == GEQ)
    {
        return _a_l >= _a_r;
    }
    else
    {
        __STATIC_ASSERT_NO_TYPE(
            "cmp function does not have value for given template type."
        );
        return true;
    }
}

template <comparison_enum_t Comparison_Enum>
__constexpr_imp std::u8string_view
    cmp_str() noexcept
{
    using enum comparison_enum_t;
    if constexpr (Comparison_Enum == EQ)
    {
        return u8"==";
    }
    else if constexpr (Comparison_Enum == NEQ)
    {
        return u8"!=";
    }
    else if constexpr (Comparison_Enum == LT)
    {
        return u8"<";
    }
    else if constexpr (Comparison_Enum == LEQ)
    {
        return u8"<=";
    }
    else if constexpr (Comparison_Enum == GT)
    {
        return u8">";
    }
    else if constexpr (Comparison_Enum == GEQ)
    {
        return u8">=";
    }
    else
    {
        __STATIC_ASSERT_NO_TYPE(
            "str function does not have value for given template type."
        );
        return u8"";
    }
}
template<comparison_enum_t Comparison_Enum>
__constexpr_imp std::u8string_view
not_cmp_str() noexcept
{
    using enum comparison_enum_t;
    if constexpr (Comparison_Enum == EQ)
    {
        return u8"!=";
    }
    else if constexpr (Comparison_Enum == NEQ)
    {
        return u8"==";
    }
    else if constexpr (Comparison_Enum == LT)
    {
        return u8">=";
    }
    else if constexpr (Comparison_Enum == LEQ)
    {
        return u8">";
    }
    else if constexpr (Comparison_Enum == GT)
    {
        return u8"<=";
    }
    else if constexpr (Comparison_Enum == GEQ)
    {
        return u8"<";
    }
    else
    {
        __STATIC_ASSERT_NO_TYPE(
            "not_cmp_str function does not have value for given template type."
        );
        return u8"";
    }
}
_END_ABC_MATCHER_NS