#pragma once

#include "abc_test/included_instances/data_generator/random_generator_object.hpp"
#include "abc_test/internal/utility/limits/min_value_concept.hpp"
#include "abc_test/internal/utility/limits/max_value_concept.hpp"
#include "concepts"

_BEGIN_ABC_DG_NS
template<typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
struct random_generator_object_t<std::basic_string<T>>
{
public:
    __no_constexpr virtual std::basic_string<T>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_object_t<T> _m_rng;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template<typename T>
    requires _ABC_NS_UTILITY::min_value_c<T>&& _ABC_NS_UTILITY::max_value_c<T>
__no_constexpr_imp std::basic_string<T>
random_generator_object_t<std::basic_string<T>>::operator()(
    utility::rng& _a_rnd_generator,
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
        size_t _l_size{1 + ( _a_rnd_generator() % (_a_index+1)) };
        basic_string<T> _l_rv;
        for (size_t _l_idx{ 0 }; _l_idx < _l_size; ++_l_idx)
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
_END_ABC_DG_NS