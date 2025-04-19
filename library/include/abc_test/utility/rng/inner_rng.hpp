#pragma once

#include "abc_test/utility/rng/types.hpp"
#include <random>

_BEGIN_ABC_UTILITY_NS

class inner_rng_t
{
public:
    virtual void progress(const std::size_t _a_n_to_progress) noexcept = 0;
    virtual void
        set_seed(const seed_t& _a_seed) noexcept
        = 0;
    virtual std::mt19937_64::result_type
        operator()() noexcept
        = 0;
    virtual std::shared_ptr<inner_rng_t> deep_copy() const noexcept = 0;
};
_END_ABC_UTILITY_NS