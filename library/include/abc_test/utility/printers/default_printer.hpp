#pragma once

#include "abc_test/utility/printers/printer_base.hpp"

#include "abc_test/utility/concepts.hpp"
#include "abc_test/utility/object_printer_parser.hpp"

#include <filesystem>

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct default_printer_t : public printer_base_t<T>
{
public:
    static constexpr bool is_specialized{false};
    __constexpr virtual std::string
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
__constexpr std::string
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
    static constexpr bool is_specialized{ true };

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
    static constexpr bool is_specialized{ true };

    __constexpr           std::string
        run_printer(
            const char& _a_object
        ) const
    {
        return "`" + std::string(1, _a_object) + "`";
    }
};

template <typename T>
    requires enum_has_list_c<T>
struct default_printer_t<T> : public printer_base_t<T>
{
    default_printer_t<T>(
        const enum_helper_string_case_t _a_enum_helper_string_case
    )
        : _m_enum_helper_string_case(_a_enum_helper_string_case)
    {

    }
    enum_helper_string_case_t _m_enum_helper_string_case;
    __constexpr std::string
        run_printer(
            const T& _a_element
        ) const
    {
        return _ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
            .print(_a_element, _m_enum_helper_string_case);
    }
};

template <typename T>
    requires (std::is_same_v<std::remove_cvref_t<T>, T>&& is_to_string_convertable_c<T> && (not std::same_as<T, bool>) && (not std::same_as<char, T>)) && (not std::floating_point<T>)
struct default_printer_t<T> : public printer_base_t<T>
{
    static constexpr bool is_specialized{ true };

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
    const std::string_view& _a_begin_str,
    std::tuple<printer_t<Ts>...>            _a_parsers,
    Ts... _a_elements_to_print
);
template <typename... Ts>
__constexpr std::string
object_printer(
    const utility::object_printer_parser_t& _a_object_print_parser,
    const std::string_view& _a_begin_str,
    Ts... _a_elements_to_print
);
template <typename... Ts>
__constexpr std::string
object_printer_with_field_names_and_custom_printers(
    const utility::object_printer_parser_t& _a_object_print_parser,
    const std::string_view& _a_begin_str,
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    std::tuple<printer_t<Ts>...>                       _a_parsers,
    Ts... _a_elements_to_print
);
template <typename... Ts>
__constexpr std::string
object_printer_with_field_names(
    const utility::object_printer_parser_t& _a_object_print_parser,
    const std::string_view& _a_begin_str,
    const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
    Ts... _a_elements_to_print
);

namespace
{
    template <typename... Ts>
    __constexpr std::string
        object_printer_internal(
            const utility::object_printer_parser_t& _a_object_print_parser,
            const std::string_view& _a_begin_str,
            const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
            _a_object_names,
            std::tuple<printer_t<Ts>...> _a_parsers,
            Ts... _a_elements_to_print
        );
    template <std::size_t I, typename... Ts>
    __constexpr void
        object_printer_internal(
            const utility::object_printer_parser_t& _a_object_print_parser,
            std::string& _a_str,
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
__constexpr std::string
object_printer_with_custom_printers(
    const utility::object_printer_parser_t& _a_object_print_parser,
    const std::string_view& _a_begin_str,
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
    const std::string_view& _a_begin_str,
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
    const std::string_view& _a_begin_str,
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
    const std::string_view& _a_begin_str,
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
            const std::string_view& _a_begin_str,
            const std::optional<std::array<std::string_view, sizeof...(Ts)>>&
            _a_object_names,
            std::tuple<printer_t<Ts>...> _a_parsers,
            Ts... _a_elements_to_print
        )
    {
        using namespace std;
        string _l_rv{ _a_begin_str };
        if (_a_object_print_parser.space_after_object_name)
        {
            _l_rv.append(" ");
        }
        _l_rv.push_back(_a_object_print_parser.begin_char);
        tuple<Ts...> _l_tuple{ tie(_a_elements_to_print...) };
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
            std::string& _a_str,
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

    __constexpr std::tuple<printer_t<Ts>...>&
        get_printers_ref() noexcept
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
        run_printer(
            const value_type& _a_parse_input
        ) const
    {
        return _a_parse_input.string();
    }
};

template <typename T>
struct default_printer_t<std::optional<T>>
    : public printer_base_t<std::optional<T>>
{
    using value_type = std::optional<T>;
    static constexpr bool is_specialized{ true };

    __constexpr
        default_printer_t(
            printer_t<T> _a_printer
        )
        : _m_printer(_a_printer)
    {
    }

    __constexpr
        default_printer_t()
        requires (std::is_default_constructible_v<default_printer_t<T>>)
    : _m_printer(mk_printer(default_printer_t<T>()))
    {
    }

    __constexpr virtual std::string
        run_printer(
            const value_type& _a_object
        ) const
    {
        using namespace std;
        string _l_rv;
        _l_rv.append("{");
        if (_a_object.has_value())
        {
            _l_rv.append(_m_printer->run_printer(_a_object.value()));
        }
        _l_rv.append("}");
        return _l_rv;
    }
private:
    printer_t<T> _m_printer;
};

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS template <typename... Ts>
__constexpr
default_printer_t<std::tuple<Ts...>>::default_printer_t(
    printer_t<Ts>... _a_printers
)
    : _m_printers{ std::move(_a_printers)... }

{
}

template <typename... Ts>
__constexpr std::string
default_printer_t<std::tuple<Ts...>>::run_printer(
    const default_printer_t<std::tuple<Ts...>>::value_type& _a_object
) const
{
    using namespace std;
    string _l_str{ "(" };
    run_internal_printer<0>(_l_str, _a_object);
    _l_str.append(")");
    return _l_str;
}

template <typename... Ts>
template <std::size_t I>
__constexpr_imp void
default_printer_t<std::tuple<Ts...>>::run_internal_printer(
    std::string& _a_str,
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

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
    requires (std::convertible_to<T, std::string_view>)
struct default_printer_t<T> : public printer_base_t<T>
{
    static constexpr bool is_specialized{ true };
    __constexpr std::string
        run_printer(
            const T& _a_object
        ) const
    {
        if constexpr (std::same_as<T, char>)
        {
            auto xi = fmt::format("\"{0}\"", _a_object);
            int x = 3;
        }
        return fmt::format("\"{0}\"", _a_object);
    }
};

template <typename T, typename U>
struct default_printer_t<std::pair<T, U>>
    : public printer_base_t<std::pair<T, U>>
{
private:
    std::pair<printer_t<T>, printer_t<U>> _m_printers;
public:
    static constexpr bool is_specialized{ true };
    using value_type = std::pair<T, U>;

    __constexpr
        default_printer_t(
            const printer_t<T>& _a_printer_t,
            const printer_t<U>& _a_printer_u
        );

    default_printer_t()
        requires (std::is_default_constructible_v<default_printer_t<T>>&& std::is_default_constructible_v<default_printer_t<U>>)
    : _m_printers(
        std::make_pair(default_printer<T>(), default_printer<U>())
    )
    {
    }

    __constexpr virtual std::string
        run_printer(const value_type& _a_object) const;
};

template <typename T>
struct default_printer_t<std::vector<T>> : public printer_base_t<std::vector<T>>
{
private:
    printer_t<T> _m_printer;
public:
    static constexpr bool is_specialized{ true };
    using value_type = std::vector<T>;

    __constexpr
        default_printer_t(const printer_t<T>& _a_printer);

    default_printer_t()
        requires (std::is_default_constructible_v<default_printer_t<T>>)
    : _m_printer(mk_printer(default_printer_t<T>()))
    {
    }

    __constexpr virtual std::string
        run_printer(const value_type& _a_object) const;
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
    : _m_printers{ std::make_pair(_a_printer_t, _a_printer_u) }
{
}

template <typename T, typename U>
__constexpr_imp std::string
default_printer_t<std::pair<T, U>>::run_printer(
    const default_printer_t<std::pair<T, U>>::value_type& _a_object
) const
{
    using namespace std;
    string _l_str{ "(" };
    _l_str.append(_m_printers.first->run_printer(_a_object.first));
    _l_str.append(", ");
    _l_str.append(_m_printers.second->run_printer(_a_object.second));
    _l_str.append(")");
    return _l_str;
}

template <typename T>
__constexpr_imp
default_printer_t<std::vector<T>>::default_printer_t(
    const printer_t<T>& _a_printer
)
    : _m_printer{ _a_printer }
{
}

template <typename T>
__constexpr_imp std::string
default_printer_t<std::vector<T>>::run_printer(
    const default_printer_t<std::vector<T>>::value_type& _a_object
) const
{
    using namespace std;
    string _l_str{ "{" };
    for (size_t _l_idx{ 0 }; _l_idx < _a_object.size(); ++_l_idx)
    {
        _l_str.append(_m_printer->run_printer(_a_object[_l_idx]));
        if (_l_idx + 1 < _a_object.size())
        {
            _l_str.append(", ");
        }
    }
    _l_str.append("}");
    return _l_str;
}

_END_ABC_UTILITY_PRINTER_NS