#pragma once

#include "abc_test/internal/ds/data_generator_memoization/typeless_data_generator_collection_stack.h"
#include "abc_test/internal/ds/test_data/enum_test_status.h"
#include "abc_test/internal/ds/test_data/post_setup_test_data.h"
#include "abc_test/internal/test_reports/assertion.h"
#include "abc_test/internal/test_reports/unexpected_report.h"
#include "abc_test/internal/utility/rng.h"

#include <fmt/ranges.h>
#include <functional>

_BEGIN_ABC_DS_NS

/*!
 * @brief Object used to represent a test currently being ran.
 */

/*!
 * @brief Structure used to represent a test which is currently invoked.
 *
 * invoked_test_data_t builds on the other test types. It can be thought of as
 * storing run-time information.
 */
struct invoked_test_data_t
{
public:
    __no_constexpr
        invoked_test_data_t() noexcept
        = delete;
    /*!
     * @brief Main constructor.
     * @param _a_seed_seq The seed for this invoked_test_data_t instance.
     * @param _a_test_info The post_setup_test_data_t component for this
     * invoked_test_data_t instance.
     * @param _a_order_ran_id A unique ID representing the order this
     * invoked_test_data_t element is being ran in.
     * @param _a_root_path The root filepath of the system, where all other
     * paths originate from.
     */
    __no_constexpr
        invoked_test_data_t(
            const utility::seed_t&        _a_seed_seq,
            const post_setup_test_data_t& _a_test_info,
            const size_t                  _a_order_ran_id,
            const std::filesystem::path& _a_root_path
        ) noexcept;
    /*!
     * @brief Returns the path of the invoked_test_data_t test to the caller.
     *
     * Each invoked_test_data_t test has a path, built from the user-defined
     * path and the name in registered_test_data_t.
     *
     * These are folders, not files. They are used by various sub-systems in the
     * testing framework to store data.
     *
     * @return The test's file path.
     */
    __constexpr const std::filesystem::path&
                      path() const noexcept;
    /*!
     * @brief Returns a cref to the invoked_test_data_t's utility::rng object.
     * @return Cref to the invoked_test_data_t's utility::rng object.
     */
    __constexpr const utility::rng&
                      get_random_generator() noexcept;
    /*!
     * @brief Returns a cref to the invoked_test_data_t's post_setup_tst_data_t
     * object.
     * @return The invoked_test_data_t's post_setup_test_data_t object.
     */
    __constexpr const post_setup_test_data_t&
        post_setup_test_data() const;
    /*!
     * @brief Gets a cref to the invoked_test_data_t's for_loop_stack_t object.
     *
     * This can be thought of as the current set of gen_data_t structures
     * currently used.
     *
     * Note it is read only; it cannot be edited.
     *
     * @return A cref to the invoked_test_data_t's for_loop_stack_t object.
     */
    __constexpr const ds::tdg_collection_stack_t&
                      for_loop_data_collection() const noexcept;
    /*!
     * @brief Decrements the invoked_test_data_t for_loop_stack_t member
     * variable.
     */
    __constexpr void
        decrement_for_loop_stack() noexcept;
    /*!
     * @brief Updates the invoked_test_data_t for_loop_stack_t member variable.
     * @param _a_ptr The gdc_itt_agnostic_data_ref_t used to update the
     * for_loop_stack_t variable.
     */
    __no_constexpr void
        update_for_loop_stack(tdg_collection_itt_ref_t _a_ptr) noexcept;
    /*!
     * @brief Increments the invoked_test_data_t's for_loop_stack_t member
     * variable.
     * @param _a_pstr The gdc_itt_agnostic_data_ref_t used to incrment the
     * for_loop_stack_t variable.
     */
    __no_constexpr void
        increment_for_loop_stack(tdg_collection_itt_ref_t _a_pstr) noexcept;
    /*!
     * @brief Gets the first child of the current for_loop_stack in the memoized
     * for_loop_stack_trie contained in the invoked_test_data_t's
     * post_setup_test_data member variable.
     *
     * If the post_setup_test_data_t member variable has no for_loop_stack_trie
     * (this corresponds to there being no specific set of for loop data
     * variables to re-generate), then this function will return a nullopt.
     *
     * To be clear, this function is used when trying to repeat parts of a test.
     * The post_setup_test_data_t, containing a specific set of for loop data
     * elements to repeat is probed, to find the first in the child of the
     * current for loop trie.
     *
     * Essentially, it is called when a new for loop is entered.
     *
     * @return Next element in the post_setup_test_data_t's repetition data when
     * the current for_loop_trie is considered the node.
     */
    __constexpr opt_idgc_memoized_element_t
        get_first_child_of_current_for_loop_stack() noexcept;
    /*!
     * @brief Gets the next element in the post_setup_test_data_t's
     * for_loop_stack_trie_t member variable, assuming it has one.
     *
     * To be clear, this function is to be called when wanting to repeat tests
     * which use gen_data_collection_t structures in loops. This function is
     * called when incrementing such a structure; it assumes that the current
     * for_loop_stack_t is correct, and finds its node in the
     * post_setup_test_data_t's for_loop_stack_trie_t member variable. It then
     * finds its "sibling" in the structure and, assuming it has one, returns
     * the for_loop_creation_data_t object which would need to be exchanged with
     * the current last element in the for_loop_stack_t to create that
     * for_loop_stack_t object.
     *
     * If the post_setup_test_data_t object contains no for_loop_stack_trie_t
     * object, there is no "successor" node, or the current node cannot be found
     * in the for_loop_stack_trie_t, then this function returns a nullopt.
     *
     * @return opt_for_loop_creation_data_t object. This function returns a
     * nullopt if it cannot find a successor value, otherwise returns the
     * successor value.
     */
    __constexpr opt_idgc_memoized_element_t
        increment_last_value_of_current_for_loop_stack() noexcept;

    /*!
     * @brief Tells the caller if the current for_loop_stack_trie_t object is in
     * the post_setup_test_data_t's for_loop_stack_trie_t member variable.
     *
     * The user may think that get_first_child_of_current_for_loop_stack() and
     * increment_last_value_of_current_for_loop_stack() are all that is needed
     * to navigate gen_data_collection_t for loops. However, when using manual
     * for loops, this function must be used, to allow the test to skip over
     * expressions.
     *
     * @return True if the current for_loop_stack_trie_t variable is contained
     * in the post_setup_test_data_t's for_loop_stack_trie_t member variable.
     */
    __constexpr bool
        is_current_for_loop_stack_in_true() const noexcept;
    /*!
     * @brief Returns a cref to this object's for_loop_stack_trie_t.
     *
     * To be clear, this is different to the post_setup_test_data_t's
     * for_loop_stack_trie_t. This structure is used when creating seeds used to
     * repeat tests; the variable in the post_setup_test_data_t is that used
     * when repeating tests.
     *
     * While the post_setup_test_data_t object can point to a nullptr (which
     * corresponds to no repetition data being available), this structure is
     * always available.
     *
     * @return A cref to the internal for_loop_stack_trie_t object.
     */
    __constexpr const ds::tdg_collection_stack_trie_t&
                      for_loop_stack_trie() const noexcept;
    /*!
     * @brief Adds the current for_loop_stack_t to the internal
     * for_loop_stack_trie_t object.
     *
     * This function is called when the overarching system wants to state that
     * some data from a gen_data_collection_t object should be repeated.
     *
     */
    __constexpr void
        add_current_for_loop_stack_to_trie() noexcept;
    /*!
     * @brief Gets the current order ran ID.
     *
     * This ID denotes the order the tests should be ran in. Note that, if
     * mult-threading is used to run the tests, then it is entirely possible
     * this order will not be adhered to.
     *
     * @return Integer representing the order the test should be ran in.
     */
    __constexpr std::size_t
                order_ran_id() const noexcept;
    /*!
     * @brief Adds an assertin to the test's list of assertions ran.
     *
     * This function can throw an exception if the unique pointer contained a
     * nullptr, or if the test_status signifies that a termination has already
     * occoured.
     *
     * @param _a_ptr A generic_assertion_ptr_t, a std::unique ptr type,
     * representing the assertion.
     */
    template <bool Single_Source, typename Assertion_Status>
    __constexpr void
        add_assertion(
            reports::assertion_ptr_t<Single_Source, Assertion_Status>& _a_ptr
        );
    /*!
     * @brief Adds an unexpected assertion to the object.
     *
     * If there already is an unexpected exception object registered, or the
     * unexpected exception is a nullptr, then this function throws an
     *
     *
     * @param _a_ur Rvalue to unique_ptr containing report.
     */
    __constexpr_imp void
        set_unexpected_termination(reports::opt_unexpected_report_t&& _a_ur);
    /*!
     * @brief Gets the number of assertions reported to the object which have
     * passed.
     * @return The number of passed assertions reported to the test.
     */
    __constexpr std::size_t
                assertions_passed() const noexcept;
    /*!
     * @brief Returns the number of failed assertions to the caller.
     * @return The number of failed assertions.
     */
    __constexpr std::size_t
                assertions_failed() const noexcept;
    /*!
     * @brief Returns the total number of assertions reported to the object.
     * @return The total number of assertions reported to the object.
     */
    __constexpr std::size_t
                assertions_recieved() const noexcept;
    /*!
     * @brief Returns the status of the test.
     * @return The status of the test.
     */
    __constexpr enum_test_status_t
        test_status() const noexcept;
    /*!
     * @brief Returns the number of warnings the object recived to the caller.
     * @return The number of warnings the object recieved.
     */
    __constexpr std::size_t
                warnings_recieved() const noexcept;
    /*!
     * @brief Returns a cref to the object's opt_unexpected_report_t to the
     * caller.
     * @return A cref to the object's opt_unexpected_report_t.
     */
    __constexpr const reports::opt_unexpected_report_t&
                      unexpected_termination() const noexcept;
    /*!
     * @brief Returns a cref to the object's generic_user_report_collection_t to
     * the caller.
     * @return A cref to the object's generic_user_report_collection_t.
     */
    __constexpr const reports::assertion_base_collection_t&
                      assertions() const noexcept;
    __constexpr void
        add_warning(reports::unexpected_non_terminating_report_ptr_t&& _a_pstr
        ) noexcept;
private:
    const post_setup_test_data_t&        _m_post_setup_test_data;
    ds::tdg_collection_stack_trie_t            _m_tests_for_loop_stack_trie;
    ds::tdg_collection_stack_t                 _m_for_loop_data_collection;
    std::size_t                          _m_order_ran_id;
    utility::rng                         _m_this_tests_random_generator;
    std::filesystem::path                _m_path;
    enum_test_status_t                   _m_test_status = enum_test_status_t::NO_TERMINATION_TEST_PASSED;
    std::size_t                          _m_total_number_assertions_recieved = 0;
    std::size_t                          _m_total_number_assertions_passed = 0;
    std::size_t                          _m_total_number_assertions_failed = 0;
    reports::assertion_base_collection_t _m_assertions;
    reports::opt_unexpected_report_t     _m_termination_report;
    reports::unexpected_non_terminating_report_collection_t _m_warnings;
};

namespace
{
__no_constexpr std::filesystem::path
               create_test_path(
                   const post_setup_test_data_t& _a_test_info,
                   const std::filesystem::path& _a_root_path
               ) noexcept;
} // namespace

_END_ABC_DS_NS

/*!
 * formatter for post_setup_test_ata object.
 */
template <>
struct fmt::formatter<_ABC_NS_DS::invoked_test_data_t> : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    __no_constexpr auto
        format(_ABC_NS_DS::invoked_test_data_t _a_iti, format_context& _a_cxt)
            const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__no_constexpr_imp
    invoked_test_data_t::invoked_test_data_t(
        const utility::seed_t&        _a_seed_seq,
        const post_setup_test_data_t& _a_post_setup_test_data,
        const size_t                  _a_order_ran_id,
        const std::filesystem::path& _a_root_path
    ) noexcept
    : _m_post_setup_test_data(_a_post_setup_test_data)
    , _m_for_loop_data_collection(ds::tdg_collection_stack_t())
    , _m_order_ran_id(_a_order_ran_id)
    , _m_this_tests_random_generator(_a_seed_seq)
    , _m_path(create_test_path(_a_post_setup_test_data, _a_root_path))
    , _m_test_status(enum_test_status_t::NO_TERMINATION_TEST_PASSED)
{}

__constexpr_imp const std::filesystem::path&
                      invoked_test_data_t::path() const noexcept
{
    return _m_path;
}

__constexpr_imp const utility::rng&
                      invoked_test_data_t::get_random_generator() noexcept
{
    return _m_this_tests_random_generator;
}

__constexpr_imp const post_setup_test_data_t&
    invoked_test_data_t::post_setup_test_data() const
{
    return _m_post_setup_test_data;
}

__constexpr_imp const ds::tdg_collection_stack_t&
    invoked_test_data_t::for_loop_data_collection() const noexcept
{
    return _m_for_loop_data_collection;
}

__constexpr_imp void
    invoked_test_data_t::decrement_for_loop_stack() noexcept
{
    _m_for_loop_data_collection.decrement();
}

__no_constexpr_imp void
    invoked_test_data_t::update_for_loop_stack(
        tdg_collection_itt_ref_t _a_ptr
    ) noexcept
{
    _m_for_loop_data_collection.update(_a_ptr);
}

__no_constexpr_imp void
    invoked_test_data_t::increment_for_loop_stack(
        tdg_collection_itt_ref_t _a_ptr
    ) noexcept
{
    _m_for_loop_data_collection.increment(_a_ptr);
}

__constexpr_imp opt_idgc_memoized_element_t
    invoked_test_data_t::get_first_child_of_current_for_loop_stack() noexcept
{
    using namespace std;
    using namespace ds;
    using enum utility::internal::internal_log_enum_t;
    const optional<idgc_memoized_element_t> _l_rv{
        not _m_post_setup_test_data.has_for_loop_stack_trie()
            ? opt_idgc_memoized_element_t{}
            : _m_post_setup_test_data.for_loop_stack_trie()
                  .find_first_child_of_sequence_in_trie(
                      _m_for_loop_data_collection.create_data_sequence()
                  )
    };
    return _l_rv;
}

__constexpr_imp opt_idgc_memoized_element_t
    invoked_test_data_t::increment_last_value_of_current_for_loop_stack(
    ) noexcept
{
    using namespace std;
    using namespace ds;
    using enum utility::internal::internal_log_enum_t;
    const opt_idgc_memoized_element_t _l_rv{
        not _m_post_setup_test_data.has_for_loop_stack_trie()
            ? opt_idgc_memoized_element_t{}
            : _m_post_setup_test_data.for_loop_stack_trie()
                  .increment_last_index(
                      _m_for_loop_data_collection.create_data_sequence()
                  )
    };
    return _l_rv;
}

__constexpr_imp bool
    invoked_test_data_t::is_current_for_loop_stack_in_true() const noexcept
{
    return (not _m_post_setup_test_data.has_for_loop_stack_trie())
               ? true
               : _m_post_setup_test_data.for_loop_stack_trie()
                     .is_sequence_in_trie(
                         _m_for_loop_data_collection.create_data_sequence()
                     );
}

__constexpr_imp const ds::tdg_collection_stack_trie_t&
                      invoked_test_data_t::for_loop_stack_trie() const noexcept
{
    return _m_tests_for_loop_stack_trie;
}

__constexpr_imp void
    invoked_test_data_t::add_current_for_loop_stack_to_trie() noexcept
{
    _m_tests_for_loop_stack_trie.add_for_loop_creation_data_sequence(
        _m_for_loop_data_collection.create_data_sequence()
    );
}

__constexpr_imp std::size_t
                invoked_test_data_t::order_ran_id() const noexcept
{
    return _m_order_ran_id;
}

template <bool Single_Source, typename Assertion_Status>
__constexpr_imp void
    invoked_test_data_t::add_assertion(
        reports::assertion_ptr_t<Single_Source, Assertion_Status>& _a_ptr
    )
{
    using namespace reports;
    using enum enum_test_status_t;
    if (terminated(_m_test_status))
    {
        // In the incorrect status; a termination has been thrown from this
        // test, why is it still running?
        throw errors::test_library_exception_t(fmt::format(
            "add_assertions function has been entered, however should have "
            "already termianted. _m_test_status = {0}",
            _m_test_status
        ));
    }
    else if (_a_ptr == nullptr)
    {
        // Input is a nullptr. Incorrect.
        throw errors::unaccounted_for_nullptr(_a_ptr.get());
        _m_test_status = TERMINATION_OCCOURED_UNEXPECTED_THROW;
    }
    else
    {
        const reports::assertion_t<Single_Source, Assertion_Status>& _l_ref{
            *_a_ptr
        };
        _m_total_number_assertions_recieved++;
        // Assertin recived, correct status (either pass or fail but no
        // terminations).
        if (_l_ref.get_pass_status())
        {
            // Increment the pass.
            _m_total_number_assertions_passed++;
        }
        else
        {
            _m_total_number_assertions_failed++;
            if (_l_ref.terminated())
            {
                // If failed and terminated change to a termiantion status.
                _m_test_status = TERMINATION_OCCOURED_TEST_FAILED;
            }
            else
            {
                // Else set to a fail status.
                _m_test_status = NO_TERMINATION_TEST_FAILED;
            }
        }
        // This has to be the last thing, or accessing _a_ptr would be invalid.
        _m_assertions.push_back(assertion_base_ptr_t(std::move(_a_ptr)));
    }
}

__constexpr_imp void
    invoked_test_data_t::set_unexpected_termination(
        reports::opt_unexpected_report_t&& _a_ur
    )
{
    using namespace reports;
    using enum enum_test_status_t;
    if (terminated(_m_test_status))
    {
        // In the incorrect status; a termination has been thrown from this
        // test, why is it still running?
        throw errors::test_library_exception_t(fmt::format(
            "set_unexpected_termination function has been entered, however "
            "should have already termianted. _m_test_status = {0}",
            _m_test_status
        ));
    }
    else if (_m_termination_report != nullptr)
    {
        throw errors::test_library_exception_t(
            "Attempting to call set_unexpected_termination, however an "
            "unexpected termination has already been registered."
        );
    }
    else if (_a_ur == nullptr)
    {
        throw errors::unaccounted_for_nullptr(_a_ur.get());
    }
    else
    {
        _m_termination_report = opt_unexpected_report_t(std::move(_a_ur));
        _m_test_status        = TERMINATION_OCCOURED_UNEXPECTED_THROW;
    }
}

__constexpr_imp std::size_t
                invoked_test_data_t::assertions_passed() const noexcept
{
    return _m_total_number_assertions_passed;
}

__constexpr_imp std::size_t
                invoked_test_data_t::assertions_failed() const noexcept
{
    return _m_total_number_assertions_failed;
}

__constexpr_imp std::size_t
                invoked_test_data_t::assertions_recieved() const noexcept
{
    return _m_total_number_assertions_recieved;
}

__constexpr_imp enum_test_status_t
    invoked_test_data_t::test_status() const noexcept
{
    return _m_test_status;
}

__constexpr_imp std::size_t
                invoked_test_data_t::warnings_recieved() const noexcept
{
    return _m_warnings.size();
}

__constexpr_imp const reports::opt_unexpected_report_t&
    invoked_test_data_t::unexpected_termination() const noexcept
{
    return _m_termination_report;
}

__constexpr_imp const reports::assertion_base_collection_t&
                      invoked_test_data_t::assertions() const noexcept
{
    return _m_assertions;
}

__constexpr_imp void
    invoked_test_data_t::add_warning(
        reports::unexpected_non_terminating_report_ptr_t&& _a_pstr
    ) noexcept
{
    _m_warnings.push_back(std::move(_a_pstr));
}

namespace
{
__no_constexpr_imp std::filesystem::path
                   create_test_path(
                       const post_setup_test_data_t& _a_test_info,
                       const std::filesystem::path& _a_root_path
                   ) noexcept
{
    using namespace std::filesystem;
    using namespace utility;
    path_t _l_path{ _a_root_path };
    for (const test_path_element_ref_t& _a_test_path_component :
         _a_test_info.test_path_hierarchy())
    {
        _l_path /= _a_test_path_component;
    }
    _l_path /= _a_test_info.registered_test_data()._m_user_data.name;
    if (not exists(_l_path))
    {
        create_directories(_l_path);
    }
    return _l_path;
}
} // namespace

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::invoked_test_data_t>::format(
        _ABC_NS_DS::invoked_test_data_t _a_iti,
        format_context&              _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_unwritten_str{"<unwritten-printer>"};
    const string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        ", {3} = {4}"
        ", {5} = {6}"
        ", {7} = {8}"
        ", {9} = {10}"
        ", {11} = {12}"
        ", {13} = {14}"
        ", {15} = {16}"
        ", {17} = {18}"
        ", {19} = {20}"
        ", {21} = {22}"
        ", {23} = {24}"
        ", {25} = {26}"
        "}}",
        typeid(_a_iti).name(),
        "_m_post_setup_test_data",
        _a_iti.post_setup_test_data(),
        "_m_repetition_tree",
        _a_iti.for_loop_stack_trie(),
        "_m_for_loop_data_collection",
        _a_iti.for_loop_data_collection(),
        "_m_order_ran_id",
        _a_iti.order_ran_id(),
        "_m_this_tests_random_generator",
        _l_unwritten_str,
        "_m_path",
        _a_iti.path().string(),
        "_m_test_status",
        _a_iti.test_status(),
        "_m_total_number_assertions_recieved",
        _a_iti.assertions_recieved(),
        "_m_total_number_assertions_passed",
        _a_iti.assertions_passed(),
        "_m_total_number_assertions_failed",
        _a_iti.assertions_failed(),
        "_m_assertions",
        _l_unwritten_str,
        "_m_termination_report",
        _l_unwritten_str,
        "_m_warnings",
        _l_unwritten_str
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}