#pragma once
#include "abc_test/internal/errors/test_library_exception.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"

#include <expected>
#include <fmt/format.h>
#include <functional>
#include <scn/scan.h>
#include <string>
#include <variant>
#include <vector>

_BEGIN_ABC_UTILITY_STR_NS
template <typename T>
using printer_fp_t = std::string (*)(const T&);
template <typename T>
using printer_fo_t = std::function<std::string(const T&)>;

namespace
{
template <typename T>
class printable_object_t;
} // namespace

template <typename T>
struct printer_t
{
public:
    __constexpr std::string
                run_printer(
                    const T& _a_str
                ) const noexcept
    {
        __STATIC_ASSERT(
            T,
            "Function requires a specialization of the function "
            "run_printer<T>()."
        );
    }

    // __constexpr
    // printer_t(const std::shared_ptr<printable_object_t<T>>& _a_func)
    // noexcept;
    // private:
    //     std::shared_ptr<printable_object_t<T>> _m_func;
};

template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr_imp std::string
                print(
                    const T& _a_object,
                    F        _a_callable
                ) noexcept
{
    return _a_callable(_a_object);
}

template <typename T>
__constexpr_imp std::string
print(
                    const T&           _a_object,
                    const printer_t<T> _a_printer = printer_t<T>()
                ) noexcept
{
    return _a_printer.run_printer(_a_object);
}
template <typename T>
    requires is_from_chars_convertable_c<T>
struct printer_t<T>
{
    __constexpr std::string
        run_printer(
            const T& _a_object
        ) const noexcept
    {
        return std::to_string(_a_object);
    }
};

template <typename T>
    requires is_tuple<T>::value
struct printer_t<T>
{
    __constexpr std::string
        run_printer(
            const T& _a_object
        ) const noexcept
    {
        return fmt::format("{0}", _a_object);
    }
};

template <typename T>
    requires is_pair<T>::value
struct printer_t<T>
{
    __constexpr std::string
        run_printer(
            const T& _a_object
        ) const noexcept
    {
        return fmt::format("{0}", _a_object);
    }
};

template <typename T>
    requires (std::convertible_to<T, std::string_view>)
struct printer_t<T>
{
    __constexpr std::string
        run_printer(
            const T& _a_object
        ) const noexcept
    {
        return fmt::format("\"{0}\"", _a_object);
    }
};
namespace
{
template <typename T>
struct printable_object_t
{
public:
    __constexpr virtual std::string
        run(const T& _a_object) const noexcept
        = 0;
};

template <typename T, typename F>
requires std::invocable<F, const T&>
class printable_t : public printable_object_t<T>
{
public:
    __constexpr
                printable_t(F _a_callable_object) noexcept;
    __constexpr std::string
                run(const T& _a_object) const noexcept;
private:
    F _m_callable_object;
};
} // namespace

_END_ABC_UTILITY_STR_NS
/*_BEGIN_ABC_NS
template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr abc::utility::str::printer_t<T>
            printer(F _a_callable) noexcept;
template <typename T>
requires fmt::formattable<T>
__constexpr abc::utility::str::printer_t<T>
            default_printer() noexcept;
_END_ABC_NS*/
_BEGIN_ABC_UTILITY_STR_NS

/*template <typename T>
__constexpr_imp std::string
                printer_t<T>::run_printer(
        const T& _a_obj
    ) const noexcept
{
    return _m_func->run(_a_obj);
}

template <typename T>
__constexpr
printer_t<T>::printer_t(
    const std::shared_ptr<printable_object_t<T>>& _a_func
) noexcept
    : _m_func(_a_func)
{}*/

namespace
{
template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr
printable_t<T, F>::printable_t(
    F _a_callable_object
) noexcept
    : _m_callable_object(_a_callable_object)
{}

template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr std::string
            printable_t<T, F>::run(
        const T& _a_object
    ) const noexcept
{
    return std::invoke(_m_callable_object, _a_object);
}
} // namespace

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_NS
/*template <typename T, typename F>
requires std::invocable<F, const T&>
__constexpr abc::utility::str::printer_t<T>
            printer(
                F _a_callable
            ) noexcept
{
    using namespace std;
    using namespace utility::str;
    return printer_t<T>(make_shared<printable_t<T, F>>(_a_callable));
}

template <typename T>
requires fmt::formattable<T>
__constexpr abc::utility::str::printer_t<T>
            default_printer() noexcept
{
    if constexpr (std::same_as<T, std::string>)
    {
        return printer<T>(
            [](const T& _a_obj)
            {
                return fmt::format("\"{}\"", _a_obj);
            }
        );
    }
    else
    {
        return printer<T>(
            [](const T& _a_obj)
            {
                return fmt::format("{}", _a_obj);
            }
        );
    }
}
*/
_END_ABC_NS