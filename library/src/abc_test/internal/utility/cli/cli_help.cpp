#include "abc_test/internal/utility/cli/cli_help.hpp"

#include "abc_test/internal/utility/cli.hpp"
#include "abc_test/internal/utility/cli/cli_info.hpp"
#include "abc_test/internal/utility/str/string_utility.hpp"

#include <fmt/core.h>
#include <fstream>
#include <optional>

_BEGIN_ABC_NS

__no_constexpr_or_inline_imp bool
    cli_help_t::process_args(
        const std::string_view               _a_flag,
        const std::vector<std::string_view>& _a_args,
        const cli_t&                         _a_cli,
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

__no_constexpr_or_inline_imp bool
    cli_config_t::process_args(
        const std::string_view               _a_flag,
        const std::vector<std::string_view>& _a_args,
        const cli_t&                         _a_cli,
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
                    _a_cli_results.add_error(fmt::format("couldn't run file"));
                    return true;
                }
                else
                {
                    const string _l_field_name{
                        abc::utility::str::remove_whitespace(
                            _l_line.substr(0, _l_equals_pos - 1)
                        )
                    };
                    vector<string> _l_strs = abc::utility::str::split_string(
                        _l_line.substr(_l_equals_pos + 1), " "
                    );
                    if (const bool _l_terminate{_a_cli.process_config_line(
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

__no_constexpr_or_inline_imp bool
    cli_auto_config_t::process_args(
        const std::string_view               _a_flag,
        const std::vector<std::string_view>& _a_args,
        const cli_t&                         _a_cli,
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
        tuple<string, rep_file_index_t, rep_write_data_type_t> _l_rep_data{};
        get<0>(_l_rep_data) = _a_args[0];
        abc::utility::parser::parse_result_t<rep_file_index_t> _l_rfli{
            abc::utility::parser::parse<rep_file_index_t>(_a_args[1])
        };
        abc::utility::parser::parse_result_t<rep_write_data_type_t> _l_rf{
            abc::utility::parser::parse<rep_write_data_type_t>(_a_args[2])
        };
        get<1>(_l_rep_data) = _l_rfli.value();
        get<2>(_l_rep_data) = _l_rf.value();
        if (const bool _l_terminate{
                _a_cli.process_repetition_data(_l_rep_data, _a_cli_results)
            };
            _l_terminate)
        {
            return true;
        }
    }
    return false;
}

_END_ABC_NS