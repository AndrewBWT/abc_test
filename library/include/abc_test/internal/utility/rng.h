#pragma once

#include "abc_test/internal/utility/internal/macros.h"

#include <random>

_BEGIN_ABC_UTILITY_NS
using global_seed_t
    = std::variant<std::monostate, unsigned int, std::vector<uint32_t>>;
using seed_t = std::vector<uint32_t>;

struct rng
{
public:
    using result_type = std::mt19937_64::result_type;

    static result_type
        min() noexcept
    {
        return std::mt19937_64::min();
    }

    static result_type
        max() noexcept
    {
        return std::mt19937_64::max();
    }

    inline void
        progress(
            const size_t _a_expected_calls
        ) noexcept
    {
        if (_m_calls > _a_expected_calls)
        {
            std::seed_seq _l_seed_seq(_m_seed.begin(), _m_seed.end());
            _m_rnd   = std::mt19937_64(_l_seed_seq);
            _m_calls = 0;
            while (_m_calls < _a_expected_calls)
            {
                ++_m_calls;
                auto _l_res = _m_rnd();
            }
        }
        while (_m_calls < _a_expected_calls)
        {
            ++_m_calls;
            auto _l_res = _m_rnd();
        }
    }

    inline result_type
        operator()()
    {
        ++_m_calls;
        return _m_rnd();
    }

    inline rng() noexcept
        : rng(seed_t())
    {}

    inline rng(
        const global_seed_t& _a_global_seed,
        const std::size_t _a_seed_to_create_size
    )
        : _m_rnd(), _m_seed({}), _m_calls(0)
    {
        using namespace std;
        if (auto _l_ptr{get_if<monostate>(&_a_global_seed)}; _l_ptr != nullptr)
        {
            _m_seed.resize(_a_seed_to_create_size);
            std::srand(std::time(0));
            for (size_t _l_idx{0};
                 _l_idx
                 < _a_seed_to_create_size;
                 ++_l_idx)
            {
                _m_seed[_l_idx] = std::rand();
            }
        }
        else if (auto _l_ptr{ get_if<unsigned int>(&_a_global_seed) }; _l_ptr != nullptr)
        {
            _m_seed.resize(_a_seed_to_create_size);
            std::srand(*_l_ptr);
            for (size_t _l_idx{ 0 };
                _l_idx
                < _a_seed_to_create_size;
                ++_l_idx)
            {
                _m_seed[_l_idx] = std::rand();
            }
        }
        else if (auto _l_ptr{ get_if<std::vector<uint32_t>>(&_a_global_seed) }; _l_ptr != nullptr)
        {
            _m_seed = *_l_ptr;
        }
        else
        {
            
        }
        std::seed_seq _l_seed_seq(_m_seed.begin(), _m_seed.end());
        _m_rnd = std::mt19937_64(_l_seed_seq);
    }

    inline rng(
        const seed_t& _a_seed
    ) noexcept
        : _m_rnd(), _m_seed(_a_seed), _m_calls(0)
    {
        std::seed_seq _l_seed_seq(_m_seed.begin(), _m_seed.end());
        _m_rnd = std::mt19937_64(_l_seed_seq);
    }

    constexpr size_t
        calls() const noexcept
    {
        return _m_calls;
    }
private:
    std::mt19937_64 _m_rnd;
    utility::seed_t _m_seed;
    size_t          _m_calls;
};

_END_ABC_UTILITY_NS