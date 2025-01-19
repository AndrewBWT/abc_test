#pragma once
#include "abc_test/internal/utility/str/parsers/default_parser.hpp"

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
        const string_view _l_str{ _a_parse_input.sv() };
        auto [ptr, ec]
            = from_chars(_l_str.data(), _l_str.data() + _l_str.size(), result);

        if (ec == std::errc())
        {
            _a_parse_input.advance((ptr - _l_str.data()) + 0);
            return expected<T, string>(result);
        }
        else
        {
            return parse_result_t<T>(unexpected(fmt::format(
                "Could not parse \"{0}\" using std::from_chars", _l_str
            ))

            );
        }
    }
};

template <typename T>
    requires (std::convertible_to<T, std::string_view>)
struct default_parser_t<T> : public parser_base_t<T>
{
    __constexpr parse_result_t<T>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using namespace std;
        if (_a_parse_input.check_and_advance('"'))
        {
        }
        char _l_prev_char{ '"' };
        T    _l_rv;
        while (true)
        {
            if (*_a_parse_input == '"' && _l_prev_char != '\\')
            {
                break;
            }
            else
            {
                _l_rv.push_back(*_a_parse_input);
                _l_prev_char = *_a_parse_input;
                ++_a_parse_input;
            }
        }
        if (_a_parse_input.check_and_advance('"'))
        {
        }
        return parse_result_t<T>(_l_rv);
    }
};

template <typename... Ts>
struct default_parser_t<std::tuple<Ts...>> : public parser_base_t<std::tuple<Ts...>>
{
    using value_type = std::tuple<Ts...>;

    default_parser_t(
        default_parser_t<Ts>... _a_parsers
    )
        : _m_parsers{ std::move(_a_parsers)... }
    {}

    default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<Ts>> && ...)
    = default;

    __constexpr virtual parse_result_t<value_type>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        value_type _l_rv;
        if (_a_parse_input.check_and_advance("("))
        {
        }
        run_parser_internal<0>(_l_rv, _a_parse_input);
        if (_a_parse_input.check_and_advance(")"))
        {
        }
        return parse_result_t<value_type>(_l_rv);
        // return run_parser_(_a_parse_input, T{});
    }
private:
    std::tuple<default_parser_t<Ts>...> _m_parsers;

    template <std::size_t I>
    __constexpr void
        run_parser_internal(
            value_type& _a_object,
            parser_input_t& _a_parse_input
        ) const
    {
        if (auto _l_result{ std::get<I>(_m_parsers).run_parser(_a_parse_input) };
            _l_result.has_value())
        {
            std::get<I>(_a_object) = _l_result.value();
        }
        if constexpr (I + 1 < std::tuple_size<value_type>{})
        {
            if (_a_parse_input.check_and_advance(", "))
            {
            }
            run_parser_internal<I + 1>(_a_object, _a_parse_input);
        }
    }
};

template <typename T, typename U>
struct default_parser_t<std::pair<T, U>> : public parser_base_t<std::pair<T, U>>
{
private:
    std::pair<default_parser_t<T>, default_parser_t<U>> _m_parsers;
public:
    using value_type = std::pair<T, U>;

    default_parser_t(
        const default_parser_t<T>& _a_parser_t,
        const default_parser_t<U>& _a_parser_u
    )
        : _m_parsers{ std::make_pair(_a_parser_t, _a_parser_u) }
    {}

    default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<T>>
    && std::is_default_constructible_v<default_parser_t<U>>)
        = default;

    __constexpr virtual parse_result_t<value_type>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        value_type _l_rv;
        if (_a_parse_input.check_and_advance("("))
        {
        }
        if (auto _l_first_result{ _m_parsers.first.run_parser(_a_parse_input) };
            _l_first_result.has_value())
        {
            _l_rv.first = _l_first_result.value();
        }
        if (_a_parse_input.check_and_advance(", "))
        {
        }
        if (auto _l_second_result{ _m_parsers.second.run_parser(_a_parse_input) };
            _l_second_result.has_value())
        {
            _l_rv.second = _l_second_result.value();
        }
        if (_a_parse_input.check_and_advance(")"))
        {
        }
        return parse_result_t<value_type>(_l_rv);
        // return run_parser_(_a_parse_input, T{});
    }
};

template <std::size_t I, typename... Ts>
__constexpr void
run_object_reader(
    utility::str::parser_input_t& _a_parse_input,
    std::tuple<Ts...>& _a_default_tuple,
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    std::tuple<default_parser_t<Ts>...>                        _a_parsers
) noexcept
{
    if (_a_parse_input.check_and_advance(std::get<I>(_a_object_names)))
    {
    }
    if (_a_parse_input.check_and_advance(" = "))
    {
    }
    if (auto _l_result{ std::get<I>(_a_parsers).run_parser(_a_parse_input) };
        _l_result.has_value())
    {
        std::get<I>(_a_default_tuple) = _l_result.value();
        if constexpr (I + 1 < std::tuple_size<std::tuple<Ts...>>{})
        {
            if (_a_parse_input.check_and_advance(", "))
            {
            }
            run_object_reader<I + 1>(
                _a_parse_input, _a_default_tuple, _a_object_names, _a_parsers
            );
        }
    }
}


template <typename T, typename... Ts>
__constexpr abc::utility::str::parse_result_t<T>
object_parser(
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    const std::string_view                             _a_begin_str,
    const char                                         _a_begin_char,
    const char                                         _a_end_char,
    utility::str::parser_input_t& _a_parse_input,
    std::tuple<default_parser_t<Ts>...>                        _a_parsers
)
{
    using namespace std;
    using namespace abc::utility::str;
    tuple<Ts...> default_tuple;
    if (_a_parse_input.check_and_advance(_a_begin_str))
    {
        return parse_result_t<T>(unexpected(fmt::format(
            "Object parser could not find _a_begin_str in _a_parse_input. "
            "_a_begin_str = \"{0}\", _a_parse_input = \"{1}\"",
            _a_begin_str,
            _a_parse_input
        )));
    }
    if (_a_parse_input.check_and_advance(' '))
    {
        return parse_result_t<T>(unexpected(fmt::format(
            "Object parser could not find expected space in _a_parse_input. "
            "_a_parse_input = \"{1}\"",
            _a_begin_str,
            _a_parse_input
        )));
    }
    if (_a_parse_input.check_and_advance(_a_begin_char))
    {
        return parse_result_t<T>(unexpected("Couldn't work out"));
    }
    run_object_reader<0>(
        _a_parse_input, default_tuple, _a_object_names, _a_parsers
    );
    if (_a_parse_input.check_and_advance(_a_end_char))
    {
        return parse_result_t<T>(unexpected("Couldn't work out"));
    }
    else
    {
        return parse_result_t<T>(std::make_from_tuple<T>(default_tuple));
    }
}

template <typename T, typename... Ts>
    requires (std::is_default_constructible_v<default_parser_t<Ts>> && ...)
__constexpr abc::utility::str::parse_result_t<T>
object_parser(
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    const std::string_view                             _a_begin_str,
    const char                                         _a_begin_char,
    const char                                         _a_end_char,
    utility::str::parser_input_t& _a_parse_input
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
_END_ABC_UTILITY_STR_NS