#pragma once
#include "abc_test/core/ds/test_data/test_function_object.hpp"

#include <concepts>
#include <memory>

_BEGIN_ABC_DS_NS

template <typename T>
requires std::invocable<T>
class test_function_callable_object_t : public test_function_object_t
{
public:
    __constexpr
    test_function_callable_object_t(T _a_callable_object) noexcept;
    __constexpr virtual void
        run() const;
private:
    T _m_callable_object;
};

template <typename T>
requires std::invocable<T>
__no_constexpr test_function_t
    mk_function(T _a_tfr);
_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
template <typename T>
requires std::invocable<T>
__constexpr_imp
    test_function_callable_object_t<T>::test_function_callable_object_t(
        T _a_callable_object
    ) noexcept
    : _m_callable_object(_a_callable_object)
{}

template <typename T>
requires std::invocable<T>
__constexpr_imp void
    test_function_callable_object_t<T>::run() const
{
    std::invoke(_m_callable_object);
}

template <typename T>
requires std::invocable<T>
__no_constexpr_imp test_function_t
    mk_function(
        T _a_tfr
    )
{
    using namespace std;
    return make_shared<test_function_callable_object_t<T>>(_a_tfr);
}

_END_ABC_DS_NS