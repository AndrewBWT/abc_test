#include "abc_test/core/global.hpp"

#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/core/test_runner.hpp"

#include <variant>

// Implementation
_BEGIN_ABC_GLOBAL_NS
__no_constexpr_or_inline_imp test_options_base_t&
    setup_global_variables(
        const test_options_base_t& _a_options
    ) noexcept
{
    using namespace reporters;
    test_options_base_t& _l_to{get_inner_global_test_options()};
    _l_to = _a_options;
    return _l_to;
}

__no_constexpr_or_inline_imp reporters::test_reporter_controller_t&
                             get_global_test_reporter_controller() noexcept
{
    using namespace reporters;
    static test_reporter_controller_t _s_gtrc;
    return _s_gtrc;
}

__no_constexpr_or_inline_imp test_runner_t&
    get_this_threads_test_runner_ref() noexcept
{
    return *get_this_threads_test_runner_ptr();
}

__no_constexpr_or_inline_imp void
    set_this_threads_test_runner(
        test_runner_t* _a_test_runner_t
    ) noexcept
{
    test_runner_t*& _l_tr{get_this_threads_test_runner_ptr()};
    _l_tr = _a_test_runner_t;
}

__no_constexpr_or_inline_imp const test_options_base_t&
    get_global_test_options() noexcept
{
    return get_inner_global_test_options();
}

__no_constexpr_or_inline_imp test_runner_t*&
    get_this_threads_test_runner_ptr() noexcept
{
    thread_local test_runner_t* _tl_tr = nullptr;
    return _tl_tr;
}

__no_constexpr_or_inline_imp ds::invoked_test_data_t&
                             get_this_threads_current_test()
{
    using namespace errors;
    test_runner_t* _l_tr{get_this_threads_test_runner_ptr()};
    if (_l_tr == nullptr)
    {
        throw test_library_exception_t(
            u8"This threads test_runner_t is a nullptr"
        );
    }
    else
    {
        return _l_tr->current_test();
    }
}

__no_constexpr_or_inline_imp const test_options_base_t*
    get_global_test_options_ptr() noexcept
{
    return &get_global_test_options();
}

__no_constexpr_or_inline_imp reporters::error_reporter_controller_t&
                             get_global_error_reporter_controller() noexcept
{
    return get_inner_global_error_reporter_controller();
}

namespace
{
__no_constexpr_or_inline_imp test_options_base_t&
    get_inner_global_test_options() noexcept
{
    static test_options_base_t _s_to;
    return _s_to;
}

__no_constexpr_or_inline_imp reporters::error_reporter_controller_t&
    get_inner_global_error_reporter_controller() noexcept
{
    using namespace reporters;
    static error_reporter_controller_t _s_erc;
    return _s_erc;
}

__no_constexpr_or_inline_imp utility::complete_global_seed_t&
                             get_inner_global_seed() noexcept
{
    using namespace utility;
    static complete_global_seed_t _s_complete_global_seed;
    return _s_complete_global_seed;
}
} // namespace

__no_constexpr_or_inline_imp const ds::test_list_t&
                                   get_global_test_list() noexcept
{
    return get_mutable_test_list();
}

__no_constexpr_or_inline_imp std::size_t
                             add_test(
                                 const ds::test_list_element_t& _a_test_list_internal
                             ) noexcept
{
    using namespace ds;
    test_list_t& _l_test_list{get_mutable_test_list()};
    _l_test_list.push_back(_a_test_list_internal);
    return 0;
}

__no_constexpr_or_inline_imp void
    clear_test_list() noexcept
{
    using namespace ds;
    test_list_t& _l_test_list{get_mutable_test_list()};
    _l_test_list.clear();
}

__no_constexpr_or_inline_imp void
    set_global_seed()
{
    using namespace std;
    using namespace utility;
    complete_global_seed_t& _l_complete_global_seed{get_inner_global_seed()};
    get_inner_global_seed() = set_complete_global_seed(global::get_global_test_options().global_seed);
}

__no_constexpr_or_inline_imp const utility::complete_global_seed_t&
                                   get_global_seed()
{
    return get_inner_global_seed();
}

__no_constexpr_or_inline_imp _ABC_NS_UTILITY::volatilte_volatile_void_ptr_t&
                             get_threads_volatie_ptr_ref()
{
    using namespace _ABC_NS_UTILITY;
    thread_local volatilte_volatile_void_ptr_t _tl_tr{nullptr};
    return _tl_tr;
}

namespace
{

__no_constexpr_or_inline_imp ds::test_list_t&
                             get_mutable_test_list() noexcept
{
    using namespace ds;
    static test_list_t _static_test_list;
    return _static_test_list;
}
} // namespace

_END_ABC_GLOBAL_NS