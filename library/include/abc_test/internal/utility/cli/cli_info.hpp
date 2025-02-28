#pragma once

#include "abc_test/internal/utility/cli/cli_option_config.hpp"
#include "abc_test/internal/utility/cli/cli_output.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/utility/parsers/default_parser.hpp"
#include "abc_test/internal/utility/printers/default_printer.hpp"
#include "abc_test/internal/utility/printers/specializations/fundamental_types.hpp"

#include <filesystem>
#include <fmt/core.h>
#include <functional>
#include <optional>
#include <string>
#include <vector>

_BEGIN_ABC_NS
template <typename Option_Class>
class cli_t;
class cli_output_t;
enum class enum_n_arguments
{
    ZERO,
    ONE,
    MULTI_ARGUMENT
};
namespace detail
{
    template <typename T>
    __constexpr std::function<std::optional<T>(const std::string_view)>
        make_parser_func()
    {
        using namespace std;
        using namespace abc::utility::parser;
        return [](const string_view _a_str)
            {
                if constexpr (std::same_as<T, std::string>
                    || std::same_as<T, std::filesystem::path>)
                {
                    return make_optional(T(_a_str));
                }
                else
                {
                    const parse_result_t<T> _l_pr{ parse(_a_str, default_parser<T>()) };
                    if (_l_pr.has_value())
                    {
                        return make_optional(_l_pr.value());
                    }
                    else
                    {
                        return optional<T>{};
                    }
                }
            };
    }

    template <typename T>
    __constexpr std::function<std::string(const T&)>
        make_printer_func() noexcept
    {
        return [](const T& _a_element)
            {
                if constexpr (std::same_as<T, std::string>)
                {
                    return _a_element;
                }
                else if constexpr (std::same_as<T, std::filesystem::path>)
                {
                    return _a_element.string();
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
    __constexpr std::function<std::optional<std::string>(T&, const U&)>
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

template<typename T, typename U>
struct cli_argument_processing_info_t
{
    std::function<std::optional<U>(const std::string_view)> parser_func = detail::make_parser_func<U>();
    std::function<std::optional<std::string>(T&, const U&)> process_parsed_value_func = detail::process_value<T, U>();
    std::function<std::string(const T&)> print_func = detail::make_printer_func<T>();
};
template <typename Option_Class>
class cli_info_t
{
public:
    __constexpr
    cli_info_t()
        = delete;

    __constexpr
    cli_info_t(
        const enum_n_arguments     _a_enum_n_arguments,
        const cli_option_config_t& _a_option_config,
        bool                       _a_enabled_for_config_file = true
    ) noexcept
        : _m_option_config(_a_option_config)
        , _m_enum_n_arguments(_a_enum_n_arguments)
        , _m_enabled_for_config_file(_a_enabled_for_config_file)
    {}

    __constexpr std::string_view
                flag() const noexcept
    {
        return _m_option_config.flag;
    }

    __constexpr std::optional<char>
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

    __constexpr enum_n_arguments
        n_arguments() const noexcept
    {
        return _m_enum_n_arguments;
    }

    __no_constexpr_imp virtual bool
        process_args(
            Option_Class&                        _a_option_class,
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t<Option_Class>&           _a_cli,
            cli_results_t&                       _a_cli_results
        ) const noexcept
        = 0;

    __constexpr virtual std::optional<std::string>
        print(
            const Option_Class& _a_option_class
        ) const noexcept
    {
        return std::nullopt;
    }

    __constexpr std::string_view
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
    std::optional<std::size_t> _m_min_arguments;
    std::optional<std::size_t> _m_max_arguments;
    enum_n_arguments           _m_enum_n_arguments;
    cli_option_config_t        _m_option_config;
    bool                       _m_enabled_for_config_file;
    bool                       _m_enabled_for_config_list;
};

template <typename Option_Class>
class cli_no_args_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_no_args_t()
        = delete;

    __constexpr virtual std::optional<std::string>
        print() const noexcept
    {
        using namespace abc::utility::printer;
        return default_printer_t<bool>().run_printer(_m_element_to_set.get());
    }

    __no_constexpr_imp virtual bool
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
                "in the form {0}.",
                _a_args
            ));
            return true;
        }
        else
        {
            _m_element_to_set.get() = (_a_flag == _m_positive_set);
            return false;
        }
    }
private:
    std::string_view             _m_positive_set;
    std::string_view             _m_negative_set;
    std::reference_wrapper<bool> _m_element_to_set;
};

template <typename Option_Class, typename T, typename U>
class cli_one_arg_t : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_one_arg_t(
        const cli_option_config_t& _a_cli_option_config,
        T Option_Class::* _a_member_var,
        const cli_argument_processing_info_t<T, U>& _a_processing_info
    ) noexcept
        : cli_info_t<Option_Class>(enum_n_arguments::ONE, _a_cli_option_config)
        , _m_member_var(_a_member_var)
        , _m_processing_info(_a_processing_info)
    {}

    __constexpr virtual std::optional<std::string>
        print(
            const Option_Class& _a_option_class
        ) const noexcept
    {
        using namespace abc::utility::printer;
        return _m_processing_info.print_func(_a_option_class.*_m_member_var);
    }

    __no_constexpr_imp virtual bool
        process_args(
            Option_Class&                        _a_option_class,
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t<Option_Class>&           _a_cli,
            cli_results_t&                       _a_cli_results
        ) const noexcept
    {
        using namespace std;
        if (_a_args.size() != 1)
        {
            _a_cli_results.add_error(fmt::format(
                "process_args expected single argument, however the vector "
                "provided was "
                "in the form {0}.",
                _a_args
            ));
            return true;
        }
        else
        {
            if (const optional<U> _l_parse_result{_m_processing_info.parser_func(_a_args[0])};
                _l_parse_result.has_value())
            {
                if (const optional<string> _l_process_okay{ _m_processing_info.process_parsed_value_func(
                        _a_option_class.*_m_member_var, _l_parse_result.value()
                    )};
                    _l_process_okay)
                {
                    _a_cli_results.add_error(fmt::format(
                        "Some post-parsing processing error encountered. "
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
                    "Could not parse std::string \"{0}\" to type {1}. "
                    "Reason:",
                    _a_args[0],
                    typeid(T).name()
                ));
                return true;
            }
        }
    }
private:
    T Option_Class::*                                       _m_member_var;
    cli_argument_processing_info_t<T, U> _m_processing_info;
};

template <typename Option_Class, typename T, typename U>
class cli_multi_args : public cli_info_t<Option_Class>
{
public:
    __constexpr
    cli_multi_args(
        const cli_option_config_t& _a_cli_option_config,
        T Option_Class::* _a_member_var,
        const cli_argument_processing_info_t<T,U>& _a_processing_info
    ) noexcept
        : cli_info_t<Option_Class>(
              enum_n_arguments::MULTI_ARGUMENT,
              _a_cli_option_config
          )
        , _m_member_var(_a_member_var)
        , _m_processing_info(_a_processing_info)
    {}

    __constexpr virtual std::optional<std::string>
        print(
            const Option_Class& _a_option_class
        ) const noexcept
    {
        using namespace abc::utility::printer;
        return _m_processing_info.print_func(_a_option_class.*_m_member_var);
    }

    __no_constexpr_imp virtual bool
        process_args(
            Option_Class&                        _a_option_class,
            const std::string_view               _a_flag,
            const std::vector<std::string_view>& _a_args,
            const cli_t<Option_Class>&           _a_cli,
            cli_results_t&                       _a_cli_results
        ) const noexcept
    {
        using namespace std;
        for (const string_view _l_arg : _a_args)
        {
            if (const optional<U> _l_parse_result{_m_processing_info.parser_func(_l_arg)};
                _l_parse_result.has_value())
            {
                if (const bool _l_process_okay{ _m_processing_info.process_parsed_value_func(
                        _a_option_class.*_m_member_var, _l_parse_result.value()
                    )};
                    _l_process_okay)
                {
                    _a_cli_results.add_error(fmt::format(
                        "Some post-parsing processing error encountered. "
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
                    "Could not parse std::string \"{0}\" to type {1}. "
                    "Reason:",
                    _l_arg,
                    typeid(T).name()
                ));
                return true;
            }
        }
        return false;
    }
private:
    T Option_Class::*                                       _m_member_var;
    cli_argument_processing_info_t<T, U> _m_processing_info;
};

_END_ABC_NS