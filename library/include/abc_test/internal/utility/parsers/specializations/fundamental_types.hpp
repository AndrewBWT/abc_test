#pragma once
#include "abc_test/internal/utility/parsers/default_parser.hpp"

_BEGIN_ABC_UTILITY_STR_NS

template <typename T>
requires is_from_chars_convertable_c<T>
struct default_parser_t<T> : public parser_base_t<T>
{
    __constexpr parse_result_t<T>
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
            return expected<T, string>(result);
        }
        else
        {
            return parse_error<T>(fmt::format(
                "Could not parse \"{0}\" using std::from_chars", _l_str
            )

            );
        }
    }
};

template <typename T, typename... Ts>
__constexpr abc::utility::str::parse_result_t<T>
            object_parser(
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                const std::string_view                             _a_begin_str,
                const char                                         _a_begin_char,
                const char                                         _a_end_char,
                utility::str::parser_input_t&                      _a_parse_input,
                std::tuple<default_parser_t<Ts>...>                _a_parsers
            );
template <typename T, typename... Ts>
requires (std::is_default_constructible_v<default_parser_t<Ts>> && ...)
__constexpr abc::utility::str::parse_result_t<T>
            object_parser(
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                const std::string_view                             _a_begin_str,
                const char                                         _a_begin_char,
                const char                                         _a_end_char,
                utility::str::parser_input_t&                      _a_parse_input
            );

namespace
{
template <std::size_t I, typename... Ts>
__constexpr void
    run_object_reader(
        utility::str::parser_input_t&                      _a_parse_input,
        std::tuple<Ts...>&                                 _a_default_tuple,
        const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
        std::tuple<default_parser_t<Ts>...>                _a_parsers
    ) noexcept;
} // namespace

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS

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

template <typename T, typename... Ts>
__constexpr_imp abc::utility::str::parse_result_t<T>
                object_parser(
                    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                    const std::string_view                             _a_begin_str,
                    const char                                         _a_begin_char,
                    const char                                         _a_end_char,
                    utility::str::parser_input_t&                      _a_parse_input,
                    std::tuple<default_parser_t<Ts>...>                _a_parsers
                )
{
    using namespace std;
    using namespace abc::utility::str;
    tuple<Ts...> default_tuple;
    _a_parse_input.check_advance_and_throw(_a_begin_str);
    _a_parse_input.check_advance_and_throw(' ');
    _a_parse_input.check_advance_and_throw(_a_begin_char);
    run_object_reader<0>(
        _a_parse_input, default_tuple, _a_object_names, _a_parsers
    );
    _a_parse_input.check_advance_and_throw(_a_end_char);
    return parse_result_t<T>(std::make_from_tuple<T>(default_tuple));
}

template <typename T, typename... Ts>
requires (std::is_default_constructible_v<default_parser_t<Ts>> && ...)
__constexpr_imp abc::utility::str::parse_result_t<T>
                object_parser(
                    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                    const std::string_view                             _a_begin_str,
                    const char                                         _a_begin_char,
                    const char                                         _a_end_char,
                    utility::str::parser_input_t&                      _a_parse_input
                )
{
    std::tuple<default_parser_t<Ts>...> _l_parsers;
    return object_parser<T>(
        _a_object_names,
        _a_begin_str,
        _a_begin_char,
        _a_end_char,
        _a_parse_input,
        _l_parsers
    );
}

namespace
{
template <std::size_t I, typename... Ts>
__constexpr_imp void
    run_object_reader(
        utility::str::parser_input_t&                      _a_parse_input,
        std::tuple<Ts...>&                                 _a_default_tuple,
        const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
        std::tuple<default_parser_t<Ts>...>                _a_parsers
    ) noexcept
{
    using namespace std;
    _a_parse_input.check_advance_and_throw(get<I>(_a_object_names));
    _a_parse_input.check_advance_and_throw(" = ");
    if (auto _l_result{std::get<I>(_a_parsers).run_parser(_a_parse_input)};
        _l_result.has_value())
    {
        get<I>(_a_default_tuple) = _l_result.value();
        if constexpr (I + 1 < tuple_size<tuple<Ts...>>{})
        {
            _a_parse_input.check_advance_and_throw(", ");
            run_object_reader<I + 1>(
                _a_parse_input, _a_default_tuple, _a_object_names, _a_parsers
            );
        }
    }
}
} // namespace

_END_ABC_UTILITY_STR_NS