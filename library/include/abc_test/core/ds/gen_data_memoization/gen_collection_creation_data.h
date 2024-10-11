#pragma once
#include "abc_test/core/ds/gen_data_memoization/gen_data_creation_data.h"

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure to hold information about a specific element of data
 * generated from a gen_data_t collection.
 *
 * To be clear, the data generated isn't contained here. Instead, we hold the
 * information used to re-create that data when the same for loop is
 * encountered.
 *
 * We refer to the data element which would be created as the iteration data.
 */

/*!
 * @brief This structure holds information pertaining to a single piece of data
 * generated from a gen_data_t element, contained within a gen_data_collection_t
 * element.
 *
 * It contains a variable of type gen_data_creation_data_t, and is closely
 * related. Whereas that class represents data from a gen_data_t instance, this
 * structure represents the data from a gen_data_t instance contained within a
 * gen_data_collection_t instance. Specifically, the additional data held
 * pertains to the index of the gen_data_t object in the gen_data_collection_t
 * collection.
 */
struct gen_collection_creation_data_t
{
public:
    /*!
     * @brief Equality operator for gen_collection_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return True if equal, false if not.
     */
    __constexpr bool
        operator==(const gen_collection_creation_data_t& _a_rhs) const noexcept
        = default;
    /*!
     * @brief Spaceship operator for gen_collection_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return Dependant on context.
     */
    __constexpr auto
        operator<=>(const gen_collection_creation_data_t& _a_rhs) const noexcept
        = default;
    /*!
     * @brief The index in the gen_data_collection_t collection where the
     * gen_data_t element that the flied was created from
     */
    std::size_t generation_collection_index = 0;
    /*!
     * @brief The internal gen_data_creation_data_t variable.
     */
    gen_data_creation_data_t flied;
};

_END_ABC_DS_NS

template <>
struct fmt::formatter<abc::ds::gen_collection_creation_data_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a gen_collection_creation_data_t object
     */
    // Cannot be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(
            abc::ds::gen_collection_creation_data_t _a_rd,
            format_context&                         _a_cxt
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<abc::ds::gen_collection_creation_data_t>::format(
        abc::ds::gen_collection_creation_data_t _a_rd,
        format_context&                         _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        ", {3} = {4}"
        "}}",
        typeid(_a_rd).name(),
        "_m_generation_collection_index",
        _a_rd.generation_collection_index,
        "_m_flied",
        _a_rd.flied
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}