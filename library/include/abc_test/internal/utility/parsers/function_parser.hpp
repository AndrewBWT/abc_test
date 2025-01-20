#pragma once

#include "abc_test/internal/utility/parsers/parser_base.hpp"

_BEGIN_ABC_UTILITY_PARSER_NS

template <typename T, typename F>
    requires std::invocable<F, parser_input_t&>
struct function_parser_t : public parser_base_t<T>
{
public:
    __constexpr function_parser_t(F _a_callable) noexcept;
    __constexpr virtual parse_result_t<T>
        run_parser(parser_input_t& _a_parse_input) const override;
private:
    F _m_callable;
};
template <typename T, typename F>
    requires std::invocable<F, parser_input_t&>
__constexpr parse_result_t<T> parse(
    const std::string_view _a_str,
    F _a_callable
);
_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PARSER_NS
template <typename T, typename F>
    requires std::invocable<F, parser_input_t&>
__constexpr_imp function_parser_t<T, F>::function_parser_t(F _a_callable) noexcept
    : _m_callable(_a_callable)
{

}
template <typename T, typename F>
    requires std::invocable<F, parser_input_t&>
__constexpr parse_result_t<T>

function_parser_t<T, F>::run_parser(
    parser_input_t& _a_parse_input
) const
{
    return std::invoke(_m_callable, _a_parse_input);
}
template <typename T, typename F>
    requires std::invocable<F, parser_input_t&>
__constexpr parse_result_t<T> parse(
    const std::string_view _a_str,
    F _a_callable
)
{
    using namespace std;
    return parse<T>(_a_str, make_shared<function_parser_t<T, F>>(_a_callable));
}
_END_ABC_UTILITY_PARSER_NS