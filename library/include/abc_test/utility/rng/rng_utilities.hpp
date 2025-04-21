#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include <vector>

_BEGIN_ABC_UTILITY_NS
using seed_t                 = std::vector<uint32_t>;
using rng_counter_t          = std::size_t;
using complete_global_seed_t = std::variant<unsigned int, seed_t>;
using global_seed_t          = std::optional<complete_global_seed_t>;

__constexpr complete_global_seed_t
    set_complete_global_seed(const global_seed_t& _a_global_seed) noexcept;
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
__constexpr complete_global_seed_t
    set_complete_global_seed(
        const global_seed_t& _a_global_seed
    ) noexcept
{
    return _a_global_seed.value_or(
        complete_global_seed_t(static_cast<unsigned int>(time(0)))
    );
}

_END_ABC_UTILITY_NS