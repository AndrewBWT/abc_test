#pragma once

#include "abc_test/utility/internal/macros.hpp"

#include <vector>

_BEGIN_ABC_UTILITY_NS
/*!
 * @brief Type synonym for a seed type in abc_test.
 */
using seed_t                 = std::vector<uint32_t>;
/*!
 * @brief A type synonym for a random generator counter. This allows the
 * developer to base a random generaotr around this value. It will incrmeent by
 * 1 each time a RNG is called. By basing an RNG around this value, more
 * complicated objects can be created at higher counter values.
 */
using rng_counter_t          = std::size_t;
/*!
 * @brief A type snyonym for a complete global seed - specifically something
 * that can eitehr be initialsied using an unsigned integer or a seed_t object.
 */
using complete_global_seed_t = std::variant<unsigned int, seed_t>;
/*!
 * @brief A type synonym for a generic global_seed_t. This is either set a
 * complete_global_seed_t, or not set at all.
 */
using global_seed_t          = std::optional<complete_global_seed_t>;
/*!
 * @brief Creates a complete_global_seed_t from a global_seed_t object.
 *
 * If the argument is a std::nullopt, then this function will use time(0) to
 * create an instance of complete_global_seed_t using an unsigned integer.
 *
 * @param _a_global_seed The global_seed_t object used to create the
 * complete_global_seed_t returned.
 * @return The complete_global_seed_t returned.
 */
__constexpr complete_global_seed_t
    set_complete_global_seed(const global_seed_t& _a_global_seed) noexcept;
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
__constexpr_imp complete_global_seed_t
    set_complete_global_seed(
        const global_seed_t& _a_global_seed
    ) noexcept
{
    return _a_global_seed.value_or(
        complete_global_seed_t(static_cast<unsigned int>(time(0)))
    );
}

_END_ABC_UTILITY_NS