#pragma once

#include "abc_test/utility/cli/cli_option_config.hpp"
#include "abc_test/utility/cli/cli_output.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/printers/default_printer.hpp"

#include <filesystem>
#include <fmt/core.h>
#include <functional>
#include <optional>
#include <string>
#include <vector>

_BEGIN_ABC_UTILITY_CLI_NS
template <typename Option_Class>
class cli_t;
class cli_output_t;

namespace detail
{
template <typename T>
__constexpr std::function<result_t<T>(abc::utility::parser::parser_input_t&)>
            make_parser_func()
{
    using namespace std;
    using namespace abc::utility::parser;
    return [](parser_input_t& _a_str)
    {
        if constexpr (same_as<T, u8string> || same_as<T, filesystem::path>)
        {
            return T(_a_str.get_u8string());
        }
        else
        {
            return default_parser<T>()->run_parser(_a_str);
        }
    };
}

template <typename T>
__constexpr std::function<std::u8string(const T&)>
            make_printer_func() noexcept
{
    return [](const T& _a_element)
    {
        if constexpr (std::same_as<T, std::u8string>)
        {
            return _a_element;
        }
        else if constexpr (std::same_as<T, std::filesystem::path>)
        {
            return _a_element.u8string();
        }
        else
        {
            return abc::utility::printer::default_printer_t<T>{}.run_printer(
                _a_element
            );
        }
    };
}

template <typename T, typename U>
__constexpr std::function<std::optional<std::u8string>(T&, const U&)>
            process_value()
{
    using namespace std;
    return [](T& _a_reference, const U& _a_parse_value)
    {
        if constexpr (std::same_as<T, std::vector<U>>)
        {
            _a_reference.push_back(_a_parse_value);
        }
        else
        {
            _a_reference = _a_parse_value;
        }
        return nullopt;
    };
}
} // namespace detail

template <typename T, typename U>
struct cli_argument_processing_info_t
{
    std::function<result_t<U>(abc::utility::parser::parser_input_t&)>
        parser_func = detail::make_parser_func<U>();
    std::function<std::optional<std::u8string>(T&, const U&)>
        process_parsed_value_func = detail::process_value<T, U>();
    std::function<std::u8string(const T&)> print_func
        = detail::make_printer_func<T>();
};

template <typename Option_Class>
class cli_info_t
{
public:
    __constexpr virtual ~cli_info_t() = default;
    __constexpr
    cli_info_t()
        = delete;

    __constexpr
    cli_info_t(
        const cli_option_config_t& _a_option_config,
        const std::size_t          _a_min_args,
        const std::size_t          _a_max_args,
        bool                       _a_enabled_for_config_file = true
    ) noexcept
        : _m_min_arguments(_a_min_args)
        , _m_max_arguments(_a_max_args)
        , _m_option_config(_a_option_config)
        , _m_enabled_for_config_file(_a_enabled_for_config_file)
    {}

    __constexpr std::u8string_view
                flag() const noexcept
    {
        return _m_option_config.flag;
    }

    __constexpr std::optional<char8_t>
                char_flag() const noexcept
    {
        return _m_option_config.single_char_flag;
    }

    __constexpr const std::optional<std::size_t>
                      min_arguments() const noexcept
    {
        return _m_min_arguments;
    }

    __constexpr const std::optional<std::size_t>
                      max_arguments() const noexcept
    {
        return _m_max_arguments;
    }

    __no_constexpr_imp virtual bool
        process_args(
            Option_Class&                          _a_option_class,
            const std::u8string_view               _a_flag,
            const std::vector<std::u8string_view>& _a_args,
            const cli_t<Option_Class>&             _a_cli,
            _ABC_NS_UTILITY_CLI::cli_results_t&    _a_cli_results
        ) const noexcept
    {
        using namespace std;
        if (_a_args.size() < _m_min_arguments
            || _a_args.size() > _m_max_arguments)
        {
            _a_cli_results.add_error(
                (_m_min_arguments == _m_max_arguments)
                    ? fmt::format(
                          u8"The flag {0} expected exactly {1} {2}, however "
                          u8"{3} "
                          u8"{4} provided",
                          _a_flag,
                          _m_min_arguments,
                          (_m_min_arguments == 1) ? u8"argument"
                                                  : u8"arguments",
                          _a_args.size(),
                          (_a_args.size() == 1) ? u8"was" : u8"were"
                      )
                    : fmt::format(
                          u8"The flag {0} expected between {1} and {2} "
                          u8"arguments, however {3} {4} provided",
                          _a_flag,
                          _m_min_arguments,
                          _m_max_arguments,
                          _a_args.size(),
                          (_a_args.size() == 1) ? u8"was" : u8"were"
                      )
            );
            return true;
        }
        else
        {
            return process_args_(
                _a_option_class, _a_flag, _a_args, _a_cli, _a_cli_results
            );
        }
    }

    __no_constexpr_imp virtual bool
        process_args_(
            Option_Class&                          _a_option_class,
            const std::u8string_view               _a_flag,
            const std::vector<std::u8string_view>& _a_args,
            const cli_t<Option_Class>&             _a_cli,
            cli_results_t&                         _a_cli_results
        ) const noexcept
        = 0;

    __constexpr virtual std::optional<std::u8string>
        print(
            const Option_Class& _a_option_class
        ) const noexcept
    {
        return std::nullopt;
    }

    __constexpr std::u8string_view
                description() const noexcept
    {
        return _m_option_config.description;
    }

    __constexpr bool
        enabled_for_config_file() const noexcept
    {
        return _m_enabled_for_config_file;
    }
private:
    std::size_t         _m_min_arguments;
    std::size_t         _m_max_arguments;
    cli_option_config_t _m_option_config;
    bool                _m_enabled_for_config_file;
    bool                _m_enabled_for_config_list;
};

template <typename Option_Class>
class cli_no_args_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_no_args_t()
        = delete;

    __constexpr virtual std::optional<std::u8string>
        print() const noexcept override
    {
        using namespace abc::utility::printer;
        return default_printer_t<bool>().run_printer(_m_element_to_set.get());
    }

    __no_constexpr_imp virtual bool
        process_args_(
            Option_Class&                          _a_option_class,
            const std::u8string_view               _a_flag,
            const std::vector<std::u8string_view>& _a_args,
            const cli_t<Option_Class>&             _a_cli,
            cli_results_t&                         _a_cli_results
        ) const noexcept
    {
        using namespace std;
        _m_element_to_set.get() = (_a_flag == _m_positive_set);
        return false;
    }
private:
    std::u8string_view             _m_positive_set;
    std::u8string_view             _m_negative_set;
    std::reference_wrapper<bool> _m_element_to_set;
};

template <typename Option_Class, typename T, typename U>
class cli_one_arg_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_one_arg_t(
        const cli_option_config_t& _a_cli_option_config,
        T Option_Class::*                           _a_member_var,
        const cli_argument_processing_info_t<T, U>& _a_processing_info
    ) noexcept
        : cli_info_t<Option_Class>(_a_cli_option_config, 1, 1)
        , _m_member_var(_a_member_var)
        , _m_processing_info(_a_processing_info)
    {}

    __constexpr virtual std::optional<std::u8string>
        print(
            const Option_Class& _a_option_class
        ) const noexcept
    {
        using namespace abc::utility::printer;
        return _m_processing_info.print_func(_a_option_class.*_m_member_var);
    }

    __no_constexpr_imp virtual bool
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
        abc::utility::parser::parser_input_t _l_pi(_a_args[0]);
        if (const result_t<U> _l_parse_result{
                _m_processing_info.parser_func(_l_pi)
            };
            _l_parse_result.has_value())
        {
            if (const optional<u8string> _l_process_okay{
                    _m_processing_info.process_parsed_value_func(
                        _a_option_class.*_m_member_var, _l_parse_result.value()
                    )
                };
                _l_process_okay)
            {
                _a_cli_results.add_error(fmt::format(
                    u8"Some post-parsing processing error encountered. Specifically \"{0}\".",
                    _l_process_okay.value()
                ));
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            _a_cli_results.add_error(fmt::format(
                u8"Could not parse std::string \"{0}\" to type {1}. "
                u8"Reason: {2}",
                _a_args[0],
                type_id<T>(),
                _l_parse_result.error()
            ));
            return true;
        }
    }
private:
    T Option_Class::*                    _m_member_var;
    cli_argument_processing_info_t<T, U> _m_processing_info;
};

template <typename Option_Class, typename T, typename U>
class cli_multi_args : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_multi_args(
        const cli_option_config_t& _a_cli_option_config,
        T Option_Class::*                           _a_member_var,
        const cli_argument_processing_info_t<T, U>& _a_processing_info
    ) noexcept
        : cli_info_t<Option_Class>(
              _a_cli_option_config,
              0,
              std::numeric_limits<std::size_t>::max()
          )
        , _m_member_var(_a_member_var)
        , _m_processing_info(_a_processing_info)
    {}

    __constexpr virtual std::optional<std::u8string>
        print(
            const Option_Class& _a_option_class
        ) const noexcept
    {
        using namespace abc::utility::printer;
        return _m_processing_info.print_func(_a_option_class.*_m_member_var);
    }

    __no_constexpr_imp virtual bool
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
        for (const u8string_view _l_arg : _a_args)
        {
            abc::utility::parser::parser_input_t _l_pi(_l_arg);
            if (const result_t<U> _l_parse_result{
                    _m_processing_info.parser_func(_l_pi)
                };
                _l_parse_result.has_value())
            {
                if (const bool _l_process_okay{
                        _m_processing_info.process_parsed_value_func(
                            _a_option_class.*_m_member_var,
                            _l_parse_result.value()
                        )
                    };
                    _l_process_okay)
                {
                    _a_cli_results.add_error(fmt::format(
                        u8"Some post-parsing processing error encountered. "
                    ));
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                _a_cli_results.add_error(fmt::format(
                    u8"Could not parse std::string \"{0}\" to type {1}. "
                    u8"Reason: {2}",
                    _l_arg,
                    type_id<T>(),
                    _l_parse_result.error()
                ));
                return true;
            }
        }
        return false;
    }
private:
    T Option_Class::*                    _m_member_var;
    cli_argument_processing_info_t<T, U> _m_processing_info;
};

_END_ABC_UTILITY_CLI_NS