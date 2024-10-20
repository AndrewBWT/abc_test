#pragma once

#include "abc_test/internal/ds/data_generator_memoization/indexed_data_generator_collection_memoized_element.h"
#include "abc_test/internal/errors/test_library_exception.h"
#include "abc_test/internal/data_generator/typeless_data_generator_collection_iterator.h"
#include "fmt/base.h"
#include "fmt/ranges.h"


_BEGIN_ABC_DS_NS
/*!
 * @brief Type synonym for a refernece wrapper containing a
 * gen_data_collection_iterator_agnostic_data_t object.
 */
using tdg_collection_itt_ref_t
    = std::reference_wrapper<_ABC_NS_DG::typeless_data_generator_collection_iterator_t>;

/*!
 * @brief This structure holds the current set of
 * gen_data_collection_agnostic_iterator_t's which allow the library to refer to
 * the current set of gen_data_t elements a test has evaluated.
 *
 * To be clear, this structure does not hold any generated data, instead it
 * holds a set of pointers to the currently active
 * gen_data_collection_agnostic_iterator_t elements. It is strictly designed to
 * be used when active tests are running.
 */
struct typeless_data_generator_collection_stack_t
{
public:
    /*!
     * @brief Default constructor.
     *
     * This is the only constructor.
     *
     */
    __constexpr
        typeless_data_generator_collection_stack_t() noexcept
        = default;
    // Can't be constexpr due to use of rest_failure_func_t argument.
    /*!
     * @brief Adds a gen_data_collection_iterator_agnostic_data_t to the
     * internal collection.
     *
     * Used when a new gen_data_collection_t element is entered.
     *
     * @param _a_ptr The pointer to add to the internal collection.
     */
    __no_constexpr void
        increment(tdg_collection_itt_ref_t _a_ptr) noexcept;
    /*!
     * Removes the current element from the for loop stack, and updates the for
     * loop counter.
     */
    /*!
     * @brief Removes the current last element in the internal
     * gen_data_collection_iterator_agnostic_data_t collection.
     *
     * Used when a gen_data_collection_t element is exited.
     *
     * If the internal collection's size is zero, this function throws an
     * exception.
     */
    __constexpr void
        decrement();
    /*!
     * Updates the last elemnet in the internal stack with the given data.
     */
    /*!
     * @brief Swaps the last element in the internal collection of
     * gen_data_collection_iterator_agnostic_data_t with the argument.
     *
     * @param _a_ptr The pointer to add to the internal collection.
     */
    __no_constexpr void
        update(tdg_collection_itt_ref_t _a_ptr);
    /*!
     * Calls all the for loop element's failure functions.
     *
     * Also generates the repetition_data_sequence_t object for this specific
     * error. This in turn is added to the current tests repetition data, which
     * in turn is delivered to the user to aid in debugging failing test cases.
     */
    /*!
     * @brief Creates a for_loop_creation_data_sequence_t object which can be
     * used to re-run the test and create the same elemnets from each gen_data_t
     * element.
     *
     * The returned item represents one point in a hierarchy of
     * gen_data_collection_t elements, rather than a set of them. Many of these
     * can be composed to create a set of data which can be used to selectively
     * only certain data. In our system, it is used to only repeat parts of
     * tests which raised failed assertions.
     *
     * The reader should note that the returned element is distinctly different
     * from create_data_sequence. That function creates the exact data sequence
     * as the gen_data_t's created. This function allows the gen_data_t elements
     * to (potentially) return different data.
     *
     * For example, a gen_data_t element may memoize some data to a file. The
     * for_loop_creation_data_t element would return the correct mode and data
     * string to access the data from the file, rather than create it as the
     * gen_data_t element originally did.
     *
     * @return for_loop_creation_data_sequence_t which represents a specific
     * sequence of elements in a sequence of gen_data_t elements.
     */
    __constexpr ds::idgc_memoized_element_sequence_t
                create_data_sequence_for_repetition() const noexcept;
    /*!
     * @brief Returns a for_loop_creation_data_sequence_t element from the
     * internal collection of gen_data_collection_iterator_agnostic_data_t
     * elements.
     *
     * This function essentially creates copies of the data contained in each
     * gen_data_collection_iterator_agnostic_data_t element.
     *
     * @return for_loop_creation_data_sequence_t which represents a specific
     * sequence of elements in a sequence of gen_data_t elements.
     */
    __constexpr idgc_memoized_element_sequence_t
        create_data_sequence() const noexcept;
    /*!
     * @brief Returns the current for lopo index to the caller.
     *
     * Internally, what ordered numerical identifier is assigned to the current
     * for loop is also maintained in this object. This function returns the
     * current one to the caller.
     *
     * @return The numerical identifier assigned to the current for loop.
     */
    __constexpr std::size_t
                current_for_loop_index() const noexcept;
private:
    std::vector<tdg_collection_itt_ref_t> _m_for_loop_pointers;
    std::vector<std::size_t>              _m_current_for_loop_indexes = {0};
};

using tdg_collection_stack_t = typeless_data_generator_collection_stack_t;
_END_ABC_DS_NS

template <>
struct fmt::formatter<_ABC_NS_DS::typeless_data_generator_collection_stack_t> : formatter<string_view>
{
    /*!
     * Provides a formatter for a for_loop_stack_t object
     */
    // can't be constexpr due to calling fmt::format
    __no_constexpr auto
        format(_ABC_NS_DS::typeless_data_generator_collection_stack_t _a_rt, format_context& _a_cxt)
            const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS

__no_constexpr_imp void
typeless_data_generator_collection_stack_t::increment(
        tdg_collection_itt_ref_t _a_ptr
    ) noexcept
{
    _m_for_loop_pointers.push_back(_a_ptr);
    _m_current_for_loop_indexes.push_back(0);
}

__constexpr_imp void
typeless_data_generator_collection_stack_t::decrement()
{
    using namespace errors;
    if (_m_for_loop_pointers.size() > 0)
    {
        _m_current_for_loop_indexes.pop_back();
        _m_current_for_loop_indexes.back()++;
        _m_for_loop_pointers.pop_back();
    }
    else
    {
        throw test_library_exception_t(fmt::format(
            "decrement called when _m_for_loop_pointers.size() == 0"
        ));
    }
}

__no_constexpr_imp void
typeless_data_generator_collection_stack_t::update(
        tdg_collection_itt_ref_t _a_ptr
    )
{
    using namespace errors;
    if (_m_for_loop_pointers.size() > 0)
    {
        _m_for_loop_pointers.back() = _a_ptr;
    }
    else
    {
        throw test_library_exception_t(
            fmt::format("update called when _m_for_loop_pointers.size() == 0")
        );
    }
}

__constexpr_imp ds::idgc_memoized_element_sequence_t
typeless_data_generator_collection_stack_t::create_data_sequence_for_repetition() const noexcept
{
    using namespace std;
    using namespace ds;
    idgc_memoized_element_sequence_t _l_rds{};
    for (const tdg_collection_itt_ref_t& _l_element : _m_for_loop_pointers)
    {
        _l_rds.push_back(idgc_memoized_element_t{
            _m_current_for_loop_indexes.back(),
            _l_element.get().get_data_generator_memoized_element(true)
        });
    }
    return _l_rds;
}

__constexpr_imp idgc_memoized_element_sequence_t
typeless_data_generator_collection_stack_t::create_data_sequence() const noexcept
{
    idgc_memoized_element_sequence_t _l_rds{};
    for (auto& _l_element : _m_for_loop_pointers)
    {
        _l_rds.push_back(idgc_memoized_element_t{
            _m_current_for_loop_indexes.back(),
            _l_element.get().get_data_generator_memoized_element(false)
        });
    }
    return _l_rds;
}

__constexpr_imp std::size_t
                tdg_collection_stack_t::current_for_loop_index() const noexcept
{
    return _m_current_for_loop_indexes.back();
}

_END_ABC_DS_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::typeless_data_generator_collection_stack_t>::format(
        _ABC_NS_DS::typeless_data_generator_collection_stack_t _a_fldc,
        format_context&                 _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        "}}",
        typeid(_a_fldc).name(),
        "_m_current_for_loop_indexes",
        _a_fldc.current_for_loop_index()
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}