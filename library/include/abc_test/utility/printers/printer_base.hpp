#pragma once

#include "abc_test/utility/internal/macros.hpp"

#include <memory>
#include <string>

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct printer_base_t;
template <typename T>
using printer_t = std::shared_ptr<printer_base_t<T>>;
template <typename T>
struct default_printer_t;

template <typename T>
struct printer_base_t
{
public:
    using value_type = T;
    virtual ~printer_base_t() = default;
    __constexpr virtual std::u8string
        run_printer(const T& _a_parse_input) const
        = 0;
};

template <typename T>
__constexpr_imp std::u8string
                print(
                    const T&                              _a_object,
                    const utility::printer::printer_t<T>& _a_printer
                    = mk_printer(default_printer_t<T>())
                ) noexcept;

template <typename T>
__constexpr abc::utility::printer::printer_t<typename T::value_type>
            mk_printer(const T& _a_printer_base) noexcept;
_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
__constexpr_imp std::u8string
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

_END_ABC_UTILITY_PRINTER_NS