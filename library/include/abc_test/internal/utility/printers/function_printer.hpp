#pragma once

#include "abc_test/internal/utility/printers/printer_base.hpp"

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T, typename F>
requires std::is_invocable_r_v<std::string, F, const T&>
struct function_printer_t : public printer_base_t<T>
{
public:
    function_printer_t() = delete;
    __constexpr
    function_printer_t(F _a_callable) noexcept;
    __constexpr virtual std::string
        run_printer(const T& _a_object) const noexcept;
private:
    F _m_callable;
};
template<typename T, typename F>
    requires std::is_invocable_r_v<std::string, F, const T&>
__constexpr utility::printer::printer_t<T>
function_printer(F _a_callable) noexcept
{
    return std::make_shared<utility::printer::function_printer_t<T, F>>(_a_callable);
}
_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS
template <typename T, typename F>
    requires std::is_invocable_r_v<std::string, F, const T&>
__constexpr_imp
    function_printer_t<T, F>::function_printer_t(
        F _a_callable
    ) noexcept
    : _m_callable(_a_callable)
{}

template <typename T, typename F>
    requires std::is_invocable_r_v<std::string, F, const T&>
__constexpr std::string

            function_printer_t<T, F>::run_printer(
        const T& _a_object
    ) const noexcept
{
    return std::invoke_r<std::string>(_m_callable, _a_object);
}
_END_ABC_UTILITY_PRINTER_NS