#pragma once

#include "abc_test/utility/printers/printer_base.hpp"

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct default_printer_t : public printer_base_t<T>
{
public:
    static constexpr bool is_specialized{false};
    __constexpr virtual std::string
        run_printer(const T& _a_object) const override;
};

template <typename T, typename... Ts>
__constexpr printer_t<T>
            default_printer(
                Ts... elements
            )
{
    return mk_printer(default_printer_t<T>(elements...));
}

template <typename T>
concept default_printable = (default_printer_t<T>::is_specialized == true);
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

_END_ABC_UTILITY_PRINTER_NS