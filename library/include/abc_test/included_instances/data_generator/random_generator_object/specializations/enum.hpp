#pragma once

#include "abc_test/included_instances/data_generator/random_generator_object.hpp"
#include "abc_test/internal/utility/enum.hpp"
#include "concepts"

_BEGIN_ABC_DG_NS

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T> && _ABC_NS_UTILITY::min_value_c<T>
         && _ABC_NS_UTILITY::max_value_c<T>
struct random_generator_object_t<T>
{
public:
    __constexpr
    random_generator_object_t(
        const T& _a_min = _ABC_NS_UTILITY::min_value_t<T>().min_value(),
        const T& _a_max = _ABC_NS_UTILITY::max_value_t<T>().max_value()
    ) noexcept;
    __no_constexpr virtual T
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    T _m_min;
    T _m_max;
    T _m_range;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T> && _ABC_NS_UTILITY::min_value_c<T>
             && _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp
    random_generator_object_t<T>::random_generator_object_t(
        const T& _a_min,
        const T& _a_max
    ) noexcept
    : _m_min(_a_min)
    , _m_max(_a_max)
    , _m_range(_ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
                   .difference(_a_min, _a_max))
{}

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T> && _ABC_NS_UTILITY::min_value_c<T>
         && _ABC_NS_UTILITY::max_value_c<T>
__no_constexpr_imp T
    random_generator_object_t<T>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    const size_t _l_val{static_cast<size_t>(_a_rnd_generator() % _m_range)};
    T            _l_rv{_m_min};
    get_thread_local_enumerate_enum_helper<T>().increment(_l_rv, _l_val);
    return _l_rv;
}

_END_ABC_DG_NS