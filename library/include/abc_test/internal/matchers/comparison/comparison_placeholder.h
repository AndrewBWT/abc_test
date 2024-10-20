#pragma once
#include "abc_test/internal/utility/internal/macros.h"

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief Object used to direct the compiler to using the matcher_t-based
 * comparison operators in abc_test/internal/matchers/comparison/operators.h.
 */
struct comparison_placeholder_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __constexpr
    comparison_placeholder_t() noexcept
        = default;
    /*!
     * @brief Dummy constructor.
     *
     * This constructor is used in tandeom with the macros to insert a string
     * which can be used to alert the system on ways to re-write the textual
     * representation of internally called macros to match the originally
     * written macro.
     *
     * @param _a_str The string argument. This is not used in the constructor.
     */
    __constexpr
    comparison_placeholder_t(const std::string_view _a_str) noexcept;
    /*!
     * @brief Equality operator for comparing comparison_placeholder_t objects.
     * @param  The element to compare this object to.
     * @return True if the objects are equal; false otherwise.
     */
    __constexpr bool
        operator==(const comparison_placeholder_t&) const
        = default;
};

_END_ABC_MATCHER_NS

_BEGIN_ABC_MATCHER_NS

__constexpr_imp
    comparison_placeholder_t::comparison_placeholder_t(
        const std::string_view _a_str
    ) noexcept
    : comparison_placeholder_t()
{}

_END_ABC_MATCHER_NS