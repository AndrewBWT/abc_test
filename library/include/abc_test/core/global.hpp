#pragma once

#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/utility/rng.hpp"

_BEGIN_ABC_DS_NS
struct registered_test_data_t;
struct invoked_test_data_t;
_END_ABC_DS_NS
_BEGIN_ABC_NS
struct test_runner_t;
_END_ABC_NS

_BEGIN_ABC_REPORTERS_NS
struct test_reporter_controller_t;
struct error_reporter_controller_t;
_END_ABC_REPORTERS_NS

/*!
 * This file contains all of the functions which access and modify global
 * variables in the abc_test framework.
 *
 * Unless you are building ontop of abc_test, it is any users of abc_test will
 * need any of the functions in this file.
 *
 * In regards to why we use global variables, there are two distinct "barriers"
 * in this abc_test; the test code written by users appears to not need any
 * references to the testing library. In actual fact, for many of the features
 * to work, it needs extensive access to the mechanisms in abc_test. The
 * functions in this file are used to access parts of the testing library that
 * would otherwise be inaccessable.
 *
 */
// Definitions
_BEGIN_ABC_GLOBAL_NS
/*!
 * Sets up the global test options (GTO). It also links the
 * global error_reporter_controller_t to the GTO.
 *
 */
__no_constexpr_or_inline test_options_base_t&
    setup_global_variables(const test_options_base_t& _a_options) noexcept;
/*!
 * Gets a reference to the global test_reporter_controller_t object.
 *
 */
__no_constexpr_or_inline reporters::test_reporter_controller_t&
                         get_global_test_reporter_controller() noexcept;
/*!
 * Gets a reference to this threads test_runner_t object.
 *
 */
__no_constexpr_or_inline test_runner_t&
    get_this_threads_test_runner_ref() noexcept;
__no_constexpr_or_inline void
    set_this_threads_test_runner(test_runner_t* _a_test_runner_t) noexcept;
/*!
 * Gets a const reference to the global test_options_base_t object.
 *
 */
__no_constexpr_or_inline const test_options_base_t&
    get_global_test_options() noexcept;
/*!
 * Gets a pointer to this threads test_runner_t object.
 *
 * This can be useful when initialising a pointer which is part of an object.
 * The default constructor could then use nullptr, whereas if it was a reference
 * this wouldn't be possible.
 *
 */
__no_constexpr_or_inline test_runner_t*&
    get_this_threads_test_runner_ptr() noexcept;
/*!
 * Gets a reference to this threads current invoked_test_info. That is, the
 * current test that is running.
 *
 * TODO: Currently this thread (assuming its not a nullptr) has a current
 * element which is set at the default constructor. If we want to remove the
 * default constructor, and only use elements created on the heap, this function
 * would need to be changed.
 *
 */
__no_constexpr_or_inline ds::invoked_test_data_t&
                         get_this_threads_current_test();
__no_constexpr_or_inline reporters::error_reporter_controller_t&
                         get_global_error_reporter_controller() noexcept;
/*!
 * Get the global test_list.
 */
// Can't be constexpr due to use of calling et_mutable_test_list
__no_constexpr_or_inline const ds::test_list_t&
                               get_global_test_list() noexcept;
/*!
 * Add a test to the global test_list.
 */
// Can't be constexpr due to use of calling et_mutable_test_list
__no_constexpr_or_inline std::size_t
    add_test(const ds::test_list_element_t& _a_test_list_internal) noexcept;
/*!
 * Clear the global test_list.
 */
// Can't be constexpr due to use of calling et_mutable_test_list
__no_constexpr_or_inline void
    clear_test_list() noexcept;
__no_constexpr_or_inline void
set_global_seed();
__no_constexpr_or_inline const utility::complete_global_seed_t&
get_global_seed();
__no_constexpr_or_inline _ABC_NS_UTILITY::volatilte_volatile_void_ptr_t&
get_threads_volatie_ptr_ref();
namespace
{
// Can't be constexpr due to use of static var.
__no_constexpr_or_inline ds::test_list_t&
                         get_mutable_test_list() noexcept;
__no_constexpr_or_inline test_options_base_t&
                         get_inner_global_test_options() noexcept;
__no_constexpr_or_inline reporters::error_reporter_controller_t&
                         get_inner_global_error_reporter_controller() noexcept;
__no_constexpr_or_inline utility::complete_global_seed_t& get_inner_global_seed() noexcept;
} // namespace

_END_ABC_GLOBAL_NS