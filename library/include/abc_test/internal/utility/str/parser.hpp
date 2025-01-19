#pragma once
#include "abc_test/internal/errors/test_library_exception.hpp"
#include "abc_test/internal/utility/concepts.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"

#include <expected>
#include <fmt/format.h>
#include <functional>
#include <scn/scan.h>
#include <string>
#include <variant>
#include <vector>

#include "abc_test/internal/utility/str/parser_input.hpp"

_BEGIN_ABC_UTILITY_STR_NS
template <typename T>
using parse_result_t = std::expected<T, std::string>;


// Forward declaration
namespace
{
template <typename T>
class parsable_object_t;
} // namespace

template <typename T>
struct parser_t
{
    __constexpr virtual parse_result_t<T>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        __STATIC_ASSERT(
            T,
            "Function requires a specialization of the function "
            "run_parser<T>()."
        );
    }
};

template <typename T, typename F>
requires std::invocable<F, parser_input_t&>
__constexpr_imp parse_result_t<T>
                parse(
                    const std::string_view _a_str,
                    F                      _a_callable
                ) noexcept
{
    using namespace std;
    using namespace errors;
    parser_input_t _l_pit(_a_str);
    try
    {
        const parse_result_t<T> _l_inner_parser_result{
            std::invoke(_a_callable, _l_pit)
        };
        if (_l_inner_parser_result.has_value())
        {
            if (not _l_pit.at_end())
            {
                return parse_result_t<T>{
                    unexpected("Parser okay but not at end of string")
                };
            }
        }
        return _l_inner_parser_result;
    }
    catch (const std::runtime_error& _a_exception)
    {
        return parse_result_t<T>{unexpected("Parser threw unexpected exception")
        };
    }
}

template <typename T>
__constexpr_imp parse_result_t<T>
                parse(
                    const std::string_view _a_str,
                    const parser_t<T>      _a_parser = parser_t<T>()
                ) noexcept
{
    using namespace std;
    using namespace errors;
    parser_input_t _l_pit(_a_str);
    try
    {
        const parse_result_t<T> _l_inner_parser_result{
            _a_parser.run_parser(_l_pit)
        };
        if (_l_inner_parser_result.has_value())
        {
            if (not _l_pit.at_end())
            {
                return parse_result_t<T>{
                    unexpected("Parser okay but not at end of string")
                };
            }
        }
        return _l_inner_parser_result;
    }
    catch (const std::runtime_error& _a_exception)
    {
        return parse_result_t<T>{unexpected("Parser threw unexpected exception")
        };
    }
}

template <typename T>
__constexpr_imp T
    run_parser_with_exception(
        const std::string_view _a_str,
        const parser_t<T>      _a_parser = parser_t<T>()
    )
{
    using namespace std;
    using namespace errors;
    const parse_result_t<T> _l_parse_result{parse(_a_str, _a_parser)};
    if (_l_parse_result.has_value())
    {
        return _l_parse_result.value();
    }
    else
    {
        throw test_library_exception_t(_l_parse_result.error());
    }
}

template <typename T>
requires is_from_chars_convertable_c<T>
struct parser_t<T>
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
            return parse_result_t<T>(unexpected(fmt::format(
                "Could not parse \"{0}\" using std::from_chars", _l_str
            ))

            );
        }
    }
};

template <typename T>
requires (std::convertible_to<T, std::string_view>)
struct parser_t<T>
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
    }
};

template <typename... Ts>
struct parser_t<std::tuple<Ts...>>
{
    using value_type = std::tuple<Ts...>;

    parser_t(
        parser_t<Ts>... _a_parsers
    )
        : _m_parsers{std::move(_a_parsers)...}
    {}

    parser_t()
    requires (std::is_default_constructible_v<parser_t<Ts>> && ...)
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
    std::tuple<parser_t<Ts>...> _m_parsers;

    template <std::size_t I>
    __constexpr void
        run_parser_internal(
            value_type&    _a_object,
            parser_input_t& _a_parse_input
        ) const
    {
        if (auto _l_result{std::get<I>(_m_parsers).run_parser(_a_parse_input)};
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
struct parser_t<std::pair<T, U>>
{
private:
    std::pair<parser_t<T>, parser_t<U>> _m_parsers;
public:
    using value_type = std::pair<T, U>;

    parser_t(
        const parser_t<T>& _a_parser_t,
        const parser_t<U>& _a_parser_u
    )
        : _m_parsers{std::make_pair(_a_parser_t, _a_parser_u)}
    {}

    parser_t()
    requires (std::is_default_constructible_v<parser_t<T>>
              && std::is_default_constructible_v<parser_t<U>>)
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
        if (auto _l_first_result{_m_parsers.first.run_parser(_a_parse_input)};
            _l_first_result.has_value())
        {
            _l_rv.first = _l_first_result.value();
        }
        if (_a_parse_input.check_and_advance(", "))
        {
        }
        if (auto _l_second_result{_m_parsers.second.run_parser(_a_parse_input)};
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
        utility::str::parser_input_t&                       _a_parse_input,
        std::tuple<Ts...>&                                 _a_default_tuple,
        const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
        std::tuple<parser_t<Ts>...>                        _a_parsers
    ) noexcept
{
    if (_a_parse_input.check_and_advance(std::get<I>(_a_object_names)))
    {
    }
    if (_a_parse_input.check_and_advance(" = "))
    {
    }
    if (auto _l_result{std::get<I>(_a_parsers).run_parser(_a_parse_input)};
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
                utility::str::parser_input_t&                       _a_parse_input,
                std::tuple<parser_t<Ts>...>                        _a_parsers
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
requires (std::is_default_constructible_v<parser_t<Ts>> && ...)
__constexpr abc::utility::str::parse_result_t<T>
            object_parser(
                const std::array<std::string_view, sizeof...(Ts)>& _a_object_names,
                const std::string_view                             _a_begin_str,
                const char                                         _a_begin_char,
                const char                                         _a_end_char,
                utility::str::parser_input_t&                       _a_parse_input
            )
{
    std::tuple<parser_t<Ts>...> _l_parsers;
    return object_parser<T>(
        _a_object_names,
        _a_begin_str,
        _a_begin_char,
        _a_end_char,
        _a_parse_input,
        _l_parsers
    );
}

/*template <typename T>
    requires abc::utility::str::scannable<T>&& std::is_fundamental<T>::value &&
(not std::is_convertible_v<T, std::string_view>)
__constexpr abc::utility::str::parser_t<T>
default_parser() noexcept
{
    using namespace std;
    return parser<T>(
        [](abc::utility::str::parse_input_t& _a_str)
        {
            T                 result{};
            const string_view _l_str{_a_str.sv()};
            auto [ptr, ec] = from_chars(
                _l_str.data(), _l_str.data() + _l_str.size(), result
            );

            if (ec == std::errc())
            {
                _a_str.advance((ptr - _l_str.data()) + 0);
                return expected<T, string>(result);
            }
            else
            {
                return expected<T, string>(
                    unexpected(fmt::format("Could not parse \"{0}\"", _l_str))

                );
            }
        }
    );
}*/
namespace
{
template <typename T>
struct parsable_object_t
{
public:
    __constexpr virtual parse_result_t<T>
        run(parser_input_t&) const noexcept = 0;
};

template <typename T, typename F>
requires std::invocable<F, parser_input_t&>
class parseable_t : public parsable_object_t<T>
{
public:
    __constexpr
                parseable_t(F _a_callable_object) noexcept;
    __constexpr parse_result_t<T>
                run(parser_input_t& _a_str) const noexcept;
private:
    F _m_callable_object;
};
} // namespace

template <typename T>
struct parser_from_func_t : parser_t<T>
{
    __constexpr
    parser_from_func_t()
    {}

    template <typename T, typename F>
    requires std::invocable<F, utility::str::parser_input_t&>
    __constexpr
    parser_from_func_t(
        F _a_callable
    ) noexcept
        : _m_func(std::make_shared<parseable_t<T, F>>(_a_callable))
    {}

    __constexpr virtual parse_result_t<T>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        return _m_func->run(_a_parse_input);
    }
private:
    std::shared_ptr<parsable_object_t<T>> _m_func;
};

/*template <typename T>
struct parser_t
{
public:
    __constexpr parse_result_t<T>
                run_parser(const std::string_view _a_str) const noexcept;
    __constexpr T
                run_parser_with_exception(const std::string_view _a_str) const;
    __constexpr parse_result_t<T>
                run_inner_parser(parse_input_t& _a_parse_input) const noexcept;
    __constexpr
    parser_t(const std::shared_ptr<parsable_object_t<T>>& _a_parser) noexcept;
private:
    std::shared_ptr<parsable_object_t<T>> _m_func;
};*/


_END_ABC_UTILITY_STR_NS
_BEGIN_ABC_NS

/*template <typename T, typename F>
requires std::invocable<F, utility::str::parse_input_t&>
__constexpr abc::utility::str::parser_t<T>
            parser(F _a_callable) noexcept;
template <typename T>
// requires abc::utility::str::scannable<T> && std::is_fundamental<T>::value &&
//(not std::is_convertible_v<T, std::string_view>)
__constexpr abc::utility::str::parser_t<T>
            default_parser() noexcept;
template <typename T>
requires abc::utility::str::scannable<T> && std::is_fundamental<T>::value
         && ( not std::is_convertible_v<T, std::string_view> )
__constexpr abc::utility::str::parser_t<T> default_parser() noexcept;
template <typename T>
requires is_tuple<T>::value
__no_constexpr abc::utility::str::parser_t<T>
               default_parser() noexcept;

template <typename T>
requires std::is_convertible_v<T, std::string_view>
__no_constexpr_imp abc::utility::str::parser_t<T>
                   default_parser()
{
    return parser<T>(
        [&](abc::utility::str::parse_input_t& _a_str)
        {
            if (*_a_str != '"')
            {
                std::expected<T, std::string>(std::unexpected("Coulnd't work"));
            }
            char _l_prev_char = '"';
            ++_a_str;
            T rv{};
            while (true)
            {
                if (_a_str.at_end())
                {
                    throw std::runtime_error("At end");
                }
                if (*_a_str == '"' && _l_prev_char != '\\')
                {
                    ++_a_str;
                    break;
                }
                else
                {
                    rv.push_back(*_a_str);
                    _l_prev_char = *_a_str;
                    ++_a_str;
                }
            }
            return std::expected<T, std::string>(rv);
        }
    );
}

template <typename... Args>
__no_constexpr_imp abc::utility::str::parser_t<std::tuple<Args...>>
                   default_parser_tuple_(
                       const std::tuple<Args...>& _a_default_arg,
                       const char                 _a_begin_char,
                       const char                 _a_end_char
                   ) noexcept
{
    auto parsers = make_default_parsers<Args...>();
    return default_parser_tuple<Args...>(_a_begin_char, _a_end_char, parsers);
}

template <typename T, typename U>
__no_constexpr_imp auto
    default_parser_pair(
        const std::pair<T, U>& _a_pair
    ) noexcept
{
    return parser<std::pair<T, U>>(
        [&](abc::utility::str::parse_input_t& _a_str)
        {
            utility::str::parser_t<T> _l_parse_t = default_parser<T>();
            utility::str::parser_t<U> _l_parse_u = default_parser<U>();
            if (_a_str.check_and_advance('('))
            {
            }
            auto _l_t_result{_l_parse_t.run_inner_parser(_a_str)};
            if (_l_t_result.has_value())
            {
                if (_a_str.check_and_advance(", "))
                {
                }
                auto _l_u_result{_l_parse_u.run_inner_parser(_a_str)};
                if (_l_u_result.has_value())
                {
                    if (_a_str.check_and_advance(")"))
                    {
                    }
                    return std::expected<std::pair<T, U>, std::string>(
                        make_pair(_l_t_result.value(), _l_u_result.value())
                    );
                }
            }
            return std::expected<std::pair<T, U>, std::string>(
                std::unexpected("Coulnd't work")
            );
        }
    );
}

template <typename T>
requires is_pair<T>::value
__no_constexpr_imp abc::utility::str::parser_t<T>
                   default_parser() noexcept
{
    return default_parser_pair(T{});
}

template <
    std::size_t       curr_idx,
    const std::size_t max_idx,
    typename... Args,
    typename... Args2>
__constexpr auto
    run_object_reader(
        utility::str::parse_input_t&                         _a_parse_input,
        std::tuple<Args...>&                                 _a_tuple,
        const std::tuple<Args2...>&                          _a_parsers,
        const std::array<std::string_view, sizeof...(Args)>& _a_object_names
    )
{
    auto parser = std::get<curr_idx>(_a_parsers);
    if (_a_parse_input.check_and_advance(std::get<curr_idx>(_a_object_names)))
    {
        return std::expected<std::tuple<Args...>, std::string>(
            std::unexpected("nopte")
        );
    }
    if (_a_parse_input.check_and_advance(" = "))
    {
        return std::expected<std::tuple<Args...>, std::string>(
            std::unexpected("nopte")
        );
    }
    auto result = parser.run_inner_parser(_a_parse_input);
    if (result.has_value())
    {
        std::get<curr_idx>(_a_tuple) = result.value();
        if constexpr (curr_idx + 1 == max_idx)
        {
            return std::expected<std::tuple<Args...>, std::string>(_a_tuple);
        }
        else
        {
            if (_a_parse_input.check_and_advance(", "))
            {
            }
            return run_object_reader<curr_idx + 1, max_idx>(
                _a_parse_input, _a_tuple, _a_parsers, _a_object_names
            );
        }
    }
    else
    {
        return std::expected<std::tuple<Args...>, std::string>(
            std::unexpected(result.error())
        );
    }
}

template <typename T, typename... Args, typename... Args2>
__constexpr abc::utility::str::parse_result_t<T>
            object_parser(
                const std::array<std::string_view, sizeof...(Args)>&
_a_object_names, const std::string_view _a_begin_str, const char _a_begin_char,
                const char _a_end_char, utility::str::parse_input_t&
_a_parse_input, const std::tuple<Args2...>&                          _a_parsers
            )
{
    using namespace std;
    using namespace abc::utility::str;
    size_t         _l_idx{0};
    tuple<Args...> default_tuple;
    const size_t   _l_tuple_size{std::tuple_size_v<std::tuple<Args...>>};
    if (_a_parse_input.check_and_advance(_a_begin_str))
    {
        return parse_result_t<T>(unexpected("Couldn't work out"));
    }
    if (_a_parse_input.check_and_advance(' '))
    {
        return parse_result_t<T>(unexpected("Couldn't work out"));
    }
    if (_a_parse_input.check_and_advance(_a_begin_char))
    {
        return parse_result_t<T>(unexpected("Couldn't work out"));
    }
    run_object_reader<0, std::tuple_size_v<tuple<Args...>>>(
        _a_parse_input, default_tuple, _a_parsers, _a_object_names
    );
    if (_a_parse_input.check_and_advance(_a_end_char))
    {
        return parse_result_t<T>(unexpected("Couldn't work out"));
    }
    else
    {
        return parse_result_t<T>(std::make_from_tuple<T>(default_tuple));
    }
}*/

_END_ABC_NS

_BEGIN_ABC_UTILITY_STR_NS

/*template <typename T>
__constexpr_imp parse_result_t<T>
                parser_t<T>::run_parser(
        const std::string_view _a_str
    ) const noexcept
{
    using namespace std;
    using namespace errors;
    parse_input_t _l_pit(_a_str);
    try
    {
        const parse_result_t<T> _l_inner_parser_result{run_inner_parser(_l_pit)
        };
        if (_l_inner_parser_result.has_value())
        {
            if (not _l_pit.at_end())
            {
                return parse_result_t<T>{
                    unexpected("Parser okay but not at end of string")
                };
            }
        }
        return _l_inner_parser_result;
    }
    catch (const std::runtime_error& _a_exception)
    {
        return parse_result_t<T>{unexpected("Parser threw unexpected exception")
        };
    }
}

template <typename T>
__constexpr_imp T
    parser_t<T>::run_parser_with_exception(
        const std::string_view _a_str
    ) const
{
    using namespace std;
    using namespace errors;
    const parse_result_t<T> _l_parse_result{run_parser(_a_str)};
    if (_l_parse_result.has_value())
    {
        return _l_parse_result.value();
    }
    else
    {
        throw test_library_exception_t(_l_parse_result.error());
    }
}

template <typename T>
__constexpr_imp parse_result_t<T>
                parser_t<T>::run_inner_parser(
        parse_input_t& _a_parse_input
    ) const noexcept
{
    return _m_func->run(_a_parse_input);
}

template <typename T>
__constexpr_imp
    parser_t<T>::parser_t(
        const std::shared_ptr<parsable_object_t<T>>& _a_func
    ) noexcept
    : _m_func(_a_func)
{}*/

namespace
{
template <typename T, typename F>
requires std::invocable<F, parser_input_t&>
__constexpr
parseable_t<T, F>::parseable_t(
    F _a_callable_object
) noexcept
    : _m_callable_object(_a_callable_object)
{}

template <typename T, typename F>
requires std::invocable<F, parser_input_t&>
__constexpr parse_result_t<T>
            parseable_t<T, F>::run(
        parser_input_t& _a_str
    ) const noexcept
{
    return std::invoke(_m_callable_object, _a_str);
}
} // namespace

_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_NS
/*template <typename T, typename F>
requires std::invocable<F, utility::str::parse_input_t&>
__constexpr abc::utility::str::parser_t<T>
            parser(
                F _a_callable
            ) noexcept
{
    using namespace std;
    using namespace abc::utility::str;
    return parser_t<T>(make_shared<parseable_t<T, F>>(_a_callable));
}

template <typename T>
    requires abc::utility::str::scannable<T>&& std::is_fundamental<T>::value &&
(not std::is_convertible_v<T, std::string_view>)
__constexpr abc::utility::str::parser_t<T>
default_parser() noexcept
{
    using namespace std;
    return parser<T>(
        [](abc::utility::str::parse_input_t& _a_str)
        {
            T                 result{};
            const string_view _l_str{_a_str.sv()};
            auto [ptr, ec] = from_chars(
                _l_str.data(), _l_str.data() + _l_str.size(), result
            );

            if (ec == std::errc())
            {
                _a_str.advance((ptr - _l_str.data()) + 0);
                return expected<T, string>(result);
            }
            else
            {
                return expected<T, string>(
                    unexpected(fmt::format("Could not parse \"{0}\"", _l_str))

                );
            }
        }
    );
}

template <typename>
struct is_tuple : std::false_type
{};

template <typename... T>
struct is_tuple<std::tuple<T...>> : std::true_type
{};

template <typename>
struct is_pair : std::false_type
{};

template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type
{};

template <typename... Args>
__no_constexpr_imp abc::utility::str::parse_result_t<std::tuple<Args...>>
                   run_tuple_parse(
                       std::tuple<Args...>&&  _a_tuple,
                       const std::string_view _a_str,
                       const size_t           _a_idx,
                       const size_t           _a_max_idx
                   )
{
    using namespace std;
    using T               = std::tuple<Args...>;
    auto _l_first_element = _a_str.find_first_of(",'\"");
    if (_l_first_element == string::npos)
    {
        if (_a_max_idx == 0 && _a_idx == 0)
        {
            get<0>(_a_tuple)
                = default_parser<tuple_element<0, T>>().run_parser(_a_str);
        }
    }
    return _a_tuple;
}

template <typename T>
__constexpr_imp abc::utility::str::parser_t<T>
                get_default_parser()
{
    return default_parser<T>();
}

template <typename... Args>
auto
    make_default_parsers()
{
    return std::make_tuple(get_default_parser<Args>()...);
}

template <
    std::size_t curr_idx,
    std::size_t max_idx,
    typename... Args,
    typename... Args2>
__no_constexpr_imp auto
    run_tuple_taker(
        abc::utility::str::parse_input_t& _a_str,
        std::tuple<Args...>&              _a_tuple,
        const std::tuple<Args2...>&       _a_parsers
    )
{
    auto parser = std::get<curr_idx>(_a_parsers);
    auto result = parser.run_inner_parser(_a_str);
    if (result.has_value())
    {
        std::get<curr_idx>(_a_tuple) = result.value();
        if constexpr (curr_idx + 1 == max_idx)
        {
            return std::expected<std::tuple<Args...>, std::string>(_a_tuple);
        }
        else
        {
            if (*_a_str != ',')
            {
            }
            if (*_a_str != ' ')
            {
            }
            return run_tuple_taker<curr_idx + 1, max_idx>(
                _a_str, _a_tuple, _a_parsers
            );
        }
    }
    else
    {
        return std::expected<std::tuple<Args...>, std::string>(
            std::unexpected(result.error())
        );
    }
}

template <typename... Args, typename... Args2>
__no_constexpr_imp abc::utility::str::parser_t<std::tuple<Args...>>
                   default_parser_tuple(
                       const char           _a_begin_char,
                       const char           _a_end_char,
                       std::tuple<Args2...> _a_parser_args
                   ) noexcept
{
    using namespace std;
    using T = std::tuple<Args...>;
    using namespace abc::utility::str;
    return parser<T>(
        [=](abc::utility::str::parse_input_t& _a_str)
        {
            auto           ki = _a_parser_args;
            size_t         _l_idx{0};
            tuple<Args...> default_tuple;
            const size_t _l_tuple_size{std::tuple_size_v<std::tuple<Args...>>};
            if (*_a_str != '(')
            {
                return parse_result_t<T>(unexpected("Couldn't work out"));
            }
            _a_str++;
            run_tuple_taker<0, std::tuple_size_v<tuple<Args...>>>(
                _a_str, default_tuple, _a_parser_args
            );
            if (*_a_str != ')')
            {
                return parse_result_t<T>(unexpected("Couldn't work out"));
            }
            else
            {
                return parse_result_t<T>(default_tuple);
            }
        }
    );
}*/

/*template <typename T>
requires is_tuple<T>::value
__no_constexpr_imp abc::utility::str::parser_t<T>
                   default_parser() noexcept
{
    return default_parser_tuple_(T{}, '(', ')');
}

template <typename T>
__constexpr abc::utility::str::parser_t<T>
            default_parser() noexcept
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "default_parser<T>()."
    );
}
*/
_END_ABC_NS