#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"

#include <memory>
#include <string>

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct printer_base_t
{
public:
    using value_type = T;
    __constexpr virtual std::string
        run_printer(const T& _a_parse_input) const
        = 0;
};

template <typename T>
using printer_t = std::shared_ptr<printer_base_t<T>>;
template <typename T>
struct default_printer_t;
template <typename T, typename ...Ts>
__constexpr_imp printer_t<T>
                default_printer(Ts... args) noexcept;
_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_NS
template <typename T>
__constexpr_imp std::string
                print(
                    const T&                              _a_object,
                    const utility::printer::printer_t<T>& _a_printer
                    = utility::printer::default_printer<T>()
                ) noexcept;
template <typename T>
__constexpr abc::utility::printer::printer_t<typename T::value_type>
mk_printer(const T& _a_printer_base) noexcept;
_END_ABC_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp std::string
                print(
                    const T&                              _a_object,
                    const utility::printer::printer_t<T>& _a_printer
                ) noexcept
{
    return _a_printer->run_printer(_a_object);
}
template <typename T>
__constexpr abc::utility::printer::printer_t<typename T::value_type>
mk_printer(
    const T& _a_printer_base
) noexcept
{
    using namespace std;
    return make_shared<T>(_a_printer_base);
}
_END_ABC_NS