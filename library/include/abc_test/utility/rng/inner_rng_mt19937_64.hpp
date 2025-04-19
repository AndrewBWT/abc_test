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
    __no_constexpr std::shared_ptr<inner_rng_t>
                            deep_copy() const noexcept override;
    __no_constexpr
        inner_rng_mt19937_64_t(const std::mt19937_64& _a_rng);
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
__no_constexpr_imp std::shared_ptr<inner_rng_t>
inner_rng_mt19937_64_t::deep_copy() const noexcept
{
    return make_shared<inner_rng_mt19937_64_t>(this->_m_rng);
}
__no_constexpr_imp
inner_rng_mt19937_64_t::inner_rng_mt19937_64_t(const std::mt19937_64& _a_rng)
    : _m_rng(_a_rng)
{

}
_END_ABC_UTILITY_NS