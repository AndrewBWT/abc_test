#pragma once

#include "abc_test/included_instances/data_generator/random_generator/default_random_generator.hpp"
#include "abc_test/internal/utility/enum.hpp"
#include "concepts"

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

    __no_constexpr virtual T
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

    __no_constexpr virtual T
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

_END_ABC_DG_NS