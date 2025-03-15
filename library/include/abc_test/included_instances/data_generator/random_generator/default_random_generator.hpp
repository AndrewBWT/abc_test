#pragma once

#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"
#include "abc_test/utility/bounds.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"
#include "concepts"

#include <memory>
#include <optional>

_BEGIN_ABC_DG_NS

template <typename T>
struct default_random_generator_t : public random_generator_base_t<T>
{
public:
    __constexpr virtual T
        operator()(
            utility::rng&     _a_rnd_generator,
            const std::size_t _a_index
        )
    {
        __STATIC_ASSERT(
            T,
            "Function requires a specialization of the function "
            "operator<T>(utility::rng, const std::size_t)."
        );
    }
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T, typename... Ts>
__constexpr data_gen::random_generator_t<T>
            default_random_generator(Ts... elements) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS

template <>
struct default_random_generator_t<bool> : public random_generator_base_t<bool>
{
    __no_constexpr virtual bool
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
};

template <typename T>
requires std::integral<T>
struct default_random_generator_t<T> : public random_generator_base_t<T>
{
public:
    __constexpr
    default_random_generator_t(
        const T& _a_min = std::numeric_limits<T>::min(),
        const T& _a_max = std::numeric_limits<T>::max()
    ) noexcept
        : _m_min(_a_min), _m_max(_a_max), _m_range(_m_max - _m_min)
    {}

    __constexpr_imp virtual T
        operator()(
            utility::rng&     _a_rnd_generator,
            const std::size_t _a_index
        )
    {
        using namespace std;
        T _l_val{static_cast<T>(_a_rnd_generator() % _m_range)};
        return _m_min + _l_val;
    }
private:
    T _m_min;
    T _m_max;
    T _m_range;
};

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T> && _ABC_NS_UTILITY::min_value_c<T>
         && _ABC_NS_UTILITY::max_value_c<T>
struct default_random_generator_t<T> : public random_generator_base_t<T>
{
public:
    __constexpr
    default_random_generator_t(
        const T& _a_min = _ABC_NS_UTILITY::min_value_t<T>().min_value(),
        const T& _a_max = _ABC_NS_UTILITY::max_value_t<T>().max_value()
    ) noexcept
        : _m_min(_a_min)
        , _m_max(_a_max)
        , _m_range(_ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
                       .difference(_a_min, _a_max))
    {}

    __no_constexpr_imp virtual T
        operator()(
            utility::rng&     _a_rnd_generator,
            const std::size_t _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        const size_t _l_val{static_cast<size_t>(_a_rnd_generator() % _m_range)};
        T            _l_rv{_m_min};
        get_thread_local_enumerate_enum_helper<T>().increment(_l_rv, _l_val);
        return _l_rv;
    }
private:
    T _m_min;
    T _m_max;
    T _m_range;
};

template <typename T, typename... Ts>
__constexpr T
    make_random_object(
        utility::rng&                         _a_rng,
        const std::size_t                     _a_size,
        std::tuple<random_generator_t<Ts>...> _a_generators
    ) noexcept;

namespace
{
template <std::size_t I, typename... Ts>
__constexpr void
    run_internal_make_object(
        utility::rng&                         _a_rng,
        const std::size_t                     _a_size,
        std::tuple<Ts...>&                    _a_tuple,
        std::tuple<random_generator_t<Ts>...> _a_generators
    ) noexcept;
} // namespace

_END_ABC_DG_NS

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
        for (size_t _l_idx{0}; _l_idx < _l_size_of_str; ++_l_idx)
        {
            _l_rv.push_back(default_random_generator_t<char>{}.operator()(
                _a_rnd_generator, _a_index
            ));
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
    default_random_generator_t(random_generator_t<Ts>... _a_rngs);
    __constexpr
    default_random_generator_t()
     requires (
     std::is_default_constructible_v<default_random_generator_t<Ts>> && ...
     );
    template <std::size_t I>
    __constexpr void
        run_rng_internal(
            value_type&       _a_object,
            utility::rng&     _a_rnd_generator,
            const std::size_t _a_index
        ) const;
    __no_constexpr virtual value_type
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
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


_BEGIN_ABC_DG_NS

template <typename T>
struct default_random_generator_t<std::optional<T>>
    : public random_generator_base_t<std::optional<T>>
{
public:
    __constexpr
    default_random_generator_t()
    requires (std::is_default_constructible_v<default_random_generator_t<T>>)
        : _m_rng(mk_random_generator(default_random_generator_t<T>()))
    {}

    __no_constexpr virtual std::optional<T>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T> _m_rng;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
__no_constexpr_imp std::optional<T>
                   default_random_generator_t<std::optional<T>>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    if (_a_index == 0)
    {
        return optional<T>{};
    }
    else
    {
        bool _l_is_opt{(_a_rnd_generator() % 2) == 0};
        if (_l_is_opt)
        {
            return optional<T>{};
        }
        else
        {
            const T _l_elem{_m_rng->operator()(_a_rnd_generator, _a_index)};
            return optional<T>{_l_elem};
        }
    }
}

/*template <typename T>
requires std::integral<T>
__constexpr_imp
    random_generator_object_t<T>::random_generator_object_t(
        const T& _a_min,
        const T& _a_max
    ) noexcept
    : _m_min(_a_min), _m_max(_a_max), _m_range(_m_max - _m_min)
{}

template <typename T>
requires std::integral<T>
__no_constexpr_imp T
    random_generator_object_t<T>::operator()(
        utility::rng& _a_rnd_generator
    )
{
    using namespace std;
    T _l_val{static_cast<T>(_a_rnd_generator() % _m_range)};
    return _m_min + _l_val;
}
*/
_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
struct default_random_generator_t<std::basic_string<T>>
    : public random_generator_base_t<std::basic_string<T>>
{
public:
    __constexpr_imp
        default_random_generator_t() noexcept;
    __constexpr
    default_random_generator_t(
        const random_generator_t<T>&          _a_rnd_gen,
        const utility::bounds_t<std::size_t>& _a_size_bounds
        = utility::bounds_t<std::size_t>()
    ) noexcept;
    __no_constexpr virtual std::basic_string<T>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T>          _m_rng;
    utility::bounds_t<std::size_t> _m_bounds;
};

template <typename T>
struct default_random_generator_t<std::vector<T>>
    : public random_generator_base_t<std::vector<T>>
{
public:
    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;
    __no_constexpr virtual std::vector<T>
        operator()(utility::rng& _a_rnd_generator, const std::size_t _a_index);
private:
    random_generator_t<T> _m_gen;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp
    default_random_generator_t<
        std::basic_string<T>>::default_random_generator_t() noexcept
    : default_random_generator_t<std::basic_string<T>>(
          default_random_generator<T>(),
          utility::bounds_t<std::size_t>()
      )
{}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp
    default_random_generator_t<std::basic_string<T>>::
        default_random_generator_t(
            const random_generator_t<T>&          _a_rnd_gen,
            const utility::bounds_t<std::size_t>& _a_size_bounds
        ) noexcept
    : _m_rng(_a_rnd_gen), _m_bounds(_a_size_bounds)
{}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
__no_constexpr_imp std::basic_string<T>
                   default_random_generator_t<std::basic_string<T>>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    // Decide on offset size. Use _a_index as upper bound. We add 1 so that we
    // don't go over the limit.
    const size_t _l_str_size_offset{
        _a_index == 0 ? 0 : (_a_rnd_generator() % (_a_index + 1))
    };
    const size_t _l_str_size{
        _m_bounds.lower() + (_l_str_size_offset % _m_bounds.difference())
    };
    basic_string<T> _l_rv(_l_str_size, T{});
    for (size_t _l_idx{0}; _l_idx < _l_str_size; ++_l_idx)
    {
        _l_rv[_l_idx] = _m_rng->operator()(_a_rnd_generator, _a_index);
    }
    return _l_rv;
}

/*template <typename T>
requires std::integral<T>
__constexpr_imp
    random_generator_object_t<T>::random_generator_object_t(
        const T& _a_min,
        const T& _a_max
    ) noexcept
    : _m_min(_a_min), _m_max(_a_max), _m_range(_m_max - _m_min)
{}

template <typename T>
requires std::integral<T>
__no_constexpr_imp T
    random_generator_object_t<T>::operator()(
        utility::rng& _a_rnd_generator
    )
{
    using namespace std;
    T _l_val{static_cast<T>(_a_rnd_generator() % _m_range)};
    return _m_min + _l_val;
}
*/

template <typename T>
__constexpr_imp
    default_random_generator_t<std::vector<T>>::default_random_generator_t(
        const random_generator_t<T>& _a_gen
    ) noexcept
    : _m_gen(_a_gen)
{}

template <typename T>
__no_constexpr_imp std::vector<T>
                   default_random_generator_t<std::vector<T>>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    const size_t _l_size{_a_rnd_generator() % _a_index};
    vector<T>    _l_rv(_l_size);
    for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
    {
        _l_rv[_l_idx] = _m_gen(_a_rnd_generator, _a_index);
    }
    return _l_rv;
}

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

/*template <typename T>
__constexpr_imp T
    default_random_generator_t<T>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "operator<T>(utility::rng, const std::size_t)."
    );
}*/

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename... Ts>
__constexpr data_gen::random_generator_t<T>
            default_random_generator(
                Ts... elements
            ) noexcept
{
    using namespace std;
    using namespace data_gen;
    return make_shared<default_random_generator_t<T>>(elements...);
}

_END_ABC_NS

_BEGIN_ABC_DG_NS
__no_constexpr_imp bool
    default_random_generator_t<bool>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    using namespace std;
    size_t _l_val{_a_rnd_generator() % 2};
    return _l_val == 0 ? false : true;
}

/*template <typename T>
requires std::integral<T>
__constexpr_imp
    random_generator_object_t<T>::random_generator_object_t(
        const T& _a_min,
        const T& _a_max
    ) noexcept
    : _m_min(_a_min), _m_max(_a_max), _m_range(_m_max - _m_min)
{}

template <typename T>
requires std::integral<T>
__no_constexpr_imp T
    random_generator_object_t<T>::operator()(
        utility::rng& _a_rnd_generator
    )
{
    using namespace std;
    T _l_val{static_cast<T>(_a_rnd_generator() % _m_range)};
    return _m_min + _l_val;
}

template <typename T>
    requires _ABC_NS_UTILITY::enum_has_list_c<T>&&
_ABC_NS_UTILITY::min_value_c<T>
&& _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp
default_random_generator_t<T>::default_random_generator_t(
    const T& _a_min,
    const T& _a_max
) noexcept
    : _m_min(_a_min)
    , _m_max(_a_max)
    , _m_range(_ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
        .difference(_a_min, _a_max))
{
}

template <typename T>
    requires _ABC_NS_UTILITY::enum_has_list_c<T>&&
_ABC_NS_UTILITY::min_value_c<T>
&& _ABC_NS_UTILITY::max_value_c<T>
__no_constexpr_imp T
default_random_generator_t<T>::operator()(
    utility::rng& _a_rnd_generator,
    const std::size_t _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    const size_t _l_val{ static_cast<size_t>(_a_rnd_generator() % _m_range) };
    T            _l_rv{ _m_min };
    get_thread_local_enumerate_enum_helper<T>().increment(_l_rv, _l_val);
    return _l_rv;
}*/

template <typename T, typename... Ts>
__constexpr_imp T
    make_random_object(
        utility::rng&                         _a_rng,
        const std::size_t                     _a_size,
        std::tuple<random_generator_t<Ts>...> _a_generators
    ) noexcept
{
    using namespace std;
    tuple<Ts...> _l_data_elements;
    run_internal_make_object<0>(
        _a_rng, _a_size, _l_data_elements, _a_generators
    );
    return make_from_tuple<T>(_l_data_elements);
}

namespace
{
template <std::size_t I, typename... Ts>
__constexpr void
    run_internal_make_object(
        utility::rng&                         _a_rng,
        const std::size_t                     _a_size,
        std::tuple<Ts...>&                    _a_tuple,
        std::tuple<random_generator_t<Ts>...> _a_generators
    ) noexcept
{
    using namespace std;
    get<I>(_a_tuple) = get<I>(_a_generators)->operator()(_a_rng, _a_size);
    if constexpr (I + 1 < tuple_size<tuple<Ts...>>{})
    {
        run_internal_make_object<I + 1>(
            _a_rng, _a_size, _a_tuple, _a_generators
        );
    }
}
} // namespace

template <typename... Ts>
__constexpr_imp
    default_random_generator_t<std::tuple<Ts...>>::default_random_generator_t(
        random_generator_t<Ts>... _a_rngs
    )
    : _m_rngs(_a_rngs)
{}

template <typename... Ts>
__constexpr_imp
    default_random_generator_t<std::tuple<Ts...>>::default_random_generator_t()
     requires (
         std::is_default_constructible_v<default_random_generator_t<Ts>> &&
         ...
    )
    : _m_rngs(std::make_tuple(
          mk_random_generator(default_random_generator_t<Ts>())...
      ))
{}

template <typename... Ts>
template <std::size_t I>
__constexpr void
    default_random_generator_t<std::tuple<Ts...>>::run_rng_internal(
        value_type&       _a_object,
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    ) const
{
    using namespace std;
    get<I>(_a_object) = get<I>(_m_rngs)->operator()(_a_rnd_generator, _a_index);
    if constexpr (I + 1 < tuple_size<value_type>{})
    {
        run_rng_internal<I + 1>(_a_object, _a_rnd_generator, _a_index);
    }
}

template <typename... Ts>
__no_constexpr_imp default_random_generator_t<std::tuple<Ts...>>::value_type
                   default_random_generator_t<std::tuple<Ts...>>::operator()(
        utility::rng&     _a_rnd_generator,
        const std::size_t _a_index
    )
{
    value_type _l_rv;
    run_rng_internal<0>(_l_rv, _a_rnd_generator, _a_index);
    return _l_rv;
}

_END_ABC_DG_NS