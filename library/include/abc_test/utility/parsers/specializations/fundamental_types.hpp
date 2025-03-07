#pragma once
#include "abc_test/utility/concepts.hpp"
#include "abc_test/utility/object_printer_parser.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"

_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<bool> : public parser_base_t<bool>
{
    __constexpr result_t<bool>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        string_view sv{_a_parse_input.sv()};
        if (_a_parse_input.check_and_advance("true"))
        {
            return result_t<bool>(true);
        }
        else if (_a_parse_input.check_and_advance("false"))
        {
            return result_t<bool>(false);
        }
        else
        {
            return result_t<bool>("Couldn't parse bool");
        }
    }
};

template <typename T>
requires is_from_chars_convertable_c<T>
struct default_parser_t<T> : public parser_base_t<T>
{
    __constexpr result_t<T>
                run_parser(
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
            return result_t<T>(result);
        }
        else
        {
            return result_t<T>(unexpected(fmt::format(
                "Could not parse \"{0}\" using std::from_chars", _l_str
            ))

            );
        }
    }
};

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_printer_with_custom_parsers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                std::tuple<parser_t<Ts>...>             _a_parsers,
                parser_input_t&                         _a_parse_input
            );

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                parser_input_t&                         _a_parse_input
            );

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names_and_custom_parsers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                std::tuple<parser_t<Ts>...>                        _a_parsers,
                parser_input_t&                                    _a_parse_input
            );

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                parser_input_t&                                    _a_parse_input
            );

namespace
{
template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_internal(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                            _a_object_names,
                std::tuple<parser_t<Ts>...> _a_parsers,
                parser_input_t&             _a_parse_input
            );

template <std::size_t I, typename... Ts>
__constexpr_imp void
    run_object_reader(
        const utility::object_printer_parser_t& _a_object_print_parser,
        const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                    _a_object_names,
        std::tuple<parser_t<Ts>...> _a_parsers,
        parser_input_t&             _a_parse_input,
        std::tuple<Ts...>&          _a_default_tuple
    );
} // namespace

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PARSER_NS

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
        return expescted<T, string>(result);
    }
    else
    {
        return parse_error<T>(
            fmt::format("Could not parse \"{0}\" using std::from_chars", _l_str)

        );
    }
}*/

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_printer_with_custom_parsers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                std::tuple<parser_t<Ts>...>             _a_parsers,
                parser_input_t&                         _a_parse_input
            )
{
    using namespace std;
    return object_parser_internal<T>(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<string_view, sizeof...(Ts)>>{},
        _a_parsers,
        _a_parse_input
    );
}

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                parser_input_t&                         _a_parse_input
            )
{
    using namespace std;
    tuple<parser_t<Ts>...> _l_printers
        = std::make_tuple(default_parser<Ts>()...);
    return object_parser_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<string_view, sizeof...(Ts)>>{},
        _l_printers,
        _a_parse_input
    );
}

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names_and_custom_parsers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                std::tuple<parser_t<Ts>...>                        _a_parsers,
                parser_input_t&                                    _a_parse_input
            )
{
    using namespace std;
    return object_parser_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<string_view, sizeof...(Ts)>>{_a_object_names},
        _a_parsers,
        _a_parse_input
    );
}

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                parser_input_t&                                    _a_parse_input
            )
{
    using namespace std;
    tuple<parser_t<Ts>...> _l_parsers
        = std::make_tuple(default_parser<Ts>()...);
    return object_parser_internal<T>(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<string_view, sizeof...(Ts)>>{_a_object_names},
        _l_parsers,
        _a_parse_input
    );
}

namespace
{
template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_internal(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::string_view&                 _a_begin_str,
                const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                            _a_object_names,
                std::tuple<parser_t<Ts>...> _a_parsers,
                parser_input_t&             _a_parse_input
            )
{
    using namespace std;
    using namespace abc::utility::str;
    tuple<Ts...> default_tuple;
    _a_parse_input.check_advance_and_throw(_a_begin_str);
    if (_a_object_print_parser.space_after_object_name)
    {
        _a_parse_input.check_advance_and_throw(' ');
    }
    _a_parse_input.check_advance_and_throw(_a_object_print_parser.begin_char);
    run_object_reader<0>(
        _a_object_print_parser,
        _a_object_names,
        _a_parsers,
        _a_parse_input,
        default_tuple
    );
    _a_parse_input.check_advance_and_throw(_a_object_print_parser.end_char);
    return parse_result_t<T>(std::make_from_tuple<T>(default_tuple));
}

template <std::size_t I, typename... Ts>
__constexpr_imp void
    run_object_reader(
        const utility::object_printer_parser_t& _a_object_print_parser,
        const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
                                    _a_object_names,
        std::tuple<parser_t<Ts>...> _a_parsers,
        parser_input_t&             _a_parse_input,
        std::tuple<Ts...>&          _a_default_tuple
    )
{
    using namespace std;
    if (_a_object_names.has_value())
    {
        _a_parse_input.check_advance_and_throw(get<I>(_a_object_names.value()));
        if (_a_object_print_parser.space_before_field_name_and_field_separator)
        {
            _a_parse_input.check_advance_and_throw(' ');
        }
        _a_parse_input.check_advance_and_throw(
            _a_object_print_parser.delimiter_between_field_name_and_field
        );
        if (_a_object_print_parser.space_after_field_name_and_field_separator)
        {
            _a_parse_input.check_advance_and_throw(' ');
        }
    }
    if (auto _l_result{get<I>(_a_parsers)->run_parser(_a_parse_input)};
        _l_result.has_value())
    {
        get<I>(_a_default_tuple) = _l_result.value();
        if constexpr (I + 1 < tuple_size<tuple<Ts...>>{})
        {
            if (_a_object_print_parser.space_before_field_delimiter)
            {
                _a_parse_input.check_advance_and_throw(' ');
            }
            _a_parse_input.check_advance_and_throw(
                _a_object_print_parser.delimiter_between_fields
            );
            if (_a_object_print_parser.space_after_field_delimieter)
            {
                _a_parse_input.check_advance_and_throw(' ');
            }
            run_object_reader<I + 1>(
                _a_object_print_parser,
                _a_object_names,
                _a_parsers,
                _a_parse_input,
                _a_default_tuple
            );
        }
    }
}
} // namespace

_END_ABC_UTILITY_PARSER_NS