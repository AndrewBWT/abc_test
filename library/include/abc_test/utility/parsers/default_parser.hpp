#pragma once

#include "abc_test/utility/parsers/parser_base.hpp"

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