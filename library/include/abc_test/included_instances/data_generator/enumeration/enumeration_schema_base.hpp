#pragma once

#include "abc_test/utility/internal/macros.hpp"

#include <memory>

_BEGIN_ABC_DG_NS
template <typename T>
struct enumeration_base_t;
template <typename T>
using enumeration_t = std::shared_ptr<enumeration_base_t<T>>;

template <typename T>
class enumeration_schema_base_t
{
public:
    __constexpr virtual ~enumeration_schema_base_t() = default;
    __constexpr
    enumeration_schema_base_t(
        const T&                _a_start_value,
        const enumeration_t<T>& _a_enumeration
    ) noexcept;
    __constexpr
    enumeration_schema_base_t() noexcept
        = delete;
    __constexpr T virtual end_value() const noexcept               = 0;
    __constexpr bool virtual is_direction_forward() const noexcept = 0;
    __no_constexpr enumerate_index_t
        number_of_complete_advancements() const noexcept;
    __no_constexpr enumerate_index_t
        remaining_entities_after_maximum_advancements() const noexcept;
    __constexpr const T&
        start_value() const noexcept;
    __no_constexpr virtual enumerate_index_t
        n_advancements_per_advancement() const noexcept;
    template <typename R>
    __constexpr R
        enumerate_all_values() const noexcept;

    __constexpr_imp bool
        less_than(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _m_enumeration->less_than(_a_l, _a_r);
    }

    __constexpr_imp bool
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _m_enumeration->equal(_a_l, _a_r);
    }

    __constexpr bool
        increment(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        )
    {
        return _m_enumeration->increment(
            _a_element, _a_n_times_to_increment, _a_max_value
        );
    }

    __constexpr virtual bool
        decrement(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        )
    {
        return _m_enumeration->decrement(
            _a_element, _a_n_times_to_increment, _a_max_value
        );
    }
    __constexpr virtual enumeration_diff_t
        difference(const T& _a_arg1, const T& _a_arg2) noexcept
    {
        return _m_enumeration->difference(_a_arg1, _a_arg2);
    }
protected:
    T                _m_start_value;
    enumeration_t<T> _m_enumeration;
};

template <typename T>
using enumeration_schema_t = std::shared_ptr<enumeration_schema_base_t<T>>;

_END_ABC_DG_NS
_BEGIN_ABC_NS

_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp
    enumeration_schema_base_t<T>::enumeration_schema_base_t(
        const T&                _a_start_value,
        const enumeration_t<T>& _a_enumeration
    ) noexcept
    : _m_start_value(_a_start_value), _m_enumeration(_a_enumeration)
{}

template <typename T>
__no_constexpr_imp enumerate_index_t
    enumeration_schema_base_t<T>::number_of_complete_advancements(
    ) const noexcept
{
    return _m_enumeration->difference(_m_start_value, end_value()).first;
}

template <typename T>
__no_constexpr_imp enumerate_index_t
    enumeration_schema_base_t<T>::remaining_entities_after_maximum_advancements(
    ) const noexcept
{
    return _m_enumeration->difference(_m_start_value, end_value()).second;
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
    ) const noexcept
{
    using namespace std;
    return enumerate_index_t{1};
}

template <typename T>
template <typename R>
__constexpr_imp R
    enumeration_schema_base_t<T>::enumerate_all_values() const noexcept
{
    R       _l_rv{};
    T       _l_current_value{start_value()};
    const T _l_last_value{end_value()};
    if (is_direction_forward())
    {
        do
        {
            _l_rv.push_back(_l_current_value);
            auto ki = n_advancements_per_advancement();
            _m_enumeration->increment(_l_current_value, ki, _l_last_value);
        }
        while (_m_enumeration->less_than(_l_current_value, _l_last_value));
    }
    else
    {
        do
        {
            _l_rv.push_back(_l_current_value);
            auto ki = n_advancements_per_advancement();
            _m_enumeration->decrement(_l_current_value, ki, _l_last_value);
        }
        while (_m_enumeration->less_than(_l_last_value, _l_current_value));
    }
    return _l_rv;
}

_END_ABC_DG_NS
_BEGIN_ABC_NS
_END_ABC_NS