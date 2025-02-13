#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"

#include <memory>

_BEGIN_ABC_DG_NS
template <typename T>
class enumeration_base_t;
template<typename T>
using enumeration_t = std::shared_ptr<enumeration_base_t<T>>;

template <typename T>
class enumeration_schema_base_t
{
public:
    __constexpr
        enumeration_schema_base_t(const T& _a_start_value) noexcept;
    __constexpr
        enumeration_schema_base_t() noexcept
        = delete;
    __constexpr T virtual end_value(
        const enumeration_t<T>& _a_edo
    ) const noexcept
        = 0;
    __constexpr bool virtual is_direction_forward(
        const enumeration_t<T>& _a_edo
    ) const noexcept
        = 0;
    __no_constexpr enumerate_index_t
                number_of_complete_advancements(
                    const enumeration_t<T>& _a_edo
                ) const noexcept;
    __no_constexpr enumerate_index_t
                remaining_entities_after_maximum_advancements(
                    const enumeration_t<T>& _a_edo
                ) const noexcept;
    __constexpr const T&
                start_value() const noexcept;
    __no_constexpr virtual enumerate_index_t
                n_advancements_per_advancement(
                    const enumeration_t<T>& _a_edo
                ) const noexcept;
protected:
    T _m_start_value;
};
template<typename T>
using enumeration_schema_t = std::shared_ptr< enumeration_schema_base_t<T>>;

_END_ABC_DG_NS
_BEGIN_ABC_NS

_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp
enumeration_schema_base_t<T>::enumeration_schema_base_t(
        const T& _a_start_value
    ) noexcept
    : _m_start_value(_a_start_value)
{}

template <typename T>
__no_constexpr_imp enumerate_index_t
enumeration_schema_base_t<T>::number_of_complete_advancements(
        const enumeration_t<T>& _a_edo
    ) const noexcept
{
    return _a_edo->difference(_m_start_value, end_value(_a_edo)).first;
}

template <typename T>
__no_constexpr_imp enumerate_index_t
enumeration_schema_base_t<T>::remaining_entities_after_maximum_advancements(
        const enumeration_t<T >& _a_edo
    ) const noexcept
{
    return _a_edo->difference(_m_start_value, end_value(_a_edo)).second;
}

template <typename T>
__constexpr_imp const T&
enumeration_schema_base_t<T>::start_value() const noexcept
{
    return _m_start_value;
}
template <typename T>
__no_constexpr_imp enumerate_index_t
enumeration_schema_base_t<T>::n_advancements_per_advancement(
    const enumeration_t<T>& _a_edo
) const noexcept
{
    using namespace std;
    return enumerate_index_t{ 1 };
}
_END_ABC_DG_NS
_BEGIN_ABC_NS
_END_ABC_NS