#pragma once
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/parsers/specializations/fundamental_types.hpp"
#include "abc_test/utility/parsers/specializations/stl_98.hpp"

#include <filesystem>
#include <variant>

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

    __constexpr virtual parse_result_t<value_type>
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

    __constexpr virtual parse_result_t<value_type>
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
                    value_type&       _a_object,
                    const std::size_t _a_idx,
                    parser_input_t&   _a_parse_input
                ) const;
};

template<>
struct default_parser_t<std::filesystem::path>
    : public parser_base_t<std::filesystem::path>
{
    __constexpr virtual parse_result_t<std::filesystem::path>
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
__constexpr
    parse_result_t<typename default_parser_t<std::tuple<Ts...>>::value_type>
    default_parser_t<std::tuple<Ts...>>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    value_type _l_rv;
    _a_parse_input.check_advance_and_throw("(");
    auto _l_res{run_parser_internal<0>(_l_rv, _a_parse_input)};
    if (_l_res.has_value())
    {
        return parse_error<value_type>(_l_res.value());
    }
    _a_parse_input.check_advance_and_throw(")");
    return parse_result_t<value_type>(_l_rv);
}

template <typename... Ts>
template <std::size_t I>
__constexpr std::optional<std::string>
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
            _a_parse_input.check_advance_and_throw(", ");
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
    : _m_parsers{std::move(_a_parsers)...}
{}

template <typename... Ts>
__constexpr
    parse_result_t<typename default_parser_t<std::variant<Ts...>>::value_type>
    default_parser_t<std::variant<Ts...>>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    value_type               _l_rv;
    default_parser_t<size_t> _l_size_t_parser;
    parse_result_t<size_t>   _l_size_t_result{
        _l_size_t_parser.run_parser(_a_parse_input)
    };
    _a_parse_input.check_advance_and_throw(" ");
    auto _l_res{
        run_parser_internal<0>(_l_rv, _l_size_t_result.value(), _a_parse_input)
    };
    if (_l_res.has_value())
    {
        return parse_error<value_type>(_l_res.value());
    }
    else
    {
        return parse_result_t<value_type>(_l_rv);
    }
}

template <typename... Ts>
template <std::size_t I>
__constexpr std::optional<std::string>
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

__constexpr parse_result_t<std::filesystem::path>
default_parser_t<std::filesystem::path>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    return std::filesystem::path(_a_parse_input.sv());
}

_END_ABC_UTILITY_PARSER_NS