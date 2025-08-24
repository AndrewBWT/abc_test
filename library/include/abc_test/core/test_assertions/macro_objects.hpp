#pragma once
#include "abc_test/core/matchers/matcher_wrapper.hpp"
#include "abc_test/core/test_reports/assertion_status/dynamic_status.hpp"
#include "abc_test/core/test_reports/matcher_based_assertion_single_line.hpp"
#include "abc_test/core/test_reports/static_assertion.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include <concepts>
#include <source_location>

_BEGIN_ABC_NS

template <typename T, bool Has_Annotation>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
struct MacroAProxy
{
    abc::matcher::matcher_wrapper_t<Has_Annotation> _m_matcher;
    std::source_location                            _m_source;
    std::string                                     _m_matcher_str;
    std::string                                     _m_macro_str;
    bool _m_data_moved_to_multi_matcher;

    MacroAProxy(
        const abc::matcher::matcher_wrapper_t<Has_Annotation>& _a_matcher,
        const std::source_location&                            _a_source,
        const std::string_view                                 _a_matcher_str,
        const std::string_view                                 _a_macro_str
    )
        : _m_matcher(_a_matcher)
        , _m_source(_a_source)
        , _m_macro_str(_a_macro_str)
        , _m_matcher_str(_a_matcher_str)
        , _m_data_moved_to_multi_matcher(false)
    {}

    inline ~MacroAProxy()
    {
        using namespace _ABC_NS_REPORTS;
        using namespace _ABC_NS_MATCHER;
        using namespace std;
        if (not _m_data_moved_to_multi_matcher)
        {
            assertion_ptr_t<true, T> _l_gur;
            bool                     _l_passed{true};
            optional<u8string>       _l_matcher_annotation{};
            ds::single_source_t      _l_source = _m_matcher.add_source_info(
                _m_macro_str, _m_matcher_str, _m_source
            );
            if constexpr (Has_Annotation)
            {
                _l_matcher_annotation
                    = optional<u8string>(_m_matcher.annotation());
            }
            matcher_result_t     _l_mr{_m_matcher.matcher_result()};
            matcher_source_map_t _l_msm;
            _m_matcher.gather_map_source(_l_msm);
            _l_passed = _l_mr.passed();
            auto _l_test_runner{
                _ABC_NS_GLOBAL::get_this_threads_test_evaluator_ref()
            };
            auto ki = _l_test_runner.get_log_infos(false);
            _l_gur
                = make_unique<const matcher_based_assertion_single_line_t<T>>(
                    _l_source,
                    _l_test_runner.get_log_infos(false),
                    bba_inner_assertion_type_t(
                        ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
                          || std::same_as<
                              T,
                              _ABC_NS_REPORTS::pass_or_terminate_t> ),
                        _l_mr,
                        ki,
                        _l_source,
                        _l_matcher_annotation,
                        _l_msm
                    ),
                    _l_test_runner.current_assertion_index()
                );
            _l_test_runner.add_assertion(_l_gur);
            _m_matcher.remove_primary_source();
            //return_result<T>(_l_passed);
        }
    }

    inline _ABC_NS_MATCHER::bba_inner_assertion_type_t
        make_bba_inner_assertion_type()
    {
        _m_data_moved_to_multi_matcher = true;
        using namespace _ABC_NS_MATCHER;
        matcher_source_map_t _l_msm;
        _m_matcher.gather_map_source(_l_msm);
        global::get_this_threads_current_test()
            .add_current_for_loop_stack_to_trie(
                not _m_matcher.matcher_result().passed()
            );
        auto ki
            = global::get_this_threads_test_evaluator_ref().get_log_infos(false
            );
        if constexpr (Has_Annotation)
        {
            bba_inner_assertion_type_t _l_tuple(
                ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
                  || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
                _m_matcher.matcher_result(),
                ki,
                std::optional<ds::single_source_t>{},
                std::optional<std::u8string>(_m_matcher.annotation()),
                _l_msm
            );
            return _l_tuple;
        }
        else
        {
            bba_inner_assertion_type_t _l_tuple(
                ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
                  || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
                _m_matcher.matcher_result(),
                ki,
                std::optional<ds::single_source_t>{},
                std::optional<std::u8string>{},
                _l_msm
            );
            return _l_tuple;
        }
    }
};

template <typename T, bool Has_Annotation>
static auto
    make_macroAProxy(
        const abc::matcher::matcher_wrapper_t<Has_Annotation>& _a_matcher,
        const std::source_location&                            _a_source,
        const std::string_view                                 _a_matcher_str,
        const std::string_view                                 _a_macro_str
    ) -> MacroAProxy<T, Has_Annotation>
{
    return MacroAProxy<T, Has_Annotation>(
        _a_matcher, _a_source, _a_matcher_str, _a_macro_str
    );
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
        using namespace std;
        using namespace _ABC_NS_REPORTS;
        using namespace _ABC_NS_ERRORS;
        auto _l_test_runner{_ABC_NS_GLOBAL::get_this_threads_test_evaluator_ref(
        )};
        assertion_ptr_t<true, T> _l_assertion{
            make_unique<static_assertion_t<T>>(
                _m_source, _l_test_runner.get_log_infos(false), _m_msg
            )
        };
        _l_test_runner.add_assertion(_l_assertion);
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

    inline
        operator _ABC_NS_MATCHER::bba_inner_assertion_type_t() const
    {
        using namespace _ABC_NS_MATCHER;
        matcher_source_map_t _l_msm;
        const bool           _l_pass{std::same_as<T, _ABC_NS_REPORTS::pass_t>};
        global::get_this_threads_current_test()
            .add_current_for_loop_stack_to_trie(not _l_pass);
        auto ki
            = global::get_this_threads_test_evaluator_ref().get_log_infos(false
            );
        bba_inner_assertion_type_t _l_tuple(
            ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
              || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
            _l_pass,
            _m_msg,
            ki,
            _m_source
        );
        return _l_tuple;
    }

    // Implicit conversion to multi_matcher_t
    /*operator assertion_wp_t<T>() const
    {
        using namespace _ABC_NS_MATCHER;
        matcher_source_map_t _l_msm;
        const bool           _l_pass{ std::same_as<T, _ABC_NS_REPORTS::pass_t>
    };
        global::get_this_threads_current_test().add_current_for_loop_stack_to_trie(
            not _l_pass
        );
        auto ki =
    global::get_this_threads_test_runner_ref().get_log_infos(false);
        bba_inner_assertion_type_t _l_tuple(
            (std::same_as<T, _ABC_NS_REPORTS::terminate_t>
                || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t>),
            _l_pass,
            _a_opt_str_msg,
            ki,
            ds::single_source_t(_a_str_representation, _a_source_location)
        );
        return assertion_wp_t<T>{_l_tuple};
    }*/
};

_END_ABC_NS