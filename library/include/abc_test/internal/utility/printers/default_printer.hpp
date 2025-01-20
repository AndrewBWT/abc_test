#pragma once

#include "abc_test/internal/utility/printers/printer_base.hpp"

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct default_printer_t : public printer_base_t<T>
{
public:
    __constexpr virtual std::string
        run_printer(const T& _a_object) const override;
};

template <typename T>
__constexpr printer_t<T>
default_printer() noexcept;
_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS
template <typename T>
__constexpr std::string
default_printer_t<T>::run_printer(
    const T& _a_object
) const
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "run_printer<T>()."
    );
}

template <typename T>
__constexpr_imp printer_t<T>
default_printer() noexcept
{
    using namespace std;
    return make_shared<default_printer_t<T>>();
}

_END_ABC_UTILITY_PRINTER_NS