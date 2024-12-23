#include "abc_test/internal/matchers/static_matcher.hpp"

#include "abc_test/internal/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_NS
__no_constexpr_or_inline_imp matcher_t
    true_matcher() noexcept
{
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_REPORTS;
    using namespace std;
    return mk_matcher_using_result(mk_matcher_result<pass_t>());
}

__no_constexpr_or_inline_imp matcher_t
    false_matcher() noexcept
{
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_REPORTS;
    using namespace std;
    return mk_matcher_using_result(mk_matcher_result<fail_t>());
}

_END_ABC_NS