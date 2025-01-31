#pragma once

#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"

#include <memory>

_BEGIN_ABC_DG_NS

template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng&, const std::size_t>
struct random_generator_function_t : public random_generator_base_t<T>
{
private:
    F _m_rnd_gen;
public:
    __constexpr
        random_generator_function_t(F _a_rnd_func) noexcept;
    __constexpr virtual T
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng&, const std::size_t>
__constexpr data_gen::random_generator_t<typename T::value_type_t>
using_function(F _a_func) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng&, const std::size_t>
__constexpr_imp
random_generator_function_t<T,F>::random_generator_function_t(
    F _a_rnd_func
) noexcept
    : _m_rnd_func(_a_rnd_func)
{
}

template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng&, const std::size_t>
__constexpr_imp T
random_generator_function_t<T,F>::operator()(
    utility::rng& _a_rnd_generator,
    const std::size_t _a_index
    )
{
    return std::invoke_r(_m_rnd_gen,_a_rnd_generator,_a_index);
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename F>
    requires std::is_invocable_r_v<T, F, utility::rng&, const std::size_t>
__constexpr data_gen::random_generator_t<typename T::value_type_t>
using_function(F _a_func) noexcept
{
    return std::make_shared<abc::data_gen::random_generator_function_t<T,F>>(_a_func);
}

_END_ABC_NS