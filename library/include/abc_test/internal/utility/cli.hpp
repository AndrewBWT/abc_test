#pragma once

#include "abc_test/internal/ds/test_data/pre_test_run_report.hpp"
#include "abc_test/internal/utility/cli/cli_auto_configuration.hpp"
#include "abc_test/internal/utility/cli/cli_help.hpp"
#include "abc_test/internal/utility/cli/cli_info.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/utility/parsers/specializations/fundamental_types.hpp"
#include "abc_test/internal/utility/parsers/specializations/stl_11.hpp"
#include "abc_test/internal/utility/parsers/specializations/stl_98.hpp"
#include "abc_test/internal/utility/printers/default_printer.hpp"
#include "abc_test/internal/utility/str/string_table.hpp"
#include "abc_test/internal/utility/str/string_utility.hpp"

#include <expected>
#include <fmt/ranges.h>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>

_BEGIN_ABC_NS
class cli_t;
using clp_parser_func_t
    = std::function<std::optional<std::string>(const std::string_view)>;

class cli_t
{
public:
    __no_constexpr
        cli_t(
            const char             _a_single_char_cml_identifier,
            const std::string_view _a_multi_char_cml_identifier
        ) noexcept;
    __no_constexpr void
        parse_arguments(
            int            _a_argc,
            char**         _a_arg_v,
            cli_results_t& _a_cli_results
        ) const noexcept;
    __no_constexpr void
        parse_arguments(
            const std::vector<std::string_view>& _a_args,
            cli_results_t&                       _a_cli_results
        ) const noexcept;
    template <typename T, typename U>
    __no_constexpr void
        add_option(
            const std::string_view     _a_flag,
            const std::string_view     _a_description,
            T&                         _a_reference,
            const std::optional<char>& _a_single_char_flag,
            cli_results_t&             _a_cli_results,
            std::function<std::optional<U>(const std::string_view)>
                _a_parser_func
            = detail::make_parser_func<U>(),
            std::function<bool(T&, const U&)> _m_process_parsed_value
            = detail::process_value<T, U>()
        ) noexcept;
    template <typename T, typename U>
    __no_constexpr void
        add_multi_element_option(
            const std::string_view     _a_flag,
            const std::string_view     _a_description,
            T&                         _a_reference,
            const std::optional<char>& _a_single_char_flag,
            cli_results_t&             _a_cli_results,
            std::function<std::optional<U>(const std::string_view)>
                _a_parser_func
            = detail::make_parser_func<U>(),
            std::function<bool(T&, const U&)> _m_process_parsed_value
            = detail::process_value<T, U>()
        ) noexcept;
    __no_constexpr void
        add_help_flag() noexcept;
    __no_constexpr void
        add_file_config_flag() noexcept;
    __no_constexpr void
                   add_auto_configuration() noexcept;
    __no_constexpr abc::utility::str::string_table_t
                   make_help_output() const noexcept;
    __constexpr bool
        process_config_line(
            const std::string_view          _a_field_name,
            const std::vector<std::string>& _a_args,
            const std::string_view          _a_source_line,
            cli_results_t&                  _a_cli_results
        ) const noexcept;
    __no_constexpr bool
        process_repetition_data(
            const std::tuple<
                std::filesystem::path,
                rep_file_index_t,
                rep_write_data_type_t>& _a_tuple_data,
            cli_results_t&              _a_cli_results
        ) const noexcept;
    __constexpr const std::optional<cli_auto_configuration_t>&
        auto_configuration() const noexcept;
private:
    __no_constexpr bool
        load_data(
            cli_results_t& _a_cli_results,
            const std::tuple<bool, size_t, std::vector<std::string>>& _l_data,
            const std::filesystem::path&                              _a_path
        ) const noexcept;
    mutable std::optional<cli_auto_configuration_t> _m_rep_data;
    std::vector<std::shared_ptr<cli_info_t>>        _m_clp_info;
    std::map<char, std::string>                     _m_char_to_clp_info;
    std::map<std::string_view, std::reference_wrapper<cli_info_t>>
                _m_sv_to_clp_info;
    __constexpr std::expected<std::string_view, std::string>
                normalise_str_from_cli_to_flag(const std::string_view _a_str
                ) const noexcept;
    __constexpr std::expected<std::string_view, std::string>
                normalise_str_from_config_to_flag(const std::string_view _a_str
                ) const noexcept;
    __constexpr const std::optional<std::reference_wrapper<cli_info_t>>
        find_clp_info(const std::string_view& _a_info) const noexcept;
    __constexpr bool
        process_args(
            const std::string_view                 _a_flag,
            const cli_info_t&                      _a_cli_info,
            const std::vector<std::string_view>&   _a_strs,
            const std::size_t                      _a_strs_size,
            std::size_t&                           _a_current_index,
            const std::optional<std::string_view>& _a_source,
            cli_results_t&                         _a_cli_results
        ) const noexcept;
    char        _m_single_char_identifier = '-';
    std::string _m_multi_char_identifier  = "--";
    __no_constexpr void
        inner_add_option(const std::shared_ptr<cli_info_t> _a_cli) noexcept;
};

namespace detail
{
__constexpr std::vector<std::string_view>
            make_strs_from_command_line_args(const int _a_argc, char** _a_argv)
        noexcept;

__constexpr
    std::optional<std::tuple<bool, std::size_t, std::vector<std::string>>>
    find_data(
        const std::filesystem::path&      _a_path,
        const std::optional<std::size_t>& _a_index
        = std::optional<std::size_t>{}
    ) noexcept
{
    using namespace std;
    fstream _l_file_input(_a_path);
    string  _l_line;
    optional<std::tuple<bool, std::size_t, std::vector<std::string>>> _l_rv;
    string           _l_str_to_find{"metadata_"};
    optional<string> _l_unprocessed_line;
    while (_l_unprocessed_line.has_value() || getline(_l_file_input, _l_line))
    {
        string _l_line_to_get = _l_unprocessed_line.has_value()
                                    ? _l_unprocessed_line.value()
                                    : _l_line;
        if (_l_unprocessed_line.has_value())
        {
            _l_unprocessed_line = nullopt;
        }
        if (_l_line_to_get.starts_with(_l_str_to_find))
        {
            auto _l_res = utility::parser::parse<pair<bool, size_t>>(
                _l_line_to_get.substr(_l_str_to_find.size())
            );
            if (_l_res.has_value())
            {
                if (_a_index.has_value())
                {
                    if (_a_index.value() == _l_res.value().second)
                    {
                        _l_rv = tuple<bool, size_t, vector<string>>{};
                        get<1>(_l_rv.value()) = _l_res.value().second;
                        get<0>(_l_rv.value()) = _l_res.value().first;
                        while (getline(_l_file_input, _l_line))
                        {
                            if (_l_line.starts_with(_l_str_to_find))
                            {
                                return _l_rv;
                            }
                            else
                            {
                                get<2>(_l_rv.value()).push_back(_l_line);
                            }
                        }
                    }
                }
                else
                {
                    if (_l_rv.has_value())
                    {
                        if (get<1>(_l_rv.value()) < _l_res.value().second)
                        {
                            get<1>(_l_rv.value()) = _l_res.value().second;
                            get<0>(_l_rv.value()) = _l_res.value().first;
                            while (getline(_l_file_input, _l_line))
                            {
                                if (_l_line.starts_with(_l_str_to_find))
                                {
                                    _l_unprocessed_line = _l_line;
                                    break;
                                }
                                else
                                {
                                    get<2>(_l_rv.value()).push_back(_l_line);
                                }
                            }
                        }
                    }
                    else
                    {
                        _l_rv = tuple<bool, size_t, vector<string>>{};
                        get<1>(_l_rv.value()) = _l_res.value().second;
                        get<0>(_l_rv.value()) = _l_res.value().first;
                        while (getline(_l_file_input, _l_line))
                        {
                            if (_l_line.starts_with(_l_str_to_find))
                            {
                                _l_unprocessed_line = _l_line;
                                break;
                            }
                            else
                            {
                                get<2>(_l_rv.value()).push_back(_l_line);
                            }
                        }
                    }
                }
            }
        }
    }
    return _l_rv;
    // Carry on reading until either EOF or another metadata line.
}
} // namespace detail

_END_ABC_NS

_BEGIN_ABC_NS
__no_constexpr_imp
    cli_t::cli_t(
        const char             _a_single_char_cml_identifier = '-',
        const std::string_view _a_multi_char_cml_identifier  = "--"
    ) noexcept
    : _m_single_char_identifier(_a_single_char_cml_identifier)
    , _m_multi_char_identifier(_a_multi_char_cml_identifier)
{}

__no_constexpr_imp void
    cli_t::parse_arguments(
        int            _a_argc,
        char**         _a_arg_v,
        cli_results_t& _a_cli_results
    ) const noexcept
{
    parse_arguments(
        detail::make_strs_from_command_line_args(_a_argc, _a_arg_v),
        _a_cli_results
    );
}

__no_constexpr_imp void
    cli_t::parse_arguments(
        const std::vector<std::string_view>& _a_args,
        cli_results_t&                       _a_cli_results
    ) const noexcept
{
    using namespace std;
    const size_t _l_strs_size{_a_args.size()};
    for (size_t _l_idx{0}; _l_idx < _l_strs_size; ++_l_idx)
    {
        const string_view _l_str{_a_args[_l_idx]};
        if (const expected<string_view, string> _l_opt_normalised_str{
                normalise_str_from_cli_to_flag(_l_str)
            };
            _l_opt_normalised_str.has_value())
        {
            if (const optional<reference_wrapper<cli_info_t>> _l_clp_info{
                    find_clp_info(_l_opt_normalised_str.value())
                };
                _l_clp_info.has_value())
            {
                if (const bool _l_terminate{process_args(
                        _l_opt_normalised_str.value(),
                        _l_clp_info.value().get(),
                        _a_args,
                        _l_strs_size,
                        _l_idx,
                        optional<string_view>{},
                        _a_cli_results
                    )};
                    _l_terminate)
                {
                    break;
                }
            }
        }
        else
        {
            _a_cli_results.add_error(_l_opt_normalised_str.error());
            return;
        }
    }
}

template <typename T, typename U>
__no_constexpr_imp void
    cli_t::add_option(
        const std::string_view     _a_flag,
        const std::string_view     _a_description,
        T&                         _a_reference,
        const std::optional<char>& _a_single_char_flag,
        cli_results_t&             _a_cli_results,
        std::function<std::optional<U>(const std::string_view)> _a_parser_func,
        std::function<bool(T&, const U&)> _m_process_parsed_value

    ) noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_one_arg_t<T, U>>(
        _a_flag,
        _a_description,
        _a_reference,
        _a_single_char_flag,
        _a_parser_func,
        _m_process_parsed_value
    ));
    _a_cli_results.add_memoized_data(
        false,
        _a_flag,
        abc::utility::printer::default_printer_t<T>().run_printer(_a_reference),
        "default value"
    );
}

template <typename T, typename U>
__no_constexpr_imp void
    cli_t::add_multi_element_option(
        const std::string_view     _a_flag,
        const std::string_view     _a_description,
        T&                         _a_reference,
        const std::optional<char>& _a_single_char_flag,
        cli_results_t&             _a_cli_results,
        std::function<std::optional<U>(const std::string_view)> _a_parser_func,
        std::function<bool(T&, const U&)> _m_process_parsed_value

    ) noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_multi_args<T, U>>(
        _a_flag,
        _a_description,
        _a_reference,
        _a_single_char_flag,
        _a_parser_func,
        _m_process_parsed_value
    ));
    _a_cli_results.add_memoized_data(
        false,
        _a_flag,
        abc::utility::printer::default_printer_t<T>().run_printer(_a_reference),
        "default value"
    );
}

__no_constexpr_imp void
    cli_t::add_help_flag() noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_help_t>());
}

__no_constexpr_imp void
    cli_t::add_file_config_flag() noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_config_t>());
}

__no_constexpr_imp void
    cli_t::add_auto_configuration() noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_auto_config_t>());
}

__no_constexpr_imp abc::utility::str::string_table_t
                   cli_t::make_help_output() const noexcept
{
    using namespace abc::utility::str;
    string_table_t _l_st({0, 1});
    _l_st.push_back("Flag(s):");
    _l_st.push_back("    ");
    _l_st.push_back("Description:");
    _l_st.new_line();
    for (const std::shared_ptr<cli_info_t>& _l_flag : _m_clp_info)
    {
        const cli_info_t& _l_cli_info{*_l_flag.get()};
        _l_st.push_back(fmt::format(
            "{0}{1}{2}",
            _m_multi_char_identifier,
            _l_cli_info.flag(),
            (_l_cli_info.char_flag().has_value()
                 ? fmt::format(
                       ", {0}{1}",
                       _m_single_char_identifier,
                       _l_cli_info.char_flag().value()
                   )
                 : "")
        ));
        _l_st.push_empty();
        _l_st.push_back(fmt::format(
            "{0}{1}",
            _l_cli_info.description(),
            not _l_cli_info.enabled_for_config_file()
                ? " Option will not be recognised in a configuration file."
                : ""
        ));
        _l_st.new_line();
    }
    return _l_st;
}

__constexpr_imp bool
    cli_t::process_config_line(
        const std::string_view          _a_field_name,
        const std::vector<std::string>& _a_args,
        const std::string_view          _a_source_line,
        cli_results_t&                  _a_cli_results
    ) const noexcept
{
    using namespace std;
    if (const expected<string_view, string> _l_opt_normalised_str{
            normalise_str_from_config_to_flag(_a_field_name)
        };
        _l_opt_normalised_str.has_value())
    {
        if (const optional<reference_wrapper<cli_info_t>> _l_clp_info{
                find_clp_info(_l_opt_normalised_str.value())
            };
            _l_clp_info.has_value())
        {
            size_t              _l_idx{0};
            vector<string_view> _l_strs;
            for (auto& _l_arg : _a_args)
            {
                _l_strs.push_back(_l_arg);
            }
            if (bool _l_terminate{process_args(
                    _l_opt_normalised_str.value(),
                    _l_clp_info.value().get(),
                    _l_strs,
                    _l_strs.size(),
                    _l_idx,
                    _a_source_line,
                    _a_cli_results
                )};
                _l_terminate)
            {
                return true;
            }
        }
    }
    else
    {
        _a_cli_results.add_error(_l_opt_normalised_str.error());
        return true;
    }
    return false;
}

__no_constexpr_imp bool
    cli_t::process_repetition_data(
        const std::tuple<
            std::filesystem::path,
            rep_file_index_t,
            rep_write_data_type_t>& _a_tuple_data,
        cli_results_t&              _a_cli_results
    ) const noexcept
{
    using namespace std;
    // Now time to load the data.
    const auto& [_l_folder, _l_rep_file_idx, _l_rep_write_data_type]{
        _a_tuple_data
    };
    // Check that the file stuff exists. Create folder if it doesn't.
    if (not filesystem::exists(_l_folder)
        || not filesystem::is_directory(_l_folder))
    {
        auto f1 = _l_folder;
        if (not filesystem::exists(_l_folder.parent_path()))
        {
            _a_cli_results.add_error(fmt::format(
                "Parent folder {0} does not exist",
                _l_folder.parent_path().string()
            ));
            return true;
        }
        filesystem::create_directory(_l_folder);
    }
    const string auto_repetition_file_name = "autofile";
    // This is the file containing the index we will run.
    optional<cli_auto_configuration_file_info_t>
        _l_file_containing_index_to_run;
    // This is the file containing the last known file.
    optional<cli_auto_configuration_file_info_t> _l_last_file;
    for (const filesystem::path& _l_file :
         filesystem::directory_iterator(_l_folder))
    {
        const string _l_file_name{_l_file.filename().string()};
        if (auto _l_pos{_l_file_name.find(auto_repetition_file_name)};
            _l_pos != string::npos || _l_pos == 0)
        {
            // The filename begins with the correct string.
            const size_t _l_first_underscore{
                _l_file_name.find_first_of("_", _l_pos)
            };
            if (_l_first_underscore == string::npos)
            {
                continue;
            }
            const size_t _l_second_underscore{
                _l_file_name.find_first_of("_", _l_first_underscore + 1)
            };
            if (_l_second_underscore == string::npos)
            {
                continue;
            }
            const size_t _l_last_info{_l_file_name.find_first_not_of(
                "0123456789", _l_second_underscore + 1
            )};
            if (_l_last_info != string::npos)
            {
                continue;
            }
            // Get the information out of the file name.
            size_t _l_begin_index{0};
            size_t _l_end_index{0};
            try
            {
                _l_begin_index = stoull(_l_file_name.substr(
                    _l_first_underscore + 1,
                    _l_second_underscore - (_l_first_underscore + 1)
                ));
                _l_end_index
                    = stoull(_l_file_name.substr(_l_second_underscore + 1));
            }
            catch (...)
            {
                continue;
            }
            //
            if (_l_rep_file_idx.index().has_value())
            {
                size_t _l_index{_l_rep_file_idx.index().value()};
                if (_l_begin_index <= _l_index && _l_end_index >= _l_index)
                {
                    _l_file_containing_index_to_run.value().file = _l_file;
                    _l_file_containing_index_to_run.value()
                        .min_and_max_configuration_indexes
                        = make_pair(_l_begin_index, _l_end_index);
                }
            }
            if (_l_last_file.has_value())
            {
                if (_l_last_file.value()
                        .min_and_max_configuration_indexes.second
                    < _l_end_index)
                {
                    _l_last_file.value().file = _l_file;
                    _l_last_file.value().min_and_max_configuration_indexes
                        = make_pair(_l_begin_index, _l_end_index);
                }
            }
            else
            {
                _l_last_file = cli_auto_configuration_file_info_t{};
                _l_last_file.value().file = _l_file;
                _l_last_file.value().min_and_max_configuration_indexes
                    = make_pair(_l_begin_index, _l_end_index);
            }
        }
    }
    // Check that if we're looking for a config index but it doesn't exist.
    if (_l_rep_file_idx.index().has_value()
        && not _l_file_containing_index_to_run.has_value())
    {
        _a_cli_results.add_error(fmt::format(
            "The file containing index {0} was required, however it could not "
            "be found",
            _l_rep_file_idx.index().value()
        ));
        return true;
    }
    // Now time to read the relevant parts of the file
    tuple<bool, size_t, vector<string>>            _l_data;
    optional<cli_auto_configuration_file_last_loaded_info_t> _l_last_loaded_info;
    if (_l_file_containing_index_to_run.has_value())
    {
        if (auto _l_r = detail::find_data(
                _l_file_containing_index_to_run.value().file,
                _l_rep_file_idx.index().value()
            );
            _l_r.has_value())
        {
            _l_data = _l_r.value();
            if (const bool _l_terminated{load_data(
                    _a_cli_results,
                    _l_data,
                    _l_file_containing_index_to_run.value().file
                )};
                _l_terminated)
            {
                return true;
            }
        }
        else
        {
            _a_cli_results.add_error("Couldn't work it out");
            return true;
        }
    }
    // If the last file has an index.
    if (_l_last_file.has_value())
    {
        if (auto _l_r = detail::find_data(_l_last_file.value().file);
            _l_r.has_value())
        {
            _l_data = _l_r.value();
            if (get<0>(_l_data) == false
                && not _l_file_containing_index_to_run.has_value())
            {
                _l_file_containing_index_to_run = _l_last_file;
                if (const bool _l_terminated{load_data(
                        _a_cli_results, _l_data, _l_last_file.value().file
                    )};
                    _l_terminated)
                {
                    return true;
                }
            }
            _l_last_loaded_info = cli_auto_configuration_file_last_loaded_info_t{};
            _l_last_loaded_info.value().file = _l_last_file.value().file;
            _l_last_loaded_info.value().last_index = get<1>(_l_data);
        }
        else
        {
            _a_cli_results.add_error("Couldn't work it out");
            return true;
        }
    }
    _m_rep_data = cli_auto_configuration_t(
        _l_folder,
        _l_rep_file_idx,
        _l_rep_write_data_type,
        _l_file_containing_index_to_run,
        _l_last_loaded_info
    );
    return false;
}
__constexpr_imp const std::optional<cli_auto_configuration_t>&
cli_t::auto_configuration() const noexcept
{
    return _m_rep_data;
}
__no_constexpr_imp bool
    cli_t::load_data(
        cli_results_t& _a_cli_results,
        const std::tuple<bool, size_t, std::vector<std::string>>& _l_data,
        const std::filesystem::path&                              _a_path
    ) const noexcept
{
    using namespace std;
    size_t _l_line_idx{1};
    for (const string_view _l_line : get<2>(_l_data))
    {
        // Split string into parts. Then process.
        if (_l_line.size() == 0 || _l_line[0] == '#')
        {
            continue;
        }
        const size_t _l_equals_pos{_l_line.find_first_of("=")};
        if (_l_equals_pos == string::npos)
        {
            _a_cli_results.add_error(fmt::format("couldn't run file"));
            return true;
        }
        else
        {
            const string   _l_field_name{abc::utility::str::remove_whitespace(
                _l_line.substr(0, _l_equals_pos - 1)
            )};
            vector<string> _l_strs = abc::utility::str::split_string(
                _l_line.substr(_l_equals_pos + 1), " "
            );
            if (bool _l_terminate{process_config_line(
                    _l_field_name,
                    _l_strs,
                    fmt::format(
                        "memoized configuration {0} from file \"{1}\", line "
                        "{2}",
                        get<1>(_l_data),
                        _a_path,
                        _l_line_idx
                    ),
                    _a_cli_results
                )};
                _l_terminate)
            {
                return true;
            }
        }
        ++_l_line_idx;
    }
    return false;
}

__constexpr_imp std::expected<std::string_view, std::string>
                cli_t::normalise_str_from_cli_to_flag(
        const std::string_view _a_str
    ) const noexcept
{
    using namespace std;
    if (_a_str.size() == 0)
    {
        return unexpected(
            "Could not normalise string to flag, as string is empty."
        );
    }
    else if (_a_str.size() > _m_multi_char_identifier.size()
             && _a_str.substr(0, _m_multi_char_identifier.size())
                    == _m_multi_char_identifier)
    {
        auto _l_str{_a_str.substr(_m_multi_char_identifier.size())};
        if (auto _l_finder{_m_sv_to_clp_info.find(_l_str)};
            _l_finder != _m_sv_to_clp_info.end())
        {
            return _l_finder->first;
        }
        else
        {
            return unexpected(fmt::format(
                "Could not find multi-character option \"{0}\".", _l_str
            ));
        }
    }
    else if (_a_str.size() == 2 && _a_str[0] == _m_single_char_identifier)
    {
        if (auto _l_finder{_m_char_to_clp_info.find(_a_str[1])};
            _l_finder != _m_char_to_clp_info.end())
        {
            return _l_finder->second;
        }
        else
        {
            return unexpected(fmt::format(
                "Could not find single character option '{0}'.", _a_str[1]
            ));
        }
    }
    else
    {
        return unexpected(fmt::format(
            "Could not match string to flag. String given as \"{0}\". "
            "Multi-line identifier is \"{1}\", and single line identifier "
            "is "
            "`{2}`.",
            _a_str,
            _m_multi_char_identifier,
            _m_single_char_identifier
        ));
    }
}

__constexpr_imp std::expected<std::string_view, std::string>
                cli_t::normalise_str_from_config_to_flag(
        const std::string_view _a_str
    ) const noexcept
{
    using namespace std;
    if (_a_str.size() == 0)
    {
        return unexpected(
            "Could not normalise string to flag, as string is empty."
        );
    }
    else if (auto _l_finder{_m_sv_to_clp_info.find(_a_str)};
             _l_finder != _m_sv_to_clp_info.end())
    {
        return _l_finder->first;
    }
    else
    {
        return unexpected(fmt::format(
            "Could not match string to flag. String given as \"{0}\". ", _a_str
        ));
    }
}

__constexpr_imp const std::optional<std::reference_wrapper<cli_info_t>>
                      cli_t::find_clp_info(
        const std::string_view& _a_info
    ) const noexcept
{
    using namespace std;
    return _m_sv_to_clp_info.at(_a_info);
}

__constexpr_imp bool
    cli_t::process_args(
        const std::string_view                 _a_flag,
        const cli_info_t&                      _a_cli_info,
        const std::vector<std::string_view>&   _a_strs,
        const std::size_t                      _a_strs_size,
        std::size_t&                           _a_current_index,
        const std::optional<std::string_view>& _a_source,
        cli_results_t&                         _a_cli_results
    ) const noexcept
{
    using namespace std;
    using enum enum_n_arguments;
    switch (_a_cli_info.n_arguments())
    {
    case ZERO:
    {
        if (const bool _l_terminate{
                _a_cli_info.process_args(_a_flag, {}, *this, _a_cli_results)
            };
            _l_terminate)
        {
            return true;
        }
        if (auto _l_opt_print(_a_cli_info.print()); _l_opt_print.has_value())
        {
            _a_cli_results.add_memoized_data(
                false,
                _a_flag,
                _l_opt_print.value(),
                (_a_source.has_value()
                     ? _a_source.value()
                     : fmt::format(
                           "command line argument {0}", _a_current_index
                       ))
            );
        }
    }
        return false;
    case ONE:
    {
        if (expected<string_view, string> _l_next_arg{
                normalise_str_from_cli_to_flag(_a_strs[++_a_current_index])
            };
            not _l_next_arg.has_value())
        {
            if (const bool _l_terminate{_a_cli_info.process_args(
                    _a_flag, {_a_strs[_a_current_index]}, *this, _a_cli_results
                )};
                _l_terminate)
            {
                return true;
            }
            if (auto _l_opt_print(_a_cli_info.print());
                _l_opt_print.has_value())
            {
                _a_cli_results.add_memoized_data(
                    false,
                    _a_flag,
                    _l_opt_print.value(),
                    (_a_source.has_value()
                         ? _a_source.value()
                         : fmt::format(
                               "command line arguments {0} and {1}",
                               _a_current_index - 1,
                               _a_current_index
                           ))
                );
            }
        }
        else
        {
            _a_cli_results.add_error(
                fmt::format("Expected a single argument, however the argument "
                            "found is a flag.")
            );
            return true;
        }
    }
    break;
    default:
        // Could be an unlimited number.
        {
            size_t _l_index_of_flag{_a_current_index};
            ++_a_current_index;
            vector<string_view> _l_strs_to_pass;
            while (_a_current_index < _a_strs.size())
            {
                expected<string_view, string> _l_next_arg
                    = normalise_str_from_cli_to_flag(_a_strs[_a_current_index]);
                if (_l_next_arg.has_value())
                {
                    break;
                }
                else
                {
                    _l_strs_to_pass.push_back(_a_strs[_a_current_index++]);
                }
            }
            --_a_current_index;
            if (_a_cli_info.min_arguments().has_value()
                && _a_cli_info.min_arguments().value() > _l_strs_to_pass.size())
            {
                _a_cli_results.add_error(fmt::format(
                    "Flag requires at least {0} arguments, however only "
                    "{1} "
                    "was able be parsed. Arguments successfully parsed =.",
                    _a_cli_info.min_arguments().value(),
                    _l_strs_to_pass.size()
                ));
                return true;
            }
            else if (_a_cli_info.max_arguments().has_value()
                     && _a_cli_info.max_arguments().value()
                            < _l_strs_to_pass.size())
            {
                _a_cli_results.add_error(fmt::format(
                    "Flag requires at most {0} arguments, however {1} "
                    "were able be parsed. Arguments successfully parsed.",
                    _a_cli_info.min_arguments().value(),
                    _l_strs_to_pass.size()
                ));
                return true;
            }
            if (const bool _l_terminate{_a_cli_info.process_args(
                    _a_flag, _l_strs_to_pass, *this, _a_cli_results
                )};
                _l_terminate)
            {
                return true;
            }
            if (auto _l_opt_print(_a_cli_info.print());
                _l_opt_print.has_value())
            {
                _a_cli_results.add_memoized_data(
                    true,
                    _a_flag,
                    _l_opt_print.value(),
                    (_a_source.has_value()
                         ? _a_source.value()
                         : fmt::format(
                               "command line arguments {0} to {1}",
                               _l_index_of_flag,
                               _a_current_index
                           ))
                );
            }
        }
    }
    return false;
}

__no_constexpr_imp void
    cli_t::inner_add_option(
        const std::shared_ptr<cli_info_t> _a_cli
    ) noexcept
{
    using namespace std;
    if (_a_cli->char_flag().has_value())
    {
        _m_char_to_clp_info.insert(
            {_a_cli->char_flag().value(), string(_a_cli->flag())}
        );
    }
    _m_clp_info.push_back(_a_cli);
    _m_sv_to_clp_info.insert({_a_cli->flag(), reference_wrapper(*_a_cli.get())}
    );
}

namespace detail
{
__constexpr_imp std::vector<std::string_view>
                make_strs_from_command_line_args(
                    const int _a_argc,
                    char**    _a_argv
                ) noexcept
{
    using namespace std;
    vector<string_view> _l_strs(_a_argc > 1 ? (_a_argc - 1) : 0);
    for (size_t _l_idx{1}; _l_idx < _a_argc; ++_l_idx)
    {
        _l_strs[_l_idx - 1] = string_view(_a_argv[_l_idx]);
    }
    return _l_strs;
}
} // namespace detail

_END_ABC_NS
