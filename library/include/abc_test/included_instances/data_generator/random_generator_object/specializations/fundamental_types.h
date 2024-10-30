#pragma once

#include "abc_test/included_instances/data_generator/random_generator_object.h"
#include "concepts"

_BEGIN_ABC_DG_NS

template <>
struct random_generator_object_t<bool>
{
    __no_constexpr virtual bool
        operator()(utility::rng& _a_rnd_generator);
};

template <typename T>
requires std::integral<T>
struct random_generator_object_t<T>
{
public:
    __constexpr
    random_generator_object_t(
        const T& _a_min = std::numeric_limits<T>::min(),
        const T& _a_max = std::numeric_limits<T>::max()
    ) noexcept;
    __no_constexpr virtual T
        operator()(utility::rng& _a_rnd_generator);
private:
    T _m_min;
    T _m_max;
    T _m_range;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
__no_constexpr_imp bool
    random_generator_object_t<bool>::operator()(
        utility::rng& _a_rnd_generator
    )
{
    using namespace std;
    size_t _l_val{_a_rnd_generator() % 2};
    return _l_val == 0 ? false : true;
}

template <typename T>
requires std::integral<T>
__constexpr_imp
    random_generator_object_t<T>::random_generator_object_t(
        const T& _a_min,
        const T& _a_max
    ) noexcept
    : _m_min(_a_min), _m_max(_a_max), _m_range(_m_max - _m_min)
{}

template <typename T>
requires std::integral<T>
__no_constexpr_imp T
    random_generator_object_t<T>::operator()(
        utility::rng& _a_rnd_generator
    )
{
    using namespace std;
    T _l_val{static_cast<T>(_a_rnd_generator() % _m_range)};
    return _m_min + _l_val;
}

_END_ABC_DG_NS