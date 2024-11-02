#pragma once

#include "abc_test/internal/utility/internal/macros.h"

#include <memory>

_BEGIN_ABC_DG_NS
template <typename T>
class enumeration_data_object_t;

template <typename T>
class enumeration_schema_t
{
public:
    __constexpr
    enumeration_schema_t(const T& _a_start_value) noexcept;
    __constexpr
    enumeration_schema_t() noexcept
        = delete;
    __constexpr T virtual end_value(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept
        = 0;
    __constexpr bool virtual is_direction_forward(
        const std::shared_ptr<enumeration_data_object_t<T>>& _a_edo
    ) const noexcept
        = 0;
    __constexpr const T&
        start_value() const noexcept;
protected:
    T _m_start_value;
};

_END_ABC_DG_NS
_BEGIN_ABC_NS

_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr
enumeration_schema_t<T>::enumeration_schema_t(
    const T& _a_start_value
) noexcept
    : _m_start_value(_a_start_value)
{}

template <typename T>
__constexpr const T&
    enumeration_schema_t<T>::start_value() const noexcept
{
    return _m_start_value;
}

_END_ABC_DG_NS
_BEGIN_ABC_NS
_END_ABC_NS