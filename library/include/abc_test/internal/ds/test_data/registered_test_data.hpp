#pragma once
#include "abc_test/internal/ds/source/single_source.hpp"
#include "abc_test/internal/ds/test_data/checked_user_defined_test_data.hpp"
#include "abc_test/internal/ds/type_synonyms.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"
#include <fmt/base.h>
#include <fmt/format.h>

#include <fmt/std.h>
#include <optional>
#include <source_location>
#include <string>
#include "abc_test/internal/ds/test_data/test_function_object.hpp"

_BEGIN_ABC_DS_NS

/*!
 * @brief This structure is used to store information about a test-case created
 * by the user. It exists in isoultion in comparison to the other tests, and has
 * had no checks done on it except for some compile-time checks.
 */
struct registered_test_data_t
{
public:
    // Default constructor deleted.
    __constexpr
    registered_test_data_t() noexcept
        = delete;
    /*!
     * @brief Constructor
     * @param _a_test_function A reference to the actual test function.
     * @param _a_user_data The user provided checked_user_defined_test_data_t
     * object. This is a structure which can be checked at compile time for
     * erronious values.
     * @param _a_source Source information about the test.
     */
    __no_constexpr
    registered_test_data_t(
        const test_function_t                  _a_test_function,
        const checked_user_defined_test_data_t _a_user_data,
        const ds::single_source_t&             _a_source
    ) noexcept;
    __no_constexpr
        registered_test_data_t(
            const test_function_t                  _a_test_function,
            const checked_user_defined_test_data_t _a_user_data
        ) noexcept;
    /*!
     * @brief Equality operator to test registered_test_data_t objects.
     * @arg _a_rhs registered_test_data_t object to compare this object to.
     * @return bool True if _a_rhs is equal to this object, false otherwise.
     */
    __constexpr bool
        operator==(const registered_test_data_t& _a_rhs) const noexcept
        = default;
    /*!
     * @brief The actual test function being ran.
     */
    test_function_t _m_test_function;
    /*!
     * @brief The user-defined test data.
     */
    user_defined_test_data_t _m_user_data;
    /*!
     * @brief The location of the test. This includes the source code and a
     * string representation of the test.
     */
    std::optional<ds::single_source_t> _m_source;
};

_END_ABC_DS_NS

/*!
 * formatter for registered_test_data_t
 */
template <>
struct fmt::formatter<_ABC_NS_DS::registered_test_data_t> : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can't be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(_ABC_NS_DS::registered_test_data_t _a_rtd, format_context& _a_ctx)
            const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__no_constexpr_imp
    registered_test_data_t::registered_test_data_t(
        const test_function_t                  _a_test_function,
        const checked_user_defined_test_data_t _a_user_data,
        const ds::single_source_t&             _a_source
    ) noexcept
    : _m_user_data(_a_user_data.user_defined_test_data())
    , _m_source(_a_source)
    , _m_test_function(_a_test_function)
{}
__no_constexpr_imp
registered_test_data_t::registered_test_data_t(
    const test_function_t                  _a_test_function,
    const checked_user_defined_test_data_t _a_user_data
) noexcept
    : _m_user_data(_a_user_data.user_defined_test_data())
    , _m_test_function(_a_test_function)
{

}
_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::registered_test_data_t>::format(
        _ABC_NS_DS::registered_test_data_t _a_rtd,
        format_context&                 _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        ", {3} = {4}"
        ", {5} = {6}"
        "}}",
        typeid(_a_rtd).name(),
        "_m_test_function",
        "hello",
        //fmt::format("{0}", static_cast<void*>(_a_rtd._m_test_function)),
        "_m_user_data",
        _a_rtd._m_user_data,
        "_m_source",
        _a_rtd._m_source
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}