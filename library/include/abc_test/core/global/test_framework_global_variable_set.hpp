#pragma once

#include "abc_test/core/options/test_options_base.hpp"
#include "abc_test/core/reporters/error_reporter.hpp"
#include "abc_test/core/reporters/test_reporter.hpp"

_BEGIN_ABC_GLOBAL_NS

struct test_framework_global_variable_set_t
{
public:
    test_framework_global_variable_set_t() = delete;
    __no_constexpr
    test_framework_global_variable_set_t(
        const test_options_base_t&            _a_options,
        _ABC_NS_REPORTERS::error_reporters_t& _a_error_reporters,
        _ABC_NS_REPORTERS::test_reporters_t&  _a_test_reporters
    ) noexcept;
    __constexpr _ABC_NS_REPORTERS::error_reporter_controller_t&
        error_reporter_controller() noexcept;
    __constexpr _ABC_NS_REPORTERS::test_reporter_controller_t&
        test_reporter_controller() noexcept;
    __constexpr const test_options_base_t&
        test_options() const noexcept;
    __constexpr const _ABC_NS_UTILITY::complete_global_seed_t&
        global_seed() const noexcept;
private:
    _ABC_NS_REPORTERS::test_reporter_controller_t  _m_trc;
    _ABC_NS_REPORTERS::error_reporter_controller_t _m_erc;
    test_options_base_t                            _m_tob;
    _ABC_NS_UTILITY::complete_global_seed_t        _m_cgs;
};

_END_ABC_GLOBAL_NS

_BEGIN_ABC_GLOBAL_NS
__no_constexpr_imp
    test_framework_global_variable_set_t::test_framework_global_variable_set_t(
        const test_options_base_t&            _a_options,
        _ABC_NS_REPORTERS::error_reporters_t& _a_error_reporters,
        _ABC_NS_REPORTERS::test_reporters_t&  _a_test_reporters
    ) noexcept
    : _m_tob(_a_options)
{
    _m_erc.add_reporters(_a_error_reporters);
    _m_trc.add_reporters(_a_test_reporters);
    _m_cgs = _ABC_NS_UTILITY::set_complete_global_seed(_m_tob.global_seed);
}
__constexpr _ABC_NS_REPORTERS::error_reporter_controller_t&
test_framework_global_variable_set_t::error_reporter_controller() noexcept
{
    return _m_erc;
}
__constexpr _ABC_NS_REPORTERS::test_reporter_controller_t&
test_framework_global_variable_set_t::test_reporter_controller() noexcept
{
    return _m_trc;
}
__constexpr const test_options_base_t&
test_framework_global_variable_set_t::test_options() const noexcept
{
    return _m_tob;
}
__constexpr const _ABC_NS_UTILITY::complete_global_seed_t&
test_framework_global_variable_set_t::global_seed() const noexcept
{
    return _m_cgs;
}
_END_ABC_GLOBAL_NS