#pragma once

// #include
// "abc_test/included_instances/options/included_instances_test_options.hpp"
#include "abc_test/included_instances/options/option_config.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"
_BEGIN_ABC_NS
class included_instances_test_options_t;
_END_ABC_NS
_BEGIN_ABC_DS_NS

class pre_test_run_report_t
{
public:
    __no_constexpr
        pre_test_run_report_t(
            const memoized_cli_history_t&            _a_memoized_data,
            const included_instances_test_options_t& _a_test_options
        ) noexcept;
    __constexpr const std::vector<std::string>&
                      get_option_data(const detail::option_config_t& _a_element
                      ) const noexcept;
    __constexpr const included_instances_test_options_t&
        get_options() const noexcept;
private:
    memoized_cli_history_t _m_memoized_option_data;
    std::reference_wrapper<const included_instances_test_options_t> _m_options;
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp
    pre_test_run_report_t::pre_test_run_report_t(
        const memoized_cli_history_t&            _a_memoized_option_data,
        const included_instances_test_options_t& _a_test_options
    ) noexcept
    : _m_memoized_option_data(_a_memoized_option_data)
    , _m_options(std::reference_wrapper(_a_test_options))
{}

__constexpr const included_instances_test_options_t&
    pre_test_run_report_t::get_options() const noexcept
{
    return _m_options.get();
}

__constexpr const std::vector<std::string>&
                  pre_test_run_report_t::get_option_data(
        const detail::option_config_t& _a_element
    ) const noexcept
{
    using namespace std;
    return get<1>(_m_memoized_option_data.at(string(get<0>(_a_element))).back()
    );
}

_END_ABC_DS_NS