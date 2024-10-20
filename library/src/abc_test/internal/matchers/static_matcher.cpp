#include "abc_test/internal/matchers/static_matcher.h"

#include "abc_test/internal/matchers/matcher_wrapper.h"

_BEGIN_ABC_NS
__no_constexpr_or_inline_imp matcher_t
    true_matcher() noexcept
{
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_REPORTS;
    return make_matcher(new static_matcher_t<pass_t>());
}

__no_constexpr_or_inline_imp matcher_t
    false_matcher() noexcept
{
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_REPORTS;
    return make_matcher(new static_matcher_t<fail_t>());
}

_END_ABC_NS