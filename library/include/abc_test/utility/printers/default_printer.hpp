#pragma once

#include "abc_test/utility/concepts.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/object_printer_parser.hpp"
#include "abc_test/utility/printers/printer_base.hpp"
#include "abc_test/utility/str/string_utility.hpp"

#include <deque>
#include <filesystem>
#include <random>
#include <set>
#include <unordered_set>

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct default_printer_t : public printer_base_t<T>
{
public:
    static constexpr bool is_specialized{false};
    __constexpr virtual std::u8string
        run_printer(const T& _a_object) const override;
};

template <typename T, typename... Ts>
__constexpr printer_t<T>
            default_printer(
                Ts... elements
            )
{
    return mk_printer(default_printer_t<T>(elements...));
}

template <typename T>
concept default_printable = (default_printer_t<T>::is_specialized == true);
_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS
template <typename T>
__constexpr std::u8string
            default_printer_t<T>::run_printer(
        const T& _a_object
    ) const
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "run_printer<T>()."
    );
}

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<bool> : public printer_base_t<bool>
{
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const bool& _a_object
                          ) const
    {
        return _a_object ? u8"true" : u8"false";
    }
};

template <typename T>
struct character_printer_t : public printer_base_t<T>
{
    __constexpr
    character_printer_t(
        const std::u8string _a_surrounding_str = std::u8string{}
    )
        : _m_surrounding_str(_a_surrounding_str)
    {}
protected:
    __constexpr std::u8string
                surround_str(
                    const std::u8string_view _a_str
                ) const noexcept
    {
        return fmt::format(u8"{0}{1}{0}", _m_surrounding_str, _a_str);
    }

    template <typename Type_To_Cast_To>
    __constexpr std::u8string
                make_hex_from_char(
                    const T _a_char
                ) const noexcept
    {
        Type_To_Cast_To _l_char_as_uint{static_cast<Type_To_Cast_To>(_a_char)};
        return fmt::format(u8"\\x{:x}", _l_char_as_uint);
    }
private:
    std::u8string _m_surrounding_str;
    bool          _m_as_hex;
};

template <>
struct default_printer_t<char> : public character_printer_t<char>
{
public:
    using character_printer_t<char>::character_printer_t;
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const char& _a_object
                          ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        return surround_str(
            is_valid_ascii_char(_a_object)
                ? fmt::format(u8"{0}", u8string(1, _a_object))
                : make_hex_from_char<uint8_t>(_a_object)
        );
    }
private:
    std::optional<char> _m_surrounding_char;
};

template <>
struct default_printer_t<char8_t> : public character_printer_t<char8_t>
{
public:
    using character_printer_t<char8_t>::character_printer_t;
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const char8_t& _a_object
                          ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        return surround_str(
            is_valid_char(_a_object)
                ? fmt::format(u8"{0}", u8string(1, _a_object))
                : make_hex_from_char<uint8_t>(_a_object)
        );
    }
};

template <>
struct default_printer_t<char16_t> : public character_printer_t<char16_t>
{
public:
    using character_printer_t<char16_t>::character_printer_t;
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const char16_t& _a_object
                          ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        return surround_str(
            is_valid_char(_a_object)
                ? fmt::format(u8"{0}", u8string(1, _a_object))
                : make_hex_from_char<uint8_t>(_a_object)
        );
    }
};

template <>
struct default_printer_t<char32_t> : public character_printer_t<char32_t>
{
public:
    using character_printer_t<char32_t>::character_printer_t;
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const char32_t& _a_object
                          ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        return surround_str(
            is_valid_char(_a_object)
                ? fmt::format(u8"{0}", u8string(1, _a_object))
                : make_hex_from_char<uint8_t>(_a_object)
        );
    }
};

struct range_based_printer_opts_t
{
    std::u8string begin_str = u8"[";
    std::u8string end_str   = u8"]";
    std::u8string delimiter = u8",";
};

template <typename T>
__constexpr range_based_printer_opts_t
    default_range_based_printer_opts()
{
    return range_based_printer_opts_t();
}

template <typename T>
requires std::ranges::range<T>
struct range_based_printer_t : public printer_base_t<T>
{
private:
    using value_t = std::ranges::range_value_t<T>;
public:
    __constexpr
    range_based_printer_t(
        const printer_t<value_t>& _a_inner_printer = default_printer<value_t>(),
        const range_based_printer_opts_t& _a_opts
        = default_range_based_printer_opts<T>()
    ) noexcept
        : _m_inner_printer(_a_inner_printer), _m_opts(_a_opts)
    {}

    __constexpr std::u8string
                run_printer(
                    const T& _a_object
                ) const
    {
        using namespace std;
        u8string                   _l_rv{_m_opts.begin_str};
        typename T::const_iterator _l_itt{std::begin(_a_object)};
        typename T::const_iterator _l_end{std::end(_a_object)};
        for (; _l_itt != _l_end; ++_l_itt)
        {
            const value_t& _l_element{*_l_itt};
            _l_rv.append(_m_inner_printer->run_printer(_l_element));
            if ((_l_itt + 1) != _l_end)
            {
                _l_rv.append(_m_opts.delimiter);
            }
        }
        _l_rv.append(_m_opts.end_str);
        return _l_rv;
    }
private:
    range_based_printer_opts_t _m_opts;
    printer_t<value_t>         _m_inner_printer;
};

//

/*template <>
struct default_printer_t<wchar_t> : public printer_base_t<wchar_t>
{
    static constexpr bool is_specialized{true};

    __no_constexpr_imp    std::string
                          run_printer(
                              const wchar_t& _a_object
                          ) const
    {
        __STATIC_ASSERT(
            wchar_t, "wchar_t not supported by abc::utility::run_printer"
        );
    }
};*/

template <typename T>
requires enum_has_list_c<T>
struct default_printer_t<T> : public printer_base_t<T>
{
    static constexpr bool is_specialized{true};

    default_printer_t<T>(
        const enum_helper_string_type_e _a_enum_helper_string_case
        = enum_helper_string_type_e::lower
    )
        : _m_enum_helper_string_case(_a_enum_helper_string_case)
    {}

    enum_helper_string_type_e _m_enum_helper_string_case;

    __constexpr               std::u8string
                              run_printer(
                                  const T& _a_element
                              ) const
    {
        return _ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
            .print(_a_element, _m_enum_helper_string_case);
    }
};

template<typename T>
    requires to_chars_convertable_c<T> && (not std::same_as<T, char>)
struct default_printer_t<T> : public printer_base_t<T>
{
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const T& _a_object
                          ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        char* _l_holder = new char[1'000](0);
        to_chars(_l_holder, _l_holder + 1'000, _a_object);
        string _l_rv(_l_holder);
        delete[] _l_holder;
        return cast_string_to_u8string(_l_rv);
    }
};

template <typename... Ts>
__constexpr std::u8string
            object_printer_with_custom_printers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                std::tuple<printer_t<Ts>...>            _a_parsers,
                Ts... _a_elements_to_print
            );
template <typename... Ts>
__constexpr std::u8string
            object_printer(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                Ts... _a_elements_to_print
            );
template <typename... Ts>
__constexpr std::u8string
            object_printer_with_field_names_and_custom_printers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
                std::tuple<printer_t<Ts>...>                         _a_parsers,
                Ts... _a_elements_to_print
            );
template <typename... Ts>
__constexpr std::u8string
            object_printer_with_field_names(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
                Ts... _a_elements_to_print
            );

namespace
{
template <typename... Ts>
__constexpr std::u8string
            object_printer_internal(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::optional<std::array<std::u8string_view, sizeof...(Ts)>>&
                                             _a_object_names,
                std::tuple<printer_t<Ts>...> _a_parsers,
                Ts... _a_elements_to_print
            );
template <std::size_t I, typename... Ts>
__constexpr void
    object_printer_internal(
        const utility::object_printer_parser_t& _a_object_print_parser,
        std::u8string&                          _a_str,
        const std::optional<std::array<std::u8string_view, sizeof...(Ts)>>&
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
        return expsectsed<T, string>(result);
    }
    else
    {
        return parse_error<T>(
            fmt::format("Could not parse \"{0}\" using std::from_chars", _l_str)

        );
    }
}*/
template <typename... Ts>
__constexpr std::u8string
            object_printer_with_custom_printers(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                std::tuple<printer_t<Ts>...>            _a_printers,
                Ts... _a_elements_to_print
            )
{
    using namespace std;
    return object_printer_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<u8string_view, sizeof...(Ts)>>{},
        _a_printers,
        _a_elements_to_print...
    );
}

template <typename... Ts>
__constexpr std::u8string
            object_printer(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                Ts... _a_elements_to_print
            )
{
    using namespace std;
    tuple<printer_t<Ts>...> _l_printers
        = std::make_tuple(default_printer<Ts>()...);
    return object_printer_internal(
        _a_object_print_parser,
        _a_begin_str,
        optional<array<u8string_view, sizeof...(Ts)>>{},
        _l_printers,
        _a_elements_to_print...
    );
}

template <typename... Ts>
__constexpr_imp std::u8string
                object_printer_with_field_names_and_custom_printers(
                    const utility::object_printer_parser_t& _a_object_print_parser,
                    const std::u8string_view&               _a_begin_str,
                    const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
                    std::tuple<printer_t<Ts>...>                         _a_parsers,
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
__constexpr_imp std::u8string
                object_printer_with_field_names(
                    const utility::object_printer_parser_t& _a_object_print_parser,
                    const std::u8string_view&               _a_begin_str,
                    const std::array<std::u8string_view, sizeof...(Ts)>& _a_object_names,
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
__constexpr std::u8string
            object_printer_internal(
                const utility::object_printer_parser_t& _a_object_print_parser,
                const std::u8string_view&               _a_begin_str,
                const std::optional<std::array<std::u8string_view, sizeof...(Ts)>>&
                                             _a_object_names,
                std::tuple<printer_t<Ts>...> _a_parsers,
                Ts... _a_elements_to_print
            )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    u8string _l_rv{_a_begin_str};
    if (_a_object_print_parser.space_after_object_name)
    {
        _l_rv.append(u8" ");
    }
    _l_rv.append(
        cast_string_to_u8string(string(1, _a_object_print_parser.begin_char))
    );
    tuple<Ts...> _l_tuple{tie(_a_elements_to_print...)};
    object_printer_internal<0>(
        _a_object_print_parser, _l_rv, _a_object_names, _a_parsers, _l_tuple
    );
    _l_rv.append(
        cast_string_to_u8string(string(1, _a_object_print_parser.end_char))
    );
    return _l_rv;
}

template <std::size_t I, typename... Ts>
__constexpr void
    object_printer_internal(
        const utility::object_printer_parser_t& _a_object_print_parser,
        std::u8string&                          _a_str,
        const std::optional<std::array<std::u8string_view, sizeof...(Ts)>>&
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
    using namespace _ABC_NS_UTILITY_STR;
    if (_a_object_names.has_value())
    {
        _a_str.append(get<I>(_a_object_names.value()));
        if (_a_object_print_parser.space_before_field_name_and_field_separator)
        {
            _a_str.append(cast_string_to_u8string(string(1, ' ')));
        }
        _a_str.append(cast_string_to_u8string(string(
            1, _a_object_print_parser.delimiter_between_field_name_and_field
        )));
        if (_a_object_print_parser.space_after_field_name_and_field_separator)
        {
            _a_str.append(cast_string_to_u8string(string(1, ' ')));
        }
    }
    _a_str.append(
        std::get<I>(_a_parsers)->run_printer(std::get<I>(_a_elements_to_print))
    );
    if constexpr (I + 1 < sizeof...(Ts))
    {
        if (_a_object_print_parser.space_before_field_delimiter)
        {
            _a_str.append(cast_string_to_u8string(string(1, ' ')));
        }
        _a_str.append(cast_string_to_u8string(
            string(1, _a_object_print_parser.delimiter_between_fields)
        ));
        if (_a_object_print_parser.space_after_field_delimieter)
        {
            _a_str.append(cast_string_to_u8string(string(1, ' ')));
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

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename... Ts>
struct default_printer_t<std::tuple<Ts...>>
    : public printer_base_t<std::tuple<Ts...>>
{
    using value_type = std::tuple<Ts...>;
    static constexpr bool is_specialized{true};
    __constexpr
    default_printer_t(printer_t<Ts>... _a_printers);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<Ts>> &&
        // ...)
        : _m_printers(std::make_tuple(mk_printer(default_printer_t<Ts>())...))
    {}

    __constexpr virtual std::u8string
                run_printer(const value_type& _a_parse_input) const;

    __constexpr std::tuple<printer_t<Ts>...>&
                get_printers_ref() noexcept
    {
        return _m_printers;
    }
private:
    std::tuple<printer_t<Ts>...> _m_printers;
    template <std::size_t I>
    __constexpr void
        run_internal_printer(std::u8string& _a_str, const value_type& _a_object)
            const;
};

template <typename... Ts>
struct default_printer_t<std::variant<Ts...>>
    : public printer_base_t<std::variant<Ts...>>
{
    using value_type = std::variant<Ts...>;
    variant_print_parse_e _m_enum_variant_print_parse
        = variant_print_parse_e::use_indexes;
    static constexpr bool is_specialized{true};

    __constexpr
    default_printer_t(
        const variant_print_parse_e _a_enum_variant_print_parse,
        printer_t<Ts>... _a_printers
    )
        : _m_printers(_a_printers)
        , _m_enum_variant_print_parse(_a_enum_variant_print_parse)
    {}

    __constexpr
    default_printer_t(
        const variant_print_parse_e _a_enum_variant_print_parse
    )
        : _m_printers(std::make_tuple(mk_printer(default_printer_t<Ts>())...))
        , _m_enum_variant_print_parse(_a_enum_variant_print_parse)
    {}

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<Ts>> &&
        // ...)
        : _m_printers(std::make_tuple(mk_printer(default_printer_t<Ts>())...))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        using enum variant_print_parse_e;
        const size_t   _l_idx{_a_object.index()};
        const u8string _l_element_str{run_internal_printer<0>(_a_object, _l_idx)
        };
        switch (_m_enum_variant_print_parse)
        {
        case no_indexes:
            return _l_element_str;
        case use_indexes:
            return fmt::format(u8"({0}, {1})", _l_idx, _l_element_str);
        default:
            throw errors::unaccounted_for_enum_exception(
                _m_enum_variant_print_parse
            );
        }
    }

    __constexpr std::tuple<printer_t<Ts>...>&
                get_printers_ref() noexcept
    {
        return _m_printers;
    }
private:
    std::tuple<printer_t<Ts>...> _m_printers;

    template <std::size_t I>
    __constexpr std::u8string
                run_internal_printer(
                    const value_type& _a_object,
                    const std::size_t _a_idx
                ) const
    {
        using namespace std;
        if (I == _a_idx)
        {
            return get<I>(_m_printers)->run_printer(get<I>(_a_object));
        }
        else if constexpr (I + 1 < tuple_size<decltype(_m_printers)>{})
        {
            return run_internal_printer<I + 1>(_a_object, _a_idx);
        }
        else
        {
            throw std::runtime_error("Couldn't work it out");
        }
    }
};

template <>
struct default_printer_t<std::filesystem::path>
    : public printer_base_t<std::filesystem::path>
{
    using value_type = std::filesystem::path;

    __no_constexpr_imp virtual std::u8string
        run_printer(
            const value_type& _a_parse_input
        ) const
    {
        return _a_parse_input.u8string();
    }
};

template <typename T>
struct default_printer_t<std::optional<T>>
    : public printer_base_t<std::optional<T>>
{
    using value_type = std::optional<T>;
    static constexpr bool is_specialized{true};

    __constexpr
    default_printer_t(
        printer_t<T> _a_printer
    )
        : _m_printer(_a_printer)
    {}

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_rv;
        _l_rv.append(u8"{");
        if (_a_object.has_value())
        {
            _l_rv.append(_m_printer->run_printer(_a_object.value()));
        }
        _l_rv.append(u8"}");
        return _l_rv;
    }
private:
    printer_t<T> _m_printer;
};

template <>
struct default_printer_t<std::monostate> : public printer_base_t<std::monostate>
{
    using value_type = std::monostate;
    static constexpr bool is_specialized{true};

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_rv;
        _l_rv.append(u8"std::monostate");
        return _l_rv;
    }
};

template <>
struct default_printer_t<std::mt19937_64>
    : public printer_base_t<std::mt19937_64>
{
    using value_type = std::mt19937_64;
    static constexpr bool is_specialized{true};

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_rv;
        _l_rv.append(u8"std::mt19937_64");
        return _l_rv;
    }
};

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS template <typename... Ts>
__constexpr
default_printer_t<std::tuple<Ts...>>::default_printer_t(
    printer_t<Ts>... _a_printers
)
    : _m_printers{std::move(_a_printers)...}

{}

template <typename... Ts>
__constexpr std::u8string
            default_printer_t<std::tuple<Ts...>>::run_printer(
        const default_printer_t<std::tuple<Ts...>>::value_type& _a_object
    ) const
{
    using namespace std;
    u8string _l_str{u8"("};
    run_internal_printer<0>(_l_str, _a_object);
    _l_str.append(u8")");
    return _l_str;
}

template <typename... Ts>
template <std::size_t I>
__constexpr_imp void
    default_printer_t<std::tuple<Ts...>>::run_internal_printer(
        std::u8string&                                          _a_str,
        const default_printer_t<std::tuple<Ts...>>::value_type& _a_object
    ) const
{
    using namespace std;
    using U = tuple_element<I, value_type>::type;
    _a_str.append(get<I>(_m_printers)->run_printer(get<I>(_a_object)));
    if constexpr (I + 1 < tuple_size<value_type>{})
    {
        _a_str.append(u8",");
        run_internal_printer<I + 1>(_a_str, _a_object);
    }
}

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
requires (std::same_as<std::basic_string<T>, std::string> || std::same_as<std::basic_string<T>, std::wstring> || std::same_as<std::basic_string<T>, std::u8string> || std::same_as<std::basic_string<T>, std::u16string> || std::same_as<std::basic_string<T>, std::u32string>)
struct default_printer_t<std::basic_string<T>>
    : public printer_base_t<std::basic_string<T>>
{
    static constexpr bool is_specialized{true};

    __constexpr
    default_printer_t()
        : default_printer_t(u8"\"")
    {}

    __constexpr
    default_printer_t(
        const std::u8string_view _a_surrounding_str
    )
        : _m_surrounding_str(_a_surrounding_str)
    {}

    __constexpr std::u8string
                run_printer(
                    const std::basic_string<T>& _a_object
                ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        using arg_type_t = basic_string<T>;
        if constexpr (same_as<arg_type_t, string>)
        {
            u8string                _l_rv{_m_surrounding_str};
            default_printer_t<char> _l_printer;
            for (const char _l_char : _a_object)
            {
                _l_rv.append(_l_printer.run_printer(_l_char));
            }
            _l_rv.append(_m_surrounding_str);
            return _l_rv;
        }
        else if constexpr (same_as<arg_type_t, u8string>
                           || same_as<arg_type_t, u16string>
                           || same_as<arg_type_t, u32string>
                           || same_as<arg_type_t, wstring>)
        {
            return fmt::format(
                u8"{0}{1}{0}",
                _m_surrounding_str,
                unicode_string_to_u8string(_a_object)
            );
        }
        else
        {
            __STATIC_ASSERT(
                T,
                "default_printer_t for basic_string type only allows certain "
                "types"
            );
        }
    }
private:
    std::u8string _m_surrounding_str;
};

template <typename T>
requires (std::same_as<std::basic_string_view<T>, std::string_view> || std::same_as<std::basic_string_view<T>, std::wstring_view> || std::same_as<std::basic_string_view<T>, std::u8string_view> || std::same_as<std::basic_string_view<T>, std::u16string_view> || std::same_as<std::basic_string_view<T>, std::u32string_view>)
struct default_printer_t<std::basic_string_view<T>>
    : public printer_base_t<std::basic_string_view<T>>
{
    static constexpr bool is_specialized{true};

    __constexpr
    default_printer_t()
        : default_printer_t(u8"\"")
    {}

    __constexpr
    default_printer_t(
        const std::u8string_view _a_surrounding_str
    )
        : _m_surrounding_str(_a_surrounding_str)
    {}

    __constexpr std::u8string
                run_printer(
                    const std::basic_string_view<T>& _a_object
                ) const
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        using arg_type_t = basic_string_view<T>;
        if constexpr (same_as<arg_type_t, string_view>)
        {
            u8string                _l_rv{_m_surrounding_str};
            default_printer_t<char> _l_printer;
            for (const char _l_char : _a_object)
            {
                _l_rv.append(_l_printer.run_printer(_l_char));
            }
            _l_rv.append(_m_surrounding_str);
            return _l_rv;
        }
        else if constexpr (same_as<arg_type_t, u8string_view>)
        {
            return fmt::format(u8"{0}{1}{0}", _m_surrounding_str, _a_object);
        }
        else if constexpr (same_as<arg_type_t, u16string_view>)
        {
            return fmt::format(
                u8"{0}{1}{0}",
                _m_surrounding_str,
                unicode_string_to_u8string(_a_object)
            );
        }
        else if constexpr (same_as<arg_type_t, u32string_view>)
        {
            return fmt::format(
                u8"{0}{1}{0}",
                _m_surrounding_str,
                unicode_string_to_u8string(_a_object)
            );
        }
        else if constexpr (same_as<arg_type_t, wstring_view>)
        {
            return fmt::format(
                u8"{0}{1}{0}",
                _m_surrounding_str,
                unicode_string_to_u8string(
                    cast_wstring_to_unicode_string(_a_object)
                )
            );
        }
        else
        {
            __STATIC_ASSERT(
                T,
                "default_printer_t for basic_string type only allows certain "
                "types"
            );
        }
    }
private:
    std::u8string _m_surrounding_str;
};

template <typename T>
requires (not (std::same_as<std::basic_string<T>, std::string> || std::same_as<std::basic_string<T>, std::wstring> || std::same_as<std::basic_string<T>, std::u8string> || std::same_as<std::basic_string<T>, std::u16string> || std::same_as<std::basic_string<T>, std::u32string>)
)
struct default_printer_t<std::basic_string<T>>
    : public range_based_printer_t<std::basic_string<T>>
{
    using range_based_printer_t<std::basic_string<T>>::range_based_printer_t;
    static constexpr bool is_specialized{true};
};

template <typename T, typename U>
struct default_printer_t<std::pair<T, U>>
    : public printer_base_t<std::pair<T, U>>
{
private:
    std::pair<printer_t<T>, printer_t<U>> _m_printers;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::pair<T, U>;

    __constexpr
    default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    );

    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>> &&
        // std::is_default_constructible_v<default_printer_t<U>>)
        : _m_printers(std::make_pair(default_printer<T>(), default_printer<U>())
          )
    {}

    __constexpr virtual std::u8string
        run_printer(const value_type& _a_object) const;
};

template <typename T, typename U>
struct default_printer_t<std::expected<T, U>>
    : public printer_base_t<std::expected<T, U>>
{
private:
    std::pair<printer_t<T>, printer_t<U>> _m_printers;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::expected<T, U>;
    variant_print_parse_e _m_enum_variant_print_parse;
    __constexpr
    default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    );

    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>> &&
        // std::is_default_constructible_v<default_printer_t<U>>)
        : _m_printers(std::make_pair(default_printer<T>(), default_printer<U>())
          )
        , _m_enum_variant_print_parse(variant_print_parse_e::use_indexes)
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        using enum variant_print_parse_e;
        const size_t _l_idx{_a_object.has_value() ? size_t(0) : size_t(1)};
        u8string     _l_element_str;
        switch (_l_idx)
        {
        case 0:
            _l_element_str = _m_printers.first->run_printer(_a_object.value());
            break;
        case 1:
            _l_element_str = _m_printers.second->run_printer(_a_object.error());
            break;
        }
        switch (_m_enum_variant_print_parse)
        {
        case no_indexes:
            return _l_element_str;
        case use_indexes:
            return fmt::format(u8"({0}, {1})", _l_idx, _l_element_str);
        default:
            throw errors::unaccounted_for_enum_exception(
                _m_enum_variant_print_parse
            );
        }
    }
};

template <typename T, typename U>
struct default_printer_t<std::unordered_multimap<T, U>>
    : public printer_base_t<std::unordered_multimap<T, U>>
{
private:
    printer_t<T> _m_key_printer;
    printer_t<U> _m_element_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::unordered_multimap<T, U>;

    __constexpr
    default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    );

    default_printer_t()
        : _m_key_printer(default_printer<T>())
        , _m_element_printer(default_printer<U>())
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"{"};
        for (const pair<T, U>& _l_element : _a_object)
        {
            _l_str.append(_m_key_printer->run_printer(_l_element.first));
            _l_str.append(u8":");
            _l_str.append(_m_element_printer->run_printer(_l_element.second));
        }
        _l_str.append(u8"}");
        return _l_str;
    }
};

template <typename T, typename U>
struct default_printer_t<std::multimap<T, U>>
    : public printer_base_t<std::multimap<T, U>>
{
private:
    printer_t<T> _m_key_printer;
    printer_t<U> _m_element_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::multimap<T, U>;

    __constexpr
    default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    );

    default_printer_t()
        : _m_key_printer(default_printer<T>())
        , _m_element_printer(default_printer<U>())
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"{"};
        for (const pair<T, U>& _l_element : _a_object)
        {
            _l_str.append(_m_key_printer->run_printer(_l_element.first));
            _l_str.append(u8":");
            _l_str.append(_m_element_printer->run_printer(_l_element.second));
        }
        _l_str.append(u8"}");
        return _l_str;
    }
};

template <typename T, typename U>
struct default_printer_t<std::unordered_map<T, U>>
    : public printer_base_t<std::unordered_map<T, U>>
{
private:
    printer_t<T> _m_key_printer;
    printer_t<U> _m_element_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::unordered_map<T, U>;

    __constexpr
    default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    );

    default_printer_t()
        : _m_key_printer(default_printer<T>())
        , _m_element_printer(default_printer<U>())
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"{"};
        for (const pair<T, U>& _l_element : _a_object)
        {
            _l_str.append(_m_key_printer->run_printer(_l_element.first));
            _l_str.append(u8":");
            _l_str.append(_m_element_printer->run_printer(_l_element.second));
        }
        _l_str.append(u8"}");
        return _l_str;
    }
};

template <typename T, typename U>
struct default_printer_t<std::map<T, U>> : public printer_base_t<std::map<T, U>>
{
private:
    printer_t<T> _m_key_printer;
    printer_t<U> _m_element_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::map<T, U>;

    __constexpr
    default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    );

    default_printer_t()
    requires (std::is_default_constructible_v<default_printer_t<T>>
              && std::is_default_constructible_v<default_printer_t<U>>)
        : _m_key_printer(default_printer<T>())
        , _m_element_printer(default_printer<U>())
    {}

    __constexpr virtual std::u8string
        run_printer(const value_type& _a_object) const;
};

template <typename T>
struct default_printer_t<std::vector<T>> : public printer_base_t<std::vector<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::vector<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(const value_type& _a_object) const;
};

template <typename T>
struct default_printer_t<std::forward_list<T>>
    : public printer_base_t<std::forward_list<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::forward_list<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"["};
        using Itt = typename value_type::const_iterator;
        const Itt _l_end{end(_a_object)};

        for (Itt _l_itt{begin(_a_object)}; _l_itt != _l_end;)
        {
            _l_str.append(_m_printer->run_printer(*_l_itt));
            ++_l_itt;
            if (_l_itt != _l_end)
            {
                _l_str.append(u8",");
            }
        }
        _l_str.append(u8"]");
        return _l_str;
    }
};

template <typename T>
struct default_printer_t<std::deque<T>> : public printer_base_t<std::deque<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::deque<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"["};
        using Itt = typename value_type::const_iterator;
        const Itt _l_end{end(_a_object)};

        for (Itt _l_itt{begin(_a_object)}; _l_itt != _l_end;)
        {
            _l_str.append(_m_printer->run_printer(*_l_itt));
            ++_l_itt;
            if (_l_itt != _l_end)
            {
                _l_str.append(u8",");
            }
        }
        _l_str.append(u8"]");
        return _l_str;
    }
};

template <typename T>
struct default_printer_t<std::list<T>> : public printer_base_t<std::list<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::list<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"["};
        using Itt = typename value_type::const_iterator;
        const Itt _l_end{end(_a_object)};

        for (Itt _l_itt{begin(_a_object)}; _l_itt != _l_end;)
        {
            _l_str.append(_m_printer->run_printer(*_l_itt));
            ++_l_itt;
            if (_l_itt != _l_end)
            {
                _l_str.append(u8",");
            }
        }
        _l_str.append(u8"]");
        return _l_str;
    }
};

template <typename T>
struct default_printer_t<std::unordered_multiset<T>>
    : public printer_base_t<std::unordered_multiset<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::unordered_multiset<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"["};
        using Itt = typename value_type::const_iterator;
        const Itt _l_end{end(_a_object)};

        for (Itt _l_itt{begin(_a_object)}; _l_itt != _l_end;)
        {
            _l_str.append(_m_printer->run_printer(*_l_itt));
            ++_l_itt;
            if (_l_itt != _l_end)
            {
                _l_str.append(u8",");
            }
        }
        _l_str.append(u8"]");
        return _l_str;
    }
};

template <typename T>
struct default_printer_t<std::set<T>> : public printer_base_t<std::set<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::set<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"["};
        using Itt = typename value_type::const_iterator;
        const Itt _l_end{end(_a_object)};

        for (Itt _l_itt{begin(_a_object)}; _l_itt != _l_end;)
        {
            _l_str.append(_m_printer->run_printer(*_l_itt));
            ++_l_itt;
            if (_l_itt != _l_end)
            {
                _l_str.append(u8",");
            }
        }
        _l_str.append(u8"]");
        return _l_str;
    }
};

template <typename T>
struct default_printer_t<std::multiset<T>>
    : public printer_base_t<std::multiset<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::multiset<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"["};
        using Itt = typename value_type::const_iterator;
        const Itt _l_end{end(_a_object)};

        for (Itt _l_itt{begin(_a_object)}; _l_itt != _l_end;)
        {
            _l_str.append(_m_printer->run_printer(*_l_itt));
            ++_l_itt;
            if (_l_itt != _l_end)
            {
                _l_str.append(u8",");
            }
        }
        _l_str.append(u8"]");
        return _l_str;
    }
};

template <typename T>
struct default_printer_t<std::unordered_set<T>>
    : public printer_base_t<std::unordered_set<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::unordered_set<T>;

    __constexpr
    default_printer_t(const printer_t<T>& _a_printer);

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        u8string _l_str{u8"["};
        using Itt = typename value_type::const_iterator;
        const Itt _l_end{end(_a_object)};

        for (Itt _l_itt{begin(_a_object)}; _l_itt != _l_end;)
        {
            _l_str.append(_m_printer->run_printer(*_l_itt));
            ++_l_itt;
            if (_l_itt != _l_end)
            {
                _l_str.append(u8",");
            }
        }
        _l_str.append(u8"]");
        return _l_str;
    }
};

template <typename T>
struct default_printer_t<T*> : public printer_base_t<T*>
{
    using value_type = T*;

    __constexpr
    default_printer_t()
    // requires (std::is_default_constructible_v<default_printer_t<T>>)

    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
#ifdef UINTPTR_MAX
        // uintptr_t is available.
        using ptr_t = uintptr_t;
#else
        using ptr_t = size_t;
#endif
        const ptr_t _l_address{
            reinterpret_cast<ptr_t>(static_cast<void*>(_a_object))
        };
        return default_printer<ptr_t>()->run_printer(_l_address);
    }
};

template <typename T>
struct default_printer_t<std::shared_ptr<T>>
    : public printer_base_t<std::shared_ptr<T>>

{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{true};
    using value_type = std::shared_ptr<T>;
    enum_pointer_print_parse_type_t _m_enum
        = enum_pointer_print_parse_type_t::AS_OBJECT;

    __constexpr
    default_printer_t(
        const printer_t<T>&                   _a_printer,
        const enum_pointer_print_parse_type_t _a_enum
        = enum_pointer_print_parse_type_t::AS_OBJECT
    )
        : _m_printer(_a_printer), _m_enum(_a_enum)
    {}

    __constexpr
    default_printer_t()
        // requires (std::is_default_constructible_v<default_printer_t<T>>)
        : _m_printer(mk_printer(default_printer_t<T>()))
    {}

    __constexpr virtual std::u8string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace _ABC_NS_UTILITY_STR;
        using enum enum_pointer_print_parse_type_t;
        switch (_m_enum)
        {
        case AS_OBJECT:
            return object_printer_with_field_names_and_custom_printers(
                object_printer_parser_t{},
                type_id<decltype(_a_object)>(),
                {u8"address", u8"data"},
                make_tuple(default_printer<T*>(), _m_printer),
                _a_object.get(),
                *(_a_object.get())
            );
        case JUST_DATA:
            return _m_printer->run_printer(*_a_object.get());
        default:
            throw errors::unaccounted_for_enum_exception(_m_enum);
        }
    }
};

_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

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
    default_printer_t<std::pair<T, U>>::default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    )
    : _m_printers{std::make_pair(_a_printer_t, _a_printer_u)}
{}

template <typename T, typename U>
__constexpr_imp std::u8string
                default_printer_t<std::pair<T, U>>::run_printer(
        const default_printer_t<std::pair<T, U>>::value_type& _a_object
    ) const
{
    using namespace std;
    u8string _l_str{u8"("};
    _l_str.append(_m_printers.first->run_printer(_a_object.first));
    _l_str.append(u8", ");
    _l_str.append(_m_printers.second->run_printer(_a_object.second));
    _l_str.append(u8")");
    return _l_str;
}

template <typename T, typename U>
__constexpr_imp
    default_printer_t<std::map<T, U>>::default_printer_t(
        const printer_t<T>& _a_printer_t,
        const printer_t<U>& _a_printer_u
    )
    : _m_key_printer(_a_printer_t), _m_element_printer(_a_printer_t)
{}

template <typename T, typename U>
__constexpr_imp std::u8string
                default_printer_t<std::map<T, U>>::run_printer(
        const default_printer_t<std::map<T, U>>::value_type& _a_object
    ) const
{
    using namespace std;
    u8string _l_str{u8"{"};
    for (const pair<T, U>& _l_element : _a_object)
    {
        _l_str.append(_m_key_printer->run_printer(_l_element.first));
        _l_str.append(u8":");
        _l_str.append(_m_element_printer->run_printer(_l_element.second));
    }
    _l_str.append(u8"}");
    return _l_str;
}

template <typename T>
__constexpr_imp
    default_printer_t<std::vector<T>>::default_printer_t(
        const printer_t<T>& _a_printer
    )
    : _m_printer{_a_printer}
{}

template <typename T>
__constexpr_imp std::u8string
                default_printer_t<std::vector<T>>::run_printer(
        const default_printer_t<std::vector<T>>::value_type& _a_object
    ) const
{
    using namespace std;
    u8string _l_str{u8"["};
    for (size_t _l_idx{0}; _l_idx < _a_object.size(); ++_l_idx)
    {
        _l_str.append(_m_printer->run_printer(_a_object[_l_idx]));
        if (_l_idx + 1 < _a_object.size())
        {
            _l_str.append(u8", ");
        }
    }
    _l_str.append(u8"]");
    return _l_str;
}

template <typename T>
struct hex_printer : public printer_base_t<T>
{
    printer_t<T> _m_printer;

    __constexpr
    hex_printer(
        const printer_t<T>& _a_printer
    )
        : _m_printer(_a_printer)
    {}

    __no_constexpr_imp std::u8string
                       run_printer(
                           const T& _a_object
                       ) const
    {
        return abc::utility::str::to_hex(_m_printer->run_printer(_a_object));
    }
};

_END_ABC_UTILITY_PRINTER_NS