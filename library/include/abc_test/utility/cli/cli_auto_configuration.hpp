#pragma once

#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/internal/macros.hpp"



#include <filesystem>
#include <fmt/xchar.h>
#include <fstream>
#include <pnp/support/internal/std/variant.hpp>
#include <pnp/support/user_types/enum.hpp>
#include <variant>
#include <pnp/support/user_types/objects.hpp>


_BEGIN_ABC_UTILITY_CLI_NS
enum class auto_configuration_load_type_e
{
    latest,
    latest_if_failure,
    automatic
};
_END_ABC_UTILITY_CLI_NS

_BEGIN_ABC_NS
template <>
auto
    utility::get_enum_list() noexcept
    -> utility::enum_list_t<utility::cli::auto_configuration_load_type_e>
{
    using enum utility::cli::auto_configuration_load_type_e;
    return {
        _ENUM_LIST_ENTRY(latest),
        _ENUM_LIST_ENTRY(latest_if_failure),
        _ENUM_LIST_ENTRY(automatic)
    };
}

_END_ABC_NS
_BEGIN_ABC_UTILITY_CLI_NS

class auto_configuration_load_configuration_t
{
public:
    __constexpr
    auto_configuration_load_configuration_t()
        : auto_configuration_load_configuration_t(0)
    {}

    __constexpr
    auto_configuration_load_configuration_t(
        const std::size_t _a_data
    )
        : _m_data(_a_data)
    {}

    __constexpr
    auto_configuration_load_configuration_t(
        const auto_configuration_load_type_e _a_data
    )
        : _m_data(_a_data)
    {}

    __constexpr std::optional<std::size_t>
                index() const noexcept
    {
        using namespace std;
        return (holds_alternative<size_t>(_m_data))
                   ? make_optional(get<size_t>(_m_data))
                   : nullopt;
    }

    inline auto_configuration_load_configuration_t(
        const std::variant<std::size_t, auto_configuration_load_type_e>&
            data_arg
    )
        : _m_data(data_arg)
    {}

    using fields = std::variant<std::size_t, auto_configuration_load_type_e>;
    fields _m_data;
};
enum class enum_auto_configuration_write_to_file_t
{
    ALWAYS_WRITE,
    AUTO,
    DO_NOT_WRITE
};
_END_ABC_UTILITY_CLI_NS
PNP_NS_BEGIN

template <>
struct enum_list<abc::utility::cli::auto_configuration_load_type_e>
{
    static constexpr enum_element<
        abc::utility::cli::auto_configuration_load_type_e>
        values[] = {
            {abc::utility::cli::auto_configuration_load_type_e::latest,
             u8"latest"                                                                         },
            {abc::utility::cli::auto_configuration_load_type_e::
                 latest_if_failure,                           u8"latest_if_failure"},
            {abc::utility::cli::auto_configuration_load_type_e::automatic,
             u8"automatic"                                                                      },
    };
};

template <>
struct enum_list<abc::utility::cli::enum_auto_configuration_write_to_file_t>
{
    static constexpr enum_element<
        abc::utility::cli::enum_auto_configuration_write_to_file_t>
        values[] = {
            {abc::utility::cli::enum_auto_configuration_write_to_file_t::
                 ALWAYS_WRITE,                                    u8"ALWAYS_WRITE"},
            {abc::utility::cli::enum_auto_configuration_write_to_file_t::AUTO,
             u8"AUTO"                                                                          },
            {abc::utility::cli::enum_auto_configuration_write_to_file_t::
                 DO_NOT_WRITE,                                    u8"DO_NOT_WRITE"},
    };
};

struct auto_configuration_load_configuration_traits
{
    static constexpr std::u8string_view
        consistent_name() noexcept
    {
        return u8"auto_configuration_load_configuration_t";
    }

    static constexpr auto
        to_tuple(
            const abc::utility::cli::auto_configuration_load_configuration_t&
                object_arg
        ) noexcept
    {
        return std::tie(object_arg._m_data);
    }
};

// std::variant<std::size_t, auto_configuration_load_type_e>
using auto_configuration_load_configuration_variant_pnp = pnp::support::generic_variant_pnp<
    abc::utility::cli::auto_configuration_load_configuration_t::fields,
    std::tuple<
    pnp::support::integer_pnp<std::size_t>,
    pnp::support::listed_enum_pnp<abc::utility::cli::auto_configuration_load_type_e>>>;

using auto_configuration_load_configuration_fields_pnp = pnp::support::generic_tuple_pnp<
    std::tuple<abc::utility::cli::auto_configuration_load_configuration_t::fields>,
    std::tuple<auto_configuration_load_configuration_variant_pnp>>;

struct auto_configuration_load_configuration_pnp
    : public pnp::object_pnp<
          abc::utility::cli::auto_configuration_load_configuration_t,
          auto_configuration_load_configuration_traits,
    auto_configuration_load_configuration_fields_pnp,
    auto_configuration_load_configuration_fields_pnp>
{};

PNP_NS_END

_BEGIN_ABC_NS
template <>
auto
    utility::get_enum_list() noexcept -> utility::enum_list_t<
        utility::cli::enum_auto_configuration_write_to_file_t>
{
    using enum utility::cli::enum_auto_configuration_write_to_file_t;
    return {
        _ENUM_LIST_ENTRY(ALWAYS_WRITE),
        _ENUM_LIST_ENTRY(AUTO),
        _ENUM_LIST_ENTRY(DO_NOT_WRITE)
    };
}

_END_ABC_NS
/*_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<cli::enum_auto_configuration_write_to_file_t>
    : public parser_base_t<cli::enum_auto_configuration_write_to_file_t>
{
    __constexpr result_t<cli::enum_auto_configuration_write_to_file_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        using namespace _ABC_NS_CLI;
        if (_a_parse_input.check_and_advance("always_write"))
        {
            return enum_auto_configuration_write_to_file_t::ALWAYS_WRITE;
        }
        else if (_a_parse_input.check_and_advance("auto"))
        {
            return enum_auto_configuration_write_to_file_t::AUTO;
        }
        else if (_a_parse_input.check_and_advance("do_not_write"))
        {
            return enum_auto_configuration_write_to_file_t::DO_NOT_WRITE;
        }
        else
        {
            return unexpected("Couldn't parse");
        }
    }
};

_END_ABC_UTILITY_PARSER_NS*/
_BEGIN_ABC_UTILITY_PARSER_NS
#if 0
template <>
struct default_parser_t<cli::auto_configuration_load_configuration_t>
    : public parser_base_t<cli::auto_configuration_load_configuration_t>
{
    __constexpr result_t<cli::auto_configuration_load_configuration_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        using namespace _ABC_NS_CLI;
        using namespace _ABC_NS_UTILITY_STR;
        if (const result_t<auto_configuration_load_type_e> _l_result{
                make_default_parser< auto_configuration_load_type_e>(
                    enum_helper_string_type_e::lower
                )
                    .run_parser(_a_parse_input)
            };
            _l_result.has_value())
        {
            return auto_configuration_load_configuration_t(_l_result.value());
        }
        else if (const result_t<size_t> _l_result{
                     default_parser_t<size_t>().run_parser(_a_parse_input)
                 };
                 _l_result.has_value())
        {
            return auto_configuration_load_configuration_t(_l_result.value());
        }
        else
        {
            return unexpected(fmt::format(
                u8"Couldn't parse string {0} to either a {1} or a {2}, which "
                u8"the "
                u8"constructor for {3} requires.",
                _a_parse_input.get_u8string(),
                type_id<auto_configuration_load_type_e>(),
                type_id<size_t>(),
                type_id<auto_configuration_load_configuration_t>()
            ));
        }
    }
};
#endif
_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_CLI_NS

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

class cli_auto_configuration_t
{
public:
    __constexpr
    cli_auto_configuration_t(
        const std::filesystem::path&                  _a_repetition_folder,
        const auto_configuration_load_configuration_t _a_rep_file_index,
        const enum_auto_configuration_write_to_file_t _a_rep_write_data_type,
        const std::optional<cli_auto_configuration_file_info_t>& _l_loaded_file,
        const std::optional<cli_auto_configuration_file_last_loaded_info_t>&
            _l_last_config_info
    ) noexcept;
    std::filesystem::path _m_repetition_folder;
    // Either auto, or auto with specific element.
    auto_configuration_load_configuration_t _m_rep_file_index;
    // Whether to write information.
    enum_auto_configuration_write_to_file_t _m_rep_write_data_type;
    // The loaded configuration
    std::optional<cli_auto_configuration_file_info_t> _m_loaded_configuration;
    // The file containing the last index.
    std::optional<cli_auto_configuration_file_last_loaded_info_t>
        _m_last_config_info;
};

_END_ABC_UTILITY_CLI_NS

_BEGIN_ABC_UTILITY_CLI_NS
__constexpr_imp
    cli_auto_configuration_t::cli_auto_configuration_t(
        const std::filesystem::path&                  _a_repetition_folder,
        const auto_configuration_load_configuration_t _a_rep_file_index,
        const enum_auto_configuration_write_to_file_t _a_rep_write_data_type,
        const std::optional<cli_auto_configuration_file_info_t>&
            _a_loaded_configuration,
        const std::optional<cli_auto_configuration_file_last_loaded_info_t>&
            _a_last_config_info
    ) noexcept
    : _m_repetition_folder(_a_repetition_folder)
    , _m_rep_file_index(_a_rep_file_index)
    , _m_rep_write_data_type(_a_rep_write_data_type)
    , _m_loaded_configuration(_a_loaded_configuration)
    , _m_last_config_info(_a_last_config_info)
{}

_END_ABC_UTILITY_CLI_NS