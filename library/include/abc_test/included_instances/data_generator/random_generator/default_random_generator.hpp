#pragma once

#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"
#include "abc_test/utility/bounds.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "concepts"

#include <memory>
#include <optional>

_BEGIN_ABC_DG_NS

/*!
 * @brief Abstract class for the default random generator. There has to be some
 * specialization of this class for it to actuall be used.
 * @tparam T
 */
template <typename T>
struct default_random_generator_t : public random_generator_base_t<T>
{
public:
    __constexpr virtual T
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        __STATIC_ASSERT(
            T,
            "Function requires a specialization of the function "
            "operator<T>(utility::rng, const utility::rng_counter_t&)."
        );
    }
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
/*!
 * @brief Factory function for creating a default_random_generator_t wrapped in
 * a random_generaotr_t object.
 * @tparam T The template type of the created random generator.
 * @tparam ...Ts The argument types for the default_random_generator_t.
 * @param ...elements The arguments for the default_random_generator_t.
 * @return The created random_generator_t object.
 */
template <typename T, typename... Ts>
__constexpr data_gen::random_generator_t<T>
            default_random_generator(Ts... elements) noexcept;

template <typename T, typename Ts>
__constexpr data_gen::random_generator_t<T>
            default_random_generatorx2(
                Ts elements
            ) noexcept
{
    using namespace std;
    using namespace data_gen;
    return make_shared<default_random_generator_t<T>>(elements);
}

template <typename T, typename... Ts>
__constexpr T
    run_default_random_generator(
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index,
        Ts... elements
    ) noexcept
{
    return default_random_generator<T>(elements...)
        ->operator()(_a_rnd_generator, _a_index);
}

_END_ABC_NS

_BEGIN_ABC_DG_NS

namespace detail
{
template <typename T>
requires std::unsigned_integral<T>
__constexpr T
    generate_rng_value_between_bounds(
        const utility::bounds_t<T>&   _a_bounds,
        const utility::rng_counter_t& _a_rng_counter,
        utility::rng_t&               _a_rng
    )
{
    using namespace std;
    if constexpr (std::same_as<T, std::uintmax_t>)
    {
        const size_t _l_denominator{
            std::min(_a_bounds.difference(), _a_rng_counter)
        };
        return
            // If denominator is max, then difference must be max. And lower==0.
            (_l_denominator == numeric_limits<std::uintmax_t>::max())
                ? _a_rng()
                :
                // If denominator is 0, no point in calling rng
                _l_denominator == 0
                ? _a_bounds.lower()
                // Otherwise, pick the value we want.
                : _a_bounds.lower()
                      + static_cast<T>(_a_rng() % (_l_denominator + 1));
    }
    else
    {
        const size_t _l_denominator{std::min(
            static_cast<size_t>(_a_bounds.difference()), _a_rng_counter
        )};
        return
            // If denominator is 0, no point in calling rng
            _l_denominator == 0
                ? _a_bounds.lower()
                // Else pick a value from lower to higher. We use modulus +1 so
                // that it is within that range. If the denominator ==
                // numeric_limits::max, don't add 1 as it will cause division by
                // 0.
                : _a_bounds.lower()
                      + static_cast<T>(_a_rng() % (_l_denominator + 1));
    }
}

template <typename T>
__constexpr std::size_t
            biggest_character_size()
{
    using namespace std;
    if constexpr (same_as<T, char>
                  || (same_as<T, wchar_t> && sizeof(wchar_t) == 4)
                  || same_as<T, char32_t>)
    {
        return 1;
    }
    else if constexpr (same_as<T, char16_t>
                       || (same_as<T, wchar_t> && sizeof(wchar_t) == 2))
    {
        return 2;
    }
    else if constexpr (same_as<T, char8_t>)
    {
        return 4;
    }
    else if constexpr (same_as<T, wchar_t>)
    {
        __STATIC_ASSERT(
            T,
            "biggest_character is not defined for the wchar_t type of this size"
        );
    }
    else
    {
        __STATIC_ASSERT(
            T, "biggest_character_size is not defined for this type"
        );
    }
}

__constexpr char32_t
    generate_valid_unicode_char32_t(
        utility::rng_t& _a_rng,
        const char32_t  _a_limit = _ABC_NS_UTILITY_STR::char32_limit<char32_t>()
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    constexpr char32_t _l_low_surrogate{high_surrogate_lower_value<char32_t>()};
    constexpr char32_t _l_high_surrogate{low_surrogate_upper_value<char32_t>()};
    char32_t           _l_rv;
    if (_a_limit < _l_low_surrogate)
    {
        // Don't have to worry about surrogates.
        _l_rv = _a_rng() % (_a_limit + 1);
    }
    else if (_a_limit < _l_high_surrogate)
    {
        // Should never happen.
        std::unreachable();
    }
    else
    {
        constexpr char32_t _l_surrogate_diff{
            _l_high_surrogate - _l_low_surrogate
        };
        const char32_t _l_limit{
            _a_limit - _l_surrogate_diff
            // Total range of char32_t = 1114111
            // Surrogate range between 55296 and 57343. Cannot be these numbers.
        };
        uint64_t _l_rnd_val{_a_rng()};
        char32_t _l_rnd{static_cast<char32_t>(_l_rnd_val) % (_l_limit + 1)};
        _l_rv = (_l_rnd >= _l_low_surrogate && _l_rnd <= _l_high_surrogate)
                    ? (_l_rnd + _l_surrogate_diff)
                    : _l_rnd;
    }
    return _l_rv;
}

template <typename T>
__constexpr std::optional<std::basic_string<T>>
            generate_valid_character(
                utility::rng_t&   _a_rng,
                const std::size_t _a_biggest_string
            ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    if (_a_biggest_string == 0)
    {
        // Should always be > 0. This should be optimised away.
        std::unreachable();
        return basic_string<T>{};
    }
    if constexpr (same_as<T, char>)
    {
        // Chars are in the ascii range, and are easy to create.
        return basic_string<T>(1, (_a_rng() % 128));
    }
    else if constexpr (same_as<T, char32_t>
                       || (same_as<T, wchar_t> && sizeof(wchar_t) == 4))
    {
        // Biggest string type, no variable lengths.
        return basic_string<T>(
            1, static_cast<T>(generate_valid_unicode_char32_t(_a_rng))
        );
    }
    else
    {
        auto _l_conversion_func = [](const char32_t _l_char)
        {
            return unicode_conversion_with_exception<T>(_l_char);
        };
        // Ths is the largest amount of space that a char32_t will take in this
        // encoding (assuming not plain ASCII chars).
        const size_t _l_biggest_char{biggest_character_size<T>()};
        if (_l_biggest_char <= _a_biggest_string)
        {
            return _l_conversion_func(generate_valid_unicode_char32_t(_a_rng));
        }
        else
        {
            // Else there is a limit on how high the char32_ts can be. This
            // value is encoded in limit by the following code.
            char32_t _l_limit;
            if constexpr (same_as<T, char16_t>
                          || (same_as<T, wchar_t> && sizeof(wchar_t) == 2))
            {
                switch (_a_biggest_string)
                {
                case 1:
                    _l_limit
                        = single_char16_limit_and_three_char8_limit<char32_t>();
                    break;
                default:
                    std::unreachable();
                }
            }
            else if constexpr (same_as<T, char8_t>)
            {
                switch (_a_biggest_string)
                {
                case 1:
                    _l_limit = ascii_limit<char32_t>();
                    break;
                case 2:
                    _l_limit = two_char8_limit<char32_t>();
                    break;
                case 3:
                    _l_limit
                        = single_char16_limit_and_three_char8_limit<char32_t>();
                    break;
                default:
                    std::unreachable();
                }
            }
            else
            {
                __STATIC_ASSERT(
                    T, "generate_valid_character not defined for this type"
                );
            }
            // Now we know the limit, we can create a string from it.
            return _l_conversion_func(
                generate_valid_unicode_char32_t(_a_rng, _l_limit)
            );
        }
    }
}
} // namespace detail

enum class enum_character_random_generator_mode_t
{
    ONLY_VALID_CHARS,
    ALL_VALID_INTEGERS
};

/*template <typename T>
struct character_random_generator_t : public random_generator_base_t<T>
{
public:
    __constexpr
    character_random_generator_t()
        = default;

    __constexpr
    character_random_generator_t(
        const utility::bounds_t<T>& _a_bounds
    )
        : _m_mode(enum_character_random_generator_mode_t::ALL_VALID_INTEGERS)
        , _m_bounds(_a_bounds)
    {}

    __no_constexpr virtual T
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using enum enum_character_random_generator_mode_t;
        using namespace std;
        constexpr char32_t _l_max_char32_t{0x10'FFFF};
        constexpr char32_t _l_low_surrogate{0xD800};
        constexpr char32_t _l_high_surrogate{0xDFFF};
        constexpr char32_t _l_surrogate_diff{
            _l_high_surrogate - _l_low_surrogate
        };
        const char32_t _l_limit{
            _l_max_char32_t - _l_surrogate_diff
            // Total range of char32_t = 1114111
            // Surrogate range between 55296 and 57343. Cannot be these numbers.
        };
        switch (_m_mode)
        {
        case ONLY_VALID_CHARS:
        {
            char32_t  _l_rnd{_a_rnd_generator() % (_l_limit + 1)};
            u32string _l_char{
                (_l_rnd >= _l_low_surrogate && _l_rnd <= _l_high_surrogate)
                    ? (_l_rnd + _l_surrogate_diff)
                    : _l_rnd
            };
            const T _l_str{ abc::convert_string<u32string, T>() };
            return _l_str.at(0);
            return abc::convert_u32string_to_u8string(_l_char).at(0);
        }
        case ALL_VALID_INTEGERS:
            return detail::generate_rng_value_between_bounds(
                _m_bounds, _a_index, _a_rnd_generator
            );
        default:
            throw errors::unaccounted_for_enum_exception(_m_mode);
        }
    }
private:
    enum_character_random_generator_mode_t _m_mode
        = enum_character_random_generator_mode_t::ONLY_VALID_CHARS;
    utility::bounds_t<T> _m_bounds;
};*/

template <>
struct default_random_generator_t<bool> : public random_generator_base_t<bool>
{
    __no_constexpr virtual bool
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );
};

/*template <>
struct default_random_generator_t<char8_t>
    : public character_random_generator_t<char8_t>
{
    using character_random_generator_t<char8_t>::character_random_generator_t;
};

template <>
struct default_random_generator_t<char16_t>
    : public random_generator_base_t<char16_t>
{
    __no_constexpr_imp virtual char16_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        return char16_t();
    }
};*/

template <typename T>
requires std::integral<T>
struct default_random_generator_t<T> : public random_generator_base_t<T>
{
public:
    __constexpr
    default_random_generator_t(
        const T& _a_arg1,
        const T& _a_arg2
    ) noexcept
        : _m_bounds(utility::bounds_t<T>(_a_arg1, _a_arg2))
    {}

    __constexpr
    default_random_generator_t(
        const utility::bounds_t<T> _a_bounds = utility::bounds_t<T>()
    ) noexcept
        : _m_bounds(_a_bounds)
    {}

    __constexpr_imp virtual T
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        return _m_bounds.lower()
               + ((_m_bounds.difference() == 0)
                      ? 0
                      : static_cast<T>(
                            _a_rnd_generator() % _m_bounds.difference()
                        ));
    }
private:
    utility::bounds_t<T> _m_bounds;
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
        : _m_min(_ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
                     .enums_idx(_a_min))
        , _m_max(_ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
                     .enums_idx(_a_max))
        , _m_range(_ABC_NS_UTILITY::get_thread_local_enumerate_enum_helper<T>()
                       .difference(_a_min, _a_max))
    {}

    __no_constexpr_imp virtual T
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        const size_t _l_val{
            _m_min
            + (_m_range == 0
                   ? 0
                   : (static_cast<size_t>(_a_rnd_generator()) % _m_range))
        };
        return get_thread_local_enumerate_enum_helper<T>().idxs_enum(_l_val);
    }
private:
    std::size_t _m_min;
    std::size_t _m_max;
    std::size_t _m_range;
};

template <typename T, typename... Ts>
__constexpr T
    make_random_object(
        utility::rng_t&                       _a_rng,
        const utility::rng_counter_t&         _a_size,
        std::tuple<random_generator_t<Ts>...> _a_generators
    ) noexcept;

namespace
{
template <std::size_t I, typename... Ts>
__constexpr void
    run_internal_make_object(
        utility::rng_t&                       _a_rng,
        const utility::rng_counter_t&         _a_size,
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
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );
private:
    random_generator_t<T> _m_gen;
};

/*template <typename T>
requires (std::convertible_to<T, std::string_view>)
struct default_random_generator_t<T> : public random_generator_base_t<T>
{
    __no_constexpr virtual T
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        T _l_rv;
        for (size_t _l_idx{0}; _l_idx < 0; ++_l_idx)
        {
            _l_rv.push_back(default_random_generator_t<char>{}.operator()(
                _a_rnd_generator, _a_index
            ));
        }
        return _l_rv;
    }
};*/

template <typename... Ts>
struct default_random_generator_t<std::tuple<Ts...>>
    : public random_generator_base_t<std::tuple<Ts...>>
{
    using value_type_t = std::tuple<Ts...>;

    __constexpr
    default_random_generator_t(random_generator_t<Ts>... _a_rngs);
    __constexpr
    default_random_generator_t();
    template <std::size_t I>
    __constexpr void
        run_rng_internal(
            value_type_t&                 _a_object,
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        ) const;
    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );

    __constexpr std::tuple<random_generator_t<Ts>...>&
                get_ref_to_inner_rngs() noexcept
    {
        return _m_rngs;
    }
private:
    std::tuple<random_generator_t<Ts>...> _m_rngs;
};

template <typename T1, typename T2>
struct default_random_generator_t<std::pair<T1, T2>>
    : public random_generator_base_t<std::pair<T1, T2>>
{
    using value_type_t = std::pair<T1, T2>;

    __constexpr
    default_random_generator_t(
        const random_generator_t<T1>& _a_arg1,
        const random_generator_t<T2>& _a_arg2
    )
        : _m_rngs(std::make_pair(_a_arg1, _a_arg2))
    {}

    __constexpr
    default_random_generator_t()
        : _m_rngs(std::make_pair(
              default_random_generator<T1>(),
              default_random_generator<T2>()
          ))
    {}

    template <std::size_t I>
    __constexpr void
        run_rng_internal(
            value_type_t&                 _a_object,
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        ) const
    {
        using namespace std;
        get<I>(_a_object)
            = get<I>(_m_rngs)->operator()(_a_rnd_generator, _a_index);
        if constexpr (I + 1 < tuple_size<value_type_t>{})
        {
            run_rng_internal<I + 1>(_a_object, _a_rnd_generator, _a_index);
        }
    }

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        value_type_t _l_rv;
        run_rng_internal<0>(_l_rv, _a_rnd_generator, _a_index);
        return _l_rv;
    }

    __constexpr std::pair<random_generator_t<T1>, random_generator_t<T2>>&
                get_ref_to_inner_rngs() noexcept
    {
        return _m_rngs;
    }
private:
    std::pair<random_generator_t<T1>, random_generator_t<T2>> _m_rngs;
};

template <typename... Ts>
struct default_random_generator_t<std::variant<Ts...>>
    : public random_generator_base_t<std::variant<Ts...>>
{
    using value_type_t = std::variant<Ts...>;

    __constexpr
    default_random_generator_t(random_generator_t<Ts>... _a_rngs);
    __constexpr
    default_random_generator_t();
    template <std::size_t I>
    __constexpr void
        run_rng_internal(
            value_type_t&                 _a_object,
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index,
            const std::size_t             _a_generate_index
        ) const;
    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );

    __constexpr std::tuple<random_generator_t<Ts>...>&
                get_ref_to_inner_rngs() noexcept
    {
        return _m_rngs;
    }
private:
    std::tuple<random_generator_t<Ts>...> _m_rngs;
};

template <>
struct default_random_generator_t<std::monostate>
    : public random_generator_base_t<std::monostate>
{
    using value_type_t = std::monostate;

    __constexpr
    default_random_generator_t()
    {}

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        return std::monostate{};
    }
};

template <>
struct default_random_generator_t<std::mt19937_64>
    : public random_generator_base_t<std::mt19937_64>
{
    using value_type_t = std::mt19937_64;

    __constexpr
    default_random_generator_t()
    {}

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        auto             _l_rnd{default_random_generator<vector<uint32_t>>()};
        vector<uint32_t> _l_seeds
            = _l_rnd->operator()(_a_rnd_generator, _a_index);
        seed_seq _l_seed(_l_seeds.begin(), _l_seeds.end());
        return std::mt19937_64(_l_seed);
    }
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
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
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
        : _m_rng(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(
        const random_generator_t<T>& _a_rnd_gen
    )
        : _m_rng(_a_rnd_gen)
    {}

    __no_constexpr virtual std::optional<T>
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );
private:
    random_generator_t<T> _m_rng;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename T>
__no_constexpr_imp std::optional<T>
                   default_random_generator_t<std::optional<T>>::operator()(
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
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
        utility::rng_t& _a_rnd_generator
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
struct default_random_generator_t<std::basic_string<T>>
    : public random_generator_base_t<std::basic_string<T>>
{
    using value_type_t = std::basic_string<T>;

    __constexpr
    default_random_generator_t(
        const utility::bounds_t<typename std::basic_string<T>::size_type>&
            _a_bounds
        = utility::bounds_t<typename std::basic_string<T>::size_type>(
            0,
            std::basic_string<T>{}.max_size()
        )
    )
        : _m_bounds(_a_bounds)
    {
        using namespace _ABC_NS_UTILITY_STR;
        if constexpr (is_char_type_c<T>)
        {
            _m_rng = std::optional<
                random_generator_t<typename std::basic_string<T>::value_type>>{
            };
        }
        else
        {
            _m_rng = default_random_generator<T>();
        }
    }

    __constexpr
    default_random_generator_t(
        const random_generator_t<typename std::basic_string<T>::value_type>&
            _a_rng,
        const utility::bounds_t<typename std::basic_string<T>::size_type>&
            _a_bounds
        = utility::bounds_t<typename std::basic_string<T>::size_type>(
            0,
            std::basic_string<T>{}.max_size()
        )
    ) noexcept
        : _m_rng(_a_rng), _m_bounds(_a_bounds)
    {}

    __no_constexpr_imp virtual std::basic_string<T>
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY_STR;
        const typename basic_string<T>::size_type _l_size{
            detail::generate_rng_value_between_bounds<
                typename basic_string<T>::size_type>(
                _m_bounds, _a_index, _a_rnd_generator
            )
        };
        basic_string<T> _l_rv(_l_size, T{});
        if constexpr (is_char_type_c<T>)
        {
            if (_m_rng.has_value())
            {
                generate_string(
                    _l_rv, _m_rng.value(), _a_rnd_generator, _a_index
                );
            }
            else
            {
                size_t _l_remaining{_l_size};
                size_t _l_idx{0};
                while (_l_remaining > 0)
                {
                    const optional<basic_string<T>> _l_opt_generated{
                        detail::generate_valid_character<T>(
                            _a_rnd_generator, _l_remaining
                        )
                    };
                    if (_l_opt_generated.has_value())
                    {
                        const basic_string<T>& _l_generated{
                            _l_opt_generated.value()
                        };
                        for (const T _l_char : _l_generated)
                        {
                            _l_rv[_l_idx++] = _l_char;
                            _l_remaining--;
                        }
                        // auto kbd = fmt::format(
                        //     u8"[{0} - {1}] = {2}", ki, _l_idx, _l_generated
                        // );
                        // std::cdout << std::string(kbd.begin(), kbd.end()) <<
                        // std::endl;
                    }
                    else
                    {
                        break;
                    }
                }
                if (_l_remaining > 0)
                {
                    _l_rv.resize(_l_idx);
                }
            }
        }
        else
        {
            generate_string(_l_rv, _m_rng, _a_rnd_generator, _a_index);
        }
        return _l_rv;
    }
private:
    std::conditional_t<
        _ABC_NS_UTILITY_STR::is_char_type_c<T>,
        std::optional<
            random_generator_t<typename std::basic_string<T>::value_type>>,
        random_generator_t<typename std::basic_string<T>::value_type>>
        _m_rng;

    __constexpr_imp void
        generate_string(
            std::basic_string<T>& _a_rv,
            random_generator_t<typename std::basic_string<T>::value_type>&
                                          _a_rng,
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        typename basic_string<T>::iterator _l_end{end(_a_rv)};
        typename basic_string<T>::iterator _l_itt{begin(_a_rv)};
        for (typename basic_string<T>::iterator _l_itt{begin(_a_rv)};
             _l_itt != _l_end;
             ++_l_itt)
        {
            *_l_itt = _a_rng->operator()(_a_rnd_generator, _a_index);
        }
    }

    utility::bounds_t<typename std::basic_string<T>::size_type> _m_bounds;
};

/*template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
struct default_random_generator_t<std::basic_string<T>>
    : public random_generator_base_t<std::basic_string<T>>
{
public:
    __constexpr
    default_random_generator_t(
        const utility::bounds_t<typename std::basic_string<T>::size_type>&
            _a_size_bounds
        = utility::bounds_t<typename std::basic_string<T>::size_type>(
            0,
            std::basic_string<T>{}.max_size()
        )
    )
        : _m_mode(enum_character_random_generator_mode_t::ONLY_VALID_CHARS)
        , _m_rng(std::variant(
              std::in_place_index<0>,
              default_random_generator<all_valid_chars_t>()
          ))
    {*/
/*
        const utility::bounds_t<typename
std::basic_string<T>::size_type>& _a_size_bounds =
utility::bounds_t<typename std::basic_string<T>::size_type>( 0,
    std::basic_string<T>{}.max_size()
)
: _m_mode(enum_character_random_generator_mode_t::ONLY_VALID_CHARS)
, _m_bounds(_a_size_bounds)
, _m_rng(default_random_generator_t<char32_t>{})
*/
/*}

__constexpr
default_random_generator_t(
    const random_generator_t<T>& _a_rnd_gen,
    const utility::bounds_t<typename std::basic_string<T>::size_type>&
        _a_size_bounds
    = utility::bounds_t<typename std::basic_string<T>::size_type>(
        0,
        std::basic_string<T>{}.max_size()
    )
) noexcept;
__no_constexpr virtual std::basic_string<T>
    operator()(
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    );
private:
using all_valid_chars_t
    = std::conditional_t<std::same_as<T, char>, char, char32_t>;
enum_character_random_generator_mode_t _m_mode;
std::variant<random_generator_t<all_valid_chars_t>, random_generator_t<T>>
                                                            _m_rng;
utility::bounds_t<typename std::basic_string<T>::size_type> _m_bounds;
};*/

template <typename T>
struct default_random_generator_t<std::vector<T>>
    : public random_generator_base_t<std::vector<T>>
{
public:
    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;
    __no_constexpr virtual std::vector<T>
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
struct default_random_generator_t<std::forward_list<T>>
    : public random_generator_base_t<std::forward_list<T>>
{
public:
    using value_type_t = std::forward_list<T>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        typename value_type_t::iterator _l_end{end(_l_rv)};
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_rv.push_front(_m_gen->operator()(_a_rnd_generator, _a_index));
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
struct default_random_generator_t<std::deque<T>>
    : public random_generator_base_t<std::deque<T>>
{
public:
    using value_type_t = std::deque<T>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv(_l_size);
        typename value_type_t::iterator       _l_itt{begin(_l_rv)};
        const typename value_type_t::iterator _l_end{end(_l_rv)};
        for (typename value_type_t::iterator _l_itt{begin(_l_rv)};
             _l_itt != _l_end;
             ++_l_itt)
        {
            *_l_itt = _m_gen->operator()(_a_rnd_generator, _a_index);
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
struct default_random_generator_t<std::list<T>>
    : public random_generator_base_t<std::list<T>>
{
public:
    using value_type_t = std::list<T>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        typename value_type_t::iterator _l_end{end(_l_rv)};
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_end = _l_rv.insert(
                _l_end, _m_gen->operator()(_a_rnd_generator, _a_index)
            );
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
struct default_random_generator_t<std::unordered_multiset<T>>
    : public random_generator_base_t<std::unordered_multiset<T>>
{
public:
    using value_type_t = std::unordered_multiset<T>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        typename value_type_t::iterator _l_end{end(_l_rv)};
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_end = _l_rv.insert(
                _l_end, _m_gen->operator()(_a_rnd_generator, _a_index)
            );
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
struct default_random_generator_t<std::multiset<T>>
    : public random_generator_base_t<std::multiset<T>>
{
public:
    using value_type_t = std::multiset<T>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        typename value_type_t::iterator _l_end{end(_l_rv)};
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_end = _l_rv.insert(
                _l_end, _m_gen->operator()(_a_rnd_generator, _a_index)
            );
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
struct default_random_generator_t<std::set<T>>
    : public random_generator_base_t<std::set<T>>
{
public:
    using value_type_t = std::set<T>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        typename value_type_t::iterator _l_end{end(_l_rv)};
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_end = _l_rv.insert(
                _l_end, _m_gen->operator()(_a_rnd_generator, _a_index)
            );
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T>
struct default_random_generator_t<std::unordered_set<T>>
    : public random_generator_base_t<std::unordered_set<T>>
{
public:
    using value_type_t = std::unordered_set<T>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen(default_random_generator<T>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        typename value_type_t::iterator _l_end{end(_l_rv)};
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_end = _l_rv.insert(
                _l_end, _m_gen->operator()(_a_rnd_generator, _a_index)
            );
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T>&
                get_rng_ref() noexcept
    {
        return _m_gen;
    }
private:
    random_generator_t<T> _m_gen;
};

template <typename T1, typename T2>
struct default_random_generator_t<std::map<T1, T2>>
    : public random_generator_base_t<std::map<T1, T2>>
{
public:
    using value_type_t = std::map<T1, T2>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen1(default_random_generator<T1>())
        , _m_gen2(default_random_generator<T2>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T1>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_rv.insert(make_pair(
                _m_gen1->operator()(_a_rnd_generator, _a_index),
                _m_gen2->operator()(_a_rnd_generator, _a_index)
            ));
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T1>&
                get_rng_ref() noexcept
    {
        return _m_gen1;
    }
private:
    random_generator_t<T1> _m_gen1;
    random_generator_t<T2> _m_gen2;
};

template <typename T1, typename T2>
struct default_random_generator_t<std::unordered_map<T1, T2>>
    : public random_generator_base_t<std::unordered_map<T1, T2>>
{
public:
    using value_type_t = std::unordered_map<T1, T2>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen1(default_random_generator<T1>())
        , _m_gen2(default_random_generator<T2>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T1>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_rv.insert(make_pair(
                _m_gen1->operator()(_a_rnd_generator, _a_index),
                _m_gen2->operator()(_a_rnd_generator, _a_index)
            ));
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T1>&
                get_rng_ref() noexcept
    {
        return _m_gen1;
    }
private:
    random_generator_t<T1> _m_gen1;
    random_generator_t<T2> _m_gen2;
};

template <typename T1, typename T2>
struct default_random_generator_t<std::multimap<T1, T2>>
    : public random_generator_base_t<std::multimap<T1, T2>>
{
public:
    using value_type_t = std::multimap<T1, T2>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen1(default_random_generator<T1>())
        , _m_gen2(default_random_generator<T2>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T1>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_rv.insert(make_pair(
                _m_gen1->operator()(_a_rnd_generator, _a_index),
                _m_gen2->operator()(_a_rnd_generator, _a_index)
            ));
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T1>&
                get_rng_ref() noexcept
    {
        return _m_gen1;
    }
private:
    random_generator_t<T1> _m_gen1;
    random_generator_t<T2> _m_gen2;
};

template <typename T1, typename T2>
struct default_random_generator_t<std::unordered_multimap<T1, T2>>
    : public random_generator_base_t<std::unordered_multimap<T1, T2>>
{
public:
    using value_type_t = std::unordered_multimap<T1, T2>;

    __constexpr
    default_random_generator_t() noexcept
        : _m_gen1(default_random_generator<T1>())
        , _m_gen2(default_random_generator<T2>())
    {}

    __constexpr
    default_random_generator_t(const random_generator_t<T1>& _a_gen) noexcept;

    __no_constexpr virtual value_type_t
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        using namespace _ABC_NS_UTILITY;
        using namespace utility;
        using size_type = value_type_t::size_type;
        const size_type _l_size{detail::generate_rng_value_between_bounds(
            bounds_t<size_type>(), _a_index, _a_rnd_generator
        )};
        value_type_t    _l_rv;
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_rv.insert(make_pair(
                _m_gen1->operator()(_a_rnd_generator, _a_index),
                _m_gen2->operator()(_a_rnd_generator, _a_index)
            ));
        }
        return _l_rv;
    }

    __constexpr random_generator_t<T1>&
                get_rng_ref() noexcept
    {
        return _m_gen1;
    }
private:
    random_generator_t<T1> _m_gen1;
    random_generator_t<T2> _m_gen2;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

/*template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
__constexpr_imp
    default_random_generator_t<std::basic_string<T>>::
        default_random_generator_t(
            const random_generator_t<T>& _a_rnd_gen,
            const utility::bounds_t<typename std::basic_string<T>::size_type>&
                _a_size_bounds
        ) noexcept
    : _m_rng(_a_rnd_gen), _m_bounds(_a_size_bounds)
{}

template <typename T>
requires _ABC_NS_UTILITY::min_value_c<T> && _ABC_NS_UTILITY::max_value_c<T>
__no_constexpr_imp std::basic_string<T>
                   default_random_generator_t<std::basic_string<T>>::operator()(
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    )
{
    using namespace std;
    using enum enum_character_random_generator_mode_t;
    const basic_string<T>::size_type _l_size{
        detail::generate_rng_value_between_bounds<basic_string<T>::size_type>(
            _m_bounds, _a_index, _a_rnd_generator
        )
    };
    basic_string<T> _l_rv(_l_size, T{});
    switch (_m_mode)
    {
    case ONLY_VALID_CHARS:
        for (size_t _l_idx{0}; _l_idx < _l_size;)
        {
            if constexpr (same_as<basic_string<T>, u8string>)
            {
                char32_t _l_char{
                    get<0>(_m_rng)->operator()(_a_rnd_generator, _a_index)
                };
                basic_string<T> _l_auxillery_str{
                    abc::convert_u32string_to_u8string(u32string(1, _l_char))
                };
                if (_l_idx + _l_auxillery_str.size() < _l_size)
                {
                    for (const T& _l_char : _l_auxillery_str)
                    {
                        _l_rv[_l_idx++] = _l_char;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        break;
    case ALL_VALID_INTEGERS:
        for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
        {
            _l_rv[_l_idx]
                = get<1>(_m_rng)->operator()(_a_rnd_generator, _a_index);
        }
        break;
    default:
        throw errors::unaccounted_for_enum_exception(_m_mode);
    }
    return _l_rv;
}*/

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
        utility::rng_t& _a_rnd_generator
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
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    )
{
    using namespace std;
    using namespace _ABC_NS_UTILITY;
    using namespace utility;
    using value_type_t = std::vector<T>;
    using size_type    = value_type_t::size_type;
    const size_type _l_size{detail::generate_rng_value_between_bounds(
        bounds_t<size_type>(), _a_index, _a_rnd_generator
    )};
    value_type_t    _l_rv(_l_size);
    const typename value_type_t::iterator _l_end{end(_l_rv)};
    for (typename value_type_t::iterator _l_itt{begin(_l_rv)}; _l_itt != _l_end;
         ++_l_itt)
    {
        *_l_itt = _m_gen->operator()(_a_rnd_generator, _a_index);
    }
    return _l_rv;
}

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

/*template <typename T>
__constexpr_imp T
    default_random_generator_t<T>::operator()(
        utility::rng_t&     _a_rnd_generator,
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
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
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
        utility::rng_t& _a_rnd_generator
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
    utility::rng_t& _a_rnd_generator,
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
        utility::rng_t&                       _a_rng,
        const utility::rng_counter_t&         _a_size,
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
        utility::rng_t&                       _a_rng,
        const utility::rng_counter_t&         _a_size,
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
    : _m_rngs(_a_rngs...)
{}

template <typename... Ts>
__constexpr_imp
    default_random_generator_t<std::tuple<Ts...>>::default_random_generator_t()
    : _m_rngs(std::make_tuple(default_random_generator<Ts>()...))
{}

template <typename... Ts>
template <std::size_t I>
__constexpr void
    default_random_generator_t<std::tuple<Ts...>>::run_rng_internal(
        value_type_t&                 _a_object,
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    ) const
{
    using namespace std;
    get<I>(_a_object) = get<I>(_m_rngs)->operator()(_a_rnd_generator, _a_index);
    if constexpr (I + 1 < tuple_size<value_type_t>{})
    {
        run_rng_internal<I + 1>(_a_object, _a_rnd_generator, _a_index);
    }
}

template <typename... Ts>
__no_constexpr_imp default_random_generator_t<std::tuple<Ts...>>::value_type_t
                   default_random_generator_t<std::tuple<Ts...>>::operator()(
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    )
{
    value_type_t _l_rv;
    run_rng_internal<0>(_l_rv, _a_rnd_generator, _a_index);
    return _l_rv;
}

template <typename... Ts>
__constexpr_imp
    default_random_generator_t<std::variant<Ts...>>::default_random_generator_t(
        random_generator_t<Ts>... _a_rngs
    )
    : _m_rngs(_a_rngs...)
{}

template <typename... Ts>
__constexpr_imp
    default_random_generator_t<std::variant<Ts...>>::default_random_generator_t(
    )
    : _m_rngs(std::make_tuple(
          mk_random_generator(default_random_generator_t<Ts>())...
      ))
{}

template <typename... Ts>
template <std::size_t I>
__constexpr void
    default_random_generator_t<std::variant<Ts...>>::run_rng_internal(
        value_type_t&                 _a_object,
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index,
        const std::size_t             _a_generate_index
    ) const
{
    using namespace std;
    if (I == _a_generate_index)
    {
        auto ki   = get<I>(_m_rngs)->operator()(_a_rnd_generator, _a_index);
        _a_object = value_type_t(in_place_index<I>, ki);
    }
    else if constexpr (I + 1 < tuple_size<std::tuple<Ts...>>{})
    {
        run_rng_internal<I + 1>(
            _a_object, _a_rnd_generator, _a_index, _a_generate_index
        );
    }
    else
    {
    }
}

template <typename... Ts>
__no_constexpr_imp default_random_generator_t<std::variant<Ts...>>::value_type_t
                   default_random_generator_t<std::variant<Ts...>>::operator()(
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    )
{
    value_type_t _l_rv;
    size_t       _l_index{detail::generate_rng_value_between_bounds(
        utility::bounds_t<size_t>(0, std::tuple_size<std::tuple<Ts...>>{} - 1),
        _a_index,
        _a_rnd_generator
    )};
    run_rng_internal<0>(_l_rv, _a_rnd_generator, _a_index, _l_index);
    return _l_rv;
}

_END_ABC_DG_NS