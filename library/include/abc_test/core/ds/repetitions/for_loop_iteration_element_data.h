#pragma once
#include "abc_test/utility/internal/log.h"
#include "abc_test/utility/internal/macros.h"
#include "fmt/format.h"

#include <tuple>
#include <vector>

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure to hold information about a specific element of data
 * generated from a gen_data_t collection. It contains no information pertainig
 * to the for loop, instead only the data neeeded by the gen_datat element.
 *
 * To be clear, the data generated isn't contained here. Instead what is stored
 * here is the information needed by the gen_data_t element to recreate that
 * data.
 *
 * We refer to the data element which would be created as the iteration data.
 */
struct for_loop_iteration_element_data_t
{
public:
    /*!
     * @brief Equality operator for for_loop_iteration_element_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return True if equal, false if not.
     */
    __constexpr bool
        operator==(const for_loop_iteration_element_data_t& _a_rhs
        ) const noexcept
        = default;
    /*!
     * @brief Spaceship operator for for_loop_iteration_element_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return Dependant on context.
     */
    __constexpr auto
        operator<=>(const for_loop_iteration_element_data_t& _a_rhs
        ) const noexcept
        = default;
    /*!
     * @brief The mode the gen_data_t element was in when the iteration data was
     * created.
     */
    std::size_t mode = 0;
    /*!
     * @brief The additional data from the gen_data_t element from when the
     * iteration data was created.
     */
    std::string additional_data;
};

_END_ABC_DS_NS

template <>
struct fmt::formatter<abc::ds::for_loop_iteration_element_data_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    // Cannot be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(
            abc::ds::for_loop_iteration_element_data_t _a_rd,
            format_context&                            _a_cxt
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<abc::ds::for_loop_iteration_element_data_t>::format(
        abc::ds::for_loop_iteration_element_data_t _a_rd,
        format_context&                            _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        ", {3} = {4}"
        "}}",
        typeid(_a_rd).name(),
        "_m_additional_data",
        _a_rd.additional_data,
        "_m_mode",
        _a_rd.mode
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}