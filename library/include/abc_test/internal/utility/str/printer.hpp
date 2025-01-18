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
requires is_to_string_convertable_t<T>
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

template <typename T, typename... Args>
struct tuple_printer_inner_t
{};

namespace
{
template <std::size_t I, typename... Ts>
__constexpr void
    object_printer_inner(
        std::string&                                       _a_str,
        const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
        std::tuple<printer_t<Ts>...>                       _a_parsers,
        std::tuple<Ts...> _a_elements_to_print
    )
{
    _a_str.append(std::get<I>(_a_object_names));
    _a_str.append(" = ");
    _a_str.append(std::get<I>(_a_parsers).run_printer(std::get<I>(_a_elements_to_print)));
    if constexpr (I + 1 < sizeof...(Ts))
    {
        _a_str.append(", ");
        object_printer_inner<I + 1>(
            _a_str, _a_object_names, _a_parsers, _a_elements_to_print
        );
    }
}
} // namespace

template <typename... Ts>
__constexpr std::string
            object_printer(
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                const std::string_view                             _a_begin_str,
                const char                                         _a_begin_char,
                const char                                         _a_end_char,
                std::tuple<printer_t<Ts>...>                       _a_parsers,
                Ts... _a_elements_to_print
            )
{
    using namespace std;
    string _l_rv{_a_begin_str};
    _l_rv.append(" ");
    _l_rv.push_back(_a_begin_char);
    std::tuple<Ts...> _l_tuple{ std::tie(_a_elements_to_print...) };
    object_printer_inner<0>(
        _l_rv, _a_object_names, _a_parsers, _l_tuple
    );
    _l_rv.push_back(_a_end_char);
    return _l_rv;
}

template <typename... Ts>
__constexpr std::string
            object_printer(
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                const std::string_view                             _a_begin_str,
                const char                                         _a_begin_char,
                const char                                         _a_end_char,
                Ts... _a_elements_to_print
            )
{
    std::tuple<printer_t<Ts>...> _l_printers;
    return object_printer(
        _a_object_names,
        _a_begin_str,
        _a_begin_char,
        _a_end_char,
        _l_printers,
        _a_elements_to_print...
    );
}

template <typename... Ts>
requires requires (printer_t<Ts>... printers, Ts... ts) {
    (printers.run_printer(ts), ...);
}
struct printer_t<std::tuple<Ts...>>
{
    using value_type = std::tuple<Ts...>;

    printer_t(
        printer_t<Ts>... printers
    )
        : printers_{std::move(printers)...}
    {}

    printer_t()
    requires (std::is_default_constructible_v<printer_t<Ts>> && ...)
    = default;

    __constexpr std::string
                run_printer(
                    const value_type& _a_object
                ) const noexcept
    {
        using namespace std;
        string _l_str{"("};
        run_internal_printer<0>(_l_str, _a_object);
        _l_str.append(")");
        return _l_str;
    }
private:
    std::tuple<printer_t<Ts>...> printers_;

    template <std::size_t Curr_Idx>
    __constexpr void
        run_internal_printer(
            std::string&      _a_str,
            const value_type& _a_object
        ) const noexcept
    {
        using U = std::tuple_element<Curr_Idx, value_type>::type;
        _a_str.append(std::get<Curr_Idx>(printers_).run_printer(
            std::get<Curr_Idx>(_a_object)
        ));
        if constexpr (Curr_Idx + 1 < std::tuple_size<value_type>{})
        {
            _a_str.append(", ");
            run_internal_printer<Curr_Idx + 1>(_a_str, _a_object);
        }
    }
};

template <typename T, typename U>
struct printer_t<std::pair<T, U>>
{
    using value_type = std::pair<T, U>;

    __constexpr
    printer_t(
        const printer_t<T>  _a_printer_t,
        const printer_t<U>& _a_printer_u
    )
        : _m_printers{std::pair<printer_t<T>, printer_t<U>>(
              make_pair(_a_printer_t, _a_printer_u)
          )}
    {}

    __constexpr
    printer_t()
    requires (std::is_default_constructible_v<printer_t<T>>
              && std::is_default_constructible_v<printer_t<U>>)
    = default;

    __constexpr std::string
                run_printer(
                    const value_type& _a_object
                ) const noexcept
    {
        using namespace std;
        string _l_str{"("};
        _l_str.append(_m_printers.first.run_printer(_a_object.first));
        _l_str.append(", ");
        _l_str.append(_m_printers.second.run_printer(_a_object.second));
        _l_str.append(")");
        return _l_str;
    }
private:
    std::pair<printer_t<T>, printer_t<U>> _m_printers;
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