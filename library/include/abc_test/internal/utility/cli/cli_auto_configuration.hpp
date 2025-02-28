#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/utility/parsers/default_parser.hpp"
#include "abc_test/internal/utility/parsers/specializations/fundamental_types.hpp"
#include "abc_test/internal/utility/printers/default_printer.hpp"

#include <filesystem>
#include <variant>

#include <fstream>

_BEGIN_ABC_NS
enum class enum_rep_file_index_t
{
    LATEST,
    LATEST_IF_FAILURE,
};

class rep_file_index_t
{
public:
    __constexpr
    rep_file_index_t()
        : rep_file_index_t(0)
    {}

    __constexpr
    rep_file_index_t(
        const std::size_t _a_data
    )
        : _m_data(_a_data)
    {}

    __constexpr
    rep_file_index_t(
        const enum_rep_file_index_t _a_data
    )
        : _m_data(_a_data)
    {}

    __constexpr std::optional<std::size_t>
                index() const noexcept
    {
        using namespace std;
        if (holds_alternative<std::size_t>(_m_data))
        {
            return make_optional(get<size_t>(_m_data));
        }
        else
        {
            return nullopt;
        }
    }
private:
    std::variant<std::size_t, enum_rep_file_index_t> _m_data;
};
enum class rep_write_data_type_t
{
    ALWAYS_WRITE,
    AUTO,
    DO_NOT_WRITE
};

template <>
struct utility::parser::default_parser_t<rep_write_data_type_t>
    : public parser_base_t<rep_write_data_type_t>
{
    __constexpr parse_result_t<rep_write_data_type_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        if (_a_parse_input.check_and_advance("always_write"))
        {
            return rep_write_data_type_t::ALWAYS_WRITE;
        }
        else if (_a_parse_input.check_and_advance("auto"))
        {
            return rep_write_data_type_t::AUTO;
        }
        else if (_a_parse_input.check_and_advance("do_not_write"))
        {
            return rep_write_data_type_t::DO_NOT_WRITE;
        }
        else
        {
            return unexpected("Couldn't parse");
        }
    }
};

template <>
struct utility::parser::default_parser_t<rep_file_index_t>
    : public parser_base_t<rep_file_index_t>
{
    __constexpr parse_result_t<rep_file_index_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        if (_a_parse_input.check_and_advance("latest"))
        {
            return rep_file_index_t(enum_rep_file_index_t::LATEST);
        }
        else if (_a_parse_input.check_and_advance("latest_if_failure"))
        {
            return rep_file_index_t(enum_rep_file_index_t::LATEST_IF_FAILURE);
        }
        else if (_a_parse_input.check_and_advance("auto"))
        {
            return rep_file_index_t(enum_rep_file_index_t::LATEST_IF_FAILURE);
        }
        else
        {
            parse_result_t<size_t> _l_res{
                default_parser_t<size_t>().run_parser(_a_parse_input)
            };
            if (_l_res.has_value())
            {
                return rep_file_index_t{_l_res.value()};
            }
            else
            {
                return unexpected("Couldn't parse");
            }
        }
    }
};

class cli_auto_configuration_file_info_t
{
public:
    std::filesystem::path     file;
    std::pair<size_t, size_t> min_and_max_configuration_indexes;
};

class cli_auto_configuration_file_last_loaded_info_t
{
public:
    std::filesystem::path file;
    std::size_t           last_index;
    std::size_t           min_index;
    std::size_t           max_index;
};
struct test_options_base_t;

class cli_auto_configuration_t
{
public:
    __constexpr
    cli_auto_configuration_t(
        const std::filesystem::path& _a_repetition_folder,
        const rep_file_index_t       _a_rep_file_index,
        const rep_write_data_type_t  _a_rep_write_data_type,
        const std::optional<cli_auto_configuration_file_info_t>& _l_loaded_file,
        const std::optional<cli_auto_configuration_file_last_loaded_info_t>&
            _l_last_config_info
    ) noexcept;
   // __no_constexpr void
   //     setup_next_file(
   //         const std::vector<std::pair<std::string, std::string>>&
   //         _a_strs_to_print,
   //         const bool _a_test_success
   //     ) const noexcept;
    std::filesystem::path _m_repetition_folder;
    // Either auto, or auto with specific element.
    rep_file_index_t _m_rep_file_index;
    // Whether to write information.
    rep_write_data_type_t _m_rep_write_data_type;
    // The loaded configuration
    std::optional<cli_auto_configuration_file_info_t> _m_loaded_configuration;
    // The file containing the last index.
    std::optional<cli_auto_configuration_file_last_loaded_info_t>
        _m_last_config_info;
//    template<typename Option_Object>
//    __constexpr void
 //       prepare_file(
 //           const std::string_view _a_autofile_name,
 //           const std::size_t _a_autofile_size,
 //           const std::vector<std::pair<std::string, std::string>>&
   //         _a_strs_to_print,
   //         const bool _a_test_success
   //     ) const noexcept;
};

_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
    cli_auto_configuration_t::cli_auto_configuration_t(
        const std::filesystem::path& _a_repetition_folder,
        const rep_file_index_t       _a_rep_file_index,
        const rep_write_data_type_t  _a_rep_write_data_type,
        const std::optional<cli_auto_configuration_file_info_t>&
            _a_loaded_configuration,
        const std::optional<cli_auto_configuration_file_last_loaded_info_t>&
            _a_last_config_info
    ) noexcept
    : _m_repetition_folder(_a_repetition_folder)
    , _m_rep_file_index(_a_rep_file_index)
    , _m_rep_write_data_type(_a_rep_write_data_type)
    , _m_last_config_info(_a_last_config_info)
    , _m_loaded_configuration(_a_loaded_configuration)
{}

/*template<typename Option_Object>
__no_constexpr_imp void
cli_auto_configuration_t::setup_next_file(
    const Option_Object&
    _a_strs_to_print,
    const bool _a_test_success
) const noexcept
{
    switch (_m_rep_write_data_type)
    {
    case rep_write_data_type_t::ALWAYS_WRITE:
        prepare_file(_a_strs_to_print, _a_test_success);
        break;
    case rep_write_data_type_t::AUTO:
        if ((not this->_m_loaded_configuration.has_value() && not _a_test_success) ||
            (_m_loaded_configuration.has_value() && _a_test_success))
        {
            prepare_file(_a_strs_to_print, _a_test_success);
        }
    default:
        break;
    }
}

__constexpr void
    cli_auto_configuration_t::prepare_file(
        const std::string_view _a_autofile_name,
        const std::size_t _a_autofile_size,
        const std::vector<std::pair<std::string, std::string>>&
                   _a_strs_to_print,
        const bool _a_test_success
    ) const noexcept
{
    using namespace std;
    ofstream _l_output;
    size_t   _l_next_index;
    if (_m_last_config_info.has_value())
    {
        _l_next_index = _m_last_config_info.value().last_index + 1;
        if (_l_next_index > _m_last_config_info.value().max_index)
        {
            const filesystem::path _l_new_path{
                filesystem::path(_m_repetition_folder)
                    .append(fmt::format(
                        "{0}_{1}_{2}",
                        _a_autofile_name,
                        _m_last_config_info.value().max_index + 1,
                        _m_last_config_info.value().max_index
                            + _a_autofile_size
                    ))
            };
            _l_output.open(_l_new_path, ios::app);
            // Create new file.
        }
        else
        {
            _l_output.open(this->_m_last_config_info.value().file, ios::app);
        }
    }
    else
    {
        _l_next_index = 1;
        const filesystem::path _l_new_path{
            filesystem::path(_m_repetition_folder)
                .append(fmt::format(
                    "{0}_{1}_{2}",
                    _a_autofile_name,
                    1,
                    +_a_autofile_size
                ))
        };
        _l_output.open(_l_new_path, ios::app);
    }
    using namespace detail;
    for (const auto& [_l_field_name, _l_field] : _a_strs_to_print)
    {
        _l_output << fmt::format("{0} = {1}", _l_field_name, _l_field) << std::endl;
    }
    _l_output.close();
}*/

_END_ABC_NS