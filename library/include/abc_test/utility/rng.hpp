#pragma once

#include "abc_test/utility/rng/complete_global_seed.hpp"
#include "abc_test/utility/rng/inner_rng.hpp"

#include <memory>
#include <random>
#include <scn/scan.h>


_BEGIN_ABC_UTILITY_NS

struct rng_t
{
public:
    using result_type = std::mt19937_64::result_type;

    __constexpr static result_type
        min() noexcept;
    __constexpr static result_type
        max() noexcept;
    __no_constexpr rng_t
        make_rng(const std::size_t _a_n_elements_to_take);

    __constexpr void
        progress(const size_t _a_expected_calls) noexcept;
    template<typename T>
    __constexpr T in_range_inclusive(const T& _a_lower, const T& _a_higher) const noexcept;
    __constexpr result_type
        operator()();

    template <typename Rng>
    __constexpr static rng_t
        make_rng(
            const complete_global_seed_t& _a_global_seed,
            const std::size_t             _a_seed_size
        ) noexcept;

    template <typename Rng>
    __constexpr static rng_t
        make_rng(const seed_t& _a_seed) noexcept;
    __constexpr
    rng_t() noexcept
        = delete;

    __constexpr size_t
        calls() const noexcept;
private:
    std::shared_ptr<inner_rng_t> _m_rng;
    // std::mt19937_64 _m_rnd;
    utility::seed_t _m_seed;
    size_t          _m_calls;

    __no_constexpr
    rng_t(
        const std::shared_ptr<inner_rng_t>& _a_rng,
        const utility::seed_t&              _a_seed
    );
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

__constexpr_imp rng_t::result_type
                rng_t::min() noexcept
{
    return std::mt19937_64::min();
}

__constexpr_imp rng_t::result_type
                rng_t::max() noexcept
{
    return std::mt19937_64::max();
}

__no_constexpr_imp rng_t
    rng_t::make_rng(
        const std::size_t _a_n_elements_to_take
    )
{
    seed_t _l_seed(_a_n_elements_to_take);
    for (size_t _l_idx{0}; _l_idx < _a_n_elements_to_take; ++_l_idx)
    {
        _l_seed[_l_idx] = _m_rng.get()->operator()();
    }
    std::shared_ptr<inner_rng_t> _l_rng_cpy{_m_rng};
    _l_rng_cpy->set_seed(_l_seed);
    return utility::rng_t(_l_rng_cpy, _l_seed);
}

__constexpr_imp void
    rng_t::progress(
        const size_t _a_expected_calls
    ) noexcept
{
    if (_m_calls > _a_expected_calls)
    {
        _m_rng->set_seed(_m_seed);
        _m_calls  = 0;
        _m_calls += _a_expected_calls;
        _m_rng.get()->progress(_a_expected_calls);
    }
    _m_calls += _a_expected_calls;
    _m_rng.get()->progress(_a_expected_calls);
}
template<typename T>
__constexpr T rng_t::in_range_inclusive(const T& _a_lower, const T& _a_higher) const noexcept
{
    const result_type _l_rv{ operator() };

}
__constexpr_imp rng_t::result_type
                rng_t::operator()()
{
    ++_m_calls;
    return _m_rng.get()->operator()();
}

template <typename Rng>
__constexpr_imp rng_t
    rng_t::make_rng(
        const complete_global_seed_t& _a_global_seed,
        const std::size_t             _a_seed_size
    ) noexcept
{
    using namespace std;
    const utility::seed_t _l_seed{_a_global_seed.get_seed(_a_seed_size)};
    return rng_t(make_shared<Rng>(_l_seed), _l_seed);
}

template <typename Rng>
__constexpr_imp rng_t
    rng_t::make_rng(
        const seed_t& _a_seed
    ) noexcept
{
    return rng_t(make_shared<Rng>(_a_seed), _a_seed);
}

__constexpr_imp size_t
    rng_t::calls() const noexcept
{
    return _m_calls;
}

__no_constexpr_imp
    rng_t::rng_t(
        const std::shared_ptr<inner_rng_t>& _a_rng,
        const utility::seed_t&              _a_seed
    )
    : _m_rng(_a_rng), _m_seed(_a_seed), _m_calls(0)
{}

_END_ABC_UTILITY_NS