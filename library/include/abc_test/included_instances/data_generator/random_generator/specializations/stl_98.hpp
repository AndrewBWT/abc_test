#pragma once

#include "abc_test/included_instances/data_generator/random_generator/default_random_generator.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"
#include "concepts"

_BEGIN_ABC_DG_NS

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
struct default_random_generator_t<std::basic_string<T>>
    : public random_generator_base_t<T>
{
public:
    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_rng
    ) noexcept;
    __no_constexpr virtual std::basic_string<T>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T> _m_rng;
};

template <typename T>
struct default_random_generator_t<std::vector<T>> : public random_generator_base_t<std::vector<T>>
{
public:
    __constexpr
        default_random_generator_t(
            const random_generator_t& _a_gen
        ) noexcept;
    __no_constexpr virtual std::vector<T>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T> _m_gen;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
default_random_generator_t<T>::default_random_generator_t(const random_generator_t<T>& _a_rnd_gen
) noexcept
    : _m_rng(_a_rng)
{

}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
__no_constexpr_imp std::basic_string<T>
                   default_random_generator_t<std::basic_string<T>>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    if (_a_index == 0)
    {
        return basic_string<T>{};
    }
    else
    {
        size_t          _l_size{1 + (_a_rnd_generator() % (_a_index + 1))};
        basic_string<T> _l_rv;
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_rv.push_back(_m_rng(_a_rnd_generator, _a_index));
        }
        return _l_rv;
    }
}

/*template <typename T>
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
*/

template <typename T>
__constexpr_imp
default_random_generator_t<std::vector<T>>::default_random_generator_t(
    const default_random_generator_t<T>& _a_gen
) noexcept
    : _m_gen(_a_gen)
{
}

template <typename T>
__no_constexpr_imp std::vector<T>
default_random_generator_t<std::vector<T>>::operator()(
    utility::rng& _a_rnd_generator,
    const std::size_t _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    const size_t _l_size{ _a_rnd_generator() % _a_index };
    vector<T> _l_rv(_l_size);
    for (size_t _l_idx{ 0 }; _l_idx < _l_size; ++_l_idx)
    {
        _l_rv[_l_idx] = _m_gen(_a_rnd_generator, _a_index);
    }
    return _l_rv;
}

_END_ABC_DG_NS