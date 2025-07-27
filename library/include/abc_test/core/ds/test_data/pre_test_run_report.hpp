#pragma once
#include "abc_test/included_instances/options/option_config.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/cli/cli_option_config.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"
_BEGIN_ABC_NS
struct included_instances_test_options_t;
_END_ABC_NS
_BEGIN_ABC_DS_NS

class pre_test_run_report_t
{
public:
    __no_constexpr
        pre_test_run_report_t(
            const memoized_cli_history_t&            _a_memoized_data,
            const test_options_base_t* _a_test_options
        ) noexcept;
    __constexpr const std::vector<std::u8string>&
                      get_option_data(const _ABC_NS_CLI::cli_option_config_t& _a_element
                      ) const noexcept;
    __constexpr const test_options_base_t&
        get_options() const noexcept;
    __constexpr void
        report_all_tests(const std::size_t _a_size) noexcept;
private:
    memoized_cli_history_t _m_memoized_option_data;
    const test_options_base_t* _m_options;
    std::size_t _m_n_tests{0};
};

_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
__no_constexpr_imp
    pre_test_run_report_t::pre_test_run_report_t(
        const memoized_cli_history_t&            _a_memoized_option_data,
        const test_options_base_t* _a_test_options
    ) noexcept
    : _m_memoized_option_data(_a_memoized_option_data)
    , _m_options(_a_test_options)
{}

__constexpr const test_options_base_t&
    pre_test_run_report_t::get_options() const noexcept
{
    return *_m_options;
}

__constexpr void
    pre_test_run_report_t::report_all_tests(
        const std::size_t _a_size
    ) noexcept
{
    _m_n_tests = _a_size;
}
__constexpr const std::vector<std::u8string>&
                  pre_test_run_report_t::get_option_data(
                      const _ABC_NS_UTILITY_CLI::cli_option_config_t& _a_element
    ) const noexcept
{
    using namespace std;
    return get<1>(_m_memoized_option_data.at(u8string(_a_element.flag)).back()
    );
}

_END_ABC_DS_NS