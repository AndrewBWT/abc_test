#pragma once
#include "abc_test/internal/utility/printers/default_printer.hpp"
#include "abc_test/internal/utility/concepts.hpp"

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
    requires is_to_string_convertable_c<T>
struct default_printer_t<T> : public printer_base_t<T>
{
    __constexpr std::string
        run_printer(
            const T& _a_object
        ) const
    {
        using namespace std;
        return to_string(_a_object);
    }
};

template <typename... Ts>
__constexpr std::string
object_printer(
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    const std::string_view                             _a_begin_str,
    const char                                         _a_begin_char,
    const char                                         _a_end_char,
    std::tuple<default_printer_t<Ts>...>                       _a_parsers,
    Ts... _a_elements_to_print
);
template <typename... Ts>
__constexpr std::string
object_printer(
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    const std::string_view                             _a_begin_str,
    const char                                         _a_begin_char,
    const char                                         _a_end_char,
    Ts... _a_elements_to_print
);

namespace
{
    template <std::size_t I, typename... Ts>
    __constexpr void
        object_printer_inner(
            std::string& _a_str,
            const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
            std::tuple<default_printer_t<Ts>...>                       _a_parsers,
            std::tuple<Ts...> _a_elements_to_print
        ) noexcept;
} // namespace

_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

/*
template <typename T>
requires is_from_chars_convertable_c<T>
__constexpr_imp parse_result_t<T>
                default_parser_t<T>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    T                 result{};
    const string_view _l_str{_a_parse_input.sv()};
    auto [ptr, ec]
        = from_chars(_l_str.data(), _l_str.data() + _l_str.size(), result);

    if (ec == std::errc())
    {
        _a_parse_input.advance((ptr - _l_str.data()) + 0);
        return expected<T, string>(result);
    }
    else
    {
        return parse_error<T>(
            fmt::format("Could not parse \"{0}\" using std::from_chars", _l_str)

        );
    }
}*/
template <typename... Ts>
__constexpr_imp std::string
object_printer(
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    const std::string_view                             _a_begin_str,
    const char                                         _a_begin_char,
    const char                                         _a_end_char,
    std::tuple<default_printer_t<Ts>...>                       _a_parsers,
    Ts... _a_elements_to_print
)
{
    using namespace std;
    string _l_rv{ _a_begin_str };
    _l_rv.append(" ");
    _l_rv.push_back(_a_begin_char);
    tuple<Ts...> _l_tuple{ tie(_a_elements_to_print...) };
    object_printer_inner<0>(
        _l_rv, _a_object_names, _a_parsers, _l_tuple
    );
    _l_rv.push_back(_a_end_char);
    return _l_rv;
}
template <typename... Ts>
__constexpr_imp std::string
object_printer(
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    const std::string_view                             _a_begin_str,
    const char                                         _a_begin_char,
    const char                                         _a_end_char,
    Ts... _a_elements_to_print
)
{
    using namespace std;
    tuple<default_printer_t<Ts>...> _l_printers;
    return object_printer(
        _a_object_names,
        _a_begin_str,
        _a_begin_char,
        _a_end_char,
        _l_printers,
        _a_elements_to_print...
    );
}

namespace
{
    template <std::size_t I, typename... Ts>
    __constexpr void
        object_printer_inner(
            std::string& _a_str,
            const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
            std::tuple<default_printer_t<Ts>...>                       _a_parsers,
            std::tuple<Ts...> _a_elements_to_print
        ) noexcept
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

_END_ABC_UTILITY_PRINTER_NS