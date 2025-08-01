#pragma once

#include "abc_test/core/errors/test_assertion_exception.hpp"
#include "abc_test/core/matchers/matcher_wrapper.hpp"
#include "abc_test/core/matchers/simulated_logic_expr.hpp"
#include "abc_test/core/test_assertions/test_block.hpp"
#include "abc_test/core/test_reports/assertion.hpp"
#include "abc_test/core/test_reports/assertion_status/fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/core/test_reports/assertion_status/terminate.hpp"
#include "abc_test/core/test_reports/matcher_based_assertion_block.hpp"
#include "abc_test/core/test_reports/matcher_based_assertion_single_line.hpp"
#include "abc_test/core/test_reports/multi_element_assertion_block.hpp"
#include "abc_test/core/test_reports/static_assertion.hpp"
#include "abc_test/core/test_evaluator.hpp"
#include "abc_test/utility/str/string_utils.hpp"

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
                const std::string_view                                    _a_macro_str,
                const std::string_view      _a_matcher_str,
                const std::source_location& _a_sl
            ) noexcept;
template <_ABC_NS_MATCHER::logic_enum_t Logic_Enum>
__constexpr _ABC_NS_MATCHER::matcher_wrapper_t<false>
            matcher_macro(
                const _ABC_NS_MATCHER::simulated_logic_expr_t<Logic_Enum>& _a_element,
                const std::string_view                                     _a_macro_str,
                const std::string_view      _a_matcher_str,
                const std::source_location& _a_sl
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
        _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>& _a_matcher,
        const std::string_view                              _a_macro_str,
        const std::string_view                              _a_matcher_str,
        const std::source_location&                         _a_sl,
        test_evaluator_t&                                      _a_test_runner
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
template <typename T, bool Has_Annotation>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr bool
    create_assertion(
        _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>&& _a_matcher,
        const std::string_view                               _a_macro_str,
        const std::string_view                               _a_matcher_str,
        const std::source_location&                          _a_sl,
        test_evaluator_t&                                       _a_test_runner
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
template <typename T, _ABC_NS_MATCHER::logic_enum_t Logic_Enum>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr bool
    create_assertion(
        _ABC_NS_MATCHER::simulated_logic_expr_t<Logic_Enum>& _a_matcher,
        const std::string_view                               _a_macro_str,
        const std::string_view                               _a_matcher_str,
        const std::source_location&                          _a_sl,
        test_evaluator_t&                                       _a_test_runner
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
        test_evaluator_t&                     _a_test_runner
    ) noexcept(not std::same_as<T, _ABC_NS_REPORTS::terminate_t>);
template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp void
    create_assertion_block(
        const multi_element_test_block_t& _a_test_block,
        test_evaluator_t&                    _a_test_runner,
        const std::string_view            _a_source_representation,
        const std::source_location&       _a_source_location
    ) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>);
template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp void
    create_assertion_block(
        const multi_element_test_block_t& _a_test_block,
        test_evaluator_t&                    _a_test_runner
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
                    const std::string_view                                    _a_macro_str,
                    const std::string_view      _a_matcher_str,
                    const std::source_location& _a_sl
                ) noexcept
{
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_from_MATCHER_macro<Has_Annotation>(
        _a_element, _a_macro_str, _a_matcher_str, _a_sl
    );
}

template <_ABC_NS_MATCHER::logic_enum_t Logic_Enum>
__constexpr_imp _ABC_NS_MATCHER::matcher_wrapper_t<false>
                matcher_macro(
                    const _ABC_NS_MATCHER::simulated_logic_expr_t<Logic_Enum>& _a_element,
                    const std::string_view                                     _a_macro_str,
                    const std::string_view      _a_matcher_str,
                    const std::source_location& _a_sl
                ) noexcept
{
    using namespace _ABC_NS_MATCHER;
    matcher_wrapper_t<false> _l_matcher(_a_element.matcher());
    _l_matcher.add_source_info(_a_macro_str, _a_matcher_str, _a_sl);
    return _l_matcher;
}

template<
    typename T, bool Has_Annotation
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
bool
create_assertion(
    _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>& _a_matcher,
    const std::string_view _a_macro_str,
    const std::string_view _a_matcher_str,
    const std::source_location& _a_sl,
    test_evaluator_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    using namespace std;
    assertion_ptr_t<true, T> _l_gur;
    bool                     _l_passed{true};
    optional<u8string>       _l_matcher_annotation{};
    ds::single_source_t      _l_source
        = _a_matcher.add_source_info(_a_macro_str, _a_matcher_str, _a_sl);
    if constexpr (Has_Annotation)
    {
        _l_matcher_annotation = optional<u8string>(_a_matcher.annotation());
    }
    matcher_result_t     _l_mr{_a_matcher.matcher_result()};
    matcher_source_map_t _l_msm;
    _a_matcher.gather_map_source(_l_msm);
    _l_passed = _l_mr.passed();
    auto ki   = _a_test_runner.get_log_infos(false);
    _l_gur    = make_unique<const matcher_based_assertion_single_line_t<T>>(
        _l_source,
        _a_test_runner.get_log_infos(false),
        bba_inner_assertion_type_t(
            ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
              || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
            _l_mr,
            ki,
            _l_source,
            _l_matcher_annotation,
            _l_msm
        ),
        _a_test_runner.current_assertion_index()
    );
    _a_test_runner.add_assertion(_l_gur);
    _a_matcher.remove_primary_source();
    return return_result<T>(_l_passed);
}

template<
    typename T, bool Has_Annotation
>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp
bool
create_assertion(
    _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>&& _a_matcher,
    const std::string_view _a_macro_str,
    const std::string_view _a_matcher_str,
    const std::source_location& _a_sl,
    test_evaluator_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    using namespace std;
    assertion_ptr_t<true, T> _l_gur;
    bool                     _l_passed{true};
    optional<u8string>       _l_matcher_annotation{};
    ds::single_source_t      _l_source
        = _a_matcher.add_source_info(_a_macro_str, _a_matcher_str, _a_sl);
    if constexpr (Has_Annotation)
    {
        _l_matcher_annotation = optional<u8string>(_a_matcher.annotation());
    }
    matcher_result_t     _l_mr{_a_matcher.matcher_result()};
    matcher_source_map_t _l_msm;
    _a_matcher.gather_map_source(_l_msm);
    _l_passed = _l_mr.passed();
    auto ki   = _a_test_runner.get_log_infos(false);
    _l_gur    = make_unique<const matcher_based_assertion_single_line_t<T>>(
        _l_source,
        _a_test_runner.get_log_infos(false),
        bba_inner_assertion_type_t(
            ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
              || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
            _l_mr,
            ki,
            _l_source,
            _l_matcher_annotation,
            _l_msm
        ),
        _a_test_runner.current_assertion_index()
    );
    _a_test_runner.add_assertion(_l_gur);
    _a_matcher.remove_primary_source();
    return return_result<T>(_l_passed);
}

template <typename T, _ABC_NS_MATCHER::logic_enum_t Logic_Enum>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp bool
create_assertion(
    _ABC_NS_MATCHER::simulated_logic_expr_t<Logic_Enum>& _a_matcher,
    const std::string_view                               _a_macro_str,
    const std::string_view                               _a_matcher_str,
    const std::source_location& _a_sl,
    test_evaluator_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_MATCHER;
    using namespace std;
    return create_assertion<T, false>(
        _a_matcher.matcher(),
        _a_macro_str,
        _a_matcher_str,
        _a_sl,
        _a_test_runner
    );
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
    test_evaluator_t& _a_test_runner
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
    multi_element_test_block_t& _a_test_block,
    test_evaluator_t& _a_test_runner,
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
    for (const bba_inner_assertion_type_t& _l_ki : _a_test_block.get_matcher())
    {
        if (not _l_ki.matcher_result().passed())
        {
            _l_passed = false;
        }
    }
    if (_a_test_block.get_matcher().size() == 0)
    {
        _l_passed = false;
    }
    matcher_res_infos_t _l_mtr{_a_test_block.get_matcher()};
    _l_gur = make_unique<multi_element_assertion_block_t<T>>(
        _l_passed,
        _a_test_block.source(),
        _a_test_runner.get_log_infos(false),
        _l_mtr,
        _a_test_block.test_annotation(),
        _a_test_runner.current_assertion_index()
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
    test_evaluator_t& _a_test_runner
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
    test_evaluator_t& _a_test_runner
) noexcept(std::same_as<T, _ABC_NS_REPORTS::pass_or_fail_t>)
{
    using namespace _ABC_NS_REPORTS;
    using namespace _ABC_NS_MATCHER;
    const assertion_t<true, T>* _l_gur;
    bool                        _l_passed{true};
    matcher_result_t            _l_mr{_a_matcher.matcher_result()};
    matcher_source_map_t        _l_msm;
    _a_matcher.gather_map_source(_l_msm);
    _l_passed = _l_mr.passed();
    _l_gur    = new assertion_t<true, T>(
        _a_source,
        _a_test_runner.get_log_infos(false),
        _a_str_to_print,
        _l_mr,
        _l_msm
    );
    _a_test_runner.add_assertion(_l_gur);
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

/*template <typename T, bool Has_Annotation>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
struct MacroAProxy
{
    abc::matcher::matcher_wrapper_t<Has_Annotation> _m_matcher;
    std::source_location                            _m_source;
    std::string                                     _m_matcher_str;
    std::string                                     _m_macro_str;

    MacroAProxy(
        const abc::matcher::matcher_wrapper_t<Has_Annotation>& _a_matcher,
        const std::source_location&                            _a_source,
        const std::string_view                                 _a_matcher_str,
        const std::string_view                                 _a_macro_str
    )
        : _m_matcher(_a_matcher), _m_source(_a_source), _m_macro_str(_a_macro_str), _m_matcher_str(_a_matcher_str)
    {}
    ~MacroAProxy()
    {
        abc::create_assertion<T>(
            _m_matcher,
            _m_macro_str,
            _m_matcher_str,
            _m_source,
            _ABC_NS_GLOBAL::get_this_threads_test_runner_ref()
        );
    }

    // Implicit conversion to multi_matcher_t
    operator assertion_wp_t<T>() const
    {
        return abc::make_entity_bba_compatable<T>(
            _m_matcher, _m_macro_str, _m_source
        );
    }

    std::string
        info() const
    {
        return "added via _MACRO_A";
    }
};
template<typename T, bool Has_Annotation>
static auto make_macroAProxy(
    const abc::matcher::matcher_wrapper_t<Has_Annotation>& _a_matcher,
    const std::source_location& _a_source,
    const std::string_view                                 _a_matcher_str,
    const std::string_view                                 _a_macro_str
) -> MacroAProxy<T, Has_Annotation>
{
    return MacroAProxy<T,Has_Annotation>(_a_matcher, _a_source, _a_matcher_str, _a_macro_str);
}
template <typename T, bool Has_Annotation>
requires std::derived_from<T, _ABC_NS_REPORTS::static_status_t>
struct MacroAProxy2
{
    std::optional<std::string> _m_msg;
    ds::single_source_t        _m_source;

    MacroAProxy2(
        const std::optional<std::string>& _a_msg,
        const ds::single_source_t&        _a_source
    )
        : _m_msg(_a_msg), _m_source(_a_source)
    {}

    ~MacroAProxy2()
    {
        create_static_assertion<T>(
            _m_msg,
            _m_source,
            _ABC_NS_GLOBAL::get_this_threads_test_runner_ref()
        );
    }

    // Implicit conversion to multi_matcher_t
    operator assertion_wp_t<T>() const
    {
        return make_entity_bba_compatable<T>(
            _m_msg,
            _m_source.source_code_representation(),
            _m_source.source_location()
        );
    }

    std::string
        info() const
    {
        return "added via _MACRO_A";
    }
};*/

_END_ABC_NS