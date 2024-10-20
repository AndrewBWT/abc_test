#include "abc_test/core/matchers/static_matcher.h"
#include "abc_test/core/matchers/matcher_wrapper.h"

_BEGIN_ABC_NS
__no_constexpr_or_inline_imp matcher_t
true_matcher() noexcept
{
    return matcher(new static_matcher_t<reports::pass_t>());
}

__no_constexpr_or_inline_imp matcher_t
false_matcher() noexcept
{
    return matcher(new static_matcher_t<reports::fail_t>());
}
_END_ABC_NS