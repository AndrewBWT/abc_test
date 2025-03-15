#pragma once
#include "abc_test/core/ds/test_data/registered_test_data.hpp"
// #include "abc_test/core/test_options.hpp"
#include "abc_test/core/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/utility/str/string_utility.hpp"

#include <vector>


_BEGIN_ABC_DS_NS

/*!
 * @brief Object represents a test after it has been processed by the runtime
 * system; that is, registered, its test path hierarchy calculated, its
 * discovery id found, its repetition tree (if it has one) found from the input
 * paramters, and a normalised number of threads allowed for the test.
 *
 */

/*!
 * @brief Structure holds test data. This data is specifically meant to be
 * created after abc_test has pre-processed all the tests it is going to run,
 * but before any actual tests have been ran
 *
 * It contains member variables which could not be known at compile time, and
 * provides an opportunity for incorrectly defined tests to be spotted and
 * stopped from running.
 */
struct post_setup_test_data_t
{
public:
    /*!
     * @brief Constructor
     * @param _a_registered_test_data Underlying registered_test_data_t object.
     * @param _a_test_hierarchy_delimiter The delimiter used to split the test
     * path.
     * @param _a_discovery_id The discovery ID of the post_setup_test_data_t
     * object.
     * @param _a_is_test_to_be_ran Denotes whether the test is to be ran.
     * @param _a_for_loop_stack_trie The for_loop_stack_trie_t object (if there
     * is any) associated with the post_setup_test_data_t object.
     * @param _a_maximum_threads_allowed The number of threads the test
     * requires. This is guaranteed to be above 0, and less than or equal to the
     * maximum number of threads the system will allow.
     */
    __constexpr
    post_setup_test_data_t(
        const registered_test_data_t&          _a_registered_test_data,
        const test_path_hierarchy_t&                 _a_test_path_hierarchy,
        const unique_id_t                      _a_discovery_id,
        const bool                             _a_is_test_to_be_ran,
        const ds::tdg_collection_stack_trie_t* _a_for_loop_stack_trie,
        const size_t                           _a_maximum_threads_allowed,
        const std::string_view _a_unique_id
    ) noexcept;
    /*!
     * @brief Returns a cref to the registered_test_data_t member variable.
     * @return A cref to the registered_test_data_t member variable.
     */
    __constexpr const registered_test_data_t&
        registered_test_data() const;
    /*!
     * @brief Returns a cref to the test_path_hierarchy_t member variable.
     * @return A cref to the test_path_hierarchy_t member variable.
     */
    __constexpr const test_path_hierarchy_t&
        test_path_hierarchy() const noexcept;
    /*!
     * @brief Tells the caller whether the test is to be ran.
     * @return True if the test is to be ran; false otherwise.
     */
    __constexpr bool
        is_test_to_be_ran() const noexcept;
    /*!
     * @brief Returns the unique_id_t of the object.
     * @return The unique_id_t of the object.
     */
    __constexpr unique_id_t
        discovery_id() const noexcept;
    /*!
     * @brief Tells the caller whether this object has a for_loop_stack_trie_t
     * object associated with it.
     * @return True if there is a trie associated with this object. false
     * otherwise.
     */
    __constexpr bool
        has_for_loop_stack_trie() const noexcept;
    /*!
     * @brief Gets a cref to the for_loop_stack_trie_t variable associated with
     * this object.
     *
     * This function will throw an exception if there is no
     * for_loop_stack_trie_t associated with it. We recomend using
     * has_for_loop_stack_trie() with this function.
     *
     * @return A cref to the for_loop_stack_tree_t variable associated with this
     * object.
     */
    __constexpr const ds::tdg_collection_stack_trie_t&
                      for_loop_stack_trie() const;
    /*!
     * @brief Returns the thread resourses required by this test, as stipulated
     * by the user.
     *
     * Note this value has been normalised; it will be > 0 and <= the number of
     * threads available to the system.
     *
     * @return The thread resourses required by this test.
     */
    __constexpr std::size_t
                thread_resourses_required() const noexcept;
    __constexpr const std::string_view
        unique_id() const noexcept;
    /*!
     * @brief Equality operator for post_setup_test_data_t objects.
     * @param _a_rhs The post_setup_test_data_t object to compare this object
     * to.
     * @return True if the objects are equal; false otherwise.
     */
    __constexpr bool
        operator==(const post_setup_test_data_t& _a_rhs) const noexcept
        = default;
private:
    // A cref to the registered_test_data_t object associated with this test.
    const registered_test_data_t& _m_registered_test_data;
    // The test path hierarchy of this test.
    test_path_hierarchy_t _m_test_path_hierarchy;
    // The unique ID of this test.
    unique_id_t _m_discovery_id;
    // The bool stipulating whether this test is to be ran.
    bool _m_is_test_to_be_ran;
    // The number of threads required by the system.
    size_t _m_thread_resourses_required;
    // A pointer to the trie associated with this test.
    const ds::tdg_collection_stack_trie_t* _m_for_loop_stack_trie;
    // The tests unique ID
    std::string _m_unique_id;
};

__constexpr bool check_if_element_in_path_set(
    const test_path_hierarchy_t& _a_test_path_hierarchy,
    const std::vector<test_path_hierarchy_t>& _a_test_path_set
) noexcept;
_END_ABC_DS_NS

/*!
 * formatter for post_setup_test_ata object.
 */
template <>
struct fmt::formatter<_ABC_NS_DS::post_setup_test_data_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    __no_constexpr auto
        format(
            _ABC_NS_DS::post_setup_test_data_t _a_pstd,
            format_context&                    _a_cxt
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp
    post_setup_test_data_t::post_setup_test_data_t(
        const registered_test_data_t&          _a_registered_test_data,
        const test_path_hierarchy_t& _a_test_path_hierarchy,
        const unique_id_t                      _a_discovery_id,
        const bool                             _a_is_test_to_be_ran,
        const ds::tdg_collection_stack_trie_t* _a_repetition_data,
        const size_t                           _a_maximum_threads_allowed,
        const std::string_view _a_unique_id
    ) noexcept
    : _m_registered_test_data(_a_registered_test_data)
    , _m_test_path_hierarchy(_a_test_path_hierarchy)
    , _m_discovery_id(_a_discovery_id)
    , _m_is_test_to_be_ran(_a_is_test_to_be_ran)
    , _m_for_loop_stack_trie(_a_repetition_data)
    , _m_thread_resourses_required(
          _a_registered_test_data._m_user_data.threads_required == 0
              ? _a_maximum_threads_allowed
              : std::min(
                    _a_maximum_threads_allowed,
                    _a_registered_test_data._m_user_data.threads_required
                )
      )
    , _m_unique_id(_a_unique_id)
{}

__constexpr_imp const registered_test_data_t&
    post_setup_test_data_t::registered_test_data() const
{
    return _m_registered_test_data;
}

__constexpr_imp const test_path_hierarchy_t&
    post_setup_test_data_t::test_path_hierarchy() const noexcept
{
    return _m_test_path_hierarchy;
}

__constexpr_imp bool
    post_setup_test_data_t::is_test_to_be_ran() const noexcept
{
    return _m_is_test_to_be_ran;
}

__constexpr_imp unique_id_t
    post_setup_test_data_t::discovery_id() const noexcept
{
    return _m_discovery_id;
}

__constexpr_imp bool
    post_setup_test_data_t::has_for_loop_stack_trie() const noexcept
{
    return _m_for_loop_stack_trie != nullptr;
}

__constexpr_imp const ds::tdg_collection_stack_trie_t&
                      post_setup_test_data_t::for_loop_stack_trie() const
{
    if (_m_for_loop_stack_trie == nullptr)
    {
        throw errors::test_library_exception_t(
            "Attempted to access post_setup_test_data_t's repetition data. "
            "However, the for_loop_stack_trie contains a nullptr. "
            "Use has_for_loop_stack_trie to check this."
        );
    }
    else
    {
        return *_m_for_loop_stack_trie;
    }
}

__constexpr_imp std::size_t
    post_setup_test_data_t::thread_resourses_required() const noexcept
{
    return _m_thread_resourses_required;
}
__constexpr_imp const std::string_view
post_setup_test_data_t::unique_id() const noexcept
{
    return _m_unique_id;
}
__constexpr_imp bool check_if_element_in_path_set(
    const test_path_hierarchy_t& _a_test_path_hierarchy,
    const std::vector<test_path_hierarchy_t>& _a_test_path_set
) noexcept
{
    using namespace std;
    const size_t _l_test_path_hierarchy_size{ _a_test_path_hierarchy.size() };
    for (const test_path_hierarchy_t& _l_path_set_element : _a_test_path_set)
    {
        bool _l_prefix_matches{ true };
        for (size_t _l_idx{ 0 }; const string_view _l_str : _l_path_set_element)
        {
            if (_l_idx >= _l_test_path_hierarchy_size)
            {
                _l_prefix_matches = false;
                break;
            }
            else if (_a_test_path_hierarchy[_l_idx] != _l_str)
            {
                _l_prefix_matches = false;
                break;
            }
            ++_l_idx;
        }
        if (_l_prefix_matches)
        {
            return true;
        }
    }
    return false;
}
_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::post_setup_test_data_t>::format(
        _ABC_NS_DS::post_setup_test_data_t _a_pstd,
        format_context&                    _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0} "
        "{{{1} = {2}"
        ", {3} = {4}"
        ", {5} = {6}"
        ", {7} = {8}"
        ", {9} = {10}",
        ", {11} = {12}",
        ", {13} = {14}"
        "}}",
        typeid(_a_pstd).name(),
        "_m_registered_test_data",
        _a_pstd.registered_test_data(),
        "_m_test_path_hierarchy",
        _a_pstd.test_path_hierarchy(),
        "_m_discovery_id",
        _a_pstd.discovery_id(),
        "_m_is_test_to_be_ran",
        _a_pstd.is_test_to_be_ran(),
        "_m_thread_resourses_required",
        _a_pstd.thread_resourses_required(),
        "_m_for_loop_stack_trie",
        _a_pstd.has_for_loop_stack_trie()
            ? fmt::format("{0}", _a_pstd.for_loop_stack_trie())
            : "nullptr"
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}
