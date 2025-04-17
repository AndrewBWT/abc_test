#pragma once

#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"
#include "abc_test/utility/bounds.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"
#include "abc_test/utility/str/conversion.hpp"
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
template <typename T, typename... Ts>
__constexpr data_gen::random_generator_t<T>
            default_random_generator(Ts... elements) noexcept;
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
    else if constexpr (same_as<wchar_t>)
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
        const char32_t  _a_limit = 0x1'0FFF
    )
{
    using namespace std;
    constexpr char32_t _l_low_surrogate{0xD800};
    constexpr char32_t _l_high_surrogate{0xDFFF};
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
        char32_t _l_rnd{_a_rng() % (_l_limit + 1)};
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
    if (_a_biggest_string == 0)
    {
        // Should always be 1. This should be optimised away.
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
        // Ths is the largest amount of space that a char32_t will take in this
        // encoding (assuming not plain ASCII chars).
        const size_t _l_biggest_char{biggest_character_size<T>()};
        if (_l_biggest_char <= _a_biggest_string)
        {
            return abc::detail::convert_char_to_string<T>(
                generate_valid_unicode_char32_t(_a_rng)
            );
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
                    _l_limit = 0xFFFF;
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
                    _l_limit = 0x7F;
                    break;
                case 2:
                    _l_limit = 0x7FF;
                    break;
                case 3:
                    _l_limit = 0xFFFF;
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
            return abc::detail::convert_char_to_string<T>(
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
        const T& _a_min = std::numeric_limits<T>::min(),
        const T& _a_max = std::numeric_limits<T>::max()
    ) noexcept
        : _m_min(_a_min), _m_max(_a_max), _m_range(_m_max - _m_min)
    {}

    __constexpr_imp virtual T
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
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
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
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
            value_type&                   _a_object,
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        ) const;
    __no_constexpr virtual value_type
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
        : _m_rng(mk_random_generator(default_random_generator_t<T>()))
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
    __constexpr
    default_random_generator_t(
        const utility::bounds_t<typename std::basic_string<T>::size_type>&
            _a_bounds
        = utility::bounds_t<typename std::basic_string<T>::size_type>(
            0,
            std::basic_string<T>{}.max_size()
        )
    )
        : _m_rng(std::nullopt), _m_bounds(_a_bounds)
    {}

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
        std::cout << "Begin RNG basic string. rng_counter = " <<
            _a_index << std::endl;
        using namespace std;
        const basic_string<T>::size_type _l_size{
            detail::generate_rng_value_between_bounds<
                basic_string<T>::size_type>(
                _m_bounds, _a_index, _a_rnd_generator
            )
        };
        std::cout << fmt::format("Size {1}",_a_index, _l_size) << std::endl;
        if (_a_index == 0)
        {
            int i = 5;
        }
        basic_string<T> _l_rv(_l_size, T{});
        if (_m_rng.has_value())
        {
            random_generator_t<typename std::basic_string<T>::value_type>&
                _l_rng{_m_rng.value()};
            for (size_t _l_idx{0}; _l_idx < _l_size; ++_l_idx)
            {
                _l_rv[_l_idx] = _l_rng->operator()(_a_rnd_generator, _a_index);
               // auto kbd = fmt::format(
               //     u8"[{0}] = {1}", _l_idx, basic_string<T>(1, _l_rv[_l_idx])
              //  );
              //  std::cout << string(kbd.begin(), kbd.end()) << std::endl;
            }
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
                    const basic_string<T>& _l_generated{_l_opt_generated.value()
                    };
                    auto ki = _l_idx;
                    for (const T _l_char : _l_generated)
                    {
                        _l_rv[_l_idx++] = _l_char;
                        _l_remaining--;
                    }
                //    auto kbd = fmt::format(
                //        u8"[{0} - {1}] = {2}", ki, _l_idx, _l_generated
                //    );
                //    std::cout << std::string(kbd.begin(), kbd.end()) << std::endl;
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
        std::cout << "End RNG basic string. rng_counter = " <<
            _a_index << std::endl;
        return _l_rv;
    }
private:
    std::optional<random_generator_t<typename std::basic_string<T>::value_type>>
                                                                _m_rng;
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
    default_random_generator_t(const random_generator_t<T>& _a_gen) noexcept;
    __no_constexpr virtual std::vector<T>
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );
private:
    random_generator_t<T> _m_gen;
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
requires (
    std::is_default_constructible_v<default_random_generator_t<Ts>> && ...
)
    : _m_rngs(std::make_tuple(
          mk_random_generator(default_random_generator_t<Ts>())...
      ))
{}

template <typename... Ts>
template <std::size_t I>
__constexpr void
    default_random_generator_t<std::tuple<Ts...>>::run_rng_internal(
        value_type&                   _a_object,
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
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
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    )
{
    value_type _l_rv;
    run_rng_internal<0>(_l_rv, _a_rnd_generator, _a_index);
    return _l_rv;
}

_END_ABC_DG_NS