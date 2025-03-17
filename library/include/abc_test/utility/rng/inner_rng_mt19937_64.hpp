#pragma once

#include "abc_test/utility/rng/inner_rng.hpp"

_BEGIN_ABC_UTILITY_NS

class inner_rng_mt19937_64_t : public inner_rng_t
{
public:
    __no_constexpr virtual void
        progress(const std::size_t _a_n_to_progress) noexcept;
    __no_constexpr
    inner_rng_mt19937_64_t(const utility::seed_t& _a_seed);
    __no_constexpr virtual void
        set_seed(const seed_t& _a_seed) noexcept;
    __no_constexpr virtual std::mt19937_64::result_type
        operator()() noexcept;
private:
    std::mt19937_64 _m_rng;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
__no_constexpr_imp void
    inner_rng_mt19937_64_t::progress(
        const std::size_t _a_n_to_progress
    ) noexcept
{
    _m_rng.discard(_a_n_to_progress);
}

__no_constexpr_imp
    inner_rng_mt19937_64_t::inner_rng_mt19937_64_t(
        const utility::seed_t& _a_seed
    )
{
    std::seed_seq _l_seed_seq(_a_seed.begin(), _a_seed.end());
    _m_rng.seed(_l_seed_seq);
}

__no_constexpr_imp void
    inner_rng_mt19937_64_t::set_seed(
        const seed_t& _a_seed
    ) noexcept
{
    std::seed_seq _l_seed_seq(_a_seed.begin(), _a_seed.end());
    _m_rng.seed(_l_seed_seq);
}

__no_constexpr_imp std::mt19937_64::result_type
                inner_rng_mt19937_64_t::operator()() noexcept
{
    return _m_rng();
}

_END_ABC_UTILITY_NS