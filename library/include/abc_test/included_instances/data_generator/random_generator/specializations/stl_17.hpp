#pragma once

#include "abc_test/included_instances/data_generator/random_generator/default_random_generator.hpp"
#include "abc_test/internal/utility/limits/max_value_concept.hpp"
#include "abc_test/internal/utility/limits/min_value_concept.hpp"
#include "concepts"

#include <optional>

_BEGIN_ABC_DG_NS

template <typename T>
struct default_random_generator_t<std::optional<T>> : public random_generator_base_t<std::optional<T>>
{
public:
    __no_constexpr virtual std::optional<T>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T> _m_rng;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
__no_constexpr_imp std::optional<T>
default_random_generator_t<std::optional<T>>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    if (_a_index == 0)
    {
        return optional<T>{};
    }
    else
    {
        bool            _l_is_opt{ (_a_rnd_generator() % 2) == 0};
        if (_l_is_opt)
        {
            return optional<T>{};
        }
        else
        {
            const T _l_elem{ _m_rng(_a_rnd_generator,_a_index) };
            return optional<T>{_l_elem};
        }
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