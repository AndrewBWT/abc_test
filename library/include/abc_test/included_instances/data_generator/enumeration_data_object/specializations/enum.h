#pragma once

#include "abc_test/included_instances/data_generator/enumeration_data_object.h"

#include <map>
#include "abc_test/internal/utility/enum.h"

_BEGIN_ABC_DG_NS
/*template <typename T>
using enum_list_t = std::vector<T>;
template <typename T>
concept enum_has_list_c = std::is_enum_v<T> && requires () {
    { get_enum_list<T>() } -> std::same_as<enum_list_t<T>>;
};
template <typename T>
__constexpr enum_list_t<T>
            get_enum_list() noexcept = delete;

template <typename T>
requires enum_has_list_c<T>
struct enumerate_enum_helper_t
{
    __constexpr
    enumerate_enum_helper_t(enum_list_t<T>&& _a_enum_list);
    __constexpr_imp bool
        less_than(const T& _a_l, const T& _a_r) const noexcept;
    __constexpr T
        min() const noexcept;
    __constexpr T
        max() const noexcept;
    __constexpr bool
        increment(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        ) const noexcept;
    ;
    __constexpr bool
        decrement(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        ) const noexcept;
private:
    std::map<std::size_t, T> _m_idx_to_elements;
    std::map<T, std::size_t> _m_elements_to_idx;
};*/

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T>
struct enumeration_data_object_t<T>
{
    __constexpr_imp
        enumeration_data_object_t(
            const std::size_t _a_difference = std::size_t(1)
        ) noexcept;
    __constexpr_imp virtual bool
        less_than(const T& _a_l, const T& _a_r) const noexcept;
    __constexpr_imp virtual bool
        equal(const T& _a_l, const T& _a_r) const noexcept;
    __constexpr_imp bool
        increment(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        );
    __constexpr_imp bool
        decrement(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        );
private:
    std::size_t _m_difference;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T>
__constexpr_imp
    enumeration_data_object_t<T>::enumeration_data_object_t(
        const std::size_t _a_difference
    ) noexcept
    : _m_difference(_a_difference)
{}

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T>
__constexpr_imp bool
    enumeration_data_object_t<T>::less_than(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    return get_thread_local_enumerate_enum_helper<T>().less_than(_a_l, _a_r);
}

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T>
__constexpr_imp bool
    enumeration_data_object_t<T>::equal(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    return _a_l == _a_r;
}

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T>
__constexpr_imp bool
    enumeration_data_object_t<T>::increment(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_max_value
    )
{
    return get_thread_local_enumerate_enum_helper<T>().increment(
        _a_element, _a_times_called, _a_max_value
    );
}

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T>
__constexpr_imp bool
    enumeration_data_object_t<T>::decrement(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_max_value
    )
{
    return get_thread_local_enumerate_enum_helper<T>().decrement(
        _a_element, _a_times_called, _a_max_value
    );
}

_END_ABC_DG_NS