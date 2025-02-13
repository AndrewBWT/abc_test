#pragma once
#include "abc_test/internal/utility/concepts.hpp"
#include "abc_test/internal/utility/object_printer_parser.hpp"
#include "abc_test/internal/utility/printers/default_printer.hpp"

_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<bool> : public printer_base_t<bool>
{
    static constexpr bool is_specialized{true};

    __constexpr           std::string
                          run_printer(
                              const bool& _a_object
                          ) const
    {
        return _a_object ? "true" : "false";
    }
};

template <>
struct default_printer_t<char> : public printer_base_t<char>
{
    static constexpr bool is_specialized{true};

    __constexpr           std::string
                          run_printer(
                              const char& _a_object
                          ) const
    {
        return "`" + std::string(1, _a_object) + "`";
    }
};

template <typename T>
    requires (std::is_same_v<std::remove_cvref_t<T>, T> && is_to_string_convertable_c<T> && (not std::same_as<T,bool>) && (not std::same_as<char, T>)) && (not std::floating_point<T>)
struct default_printer_t<T> : public printer_base_t<T>
{
    static constexpr bool is_specialized{true};

    __constexpr           std::string
                          run_printer(
                              const T& _a_object
                          ) const
    {
        using namespace std;
        return to_string(_a_object);
    }
};

template <typename T>
    requires std::floating_point<T>
struct default_printer_t<T> : public printer_base_t<T>
{
    static constexpr bool is_specialized{ true };

    __constexpr           std::string
        run_printer(
            const T& _a_object
        ) const
    {
        char* _l_holder = new char[1000](0);
        std::to_chars(_l_holder, _l_holder + 1000, _a_object);
       std::string _l_rv(_l_holder);
       delete[] _l_holder;
       return _l_rv;
    }
};

template <typename... Ts>
__constexpr std::string
            object_printer_with_custom_printers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                std::tuple<printer_t<Ts>...>            _a_parsers,
                Ts... _a_elements_to_print
            );
template <typename... Ts>
__constexpr std::string
            object_printer(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                Ts... _a_elements_to_print
            );
template <typename... Ts>
__constexpr std::string
            object_printer_with_field_names_and_custom_printers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                std::tuple<printer_t<Ts>...>                       _a_parsers,
                Ts... _a_elements_to_print
            );
template <typename... Ts>
__constexpr std::string
            object_printer_with_field_names(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                Ts... _a_elements_to_print
            );

namespace
{
template <typename... Ts>
__constexpr std::string
            object_printer_internal(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                             _a_object_names,
                std::tuple<printer_t<Ts>...> _a_parsers,
                Ts... _a_elements_to_print
            );
template <std::size_t I, typename... Ts>
__constexpr void
    object_printer_internal(
        const utility::object_printer_parser_t& _a_object_print_parser,
        std::string&                            _a_str,
        const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                     _a_object_names,
        std::tuple<printer_t<Ts>...> _a_parsers,
        std::tuple<Ts...>            _a_elements_to_print
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
__constexpr std::string
            object_printer_with_custom_printers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                std::tuple<printer_t<Ts>...>            _a_printers,
                Ts... _a_elements_to_print
            )
{
    using namespace std;
    return object_printer_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<string_view, sizeof...(Ts)>>{},
        _a_printers,
        _a_elements_to_print...
    );
}

template <typename... Ts>
__constexpr std::string
            object_printer(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                Ts... _a_elements_to_print
            )
{
    using namespace std;
    tuple<printer_t<Ts>...> _l_printers
        = std::make_tuple(default_printer<Ts>()...);
    return object_printer_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<string_view, sizeof...(Ts)>>{},
        _l_printers,
        _a_elements_to_print...
    );
}

template <typename... Ts>
__constexpr_imp std::string
                object_printer_with_field_names_and_custom_printers(
                    const utility::object_printer_parser_t& _a_object_print_parser,
                    const std::string_view&                 _a_begin_str,
                    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                    std::tuple<printer_t<Ts>...>                       _a_parsers,
                    Ts... _a_elements_to_print
                )
{
    using namespace std;
    return object_printer_internal(
        _a_object_print_parser,
        _a_begin_str,
        _a_object_names,
        _a_parsers,
        _a_elements_to_print...
    );
}

template <typename... Ts>
__constexpr_imp std::string
                object_printer_with_field_names(
                    const utility::object_printer_parser_t& _a_object_print_parser,
                    const std::string_view&                 _a_begin_str,
                    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                    Ts... _a_elements_to_print
                )
{
    using namespace std;
    tuple<printer_t<Ts>...> _l_printers
        = std::make_tuple(mk_printer(default_printer_t<Ts>())...);
    return object_printer_internal(
        _a_object_print_parser,
        _a_begin_str,
        _a_object_names,
        _l_printers,
        _a_elements_to_print...
    );
}

namespace
{
template <typename... Ts>
__constexpr std::string
            object_printer_internal(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                             _a_object_names,
                std::tuple<printer_t<Ts>...> _a_parsers,
                Ts... _a_elements_to_print
            )
{
    using namespace std;
    string _l_rv{_a_begin_str};
    if (_a_object_print_parser.space_after_object_name)
    {
        _l_rv.append(" ");
    }
    _l_rv.push_back(_a_object_print_parser.begin_char);
    tuple<Ts...> _l_tuple{tie(_a_elements_to_print...)};
    object_printer_internal<0>(
        _a_object_print_parser, _l_rv, _a_object_names, _a_parsers, _l_tuple
    );
    _l_rv.push_back(_a_object_print_parser.end_char);
    return _l_rv;
}

template <std::size_t I, typename... Ts>
__constexpr void
    object_printer_internal(
        const utility::object_printer_parser_t& _a_object_print_parser,
        std::string&                            _a_str,
        const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                     _a_object_names,
        std::tuple<printer_t<Ts>...> _a_parsers,
        std::tuple<Ts...>            _a_elements_to_print
    ) noexcept
{
    /*
        bool space_after_object_name = true;
    bool space_before_field_name_and_field_separator = true;
    bool space_after_field_name_and_field_separator = true;
    bool space_after_field_delimieter = true;
    bool space_before_field_delimiter = false;
    */
    using namespace std;
    if (_a_object_names.has_value())
    {
        _a_str.append(get<I>(_a_object_names.value()));
        if (_a_object_print_parser.space_before_field_name_and_field_separator)
        {
            _a_str.push_back(' ');
        }
        _a_str.push_back(
            _a_object_print_parser.delimiter_between_field_name_and_field
        );
        if (_a_object_print_parser.space_after_field_name_and_field_separator)
        {
            _a_str.push_back(' ');
        }
    }
    _a_str.append(
        std::get<I>(_a_parsers)->run_printer(std::get<I>(_a_elements_to_print))
    );
    if constexpr (I + 1 < sizeof...(Ts))
    {
        if (_a_object_print_parser.space_before_field_delimiter)
        {
            _a_str.push_back(' ');
        }
        _a_str.push_back(_a_object_print_parser.delimiter_between_fields);
        if (_a_object_print_parser.space_after_field_delimieter)
        {
            _a_str.push_back(' ');
        }
        object_printer_internal<I + 1>(
            _a_object_print_parser,
            _a_str,
            _a_object_names,
            _a_parsers,
            _a_elements_to_print
        );
    }
}
} // namespace

_END_ABC_UTILITY_PRINTER_NS