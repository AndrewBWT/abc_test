#pragma once

#include "abc_test/core/ds/test_collections/test_tree_sentinel.hpp"
#include "abc_test/core/ds/test_data/post_setup_test_data.hpp"
#include "abc_test/core/options/test_options_base.hpp"

#include <memory>
#include <ranges>
#include <vector>

_BEGIN_ABC_DS_NS
// Forward declarations
class test_tree_t;
struct test_tree_iterator_t;

namespace
{
/*!
 * @brief Type synonym for a node in the tree.
 */
using test_tree_child_node  = std::unique_ptr<test_tree_t>;
/*!
 * @brief Type synonym for a set of nodes in the tree.
 */
using test_tree_child_nodes = std::vector<test_tree_child_node>;
/*!
 * @brief Type synonym for the node types in the tree.
 */
using node_t          = std::reference_wrapper<const post_setup_test_data_t>;
/*!
 * @brief The synonym for collections of node_t elements.
 */
using flat_test_set   = std::vector<node_t>;


using test_tree_ref_t = std::reference_wrapper<test_tree_t>;
} // namespace

/*!
 * @brief Structure used to store post_setup_test_data_t representations of
 * tests.
 *
 * The structure itself is a tree which uses the test's path to order its nodes.
 * Each node contains a collection of tests, which are ordered by the test's
 * name.
 *
 * This structure orders them according to their path and name. This way, it is
 * simple to calculate whether any pairs of names and paths are identical. It
 * also allows us to iterate over them in this order, which is the default way
 * they are ran.
 */
class test_tree_t
{
public:
    /*!
     * @brief Default constructor.
     *
     * Constructs an empty tree.
     */
    __constexpr
    test_tree_t() noexcept
        = default;
    /*!
     * @brief Adds a single test to the underlying tree.
     *
     * A test_options_base_t object is used as it contains some information
     * pertaining to logical inconsistencies.
     *
     * @param _a_test The test to be added to the tree.
     * @param _a_options The test_options_base_t object used when checking for
     * errors.
     * @return An optional error; if there were any inconsistencies found when
     * adding the test, this object will contain a string description of them.
     * If not, it contains a nullopt.
     */
    __no_constexpr errors::opt_setup_error_t
        add_test(const node_t& _a_test, const test_options_base_t& _a_options)
            noexcept;
    /*!
     * @brief Returns an iterator to the beginning of the tree.
     *
     * Using this function and end() allows the user to iterate over the tree's
     * elements.
     *
     * @return An iterator to the beginning of hte tree.
     */
    __no_constexpr_or_inline test_tree_iterator_t
        begin() const noexcept;
    /*!
     * @brief Returns a sentinel representing the end of an iterator pointing to
     * the tree.
     * @return Sentinel representing the end of the test_tree_t.
     */
    __no_constexpr_or_inline test_tree_iterator_sentinel_t
        end() const noexcept;
    friend class test_tree_iterator_t;
private:
    test_path_element_t   _m_node_name;
    flat_test_set         _m_nodes_tests;
    test_tree_child_nodes _m_nodes_child_nodes;
    /*!
     * @brief Tells callee whether the index given can be incremented and still
     * access this node's chidren.
     * @param _a_idx
     * @return
     */
    __constexpr bool
        has_test_index_less_than(const size_t _a_idx) const noexcept;
    /*!
     * @brief Constructor for test_tree_t element.
     * @param _a_node_name The string used to represent the node.
     */
    __constexpr
    test_tree_t(const test_path_element_ref_t _a_node_name) noexcept;
    /*!
     * @brief Checks if the current element has any children.
     * @return True if there are children, false otherwise.
     */
    __constexpr bool
        has_test_children() const noexcept;
    /*!
     * @brief Given a reference to a tree node and a test_path_hierarchy_t, this
     * function finds the test_Tree_ref_t node which corresponds to that
     * hierarchy.
     * @param _a_tree_root The root of the tree to search.
     * @param _a_path_hierarchy The path hiearchy used to find the node.
     * @return A reference to the node in _a_tree_root which corresponds to
     * _a_path_hiearchy.
     */
    __constexpr friend test_tree_ref_t
        find_or_create_test_node(
            test_tree_ref_t             _a_tree_root,
            const test_path_hierarchy_t _a_path_hierarchy
        ) noexcept;
    /*!
     * @brief Static function which performs the insertion of a test.
     * @param _a_test_tree The root of the test_tree_t to insert into.
     * @param _a_test A pointer to the test being inserted.
     * @param _a_test_ref A reference to the same test.
     * @return opt_setup_error_t. Empty if there is no error, otherwise a text
     * description of the error.
     */
    __no_constexpr friend errors::opt_setup_error_t
        static_add_test(
            std::reference_wrapper<test_tree_t> _a_test_tree,
            const node_t&                       _a_test,
            const ds::post_setup_test_data_t&   _a_test_ref
        ) noexcept;
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp errors::opt_setup_error_t
                   test_tree_t::add_test(
        const node_t&              _a_test,
        const test_options_base_t& _a_options
    ) noexcept
{
    using namespace std;
    using namespace errors;
    const post_setup_test_data_t& _l_test{_a_test.get()};
    if (_l_test.thread_resourses_required() > _a_options.threads)
    {
        return opt_setup_error_t(setup_error_t(
            fmt::format(
                u8"setup_test_error: post_setup_test_data_t's required thread "
                u8"resourses greater than those allocated to the system. "
                u8"post_setup_test_data_t requires {0} threads, while the "
                u8"system "
                u8"has {1} threads available. "
                u8"post_setup_test_data_t = {2}",
                _l_test.thread_resourses_required(),
                _a_options.threads,
                string_view_to_u8string(fmt::format("{0}", _l_test))
            ),
            false
        ));
    }
    else
    {
        vector<size_t> _l_empty_indexes{};
        for (size_t              _l_idx{0};
             const u8string_view _l_element : _l_test.test_path_hierarchy())
        {
            if (_l_element.empty())
            {
                _l_empty_indexes.push_back(_l_idx);
            }
            ++_l_idx;
        }
        if (_l_empty_indexes.size() > 0)
        {
            return opt_setup_error_t(setup_error_t(
                fmt::format(
                    u8"setup_test_error: post_setup_test_data_t's "
                    u8"test_path_hierarchy contains empty strings. "
                    u8"Specifically "
                    u8"at indexes {0}. Complete test_path_hierarchy = {1}. "
                    u8"post_setup_test_data_t = {2}",
                    _l_empty_indexes,
                    _l_test.test_path_hierarchy(),
                    string_view_to_u8string(fmt::format("{0}", _l_test))
                ),
                false
            ));
        }
        else
        {
            return static_add_test(std::ref(*this), _a_test, _l_test);
        }
    }
}

__constexpr_imp bool
    test_tree_t::has_test_index_less_than(
        const size_t _a_idx
    ) const noexcept
{
    return _a_idx + 1 < _m_nodes_tests.size();
}

__constexpr_imp
    test_tree_t::test_tree_t(
        const test_path_element_ref_t _a_node_name
    ) noexcept
    : _m_node_name(test_path_element_t(_a_node_name))
{}

__constexpr_imp bool
    test_tree_t::has_test_children() const noexcept
{
    return _m_nodes_tests.size() > 0;
}

__constexpr_imp test_tree_ref_t
    find_or_create_test_node(
        test_tree_ref_t             _a_tree_root,
        const test_path_hierarchy_t _a_path_hierarchy
    ) noexcept
{
    using namespace std;
    test_tree_ref_t _l_current_node_ref{_a_tree_root};
    for (const test_path_element_t& _l_path_element : _a_path_hierarchy)
    {
        test_tree_t& _l_current_node{_l_current_node_ref.get()};
        ranges::subrange<test_tree_child_nodes::iterator> _l_pair_itt{
            ranges::equal_range(
                _l_current_node._m_nodes_child_nodes,
                _l_path_element,
                ranges::less{},
                [](test_tree_child_node& _l_ref)
                {
                    return _l_ref.get()->_m_node_name;
                }
            )
        };
        if (_l_pair_itt.begin() != _l_pair_itt.end())
        {
            // There is a subrange of equal elements. But we know theres only
            // one. So set it as the current.
            _l_current_node_ref = std::ref(*_l_pair_itt.begin()->get());
        }
        else
        {
            // No elements equal to the range. Going to have to add one.
            test_tree_child_node _l_element_to_insert{
                test_tree_child_node(new test_tree_t(_l_path_element))
            };
            test_tree_child_nodes::iterator _l_inserted_itt{
                _l_current_node._m_nodes_child_nodes.insert(
                    _l_pair_itt.end(), std::move(_l_element_to_insert)
                )
            };
            _l_current_node_ref = std::ref(*_l_inserted_itt->get());
        }
    }
    return _l_current_node_ref;
}

__no_constexpr_imp errors::opt_setup_error_t
                   static_add_test(
                       std::reference_wrapper<test_tree_t> _a_test_tree,
                       const node_t&                       _a_test,
                       const ds::post_setup_test_data_t&   _a_test_ref
                   ) noexcept
{
    // Find the node of the test hierarchy.
    using namespace std;
    using namespace errors;
    test_tree_ref_t   _l_node{find_or_create_test_node(
        _a_test_tree, _a_test_ref.test_path_hierarchy()
    )};
    const string_view _a_name{
        _a_test.get().registered_test_data()._m_user_data.name
    };
    if (_l_node.get()._m_nodes_tests.size() == 0)
    {
        _l_node.get()._m_nodes_tests.push_back(_a_test);
        return opt_setup_error_t{};
    }
    else
    {
        // find the place to insert the test, using the name to order the
        // tests.
        ranges::subrange<flat_test_set::iterator> _l_name_range_itts{
            ranges::equal_range(
                _l_node.get()._m_nodes_tests,
                _a_name,
                ranges::less{},
                [](const node_t& _a_ref)
                {
                    return _a_ref.get()
                        .registered_test_data()
                        ._m_user_data.name;
                }
            )
        };
        if (_l_name_range_itts.begin() != _l_name_range_itts.end())
        {
            // There is a range of equal elements. Can't add, gotta throw an
            // error.
            return opt_setup_error_t(setup_error_t(
                fmt::format(
                    u8"setup_test_error: post_setup_test_data_t's "
                    u8"registered_test_data has the same name as a "
                    u8"current entry in the test_tree_t object. "
                    u8"The post_setup_test_data object we are attempting to "
                    u8"insert is {0}, "
                    u8"while the post_setup_test_data_t object blocking its "
                    u8"insertion is {1}. ",
                    string_view_to_u8string(fmt::format("{0}", _a_test.get())),
                    string_view_to_u8string(
                        fmt::format("{0}", (_l_name_range_itts.begin()->get()))
                    )
                ),
                false
            ));
        }
        else
        {
            // No equal members, just insert at the end of the range.
            _l_node.get()._m_nodes_tests.insert(
                _l_name_range_itts.end(), _a_test
            );
            return opt_setup_error_t{};
        }
    }
}

_END_ABC_DS_NS