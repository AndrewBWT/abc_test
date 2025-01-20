#pragma once

#include "abc_test/internal/data_generator/data_generator_collection.hpp"
#include "abc_test/internal/errors/test_library_exception.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"

/*!
 * Code to run a manual for loop iteration. The manual_for_loop object is given
 * as the first argument and the code to be ran as the second.
 */
#define RUN(_a_manual_for_loop, _a_code) \
    if (_a_manual_for_loop.run_next())   \
    {                                    \
        _a_code;                         \
    }                                    \
    ++_a_manual_for_loop;


_BEGIN_ABC_NS

struct manual_data_generator_t
    : public _ABC_NS_DG::typeless_data_generator_collection_iterator_t
{
public:
    using tertiary_type = std::size_t;
    /*!
     * Default constructor for the manual_for_loop_t object.
     */
    __no_constexpr
    manual_data_generator_t() noexcept;
    /*!
     * Destructor for the object.
     *
     * Behind the scenes, this function tidies up the for loop data which stores
     * repetition data.
     */
    __constexpr ~manual_data_generator_t() noexcept;
    /*!
     * Tells the caller whether the next test is to be ran.
     */
    __constexpr bool
        run_next();
    /*!
     * Increment element
     */
    __no_constexpr manual_data_generator_t&
        operator++();
    __no_constexpr manual_data_generator_t
        operator++(int);
private:
    std::size_t                 _m_idx;
    bool                        _m_should_idx_be_ran;
    ds::idgc_memoized_element_t _m_repetition_data;
    __constexpr ds::dg_memoized_element_t
    generate_data_generator_memoized_element(
        const bool _a_get_data_for_repeating
    ) const noexcept;
};

_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp
    manual_data_generator_t::manual_data_generator_t() noexcept
    : _m_idx{0}
    , _m_should_idx_be_ran{false}
    , _m_repetition_data(ds::idgc_memoized_element_t(
          0,
        ds::dgc_memoized_element_t{ 0,
          ds::dg_memoized_element_t{0,
        abc::utility::printer::print(0) } }
      ))
{
    using namespace global;
    using namespace ds;
    invoked_test_data_t& _l_current_test{get_this_threads_current_test()};
    _l_current_test.increment_for_loop_stack(*this);
    _m_should_idx_be_ran = _l_current_test.is_current_for_loop_stack_in_true();
}

__constexpr_imp manual_data_generator_t::~manual_data_generator_t() noexcept
{
    global::get_this_threads_current_test().decrement_for_loop_stack();
}

__constexpr_imp bool
    manual_data_generator_t::run_next()
{
    bool _l_rv{_m_should_idx_be_ran};
    return _l_rv;
}

/*!
 * Increment element
 */
__no_constexpr_imp manual_data_generator_t&
    manual_data_generator_t::operator++()
{
    using namespace std;
    using namespace global;
    using namespace ds;
    _m_idx++;
    invoked_test_data_t& _l_current_test{get_this_threads_current_test()};
    _l_current_test.update_for_loop_stack(*this);
    _m_should_idx_be_ran = _l_current_test.is_current_for_loop_stack_in_true();
    return *this;
}

/*!
 * Increment element
 */
__no_constexpr_imp manual_data_generator_t
    manual_data_generator_t::operator++(
        int
    )
{
    auto _l_cpy{*this};
    ++(*this);
    return _l_cpy;
}
__constexpr ds::dg_memoized_element_t
manual_data_generator_t::generate_data_generator_memoized_element(
    const bool _a_get_data_for_repeating
) const noexcept
{
    return { 0,abc::utility::printer::print(_m_idx) };
}
_END_ABC_NS