#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"
#include <string>
#include <optional>

_BEGIN_ABC_NS
struct cli_option_config_t
{
    std::string_view flag;
    std::string_view description;
    std::optional<char> single_char_flag;
};
_END_ABC_NS