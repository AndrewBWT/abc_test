#pragma once
#include "abc_test/internal/utility/parsers/default_parser.hpp"

_BEGIN_ABC_UTILITY_PARSER_NS

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
        _a_parse_input.check_advance_and_throw('"');
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
        _a_parse_input.check_advance_and_throw('"');
        return parse_result_t<T>(_l_rv);
    }
};

template <typename T, typename U>
struct default_parser_t<std::pair<T, U>> : public parser_base_t<std::pair<T, U>>
{
private:
    std::pair<default_parser_t<T>, default_parser_t<U>> _m_parsers;
public:
    using value_type = std::pair<T, U>;

    __constexpr
    default_parser_t(
        const default_parser_t<T>& _a_parser_t,
        const default_parser_t<U>& _a_parser_u
    );
    default_parser_t()
    requires (std::is_default_constructible_v<default_parser_t<T>>
              && std::is_default_constructible_v<default_parser_t<U>>)
    = default;

    __constexpr virtual parse_result_t<value_type>
        run_parser(parser_input_t& _a_parse_input) const;
};

template <typename T>
struct default_parser_t<std::vector<T>> : public parser_base_t<std::vector<T>>
{
private:
    parser_t<T> _m_parser;
public:

    __constexpr
        default_parser_t(const parser_t<T>& _a_parser)
        : _m_parser(_a_parser)
    {

    }
    default_parser_t()
        requires (std::is_default_constructible_v<default_parser_t<T>>)
    : _m_parser(mk_parser(default_parser_t<T>()))
    {
    }

    __constexpr virtual parse_result_t<std::vector<T>>
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
        const default_parser_t<T>& _a_parser_t,
        const default_parser_t<U>& _a_parser_u
    )
    : _m_parsers{std::make_pair(_a_parser_t, _a_parser_u)}
{}

template <typename T, typename U>
__constexpr_imp
    parse_result_t<typename default_parser_t<std::pair<T, U>>::value_type>
    default_parser_t<std::pair<T, U>>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    value_type _l_rv;
    _a_parse_input.check_advance_and_throw("(");
    if (auto _l_first_result{_m_parsers.first.run_parser(_a_parse_input)};
        _l_first_result.has_value())
    {
        _l_rv.first = _l_first_result.value();
        _a_parse_input.check_advance_and_throw(", ");
        if (auto _l_second_result{_m_parsers.second.run_parser(_a_parse_input)};
            _l_second_result.has_value())
        {
            _l_rv.second = _l_second_result.value();
            _a_parse_input.check_advance_and_throw(")");
            return parse_result_t<value_type>(_l_rv);
        }
        else
        {
            return parse_error<value_type>(fmt::format(
                "Could not parse std::tuple element 2. Failed with error "
                "message: {0}",
                _l_second_result.error()
            ));
        }
    }
    else
    {
        return parse_error<value_type>(fmt::format(
            "Could not parse std::tuple element 1. Failed with error "
            "message: {0}",
            _l_first_result.error()
        ));
    }
}

template <typename T>
__constexpr_imp parse_result_t<std::vector<T>>
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
                return parse_error<vector<T>>(_l_result.error());
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
                return parse_error<vector<T>>("unexpected character");
            }
        }
        return parse_error<vector<T>>("Shouldn't be poissibel to get here");
    }
}
_END_ABC_UTILITY_PARSER_NS