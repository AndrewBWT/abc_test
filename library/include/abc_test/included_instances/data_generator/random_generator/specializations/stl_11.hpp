#pragma once

#include "abc_test/included_instances/data_generator/random_generator/default_random_generator.hpp"

_BEGIN_ABC_DG_NS

template <typename T, std::size_t N>
struct default_random_generator_t<std::array<T, N>> : public random_generator_base_t<std::array<T, N>>
{
public:
    __constexpr
        default_random_generator_t(
            const random_generator_t<T>& _a_gen
        ) noexcept;
    __no_constexpr virtual std::array<T, N>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T> _m_gen;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T, std::size_t N>
__constexpr_imp
default_random_generator_t<std::array<T, N>>::default_random_generator_t(
    const random_generator_t<T>& _a_gen
) noexcept
    : _m_gen(_a_gen)
{
}

template <typename T, std::size_t N>
__no_constexpr_imp std::array<T, N>
default_random_generator_t<std::array<T, N>>::operator()(
    utility::rng& _a_rnd_generator,
    const std::size_t _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    array<T, N> _l_rv{};
    for (size_t _l_idx{ 0 }; _l_idx < N; ++_l_idx)
    {
        _l_rv[_l_idx] = (*_m_gen)(_a_rnd_generator, _a_index)();
    }
    return _l_rv;
}

_END_ABC_DG_NS