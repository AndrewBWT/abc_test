#pragma once

#include "abc_test/utility/rng.hpp"

#include <memory>

_BEGIN_ABC_DG_NS

template <typename T>
struct random_generator_base_t
{
public:
    using value_type_t = T;
    __constexpr_imp virtual T
        operator()(utility::rng_t& _a_rnd_generator, const utility::rng_counter_t& _a_index)
        = 0;
};

template <typename T>
using random_generator_t = std::shared_ptr<random_generator_base_t<T>>;
_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T>
__constexpr data_gen::random_generator_t<typename T::value_type_t>
            mk_random_generator(const T& _a_random_generator_base) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp data_gen::random_generator_t<typename T::value_type_t>
                mk_random_generator(
                    const T& _a_random_generator_base
                ) noexcept
{
    using namespace std;
    return make_shared<T>(_a_random_generator_base);
}

_END_ABC_NS