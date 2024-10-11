#pragma once
#include "abc_test/utility/internal/log.h"
#include "abc_test/utility/internal/macros.h"
#include "fmt/format.h"

#include <tuple>
#include <vector>

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure which holds information pertaining to a single piece of data
 * created from a gen_data_t element.
 *
 * Each derived class of gen_data_t has two pieces of data that, when put
 * together with the original gen_data_t object, should be able to re-generate
 * the same piece of data. This is the mode and the additional data string. This
 * class contains these data points.
 */
struct gen_data_creation_data_t
{
public:
    /*!
     * @brief Equality operator for gen_data_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return True if equal, false if not.
     */
    __constexpr bool
        operator==(const gen_data_creation_data_t& _a_rhs) const noexcept
        = default;
    /*!
     * @brief Spaceship operator for gen_data_creation_data_t elements.
     * @param _a_rhs The element to compare this instance to.
     * @return Dependant on context.
     */
    __constexpr auto
        operator<=>(const gen_data_creation_data_t& _a_rhs) const noexcept
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

_END_ABC_DS_NS

template <>
struct fmt::formatter<abc::ds::gen_data_creation_data_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a gen_data_creation_data_t object
     */
    // Cannot be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(abc::ds::gen_data_creation_data_t _a_rd, format_context& _a_cxt)
            const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<abc::ds::gen_data_creation_data_t>::format(
        abc::ds::gen_data_creation_data_t _a_rd,
        format_context&                   _a_ctx
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