#pragma once
#include "abc_test/utility/rng.hpp"

namespace abc::utility
{
class simple_rng_t : public inner_rng_t
{
public:
    inline simple_rng_t(
        const seed_t& _a_seed
    )
        : _m_rng{ size_t{0} }
    {
        for (auto& _l_element : _a_seed)
        {
            _m_rng += _l_element;
        }
    }
    inline simple_rng_t(
        const std::size_t _a_rng
    )
        : _m_rng(_a_rng)
    {

    }
    inline void
        set_seed(
            const abc::utility::seed_t& _a_seed
        ) noexcept
    {
        _m_rng = 0;
        for (auto& _l_element : _a_seed)
        {
            _m_rng += _l_element;
        }
    }

    inline std::unique_ptr<inner_rng_t>
        make_rng(const seed_t& _a_seed) const noexcept
    {
        return make_unique<simple_rng_t>(_a_seed);
    }
    inline virtual void progress(const std::size_t _a_n_to_progress) noexcept
    {
        _m_rng += _a_n_to_progress;
    }
    inline std::mt19937_64::result_type
        operator()() noexcept
    {
        const std::mt19937_64::result_type _l_rv{
            static_cast<std::mt19937_64::result_type>(_m_rng)
        };
        ++_m_rng;
        return _l_rv;
    }
    inline std::unique_ptr<inner_rng_t>

        deep_copy() const noexcept
    {
        using namespace std;
        return make_unique<simple_rng_t>(_m_rng);
    }
private:
    std::size_t _m_rng{0};
};
} // namespace test