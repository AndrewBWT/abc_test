#pragma once

#include "abc_test/internal/utility/printers/printer_base.hpp"

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T, typename F>
requires std::invocable<F, const T&>
struct function_printer_t : public printer_base_t<T>
{
public:
    __constexpr
    function_printer_t(F _a_callable) noexcept;
    __constexpr virtual std::string
        run_printer(const T& _a_object) const noexcept;
private:
    F _m_callable;
};

template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr std::string
            print(const T& _a_object, F _a_callable);
_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS
template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr_imp
    function_printer_t<T, F>::function_printer_t(
        F _a_callable
    ) noexcept
    : _m_callable(_a_callable)
{}

template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr std::string

            function_printer_t<T, F>::run_printer(
        const T& _a_object
    ) const noexcept
{
    return std::invoke(_m_callable, _a_object);
}

template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr std::string
            print(
                const T& _a_object,
                F        _a_callable
            )
{
    using namespace std;
    return print<T>(
        _a_object, make_shared<function_printer_t<T, F>>(_a_callable)
    );
}

_END_ABC_UTILITY_PRINTER_NS