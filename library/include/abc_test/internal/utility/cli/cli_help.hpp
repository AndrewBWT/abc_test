#pragma once
#include "abc_test/internal/utility/cli/cli_info.hpp"

#include <optional>

_BEGIN_ABC_NS
class cli_t;

class cli_help_t : public cli_info_t
{
public:
    __constexpr
    cli_help_t(
        const std::string_view     _a_flag      = "help",
        const std::optional<char>& _a_char_flag = std::make_optional('h')
    )
        : cli_info_t(
              enum_n_arguments::ZERO,
              _a_flag,
              "Prints this message, terminates argument processing, and "
              "signals to the output that processing should not "
              "continue.",
              _a_char_flag,
              false
          )
    {}

    __no_constexpr_or_inline virtual void
        process_args(
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t&                         _a_cli,
            cli_output_t&                        _a_cli_output
        ) const noexcept;
};

class cli_config_t : public cli_info_t
{
public:
    __constexpr
    cli_config_t(
        const std::string_view     _a_flag      = "config",
        const std::optional<char>& _a_char_flag = std::make_optional('c')
    )
        : cli_info_t(
              enum_n_arguments::ONE,
              _a_flag,
              "Specifies a configuration file for the command line parser to "
              "read from.",
              _a_char_flag,
              false
          )
    {}

    __no_constexpr_or_inline virtual void
        process_args(
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t&                         _a_cli,
            cli_output_t&                        _a_cli_output
        ) const noexcept;
};

class cli_auto_config_t : public cli_info_t
{
public:
    __constexpr
        cli_auto_config_t(
            const std::string_view     _a_flag = "auto_configuration",
            const std::optional<char>& _a_char_flag = {}
        )
        : cli_info_t(
            enum_n_arguments::MULTI_ARGUMENT,
            _a_flag,
            "Denotes this run as automatically configured. Requires exactly "
            "three arguments; the first must be a folder location. The second "
            "is either a positive integer or the strings latest or "
            "latest_if_failure. The third is any of the strings always_write, "
            "write_if_different_or_new_run_that_failed or do_not_write. Using this "
            "option will "
            "read "
            "memoized configurations from the folder given. These are "
            "configurations indexed by integer, with an additional bool "
            "denoting whether the test was a success or failure. If the second "
            "argument is an integer, that configuration is found and loaded (if "
            "it exists). If it is the string \"latest\", it will run the most "
            "recent configuration. If it is the string \"latest_if_failure\", it "
            "will only load the configuration if the previous test was a failure. "
            "Otherwise it will will not load any configuration. The third argument "
            "denotes whether to write data after the test is run to the folder. If "
            "the string is \"always_write\", the data is always written. If the "
            "string is \"\", the data is only written if it was loaded and "
            "overridden by the user, or no configuration was automatically loaded "
            "and instead a clean run was performd, which failed. If the string is "
            "\"do_not_write\", the configuration is never written to any files. ",
            _a_char_flag,
            false
        )
    {
    }

    __no_constexpr_or_inline virtual void
        process_args(
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t& _a_cli,
            cli_output_t& _a_cli_output
        ) const noexcept;
};

_END_ABC_NS