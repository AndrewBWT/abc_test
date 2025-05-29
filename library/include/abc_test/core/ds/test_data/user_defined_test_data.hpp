#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/unicode.hpp"

#include <fmt/base.h>
#include <optional>
#include <string>
#include <typeinfo>

_BEGIN_ABC_DS_NS

/*!
 * @brief Contains user-defined information for a test function.
 *
 * All the fields are public. This object is designed to be declared as part of
 * a test function.
 */

/*!
 * @brief This structure holds user-defined information for an associated test
 * function.
 *
 * All fields are public, as it is designed to be declared when creating a test.
 */
class user_defined_test_data_t
{
public:
    /*!
     * @brief Equality operator to compare user_defined_test_data_t objects.
     * @param _a_rhs The user_defined_test_data_t to compare this object to.
     * @return True if the objects are equal, false otherwise.
     */
    __constexpr bool
        operator==(const user_defined_test_data_t& _a_rhs) const noexcept
        = default;
    __constexpr std::u8string
                make_uid(const std::u8string_view _a_delimiter) const noexcept;
    /*!
     * @brief The name of the test. There are some conditions on the name of the
     * test, however these are only relevant when considering groups of tests.
     */
    std::string_view name;
    /*!
     * @brief The description of the test. This is an optional field which
     * provides a description to the reader.
     */
    std::optional<std::string_view> description;
    /*!
     * @brief The "path" of the test. This is both a file path, and a set of
     * identifiers used to organise tests.
     */
    std::string_view path;
    /*!
     * @brief The number of threads required by the test. By default this value
     * is set at 1.
     */
    std::size_t threads_required = 1;
};

_END_ABC_DS_NS

_BEGIN_ABC_NS
/*!
 * @brief A type synonym expoed to the user, used to make writing tests easier.
 */
using test_case_t = _ABC_NS_DS::user_defined_test_data_t;
_END_ABC_NS

template <>
struct fmt::formatter<_ABC_NS_DS::user_defined_test_data_t>
    : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can't be cosntepxr due to use of fmt::format.
    /*!
     * @brief Formats a user_defined_test_data_t object.
     * @param _a_rtd The user_defined_test_data_t object.
     * @param _a_ctx The format_context.
     * @return The formatted string.
     */
    __no_constexpr auto
        format(
            _ABC_NS_DS::user_defined_test_data_t _a_rtd,
            format_context&                      _a_ctx
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr std::u8string
            user_defined_test_data_t::make_uid(
        const std::u8string_view _a_delimiter
    ) const noexcept
{
    using namespace std;
    return u8string{}
        .append(convert_ascii_to_unicode<char8_t>(this->name).value())
        .append(_a_delimiter)
        .append(convert_ascii_to_unicode<char8_t>(this->path).value());
}

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::user_defined_test_data_t>::format(
        _ABC_NS_DS::user_defined_test_data_t _a_rtd,
        format_context&                      _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        ", {3} = {4}"
        ", {5} = {6}"
        ", {7} = {8}"
        "}}",
        typeid(_a_rtd).name(),
        "name",
        _a_rtd.name,
        "description",
        _a_rtd.description,
        "path",
        _a_rtd.path,
        "threads_required",
        _a_rtd.threads_required
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}