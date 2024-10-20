#pragma once
#include "abc_test/core/matchers/matcher_base.h"
#include "abc_test/core/test_reports/assertion_status/fail.h"
#include "abc_test/core/test_reports/assertion_status/pass.h"

_BEGIN_ABC_NS
// Forward declarations
template <bool Is_Annotated>
struct matcher_wrapper_t;
using matcher_t = matcher_wrapper_t<false>;

/*!
 * @brief Matcher object which creates the matcher at compile time.
 *
 * It can either be instnatiated with reports::pass_t or reports::fail_t,
 * representing a passing and failing matcher respectively.
 * @tparam Assertion_Status The type parameter used to representing the internal
 * matcher's behaviour. This can be either reports::pass_t or reports::fail_t.
 */
template <typename Assertion_Status>
requires std::same_as<Assertion_Status, reports::pass_t>
         || std::same_as<Assertion_Status, reports::fail_t>
class static_matcher_t : public matcher_base_t
{
public:
    /*!
     * @brief Constructor.
     */
    __constexpr
        static_matcher_t() noexcept = default;
private:
    __constexpr virtual matcher_result_t
        run(test_runner_t& _a_test_runner) final override;
};

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

_BEGIN_ABC_NS
template <typename Assertion_Status>
requires std::same_as<Assertion_Status, reports::pass_t>
         || std::same_as<Assertion_Status, reports::fail_t>
__constexpr_imp matcher_result_t
    static_matcher_t<Assertion_Status>::run(
        test_runner_t& _a_test_runner
    )
{
    using namespace reports;
    if constexpr (std::same_as<Assertion_Status, pass_t>)
    {
        return matcher_result_t(true, true, "true");
    }
    else if constexpr (std::same_as<Assertion_Status, fail_t>)
    {
        return matcher_result_t(true, false, "false");
    }
    else
    {
        __STATIC_ASSERT(
            Assertion_Status, "Cannot instnatiate run method for static_matcher"
        );
    }
}

_END_ABC_NS