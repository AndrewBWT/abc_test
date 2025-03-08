#pragma once

#include "abc_test/included_instances/data_generator/random_generator/default_random_generator.hpp"

_BEGIN_ABC_DG_NS

template <typename T, std::size_t N>
struct default_random_generator_t<std::array<T, N>>
    : public random_generator_base_t<std::array<T, N>>
{
public:
    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;
    __no_constexpr virtual std::array<T, N>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
requires (std::convertible_to<T, std::string_view>)
struct default_random_generator_t<T> : public random_generator_base_t<T>
{
    __no_constexpr virtual T
        operator()(
            utility::rng&     _a_rnd_generator,
            const std::size_t _a_index
        )
    {
        T            _l_rv;
        const size_t _l_size_of_str{
            _a_index == 0 ? 0 : (_a_rnd_generator() % _a_index)
        };
        for (size_t _l_idx{ 0 }; _l_idx < _l_size_of_str; ++_l_idx)
        {
            _l_rv.push_back(default_random_generator_t<char>{}.operator()(_a_rnd_generator, _a_index));
        }
        return _l_rv;
    }
};

template <typename... Ts>
struct default_random_generator_t<std::tuple<Ts...>>
    : public random_generator_base_t<std::tuple<Ts...>>
{
    using value_type = std::tuple<Ts...>;

    __constexpr
    default_random_generator_t(
        random_generator_t<Ts>... _a_rngs
    )
        : _m_rngs(_a_rngs)
    {}

    __constexpr
    default_random_generator_t()
    requires (
        std::is_default_constructible_v<default_random_generator_t<Ts>> && ...
    )
        : _m_rngs(std::make_tuple(
              mk_random_generator(default_random_generator_t<Ts>())...
          ))
    {}

    template <std::size_t I>
    __constexpr void
        run_rng_internal(
            value_type&       _a_object,
            utility::rng&     _a_rnd_generator,
            const std::size_t _a_index
        ) const
    {
        using namespace std;
        get<I>(_a_object)
            = get<I>(_m_rngs)->operator()(_a_rnd_generator, _a_index);
        if constexpr (I + 1 < tuple_size<value_type>{})
        {
            run_rng_internal<I + 1>(_a_object, _a_rnd_generator, _a_index);
        }
    }

    __no_constexpr_imp virtual value_type
        operator()(
            utility::rng&     _a_rnd_generator,
            const std::size_t _a_index
        )
    {
        value_type _l_rv;
        run_rng_internal<0>(_l_rv, _a_rnd_generator, _a_index);
        return _l_rv;
    }
private:
    std::tuple<random_generator_t<Ts>...> _m_rngs;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T, std::size_t N>
__constexpr_imp
    default_random_generator_t<std::array<T, N>>::default_random_generator_t(
        const random_generator_t<T>& _a_gen
    ) noexcept
    : _m_gen(_a_gen)
{}

template <typename T, std::size_t N>
__no_constexpr_imp std::array<T, N>
                   default_random_generator_t<std::array<T, N>>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    array<T, N> _l_rv{};
    for (size_t _l_idx{0}; _l_idx < N; ++_l_idx)
    {
        _l_rv[_l_idx] = (*_m_gen)(_a_rnd_generator, _a_index)();
    }
    return _l_rv;
}

_END_ABC_DG_NS