#include "abc_test/core/global.hpp"

#include "abc_test/core/global/test_framework_global_variable_set.hpp"
#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/core/test_evaluator.hpp"

#include <variant>

// Implementation
_BEGIN_ABC_GLOBAL_NS

__no_constexpr_or_inline_imp const test_framework_global_variable_set_t&
    push_global_variable_set(
        const test_options_base_t&            _a_options,
        const _ABC_NS_REPORTERS::error_reporters_t& _a_error_reporters,
        const _ABC_NS_REPORTERS::test_reporters_t&  _a_test_reporters
    ) noexcept
{
    using namespace reporters;
    using namespace std;
    list<test_framework_global_variable_set_t>& _l_tfgvs{
        detail::get_inner_global_variable_set()
    };
    _l_tfgvs.emplace_back(_a_options, _a_error_reporters, _a_test_reporters);
    return _l_tfgvs.back();
}

__no_constexpr_or_inline_imp void
    pop_global_variable_set() noexcept
{
    using namespace reporters;
    using namespace std;
    detail::get_inner_global_variable_set().pop_back();
}

__no_constexpr_or_inline_imp reporters::test_reporter_controller_t&
                             get_global_test_reporter_controller() noexcept
{
    return detail::get_inner_global_variable_set()
        .back()
        .test_reporter_controller();
}

__no_constexpr_or_inline_imp test_evaluator_t&
    get_this_threads_test_evaluator_ref() noexcept
{
    return *detail::get_inner_threads_test_evaluator_set().back();
}

__no_constexpr_or_inline_imp void
    push_this_threads_test_runner(
        test_evaluator_t* _a_test_runner_t
    ) noexcept
{
    detail::get_inner_threads_test_evaluator_set().push_back(_a_test_runner_t);
}

__no_constexpr_or_inline_imp void
    pop_this_threads_test_runner() noexcept
{
    detail::get_inner_threads_test_evaluator_set().pop_back();
}

__no_constexpr_or_inline_imp const test_options_base_t&
    get_global_test_options() noexcept
{
    return detail::get_inner_global_variable_set().back().test_options();
}

__no_constexpr_or_inline_imp ds::invoked_test_data_t&
                             get_this_threads_current_test()
{
    using namespace errors;
    test_evaluator_t& _l_tr{get_this_threads_test_evaluator_ref()};
    return _l_tr.current_test();
}

__no_constexpr_or_inline_imp const test_options_base_t*
    get_global_test_options_ptr() noexcept
{
    return &get_global_test_options();
}

__no_constexpr_or_inline_imp reporters::error_reporter_controller_t&
                             get_global_error_reporter_controller() noexcept
{
    return detail::get_inner_global_variable_set()
        .back()
        .error_reporter_controller();
}

namespace detail
{
__no_constexpr_or_inline_imp std::list<test_framework_global_variable_set_t>&
                             get_inner_global_variable_set() noexcept
{
    static std::list<test_framework_global_variable_set_t> _s_tfgvs{};
    return _s_tfgvs;
}

__no_constexpr_or_inline_imp std::list<test_evaluator_t*>&
                             get_inner_threads_test_evaluator_set() noexcept
{
    thread_local std::list<test_evaluator_t*> _s_test_evaluators{};
    return _s_test_evaluators;
}
} // namespace detail

__no_constexpr_or_inline_imp const ds::test_list_t&
                                   get_global_test_list() noexcept
{
    return detail::get_mutable_test_list();
}

__no_constexpr_or_inline_imp std::size_t
                             add_test(
                                 const ds::test_list_element_t& _a_test_list_internal
                             ) noexcept
{
    using namespace ds;
    test_list_t& _l_test_list{detail::get_mutable_test_list()};
    _l_test_list.push_back(_a_test_list_internal);
    return 0;
}

__no_constexpr_or_inline_imp void
    clear_test_list() noexcept
{
    using namespace ds;
    test_list_t& _l_test_list{detail::get_mutable_test_list()};
    _l_test_list.clear();
}

__no_constexpr_or_inline_imp const utility::complete_global_seed_t&
                                   get_global_seed()
{
    return detail::get_inner_global_variable_set().back().global_seed();
}

__no_constexpr_or_inline_imp _ABC_NS_UTILITY::volatilte_volatile_void_ptr_t&
                             get_threads_volatie_ptr_ref()
{
    using namespace _ABC_NS_UTILITY;
    thread_local volatilte_volatile_void_ptr_t _tl_tr{nullptr};
    return _tl_tr;
}

namespace detail
{

__no_constexpr_or_inline_imp ds::test_list_t&
                             get_mutable_test_list() noexcept
{
    using namespace ds;
    static test_list_t _static_test_list;
    return _static_test_list;
}
} // namespace detail

_END_ABC_GLOBAL_NS