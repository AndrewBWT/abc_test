#pragma once
#include "abc_test/utility/internal/log.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include <fmt/format.h>
#include "abc_test/utility/printers/default_printer.hpp"

#include <tuple>
#include <vector>
#include "abc_test/utility/types.hpp"
#include <array>

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure which holds memoized data from a data_generator_t element.
 * This data can be used to re-generate data from that data_generator_t element.
 *
 * Each derived class of data_generator_t has two pieces of data that, when put
 * together with the original data_generator_t object, should be able to
 * re-generate some data from that data_generator_t. Instances of this class
 * contain that data.
 */
struct data_generator_memoized_element_t
{
public:
    /*!
     * @brief Equality operator for gen_data_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return True if equal, false if not.
     */
    __constexpr bool
        operator==(const data_generator_memoized_element_t& _a_rhs
        ) const noexcept
        = default;
    /*!
     * @brief Spaceship operator for gen_data_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return Dependant on context.
     */
    __constexpr auto
        operator<=>(const data_generator_memoized_element_t& _a_rhs
        ) const noexcept
        = default;
    /*!
     * @brief The mode the gen_data_t element was in when the iteration data was
     * created. Classes derived from gen_data_t can have different modes -
     * integers which change the behaviour of the class. This variable holds that
     * data.
     */
    std::size_t mode = 0;
    /*!
     * @brief The additional data from the gen_data_t element from when the
     * iteration data was created. This data isn't usually created when the
     * gen_data_t generates its data. However, it can be retrieved and stored
     * and used to re-generate that data. This variable holds that string.
     */
    std::u8string additional_data;
};

/*!
 * @brief Type synonym which shortens the name.
 */
using dg_memoized_element_t = data_generator_memoized_element_t;

using opt_dg_memoized_element_t = std::optional<dg_memoized_element_t>;

_END_ABC_DS_NS
_BEGIN_ABC_UTILITY_PRINTER_NS
template <>
struct default_printer_t<
    abc::ds::data_generator_memoized_element_t>
    : public printer_base_t<
    abc::ds::data_generator_memoized_element_t>
{
    static constexpr bool is_specialized{ true };

    __no_constexpr_imp    std::u8string
        run_printer(
            const abc::ds::data_generator_memoized_element_t&
            _a_object
        ) const
    {
        using namespace _ABC_NS_UTILITY_STR;
        return object_printer_with_field_names(
            object_printer_parser_t{},
            type_id<decltype(_a_object)>(),
            { u8"mode",u8"additional_data" },
            _a_object.mode,
            _a_object.additional_data
        );
    }
};
_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_DS_NS
_END_ABC_DS_NS