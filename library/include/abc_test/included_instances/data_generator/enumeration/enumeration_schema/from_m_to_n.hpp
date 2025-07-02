#pragma once

#include "abc_test/included_instances/data_generator/enumeration/default_enumeration.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_to_n_inner.hpp"
#include <optional>

_BEGIN_ABC_NS
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            all_values() noexcept;

template <typename T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_m_to_n(const T& _a_lower, const T& _a_upper) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_val_to_max(const T& _a_value) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_max_to_val(
                const T&                      _a_valueconst,
                _ABC_NS_DG::enumeration_t<T>& _a_enumerate_base
            ) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_val_to_min(const T& _a_value) noexcept;
template <typename T>
// requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_min_to_val(const T& _a_value) noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            all_values_moving_forward() noexcept;
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            all_values_moving_backward() noexcept;
template <typename T>
__constexpr _ABC_NS_DG::enumeration_schema_t<T>
            from_m_to_n_using_k_values(
                const T&          _a_lower,
                const T&          _a_upper,
                const std::size_t _a_k
            ) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                all_values() noexcept
{
    return all_values(default_enumeration<T>());
}

template <typename T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_m_to_n(
                    const T& _a_lower,
                    const T& _a_upper
                ) noexcept
{
    auto kt = abc::default_enumeration<T>();
    return from_m_to_n(_a_lower, _a_upper, abc::default_enumeration<T>());
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_val_to_max(
                    const T& _a_value
                ) noexcept
{
    return from_val_to_max(_a_value, default_enumeration<T>());
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_max_to_val(
                    const T& _a_value
                ) noexcept
{
    return from_max_to_val(_a_value, default_enumeration<T>());
}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_val_to_min(
                    const T& _a_value
                ) noexcept
{
    return from_val_to_min(_a_value, default_enumeration<T>());
}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_min_to_val(
                    const T& _a_value
                ) noexcept
{
    return from_min_to_val(_a_value, default_enumeration<T>());
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                all_values_moving_forward() noexcept
{
    return all_values_moving_forward(default_enumeration<T>());
}

template <typename T>
requires _ABC_NS_UTILITY::max_value_c<T> && _ABC_NS_UTILITY::min_value_c<T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                all_values_moving_backward() noexcept
{
    return all_values_moving_backward(default_enumeration<T>());
}

template <typename T>
__constexpr_imp _ABC_NS_DG::enumeration_schema_t<T>
                from_m_to_n_using_k_values(
                    const T&          _a_lower,
                    const T&          _a_upper,
                    const std::size_t _a_k
                ) noexcept
{
    return from_m_to_n_using_k_values(
        _a_lower, _a_upper, _a_k, default_enumeration<T>()
    );
}

_END_ABC_NS