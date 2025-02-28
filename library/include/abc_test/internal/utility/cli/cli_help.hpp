#pragma once
#include "abc_test/internal/utility/cli.hpp"
#include "abc_test/internal/utility/cli/cli_info.hpp"
#include "abc_test/internal/utility/str/string_utility.hpp"

#include <fmt/core.h>
#include <fstream>
#include <optional>

_BEGIN_ABC_NS
template <typename Option_Class>
class cli_t;

template <typename Option_Class>
class cli_help_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_help_t(
        const std::string_view     _a_flag      = "help",
        const std::optional<char>& _a_char_flag = std::make_optional('h')
    )
        : cli_info_t<Option_Class>(
              enum_n_arguments::ZERO,
              _a_flag,
              "Prints this message, terminates argument processing, and "
              "signals to the output that processing should not "
              "continue.",
              _a_char_flag,
              false
          )
    {}

    __constexpr virtual bool
        process_args(
            Option_Class&                        _a_option_class,
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t<Option_Class>&           _a_cli,
            cli_results_t&                       _a_cli_results
        ) const noexcept
    {
        using namespace std;
        if (_a_args.size() != 0)
        {
            _a_cli_results.add_error(fmt::format(
                "process_args expected no arguments, however the vector "
                "provided was "
                "in the form ."
            ));
            return true;
        }
        else
        {
            _a_cli_results.add_table_output(_a_cli.make_help_output());
            return false;
        }
    }
};

template <typename Option_Class>
class cli_config_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_config_t(
        const std::string_view     _a_flag      = "config",
        const std::optional<char>& _a_char_flag = std::make_optional('c')
    )
        : cli_info_t<Option_Class>(
              enum_n_arguments::ONE,
              _a_flag,
              "Specifies a configuration file for the command line parser to "
              "read from.",
              _a_char_flag,
              false
          )
    {}

    __constexpr virtual bool
        process_args(
            Option_Class&                        _a_option_class,
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t<Option_Class>&           _a_cli,
            cli_results_t&                       _a_cli_results
        ) const noexcept
    {
        using namespace std;
        // _a_cli.set_terminate_early(true);
        if (_a_args.size() != 1)
        {
            _a_cli_results.add_error(fmt::format(
                "process_args expected one argument, however the vector "
                "provided was "
                "in the form ."
            ));
            return true;
        }
        else
        {
            // Read the file.
            // Go thorugh the args and
            fstream _l_config_file{string(_a_args[0])};
            size_t  _l_line_idx{0};
            if (_l_config_file.is_open())
            {
                string _l_line;
                while (getline(_l_config_file, _l_line))
                {
                    ++_l_line_idx;
                    // Split string into parts. Then process.
                    if (_l_line.size() == 0 || _l_line[0] == '#')
                    {
                        continue;
                    }
                    const size_t _l_equals_pos{_l_line.find_first_of("=")};
                    if (_l_equals_pos == string::npos)
                    {
                        _l_config_file.close();
                        _a_cli_results.add_error(fmt::format("couldn't run file"
                        ));
                        return true;
                    }
                    else
                    {
                        const string _l_field_name{
                            abc::utility::str::remove_whitespace(
                                _l_line.substr(0, _l_equals_pos - 1)
                            )
                        };
                        vector<string> _l_strs
                            = abc::utility::str::split_string(
                                _l_line.substr(_l_equals_pos + 1), " "
                            );
                        if (const bool _l_terminate{_a_cli.process_config_line(
                                _a_option_class,
                                _l_field_name,
                                _l_strs,
                                fmt::format(
                                    "config file \"{0}\", line {1}",
                                    _a_args[0],
                                    _l_line_idx
                                ),
                                _a_cli_results
                            )};
                            _l_terminate)
                        {
                            return true;
                        }
                    }
                }
                return false;
            }
            else
            {
                _a_cli_results.add_error(fmt::format("couldn't run file"));
                return true;
            }
        }
    }
};

template <typename Option_Class>
class cli_auto_config_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_auto_config_t(
        const std::string_view     _a_flag      = "auto_configuration",
        const std::optional<char>& _a_char_flag = {}
    )
        : cli_info_t<Option_Class>(
              enum_n_arguments::MULTI_ARGUMENT,
              _a_flag,
              "Denotes this run as automatically configured. Requires exactly "
              "three arguments; the first must be a folder location. The "
              "second "
              "is either a positive integer or the strings latest or "
              "latest_if_failure. The third is any of the strings "
              "always_write, "
              "write_if_different_or_new_run_that_failed or do_not_write. "
              "Using this "
              "option will "
              "read "
              "memoized configurations from the folder given. These are "
              "configurations indexed by integer, with an additional bool "
              "denoting whether the test was a success or failure. If the "
              "second "
              "argument is an integer, that configuration is found and loaded "
              "(if "
              "it exists). If it is the string \"latest\", it will run the "
              "most "
              "recent configuration. If it is the string "
              "\"latest_if_failure\", it "
              "will only load the configuration if the previous test was a "
              "failure. "
              "Otherwise it will will not load any configuration. The third "
              "argument "
              "denotes whether to write data after the test is run to the "
              "folder. If "
              "the string is \"always_write\", the data is always written. If "
              "the "
              "string is \"\", the data is only written if it was loaded and "
              "overridden by the user, or no configuration was automatically "
              "loaded "
              "and instead a clean run was performd, which failed. If the "
              "string is "
              "\"do_not_write\", the configuration is never written to any "
              "files. ",
              _a_char_flag,
              false
          )
    {}

    __constexpr virtual bool
        process_args(
            Option_Class&                        _a_option_class,
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t<Option_Class>&           _a_cli,
            cli_results_t&                       _a_cli_results
        ) const noexcept
    {
        using namespace std;
        if (_a_args.size() != 3)
        {
            _a_cli_results.add_error(fmt::format(
                "process_args expected three argument, however the vector "
                "provided was "
                "in the form ."
            ));
            return true;
        }
        else
        {
            tuple<string, rep_file_index_t, rep_write_data_type_t> _l_rep_data{
            };
            get<0>(_l_rep_data) = _a_args[0];
            abc::utility::parser::parse_result_t<rep_file_index_t> _l_rfli{
                abc::utility::parser::parse<rep_file_index_t>(_a_args[1])
            };
            abc::utility::parser::parse_result_t<rep_write_data_type_t> _l_rf{
                abc::utility::parser::parse<rep_write_data_type_t>(_a_args[2])
            };
            get<1>(_l_rep_data) = _l_rfli.value();
            get<2>(_l_rep_data) = _l_rf.value();
            if (const bool _l_terminate{_a_cli.process_repetition_data(
                    _a_option_class, _l_rep_data, _a_cli_results
                )};
                _l_terminate)
            {
                return true;
            }
        }
        return false;
    }
};

_END_ABC_NS