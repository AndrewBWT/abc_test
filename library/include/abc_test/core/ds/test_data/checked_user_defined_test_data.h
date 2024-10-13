#pragma once
#include "abc_test/core/ds/test_data/user_defined_test_data.h"

_BEGIN_ABC_DS_NS

/*!
 * @brief Structure used to construct test elements. It is specifically designed
 * to perform compile-time checks on the test.
 *
 * checked_user_defined_test_data_t can be thought of as a wrapper around an
 * user_defined_test_data_t object. It checks specific data points at compile
 * time, allowing these errors to be caught early.
 */
class checked_user_defined_test_data_t
{
public:
    /*!
     * @brief Static function used to create instances of this class. Only way
     * of creating such instances.
     *
     * It performs compile-time checks; if this function fails to compile, then
     * the user-provided data is invalid.
     *
     * @param _a_udfd user_defined_test_data_t element to check.
     * @return Constructed checked_user_defined_test_data_t element.
     */
    __constexpr static checked_user_defined_test_data_t
        test_data(const user_defined_test_data_t& _a_udfd);
    /*!
     * @brief Returns the object's user_defined_test_data_t to the caller.
     * @return Object's user_defined_test_data_t member variable.
     */
    __constexpr const user_defined_test_data_t&
        user_defined_test_data() const noexcept;
private:
    /*!
     * @brief Private constructor. Cannot be accessed by the user.
     * 
     * It is this function which performs the tests.
     * @param _a_udfd user_defined_test_data_t object.
     */
    __constexpr
    checked_user_defined_test_data_t(const user_defined_test_data_t& _a_udfd
    ) noexcept;
    //*! Member variable of the user_defined_test_data_t object.
    user_defined_test_data_t _m_udtd;
};

_END_ABC_DS_NS

template <>
struct fmt::formatter<abc::ds::checked_user_defined_test_data_t>
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
            abc::ds::checked_user_defined_test_data_t _a_rtd,
            format_context&                           _a_ctx
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp checked_user_defined_test_data_t
    checked_user_defined_test_data_t::test_data(
        const user_defined_test_data_t& _a_udfd
    )
{
    if (_a_udfd.name == "")
    {
        throw "name cannot be blank. Must have some value";
    }
    if (_a_udfd.threads_required == 0)
    {
        throw "threads_required cannot be 0. Must be a strictly positive "
              "integer";
    }
    return checked_user_defined_test_data_t(_a_udfd);
}

__constexpr_imp const user_defined_test_data_t&
    checked_user_defined_test_data_t::user_defined_test_data() const noexcept
{
    return _m_udtd;
}

__constexpr_imp
    checked_user_defined_test_data_t::checked_user_defined_test_data_t(
        const user_defined_test_data_t& _a_udfd
    ) noexcept
    : _m_udtd(_a_udfd)
{}

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<abc::ds::checked_user_defined_test_data_t>::format(
        abc::ds::checked_user_defined_test_data_t _a_rtd,
        format_context&                           _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}{{"
        "{1} = {2}"
        "}}",
        typeid(_a_rtd).name(),
        "_m_udtd",
        _a_rtd.user_defined_test_data()
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}