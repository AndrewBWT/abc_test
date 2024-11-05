#pragma once

#include "abc_test/included_instances/data_generator/random_generator_object.hpp"
#include "concepts"

_BEGIN_ABC_DG_NS

template <typename T, std::size_t N>
struct random_generator_object_t<std::array<T,N>>
{
public:
    __constexpr
        random_generator_object_t(
            const std::shared_ptr<random_generator_object_t<T>>& _a_gen
        ) noexcept;
    __no_constexpr virtual std::array<T, N>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    std::shared_ptr<random_generator_object_t<T>> _m_gen;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T, std::size_t N>
__constexpr_imp
random_generator_object_t<std::array<T, N>>::random_generator_object_t(
    const std::shared_ptr<random_generator_object_t<T>>& _a_gen
) noexcept
    : _m_gen(_a_gen)
{}

template <typename T, std::size_t N>
__no_constexpr_imp std::array<T, N>
random_generator_object_t<std::array<T, N>>::operator()(
    utility::rng& _a_rnd_generator,
    const std::size_t _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    array<T, N> _l_rv{};
    for (size_t _l_idx{ 0 }; _l_idx < N; ++_l_idx)
    {
        _l_rv[_l_idx] = _m_gen(_a_rnd_generator, _a_index);
    }
    return _l_rv;
}

_END_ABC_DG_NS