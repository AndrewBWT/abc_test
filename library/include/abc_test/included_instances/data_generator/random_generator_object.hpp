#pragma once

#include "abc_test/internal/utility/rng.hpp"

#include <memory>

_BEGIN_ABC_DG_NS
template <typename T>
using random_func_t = std::function<T(utility::rng&)>;
template <typename T>
using random_func_ptr_t = T (*)(utility::rng&);

template <typename T>
struct random_generator_object_t
{
private:
    random_func_t<T> _m_rnd_gen;
public:
    __constexpr_imp
        random_generator_object_t(random_func_t<T>& _a_rnd_func) noexcept;
    __constexpr_imp virtual T
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T>
__constexpr std::shared_ptr<abc::data_gen::random_generator_object_t<T>>
            using_function(abc::data_gen::random_func_t<T>& _a_func) noexcept;
template <typename T>
__constexpr std::shared_ptr<abc::data_gen::random_generator_object_t<T>>
    using_function(abc::data_gen::random_func_ptr_t<T> _a_func) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr_imp
    random_generator_object_t<T>::random_generator_object_t(
        random_func_t<T>& _a_rnd_func
    ) noexcept
{}

template <typename T>
__constexpr_imp T
    random_generator_object_t<T>::operator()(
        utility::rng& _a_rnd_generator,
        const std::size_t _a_index
    )
{
    return _m_rnd_gen(_a_rnd_generator);
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp std::shared_ptr<abc::data_gen::random_generator_object_t<T>>
                using_function(
                    abc::data_gen::random_func_t<T>& _a_func
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return random_generator_object_t(_a_func);
}

template <typename T>
__constexpr_imp std::shared_ptr<abc::data_gen::random_generator_object_t<T>>
                using_function(
                    abc::data_gen::random_func_ptr_t<T> _a_func
                ) noexcept
{
    using namespace _ABC_NS_DG;
    return random_generator_object_t(random_func_t<T>([_a_func](utility::rng& _a_rng) {
        return _a_func(_a_rng); }));
}

_END_ABC_NS