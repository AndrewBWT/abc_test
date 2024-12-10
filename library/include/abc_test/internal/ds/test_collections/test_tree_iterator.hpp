#pragma once

#include "abc_test/internal/ds/test_collections/test_tree.hpp"
#include "abc_test/internal/ds/test_collections/test_tree_sentinel.hpp"

#include <stack>
#include <vector>

_BEGIN_ABC_DS_NS
/*!
 * @brief Type synonym for a reference to a const test_tree_t.
 */
using test_tree_iterator_element_t = std::reference_wrapper<const test_tree_t>;

/*!
 * @brief This class describes the iterator for the test_tree_t class.
 *
 */
class test_tree_iterator_t
{
public:
    // No default constructor.
    __no_constexpr
        test_tree_iterator_t() noexcept
        = delete;
    /*!
     * @brief Constructor for iterator.
     * @param _a_node_ptr This is the node of the test_tree_t being iterated
     * over.
     */
    __no_constexpr
        test_tree_iterator_t(test_tree_iterator_element_t _a_node_ptr) noexcept;
    /*!
     * @brief Increments the iterator.
     * @return A reference to the iterator object after the increment has
     * occoured.
     */
    __no_constexpr test_tree_iterator_t&
        operator++();
    /*!
     * @brief Increments the iterator.
     * @param Dummy int paratmer.
     * @return The new iterator object after the increment has occoured.
     */
    __no_constexpr test_tree_iterator_t
        operator++(int);
    /*!
     * @brief Equality operator between the iterator and sentinel classes.
     * @param _a_rhs The sentinel to check for equality.
     * @return True if this iterator is at the end, false otherwise.
     */
    __no_constexpr bool
        operator==(const test_tree_iterator_sentinel_t& _a_rhs) const;
    /*!
     * @brief Dereference operator for the iterator.
     * @return A cref to the post_setup_test_data_t internal object.
     */
    __no_constexpr const post_setup_test_data_t&
        operator*() const;
private:
    flat_test_set::const_iterator _m_current_text_itt;
    using tree_stack_t = std::stack<test_tree_iterator_element_t>;
    tree_stack_t _m_stack;
    /*!
     * @brief Sets the iterator up at the next node in the tree, with the
     * internal iterator to the test set also set at the correct value.
     */
    __no_constexpr void
        find_next_test();
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp
    test_tree_iterator_t::test_tree_iterator_t(
        test_tree_iterator_element_t _a_node_ptr
    ) noexcept
    : _m_stack(tree_stack_t({_a_node_ptr}))
{
    if (_m_stack.size() > 0)
    {
        find_next_test();
    }
}

__no_constexpr_imp test_tree_iterator_t&
    test_tree_iterator_t::operator++()
{
    using namespace errors;
    if (_m_stack.size() == 0)
    {
        throw test_library_exception_t(
            "Could not increment tree_iterator_t as internal stack is empty"
        );
    }
    else
    {
        const test_tree_t& _l_element{_m_stack.top().get()};
        {
            ++_m_current_text_itt;
            // Is the current one at the end? If so then increment.
            if (_m_current_text_itt == _l_element._m_nodes_tests.end())
            {
                _m_stack.pop();
                test_tree_child_nodes::const_reverse_iterator _l_end{
                    std::rend(_l_element._m_nodes_child_nodes)
                };
                for (test_tree_child_nodes::const_reverse_iterator _l_itt{
                         std::rbegin(_l_element._m_nodes_child_nodes)
                    };
                    _l_itt != _l_end;
                    ++_l_itt)
                {
                    _m_stack.push({ *_l_itt->get() });
                }
                if (_m_stack.size() > 0)
                {
                    find_next_test();
                }
            }
        }
    }
    return *this;
}

__no_constexpr_imp test_tree_iterator_t
    test_tree_iterator_t::operator++(
        int
    )
{
    auto _l_cpy{*this};
    ++(*this);
    return _l_cpy;
}

__no_constexpr_imp bool
    test_tree_iterator_t::operator==(
        const test_tree_iterator_sentinel_t& _a_rhs
    ) const
{
    return _m_stack.size() == 0;
}

__no_constexpr_imp void
    test_tree_iterator_t::find_next_test()
{
    // We now from the callers sights that:
    // The size is definitely > 0
    using namespace errors;
    test_tree_iterator_element_t _l_element_ref{_m_stack.top()};
    // Now gotta go through the tree until we find one with a child.
    while (not _l_element_ref.get().has_test_children())
    {
        // New scope to allow us to get the reference and not have to re-assign
        // it later. Reference is only used so we don't have to use get() all
        // the time.
        {
            const test_tree_t& _l_element{_l_element_ref.get()};
            // Remove the current element from the stack and add all its
            // children.
            _m_stack.pop();
            test_tree_child_nodes::const_reverse_iterator _l_end{
                std::rend(_l_element._m_nodes_child_nodes)
            };
            for (test_tree_child_nodes::const_reverse_iterator _l_itt{
                     std::rbegin(_l_element._m_nodes_child_nodes)
                 };
                 _l_itt != _l_end;
                 ++_l_itt)
            {
                _m_stack.push({*_l_itt->get()});
            }
        }
        // Now find the first element in the stack and pull it out.
        if (_m_stack.size() > 0)
        {
            _l_element_ref = _m_stack.top();
        }
        else
        {
            // If there's nothing remaining, return. Set iterator to dummy
            // value.
            _m_current_text_itt = flat_test_set::const_iterator();
            return;
        }
    }
    _m_current_text_itt = _m_stack.top().get()._m_nodes_tests.begin();
}

__no_constexpr_imp const post_setup_test_data_t&
    test_tree_iterator_t::operator*() const
{
    using namespace errors;
    if (_m_stack.size() == 0
        || _m_current_text_itt == _m_stack.top().get()._m_nodes_tests.end())
    {
        throw test_library_exception_t("Could not dereference tree_iterator.");
    }
    else
    {
        return *_m_current_text_itt;
    }
}

_END_ABC_DS_NS