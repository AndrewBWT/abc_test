#pragma once
#include "abc_test/internal/utility/internal/log.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"
#include <fmt/format.h>

#include <tuple>
#include <vector>

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
     * integers which change th behaviour of the class. This variable holds that
     * data.
     */
    std::size_t mode = 0;
    /*!
     * @brief The additional data from the gen_data_t element from when the
     * iteration data was created. This data isn't usually created when the
     * gen_data_t generates its data. However, it can be retrieved and stored
     * and used to re-generate that data. This variable holds that string.
     */
    std::string additional_data;
};

/*!
 * @brief Type synonym which shortens the name.
 */
using dg_memoized_element_t = data_generator_memoized_element_t;

using opt_dg_memoized_element_t = std::optional<dg_memoized_element_t>;

_END_ABC_DS_NS

template <>
struct fmt::formatter<_ABC_NS_DS::data_generator_memoized_element_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a gen_data_creation_data_t object
     */
    // Cannot be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(
            _ABC_NS_DS::data_generator_memoized_element_t _a_rd,
            format_context&                            _a_cxt
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::data_generator_memoized_element_t>::format(
        _ABC_NS_DS::data_generator_memoized_element_t _a_rd,
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