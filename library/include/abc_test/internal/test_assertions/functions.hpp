#pragma once

#include "abc_test/internal/errors/test_assertion_exception.hpp"
#include "abc_test/internal/matchers/matcher_wrapper.hpp"
#include "abc_test/internal/test_assertions/test_block.hpp"
#include "abc_test/internal/test_reports/assertion.hpp"
#include "abc_test/internal/test_reports/assertion_status/fail.hpp"
#include "abc_test/internal/test_reports/assertion_status/pass.hpp"
#include "abc_test/internal/test_reports/assertion_status/pass_or_fail.hpp"
#include "abc_test/internal/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/internal/test_reports/assertion_status/terminate.hpp"
#include "abc_test/internal/test_reports/matcher_based_assertion_block.hpp"
#include "abc_test/internal/test_reports/matcher_based_assertion_single_line.hpp"
#include "abc_test/internal/test_reports/multi_element_assertion_block.hpp"
#include "abc_test/internal/test_reports/static_assertion.hpp"
#include "abc_test/internal/test_runner.hpp"
#include "abc_test/internal/utility/str/string_utility.hpp"

#include <concepts>


_BEGIN_ABC_NS
/*!
 * @brief Function for use with the _MATCHER macro. It adds a single_source_t to
 * the given object.
 * @tparam Has_Annotation Denotes whether the matcher_wrapper_t created and
 * taken as an argument is annotated.
 * @param _a_element The matcher_wrapper_t element to use.
 * @param _a_source The ds::single_source_t object.
 * @return A matcher_wrapper_t created using _a_element and _a_source.
 *
 * This is adhered to using a concept.
 */
template <bool Has_Annotation>
__constexpr _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>
            matcher_macro(
                const _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>& _a_element,
                const _ABC_NS_DS::single_source_t&                        _a_source
            ) noexcept;
/*!
 * @brief function to create an assertion.
 *
 * This function registers the exception with the test_runner_t, before
 * potentially terminating the test function.
 *
 * @tparamt T The assertion type. Must be derived from reports::dynamic_type_t.
 * @paramt Has_Annotation Whether the argument matcher has an annotation.
 * @param _a_matcher The matcher to test.
 * @param _a_source The source of the assertion.
 * @param _a_test_runner The test_runner_t to register the assertion with.
 * @return True if the assertion passed, false if it failed.
 */
template <typename T, bool Has_Annotation>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr bool
    create_assertion(
        const _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>& _a_matcher,
        const std::string_view _a_macro_str,
        const std::string_view _a_matcher_str,
        const std::source_location& _a_sl,
        test_runner_t&                                            _a_test_runner
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
/*!
 * @brief Creates a static assertion in the test function.
 *
 * @tparam T The assertion type. Must be derived from reports::static_status_t.
 * @param _a_str_to_print The optional annotation to attach to the assertion.
 * @param _a_source The source of the assertion.
 * @param _a_test_runner The test_runner_t object which the assertion is being
 * reported to.
 * @return True if the assertion is true. False otherwise.
 */
template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::static_status_t>
__constexpr bool
    create_static_assertion(
        const std::optional<std::string>&  _a_str_to_print,
        const _ABC_NS_DS::single_source_t& _a_source,
        test_runner_t&                     _a_test_runner
    ) noexcept(not std::same_as<T, _ABC_NS_REPORTS::terminate_t>);
/*!
 * @brief Creates an assertion block in the test. Should be used with the macros
 * described in abc_test/internal/test_assertions/macros.h.
 *
 * @tparam T The assertion status. Must be derived from
 * reports::dynamic_status_t.
 * @param _a_test_block The test_block object being used to create the
 * assertion.
 * @param _a_test_runner The test_runner_t used to report the test.
 */
template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr void
    create_assertion_block(
        const single_element_test_block_t& _a_test_block,
        test_runner_t&                     _a_test_runner,
        const std::string_view             _a_source_representation,
        const std::source_location&        _a_source_location
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr void
    create_assertion_block(
        const single_element_test_block_t& _a_test_block,
        test_runner_t&                     _a_test_runner
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp void
    create_assertion_block(
        const multi_element_test_block_t& _a_test_block,
        test_runner_t&                    _a_test_runner,
        const std::string_view            _a_source_representation,
        const std::source_location&       _a_source_location
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp void
    create_assertion_block(
        const multi_element_test_block_t& _a_test_block,
        test_runner_t&                    _a_test_runner
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);

namespace
{
template <typename T>
__constexpr bool
    return_result(const bool _a_pass)
        noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
} // namespace

_END_ABC_NS

_BEGIN_ABC_NS
template <bool Has_Annotation>
__constexpr_imp _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>
                matcher_macro(
                    const _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>& _a_element,
                    const _ABC_NS_DS::single_source_t&                        _a_source
                ) noexcept
{
    using namespace _ABC_NS_MATCHER;
    return matcher_wrapper_t<Has_Annotation>(_a_element, _a_source);
}

template<
    typename T, bool Has_Annotation
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
bool
create_assertion(
    const _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>& _a_matcher,
    const std::string_view _a_macro_str,
    const std::string_view _a_matcher_str,
    const std::source_location& _a_sl,
    test_runner_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    using namespace std;
    assertion_ptr_t<true, T> _l_gur;
    bool                     _l_passed{true};
    optional<string>         _l_matcher_annotation{};
    ds::single_source_t _l_source = _a_matcher.internal_matcher()->add_source_info(
        _a_macro_str, _a_matcher_str, _a_sl);
    if constexpr (Has_Annotation)
    {
        _l_matcher_annotation = optional<string>(_a_matcher.annotation());
    }
    if (not _a_matcher.has_matcher_base())
    {
        _l_gur = make_unique<matcher_based_assertion_single_line_t<T>>(
            _l_source,
            _a_test_runner.get_log_infos(false),
            matcher_result_with_annotation_and_source_info_t(
                matcher_result_t(),
                _l_source,
                _l_matcher_annotation,
                matcher_source_map_t()
            )
        );
        _a_test_runner.add_assertion_and_warning(
            _l_gur,
            "Matcher_t object has not been initialised. Assertion is set to "
            "true"
        );
    }
    else
    {
        matcher_base_t&      _l_matcher_base{_a_matcher.matcher_base()};
        matcher_result_t     _l_mr{_l_matcher_base.matcher_result()};
        matcher_source_map_t _l_msm;
        _l_matcher_base.gather_map_source(_l_msm);
        _l_passed = _l_mr.passed();
        _l_gur    = make_unique<const matcher_based_assertion_single_line_t<T>>(
            _l_source,
            _a_test_runner.get_log_infos(false),
            matcher_result_with_annotation_and_source_info_t(
                _l_mr,
                _l_source,
                _l_matcher_annotation,
                _l_msm
            )
        );
        _a_test_runner.add_assertion(_l_gur);
    }
    return return_result<T>(_l_passed);
}

template<
    typename T
>
    requires std::derived_from<T, _ABC_NS_REPORTS::static_status_t>
__constexpr_imp
bool
create_static_assertion(
    const std::optional<std::string>& _a_str_to_print,
    const _ABC_NS_DS::single_source_t& _a_source,
    test_runner_t& _a_test_runner
) noexcept(not std::same_as<T, _ABC_NS_REPORTS::terminate_t>)
{
    using namespace std;
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_ERRORS;
    assertion_ptr_t<true, T> _l_assertion{make_unique<static_assertion_t<T>>(
        _a_source, _a_test_runner.get_log_infos(false), _a_str_to_print
    )};
    _a_test_runner.add_assertion(_l_assertion);
    if constexpr (same_as<T, pass_t>)
    {
        return true;
    }
    else if constexpr (same_as<T, fail_t>)
    {
        return false;
    }
    else if constexpr (same_as<T, terminate_t>)
    {
        throw test_assertion_exception_t{};
    }
    else
    {
        __STATIC_ASSERT(T, "create_static_assertion_Failure")
    }
}

template<
    typename T
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
void
create_assertion_block(
    single_element_test_block_t& _a_test_block,
    test_runner_t& _a_test_runner,
    const std::string_view _a_source_representation,
    const std::source_location& _a_source_location
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    _a_test_block.register_end(_ABC_NS_DS::single_source_t(
        _a_source_representation, _a_source_location
    ));
    _a_test_block.set_processed();
    assertion_ptr_t<false, T> _l_gur;
    bool _l_passed{_a_test_block.get_matcher().matcher_result().passed()};
    matcher_result_with_annotation_and_source_info_t _l_mtr{
        _a_test_block.get_matcher()
    };
    _l_gur = make_unique<matcher_based_assertion_block_t<T>>(
        _l_passed,
        _a_test_block.source(),
        _a_test_runner.get_log_infos(false),
        _l_mtr,
        _a_test_block.test_annotation()
    );
    _a_test_runner.add_assertion(_l_gur);
    return_result<T>(_l_passed);
}

template<
    typename T
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
void
create_assertion_block(
    const single_element_test_block_t& _a_test_block,
    test_runner_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    assertion_ptr_t<false, T> _l_gur;
    bool _l_passed{_a_test_block.get_matcher().matcher_result().passed()};
    matcher_result_with_annotation_and_source_info_t _l_mtr{
        _a_test_block.get_matcher()
    };
    _l_gur = make_unique<matcher_based_assertion_block_t<T>>(
        _l_passed,
        _a_test_block.source(),
        _a_test_runner.get_log_infos(false),
        _l_mtr,
        _a_test_block.test_annotation()
    );
    _a_test_runner.add_assertion(_l_gur);
    return_result<T>(_l_passed);
}

template<
    typename T
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
void
create_assertion_block(
    multi_element_test_block_t& _a_test_block,
    test_runner_t& _a_test_runner,
    const std::string_view _a_source_representation,
    const std::source_location& _a_source_location
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    _a_test_block.register_end(_ABC_NS_DS::single_source_t(
        _a_source_representation, _a_source_location
    ));
    _a_test_block.set_processed();
    assertion_ptr_t<false, T> _l_gur;
    bool                      _l_passed{true};
    for (const matcher_result_with_annotation_and_source_info_t& _l_ki :
         _a_test_block.get_matcher())
    {
        if (not _l_ki.matcher_result().passed())
        {
            _l_passed = false;
        }
    }
    matcher_res_infos_t _l_mtr{_a_test_block.get_matcher()};
    _l_gur = make_unique<multi_element_assertion_block_t<T>>(
        _l_passed,
        _a_test_block.source(),
        _a_test_runner.get_log_infos(false),
        _l_mtr,
        _a_test_block.test_annotation()
    );
    _a_test_runner.add_assertion(_l_gur);
    return_result<T>(_l_passed);
}

template<
    typename T
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
void
create_assertion_block(
    const multi_element_test_block_t& _a_test_block,
    test_runner_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    assertion_ptr_t<false, T> _l_gur;
    bool                      _l_passed{true};
    matcher_res_infos_t       _l_mtr{_a_test_block.get_matcher()};
    _l_gur = make_unique<multi_element_assertion_block_t<T>>(
        _l_passed,
        _a_test_block.source(),
        _a_test_runner.get_log_infos(false),
        _l_mtr,
        _a_test_block.test_annotation()
    );
    _a_test_runner.add_assertion(_l_gur);
    return_result<T>(_l_passed);
}

template<
    typename T
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
void
matcher_based_assertion_block(
    const _ABC_NS_MATCHER::matcher_wrapper_t<false>& _a_matcher,
    const std::optional<std::string_view>& _a_str_to_print,
    const _ABC_NS_DS::single_source_t& _a_source,
    test_runner_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    const assertion_t<true, T>* _l_gur;
    bool                        _l_passed{true};
    if (not _a_matcher.has_matcher_base())
    {
        _l_gur = new matcher_based_assertion_block_t<T>(
            _a_source,
            _a_test_runner.get_log_infos(false),
            _a_str_to_print,
            matcher_result_t(),
            matcher_source_map_t()
        );
        _a_test_runner.add_assertion_and_warning(
            _l_gur,
            "Matcher_t object has not been initialised. Assertion is set to "
            "true"
        );
    }
    else
    {
        const matcher_base_t& _l_matcher_base{_a_matcher.matcher_base()};
        matcher_result_t      _l_mr{_l_matcher_base.matcher_result()};
        matcher_source_map_t  _l_msm;
        _l_matcher_base.gather_map_source(_l_msm);
        _l_passed = _l_mr.passed();
        _l_gur    = new assertion_t<T>(
            _a_source,
            _a_test_runner.get_log_infos(false),
            _a_str_to_print,
            _l_mr,
            _l_msm
        );
        _a_test_runner.add_assertion(_l_gur);
    }
    return return_result<T>(_l_passed);
}

namespace
{
template<
        typename T
    >
    __constexpr_imp
        bool
        return_result(
            const bool _a_pass
        ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace std;
    using namespace _ABC_NS_ERRORS;
    using namespace _ABC_NS_REPORTS;
    if constexpr (same_as<T, pass_or_terminate_t>)
    {
        if (not _a_pass)
        {
            throw test_assertion_exception_t{};
        }
        return _a_pass;
    }
    else if constexpr (same_as<T, pass_or_fail_t>)
    {
        return _a_pass;
    }
    else
    {
        __STATIC_ASSERT(T, "return result failure");
    }
}
} // namespace

_END_ABC_NS