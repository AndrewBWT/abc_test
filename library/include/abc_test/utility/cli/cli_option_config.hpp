#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <optional>
#include <string>

_BEGIN_ABC_UTILITY_CLI_NS

/*!
 * @brief Struct which contains fields for a cli_info_t item.
 */
struct cli_option_config_t
{
    /*!
     * @brief Contains the flag used to identify this option. Note that the user
     * should not include any delimiters at the start.
     */
    std::string_view flag;
    /*!
     * @brief Contains the description of the option. Used when printing out the
     * help message.
     */
    std::string_view description;
    /*!
     * @brief Contains the (optional) short-flag, based around a single
     * character.
     */
    std::optional<char> single_char_flag;
};

_END_ABC_UTILITY_CLI_NS