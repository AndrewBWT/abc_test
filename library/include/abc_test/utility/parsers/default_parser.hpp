#pragma once

#include "abc_test/utility/concepts.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/object_printer_parser.hpp"
#include "abc_test/utility/parsers/parser_base.hpp"
#include "abc_test/utility/str/string_utils.hpp"

#include <filesystem>
#include <fmt/xchar.h>
#include <functional>
#include <variant>

#include <random>

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
requires abc::utility::enum_has_list_c<T>
struct default_parser_t<T> : public parser_base_t<T>
{
    using value_type_t = T;
    default_parser_t(
        const enum_helper_string_type_e _a_enum_helper_string_case
        = enum_helper_string_type_e::unchanged
    )
        : _m_enum_helper_string_case(_a_enum_helper_string_case)
    {}

    enum_helper_string_type_e _m_enum_helper_string_case;

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
    requires enum_has_list_c<T>
__constexpr default_parser_t<T>
make_default_parser(enum_helper_string_type_e case_type) {
    return default_parser_t<T>(case_type);
}
template <typename T>
struct character_parser_t : public parser_base_t<T>
{
    __constexpr
    character_parser_t(
        const std::u32string _a_surrounding_str = std::u32string{U"'"}
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
        using namespace _ABC_NS_UTILITY_STR;
        if (_a_parse_input.check_and_advance(_m_surrounding_str))
        {
            return monostate{};
        }
        else
        {
            return unexpected(fmt::format(
                u8"Could not find expected surrounding string \"{0}\".",
                unicode_string_to_u8string(_m_surrounding_str)
            ));
        }
    }
private:
    std::u32string _m_surrounding_str;
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
    using namespace _ABC_NS_UTILITY_STR;
    const u32string _l_sv{_a_parse_input.get_u32string(_a_characters_to_take)};
    const result_t<string> _l_sv_as_str{convert_unicode_to_ascii(_l_sv)};
    if (_l_sv_as_str.has_value())
    {
        uint32_t _l_res;
        from_chars(
            _l_sv_as_str.value().data(),
            _l_sv_as_str.value().data() + _l_sv_as_str.value().size(),
            _l_res,
            16
        );
        return static_cast<T>(_l_res);
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
            if (_a_parse_input.check_and_advance(U"\\x"))
            {
                if (const result_t<T> _l_char_result{
                        detail::from_hex<T>(_a_parse_input, 2)
                    };
                    _l_char_result.has_value())
                {
                    _l_char = _l_char_result.value();
                    _a_parse_input.advance(2);
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
        using namespace _ABC_NS_UTILITY_STR;
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
                    _a_parse_input.advance(4);
                }
                else
                {
                    return unexpected(_l_char_result.error());
                }
            }
            else
            {
                auto _l_result{unicode_conversion<char16_t>(
                    _a_parse_input.process_characters(1)
                )};
                if (not _l_result.has_value())
                {
                    return unexpected(_l_result.error());
                }
                else
                {
                    auto& _l_result_val{_l_result.value()};
                    switch (_l_result_val.size())
                    {
                    case 0:
                        return unexpected(u8"");
                    case 1:
                        _l_char = _l_result_val.at(0);
                        break;
                    default:
                        std::unreachable();
                    }
                }
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
                        detail::from_hex<T>(_a_parse_input, 8)
                    };
                    _l_char_result.has_value())
                {
                    _l_char = _l_char_result.value();
                    _a_parse_input.advance(8);
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
    requires from_chars_convertable_c<T> && (not std::same_as<T, char>)
struct default_parser_t<T> : public parser_base_t<T>
{
    __constexpr result_t<T>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        T               result{};
        const u32string _l_u32str{
            _a_parse_input.take_string_containing(U"0123456789-.")
        };
        const result_t<string> _l_str_opt{convert_unicode_to_ascii(_l_u32str)};
        if (_l_str_opt.has_value())
        {
            auto& _l_str{_l_str_opt.value()};
            auto [ptr, ec] = from_chars(
                _l_str.data(), _l_str.data() + _l_str.size(), result
            );

            if (ec == std::errc())
            {
                return result_t<T>(result);
            }
            else if (ec == errc::invalid_argument)
            {
                return result_t<T>(unexpected(fmt::format(
                    u8"Could not parse \"{0}\" using std::from_chars, as the "
                    u8"std::string argument is invalid",
                    unicode_string_to_u8string(_l_u32str)
                )));
            }
            else if (ec == errc::result_out_of_range)
            {
                return result_t<T>(unexpected(fmt::format(
                    u8"Could not parse \"{0}\" using std::from_chars as, after "
                    u8"conversion, number is out of range. Min and max value "
                    u8"of "
                    u8"{1} are {2} and {3}",
                    unicode_string_to_u8string(_l_u32str),
                    type_id<T>(),
                    numeric_limits<T>::min(),
                    numeric_limits<T>::max()
                )

                ));
            }
            else
            {
                return result_t<T>(unexpected(fmt::format(
                    u8"Could not parse \"{0}\" using std::from_chars for some "
                    u8"unknown reason. The error code returned was unexpected. "
                    u8"Its "
                    u8"integer represention is {1}",
                    unicode_string_to_u8string(_l_u32str),
                    std::to_underlying(ec)
                )

                ));
            }
        }
        else
        {
            return unexpected(u8"unknown");
        }
    }
};

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_printer_with_custom_parsers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                std::tuple<parser_t<Ts>...>             _a_parsers,
                parser_input_t&                         _a_parse_input
            );

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                parser_input_t&                         _a_parse_input
            );

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names_and_custom_parsers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
                std::tuple<parser_t<Ts>...>                          _a_parsers,
                parser_input_t&                                      _a_parse_input
            );

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
                parser_input_t&                                      _a_parse_input
            );

namespace
{
template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_internal(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::optional<std::array<std::u8string_view, sizeof...(Ts)>>&
                                            _a_object_names,
                std::tuple<parser_t<Ts>...> _a_parsers,
                parser_input_t&             _a_parse_input
            );

template <std::size_t I, typename... Ts>
__constexpr_imp void
    run_object_reader(
        const utility::object_printer_parser_t& _a_object_print_parser,
        const std::optional<std::array<std::u8string_view, sizeof...(Ts)>>&
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
                const std::u8string_view&               _a_begin_str,
                std::tuple<parser_t<Ts>...>             _a_parsers,
                parser_input_t&                         _a_parse_input
            )
{
    using namespace std;
    return object_parser_internal<T>(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<u8string_view, sizeof...(Ts)>>{},
        _a_parsers,
        _a_parse_input
    );
}

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                parser_input_t&                         _a_parse_input
            )
{
    using namespace std;
    tuple<parser_t<Ts>...> _l_printers
        = std::make_tuple(default_parser<Ts>()...);
    return object_parser_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<u8string_view, sizeof...(Ts)>>{},
        _l_printers,
        _a_parse_input
    );
}

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names_and_custom_parsers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
                std::tuple<parser_t<Ts>...>                          _a_parsers,
                parser_input_t&                                      _a_parse_input
            )
{
    using namespace std;
    return object_parser_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<u8string_view, sizeof...(Ts)>>{_a_object_names},
        _a_parsers,
        _a_parse_input
    );
}

template <typename T, typename... Ts>
__constexpr result_t<T>
            object_parser_with_field_names(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
                parser_input_t&                                      _a_parse_input
            )
{
    using namespace std;
    tuple<parser_t<Ts>...> _l_parsers
        = std::make_tuple(default_parser<Ts>()...);
    return object_parser_internal<T>(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<u8string_view, sizeof...(Ts)>>{_a_object_names},
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
                const std::u8string_view&               _a_begin_str,
                const std::optional<std::array<std::u8string_view, sizeof...(Ts)>>&
                                            _a_object_names,
                std::tuple<parser_t<Ts>...> _a_parsers,
                parser_input_t&             _a_parse_input
            )
{
    using namespace std;
    using namespace abc::utility::str;
    using namespace _ABC_NS_UTILITY_STR;
    tuple<Ts...> default_tuple;
    _a_parse_input.check_advance_and_throw(
        unicode_conversion_with_exception<char32_t>(_a_begin_str)
    );
    if (_a_object_print_parser.space_after_object_name)
    {
        _a_parse_input.check_advance_and_throw(U' ');
    }
    _a_parse_input.check_advance_and_throw(
        unicode_conversion_with_exception<char32_t>(
            _a_object_print_parser.begin_char
        )
    );
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
        using namespace _ABC_NS_UTILITY_STR;
        using arg_type_t = basic_string<T>;
        char32_t   _l_char{U'"'};
        u32string  _l_str{};
        arg_type_t _l_rv_str;
        auto       _l_add_str_func = [&](const u32string_view _a_str)
        {
            if constexpr (same_as<arg_type_t, string>)
            {
                const result_t<arg_type_t> _l_rv{convert_unicode_to_ascii(_a_str
                )};
                if (_l_rv.has_value())
                {
                    _l_rv_str.append(_l_rv.value());
                }
                else
                {
                    // return unexpected(fmt::format(u8"Could not convert"));
                }
            }
            else
            {
                _l_rv_str.append(unicode_conversion<T>(_a_str).value());
            }
        };
        // Function for reading hex digits into a T value.
        auto read_hex_digits = [&]() -> result_t<T>
        {
            const size_t hex_size  = sizeof(T);
            T            _l_result = 0;
            string       _l_previously_processed_chars;
            for (size_t _l_idx{0}; _l_idx < hex_size; ++_l_idx)
            {
                const u32string _l_str{_a_parse_input.process_characters(2)};
                const result_t<string> _l_str_as_ascii_opt{
                    convert_unicode_to_ascii(_l_str)
                };
                if (_l_str_as_ascii_opt.has_value())
                {
                    const char* _l_str_as_ascii{
                        _l_str_as_ascii_opt.value().data()
                    };
                    uint8_t _l_char;
                    auto [_l_ptr, _l_ec] = from_chars(
                        _l_str_as_ascii, _l_str_as_ascii + 2, _l_char, 16
                    );
                    if (_l_ec == errc())
                    {
                        _l_result |= static_cast<T>(_l_char)
                                     << ((hex_size - _l_idx - 1) * 8);
                        _l_previously_processed_chars.append(
                            string_view(_l_str_as_ascii, _l_str_as_ascii + 2)
                        );
                    }
                    else
                    {
                        auto _l_prev_str_info{
                            (_l_idx == 0)
                                ? u8""
                                : fmt::format(
                                      u8" Previously the characters \"{0}\""
                                      u8" had been parsed correctly.",
                                      cast_string_to_u8string(
                                          _l_previously_processed_chars
                                      )
                                  )
                        };
                        // Else return failure
                        return unexpected{fmt::format(
                            u8"Failure occoured "
                            u8"when attempting to parse {0} characters "
                            u8"into hex, to represent the type {1}. "
                            u8"The failure occoured when parsing the {2} hex "
                            u8"pair \"{3}\".{4}",
                            hex_size * 2,
                            type_id<T>(),
                            positive_integer_to_placement(_l_idx + 1),
                            cast_string_to_u8string(string_view(
                                _l_str_as_ascii, _l_str_as_ascii + 2
                            )),
                            _l_prev_str_info
                        )};
                    }
                }
                else
                {
                    return std::unexpected(u8"whoah");
                }
            }
            return _l_result;
        };
        bool _l_finished{false};
        if (_a_parse_input.check_and_advance(_l_char))
        {
            do
            {
                const u32string _l_str2{
                    _a_parse_input.continue_until_char(U"\\\"")
                };
                _l_add_str_func(_l_str2);
                switch (_a_parse_input.peek_char())
                {
                case U'"':
                    _l_finished = true;
                    _a_parse_input.advance(1);
                    break;
                case U'\\':
                    switch (_a_parse_input.peek_char(1))
                    {
                    case U'x':
                        // Reading a hex stream now.
                        _a_parse_input.advance(2);
                        {
                            const result_t<T> _l_hex_char{read_hex_digits()};
                            if (_l_hex_char.has_value())
                            {
                                _l_rv_str.push_back(_l_hex_char.value());
                            }
                            else
                            {
                                return unexpected(_l_hex_char.error());
                            }
                        }
                        break;
                    case U'"':
                    case U'\\':
                    {
                        _a_parse_input.advance(1);
                        const u32string _l_str(
                            _a_parse_input.process_characters(1)
                        );
                        _l_add_str_func(_l_str);
                    }
                    break;
                    default:
                        return std::unexpected(u8"hello");
                        break;
                    }
                    break;
                }
            }
            while (not _l_finished);
        }

        else
        {
            return unexpected(fmt::format(
                u8"Expected {0} to have a prefix of the string \"{1}\". "
                u8"Remaining string = {2}",
                type_id<T>(),
                unicode_char_to_u8string(_l_char),
                _a_parse_input.get_u8string()
            ));
        }

        return _l_rv_str;
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

template <typename T, typename U>
struct default_parser_t<std::expected<T, U>>
    : public parser_base_t<std::expected<T, U>>
{
private:
    std::pair<parser_t<T>, parser_t<U>> _m_parsers;
public:
    using value_type = std::expected<T, U>;

    __constexpr
    default_parser_t(
        const parser_t<T>& _a_parser_t,
        const parser_t<U>& _a_parser_u
    );

    default_parser_t()
        // requires (std::is_default_constructible_v<default_parser_t<T>>&&
        // std::is_default_constructible_v<default_parser_t<U>>)
        : _m_parsers(std::make_pair(default_parser<T>(), default_parser<U>()))
    {}

    __constexpr virtual result_t<value_type>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using namespace std;
        value_type _l_rv;
        _a_parse_input.check_advance_and_throw(U"(");
        if (auto _l_first_result{
                default_parser_t<size_t>().run_parser(_a_parse_input)
            };
            _l_first_result.has_value())
        {
            _a_parse_input.check_advance_and_throw(U",");
            _a_parse_input.process_whitespace();
            switch (_l_first_result.value())
            {
            case 0:
                if (result_t<T> _l_result{
                        _m_parsers.first->run_parser(_a_parse_input)
                    };
                    _l_result.has_value())
                {
                    _l_rv = _l_result.value();
                }
                else
                {
                    return unexpected(fmt::format(
                        u8"Could not parse std::expected element 1. "
                        u8"Failed with error "
                        u8"message:"
                    ));
                }
                break;
            case 1:
                if (const result_t<U> _l_result{
                        _m_parsers.second->run_parser(_a_parse_input)
                    };
                    _l_result.has_value())
                {
                    _l_rv = unexpected(_l_result.value());
                }
                else
                {
                    return unexpected(fmt::format(
                        u8"Could not parse std::expected element 2. "
                        u8"Failed with error "
                        u8"message: \"{0}\".",
                        _l_result.error()
                    ));
                }
                break;
            }
            _a_parse_input.check_advance_and_throw(U")");
            return _l_rv;
        }
        else
        {
            return unexpected(fmt::format(
                u8"Could not parse std::expected element 1. Failed with "
                u8"error "
                u8"message: {0}",
                _l_first_result.error()
            ));
        }
    }
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
        _a_parse_input.check_advance_and_throw(U",");
        _a_parse_input.process_whitespace();
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
    _a_parse_input.check_advance_and_throw(U"[");
    if (_a_parse_input.check_and_advance(U"]"))
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
            else if (_a_parse_input.check_and_advance(U"]"))
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
        // requires (std::is_default_constructible_v<default_parser_t<Ts>>
        // &&
        // ...)
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
    variant_print_parse_e _m_enum_variant_print_parse
        = variant_print_parse_e::use_indexes;

    __constexpr
    default_parser_t(
        const variant_print_parse_e _a_enum_variant_print_parse
    )
        : _m_enum_variant_print_parse(_a_enum_variant_print_parse)
        , _m_parsers(std::make_tuple(mk_parser(default_parser_t<Ts>())...))
        
    {}

    __constexpr
    default_parser_t(
        const variant_print_parse_e _a_enum_variant_print_parse,
        parser_t<Ts>... _a_parsers
    )
        : _m_parsers(_a_parsers...)
        , _m_enum_variant_print_parse(_a_enum_variant_print_parse)
    {}

    __constexpr
    default_parser_t()
   // requires (std::is_default_constructible_v<default_parser_t<Ts>> && ...)
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
        // requires (std::is_default_constructible_v<default_parser_t<T>>)
        : _m_parser(mk_parser(default_parser_t<T>()))
    {}

    __constexpr virtual result_t<value_type>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using namespace std;
        _a_parse_input.check_advance_and_throw(U"{");
        if (_a_parse_input.check_and_advance(U"}"))
        {
            return result_t<value_type>(nullopt);
        }
        else
        {
            return _m_parser->run_parser(_a_parse_input)
                .transform(
                    [&](const T& _a_object)
                    {
                        _a_parse_input.check_advance_and_throw(U"}");
                        return make_optional(_a_object);
                    }
                );
        }
    }
private:
    parser_t<T> _m_parser;
};

template <typename T>
struct default_parser_t<std::shared_ptr<T>>
    : public parser_base_t<std::shared_ptr<T>>
{
    using value_type = std::shared_ptr<T>;
    enum_pointer_print_parse_type_t _m_enum
        = enum_pointer_print_parse_type_t::AS_OBJECT;

    __constexpr
    default_parser_t(
        parser_t<T>                           _a_parser,
        const enum_pointer_print_parse_type_t _a_enum
        = enum_pointer_print_parse_type_t::AS_OBJECT
    )
        : _m_enum(_a_enum)
        , _m_parser(_a_parser)
    {}

    __constexpr
    default_parser_t()
        // requires (std::is_default_constructible_v<default_parser_t<T>>)
        : _m_parser(mk_parser(default_parser_t<T>()))
    {}

    __constexpr virtual result_t<value_type>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        using enum enum_pointer_print_parse_type_t;
        switch (_m_enum)
        {
        case JUST_DATA:
            return _m_parser->run_parser(_a_parse_input)
                .transform(
                    [](const T& _l_value)
                    {
                        return std::make_shared<T>(_l_value);
                    }
                );
        default:
            throw errors::unaccounted_for_enum_exception(_m_enum);
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

template <>
struct default_parser_t<std::monostate> : public parser_base_t<std::monostate>
{
    __constexpr virtual result_t<std::monostate>
        run_parser(parser_input_t& _a_parse_input) const
    {
        if (_a_parse_input.check_and_advance(u8"std::monostate"))
        {
            return std::monostate{};
        }
        else
        {
            return std::unexpected(u8"");
        }
    }
};

template <>
struct default_parser_t<std::mt19937_64> : public parser_base_t<std::mt19937_64>
{
    __constexpr virtual result_t<std::mt19937_64>
        run_parser(parser_input_t& _a_parse_input) const
    {
        if (_a_parse_input.check_and_advance(u8"std::mt19937_64"))
        {
            return std::mt19937_64{};
        }
        else
        {
            return std::unexpected(u8"");
        }
    }
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
            u8"message: \"{1}\".",
            I,
            _l_result.error()
        ));
    }
}

template <typename... Ts>
__constexpr result_t<typename default_parser_t<std::variant<Ts...>>::value_type>
            default_parser_t<std::variant<Ts...>>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    using enum variant_print_parse_e;
    switch (_m_enum_variant_print_parse)
    {
    case no_indexes:
    {
        value_type               _l_rv;
        const optional<u8string> _l_res{
            run_parser_internal<0>(_l_rv, _a_parse_input)
        };
        return (_l_res.has_value()) ? unexpected(_l_res.value())
                                    : result_t<value_type>(_l_rv);
    }
    case use_indexes:
    {
        _a_parse_input.check_advance_and_throw(U"(");
        return default_parser_t<size_t>{}
            .run_parser(_a_parse_input)
            .and_then(
                [&](const size_t _a_size)
                {
                    _a_parse_input.check_advance_and_throw(U",");
                    _a_parse_input.process_whitespace();
                    value_type               _l_rv;
                    const optional<u8string> _l_res{
                        run_parser_internal<0>(_l_rv, _a_size, _a_parse_input)
                    };
                    _a_parse_input.check_advance_and_throw(U")");
                    return (_l_res.has_value()) ? unexpected(_l_res.value())
                                                : result_t<value_type>(_l_rv);
                }
            );
    }
    default:
        throw errors::unaccounted_for_enum_exception(_m_enum_variant_print_parse
        );
    }
}

template <typename... Ts>
template <std::size_t I>
__constexpr std::optional<std::u8string>
            default_parser_t<std::variant<Ts...>>::run_parser_internal(
        value_type&     _a_object,
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    if constexpr (I + 1 == tuple_size<std::tuple<Ts...>>{})
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
        if (auto _l_result{get<I>(_m_parsers)->run_parser(_a_parse_input)};
            _l_result.has_value())
        {
            _a_object = value_type(in_place_index<I>, _l_result.value());
            return nullopt;
        }
        else
        {
            return run_parser_internal<I + 1>(_a_object, _a_parse_input);
        }
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
            _a_object = value_type(in_place_index<I>, _l_result.value());
            return nullopt;
        }
        else
        {
            return optional<u8string>(fmt::format(
                u8"Could not parse std::variant element {0}. Failed with "
                u8"error "
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

template <typename T>
struct hex_parser_t : public parser_base_t<T>
{
    parser_t<T> _m_parser;

    __constexpr
    hex_parser_t(
        const parser_t<T>& _a_parser
    )
        : _m_parser(_a_parser)
    {}

    __no_constexpr_imp result_t<T>
                       run_parser(
                           parser_input_t& _a_parse_input
                       ) const
    {
        using namespace std;
        using namespace abc::utility::str;
        const u8string _l_str{_a_parse_input.get_u8string()};
        _a_parse_input.advance_to_end();
        result_t<parser_input_t> _l_intermediary{from_hex(_l_str).transform(
            [](const u8string& _a_str)
            {
                return parser_input_t(_a_str);
            }
        )};
        return _l_intermediary.and_then(
            [&](parser_input_t& _l_arg) -> result_t<T>
            {
                return _m_parser->run_parser(_l_arg);
            }
        );
    }
};

_END_ABC_UTILITY_PARSER_NS