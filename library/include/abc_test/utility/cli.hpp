#pragma once

#include "abc_test/core/ds/test_data/pre_test_run_report.hpp"
#include "abc_test/utility/cli/cli_auto_configuration.hpp"
#include "abc_test/utility/cli/cli_help.hpp"
#include "abc_test/utility/cli/cli_info.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
#include "abc_test/utility/str/string_table.hpp"
#include "abc_test/utility/str/string_utility.hpp"

#include <expected>
#include <fmt/ranges.h>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>

_BEGIN_ABC_UTILITY_CLI_NS

namespace detail
{
struct found_data_t;
} // namespace detail

template <typename Option_Class>
class cli_t
{
public:
    __no_constexpr
        cli_t(
            const char8_t            _a_single_char_cml_identifier,
            const std::u8string_view _a_multi_char_cml_identifier
        ) noexcept;
    __no_constexpr void
        parse_arguments(
            Option_Class&  _a_option_class,
            int            _a_argc,
            char**         _a_arg_v,
            cli_results_t& _a_cli_results
        ) const noexcept;
    __no_constexpr void
        parse_arguments(
            Option_Class&                          _a_option_class,
            const std::vector<std::u8string>& _a_args,
            cli_results_t&                         _a_cli_results
        ) const noexcept;
    template <typename T, typename U = T>
    __no_constexpr void
        add_option(
            const cli_option_config_t& _a_option,
            T Option_Class::*                           _a_member_var,
            cli_results_t&                              _a_cli_results,
            const cli_argument_processing_info_t<T, U>& _a_cli_processing_funcs
            = cli_argument_processing_info_t<T, U>{}
        ) noexcept;
    template <typename T, typename U>
    __no_constexpr void
        add_multi_element_option(
            const cli_option_config_t& _a_cli_option,
            T Option_Class::*                           _a_member_var,
            cli_results_t&                              _a_cli_results,
            const cli_argument_processing_info_t<T, U>& _a_cli_processing_funcs
            = cli_argument_processing_info_t<T, U>{}
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
            Option_Class&                     _a_option_class,
            const std::u8string_view          _a_field_name,
            const std::vector<std::u8string>& _a_args,
            const std::u8string_view          _a_source_line,
            cli_results_t&                    _a_cli_results
        ) const noexcept;
    __no_constexpr bool
        process_repetition_data(
            Option_Class& _a_option_class,
            const std::tuple<
                std::filesystem::path,
                auto_configuration_load_configuration_t,
                enum_auto_configuration_write_to_file_t>& _a_tuple_data,
            cli_results_t&                                _a_cli_results
        ) const noexcept;
    __constexpr const std::optional<cli_auto_configuration_t>&
                      auto_configuration() const noexcept;
    __no_constexpr void
        setup_next_file(
            const std::string_view _a_autofile_name,
            const std::size_t      _a_autofile_size,
            const Option_Class&    _a_option_class,
            const bool             _a_test_success
        ) const noexcept;
private:
    __no_constexpr bool
        load_data(
            Option_Class&                _a_option_class,
            cli_results_t&               _a_cli_results,
            const detail::found_data_t&  _l_data,
            const std::filesystem::path& _a_path
        ) const noexcept;
    mutable std::optional<cli_auto_configuration_t>        _m_rep_data;
    std::vector<std::shared_ptr<cli_info_t<Option_Class>>> _m_clp_info;
    std::map<char8_t, std::u8string>                       _m_char_to_clp_info;
    std::map<
        std::u8string_view,
        std::reference_wrapper<cli_info_t<Option_Class>>>
                _m_sv_to_clp_info;
    __constexpr result_t<std::u8string_view>
                normalise_str_from_cli_to_flag(const std::u8string_view _a_str
                ) const noexcept;
    __constexpr result_t<std::u8string_view>
        normalise_str_from_config_to_flag(const std::u8string_view _a_str
        ) const noexcept;
    __constexpr const
        std::optional<std::reference_wrapper<cli_info_t<Option_Class>>>
        find_clp_info(const std::u8string_view& _a_info) const noexcept;
    __constexpr bool
        process_args(
            Option_Class&                            _a_option_class,
            const std::u8string_view                 _a_flag,
            const cli_info_t<Option_Class>&          _a_cli_info,
            const std::vector<std::u8string>&   _a_strs,
            const std::size_t                        _a_strs_size,
            std::size_t&                             _a_current_index,
            const std::optional<std::u8string_view>& _a_source,
            cli_results_t&                           _a_cli_results
        ) const noexcept;
    char8_t       _m_single_char_identifier = '-';
    std::u8string _m_multi_char_identifier  = "--";
    __no_constexpr void
        inner_add_option(const std::shared_ptr<cli_info_t<Option_Class>> _a_cli
        ) noexcept;
    __no_constexpr void
        write_file(
            const std::string_view _a_autofile_name,
            const std::size_t      _a_autofile_size,
            const Option_Class&    _a_option_class,
            const bool             _a_test_success
        ) const noexcept;
    __constexpr std::vector<std::pair<std::u8string, std::u8string>>
                get_config_file_data(const Option_Class& _a_option_class
                ) const noexcept;
};

template <typename Option_Class>
__no_constexpr_imp result_t<cli_t<Option_Class>>
                   make_cli(
                       const char8_t            _a_single_char_cli_identifier = char8_t('-'),
                       const std::u8string_view _a_multi_char_cli_identifier = u8"--"
                   ) noexcept
{
    using namespace std;
    return result_t<cli_t<Option_Class>>(cli_t<Option_Class>(
        _a_single_char_cli_identifier, _a_multi_char_cli_identifier
    ));
}

namespace detail
{
__constexpr std::vector<std::u8string>
            make_strs_from_command_line_args(const int _a_argc, char** _a_argv)
        noexcept;

struct found_data_t
{
    bool                       test_passed;
    std::size_t                configuration_index;
    std::vector<std::u8string> configuration_data;

    __no_constexpr             std::optional<std::string>
                               process_lines(
                                   std::fstream&            _a_fstream,
                                   const std::u8string_view _a_str_to_find
                               ) noexcept;
};

__constexpr std::optional<found_data_t>
            find_data(
                const std::filesystem::path&      _a_path,
                const std::optional<std::size_t>& _a_index
                = std::optional<std::size_t>{}
            ) noexcept;
} // namespace detail

_END_ABC_UTILITY_CLI_NS

_BEGIN_ABC_UTILITY_CLI_NS
template <typename Option_Class>
__no_constexpr_imp
    cli_t<Option_Class>::cli_t(
        const char8_t            _a_single_char_cml_identifier,
        const std::u8string_view _a_multi_char_cml_identifier
    ) noexcept
    : _m_single_char_identifier(_a_single_char_cml_identifier)
    , _m_multi_char_identifier(_a_multi_char_cml_identifier)
{}

template <typename Option_Class>
__no_constexpr_imp void
    cli_t<Option_Class>::parse_arguments(
        Option_Class&  _a_option_class,
        int            _a_argc,
        char**         _a_arg_v,
        cli_results_t& _a_cli_results
    ) const noexcept
{
    parse_arguments(
        _a_option_class,
        detail::make_strs_from_command_line_args(_a_argc, _a_arg_v),
        _a_cli_results
    );
}

template <typename Option_Class>
__no_constexpr_imp void
    cli_t<Option_Class>::parse_arguments(
        Option_Class&                          _a_option_class,
        const std::vector<std::u8string>& _a_args,
        cli_results_t&                         _a_cli_results
    ) const noexcept
{
    using namespace std;
    const size_t _l_strs_size{_a_args.size()};
    for (size_t _l_idx{0}; _l_idx < _l_strs_size; ++_l_idx)
    {
        const u8string_view _l_str{_a_args[_l_idx]};
        if (const result_t<u8string_view> _l_opt_normalised_str{
                normalise_str_from_cli_to_flag(_l_str)
            };
            _l_opt_normalised_str.has_value())
        {
            if (const optional<reference_wrapper<cli_info_t<Option_Class>>>
                    _l_clp_info{find_clp_info(_l_opt_normalised_str.value())};
                _l_clp_info.has_value())
            {
                if (const bool _l_terminate{process_args(
                        _a_option_class,
                        _l_opt_normalised_str.value(),
                        _l_clp_info.value().get(),
                        _a_args,
                        _l_strs_size,
                        _l_idx,
                        optional<u8string_view>{},
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

template <typename Option_Class>
template <typename T, typename U>
__no_constexpr_imp void
    cli_t<Option_Class>::add_option(
        const cli_option_config_t& _a_option,
        T Option_Class::*                           _a_member_var,
        cli_results_t&                              _a_cli_results,
        const cli_argument_processing_info_t<T, U>& _a_cli_processing_funcs

    ) noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_one_arg_t<Option_Class, T, U>>(
        _a_option, _a_member_var, _a_cli_processing_funcs
    ));
}

template <typename Option_Class>
template <typename T, typename U>
__no_constexpr_imp void
    cli_t<Option_Class>::add_multi_element_option(
        const cli_option_config_t& _a_cli_option,
        T Option_Class::*                           _a_member_var,
        cli_results_t&                              _a_cli_results,
        const cli_argument_processing_info_t<T, U>& _a_cli_processing_funcs

    ) noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_multi_args<Option_Class, T, U>>(
        _a_cli_option, _a_member_var, _a_cli_processing_funcs
    ));
}

template <typename Option_Class>
__no_constexpr_imp void
    cli_t<Option_Class>::add_help_flag() noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_help_t<Option_Class>>());
}

template <typename Option_Class>
__no_constexpr_imp void
    cli_t<Option_Class>::add_file_config_flag() noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_config_t<Option_Class>>());
}

template <typename Option_Class>
__no_constexpr_imp void
    cli_t<Option_Class>::add_auto_configuration() noexcept
{
    using namespace std;
    inner_add_option(make_shared<cli_auto_config_t<Option_Class>>());
}

template <typename Option_Class>
__no_constexpr_imp abc::utility::str::string_table_t
                   cli_t<Option_Class>::make_help_output() const noexcept
{
    using namespace abc::utility::str;
    string_table_t _l_st({0, 1});
    _l_st.push_back(u8"Flag(s):");
    _l_st.push_back(u8"    ");
    _l_st.push_back(u8"Description:");
    _l_st.new_line();
    for (const std::shared_ptr<cli_info_t<Option_Class>>& _l_flag : _m_clp_info)
    {
        const cli_info_t<Option_Class>& _l_cli_info{*_l_flag.get()};
        _l_st.push_back(fmt::format(
            u8"{0}{1}{2}",
            _m_multi_char_identifier,
            _l_cli_info.flag(),
            (_l_cli_info.char_flag().has_value()
                 ? fmt::format(
                       u8", {0}{1}",
                       _m_single_char_identifier,
                       _l_cli_info.char_flag().value()
                   )
                 : u8"")
        ));
        _l_st.push_empty();
        _l_st.push_back(fmt::format(
            u8"{0}{1}",
            _l_cli_info.description(),
            not _l_cli_info.enabled_for_config_file()
                ? u8" Option will not be recognised in a configuration file."
                : u8""
        ));
        _l_st.new_line();
    }
    return _l_st;
}

template <typename Option_Class>
__constexpr_imp bool
    cli_t<Option_Class>::process_config_line(
        Option_Class&                     _a_option_class,
        const std::u8string_view          _a_field_name,
        const std::vector<std::u8string>& _a_args,
        const std::u8string_view          _a_source_line,
        cli_results_t&                    _a_cli_results
    ) const noexcept
{
    using namespace std;
    if (const result_t<u8string_view> _l_opt_normalised_str{
            normalise_str_from_config_to_flag(_a_field_name)
        };
        _l_opt_normalised_str.has_value())
    {
        if (const optional<reference_wrapper<cli_info_t<Option_Class>>>
                _l_clp_info{find_clp_info(_l_opt_normalised_str.value())};
            _l_clp_info.has_value())
        {
            size_t                _l_idx{0};
            vector<u8string> _l_strs;
            for (auto& _l_arg : _a_args)
            {
                _l_strs.push_back(_l_arg);
            }
            if (bool _l_terminate{process_args(
                    _a_option_class,
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

template <typename Option_Class>
__no_constexpr_imp bool
    cli_t<Option_Class>::process_repetition_data(
        Option_Class& _a_option_class,
        const std::tuple<
            std::filesystem::path,
            auto_configuration_load_configuration_t,
            enum_auto_configuration_write_to_file_t>& _a_tuple_data,
        cli_results_t&                                _a_cli_results
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
                u8"Parent folder {0} does not exist",
                _l_folder.parent_path().u8string()
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
            u8"The file containing index {0} was required, however it could "
            u8"not "
            u8"be found",
            _l_rep_file_idx.index().value()
        ));
        return true;
    }
    // Now time to read the relevant parts of the file
    detail::found_data_t _l_data;
    optional<cli_auto_configuration_file_last_loaded_info_t>
        _l_last_loaded_info;
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
                    _a_option_class,
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
            _a_cli_results.add_error(u8"Couldn't work it out");
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
            if (_l_data.test_passed == false
                && not _l_file_containing_index_to_run.has_value())
            {
                _l_file_containing_index_to_run = _l_last_file;
                if (const bool _l_terminated{load_data(
                        _a_option_class,
                        _a_cli_results,
                        _l_data,
                        _l_last_file.value().file
                    )};
                    _l_terminated)
                {
                    return true;
                }
            }
            _l_last_loaded_info
                = cli_auto_configuration_file_last_loaded_info_t{
                    .file       = _l_last_file.value().file,
                    .last_index = _l_data.configuration_index
                };
        }
        else
        {
            _a_cli_results.add_error(u8"Couldn't work it out");
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

template <typename Option_Class>
__constexpr_imp const std::optional<cli_auto_configuration_t>&
                      cli_t<Option_Class>::auto_configuration() const noexcept
{
    return _m_rep_data;
}

template <typename Option_Class>
__no_constexpr void
    cli_t<Option_Class>::setup_next_file(
        const std::string_view _a_autofile_name,
        const std::size_t      _a_autofile_size,
        const Option_Class&    _a_option_class,
        const bool             _a_test_success
    ) const noexcept
{
    if (_m_rep_data.has_value())
    {
        auto& _l_rep_data{_m_rep_data.value()};
        switch (_l_rep_data._m_rep_write_data_type)
        {
        case enum_auto_configuration_write_to_file_t::ALWAYS_WRITE:
            write_file(
                _a_autofile_name,
                _a_autofile_size,
                _a_option_class,
                _a_test_success
            );
            break;
        case enum_auto_configuration_write_to_file_t::AUTO:
            if ((not _l_rep_data._m_loaded_configuration.has_value()
                 && not _a_test_success)
                || (_l_rep_data._m_loaded_configuration.has_value()
                    && _a_test_success))
            {
                write_file(
                    _a_autofile_name,
                    _a_autofile_size,
                    _a_option_class,
                    _a_test_success
                );
            }
        default:
            break;
        }
    }
}

template <typename Option_Class>
__no_constexpr_imp bool
    cli_t<Option_Class>::load_data(
        Option_Class&                _a_option_class,
        cli_results_t&               _a_cli_results,
        const detail::found_data_t&  _l_data,
        const std::filesystem::path& _a_path
    ) const noexcept
{
    using namespace std;
    size_t _l_line_idx{1};
    for (const u8string_view _l_line : _l_data.configuration_data)
    {
        // Split string into parts. Then process.
        if (_l_line.size() == 0 || _l_line[0] == char8_t('#'))
        {
            continue;
        }
        const size_t _l_equals_pos{_l_line.find_first_of(u8"=")};
        if (_l_equals_pos == u8string::npos)
        {
            _a_cli_results.add_error(fmt::format(u8"couldn't run file"));
            return true;
        }
        else
        {
            const u8string   _l_field_name{abc::utility::str::remove_whitespace(
                _l_line.substr(0, _l_equals_pos - 1)
            )};
            vector<u8string> _l_strs = abc::utility::str::split_string<char8_t>(
                _l_line.substr(_l_equals_pos + 1), u8" "
            );
            if (bool _l_terminate{process_config_line(
                    _a_option_class,
                    _l_field_name,
                    _l_strs,
                    fmt::format(
                        u8"memoized configuration {0} from file \"{1}\", line "
                        u8"{2}",
                        _l_data.configuration_index,
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

template <typename Option_Class>
__constexpr_imp result_t<std::u8string_view>
                cli_t<Option_Class>::normalise_str_from_cli_to_flag(
        const std::u8string_view _a_str
    ) const noexcept
{
    using namespace std;
    if (_a_str.size() == 0)
    {
        return unexpected(
            u8"Could not normalise string to flag, as string is empty."
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
                u8"Could not find multi-character option \"{0}\".", _l_str
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
                u8"Could not find single character option '{0}'.", _a_str[1]
            ));
        }
    }
    else
    {
        return unexpected(fmt::format(
            u8"Could not match string to flag. String given as \"{0}\". "
            u8"Multi-line identifier is \"{1}\", and single line identifier "
            u8"is "
            u8"`{2}`.",
            _a_str,
            _m_multi_char_identifier,
            _m_single_char_identifier
        ));
    }
}

template <typename Option_Class>
__constexpr_imp result_t<std::u8string_view>
                cli_t<Option_Class>::normalise_str_from_config_to_flag(
        const std::u8string_view _a_str
    ) const noexcept
{
    using namespace std;
    if (_a_str.size() == 0)
    {
        return unexpected(
            u8"Could not normalise string to flag, as string is empty."
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
            u8"Could not match string to flag. String given as \"{0}\". ",
            _a_str
        ));
    }
}

template <typename Option_Class>
__constexpr_imp const
    std::optional<std::reference_wrapper<cli_info_t<Option_Class>>>
    cli_t<Option_Class>::find_clp_info(
        const std::u8string_view& _a_info
    ) const noexcept
{
    using namespace std;
    return _m_sv_to_clp_info.at(_a_info);
}

template <typename Option_Class>
__constexpr_imp bool
    cli_t<Option_Class>::process_args(
        Option_Class&                            _a_option_class,
        const std::u8string_view                 _a_flag,
        const cli_info_t<Option_Class>&          _a_cli_info,
        const std::vector<std::u8string>&   _a_strs,
        const std::size_t                        _a_strs_size,
        std::size_t&                             _a_current_index,
        const std::optional<std::u8string_view>& _a_source,
        cli_results_t&                           _a_cli_results
    ) const noexcept
{
    using namespace std;
    size_t _l_index_of_flag{_a_current_index};
    ++_a_current_index;
    vector<u8string_view> _l_strs_to_pass;
    while (_a_current_index < _a_strs.size())
    {
        const result_t<u8string_view> _l_next_arg
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
    if (const bool _l_terminate{_a_cli_info.process_args(
            _a_option_class, _a_flag, _l_strs_to_pass, *this, _a_cli_results
        )};
        _l_terminate)
    {
        return true;
    }
    if (auto _l_opt_print(_a_cli_info.print(_a_option_class));
        _l_opt_print.has_value())
    {
        _a_cli_results.add_memoized_data(
            true,
            _a_flag,
            _l_opt_print.value(),
            (_a_source.has_value() ? _a_source.value()
                                   : fmt::format(
                                         u8"command line arguments {0} to {1}",
                                         _l_index_of_flag,
                                         _a_current_index
                                     ))
        );
    }
    return false;
}

template <typename Option_Class>
__no_constexpr_imp void
    cli_t<Option_Class>::inner_add_option(
        const std::shared_ptr<cli_info_t<Option_Class>> _a_cli
    ) noexcept
{
    using namespace std;
    if (_a_cli->char_flag().has_value())
    {
        _m_char_to_clp_info.insert(
            {_a_cli->char_flag().value(), u8string(_a_cli->flag())}
        );
    }
    _m_clp_info.push_back(_a_cli);
    _m_sv_to_clp_info.insert({_a_cli->flag(), reference_wrapper(*_a_cli.get())}
    );
}

template <typename Option_Class>
__no_constexpr_imp void
    cli_t<Option_Class>::write_file(
        const std::string_view _a_autofile_name,
        const std::size_t      _a_autofile_size,
        const Option_Class&    _a_option_class,
        const bool             _a_test_success
    ) const noexcept
{
    using namespace std;
    auto&    _l_rep_data{_m_rep_data.value()};
    ofstream _l_output;
    size_t   _l_next_index;
    if (_l_rep_data._m_last_config_info.has_value())
    {
        _l_next_index = _l_rep_data._m_last_config_info.value().last_index + 1;
        if (_l_next_index > _l_rep_data._m_last_config_info.value().max_index)
        {
            const filesystem::path _l_new_path{
                filesystem::path(_l_rep_data._m_repetition_folder)
                    .append(fmt::format(
                        "{0}_{1}_{2}",
                        _a_autofile_name,
                        _l_rep_data._m_last_config_info.value().max_index + 1,
                        _l_rep_data._m_last_config_info.value().max_index
                            + _a_autofile_size
                    ))
            };
            _l_output.open(_l_new_path, ios::app);
            // Create new file.
        }
        else
        {
            _l_output.open(
                _l_rep_data._m_last_config_info.value().file, ios::app
            );
        }
    }
    else
    {
        _l_next_index = 1;
        const filesystem::path _l_new_path{
            filesystem::path(_l_rep_data._m_repetition_folder)
                .append(fmt::format(
                    "{0}_{1}_{2}", _a_autofile_name, 1, +_a_autofile_size
                ))
        };
        _l_output.open(_l_new_path, ios::app);
    }
    u8string _l_printed_str{
        abc::utility::printer::default_printer_t<pair<bool, size_t>>{}
            .run_printer(make_pair(_a_test_success, _l_next_index))
    };
    _l_output << _a_option_class.autofile_metadata_string << "_"
              << string(_l_printed_str.begin(), _l_printed_str.end())
              << std::endl;
    for (const auto& [_l_field, _l_data] :
         get_config_file_data(_a_option_class))
    {
        const u8string _l_str{ fmt::format(u8"{0} = {1}", _l_field, _l_data) };
        _l_output << string(_l_str.begin(), _l_str.end()) << endl;
    }
    _l_output.close();
}

template <typename Option_Class>
__constexpr_imp std::vector<std::pair<std::u8string, std::u8string>>
                cli_t<Option_Class>::get_config_file_data(
        const Option_Class& _a_option_class
    ) const noexcept
{
    using namespace std;
    vector<pair<u8string, u8string>> _l_rv;
    for (const shared_ptr<cli_info_t<Option_Class>>& _l_ptr : _m_clp_info)
    {
        const cli_info_t<Option_Class>& _l_info{*_l_ptr.get()};
        const optional<u8string> _l_opt_str{_l_info.print(_a_option_class)};
        if (_l_opt_str.has_value())
        {
            _l_rv.push_back(
                make_pair(u8string(_l_info.flag()), _l_opt_str.value())
            );
        }
    }
    return _l_rv;
}

namespace detail
{
__constexpr_imp std::vector<std::u8string>
                make_strs_from_command_line_args(
                    const int _a_argc,
                    char**    _a_argv
                ) noexcept
{
    using namespace std;
    vector<u8string> _l_strs(_a_argc > 1 ? (_a_argc - 1) : 0);
    for (size_t _l_idx{1}; _l_idx < _a_argc; ++_l_idx)
    {
        _l_strs[_l_idx - 1] = checkless_convert_ascii_to_unicode_string<u8string>(_a_argv[_l_idx]);
    }
    return _l_strs;
}

__no_constexpr_imp std::optional<std::string>
                   found_data_t::process_lines(
        std::fstream&            _a_fstream,
        const std::u8string_view _a_str_to_find
    ) noexcept
{
    using namespace std;
    string _l_unprocessed_line;
    while (getline(_a_fstream, _l_unprocessed_line))
    {
        u8string _l_line{abc::unpack_string_to_u8string(_l_unprocessed_line)};
        if (_l_line.starts_with(_a_str_to_find))
        {
            return _l_unprocessed_line;
        }
        else
        {
            configuration_data.push_back(_l_line);
        }
    }
    return nullopt;
}

__constexpr_imp std::optional<found_data_t>
                find_data(
                    const std::filesystem::path&      _a_path,
                    const std::optional<std::size_t>& _a_index
                ) noexcept
{
    using namespace std;
    fstream                _l_file_input(_a_path);
    string                 _l_line;
    optional<found_data_t> _l_rv;
    u8string               _l_str_to_find{u8"metadata_"};
    optional<string>       _l_unprocessed_line;
    while (_l_unprocessed_line.has_value() || getline(_l_file_input, _l_line))
    {
        const string&  _l_line_to_get_as_str = _l_unprocessed_line.has_value()
                                                   ? _l_unprocessed_line.value()
                                                   : _l_line;
        const u8string _l_line_to_get{
            abc::unpack_string_to_u8string(_l_line_to_get_as_str)
        };
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
                auto& [_l_passed, _l_index]{_l_res.value()};
                if (_a_index.has_value())
                {
                    if (_a_index.value() == _l_index)
                    {
                        // We've found the correct configuration.
                        found_data_t _l_fd{_l_passed, _l_index};
                        // Process its line, close the stream and return it.
                        _l_fd.process_lines(_l_file_input, _l_str_to_find);
                        _l_file_input.close();
                        return _l_fd;
                    }
                }
                else
                {
                    if (_l_rv.has_value())
                    {
                        found_data_t& _l_fd{_l_rv.value()};
                        if (_l_fd.configuration_index < _l_index)
                        {
                            _l_fd = found_data_t{_l_passed, _l_index};
                            _l_unprocessed_line = _l_fd.process_lines(
                                _l_file_input, _l_str_to_find
                            );
                        }
                    }
                    else
                    {
                        _l_rv               = found_data_t{_l_passed, _l_index};
                        _l_unprocessed_line = _l_rv.value().process_lines(
                            _l_file_input, _l_str_to_find
                        );
                    }
                }
            }
        }
    }
    return _l_rv;
    // Carry on reading until either EOF or another metadata line.
}
} // namespace detail

_END_ABC_UTILITY_CLI_NS
