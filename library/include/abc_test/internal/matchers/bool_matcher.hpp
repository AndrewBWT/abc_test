#pragma once
#include "abc_test/internal/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief Matcher object which takes a single boolean and considers that to be
 * the matcher's result.
 */
class bool_matcher_t : public matcher_base_t
{
public:
    /*!
     * @brief Constructor.
     * @param _a_pass The result of the matcher.
     */
    __constexpr
    bool_matcher_t(const bool _a_pass) noexcept;
private:
    bool _m_pass;
    __constexpr virtual matcher_result_t
        run(test_runner_t& _a_test_runner) final override;
};
_END_ABC_MATCHER_NS
_BEGIN_ABC_NS
/*!
 * @brief Free function which builds a bool_matcher_t in a matcher_t object.
 * @param _a_bool The result of the matcher.
 * @return A matcher_t containing a bool_matcher_t with _a_bool as its result.
 */
__no_constexpr_imp matcher_t
    bool_matcher(const bool _a_bool) noexcept;

_END_ABC_NS

_BEGIN_ABC_MATCHER_NS
__constexpr_imp
    bool_matcher_t::bool_matcher_t(
        const bool _a_pass
    ) noexcept
    : _m_pass(_a_pass)
{}

__constexpr_imp matcher_result_t
    bool_matcher_t::run(
        test_runner_t& _a_test_runner
    )
{
    return matcher_result_t(true, _m_pass, fmt::format("{0}", _m_pass));
}
_END_ABC_MATCHER_NS
_BEGIN_ABC_NS

__no_constexpr_imp matcher_t
    bool_matcher(
        const bool _a_bool
    ) noexcept
{
    using namespace _ABC_NS_MATCHER;
    return make_matcher(new bool_matcher_t(_a_bool));
}

_END_ABC_NS