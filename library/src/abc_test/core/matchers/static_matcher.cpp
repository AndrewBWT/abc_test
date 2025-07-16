#include "abc_test/core/matchers/static_matcher.hpp"

#include "abc_test/core/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_NS
__no_constexpr_or_inline_imp matcher_t
    true_matcher() noexcept
{
    return true_matcher(u8"true");
}
__no_constexpr_or_inline_imp matcher_t
true_matcher(const std::u8string_view _a_msg) noexcept
{
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_REPORTS;
    using namespace std;
    return mk_matcher_using_result(mk_matcher_result<pass_t>(_a_msg));
}

__no_constexpr_or_inline_imp matcher_t
    false_matcher() noexcept
{
    return false_matcher(u8"false");
}

__no_constexpr_or_inline matcher_t
false_matcher(const std::u8string_view _a_msg) noexcept
{
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_REPORTS;
    using namespace std;
    return mk_matcher_using_result(mk_matcher_result<fail_t>(_a_msg));
}

_END_ABC_NS