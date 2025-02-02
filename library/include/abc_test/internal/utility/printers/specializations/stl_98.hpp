#pragma once
#include "abc_test/internal/utility/printers/default_printer.hpp"

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
    requires (std::is_default_constructible_v<default_printer_t<T>> && std::is_default_constructible_v<default_printer_t<U>>)
        : _m_printers(
              std::make_pair(default_printer<T>(), default_printer<U>())
          )
    {}

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
    {}

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
    : _m_printers{std::make_pair(_a_printer_t, _a_printer_u)}
{}

template <typename T, typename U>
__constexpr_imp std::string
                default_printer_t<std::pair<T, U>>::run_printer(
        const default_printer_t<std::pair<T, U>>::value_type& _a_object
    ) const
{
    using namespace std;
    string _l_str{"("};
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
    : _m_printer{_a_printer}
{}

template <typename T>
__constexpr_imp std::string
                default_printer_t<std::vector<T>>::run_printer(
        const default_printer_t<std::vector<T>>::value_type& _a_object
    ) const
{
    using namespace std;
    string _l_str{"{"};
    for (size_t _l_idx{0}; _l_idx < _a_object.size(); ++_l_idx)
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