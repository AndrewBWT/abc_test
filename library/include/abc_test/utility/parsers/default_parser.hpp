#pragma once

#include "abc_test/utility/parsers/parser_base.hpp"

#include "abc_test/utility/concepts.hpp"
#include "abc_test/utility/object_printer_parser.hpp"

#include <filesystem>
#include <variant>


_BEGIN_ABC_UTILITY_PARSER_NS

template <typename T>
struct default_parser_t : public parser_base_t<T>
{
public:
    using value_type_t = T;
    __constexpr virtual result_t<T>
        run_parser(parser_input_t& _a_parse_input) const override;
};

template <typename T, typename... Ts>
__constexpr parser_t<T>
            default_parser(Ts... elements) noexcept;
_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_PARSER_NS
template <typename T>
__constexpr result_t<T>
            default_parser_t<T>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "run_parser<T>()."
    );
}

template <typename T, typename... Ts>
__constexpr parser_t<T>
            default_parser(
                Ts... elements
            ) noexcept
{
    using namespace std;
    return make_shared<default_parser_t<T>>(elements...);
}

_END_ABC_UTILITY_PARSER_NS

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
        string_view sv{ _a_parse_input.sv() };
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

template <>
struct default_parser_t<char> : public parser_base_t<char>
{
    __constexpr result_t<char>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using namespace std;
        string_view sv{ _a_parse_input.sv() };
        if (_a_parse_input.check_and_advance("`"))
        {
            char _l_rv{ *_a_parse_input };
            _a_parse_input.advance(1);
            if (_a_parse_input.check_and_advance("`"))
            {
                return _l_rv;
            }
            else
            {
                return unexpected(
                    fmt::format("Could not find expected character `{0}`.", "`")
                );
            }
        }
        else
        {
            return unexpected(
                fmt::format("Could not find expected character `{0}`.", "`")
            );
        }
    }
};

template <typename T>
    requires is_from_chars_convertable_c<T> && (not std::same_as<T, char>)
struct default_parser_t<T> : public parser_base_t<T>
{
    __constexpr result_t<T>
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
    const std::string_view& _a_begin_str,
    std::tuple<parser_t<Ts>...>             _a_parsers,
    parser_input_t& _a_parse_input
);

template <typename T, typename... Ts>
__constexpr result_t<T>
object_parser(
    const utility::object_printer_parser_t& _a_object_print_parser,
    const std::string_view& _a_begin_str,
    parser_input_t& _a_parse_input
);

template <typename T, typename... Ts>
__constexpr result_t<T>
object_parser_with_field_names_and_custom_parsers(
    const utility::object_printer_parser_t& _a_object_print_parser,
    const std::string_view& _a_begin_str,
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    std::tuple<parser_t<Ts>...>                        _a_parsers,
    parser_input_t& _a_parse_input
);

template <typename T, typename... Ts>
__constexpr result_t<T>
object_parser_with_field_names(
    const utility::object_printer_parser_t& _a_object_print_parser,
    const std::string_view& _a_begin_str,
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    parser_input_t& _a_parse_input
);

namespace
{
    template <typename T, typename... Ts>
    __constexpr result_t<T>
        object_parser_internal(
            const utility::object_printer_parser_t& _a_object_print_parser,
            const std::string_view& _a_begin_str,
            const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
            _a_object_names,
            std::tuple<parser_t<Ts>...> _a_parsers,
            parser_input_t& _a_parse_input
        );

    template <std::size_t I, typename... Ts>
    __constexpr_imp void
        run_object_reader(
            const utility::object_printer_parser_t& _a_object_print_parser,
            const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
            _a_object_names,
            std::tuple<parser_t<Ts>...> _a_parsers,
            parser_input_t& _a_parse_input,
            std::tuple<Ts...>& _a_default_tuple
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
    const std::string_view& _a_begin_str,
    std::tuple<parser_t<Ts>...>             _a_parsers,
    parser_input_t& _a_parse_input
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
    const std::string_view& _a_begin_str,
    parser_input_t& _a_parse_input
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
    const std::string_view& _a_begin_str,
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    std::tuple<parser_t<Ts>...>                        _a_parsers,
    parser_input_t& _a_parse_input
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
    const std::string_view& _a_begin_str,
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    parser_input_t& _a_parse_input
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
            const std::string_view& _a_begin_str,
            const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
            _a_object_names,
            std::tuple<parser_t<Ts>...> _a_parsers,
            parser_input_t& _a_parse_input
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
            parser_input_t& _a_parse_input,
            std::tuple<Ts...>& _a_default_tuple
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
        if (auto _l_result{ get<I>(_a_parsers)->run_parser(_a_parse_input) };
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

_BEGIN_ABC_UTILITY_PARSER_NS

template <typename T>
    requires (std::convertible_to<T, std::string_view>)
struct default_parser_t<T> : public parser_base_t<T>
{
    __constexpr result_t<T>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using namespace std;
        _a_parse_input.check_advance_and_throw('"');
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
        _a_parse_input.check_advance_and_throw('"');
        return result_t<T>(_l_rv);
    }
};

template <typename T, typename U>
struct default_parser_t<std::pair<T, U>> : public parser_base_t<std::pair<T, U>>
{
private:
    std::pair<parser_t<T>, parser_t<U>> _m_parsers;
public:
    using value_type = std::pair<T, U>;

    __constexpr
        default_parser_t(
            const parser_t<T>& _a_parser_t,
            const parser_t<U>& _a_parser_u
        );

    default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<T>>&& std::is_default_constructible_v<default_parser_t<U>>)
    : _m_parsers(std::make_pair(default_parser<T>(), default_parser<U>()))
    {
    }

    __constexpr virtual result_t<value_type>
        run_parser(parser_input_t& _a_parse_input) const;
};

template <typename T>
struct default_parser_t<std::vector<T>> : public parser_base_t<std::vector<T>>
{
private:
    parser_t<T> _m_parser;
public:
    __constexpr
        default_parser_t(
            const parser_t<T>& _a_parser
        )
        : _m_parser(_a_parser)
    {
    }

    default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<T>>)
    : _m_parser(mk_parser(default_parser_t<T>()))
    {
    }

    __constexpr virtual result_t<std::vector<T>>
        run_parser(parser_input_t& _a_parse_input) const;
};

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PARSER_NS

/*template <typename T>
requires (std::convertible_to<T, std::string_view>)
__constexpr_imp parse_result_t<T>
                default_parser_t<T>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    if (_a_parse_input.check_and_advance('"'))
    {
    }
    char _l_prev_char{'"'};
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
}*/

template <typename T, typename U>
__constexpr_imp
default_parser_t<std::pair<T, U>>::default_parser_t(
    const parser_t<T>& _a_parser_t,
    const parser_t<U>& _a_parser_u
)
    : _m_parsers{ std::make_pair(_a_parser_t, _a_parser_u) }
{
}

template <typename T, typename U>
__constexpr_imp
result_t<typename default_parser_t<std::pair<T, U>>::value_type>
default_parser_t<std::pair<T, U>>::run_parser(
    parser_input_t& _a_parse_input
) const
{
    using namespace std;
    value_type _l_rv;
    _a_parse_input.check_advance_and_throw("(");
    if (auto _l_first_result{ _m_parsers.first->run_parser(_a_parse_input) };
        _l_first_result.has_value())
    {
        _l_rv.first = _l_first_result.value();
        _a_parse_input.check_advance_and_throw(", ");
        if (auto _l_second_result{ _m_parsers.second->run_parser(_a_parse_input)
            };
            _l_second_result.has_value())
        {
            _l_rv.second = _l_second_result.value();
            _a_parse_input.check_advance_and_throw(")");
            return result_t<value_type>(_l_rv);
        }
        else
        {
            return unexpected(fmt::format(
                "Could not parse std::tuple element 2. Failed with error "
                "message: {0}",
                _l_second_result.error()
            ));
        }
    }
    else
    {
        return unexpected(fmt::format(
            "Could not parse std::tuple element 1. Failed with error "
            "message: {0}",
            _l_first_result.error()
        ));
    }
}

template <typename T>
__constexpr_imp result_t<std::vector<T>>
default_parser_t<std::vector<T>>::run_parser(
    parser_input_t& _a_parse_input
) const
{
    using namespace std;

    vector<T> _l_rv;
    _a_parse_input.check_advance_and_throw("{");
    if (*_a_parse_input == '}')
    {
        _a_parse_input.advance(1);
        return _l_rv;
    }
    else
    {
        //_a_parse_input.advance(1);
        while (true)
        {
            auto _l_result{ _m_parser->run_parser(_a_parse_input) };
            if (not _l_result.has_value())
            {
                return unexpected(_l_result.error());
            }
            _l_rv.push_back(_l_result.value());
            if (_a_parse_input.check_and_advance(", "))
            {
                continue;
            }
            else if (*_a_parse_input == '}')
            {
                _a_parse_input.advance(1);
                return _l_rv;
            }
            else
            {
                return unexpected("unexpected character");
            }
        }
        return unexpected("Shouldn't be poissibel to get here");
    }
}

_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_PARSER_NS

template <typename... Ts>
struct default_parser_t<std::tuple<Ts...>>
    : public parser_base_t<std::tuple<Ts...>>
{
    using value_type = std::tuple<Ts...>;

    __constexpr
        default_parser_t(parser_t<Ts>... _a_parsers);

    __constexpr
        default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<Ts>> && ...)
    : _m_parsers(std::make_tuple(mk_parser(default_parser_t<Ts>())...))
    {
    }

    __constexpr virtual result_t<value_type>
        run_parser(parser_input_t& _a_parse_input) const;

    __constexpr std::tuple<parser_t<Ts>...>&
        get_parsers_ref() noexcept
    {
        return _m_parsers;
    }
private:
    std::tuple<parser_t<Ts>...> _m_parsers;
    template <std::size_t I>
    __constexpr std::optional<std::string>
        run_parser_internal(
            value_type& _a_object,
            parser_input_t& _a_parse_input
        ) const;
};

template <typename... Ts>
struct default_parser_t<std::variant<Ts...>>
    : public parser_base_t<std::variant<Ts...>>
{
    using value_type = std::variant<Ts...>;

    __constexpr
        default_parser_t(parser_t<Ts>... _a_parsers);

    __constexpr
        default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<Ts>> && ...)
    : _m_parsers(std::make_tuple(mk_parser(default_parser_t<Ts>())...))
    {
    }

    __constexpr virtual result_t<value_type>
        run_parser(parser_input_t& _a_parse_input) const;

    __constexpr std::tuple<parser_t<Ts>...>&
        get_parsers_ref() noexcept
    {
        return _m_parsers;
    }
private:
    std::tuple<parser_t<Ts>...> _m_parsers;
    template <std::size_t I>
    __constexpr std::optional<std::string>
        run_parser_internal(
            value_type& _a_object,
            const std::size_t _a_idx,
            parser_input_t& _a_parse_input
        ) const;
};

template <typename T>
struct default_parser_t<std::optional<T>>
    : public parser_base_t<std::optional<T>>
{
    using value_type = std::optional<T>;

    __constexpr
        default_parser_t(
            parser_t<T> _a_parser
        )
        : _m_parser(_a_parser)
    {
    }

    __constexpr
        default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<T>>)
    : _m_parser(mk_parser(default_parser_t<T>()))
    {
    }

    __constexpr virtual result_t<value_type>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using namespace std;
        value_type _l_rv;
        _a_parse_input.check_advance_and_throw("{");
        if (_a_parse_input.check_and_advance("}"))
        {
            return result_t<value_type>(nullopt);
        }
        else
        {
            const result_t<T> _l_result{ _m_parser->run_parser(_a_parse_input) };
            if (_l_result.has_value())
            {
                _a_parse_input.check_advance_and_throw("}");
                return result_t<value_type>(_l_result.value());
            }
            else
            {
                return unexpected("error");
            }
        }
    }
private:
    parser_t<T> _m_parser;
};

template <>
struct default_parser_t<std::filesystem::path>
    : public parser_base_t<std::filesystem::path>
{
    __constexpr virtual result_t<std::filesystem::path>
        run_parser(parser_input_t& _a_parse_input) const;
};

_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_PARSER_NS
template <typename... Ts>
__constexpr
default_parser_t<std::tuple<Ts...>>::default_parser_t(
    parser_t<Ts>... _a_parsers
)
    : _m_parsers{ std::move(_a_parsers)... }
{
}

template <typename... Ts>
__constexpr result_t<typename default_parser_t<std::tuple<Ts...>>::value_type>
default_parser_t<std::tuple<Ts...>>::run_parser(
    parser_input_t& _a_parse_input
) const
{
    using namespace std;
    value_type _l_rv;
    _a_parse_input.check_advance_and_throw("(");
    auto _l_res{ run_parser_internal<0>(_l_rv, _a_parse_input) };
    if (_l_res.has_value())
    {
        return unexpected(_l_res.value());
    }
    _a_parse_input.check_advance_and_throw(")");
    return result_t<value_type>(_l_rv);
}

template <typename... Ts>
template <std::size_t I>
__constexpr std::optional<std::string>
default_parser_t<std::tuple<Ts...>>::run_parser_internal(
    value_type& _a_object,
    parser_input_t& _a_parse_input
) const
{
    using namespace std;
    if (auto _l_result{ get<I>(_m_parsers)->run_parser(_a_parse_input) };
        _l_result.has_value())
    {
        std::get<I>(_a_object) = _l_result.value();
        if constexpr (I + 1 < tuple_size<value_type>{})
        {
            _a_parse_input.check_advance_and_throw(",");
            _a_parse_input.process_whitespace();
            return run_parser_internal<I + 1>(_a_object, _a_parse_input);
        }
        else
        {
            return optional<string>{};
        }
    }
    else
    {
        return optional<string>(fmt::format(
            "Could not parse std::tuple element {0}. Failed with error "
            "message: {1}",
            I,
            _l_result.error()
        ));
    }
}

template <typename... Ts>
__constexpr
default_parser_t<std::variant<Ts...>>::default_parser_t(
    parser_t<Ts>... _a_parsers
)
    : _m_parsers{ std::move(_a_parsers)... }
{
}

template <typename... Ts>
__constexpr result_t<typename default_parser_t<std::variant<Ts...>>::value_type>
default_parser_t<std::variant<Ts...>>::run_parser(
    parser_input_t& _a_parse_input
) const
{
    using namespace std;
    value_type               _l_rv;
    default_parser_t<size_t> _l_size_t_parser;
    result_t<size_t> _l_size_t_result{ _l_size_t_parser.run_parser(_a_parse_input
    ) };
    _a_parse_input.check_advance_and_throw(" ");
    auto _l_res{
        run_parser_internal<0>(_l_rv, _l_size_t_result.value(), _a_parse_input)
    };
    if (_l_res.has_value())
    {
        return unexpected(_l_res.value());
    }
    else
    {
        return result_t<value_type>(_l_rv);
    }
}

template <typename... Ts>
template <std::size_t I>
__constexpr std::optional<std::string>
default_parser_t<std::variant<Ts...>>::run_parser_internal(
    value_type& _a_object,
    const std::size_t _a_idx,
    parser_input_t& _a_parse_input
) const
{
    using namespace std;
    if (_a_idx == I)
    {
        if (auto _l_result{ get<I>(_m_parsers)->run_parser(_a_parse_input) };
            _l_result.has_value())
        {
            std::get<I>(_a_object) = _l_result.value();
            return optional<string>{};
        }
        else
        {
            return optional<string>(fmt::format(
                "Could not parse std::variant element {0}. Failed with error "
                "message: {1}",
                I,
                _l_result.error()
            ));
        }
    }
    else if constexpr (I + 1 == tuple_size<std::tuple<Ts...>>{})
    {
        return optional<string>(fmt::format(
            "Could not parse std::variant element {0}. Tuple does not contain "
            "index",
            I
        ));
    }
    else
    {
        return run_parser_internal<I + 1>(_a_object, _a_idx, _a_parse_input);
    }
}

__constexpr result_t<std::filesystem::path>
default_parser_t<std::filesystem::path>::run_parser(
    parser_input_t& _a_parse_input
) const
{
    return std::filesystem::path(_a_parse_input.sv());
}

_END_ABC_UTILITY_PARSER_NS