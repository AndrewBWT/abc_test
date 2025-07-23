#pragma once

#include "abc_test/core/data_generator/data_generator.hpp"
#include "abc_test/core/data_generator/data_generator_collection.hpp"
#include "abc_test/core/data_generator/data_generator_collection_sentinel.hpp"
#include "abc_test/core/data_generator/typeless_data_generator_collection_iterator.hpp"
_BEGIN_ABC_NS
struct test_evaluator_t;
_END_ABC_NS
_BEGIN_ABC_DG_NS
// Forward declarations
template <typename T>
struct data_generator_collection_t;

/*!
 * @brief Iterator object used to iterate over data data_generator_collection_t
 * objects.
 * @tparam T The type of object which is produced from this iterator.
 */
template <typename T>
struct data_generator_collection_iterator_t
    : public typeless_data_generator_collection_iterator_t
{
public:
    /*!
     * @brief The difference type of the iterator.
     */
    using difference_type = std::ptrdiff_t;
    /*!
     * @brief The value type of the iterator.
     */
    using value_type      = T;
    /*!
     * @brief The reference type of the iterator.
     */
    using reference
        = const data_generator_collection_iterator_t<T>::value_type&;
    /*!
     * @brief Constructor.
     * @param _a_begin_iterator The begin iterator for the iterator.
     * @param _a_iterator_length The number of elements in the iterator.
     * @param _a_test_runner The associated test_runner.
     */
    __constexpr
    data_generator_collection_iterator_t(
        dgc_internal_itt_t<T> _a_begin_iterator,
        const std::size_t     _a_iterator_length
    );
    /*!
     * @brief Destructor.
     *
     * Runs bespoke code to ensure that the associated for loop data is kept
     * consistent with the current set of gen_data_collection_t elements.
     */
    __constexpr ~data_generator_collection_iterator_t();
    /*!
     * @brief Increment operator.
     *
     * Increments the iterator to the next element.
     * @return A reference to this object.
     */
    __constexpr data_generator_collection_iterator_t<T>&
                operator++();
    /*!
     * @brief Increments the iterator.
     * @param  Dummy paramter.
     * @return A new object with the element incremented.
     */
    __constexpr data_generator_collection_iterator_t<T>
                operator++(int);
    /*!
     * @brief Equality comparison operator.
     * @param _a_rhs The sentinel to compare this element to.
     * @return True if this iterator is at the end of its range; false
     * otherwise.
     */
    __constexpr bool
        operator==(const data_generator_collection_sentinel_t& _a_rhs
        ) const noexcept;
    /*!
     * @brief Dereference operator.
     * @return Gets a cref to the current T element.
     */
    __constexpr reference
        operator*() const;
private:
    dgc_internal_itt_t<T>                     _m_this_iterator;
    ds::opt_idgc_memoized_element_t           _m_repetition_data;
    size_t                                    _m_this_iterators_index;
    bool                                      _m_add_repeatable_test_config;
    std::size_t                               _m_iterator_length;
    mutable std::optional<logging::log_msg_t> _m_log_msg;
    /*!
     * Increment iterator using an optional set of repetition data.
     */
    __constexpr void
        increment_iterator(
            const ds::idgc_memoized_element_t& _a_opt_repetition_data
        );
    __constexpr virtual ds::dg_memoized_element_t
        generate_data_generator_memoized_element(
            const bool _a_get_original_dg_memoized_element_data
        ) const noexcept;
    __constexpr bool
        at_end_of_iterator() const noexcept;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp
    data_generator_collection_iterator_t<T>::
        data_generator_collection_iterator_t(
            dgc_internal_itt_t<T> _a_begin_iterator,
            const std::size_t     _a_iterator_length
        )
    : _m_this_iterator(_a_begin_iterator)
    , _m_this_iterators_index{0}
    , _m_iterator_length(_a_iterator_length)
{
    using namespace std;
    using namespace ds;
    using namespace errors;
    invoked_test_data_t& _l_current_test{global::get_this_threads_current_test()};
    // If this is a repetition.
    if (_l_current_test.post_setup_test_data().has_for_loop_stack_trie())
    {
        // Get the first element in the for loop stack and move the iterator to
        // that element.
        const opt_idgc_memoized_element_t _l_initial_idgc_element{
            _l_current_test.get_first_child_of_current_for_loop_stack()
        };
        if (_l_initial_idgc_element.has_value())
        {
            increment_iterator(_l_initial_idgc_element.value());
        }
        else
        {
            this->_m_generation_collection_index = _m_iterator_length;
            // throw errors::test_library_exception_t(fmt::format(
            // u8"Could not initialise tests for loop stack. For loop stack = "
            // u8"{0}",
            // checkless_convert_ascii_to_unicode<u8string>(
            //    fmt::format("{}", _l_current_test.for_loop_stack_trie())
            // )
            // ));
        }
    }
    // If not at the end, increment the current test's CURRENT for loop stack
    // too.
    _l_current_test.increment_for_loop_stack(std::ref(*this));
}

template <typename T>
__constexpr_imp data_generator_collection_iterator_t<
    T>::~data_generator_collection_iterator_t()
{
    global::get_this_threads_current_test().decrement_for_loop_stack();
}

template <typename T>
__constexpr_imp data_generator_collection_iterator_t<T>&
                data_generator_collection_iterator_t<T>::operator++()
{
    using namespace std;
    using namespace ds;
    typeless_data_generator_collection_iterator_t::reset();
    invoked_test_data_t& _l_current_test{global::get_this_threads_current_test()
    };
    // If we're in a repetition, get the next element in the for loop stack...
    if (_l_current_test.post_setup_test_data().has_for_loop_stack_trie())
    {
        const opt_idgc_memoized_element_t _l_opt_idgcme{
            _l_current_test.increment_last_value_of_current_for_loop_stack()
        };
        if (_l_opt_idgcme.has_value())
        {
            increment_iterator(_l_opt_idgcme.value());
        }
        else
        {
            ++_m_generation_collection_index;
            // throw errors::test_library_exception_t(fmt::format(
            // "Could not increment tests for loop stack. For loop stack = "
            // "{0}",
            // _l_current_test.for_loop_stack_trie()
            // ));
        }
    }
    else
    {
        // Else move to the next generated element.
        const bool _l_has_next_element{(*_m_this_iterator)->generate_next()};
        if (_l_has_next_element)
        {
            ++_m_this_iterators_index;
        }
        else
        {
            // Check to ensure the new iterator actually has elements
            _m_this_iterators_index = 0;
            bool _l_iterator_has_elements{false};
            // Move through the elements until we find one with an element.
            do
            {
                ++_m_this_iterator;
                ++this->_m_generation_collection_index;
                if (at_end_of_iterator())
                {
                    break;
                }
                else if ((*_m_this_iterator)->has_current_element())
                {
                    _l_iterator_has_elements = true;
                }
            }
            while (not _l_iterator_has_elements);
        }
    }
    if (not at_end_of_iterator())
    {
        _l_current_test.update_for_loop_stack(std::ref(*this));
    }
    return *this;
}

template <typename T>
__constexpr_imp data_generator_collection_iterator_t<T>
                data_generator_collection_iterator_t<T>::operator++(
        int
    )
{
    auto _l_cpy{*this};
    ++(*this);
    return _l_cpy;
}

template <typename T>
__constexpr_imp bool
    data_generator_collection_iterator_t<T>::operator==(
        const data_generator_collection_sentinel_t& _a_rhs
    ) const noexcept
{
    return at_end_of_iterator();
}

template <typename T>
__constexpr_imp data_generator_collection_iterator_t<T>::reference
                data_generator_collection_iterator_t<T>::operator*() const
{
    if (at_end_of_iterator()
        || (*_m_this_iterator)->has_current_element() == false)
    {
        throw std::runtime_error("Could not deference iterator");
    }
    else
    {
        auto&     _l_this_iterator_ref{*_m_this_iterator};
        reference _l_current_element{_l_this_iterator_ref->current_element()};
        _m_log_msg = logging::log_msg_t(
            ds::single_source_t(),
            fmt::format(
                u8"{0} = {1}",
                u8"data generator's value",
                abc::utility::printer::default_printer_t<
                    std::remove_cvref_t<decltype(_l_current_element)>>()
                    .run_printer(_l_current_element)
            ),
            false
        );
        return _l_current_element;
    }
}

template <typename T>
__constexpr_imp void
    data_generator_collection_iterator_t<T>::increment_iterator(
        const ds::idgc_memoized_element_t& _a_opt_repetition_data
    )
{
    using namespace ds;
    // if (not _a_opt_repetition_data.has_value())
    // {
    //     throw errors::test_library_exception_t("");
    // }
    // else
    {
        const idgc_memoized_element_t& _l_rd{_a_opt_repetition_data};
        size_t                         _l_generation_collection_index{
            _l_rd.for_loop_iteration_data.generation_collection_index
        };
        // If less than the element, move forewards.
        while (this->_m_generation_collection_index
               < _l_generation_collection_index)
        {
            ++this->_m_generation_collection_index;
            ++_m_this_iterator;
        }
        // If greater than the element move backwards.
        while (this->_m_generation_collection_index
               > _l_generation_collection_index)
        {
            --this->_m_generation_collection_index;
            --++_m_this_iterator;
        }
        dgc_element_t<T>& _l_this_iterator_ref{*_m_this_iterator};
        _l_this_iterator_ref
            ->set_data_generator_using_data_generator_memoized_element(
                _l_rd.for_loop_iteration_data.flied
            );
    }
}

template <typename T>
__constexpr_imp ds::dg_memoized_element_t
                data_generator_collection_iterator_t<T>::
        generate_data_generator_memoized_element(
            const bool _a_get_original_dg_memoized_element_data
        ) const noexcept
{
    return (*_m_this_iterator)
        ->get_data_generator_memoized_element(
            _a_get_original_dg_memoized_element_data
        );
}

template <typename T>
__constexpr_imp bool
    data_generator_collection_iterator_t<T>::at_end_of_iterator() const noexcept
{
    return this->_m_generation_collection_index == _m_iterator_length;
}

_END_ABC_DG_NS