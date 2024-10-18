#pragma once

#pragma once

#include "abc_test/core/ds/data_generator_memoization/data_generator_collection_memoized_element.h"
#include "abc_test/core/ds/data_generator_memoization/data_generator_memoized_element.h"
#include "abc_test/utility/internal/macros.h"


_BEGIN_ABC_NS

/*!
 * @brief Abstract object used to hold data generator information.
 *
 * This is designed to be a typeless class - the data members are encoded in
 * strings, and these are able to be generated when required.
 */
struct typeless_data_generator_collection_iterator_t
{
public:
    /*!
     * @brief Returns the dgc_memoized_element_t element to the caller.
     * @param _a_get_data_for_repeating Whether to get the data for repeating
     * the test, or the original data.
     * @return The dgc_memoized_element_t representative of the iterator at this point.
     */
    __constexpr ds::dgc_memoized_element_t
        get_data_generator_memoized_element(const bool _a_get_data_for_repeating
        ) noexcept;
protected:
    __constexpr virtual ds::dg_memoized_element_t
        generate_data_generator_memoized_element(
            const bool _a_get_data_for_repeating
        ) const noexcept
        = 0;
private:
    std::size_t                   _m_generation_collection_index;
    ds::opt_dg_memoized_element_t _m_flid;
    ds::opt_dg_memoized_element_t _m_repeat_flid;
};

_END_ABC_NS

_BEGIN_ABC_NS

__constexpr_imp ds::dgc_memoized_element_t
                typeless_data_generator_collection_iterator_t::
        get_data_generator_memoized_element(
            const bool _a_get_data_for_repeating
        ) noexcept
{
    using namespace ds;
    opt_dg_memoized_element_t& _l_opt_dg{
        _a_get_data_for_repeating ? _m_repeat_flid : _m_flid
    };
    if (not _l_opt_dg.has_value())
    {
        _l_opt_dg
            = generate_data_generator_memoized_element(_a_get_data_for_repeating
            );
    }
    return dgc_memoized_element_t{
        _m_generation_collection_index, _l_opt_dg.value()
    };
}

_END_ABC_NS