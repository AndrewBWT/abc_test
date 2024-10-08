#pragma once

#include <vector>
#include "abc_test/core/ds/test_collections/test_tree.h"
#include <stack>

_BEGIN_ABC_DS_NS
/*!
 * @brief The iterator class for test_tree_t.
 */
class test_tree_iterator_t {
public:
    /*!
     * @brief Default constructor.
     */
    __no_constexpr
        test_tree_iterator_t(
        ) noexcept = default;
    /*!
     * @brief Constructor for specific iterator element.
     * @param _a_node_ptr The pointer to the element the iterator is to point at.
     */
    __no_constexpr
        test_tree_iterator_t(
            test_tree_const_ptr_t _a_node_ptr
        ) noexcept;
    /*!
     * @brief Increments the iterator
     * @return A reference to the incremented iterator
     */
    __no_constexpr
        test_tree_iterator_t&
        operator++(
        );
    /*!
     * @brief Increments the iterator.
     * @param 
     * @return A new object of the current element iterated. 
     */
    __no_constexpr
        test_tree_iterator_t
        operator++(
            int
        );
    /*!
     * @brief Equality operator for test_tree_iterator_t elements.
     */
    __constexpr
        bool
        operator==(
            const test_tree_iterator_t& _a_rhs
            ) const = default;
    /*!
     * @brief Dereference operator for test_tree_iterator_t element
     * @return 
     */
    __no_constexpr
        const post_setup_test_data_t&
        operator*(
        ) const;
private:
    size_t _m_test_index = 0;
    using tree_stack_t = std::stack<test_tree_const_ptr_t>;
    tree_stack_t _m_stack;
    __no_constexpr
        void
        find_next_test(
        );
};
_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp
test_tree_iterator_t::test_tree_iterator_t(
    test_tree_const_ptr_t _a_node_ptr
) noexcept
    : _m_test_index{ 0 }
    , _m_stack(
        _a_node_ptr != nullptr ?
        tree_stack_t({ _a_node_ptr }) :
        tree_stack_t{})
{
    if (_m_stack.size() > 0)
    {
        find_next_test();
    }
}
__no_constexpr_imp
test_tree_iterator_t&
test_tree_iterator_t::operator++(
)
{
    using namespace errors;
    if (_m_stack.size() == 0)
    {
        throw test_library_exception_t(
            "Could not increment tree_iterator_t as stack is empty");
    }
    else
    {
        test_tree_const_ptr_t _l_current_tuple{ _m_stack.top() };
        if (_l_current_tuple == nullptr)
        {
            throw test_library_exception_t(
                "Could not increment tree_iterator_t as top element in stack is nullptr");
        }
        else
        {
            //Is the current (if incremented) index within the bounds
            // of the vector of elements at that point in the tree?
            if (_l_current_tuple->has_test_index_less_than(_m_test_index))
            {
                //Its okay you can increment
                ++_m_test_index;
            }
            else
            {
                //No incrementing. Reset test_index and move on to the next one in the stack.
                _m_test_index = 0;
                _m_stack.pop();
                if (_m_stack.size() > 0)
                {
                    find_next_test();
                }
            }
        }
    }
    return *this;
}
__no_constexpr_imp
test_tree_iterator_t
test_tree_iterator_t::operator++(
int)
    {
        auto _l_cpy{ *this };
        ++(*this);
        return _l_cpy;
    }
__no_constexpr_imp
void
test_tree_iterator_t::find_next_test(
)
{
    //We now from the caller sights that:
    //The size is definitely > 0
    using namespace errors;
    const test_tree_t* _l_current_tuple{ _m_stack.top() };
    if (_l_current_tuple == nullptr)
    {
        throw test_library_exception_t(
            "Could not increment tree_iterator_t as top element in stack is nullptr");
    }
    else
    {
        //Now gotta go through the tree until we find one with a child.
        while (not _l_current_tuple->has_test_children())
        {
            //Remove the current element from the stack.
            _m_stack.pop();
            test_tree_child_nodes::const_reverse_iterator _l_end{ std::rend(_l_current_tuple->_m_nodes_child_nodes) };
            for (test_tree_child_nodes::const_reverse_iterator _l_itt{ std::rbegin(_l_current_tuple->_m_nodes_child_nodes) };
                _l_itt != _l_end; ++_l_itt)
            {
                //Push all the new elemnets onto the stack.
                _m_stack.push({ (*_l_itt).get() });
            }
            //If the size of the stack is > 0.
            if (_m_stack.size() > 0)
            {
                //Go through the elements in the stack until we find one that
                //isn't nullptr.
                do
                {
                    if (_m_stack.size() == 0)
                    {
                        return;
                    }
                    _l_current_tuple = _m_stack.top();
                } while (_l_current_tuple == nullptr);
            }
            else
            {
                break;
            }
        }
    }
}
__no_constexpr_imp
const post_setup_test_data_t&
test_tree_iterator_t::operator*(
) const
{
    using namespace errors;
    if (_m_stack.size() == 0 ||
        _m_stack.top() == nullptr ||
        _m_stack.top()->_m_nodes_tests.size() <= _m_test_index)
    {
        throw test_library_exception_t("Could not dereference tree_iterator");
    }
    return _m_stack.top()->_m_nodes_tests[_m_test_index].get();
}
_END_ABC_DS_NS