#pragma once

#include "abc_test/included_instances/data_generator/enumeration_schema.h"

#include <optional>

_BEGIN_ABC_NS
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
class enumeration_data_generator_t;

template <typename T>
class enumeration_schema_from_m_move_k_elements_t
    : public enumeration_schema_t<T>
{
public:
    __constexpr
    enumeration_schema_from_m_move_k_elements_t(
        const T& _a_start_value,
        const typename enumeration_data_generator_t<T>::tertiary_type&
                   _a_elements,
        const bool _a_forward_direction
    ) noexcept;
    __constexpr T virtual end_value(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept override;
    __constexpr bool virtual is_direction_forward(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept override;
private:
    bool             _m_is_direction_forwards;
    std::optional<T> _m_end_value;
    typename enumeration_data_generator_t<T>::tertiary_type& _m_elements;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS

template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_enumerate_k_elements(
                    const T& _a_start_value,
                    const typename _ABC_NS_DG::enumeration_data_generator_t<
                        T>::tertiary_type& _a_elements,
                    const bool             _a_forward_direction
                ) noexcept;
template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_move_forward_k_elements(
                    const T& _a_start_value,
                    const typename _ABC_NS_DG::enumeration_data_generator_t<
                        T>::tertiary_type& _a_elements
                ) noexcept;
template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_move_backward_k_elements(
                    const T& _a_start_value,
                    const typename _ABC_NS_DG::enumeration_data_generator_t<
                        T>::tertiary_type& _a_elements
                ) noexcept;

_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp
    enumeration_schema_from_m_move_k_elements_t<T>::
        enumeration_schema_from_m_move_k_elements_t(
            const T& _a_start_value,
            const typename _ABC_NS_DG::enumeration_data_generator_t<
                T>::tertiary_type& _a_elements,
            const bool             _a_forward_direction
        ) noexcept
    : enumeration_schema_t<T>(_a_start_value)
    , _m_is_direction_forwards(_a_forward_direction)
    , _m_elements(_a_elements)
{}

template <typename T>
__constexpr_imp T
    enumeration_schema_from_m_move_k_elements_t<T>::end_value(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept
{
    if (not _m_end_value.has_value())
    {
        enumerate_index_t _l_n_times_to_call{_a_elements._m_elements};
        if constexpr (max_value_c<T>)
        {
            if constexpr (min_value_c<T>)
            {
                bool _l_pc = _m_forward ? _m_enumerable->increment(
                                              _m_end_value,
                                              _l_n_times_to_call,
                                              max_value_t<T>().max_value()
                                          )
                                        : _m_enumerable->decrement(
                                              _m_end_value,
                                              _l_n_times_to_call,
                                              min_value_t<T>().min_value()
                                          );
            }
            else
            {
                bool _l_pc = _m_forward ? _m_enumerable->increment(
                                              _m_end_value,
                                              _l_n_times_to_call,
                                              max_value_t<T>().max_value()
                                          )
                                        : _m_enumerable->decrement(
                                              _m_end_value, _l_n_times_to_call
                                          );
            }
        }
        else
        {
            if constexpr (min_value_c<T>)
            {
                bool _l_pc = _m_forward ? _m_enumerable->increment(
                                              _m_end_value, _l_n_times_to_call
                                          )
                                        : _m_enumerable->decrement(
                                              _m_end_value,
                                              _l_n_times_to_call,
                                              min_value_t<T>().min_value()
                                          );
            }
            else
            {
                bool _l_pc = _m_forward ? _m_enumerable->increment(
                                              _m_end_value, _l_n_times_to_call
                                          )
                                        : _m_enumerable->decrement(
                                              _m_end_value, _l_n_times_to_call
                                          );
            }
        }
    }
    return _m_end_value.value();
}

template <typename T>
__constexpr_imp bool
    enumeration_schema_from_m_move_k_elements_t<T>::is_direction_forward(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept
{
    return _m_is_direction_forwards;
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_enumerate_k_elements(
                    const T& _a_start_value,
                    const typename _ABC_NS_DG::enumeration_data_generator_t<
                        T>::tertiary_type& _a_elements,
                    const bool             _a_forward_direction
                ) noexcept
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return make_shared<enumeration_schema_from_m_move_k_elements_t<T>>(
        _a_start_value, _a_elements, _a_forward_direction
    );
}

template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_move_forward_k_elements(
                    const T& _a_start_value,
                    const typename _ABC_NS_DG::enumeration_data_generator_t<
                        T>::tertiary_type& _a_elements
                ) noexcept
{
    return from_m_enumerate_k_elements(_a_start_value, _a_elements, true);
}

template <typename T>
__constexpr_imp std::shared_ptr<_ABC_NS_DG::enumeration_schema_t<T>>
                from_m_move_backward_k_elements(
                    const T& _a_start_value,
                    const typename _ABC_NS_DG::enumeration_data_generator_t<
                        T>::tertiary_type& _a_elements
                ) noexcept
{
    return from_m_enumerate_k_elements(_a_start_value, _a_elements, false);
}

_END_ABC_NS