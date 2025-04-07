#pragma once

#include "abc_test/utility/concepts.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/object_printer_parser.hpp"
#include "abc_test/utility/parsers/parser_base.hpp"

#include <filesystem>
#include <fmt/xchar.h>
#include <functional>
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
        if (_a_parse_input.check_and_advance(U"true"))
        {
            return result_t<bool>(true);
        }
        else if (_a_parse_input.check_and_advance(U"false"))
        {
            return result_t<bool>(false);
        }
        else
        {
            return result_t<bool>(u8"Couldn't parse bool");
        }
    }
};

template <typename T>
requires enum_has_list_c<T>
struct default_parser_t<T> : public parser_base_t<T>
{
    using value_type_t = T;

    default_parser_t(
        const enum_helper_string_case_t _a_enum_helper_string_case
        = enum_helper_string_case_t::unchanged
    )
        : _m_enum_helper_string_case(_a_enum_helper_string_case)
    {}

    enum_helper_string_case_t _m_enum_helper_string_case;

    __constexpr               result_t<T>
                              run_parser(
                                  parser_input_t& _a_parse_input
                              ) const
    {
        return _ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
            .parse_enum(_a_parse_input, _m_enum_helper_string_case);
    }
};

template <typename T>
struct character_parser_t : public parser_base_t<T>
{
    __constexpr
    character_parser_t(
        const std::u8string _a_surrounding_str = std::u8string{}
    )
        : _m_surrounding_str(_a_surrounding_str)
    {}
protected:
    __constexpr result_t<std::monostate>
                remove_surrounding_str(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        if (_a_parse_input.check_and_advance(_m_surrounding_str))
        {
            return monostate{};
        }
        else
        {
            return unexpected(fmt::format(
                u8"Could not find expected surrounding string \"{0}\".",
                _m_surrounding_str
            ));
        }
    }
private:
    std::u8string _m_surrounding_str;
};

namespace detail
{
template <typename T>
__constexpr result_t<T>
            from_hex(
                parser_input_t&   _a_parse_input,
                const std::size_t _a_characters_to_take
            ) noexcept
{
    using namespace std;
    const u8string_view _l_sv{_a_parse_input.get_u32string(_a_characters_to_take
    )};
    const result_t<string> _l_sv_as_str{abc::convert_u32string_to_string(_l_sv)
    };
    if (_l_sv_as_str.has_value())
    {
        T _l_res;
        auto [ptr, ec] = from_chars(
            _l_sv_as_str.value().data(),
            _l_sv_as_str.value().data() + _l_sv_as_str.value().size(),
            _l_res,
            16
        );
        return _l_res;
    }
    else
    {
        return unexpected(_l_sv_as_str.error());
    }
}
} // namespace detail

template <typename T>
requires std::same_as<T, char> || std::same_as<T, char8_t>
struct default_parser_t<T> : public character_parser_t<T>
{
public:
    using character_parser_t<T>::character_parser_t;

    __constexpr result_t<T>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        T _l_char;
        if (const result_t<monostate> _l_result_1{
                this->remove_surrounding_str(_a_parse_input)
            };
            _l_result_1.has_value())
        {
            if (_a_parse_input.check_and_advance(u8"\\x"))
            {
                if (const result_t<T> _l_char_result{
                        detail::from_hex<T>(_a_parse_input, 2)
                    };
                    _l_char_result.has_value())
                {
                    _l_char = _l_char_result.value();
                }
                else
                {
                    return unexpected(_l_char_result.error());
                }
            }
            else
            {
                _l_char = static_cast<T>(*_a_parse_input);
                _a_parse_input.advance(1);
            }
            if (const result_t<monostate> _l_result_2{
                    this->remove_surrounding_str(_a_parse_input)
                };
                _l_result_2.has_value())
            {
                return _l_char;
            }
            else
            {
                return unexpected(_l_result_2.error());
            }
        }
        else
        {
            return unexpected(_l_result_1.error());
        }
    }
};

template <typename T>
    requires std::same_as<T, char16_t> || (sizeof(wchar_t) == 2 && std::same_as<T, wchar_t>)
struct default_parser_t<T> : public character_parser_t<T>
{
public:
    using character_parser_t<T>::character_parser_t;

    __constexpr result_t<T>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        T _l_char;
        if (const result_t<monostate> _l_result_1{
                this->remove_surrounding_str(_a_parse_input)
            };
            _l_result_1.has_value())
        {
            if (_a_parse_input.check_and_advance(U"\\x"))
            {
                if (const result_t<T> _l_char_result{
                        detail::from_hex<T>(_a_parse_input, 4)
                    };
                    _l_char_result.has_value())
                {
                    _l_char = _l_char_result.value();
                }
                else
                {
                    return unexpected(_l_char_result.error());
                }
            }
            else
            {
                _l_char = abc::convert_u32string_to_u16string(
                              _a_parse_input.process_characters(1)
                )
                              .at(0);
            }
            if (const result_t<monostate> _l_result_2{
                    this->remove_surrounding_str(_a_parse_input)
                };
                _l_result_2.has_value())
            {
                return _l_char;
            }
            else
            {
                return unexpected(_l_result_2.error());
            }
        }
        else
        {
            return unexpected(_l_result_1.error());
        }
    }
};

template <typename T>
    requires std::same_as<T, char32_t> || (sizeof(wchar_t) == 4 && std::same_as<T, wchar_t>)
struct default_parser_t<T> : public character_parser_t<T>
{
public:
    using character_parser_t<T>::character_parser_t;

    __constexpr result_t<T>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        T _l_char;
        if (const result_t<monostate> _l_result_1{
                this->remove_surrounding_str(_a_parse_input)
            };
            _l_result_1.has_value())
        {
            if (_a_parse_input.check_and_advance(U"\\x"))
            {
                if (const result_t<T> _l_char_result{
                        detail::from_hex<T>(_a_parse_input, 4)
                    };
                    _l_char_result.has_value())
                {
                    _l_char = _l_char_result.value();
                }
                else
                {
                    return unexpected(_l_char_result.error());
                }
            }
            else
            {
                _l_char = _a_parse_input.process_characters(1).at(0);
            }
            if (const result_t<monostate> _l_result_2{
                    this->remove_surrounding_str(_a_parse_input)
                };
                _l_result_2.has_value())
            {
                return _l_char;
            }
            else
            {
                return unexpected(_l_result_2.error());
            }
        }
        else
        {
            return unexpected(_l_result_1.error());
        }
    }
};

/*template <>
struct default_parser_t<wchar_t> : public parser_base_t<wchar_t>
{
    __no_constexpr_imp result_t<wchar_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        __STATIC_ASSERT(
            wchar_t, "wchar_t not supported by abc::utility::run_parser"
        );
    }
};*/

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
        T                      result{};
        const result_t<string> _l_result_str{_a_parse_input.ascii_string()};
        if (_l_result_str.has_value())
        {
            const string& _l_str{_l_result_str.value()};
            auto [ptr, ec] = from_chars(
                _l_str.data(), _l_str.data() + _l_str.size(), result
            );

            if (ec == std::errc())
            {
                _a_parse_input.advance((ptr - _l_str.data()) + 0);
                return result_t<T>(result);
            }
            else
            {
                return result_t<T>(unexpected(fmt::format(
                    u8"Could not parse \"{0}\" using std::from_chars",
                    convert_string_to_u8string(_l_result_str.value()).value()
                ))

                );
            }
        }
        else
        {
            return result_t<T>(unexpected(fmt::format(
                u8"Could not convert std::u8string to std::string. Error was "
                u8"\"{0}\".",
                _l_result_str.error()
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

_BEGIN_ABC_UTILITY_PARSER_NS

/*template <typename T>
struct default_parser_t<std::basic_string<T>>
    : public parser_base_t<std::basic_string<T>>
{
    __constexpr
    default_parser_t()
        : default_parser_t<std::basic_string<T>>(default_parser<T>())
    {}

    __constexpr
    default_parser_t(
        const parser_t<T>& _a_parser
    ) noexcept
        : _m_parser(_a_parser)
    {}

    __constexpr result_t<std::basic_string<T>>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        _a_parse_input.check_advance_and_throw('"');
        T                    _l_prev_char{'"'};
        std::basic_string<T> _l_rv;
        while (true)
        {
            if (*_a_parse_input == '"' && _l_prev_char != '\\')
            {
                break;
            }
            else
            {
                const result_t<T> _l_result{_m_parser->run_parser(_a_parse_input
                )};
                if (_l_result.has_value())
                {
                    _l_rv.push_back(_l_result.value());
                    _l_prev_char = _l_result.value();
                }
            }
        }
        _a_parse_input.check_advance_and_throw('"');
        return result_t<std::basic_string<T>>(_l_rv);
    }
private:
    parser_t<T> _m_parser;
};*/

template <typename T>
requires (std::same_as<std::basic_string<T>, std::string> || std::same_as<std::basic_string<T>, std::wstring> || std::same_as<std::basic_string<T>, std::u8string> || std::same_as<std::basic_string<T>, std::u16string> || std::same_as<std::basic_string<T>, std::u32string>)
struct default_parser_t<std::basic_string<T>>
    : public parser_base_t<std::basic_string<T>>
{
    static constexpr bool is_specialized{true};

    __constexpr           result_t<std::basic_string<T>>
                          run_parser(
                              parser_input_t& _a_parse_input
                          ) const
    {
        using namespace std;
        using arg_type_t = basic_string<T>;
        char32_t  _l_char{U'"'};
        char32_t  _l_prev_char;
        char32_t  _l_char2{U'\\'};
        size_t    _l_idx{0};
        u32string _l_str{};
        if (_a_parse_input.check_and_advance(_l_char))
        {
            const u32string _l_str{
                _a_parse_input.continue_until_char1_but_not_proceeded_by_char2(
                    U'"', U'\\'
                )
            };
            _a_parse_input.advance(1);
            if constexpr (same_as<arg_type_t, string>)
            {
                const result_t<string> _l_rv{
                    abc::convert_u32string_to_string(_l_str)
                };
                if (_l_rv.has_value())
                {
                    return _l_rv;
                }
                else
                {
                    return unexpected(fmt::format(u8"Could not convert"));
                }
            }
            else if constexpr (same_as<arg_type_t, u8string>)
            {
                const result_t<u8string> _l_rv{
                    abc::convert_u32string_to_u8string(_l_str)
                };
                if (_l_rv.has_value())
                {
                    return _l_rv;
                }
                else
                {
                    return unexpected(fmt::format("Could not convert"));
                }
            }
            else if constexpr (same_as<arg_type_t, u16string>
                               || (same_as<arg_type_t, wstring>
                                   && (sizeof(wchar_t) == 2)))
            {
                const result_t<u16string> _l_rv{
                    abc::convert_u32string_to_u16string(_l_str)
                };
                if (_l_rv.has_value())
                {
                    return _l_rv;
                }
                else
                {
                    return unexpected(fmt::format("Could not convert"));
                }
            }
            else if constexpr (same_as<arg_type_t, u32string>
                               || (same_as<arg_type_t, wstring>
                                   && (sizeof(wchar_t) == 4)))
            {
                return _l_str;
            }
            else
            {
                __STATIC_ASSERT(
                    T, "wchar_t of this specific size is not supported"
                );
            }
        }
        else
        {
            return unexpected(fmt::format(
                u8"Expected {0} to have a prefix of the string \"{1}\". "
                u8"Remaining string = {2}",
                type_id<T>(),
                convert_u32string_to_u8string(u32string(1, _l_char)),
                _a_parse_input.get_u8string()
            ));
        }
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
    requires (std::is_default_constructible_v<default_parser_t<T>> && std::is_default_constructible_v<default_parser_t<U>>)
        : _m_parsers(std::make_pair(default_parser<T>(), default_parser<U>()))
    {}

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
    {}

    default_parser_t()
    requires (std::is_default_constructible_v<default_parser_t<T>>)
        : _m_parser(mk_parser(default_parser_t<T>()))
    {}

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
    : _m_parsers{std::make_pair(_a_parser_t, _a_parser_u)}
{}

template <typename T, typename U>
__constexpr_imp result_t<typename default_parser_t<std::pair<T, U>>::value_type>
                default_parser_t<std::pair<T, U>>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    value_type _l_rv;
    _a_parse_input.check_advance_and_throw(U"(");
    if (auto _l_first_result{_m_parsers.first->run_parser(_a_parse_input)};
        _l_first_result.has_value())
    {
        _l_rv.first = _l_first_result.value();
        _a_parse_input.check_advance_and_throw(U", ");
        if (auto _l_second_result{_m_parsers.second->run_parser(_a_parse_input)
            };
            _l_second_result.has_value())
        {
            _l_rv.second = _l_second_result.value();
            _a_parse_input.check_advance_and_throw(U")");
            return result_t<value_type>(_l_rv);
        }
        else
        {
            return unexpected(fmt::format(
                u8"Could not parse std::tuple element 2. Failed with error "
                u8"message: {0}",
                _l_second_result.error()
            ));
        }
    }
    else
    {
        return unexpected(fmt::format(
            u8"Could not parse std::tuple element 1. Failed with error "
            u8"message: {0}",
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
    _a_parse_input.check_advance_and_throw(U"{");
    if (_a_parse_input.check_and_advance(U"}"))
    {
        return _l_rv;
    }
    else
    {
        //_a_parse_input.advance(1);
        while (true)
        {
            auto _l_result{_m_parser->run_parser(_a_parse_input)};
            if (not _l_result.has_value())
            {
                return unexpected(_l_result.error());
            }
            _l_rv.push_back(_l_result.value());
            if (_a_parse_input.check_and_advance(U","))
            {
                _a_parse_input.process_whitespace();
                continue;
            }
            else if (_a_parse_input.check_and_advance(U"}"))
            {
                return _l_rv;
            }
            else
            {
                return unexpected(u8"unexpected character");
            }
        }
        return unexpected(u8"Shouldn't be poissibel to get here");
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
    {}

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
    __constexpr std::optional<std::u8string>
                run_parser_internal(
                    value_type&     _a_object,
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
    {}

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
    __constexpr std::optional<std::u8string>
                run_parser_internal(
                    value_type&       _a_object,
                    const std::size_t _a_idx,
                    parser_input_t&   _a_parse_input
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
    {}

    __constexpr
    default_parser_t()
    requires (std::is_default_constructible_v<default_parser_t<T>>)
        : _m_parser(mk_parser(default_parser_t<T>()))
    {}

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
            const result_t<T> _l_result{_m_parser->run_parser(_a_parse_input)};
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
    : _m_parsers{std::move(_a_parsers)...}
{}

template <typename... Ts>
__constexpr result_t<typename default_parser_t<std::tuple<Ts...>>::value_type>
            default_parser_t<std::tuple<Ts...>>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    value_type _l_rv;
    _a_parse_input.check_advance_and_throw(U"(");
    auto _l_res{run_parser_internal<0>(_l_rv, _a_parse_input)};
    if (_l_res.has_value())
    {
        return unexpected(_l_res.value());
    }
    _a_parse_input.check_advance_and_throw(U")");
    return result_t<value_type>(_l_rv);
}

template <typename... Ts>
template <std::size_t I>
__constexpr std::optional<std::u8string>
            default_parser_t<std::tuple<Ts...>>::run_parser_internal(
        value_type&     _a_object,
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    if (auto _l_result{get<I>(_m_parsers)->run_parser(_a_parse_input)};
        _l_result.has_value())
    {
        std::get<I>(_a_object) = _l_result.value();
        if constexpr (I + 1 < tuple_size<value_type>{})
        {
            _a_parse_input.check_advance_and_throw(U",");
            _a_parse_input.process_whitespace();
            return run_parser_internal<I + 1>(_a_object, _a_parse_input);
        }
        else
        {
            return nullopt;
        }
    }
    else
    {
        return optional<u8string>(fmt::format(
            u8"Could not parse std::tuple element {0}. Failed with error "
            u8"message: {1}",
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
    : _m_parsers{std::move(_a_parsers)...}
{}

template <typename... Ts>
__constexpr result_t<typename default_parser_t<std::variant<Ts...>>::value_type>
            default_parser_t<std::variant<Ts...>>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    value_type               _l_rv;
    default_parser_t<size_t> _l_size_t_parser;
    result_t<size_t> _l_size_t_result{_l_size_t_parser.run_parser(_a_parse_input
    )};
    _a_parse_input.check_advance_and_throw(U" ");
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
__constexpr std::optional<std::u8string>
            default_parser_t<std::variant<Ts...>>::run_parser_internal(
        value_type&       _a_object,
        const std::size_t _a_idx,
        parser_input_t&   _a_parse_input
    ) const
{
    using namespace std;
    if (_a_idx == I)
    {
        if (auto _l_result{get<I>(_m_parsers)->run_parser(_a_parse_input)};
            _l_result.has_value())
        {
            std::get<I>(_a_object) = _l_result.value();
            return nullopt;
        }
        else
        {
            return optional<u8string>(fmt::format(
                u8"Could not parse std::variant element {0}. Failed with error "
                u8"message: {1}",
                I,
                _l_result.error()
            ));
        }
    }
    else if constexpr (I + 1 == tuple_size<std::tuple<Ts...>>{})
    {
        return optional<u8string>(fmt::format(
            u8"Could not parse std::variant element {0}. Tuple does not "
            u8"contain "
            u8"index",
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
    return std::filesystem::path(_a_parse_input.get_u32string());
}

_END_ABC_UTILITY_PARSER_NS