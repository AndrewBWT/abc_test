#pragma once
#include "abc_test/core/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_NS
/*!
 * @brief Free function which builds a bool_matcher_t in a matcher_t object.
 * @param _a_bool The result of the matcher.
 * @return A matcher_t containing a bool_matcher_t with _a_bool as its result.
 */
__no_constexpr_imp matcher_t
    bool_matcher(const bool _a_bool) noexcept;

_END_ABC_NS

_BEGIN_ABC_NS

__no_constexpr_imp matcher_t
    bool_matcher(
        const bool _a_bool
    ) noexcept
{
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        _a_bool, matcher_result_infos_t(fmt::format(u8"{0}", _a_bool))
    ));
}

_END_ABC_NS