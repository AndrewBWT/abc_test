#pragma once
#include "abc_test/internal/utility/printers/default_printer.hpp"

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename... Ts>
struct default_printer_t<std::tuple<Ts...>>
    : public printer_base_t<std::tuple<Ts...>>
{
    using value_type = std::tuple<Ts...>;
    static constexpr bool is_specialized{ true };
    __constexpr
    default_printer_t(printer_t<Ts>... _a_printers);
    __constexpr
    default_printer_t()
        requires (std::is_default_constructible_v<default_printer_t<Ts>> && ...)
    : _m_printers(std::make_tuple(mk_printer(default_printer_t<Ts>())...))
    {

    }
    __constexpr virtual std::string
        run_printer(const value_type& _a_parse_input) const;
    __constexpr std::tuple<printer_t<Ts>...>& get_printers_ref() noexcept
    {
        return _m_printers;
    }
private:
    std::tuple<printer_t<Ts>...> _m_printers;
    template <std::size_t I>
    __constexpr void
        run_internal_printer(std::string& _a_str, const value_type& _a_object)
            const;
};
template <>
struct default_printer_t<std::filesystem::path>
    : public printer_base_t<std::filesystem::path>
{
    using value_type = std::filesystem::path;
    __no_constexpr_imp virtual std::string
        run_printer(const value_type& _a_parse_input) const
    {
        return _a_parse_input.string();
    }
};

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS
template <typename... Ts>
__constexpr
default_printer_t<std::tuple<Ts...>>::default_printer_t(
    printer_t<Ts>... _a_printers
)
    : _m_printers{std::move(_a_printers)...}
{}

template <typename... Ts>
__constexpr std::string
            default_printer_t<std::tuple<Ts...>>::run_printer(
        const default_printer_t<std::tuple<Ts...>>::value_type& _a_object
    ) const
{
    using namespace std;
    string _l_str{"("};
    run_internal_printer<0>(_l_str, _a_object);
    _l_str.append(")");
    return _l_str;
}

template <typename... Ts>
template <std::size_t I>
__constexpr_imp void
    default_printer_t<std::tuple<Ts...>>::run_internal_printer(
        std::string&                                            _a_str,
        const default_printer_t<std::tuple<Ts...>>::value_type& _a_object
    ) const
{
    using namespace std;
    using U = tuple_element<I, value_type>::type;
    _a_str.append(get<I>(_m_printers)->run_printer(get<I>(_a_object)));
    if constexpr (I + 1 < tuple_size<value_type>{})
    {
        _a_str.append(", ");
        run_internal_printer<I + 1>(_a_str, _a_object);
    }
}

_END_ABC_UTILITY_PRINTER_NS