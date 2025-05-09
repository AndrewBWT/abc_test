#pragma once

#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"

#include <memory>

_BEGIN_ABC_DG_NS

template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng_t&, const utility::rng_counter_t&>
struct random_generator_function_t : public random_generator_base_t<T>
{
private:
    F _m_rnd_func;
public:
    __constexpr
        random_generator_function_t(F _a_rnd_func) noexcept;
    __constexpr virtual T
        operator()(utility::rng_t& _a_rnd_generator, const utility::rng_counter_t& _a_index);
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng_t&, const utility::rng_counter_t&>
__constexpr data_gen::random_generator_t<T>
using_function(F _a_func) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng_t&, const utility::rng_counter_t&>
__constexpr_imp
random_generator_function_t<T,F>::random_generator_function_t(
    F _a_rnd_func
) noexcept
    : _m_rnd_func(_a_rnd_func)
{
}

template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng_t&, const utility::rng_counter_t&>
__constexpr_imp T
random_generator_function_t<T,F>::operator()(
    utility::rng_t& _a_rnd_generator,
    const utility::rng_counter_t& _a_index
    )
{
    return std::invoke_r<T>(_m_rnd_func,_a_rnd_generator,_a_index);
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng_t&, const utility::rng_counter_t&>
__constexpr data_gen::random_generator_t<T>
using_function(F _a_func) noexcept
{
    using namespace std;
    using namespace abc::data_gen;
    return make_shared<random_generator_function_t<T,F>>(_a_func);
}

_END_ABC_NS