#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"

#include <fmt/format.h>
#include <stdexcept>
#include <string>

_BEGIN_ABC_UTILITY_PARSER_NS

class parser_at_end_of_string_exception_t : std::runtime_error
{
public:
    __constexpr
    parser_at_end_of_string_exception_t()
        = delete;
    __no_constexpr
    parser_at_end_of_string_exception_t(
        const std::string_view _a_str,
        const std::size_t      _a_elements_read
    ) noexcept;
};

class parser_could_not_match_string_t : std::runtime_error
{
public:
    __constexpr
    parser_could_not_match_string_t()
        = delete;
    __no_constexpr
    parser_could_not_match_string_t(
        const std::string_view _a_str,
        const std::string_view _a_remaining_string,
        const std::string_view _a_str_failed_to_match
    ) noexcept;
};
_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PARSER_NS
__no_constexpr_imp
    parser_at_end_of_string_exception_t::parser_at_end_of_string_exception_t(
        const std::string_view _a_str,
        const std::size_t      _a_elements_read
    ) noexcept
    : std::runtime_error(fmt::format(
          "Parser at end of string \"{0}\". Parser read {1} characters",
          _a_str,
          _a_elements_read
      ))
{}

__no_constexpr_imp
    parser_could_not_match_string_t::parser_could_not_match_string_t(
        const std::string_view _a_str,
        const std::string_view _a_remaining_string,
        const std::string_view _a_str_failed_to_match
    ) noexcept
    : std::runtime_error(fmt::format(
          "Parser could not find string it expected to find. Remaining string "
          "= \"{0}\", string parser failed to match = \"{1}\", total string = "
          "\"{2}\".",
          _a_remaining_string,
          _a_str_failed_to_match,
          _a_str
      ))
{}

_END_ABC_UTILITY_PARSER_NS