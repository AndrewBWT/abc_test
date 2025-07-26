#include "abc_test/core/ds/test_data/invoked_test_data.hpp"
#include "abc_test/core/options/test_options_base.hpp"

_BEGIN_ABC_DS_NS
__no_constexpr_or_inline_imp bool get_global_retain_passed_assertions() noexcept
{
    return global::get_this_threads_test_options().individual_test_options.retain_passed_assertions;
}
_END_ABC_DS_NS