#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"
#include <expected>
#include <string>

_BEGIN_ABC_UTILITY_PARSER_NS
template <typename T>
using parse_result_t = std::expected<T, std::string>;

template<typename T>
__constexpr parse_result_t<T> parse_error(const std::string_view _a_str) noexcept;
_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PARSER_NS

template<typename T>
__constexpr_imp parse_result_t<T> parse_error(const std::string_view _a_str) noexcept
{
    using namespace std;
    return parse_result_t<T>(unexpected(_a_str));
}
_END_ABC_UTILITY_PARSER_NS