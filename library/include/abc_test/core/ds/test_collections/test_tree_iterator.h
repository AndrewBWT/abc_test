#pragma once

#include <vector>
#include "abc_test/core/ds/test_collections/test_tree.h"
#include <stack>

_BEGIN_ABC_DS_NS
    struct test_tree_iterator_t {
    public:
        /*!
        * Default constructor for tree iterator
        */
        __no_constexpr
            test_tree_iterator_t(
            ) noexcept;
        /*!
        * Default constructor for tree iterator
        */
        __no_constexpr
            test_tree_iterator_t(
                test_tree_const_ptr_t _a_node_ptr
            ) noexcept;
        /*!
        * Increment operator for iterator
        */
        __no_constexpr
            test_tree_iterator_t&
            operator++(
                );
        /*!
        * Increment operator for iterator
        */
        __no_constexpr
            test_tree_iterator_t
            operator++(
                int
            );
        /*!
        * Equals comparison for iterator
        */
        __constexpr
            bool
            operator==(
                const test_tree_iterator_t& _a_rhs
                ) const;
        /*!
        * Dereference operator for iterator.
        */
        __no_constexpr
            const post_setup_test_data_t&
            operator*(
            ) const;
    private:
        size_t _m_test_index;
        using tree_stack_t = std::stack<test_tree_const_ptr_t>;
        tree_stack_t _m_stack;
        __no_constexpr
            void
            find_next_test(
            ) noexcept;
    };
_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
    __no_constexpr_imp
        test_tree_iterator_t::test_tree_iterator_t(
        ) noexcept
        : test_tree_iterator_t(nullptr)
    {

    }
    __no_constexpr_imp
        test_tree_iterator_t::test_tree_iterator_t(
            test_tree_const_ptr_t _a_node_ptr
        ) noexcept
        : _m_test_index{ 0 }
        , _m_stack(
            _a_node_ptr != nullptr ?
            tree_stack_t({_a_node_ptr}) :
            tree_stack_t{})
    {
        find_next_test();
    }
    __no_constexpr_imp
        test_tree_iterator_t&
        test_tree_iterator_t::operator++(
            )
    {
        using namespace errors;
        if (_m_stack.size() == 0)
        {
            throw test_library_exception_t("Could not increment tree_iterator");
        }
        else
        {
            test_tree_const_ptr_t _l_current_tuple{ _m_stack.top() };
            if (_l_current_tuple == nullptr)
            {
                throw test_library_exception_t("Could not increment tree_iterator");
            }
            else
            {
                if (_l_current_tuple->has_test_index_less_than(_m_test_index))
                {
                    ++_m_test_index;
                }
                else
                {
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
        ) noexcept
    {
        if (_m_stack.size() > 0)
        {
            const test_tree_t* _l_current_tuple{ _m_stack.top() };
            if (_l_current_tuple != nullptr)
            {
                while (not _l_current_tuple->has_test_children())
                {
                    _m_stack.pop();
                    test_tree_child_nodes::const_reverse_iterator _l_end{ std::rend(_l_current_tuple->_m_nodes_child_nodes) };
                    for (test_tree_child_nodes::const_reverse_iterator _l_itt{ std::rbegin(_l_current_tuple->_m_nodes_child_nodes) };
                        _l_itt != _l_end; ++_l_itt)
                    {
                        _m_stack.push({ (*_l_itt).get() });
                    }
                    if (_m_stack.size() > 0)
                    {
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
    }
    __constexpr_imp
        bool
        test_tree_iterator_t::operator==(
            const test_tree_iterator_t& _a_rhs
            ) const
    {
        __cmp_test(_m_test_index);
        __cmp_test(_m_stack);
        return true;
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
        return *_m_stack.top()->_m_nodes_tests[_m_test_index];
    }
    _END_ABC_DS_NS