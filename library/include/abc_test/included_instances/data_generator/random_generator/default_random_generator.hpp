#pragma once

#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"

#include <memory>

_BEGIN_ABC_DG_NS

template <typename T>
struct default_random_generator_t : public random_generator_base_t<T>
{
public:
    __constexpr_imp virtual T
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T, typename... Ts>
__constexpr data_gen::random_generator_t<T>
            default_random_generator(Ts... elements) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS

template <typename T>
__constexpr_imp T
    default_random_generator_t<T>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "operator<T>(utility::rng, const std::size_t)."
    );
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename... Ts>
__constexpr data_gen::random_generator_t<T>
            default_random_generator(
                Ts... elements
            ) noexcept
{
    using namespace std;
    using namespace data_gen;
    return make_shared<default_random_generator_t<T>>(elements...);
}

_END_ABC_NS