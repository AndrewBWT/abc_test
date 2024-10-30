#pragma once

#include "abc_test/internal/data_generator/data_generator_collection.h"
#include "abc_test/internal/data_generator/data_generator_with_file_support.h"
#include "abc_test/internal/test_runner.h"

#include <initializer_list>
#include <vector>

_BEGIN_ABC_DG_NS

template <typename T>
class static_data_generator_t
{
public:
    using generator_type = T;
    using tertiary_type  = std::size_t;

    __constexpr ~static_data_generator_t();
    template <typename R>
    __constexpr
                static_data_generator_t(R&& _a_elements);
    __constexpr static_data_generator_t<T>&
                operator=(const static_data_generator_t<T>& other);
    __constexpr
    static_data_generator_t(const static_data_generator_t<T>& other);
    __constexpr void
        set_generator_using_tertiary_data(const tertiary_type& _a_tertiary_data
        );
    __constexpr bool
        has_current_element() const;
    __constexpr bool
        generate_next();
    __constexpr const T&
        current_element() const;
    __constexpr void
        reset();
    __constexpr const tertiary_type&
        tertiary_data() const;
private:
    abc::utility::str::rw_info_t<std::size_t> _m_rw_info;
    std::size_t                               _m_idx{0};
    T*                                        _m_elements{nullptr};
    std::size_t                               _m_elements_size{0};
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
/*!
 * Constructor which should be used. It creates a singleton collection.
 */
template <typename T, typename R = std::initializer_list<T>>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>, true>
    iterate_over(R&& _a_init_list);
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp static_data_generator_t<T>::~static_data_generator_t()
{
    delete[] _m_elements;
}

template <typename T>
template <typename R>
__constexpr_imp
    static_data_generator_t<T>::static_data_generator_t(
        R&& _a_elements
    )
    : _m_elements(new T[std::ranges::size(_a_elements)])
    , _m_elements_size(std::ranges::size(_a_elements))
{
    std::ranges::copy(_a_elements, _m_elements);
}

template <typename T>
__constexpr_imp static_data_generator_t<T>&
                static_data_generator_t<T>::operator=(
        const static_data_generator_t<T>& other
    ) // III. copy assignment
{
    if (this == &other)
    {
        return *this;
    }
    static_data_generator_t<T> temp(other);
    std::swap(_m_rw_info, temp._m_rw_info);
    std::swap(_m_idx, temp._m_idx);
    std::swap(_m_elements, temp._m_elements);
    std::swap(_m_elements_size, temp._m_elements_size);
    return *this;
}

template <typename T>
__constexpr_imp
    static_data_generator_t<T>::static_data_generator_t(
        const static_data_generator_t<T>& other
    ) // II. copy constructor
    : _m_rw_info(other._m_rw_info)
    , _m_idx(other._m_idx)
    , _m_elements_size(other._m_elements_size)
    , _m_elements(new T[other._m_elements_size])
{
    std::ranges::copy(
        other._m_elements, other._m_elements + _m_elements_size, _m_elements
    );
}

template <typename T>
__constexpr_imp void
    static_data_generator_t<T>::set_generator_using_tertiary_data(
        const tertiary_type& _a_tertiary_data
    )
{
    _m_idx = _a_tertiary_data;
}

template <typename T>
__constexpr_imp bool
    static_data_generator_t<T>::has_current_element() const
{
    return _m_idx < _m_elements_size;
}

template <typename T>
__constexpr_imp bool
    static_data_generator_t<T>::generate_next()
{
    return ++_m_idx < _m_elements_size;
}

template <typename T>
__constexpr_imp const T&
    static_data_generator_t<T>::current_element() const
{
    return _m_elements[_m_idx];
}

template <typename T>
__constexpr_imp void
    static_data_generator_t<T>::reset()
{
    _m_idx = 0;
}

template <typename T>
__constexpr_imp const static_data_generator_t<T>::tertiary_type&
                      static_data_generator_t<T>::tertiary_data() const
{
    return _m_idx;
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename R>
requires std::same_as<std::ranges::range_value_t<R>, T>
__constexpr_imp
    _ABC_NS_DG::data_generator_collection_t<std::ranges::range_value_t<R>, true>
    iterate_over(
        R&& _a_init_list
    )
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return unary_collection<T>(
        make_shared<data_generator_with_file_support_t<
            static_data_generator_t<T>,
            false>>(static_data_generator_t<T>(_a_init_list))
    );
}

_END_ABC_NS