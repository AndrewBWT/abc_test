#pragma once

#include "abc_test/utility/rng/inner_rng.hpp"

_BEGIN_ABC_UTILITY_NS

/*!
 * @brief The included concrete instantiation of inner_rng_t, meant for general
 * use with abc_test.
 */
class inner_rng_mt19937_64_t : public inner_rng_t
{
public:
    /*!
     * @brief Constructor, which sets the seed using the given value.
     * @param _a_seed The seed used to set the RNG.
     */
    __no_constexpr
        inner_rng_mt19937_64_t(const utility::seed_t& _a_seed) noexcept;
    /*!
     * @brief Construcotr which sets the inner RNG using the given argument.
     * @param _a_rng The RNG copied to the internal RNG.
     */
    __no_constexpr
        inner_rng_mt19937_64_t(const std::mt19937_64& _a_rng) noexcept;
    /*!
     * @brief Calls the internal rng and progresses it by the argument given.
     * @param _a_n_to_progress The number of times the RNG should be progressed.
     */
    __no_constexpr virtual void
        progress(const std::size_t _a_n_to_progress) noexcept override;
    __no_constexpr virtual void
        set_seed(const seed_t& _a_seed) noexcept override;
    /*!
     * @brief Calls the internal RNG and returns a randomly generated integer.
     * @return The randomly generated integer.
     */
    __no_constexpr virtual std::mt19937_64::result_type
        operator()() noexcept override;
    /*!
     * @brief Function which performs a deep copy of the internal RNG.
     * @return The copied RNG. Guaranteed to not be nullptr.
     */
    __no_constexpr virtual std::unique_ptr<inner_rng_t>
        deep_copy() const noexcept override;
private:
    std::mt19937_64 _m_rng;
    /*
     * Cannot be constexpr due to use of std::mt19937_64 constructor taking a
     * seed_seq.
     */
    __no_constexpr
        inner_rng_mt19937_64_t(const std::seed_seq& _a_seed_seq) noexcept;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
__no_constexpr_imp
    inner_rng_mt19937_64_t::inner_rng_mt19937_64_t(
        const utility::seed_t& _a_seed
    ) noexcept
    : inner_rng_mt19937_64_t(std::seed_seq(_a_seed.begin(), _a_seed.end()))
// : _m_rng(std::seed_seq(_a_seed.begin(), _a_seed.end());)
{}

__no_constexpr_imp
    inner_rng_mt19937_64_t::inner_rng_mt19937_64_t(
        const std::mt19937_64& _a_rng
    ) noexcept
    : _m_rng(_a_rng)
{}

__no_constexpr_imp void
    inner_rng_mt19937_64_t::progress(
        const std::size_t _a_n_to_progress
    ) noexcept
{
    _m_rng.discard(_a_n_to_progress);
}

__no_constexpr_imp void
    inner_rng_mt19937_64_t::set_seed(
        const seed_t& _a_seed
    ) noexcept
{
    using namespace std;
    seed_seq _l_seed_seq(_a_seed.begin(), _a_seed.end());
    _m_rng.seed(_l_seed_seq);
}

__no_constexpr_imp std::mt19937_64::result_type
                   inner_rng_mt19937_64_t::operator()() noexcept
{
    return _m_rng();
}

__no_constexpr_imp std::unique_ptr<inner_rng_t>
                   inner_rng_mt19937_64_t::deep_copy() const noexcept
{
    return make_unique<inner_rng_mt19937_64_t>(_m_rng);
}

__no_constexpr_imp
    inner_rng_mt19937_64_t::inner_rng_mt19937_64_t(
        const std::seed_seq& _a_seed_seq
    ) noexcept
    : _m_rng(_a_seed_seq)
{}

_END_ABC_UTILITY_NS