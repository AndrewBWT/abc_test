#pragma once

#include "abc_test/utility/rng/rng_utilities.hpp"

#include <random>

_BEGIN_ABC_UTILITY_NS

/*!
 * @brief Abstract class which represents the RNG.
 */
class inner_rng_t
{
public:
    /*!
     * @brief Advances the RNG by the given number of advancements.
     * @param _a_n_to_progress The number of number generations the state of the
     * RNG will advance by.
     * 
     */
    virtual void
        progress(const std::size_t _a_n_to_progress) noexcept
        = 0;
    /*!
     * @brief Sets the seed of the internal RNG.
     * @param _a_seed The seed used to set the RNG.
     */
    virtual void
        set_seed(const seed_t& _a_seed) noexcept
        = 0;
    /*!
     * @brief Generates a random number of the type
     * std::mt19937_64::result_type.
     * @return The randomly generated number.
     */
    virtual std::mt19937_64::result_type
        operator()() noexcept
        = 0;
    /*!
     * @brief Copies this object to a new shared ptr containing an inner_rng_t
     * object.
     * @return The copy of this.
     * 
     * This should be guaranteed not to be a nullptr.
     */
    virtual std::unique_ptr<inner_rng_t>
        deep_copy() const noexcept = 0;
    virtual std::unique_ptr<inner_rng_t>
        make_rng(const seed_t& _a_seed) const noexcept = 0;
};

_END_ABC_UTILITY_NS