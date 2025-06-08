#pragma once
#include "abc_test/core/test_reports/assertion_status/fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass.hpp"
#include "abc_test/core/matchers/matcher_result.hpp"


_BEGIN_ABC_MATCHER_NS
// Forward declaration
template <bool Is_Annotated>
struct matcher_wrapper_t;
_END_ABC_MATCHER_NS
_BEGIN_ABC_NS
// Forward declarations
using matcher_t = _ABC_NS_MATCHER::matcher_wrapper_t<false>;
_END_ABC_NS

_BEGIN_ABC_MATCHER_NS
template <typename Assertion_Status>
requires std::same_as<Assertion_Status, _ABC_NS_REPORTS::pass_t>
         || std::same_as<Assertion_Status, _ABC_NS_REPORTS::fail_t>
__constexpr matcher_result_t
    mk_matcher_result() noexcept;
 // namespace

_END_ABC_MATCHER_NS
_BEGIN_ABC_NS
/*!
 * @brief Function creates a matcher_t containing a passing static_matcher_t
 * object.
 * @return A matcher_t containing a passing static_matcher_t object.
 */
__no_constexpr_or_inline matcher_t
    true_matcher() noexcept;
/*!
 * @brief Function creates a matcher_t containing a failing static_matcher_t
 * object.
 * @return A matcher_t object containing a failing static_matcher_t object.
 */
__no_constexpr_or_inline matcher_t
    false_matcher() noexcept;
_END_ABC_NS

_BEGIN_ABC_MATCHER_NS
template <typename Assertion_Status>
requires std::same_as<Assertion_Status, _ABC_NS_REPORTS::pass_t>
         || std::same_as<Assertion_Status, _ABC_NS_REPORTS::fail_t>
__constexpr_imp matcher_result_t
    mk_matcher_result() noexcept
{
    using namespace reports;
    if constexpr (std::same_as<Assertion_Status, pass_t>)
    {
        return matcher_result_t(true, matcher_result_infos_t(u8"true"));
    }
    else if constexpr (std::same_as<Assertion_Status, fail_t>)
    {
        return matcher_result_t(false, matcher_result_infos_t(u8"false"));
    }
    else
    {
        __STATIC_ASSERT(
            Assertion_Status, "Cannot instnatiate run method for static_matcher"
        );
    }
}
 // namespace

_END_ABC_MATCHER_NS