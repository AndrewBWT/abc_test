#pragma once
#include "abc_test/internal/errors/test_library_exception.h"
#include "abc_test/internal/utility/internal/macros.h"

#include <functional>
#include <variant>

_BEGIN_ABC_DG_NS
using enumerate_index_t = std::size_t;
template <typename T>
using cmp_func_t = std::function<bool(const T&, const T&)>;

template <typename T>
struct enumeration_data_object_t
{
private:
    std::function<bool(const T&, const T&)> _m_less_than_func;
    std::function<bool(const T&, const T&)> _m_equal_func;
    std::function<bool(T&, enumerate_index_t&, const std::optional<T>&)>
        _m_increment_func;
    std::function<bool(T&, enumerate_index_t&, const std::optional<T>&)>
        _m_decrement_func;
    std::function<std::size_t(const T&, const T&)> _m_difference_func;
public:
    __constexpr enumeration_data_object_t() = delete;
    __constexpr_imp virtual bool
        less_than(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _m_less_than_func(_a_l, _a_r);
    }

    __constexpr_imp virtual bool
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _m_equal_func(_a_l, _a_r);
    }
    __constexpr virtual bool
        increment(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        );
    __constexpr virtual bool
        decrement(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        );
    __constexpr virtual std::size_t difference(
        const T& _a_arg1,
        const T& _a_arg2
    ) noexcept
    {
        return _m_difference_func(_a_arg1, _a_arg2);
    }
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp bool
enumeration_data_object_t<T>::increment(
        T&                      _a_element,
        enumerate_index_t&      _a_n_times_to_increment,
        const std::optional<T>& _a_max_value
    )
{
    return _m_increment_func(_a_element, _a_n_times_to_increment, _a_max_value);
}

template <typename T>
__constexpr_imp bool
enumeration_data_object_t<T>::decrement(
        T&                      _a_element,
        enumerate_index_t&      _a_n_times_to_increment,
        const std::optional<T>& _a_max_value
    )
{
    return _m_decrement_func(_a_element, _a_n_times_to_increment, _a_max_value);
}

_END_ABC_DG_NS