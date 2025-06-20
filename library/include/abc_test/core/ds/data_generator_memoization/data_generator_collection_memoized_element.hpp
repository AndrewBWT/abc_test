#pragma once
#include "abc_test/core/ds/data_generator_memoization/data_generator_memoized_element.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
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
struct data_generator_collection_memoized_element_t
{
public:
    /*!
     * @brief Equality operator for gen_collection_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return True if equal, false if not.
     */
    __constexpr bool
        operator==(const data_generator_collection_memoized_element_t& _a_rhs
        ) const noexcept
        = default;
    /*!
     * @brief Spaceship operator for gen_collection_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return Dependant on context.
     */
    __constexpr auto
        operator<=>(const data_generator_collection_memoized_element_t& _a_rhs
        ) const noexcept
        = default;
    /*!
     * @brief The index in the gen_data_collection_t collection where the
     * gen_data_t element that the flied was created from
     */
    std::size_t generation_collection_index = 0;
    /*!
     * @brief The internal gen_data_creation_data_t variable.
     */
    dg_memoized_element_t flied;
};

using dgc_memoized_element_t = data_generator_collection_memoized_element_t;

_END_ABC_DS_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<abc::ds::data_generator_collection_memoized_element_t>
    : public printer_base_t<
          abc::ds::data_generator_collection_memoized_element_t>
{
    static constexpr bool is_specialized{true};

    __no_constexpr        std::u8string
        run_printer(const abc::ds::data_generator_collection_memoized_element_t&
                        _a_object) const;
};

_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

__no_constexpr_imp std::u8string
    default_printer_t<abc::ds::data_generator_collection_memoized_element_t>::
        run_printer(
            const abc::ds::data_generator_collection_memoized_element_t&
                _a_object
        ) const
{
    using namespace _ABC_NS_UTILITY_STR;
    return object_printer_with_field_names(
        object_printer_parser_t{},
        type_id<decltype(_a_object)>(),
        {u8"generation_collection_index", u8"flied"},
        _a_object.generation_collection_index,
        _a_object.flied
    );
}

_END_ABC_UTILITY_PRINTER_NS