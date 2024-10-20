#pragma once

#include "abc_test/core/matchers/operator_based_matcher.h"
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_NS

/*!
 * @brief Enum containing the different logical values used with the
 * logic_matcher_t.
 */
enum logic_enum_t
{
    /*!
     * @brief Represents the not (!) operator.
     */
    NOT,
    /*!
     * @brief Represents the or (||) operator.
     */
    OR,
    /*!
     * @brief Represents the and (&&) operator.
     */
    AND
};

/*!
 * @brief Gets the precedence of the operator given as a tempalte.
 * @tparam Logic_Enum The template to find the precedence of.
 * @return The precdence of the templated enum.
 */
template <logic_enum_t Logic_Enum>
__constexpr precedence_t
    logic_precedence() noexcept;
/*!
 * @brief Returns a string version of the operator given as a template.
 * @tparam The template used to create the string.
 * @return A char* representative of the enum template.
 */
template <logic_enum_t Logic_Enum>
__constexpr const char*
    logic_str() noexcept;
/*!
 * @brief Function which takes two booealn values and runs the given logical
 * operator, descirbed by the template paramter, on them.
 *
 * Note if the logical operator is NOT, this still runs. It just evaluates the
 * first argument using the not operator.
 *
 * @tparam The template used to run the logic.
 * @param _a_left_result The left boolean result.
 * @param _a_right_result The right boolean result.
 * @return True if the _a_left_result Logic_Enum _a_right_result returns true.
 * False otherwise.
 */
template <logic_enum_t Logic_Enum>
__constexpr bool
    compute_logic_result(const bool _a_left_result, const bool _a_right_result)
        noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
template <logic_enum_t Logic_Enum>
__constexpr_imp precedence_t
    logic_precedence() noexcept
{
    using enum logic_enum_t;
    if constexpr (Logic_Enum == NOT)
    {
        return 3;
    }
    else if constexpr (Logic_Enum == OR)
    {
        return 15;
    }
    else if constexpr (Logic_Enum == AND)
    {
        return 14;
    }
    else
    {
        __STATIC_ASSERT(
            Logic_Enum,
            "precedence function does not have value for given template type."
        );
        return 0;
    }
}

template <logic_enum_t Logic_Enum>
__constexpr_imp const char*
    logic_str() noexcept
{
    using enum logic_enum_t;
    if constexpr (Logic_Enum == NOT)
    {
        return "!";
    }
    else if constexpr (Logic_Enum == OR)
    {
        return "||";
    }
    else if constexpr (Logic_Enum == AND)
    {
        return "&&";
    }
    else
    {
        __STATIC_ASSERT(
            Logic_Enum,
            "precedence function does not have value for given template type."
        );
        return "";
    }
}

template <logic_enum_t Logic_Enum>
__constexpr_imp bool
    compute_logic_result(
        const bool _a_left_result,
        const bool _a_right_result
    ) noexcept
{
    using enum logic_enum_t;
    if constexpr (Logic_Enum == NOT)
    {
        return false;
    }
    else if constexpr (Logic_Enum == OR)
    {
        return _a_left_result || _a_right_result;
    }
    else if constexpr (Logic_Enum == AND)
    {
        return _a_left_result && _a_right_result;
    }
    else
    {
        __STATIC_ASSERT(
            Logic_Enum,
            "compute_logic_result function does not have value for given "
            "template type."
        );
        return "";
    }
}

_END_ABC_NS