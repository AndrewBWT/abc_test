#pragma once
#include "abc_test/utility/cli.hpp"
#include "abc_test/utility/cli/cli_info.hpp"
#include "abc_test/utility/str/string_utils.hpp"

#include <fmt/core.h>
#include <fstream>
#include <optional>

_BEGIN_ABC_UTILITY_CLI_NS
template <typename Option_Class>
class cli_t;

template <typename Option_Class>
class cli_help_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_help_t(
        const std::u8string_view      _a_flag      = u8"help",
        const std::optional<char8_t>& _a_char_flag = char8_t('h')
    )
        : cli_info_t<Option_Class>(
              cli_option_config_t{
                  _a_flag,
                  u8"Prints this message, terminates argument processing, and "
                  u8"signals to the output that processing should not "
                  u8"continue.",
                  _a_char_flag
              },
              0,
              0,
              false
          )
    {}

    __constexpr virtual bool
        process_args_(
            Option_Class&                          _a_option_class,
            const std::u8string_view               _a_flag,
            const std::vector<std::u8string_view>& _a_args,
            const cli_t<Option_Class>&             _a_cli,
            cli_results_t&                         _a_cli_results
        ) const noexcept
    {
        _a_cli_results.add_table_output(_a_cli.make_help_output());
        return true;
    }
};

template <typename Option_Class>
class cli_config_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_config_t(
        const std::u8string_view      _a_flag      = u8"config",
        const std::optional<char8_t>& _a_char_flag = char8_t('c')
    )
        : cli_info_t<Option_Class>(
              cli_option_config_t{
                  _a_flag,
                  u8"Specifies a configuration file for the command line "
                  u8"parser "
                  u8"to read from.",
                  _a_char_flag
              },
              1,
              1,
              false
          )
    {}

    __constexpr virtual bool
        process_args_(
            Option_Class&                          _a_option_class,
            const std::u8string_view               _a_flag,
            const std::vector<std::u8string_view>& _a_args,
            const cli_t<Option_Class>&             _a_cli,
            cli_results_t&                         _a_cli_results
        ) const noexcept
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        // _a_cli.set_terminate_early(true);
        // Read the file.
        // Go thorugh the args and
        const result_t<string> _l_result_file_name{
            convert_unicode_to_ascii(_a_args[0])
        };
        if (_l_result_file_name.has_value())
        {
            auto&   _l_file_name{_l_result_file_name.value()};
            fstream _l_config_file{_l_file_name};
            size_t  _l_line_idx{0};
            if (_l_config_file.is_open())
            {
                string _l_unprocessed_line;
                while (getline(_l_config_file, _l_unprocessed_line))
                {
                    ++_l_line_idx;
                    u8string _l_line{cast_string_to_u8string(_l_unprocessed_line
                    )};
                    // Split string into parts. Then process.
                    if (_l_line.size() == 0 || _l_line[0] == char8_t('#'))
                    {
                        continue;
                    }
                    const size_t _l_equals_pos{_l_line.find_first_of(u8"=")};
                    if (_l_equals_pos == string::npos)
                    {
                        _l_config_file.close();
                        _a_cli_results.add_error(
                            fmt::format(u8"couldn't run file 1")
                        );
                        return true;
                    }
                    else
                    {
                        const u8string _l_field_name{
                            abc::utility::str::remove_whitespace(
                                _l_line.substr(0, _l_equals_pos - 1)
                            )
                        };
                        vector<u8string> _l_strs
                            = abc::utility::str::split_string<char8_t>(
                                _l_line.substr(_l_equals_pos + 1), u8" "
                            );
                        if (const bool _l_terminate{_a_cli.process_config_line(
                                _a_option_class,
                                _l_field_name,
                                _l_strs,
                                fmt::format(
                                    u8"config file \"{0}\", line {1}",
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
                _a_cli_results.add_error(
                    fmt::format(u8"Couldn't open file \"{0}\"", _a_args[0])
                );
                return true;
            }
        }
        else
        {
            _a_cli_results.add_error(fmt::format(
                u8"Could not convert file name from std::u8string to "
                u8"std::string. Conversion error was \"{0}\".",
                _l_result_file_name.error()
            ));
            return true;
        }
    }
};

template <typename Option_Class>
class cli_auto_config_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_auto_config_t(
        const std::u8string_view      _a_flag      = u8"auto_configuration",
        const std::optional<char8_t>& _a_char_flag = {}
    )
        : cli_info_t<Option_Class>(
              cli_option_config_t{
                  _a_flag,
                  u8"Denotes this run as automatically configured. Requires "
                  u8"exactly "
                  u8"three arguments; the first must be a folder location. The "
                  u8"second "
                  u8"is either a positive integer or the strings latest or "
                  u8"latest_if_failure. The third is any of the strings "
                  u8"always_write, "
                  u8"write_if_different_or_new_run_that_failed or "
                  u8"do_not_write. "
                  u8"Using this "
                  u8"option will "
                  u8"read "
                  u8"memoized configurations from the folder given. These are "
                  u8"configurations indexed by integer, with an additional "
                  u8"bool "
                  u8"denoting whether the test was a success or failure. If "
                  u8"the "
                  u8"second "
                  u8"argument is an integer, that configuration is found and "
                  u8"loaded "
                  u8"(if "
                  u8"it exists). If it is the string \"latest\", it will run "
                  u8"the "
                  u8"most "
                  u8"recent configuration. If it is the string "
                  u8"\"latest_if_failure\", it "
                  u8"will only load the configuration if the previous test was "
                  u8"a "
                  u8"failure. "
                  u8"Otherwise it will will not load any configuration. The "
                  u8"third "
                  u8"argument "
                  u8"denotes whether to write data after the test is run to "
                  u8"the "
                  u8"folder. If "
                  u8"the string is \"always_write\", the data is always "
                  u8"written. "
                  u8"If "
                  u8"the "
                  u8"string is \"\", the data is only written if it was loaded "
                  u8"and "
                  u8"overridden by the user, or no configuration was "
                  u8"automatically "
                  u8"loaded "
                  u8"and instead a clean run was performd, which failed. If "
                  u8"the "
                  u8"string is "
                  u8"\"do_not_write\", the configuration is never written to "
                  u8"any "
                  u8"files. ",
                  _a_char_flag
              },
              3,
              3,
              false
          )
    {}

    __constexpr virtual bool
        process_args_(
            Option_Class&                          _a_option_class,
            const std::u8string_view               _a_flag,
            const std::vector<std::u8string_view>& _a_args,
            const cli_t<Option_Class>&             _a_cli,
            cli_results_t&                         _a_cli_results
        ) const noexcept
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        using namespace _ABC_NS_UTILITY_PARSER;
        tuple<
            u8string,
            auto_configuration_load_configuration_t,
            enum_auto_configuration_write_to_file_t>
            _l_rep_data{};
        get<0>(_l_rep_data) = _a_args[0];
        parser_result_t<auto_configuration_load_configuration_t> _l_rfli{
            parse<auto_configuration_load_configuration_t>(_a_args[1])
        };
        parser_result_t<enum_auto_configuration_write_to_file_t> _l_rf{
            parse<enum_auto_configuration_write_to_file_t>(
                _a_args[2],
                mk_parser(
                    default_parser_t<enum_auto_configuration_write_to_file_t>(
                        enum_helper_string_type_e::lower
                    )
                )
            )
        };
        if (not _l_rfli.has_value())
        {
            _a_cli_results.add_error(fmt::format(
                u8"Error parsing {0}. Parser reported error: \"{1}\".",
                type_id<auto_configuration_load_configuration_t>(),
                _l_rfli.error().errors
            ));
            return true;
        }
        else if (not _l_rf.has_value())
        {
            _a_cli_results.add_error(fmt::format(
                u8"Error parsing {0}. Parser reported error: \"{1}\".",
                type_id<enum_auto_configuration_write_to_file_t>(),
                _l_rf.error().errors
            ));
            return true;
        }
        else
        {
            get<1>(_l_rep_data) = _l_rfli.value();
            get<2>(_l_rep_data) = _l_rf.value();
            if (const bool _l_terminate{_a_cli.process_repetition_data(
                    _a_option_class, _l_rep_data, _a_cli_results
                )};
                _l_terminate)
            {
                return true;
            }
            return false;
        }
    }
};

_END_ABC_UTILITY_CLI_NS