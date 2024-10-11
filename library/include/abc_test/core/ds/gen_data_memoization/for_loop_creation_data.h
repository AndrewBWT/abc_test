#pragma once
#include "abc_test/core/ds/gen_data_memoization/gen_collection_creation_data.h"

_BEGIN_ABC_DS_NS

/*!
 * @brief This structure holds information pertainig to a single piece of data
 * generated from a gen_data_t element, contained within a gen_data_collection_t
 * element, itself one of a sequence of gen_data_collection_t elements.
 *
 * It contains a variable of type gen_collection_creation_data_t. Conceptually
 * the only addition is that there is now an index as to which
 * gen_data_collection_t this object is referencing. However, we call it a
 * for_loop index, as gen_data_collection_t elements are designed to be used in
 * them.
 */
struct for_loop_creation_data_t
{
public:
    /*!
     * @brief Equality operator for for_loop_creation_data_t elements.
     * @param _a_rhs The element to compare this element to.
     * @return True if equal, false if not.
     */
    __constexpr bool
        operator==(const for_loop_creation_data_t& _a_rhs) const noexcept
        = default;
    /*!
     * @brief Spaceship operator for for_loop_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return Dependant on context.
     */
    __constexpr auto
        operator<=>(const for_loop_creation_data_t& _a_rhs) const noexcept
        = default;
    /*!
     * @brief The index of the for loop this data point refers to.
     */
    std::size_t for_loop_index = 0;
    /*!
     * @brief The specific set of data for the gen_data_collection_t this
     * structure points to.
     */
    gen_collection_creation_data_t for_loop_iteration_data;
};

/*!
 * @brief A synonym for sequence of for_loop_creation_data_t elements.
 */
using for_loop_creation_data_sequence_t = std::vector<for_loop_creation_data_t>;
/*!
 * @brief A synonym for an optional for_loop_creation_data_t element.
 */
using opt_for_loop_creation_data_t = std::optional<for_loop_creation_data_t>;
_END_ABC_DS_NS

template <>
struct fmt::formatter<abc::ds::for_loop_creation_data_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    // Cannot be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(abc::ds::for_loop_creation_data_t _a_rd, format_context& _a_cxt)
            const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<abc::ds::for_loop_creation_data_t>::format(
        abc::ds::for_loop_creation_data_t _a_rd,
        format_context&                   _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0} {{"
        "{1} = {2}"
        ", {3} = {4}}}",
        typeid(_a_rd).name(),
        "_m_for_loop_index",
        _a_rd.for_loop_index,
        "_m_flid",
        _a_rd.for_loop_iteration_data
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}