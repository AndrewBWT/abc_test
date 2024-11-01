#pragma once

#pragma once

#include "abc_test/internal/ds/data_generator_memoization/data_generator_collection_memoized_element.h"
#include "abc_test/internal/ds/data_generator_memoization/data_generator_memoized_element.h"
#include "abc_test/internal/utility/internal/macros.h"


_BEGIN_ABC_DG_NS

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
     * @return The dgc_memoized_element_t representative of the iterator at this
     * point.
     */
    __constexpr ds::dgc_memoized_element_t
                get_data_generator_memoized_element(
                    const bool _a_write_data,
                    const bool _a_get_data_for_repeating
                ) noexcept;
    __constexpr void
        reset() noexcept;
protected:
    __constexpr virtual ds::dg_memoized_element_t
        generate_data_generator_memoized_element(
            const bool _a_get_data_for_repeating
        ) const noexcept
        = 0;
protected:
    std::size_t                   _m_generation_collection_index{0};
    ds::opt_dg_memoized_element_t _m_flid;
    ds::opt_dg_memoized_element_t _m_repeat_flid;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

__constexpr_imp void
    typeless_data_generator_collection_iterator_t::reset() noexcept
{
    using namespace ds;
    _m_flid        = opt_dg_memoized_element_t{};
    _m_repeat_flid = opt_dg_memoized_element_t{};
}

__constexpr_imp ds::dgc_memoized_element_t
                typeless_data_generator_collection_iterator_t::
        get_data_generator_memoized_element(
            const bool _a_write_data,
            const bool _a_get_original_dg_memoized_element_data
        ) noexcept
{
    using namespace ds;
    if (_a_write_data)
    {
        opt_dg_memoized_element_t& _l_opt_dg{
            _a_get_original_dg_memoized_element_data ? _m_flid : _m_repeat_flid
        };
        if (not _l_opt_dg.has_value())
        {
            _l_opt_dg = generate_data_generator_memoized_element(
                _a_get_original_dg_memoized_element_data
            );
        }
        return dgc_memoized_element_t{
            _m_generation_collection_index, _l_opt_dg.value()
        };
    }
    else
    {
        dg_memoized_element_t _l_dg = generate_data_generator_memoized_element(
            _a_get_original_dg_memoized_element_data
        );
        return dgc_memoized_element_t{
    _m_generation_collection_index, _l_dg
        };
    }
}

_END_ABC_DG_NS