#pragma once
#include "abc_test/external/bigint/BigIntegerUtils.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_base.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_schema/from_m_to_n_inner.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_schema_base.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"

#include <numeric>
_BEGIN_ABC_DG_NS

template <typename T>
struct default_enumeration_t : public enumeration_base_t<T>
{
public:
    using value_type_t = T;
    __constexpr_imp virtual bool
        less_than(const T& _a_l, const T& _a_r) const noexcept;
    __constexpr_imp virtual bool
        equal(const T& _a_l, const T& _a_r) const noexcept;
    __constexpr virtual bool
        increment(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        );
    __constexpr virtual bool
        decrement(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        );
    __constexpr virtual enumeration_diff_t
        difference(const T& _a_arg1, const T& _a_arg2) noexcept;
};

_END_ABC_DG_NS
_BEGIN_ABC_NS
template <typename T, typename... Ts>
__constexpr data_gen::enumeration_t<T>
            default_enumeration(Ts... _a_elements) noexcept;
_END_ABC_NS

_BEGIN_ABC_NS
template <typename T, typename... Ts>
__constexpr_imp data_gen::enumeration_t<T>
                default_enumeration(
                    Ts... _a_elements
                ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_DG;
    return make_shared<default_enumeration_t<T>>(_a_elements...);
}

_END_ABC_NS
_BEGIN_ABC_DG_NS

template <typename T>
__constexpr_imp bool
    default_enumeration_t<T>::less_than(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "less_than<T>()."
    );
    return false;
}

template <typename T>
__constexpr_imp bool
    default_enumeration_t<T>::equal(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "equal<T>()."
    );
    return false;
}

template <typename T>
__constexpr bool
    default_enumeration_t<T>::increment(
        T&                      _a_element,
        enumerate_index_t&      _a_n_times_to_increment,
        const std::optional<T>& _a_max_value
    )
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "increment<T>()."
    );
    return false;
}

template <typename T>
__constexpr bool
    default_enumeration_t<T>::decrement(
        T&                      _a_element,
        enumerate_index_t&      _a_n_times_to_increment,
        const std::optional<T>& _a_max_value
    )
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "decrement<T>()."
    );
    return false;
}

template <typename T>
__constexpr enumeration_diff_t
    default_enumeration_t<T>::difference(
        const T& _a_arg1,
        const T& _a_arg2
    ) noexcept
{
    __STATIC_ASSERT(
        T,
        "Function requires a specialization of the function "
        "difference<T>()."
    );
    return enumeration_diff_t{};
}

template <typename T>
concept has_addition_c = requires (const T& _a_element) {
    { _a_element + _a_element };
};
template <typename T>
concept has_subtraction_c = requires (const T& _a_element) {
    { _a_element - _a_element };
};
template <typename T>
concept has_less_than_c = requires (const T& _a_element) {
    { _a_element < _a_element } -> std::same_as<bool>;
};
template <typename T>
concept has_equal_c = requires (const T& _a_element) {
    { _a_element == _a_element } -> std::same_as<bool>;
};

template <typename T>
    requires has_addition_c<T>&& has_subtraction_c<T>&& has_less_than_c<T>
&& has_equal_c<T> && (not std::is_enum_v<T>)
&& (not std::floating_point<T>)
struct default_enumeration_t<T> : public enumeration_base_t<T>
{
    __constexpr virtual enumeration_diff_t
        difference(
            const T& _a_arg1,
            const T& _a_arg2
        ) noexcept
    {
        const T _l_straight_difference{
            less_than(_a_arg1, _a_arg2) ? static_cast<T>(_a_arg2 - _a_arg1)
                                        : static_cast<T>(_a_arg1 - _a_arg2)
        };
        const T _l_divisor{
            static_cast<T>(_l_straight_difference / _m_difference)
        };
        T _l_remainder = static_cast<T>(_l_straight_difference % _m_difference);
        return {
            static_cast<std::size_t>(_l_divisor),
            static_cast<std::size_t>(_l_remainder)
        };
    }

    __constexpr
    default_enumeration_t(
        const T _a_difference = T(1)
    ) noexcept
        : _m_difference(_a_difference)
    {}

    __constexpr_imp virtual bool
        less_than(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _a_l < _a_r;
    }

    __constexpr_imp virtual bool
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _a_l == _a_r;
    }

    __constexpr bool
        increment(
            T&                      _a_element,
            enumerate_index_t&      _a_times_called,
            const std::optional<T>& _a_max_value = std::optional<T>{}
        )
    {
        using namespace std;
        const T _l_current_idx{_a_element};
        if (_a_max_value.has_value())
        {
            const T _l_max_times_called{static_cast<T>(
                (_a_max_value.value() - _l_current_idx) / _m_difference
            )};
            if (_a_times_called > _l_max_times_called)
            {
                if (_l_max_times_called == 0)
                {
                    return false;
                }
                else
                {
                    _a_element
                        = _a_element + _m_difference * _l_max_times_called;
                    _a_times_called -= _l_max_times_called;
                    return true;
                }
            }
            else
            {
                _a_element      = _a_element + _m_difference * _a_times_called;
                _a_times_called = 0;
                return true;
            }
        }
        else
        {
            _a_element      = _a_element + _m_difference * _a_times_called;
            _a_times_called = 0;
            return true;
        }
    }

    __constexpr bool
        decrement(
            T&                      _a_element,
            enumerate_index_t&      _a_times_called,
            const std::optional<T>& _a_max_value = std::optional<T>{}
        )
    {
        using namespace std;
        const T _l_current_idx{_a_element};
        if (_a_max_value.has_value())
        {
            const T _l_max_times_called{static_cast<T>(
                (_l_current_idx - _a_max_value.value()) / _m_difference
            )};
            if (_a_times_called > _l_max_times_called)
            {
                if (_l_max_times_called == 0)
                {
                    return false;
                }
                else
                {
                    _a_element
                        = _a_element - _m_difference * _l_max_times_called;
                    _a_times_called += _l_max_times_called;
                    return true;
                }
            }
            else
            {
                _a_element      = _a_element - _m_difference * _a_times_called;
                _a_times_called = 0;
                return true;
            }
        }
        else
        {
            _a_element      = _a_element - _m_difference * _a_times_called;
            _a_times_called = 0;
            return true;
        }
    }
private:
    T _m_difference;
};

namespace
{
template <typename T>
requires std::floating_point<T>
__constexpr T
    find_point_at_which_increment_no_longer_works(
        const T& _a_add_val
    )
{
    using namespace std;
    T _l_lower{0};
    T _l_higher{numeric_limits<T>::max()};
    while (true)
    {
        if (_l_lower == _l_higher)
        {
            // There is no point where adding _a_add_val will result in
            // the same result. So we just return the max value.
            return numeric_limits<T>::max();
        }
        const T _l_midpoint{midpoint(_l_lower, _l_higher)};
        if (_l_midpoint + _a_add_val == _l_midpoint
            && _l_midpoint - _a_add_val != _l_midpoint)
        {
            return _l_midpoint;
        }
        else if (_l_midpoint + _a_add_val != _l_midpoint)
        {
            _l_lower = _l_midpoint;
        }
        else
        {
            _l_higher = _l_midpoint;
        }
    }
}

template <typename T>
requires std::floating_point<T>
__constexpr std::size_t
            encode_into_size_t(
                T _a_input
            )
{
    using namespace std;
    size_t _l_input_as_int{0};
    std::memcpy(&_l_input_as_int, &_a_input, sizeof(T));
    bitset<CHAR_BIT * sizeof(T)>      _l_bitset{_l_input_as_int};
    bitset<sizeof(size_t) * CHAR_BIT> _l_sizet_bitset{};
    if (_l_bitset[sizeof(T) * CHAR_BIT - 1] == true)
    {
        _l_sizet_bitset[sizeof(size_t) * CHAR_BIT - 1] = true;
    }
    for (size_t _l_idx{sizeof(T) * CHAR_BIT - 1}; _l_idx > 0; --_l_idx)
    {
        _l_sizet_bitset[sizeof(size_t) * CHAR_BIT - _l_idx - 1]
            = _l_bitset[sizeof(T) * CHAR_BIT - _l_idx - 1];
    }
    return static_cast<size_t>(_l_sizet_bitset.to_ullong());
}
} // namespace

template <typename T>
requires (std::floating_point<T>)
struct default_enumeration_t<T> : public enumeration_base_t<T>
{
    static const std::size_t _c_highest_bit
        = 0b1000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000;
    static const std::size_t _c_non_highest_bit
        = 0b0111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111;
    std::optional<T> _m_point_at_which_increment_will_not_work;

    template <typename T1>
    static T1
        encode_into_t(
            const std::size_t _a_element
        ) noexcept
    {
        const bool _l_sign_bit{(_c_highest_bit & _a_element) == _c_highest_bit};
        T1         _l_rv{0};
        const std::size_t _l_element{_a_element & _c_non_highest_bit};
        std::memcpy(&_l_rv, &_l_element, sizeof(T1));
        if (_l_sign_bit)
        {
            _l_rv = -_l_rv;
        }
        return _l_rv;
    }

    static std::size_t
        encode_into_size_t(
            T _a_input
        )
    {
        using namespace std;
        size_t _l_input_as_int{0};
        std::memcpy(&_l_input_as_int, &_a_input, sizeof(T));
        bitset<CHAR_BIT * sizeof(T)>      _l_bitset{_l_input_as_int};
        bitset<sizeof(size_t) * CHAR_BIT> _l_sizet_bitset{};
        if (_l_bitset[sizeof(T) * CHAR_BIT - 1] == true)
        {
            _l_sizet_bitset[sizeof(size_t) * CHAR_BIT - 1] = true;
        }
        for (size_t _l_idx{sizeof(T) * CHAR_BIT - 1}; _l_idx > 0; --_l_idx)
        {
            _l_sizet_bitset[sizeof(size_t) * CHAR_BIT - _l_idx - 1]
                = _l_bitset[sizeof(T) * CHAR_BIT - _l_idx - 1];
        }
        return static_cast<size_t>(_l_sizet_bitset.to_ullong());
    }

    __constexpr virtual enumeration_diff_t
        difference(
            const T& _a_arg1,
            const T& _a_arg2
        ) noexcept
    {
        using namespace std;
        /*!
         * For floating point types (henceforth called FP) this is harder.
         *
         * This is because we cannot guarantee that continually adding
         * difference will always incremeht a number.
         *
         * So instead, the algorithm is in two stages:
         *
         * 1) Search for the number which adding _m_difference to will not
         * change the result. If there is no number which this occours in the
         * range [_a_arg1, _a_arg2], then find the difference between the two
         * dividied by _m_difference, find the remainder of the difference
         * between the two and _m_difference, then return these values.
         *
         * 2) Call this resulting number k. Find the difference between the
         * start number and k, then divide by _m_difference. Call this number
         * s1. Then find the number of times nextafter must be called on k to
         * obtain arg_2. Add s1 to this number, call it s2. Return {s2,0}.
         */
        const T& _l_lesser{less_than(_a_arg1, _a_arg2) ? _a_arg1 : _a_arg2};
        const T& _l_greater{less_than(_a_arg1, _a_arg2) ? _a_arg2 : _a_arg1};
        // If numbers are equal nothing to be done.
        if (_l_lesser == _l_greater)
        {
            return {0, 0};
        }
        // lesser < greater < neg_add_barrier < pos_add_barrier
        if (_l_greater < _m_neg_add_barrier)
        {
            return {get_increments_between_values(_l_lesser, _l_greater), 0};
        }
        // lesser < neg_add_barrier <= greater < pos_add_barrier
        else if (_l_lesser < _m_neg_add_barrier
                 && _l_greater < _m_pos_add_barrier)
        {
            const pair<size_t, size_t>
                _l_div_and_rem_from_neg_barrier_to_greater{
                    get_additions_between_elements(
                        _m_neg_add_barrier, _l_greater
                    )
                };
            return {
                get_increments_between_values(_l_lesser, _m_neg_add_barrier)
                    + _l_div_and_rem_from_neg_barrier_to_greater.first,
                _l_div_and_rem_from_neg_barrier_to_greater.second
            };
        }
        // lesser < neg_add_barrier < pos_add_barrier <= greater
        else if (_l_lesser < _m_neg_add_barrier
                 && _m_pos_add_barrier <= _l_greater)
        {
            return {
                get_increments_between_values(_l_lesser, _m_neg_add_barrier)
                    + get_additions_between_elements(
                          _m_neg_add_barrier, _m_pos_add_barrier
                    )
                          .first
                    + get_increments_between_values(
                        _m_pos_add_barrier, _l_greater
                    ),
                0
            };
        }
        // neg_add_barrier <= lesser < greater < pos_add_barrier
        else if (_m_neg_add_barrier <= _l_lesser
                 && _l_greater < _m_pos_add_barrier)
        {
            return get_additions_between_elements(_l_lesser, _l_greater);
        }
        // neg_add_barrier <= lesser < pos_add_barrier <= greater
        else if (_m_neg_add_barrier <= _l_lesser
                 && _m_pos_add_barrier <= _l_greater)
        {
            const pair<size_t, size_t>
                _l_div_and_rem_from_lesser_to_pos_barrier{
                    get_additions_between_elements(
                        _l_lesser, _m_pos_add_barrier
                    )
                };
            return {
                get_increments_between_values(_m_pos_add_barrier, _l_greater)
                    + _l_div_and_rem_from_lesser_to_pos_barrier.first,
                _l_div_and_rem_from_lesser_to_pos_barrier.second
            };
        }
        // neg_add_barrier < < pos_add_barrier <= lesser < greater
        else
        {
            return {get_increments_between_values(_l_lesser, _l_greater), 0};
        }
    }

    __constexpr
    default_enumeration_t(
        const T _a_difference,
        const T _a_add_barrier
    ) noexcept
        : _m_difference(_a_difference)
        , _m_pos_add_barrier(_a_add_barrier)
        , _m_neg_add_barrier(-_a_add_barrier)
    {}

    __constexpr
    default_enumeration_t(
        const T _a_difference = T(1)
    ) noexcept
        : default_enumeration_t<T>(
              _a_difference,
              find_point_at_which_increment_no_longer_works(_a_difference)
          )
    {}

    __constexpr_imp virtual bool
        less_than(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _a_l < _a_r;
    }

    __constexpr_imp virtual bool
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _a_l == _a_r;
    }

    __constexpr bool
        increment(
            T&                      _a_element,
            enumerate_index_t&      _a_times_called,
            const std::optional<T>& _a_max_value = std::optional<T>{}
        )
    {
        using namespace std;
        // _a_element < _m_neg_add_barrier.
        if (_a_element < _m_neg_add_barrier)
        {
            // This says we're gonna have to exit if we're not done moving
            // through the current range.
            bool _l_hard_limit{
                _a_max_value.has_value()
                && _a_max_value.value() <= _m_neg_add_barrier
            };
            // What is the limit we're approaching for this if statmenet?
            T _l_below_neg_add_barrier_limit{
                _l_hard_limit ? _a_max_value.value() : _m_neg_add_barrier
            };
            while (_a_times_called > 0)
            {
                _a_element = std::nextafter(
                    _a_element, _l_below_neg_add_barrier_limit
                );
                --_a_times_called;
                if (_a_element == _l_below_neg_add_barrier_limit)
                {
                    if (_l_hard_limit)
                    {
                        return false;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (_a_times_called == 0)
            {
                return true;
            }
        }
        if (_a_element < _m_pos_add_barrier)
        {
            // This says we're gonna have to exit if we're not done moving
            // through the current range.
            bool _l_hard_limit{
                _a_max_value.has_value()
                && _a_max_value.value() <= _m_pos_add_barrier
            };
            // What is the limit we're approaching for this if statmenet?
            T _l_below_pos_add_barrier_limit{
                _l_hard_limit ? _a_max_value.value() : _m_pos_add_barrier
            };
            const enumerate_index_t _l_n_increments_to_get_to_max_value{
                static_cast<size_t>(
                    (_l_below_pos_add_barrier_limit - _a_element)
                    / _m_difference
                )
            };
            if (_l_n_increments_to_get_to_max_value >= _a_times_called)
            {
                size_t _l_times_called{to_size_t(_a_times_called)};
                _a_element += static_cast<T>(_l_times_called) * _m_difference;
                _a_times_called = 0;
                return true;
            }
            else
            {
                _a_times_called += (_l_n_increments_to_get_to_max_value);
                _a_element       = _l_below_pos_add_barrier_limit;
                if (_l_hard_limit)
                {
                    return false;
                }
            }
        }

        // if (_a_element < numeric_limits<T>::max())
        {
            // This says we're gonna have to exit if we're not done moving
            // through the current range.
            bool _l_hard_limit{_a_max_value.has_value()};
            // What is the limit we're approaching for this if statmenet?
            T _l_below_max_numb_limit{
                _l_hard_limit ? _a_max_value.value() : numeric_limits<T>::max()
            };
            while (_a_times_called > 0)
            {
                _a_element
                    = std::nextafter(_a_element, _l_below_max_numb_limit);
                --_a_times_called;
                if (_a_element == _l_below_max_numb_limit)
                {
                    return false;
                }
            }
            return true;
        }
    }

    __constexpr bool
        decrement(
            T&                      _a_element,
            enumerate_index_t&      _a_times_called,
            const std::optional<T>& _a_max_value = std::optional<T>{}
        )
    {
        using namespace std;
        // _a_element < _m_neg_add_barrier.
        if (_a_element > _m_pos_add_barrier)
        {
            // This says we're gonna have to exit if we're not done moving
            // through the current range.
            bool _l_hard_limit{
                _a_max_value.has_value()
                && _a_max_value.value() >= _m_pos_add_barrier
            };
            // What is the limit we're approaching for this if statmenet?
            T _l_above_pos_add_barrier_limit{
                _l_hard_limit ? _a_max_value.value() : _m_pos_add_barrier
            };
            while (_a_times_called > 0)
            {
                _a_element = std::nextafter(
                    _a_element, _l_above_pos_add_barrier_limit
                );
                --_a_times_called;
                if (_a_element == _l_above_pos_add_barrier_limit)
                {
                    if (_l_hard_limit)
                    {
                        return false;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (_a_times_called == 0)
            {
                return true;
            }
        }
        if (_a_element > _m_neg_add_barrier)
        {
            // This says we're gonna have to exit if we're not done moving
            // through the current range.
            bool _l_hard_limit{
                _a_max_value.has_value()
                && _a_max_value.value() >= _m_neg_add_barrier
            };
            // What is the limit we're approaching for this if statmenet?
            T _l_above_neg_add_barrier_limit{
                _l_hard_limit ? _a_max_value.value() : _m_neg_add_barrier
            };
            const enumerate_index_t _l_n_decrements_to_get_to_max_value{
                static_cast<size_t>(
                    (_a_element - _l_above_neg_add_barrier_limit)
                    / _m_difference
                )
            };
            if (_l_n_decrements_to_get_to_max_value >= _a_times_called)
            {
                size_t _l_times_called{to_size_t(_a_times_called)};
                _a_element -= static_cast<T>(_l_times_called) * _m_difference;
                _a_times_called = 0;
                return true;
            }
            else
            {
                _a_times_called += (_l_n_decrements_to_get_to_max_value);
                _a_element       = _l_above_neg_add_barrier_limit;
                if (_l_hard_limit)
                {
                    return false;
                }
            }
        }

        // if (_a_element < numeric_limits<T>::max())
        {
            // This says we're gonna have to exit if we're not done moving
            // through the current range.
            bool _l_hard_limit{_a_max_value.has_value()};
            // What is the limit we're approaching for this if statmenet?
            T _l_below_min_numb_limit{
                _l_hard_limit ? _a_max_value.value()
                              : numeric_limits<T>::lowest()
            };
            while (_a_times_called > 0)
            {
                _a_element
                    = std::nextafter(_a_element, _l_below_min_numb_limit);
                --_a_times_called;
                if (_a_element == _l_below_min_numb_limit)
                {
                    return false;
                }
            }
            return true;
        }
    }
private:
    T           _m_difference;
    T           _m_pos_add_barrier;
    T           _m_neg_add_barrier;

    __constexpr std::size_t
                get_increments_between_values(
                    const T& _a_lesser,
                    const T& _a_greater
                )
    {
        // This uses the code from
        // https://stackoverflow.com/questions/55049251/how-many-values-can-be-represented-in-a-given-range-by-a-float
        size_t _l_arg1{encode_into_size_t(_a_lesser)};
        size_t _l_arg2{encode_into_size_t(_a_greater)};
        if (_c_highest_bit & _l_arg1)
        {
            _l_arg1 = _c_highest_bit - _l_arg1;
        }
        if (_c_highest_bit & _l_arg2)
        {
            _l_arg2 = _c_highest_bit - _l_arg2;
        }
        // Return the distance between the two transformed encodings.
        return _l_arg2 - _l_arg1;
    }

    __constexpr std::pair<std::size_t, std::size_t>
                get_additions_between_elements(
                    const T& _a_lower,
                    const T& _a_higher
                ) const noexcept
    {
        const T _l_difference{_a_higher - _a_lower};
        return {
            static_cast<size_t>(_l_difference / _m_difference),
            static_cast<size_t>(remainder(_l_difference, _m_difference))
        };
    }
};

template <>
struct default_enumeration_t<bool> : public enumeration_base_t<bool>
{
    __constexpr_imp virtual bool
        less_than(const bool& _a_l, const bool& _a_r) const noexcept;
    __constexpr_imp virtual bool
        equal(const bool& _a_l, const bool& _a_r) const noexcept;
    __no_constexpr bool
        increment(
            bool&                      _a_element,
            enumerate_index_t&         _a_n_times_to_increment,
            const std::optional<bool>& _a_max_value
        );
    __no_constexpr bool
        decrement(
            bool&                      _a_element,
            enumerate_index_t&         _a_n_times_to_increment,
            const std::optional<bool>& _a_max_value
        );

    __no_constexpr_imp enumeration_diff_t
        difference(
            const bool& _a_arg1,
            const bool& _a_arg2
        ) noexcept
    {
        if (_a_arg1 == _a_arg2)
        {
            return {0, 0};
        }
        else
        {
            return {1, 0};
        }
    }
};

template <typename T, std::size_t N>
struct default_enumeration_t<std::array<T, N>>
    : public enumeration_base_t<std::array<T, N>>
{
private:
    enumeration_schema_t<T> _m_enumerate;
    std::size_t             _m_n_jumps;
public:
    __constexpr_imp
        default_enumeration_t(
            const std::size_t              _a_n_jumps   = std::size_t{1},
            const enumeration_schema_t<T>& _a_enumerate = all_values<T>()
        );
    __constexpr_imp virtual bool
        less_than(const std::array<T, N>& _a_l, const std::array<T, N>& _a_r)
            const noexcept;
    __constexpr_imp virtual bool
        equal(const std::array<T, N>& _a_l, const std::array<T, N>& _a_r)
            const noexcept;
    __constexpr_imp bool
        increment(
            std::array<T, N>&                      _a_array,
            enumerate_index_t&                     _a_n_times_to_increment,
            const std::optional<std::array<T, N>>& _a_max_value
        );
    __constexpr_imp bool
        decrement(
            std::array<T, N>&                      _a_array,
            enumerate_index_t&                     _a_n_times_to_increment,
            const std::optional<std::array<T, N>>& _a_max_value
        );

    __constexpr virtual enumeration_diff_t
        difference(
            const std::array<T, N>& _a_arg1,
            const std::array<T, N>& _a_arg2
        ) noexcept
    {
        return {0, 0};
    }
};

template <typename T>
struct default_enumeration_t<std::optional<T>>
    : public enumeration_base_t<std::optional<T>>
{
private:
    enumeration_schema_t<T> _m_enumerate;
    std::size_t             _m_n_jumps;
public:
    __constexpr_imp
        default_enumeration_t(
            const std::size_t              _a_n_jumps = std::size_t{1},
            const enumeration_schema_t<T>& _a_enumerate
            = all_values<T>(default_enumeration<T>())
        )
        : _m_enumerate(_a_enumerate)
        , _m_n_jumps(_a_n_jumps)
    {}

    __constexpr_imp virtual bool
        less_than(
            const std::optional<T>& _a_l,
            const std::optional<T>& _a_r
        ) const noexcept
    {
        if (_a_l.has_value() && _a_r.has_value())
        {
            return _m_enumerate->less_than(_a_l.value(), _a_r.value());
        }
        else
        {
            if (_a_l.has_value())
            {
                return false;
            }
            else if (_a_r.has_value())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    __constexpr_imp virtual bool
        equal(
            const std::optional<T>& _a_l,
            const std::optional<T>& _a_r
        ) const noexcept
    {
        if (_a_l.has_value() && _a_r.has_value())
        {
            return _m_enumerate->equal(_a_l.value(), _a_r.value());
        }
        else
        {
            if (_a_l.has_value() || _a_r.has_value())
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

    __constexpr_imp bool
        increment(
            std::optional<T>&                      _a_opt,
            enumerate_index_t&                     _a_n_times_to_increment,
            const std::optional<std::optional<T>>& _a_max_value
        )
    {
        using namespace std;
        const optional<T> _l_max_value{
            _a_max_value.has_value() ? _a_max_value.value()
                                     : make_optional(_m_enumerate->end_value())
        };
        if (less_than(_l_max_value, _a_opt))
        {
            _ABC_UNREACHABLE(u8"couldn't reach");
        }
        else
        {
            if (equal(_l_max_value, _a_opt) || _a_n_times_to_increment == 0)
            {
                return _a_n_times_to_increment == 0;
            }
            if (not _a_opt.has_value())
            {
                _a_opt = make_optional(_m_enumerate->start_value());
                --_a_n_times_to_increment;
            }
            return _m_enumerate->increment(
                _a_opt.value(), _a_n_times_to_increment, _l_max_value.value()
            );
        }
    }

    __constexpr_imp bool
        decrement(
            std::optional<T>&                      _a_opt,
            enumerate_index_t&                     _a_n_times_to_increment,
            const std::optional<std::optional<T>>& _a_min_value
        )
    {
        using namespace std;
        const optional<T> _l_min_value{
            _a_min_value.has_value() ? _a_min_value.value() : std::nullopt
        };
        if (less_than(_a_opt, _l_min_value))
        {
            _ABC_UNREACHABLE(u8"couldn't reach");
        }
        else
        {
            if (equal(_l_min_value, _a_opt) || _a_n_times_to_increment == 0)
            {
                return _a_n_times_to_increment == 0;
            }
            if (_a_opt.has_value())
            {
                // bool _l_opt_result;
                if (_l_min_value.has_value())
                {
                    // _l_opt_result = _m_enumerate->decrement(
                    //     _a_opt.value(),
                    //     _a_n_times_to_increment,
                    //     _l_min_value.value()
                    // );
                }
                else
                {
                    // _l_opt_result = _m_enumerate->decrement(
                    //     _a_opt.value(),
                    //     _a_n_times_to_increment,
                    //     _m_enumerate->start_value()
                    // );
                }
                if (_a_n_times_to_increment == 0)
                {
                    return true;
                }
            }
            if (_a_n_times_to_increment > 0 && _l_min_value == std::nullopt)
            {
                --_a_n_times_to_increment;
                _a_opt = std::nullopt;
            }
            return _a_n_times_to_increment == 0;
        }
    }

    __constexpr virtual enumeration_diff_t
        difference(
            const std::optional<T>& _a_arg1,
            const std::optional<T>& _a_arg2
        ) noexcept
    {
        return {0, 0};
    }
};

template <typename T>
struct default_enumeration_t<std::vector<T>>
    : public enumeration_base_t<std::vector<T>>
{
private:
    enumeration_schema_t<T> _m_enumerate;
    std::size_t             _m_n_jumps;
public:
    __constexpr_imp
        default_enumeration_t(const enumeration_schema_t<T>& _a_enumerate);
    __constexpr_imp
        default_enumeration_t(
            const std::size_t              _a_n_jumps = std::size_t{1},
            const enumeration_schema_t<T>& _a_schema  = all_values<T>()
        );
    __constexpr_imp virtual bool
        less_than(const std::vector<T>& _a_l, const std::vector<T>& _a_r)
            const noexcept;
    __constexpr_imp bool
        is_return_value_within_range(
            const std::vector<T>&                _a_element,
            const std::optional<std::vector<T>>& _a_opt_max_value
        ) const noexcept;
    __constexpr_imp void
        either_increment_vector_or_create_next_largest_vector(
            std::vector<T>&    _a_vector,
            enumerate_index_t& _a_n_times_to_increment
        ) const noexcept;
    __constexpr_imp virtual bool
        equal(const std::vector<T>& _a_l, const std::vector<T>& _a_r)
            const noexcept;

    __constexpr_imp bool
        increment(
            std::vector<T>&                      _a_array,
            enumerate_index_t&                   _a_n_times_to_increment,
            const std::optional<std::vector<T>>& _a_max_value
        );
    __constexpr_imp bool
        decrement(
            std::vector<T>&                      _a_array,
            enumerate_index_t&                   _a_n_times_to_increment,
            const std::optional<std::vector<T>>& _a_max_value
        );

    __constexpr virtual enumeration_diff_t
        difference(
            const std::vector<T>& _a_arg1,
            const std::vector<T>& _a_arg2
        ) noexcept
    {
        using namespace std;
        enumeration_diff_t _l_rv{0, 0};
        // Find the lesser of the two arguments.
        const bool _l_arg_1_lesser{less_than(_a_arg1, _a_arg2)};
        auto&      _l_lesser{_l_arg_1_lesser ? _a_arg1 : _a_arg2};
        auto&      _l_greater{_l_arg_1_lesser ? _a_arg2 : _a_arg1};
        // Find out how many advancements per "increment".
        const enumerate_index_t _l_divisor{
            _m_enumerate->n_advancements_per_advancement()
        };
        size_t             _l_greater_idx{_l_greater.size() - 1};
        vector<T>          _l_local_lesser{_l_lesser};
        enumeration_diff_t _l_biggest_difference{_m_enumerate->difference(
            _m_enumerate->start_value(), _m_enumerate->end_value()
        )};
        // This is the "size" of the enumerator. Think of it as an entity with
        // ten elements would have a biggest_difference of 10. It is like the
        // arity of the entity.
        _l_biggest_difference.second += 1;
        for (size_t _l_idx{_l_lesser.size()}; _l_idx > 0; --_l_idx)
        {
            size_t _l_idx_offset{_l_idx - 1};
            // To get to i requires (X,Y).
            // enumeration_diff_t _l_this_diff{0, 0};
            if (_m_enumerate->equal(
                    _l_lesser[_l_idx_offset], _l_greater[_l_greater_idx]
                ))
            {
            }
            else if (_m_enumerate->less_than(
                         _l_lesser[_l_idx_offset], _l_greater[_l_greater_idx]
                     ))
            {
                enumeration_diff_t _l_local_diff = _m_enumerate->difference(
                    _l_lesser[_l_idx_offset], _l_greater[_l_greater_idx]
                );
                _l_rv.first  += _l_local_diff.first;
                _l_rv.second += _l_local_diff.second;
            }
            else
            {
                enumeration_diff_t _l_local_diff = _m_enumerate->difference(
                    _l_lesser[_l_idx_offset], _l_greater[_l_greater_idx]
                );
                enumeration_diff_t _l_local_diff_2 = _l_biggest_difference;
                if (_l_local_diff_2.second > _l_local_diff.second)
                {
                    _l_local_diff_2.first--;
                    _l_local_diff_2.second += _l_divisor;
                }
                _l_local_diff_2.second -= _l_local_diff.second;
                _l_local_diff_2.first  -= _l_local_diff.first;
                _l_rv.first            += _l_local_diff_2.first;
                _l_rv.second           += _l_local_diff_2.second;
            }
            _l_greater_idx--;
        }
        // Gone through all of the shared indexes. What remains is how to
        // increment the lesser vector to the greater's value.
        enumeration_diff_t _l_rolling_product{1, 1};
        // Gone through all of them. Now gotta go through the remaining indexes.
        for (size_t _l_idx{_l_greater.size() - _l_lesser.size()}; _l_idx > 0;
             --_l_idx)
        {
            enumeration_diff_t _l_local_diff = _m_enumerate->difference(
                _m_enumerate->start_value(), _l_greater[_l_idx - 1]
            );
            _l_local_diff.second += 1;
            _l_rv.first  += _l_local_diff.first * _l_rolling_product.first;
            _l_rv.second += _l_local_diff.second * _l_rolling_product.second;
            _l_rolling_product.first  *= _l_biggest_difference.first;
            _l_rolling_product.second *= _l_biggest_difference.second;
        }
        enumerate_index_t _l_remainder{_l_rv.second};
        _l_rv.first  += (_l_remainder / _l_divisor);
        _l_rv.second  = (_l_remainder % _l_divisor);
        string si     = bigUnsignedToString(_l_rv.first);
        return _l_rv;
    }
};

template <typename T>
requires _ABC_NS_UTILITY::enum_has_list_c<T>
struct default_enumeration_t<T> : public enumeration_base_t<T>
{
public:
    using value_type_t = T;

    __constexpr_imp virtual bool
        less_than(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        const auto& _l_ti{utility::get_thread_local_enumerate_enum_helper<T>()};
        return _l_ti.less_than(_a_l, _a_r);
    }

    __constexpr_imp virtual bool
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        const auto& _l_ti{utility::get_thread_local_enumerate_enum_helper<T>()};
        return _l_ti.equal(_a_l, _a_r);
    }

    __constexpr virtual bool
        increment(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        )
    {
        const auto& _l_ti{utility::get_thread_local_enumerate_enum_helper<T>()};
        return _l_ti.increment(
            _a_element, _a_n_times_to_increment, _a_max_value
        );
    }

    __constexpr virtual bool
        decrement(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        )
    {
        const auto& _l_ti{utility::get_thread_local_enumerate_enum_helper<T>()};
        return _l_ti.decrement(
            _a_element, _a_n_times_to_increment, _a_max_value
        );
    }

    __constexpr virtual enumeration_diff_t
        difference(
            const T& _a_arg1,
            const T& _a_arg2
        ) noexcept
    {
        const auto& _l_ti{utility::get_thread_local_enumerate_enum_helper<T>()};
        return std::make_pair(_l_ti.difference(_a_arg1, _a_arg2), 0);
    }
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

// For some reason, it appears that MSVC rejects this code when it shouldn't.
/*template <typename T>
requires has_limits_c<T>
__constexpr_imp T
    min_value_t<T>::min_value() const noexcept
{
    return std::numeric_limits<T>::min();
}

template <typename T>
requires has_limits_c<T>
__constexpr_imp T
    max_value_t<T>::max_value() const noexcept
{
    return std::numeric_limits<T>::max();
}

template <typename T>
    requires has_addition_c<T>&& has_subtraction_c<T>&& has_less_than_c<T>
&& has_equal_c<T> && (not std::is_enum_v<T>)
__constexpr_imp
    enumeration_data_object_t<T>::enumeration_data_object_t(
        const T _a_difference
    ) noexcept
    : _m_difference(_a_difference)
{}

template <typename T>
    requires has_addition_c<T>&& has_subtraction_c<T>&& has_less_than_c<T>
&& has_equal_c<T> && (not std::is_enum_v<T>)
__constexpr_imp bool
    enumeration_data_object_t<T>::less_than(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    return _a_l < _a_r;
}

template <typename T>
    requires has_addition_c<T>&& has_subtraction_c<T>&& has_less_than_c<T>
&& has_equal_c<T> && (not std::is_enum_v<T>)
__constexpr_imp bool
    enumeration_data_object_t<T>::equal(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    return _a_l == _a_r;
}

template <typename T>
    requires has_addition_c<T>&& has_subtraction_c<T>&& has_less_than_c<T>
&& has_equal_c<T> && (not std::is_enum_v<T>)
__constexpr bool
    enumeration_data_object_t<T>::increment(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_max_value
    )
{
    using namespace std;
    const T _l_current_idx{_a_element};
    if (_a_max_value.has_value())
    {
        const T _l_max_times_called{static_cast<T>(
            (_a_max_value.value() - _l_current_idx) / _m_difference
        )};
        if (_a_times_called > _l_max_times_called)
        {
            if (_l_max_times_called == 0)
            {
                return false;
            }
            else
            {
                _a_element = _a_element + _m_difference * _l_max_times_called;
                _a_times_called -= _l_max_times_called;
                return true;
            }
        }
        else
        {
            _a_element      = _a_element + _m_difference * _a_times_called;
            _a_times_called = 0;
            return true;
        }
    }
    else
    {
        _a_element      = _a_element + _m_difference * _a_times_called;
        _a_times_called = 0;
        return true;
    }
}

template <typename T>
    requires has_addition_c<T>&& has_subtraction_c<T>&& has_less_than_c<T>
&& has_equal_c<T> && (not std::is_enum_v<T>)
__constexpr bool
    enumeration_data_object_t<T>::decrement(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_max_value
    )
{
    using namespace std;
    const T _l_current_idx{_a_element};
    if (_a_max_value.has_value())
    {
        const T _l_max_times_called{static_cast<T>(
            (_l_current_idx - _a_max_value.value()) / _m_difference
        )};
        if (_a_times_called > _l_max_times_called)
        {
            if (_l_max_times_called == 0)
            {
                return false;
            }
            else
            {
                _a_element = _a_element - _m_difference * _l_max_times_called;
                _a_times_called += _l_max_times_called;
                return true;
            }
        }
        else
        {
            _a_element      = _a_element - _m_difference * _a_times_called;
            _a_times_called = 0;
            return true;
        }
    }
    else
    {
        _a_element      = _a_element - _m_difference * _a_times_called;
        _a_times_called = 0;
        return true;
    }
}*/

__constexpr_imp bool
    default_enumeration_t<bool>::less_than(
        const bool& _a_l,
        const bool& _a_r
    ) const noexcept
{
    return _a_l < _a_r;
}

__constexpr_imp bool
    default_enumeration_t<bool>::equal(
        const bool& _a_l,
        const bool& _a_r
    ) const noexcept
{
    return _a_l == _a_r;
}

__no_constexpr_imp bool
    default_enumeration_t<bool>::increment(
        bool&                      _a_element,
        enumerate_index_t&         _a_n_times_to_increment,
        const std::optional<bool>& _a_max_value
    )
{
    if (_a_n_times_to_increment > 0 && _a_element == false
        && _a_max_value == true)
    {
        _a_element = true;
        _a_n_times_to_increment--;
        return true;
    }
    else
    {
        return false;
    }
}

__no_constexpr_imp bool
    default_enumeration_t<bool>::decrement(
        bool&                      _a_element,
        enumerate_index_t&         _a_n_times_to_increment,
        const std::optional<bool>& _a_max_value
    )
{
    if (_a_n_times_to_increment > 0 && _a_element == true
        && _a_max_value == false)
    {
        _a_element = false;
        _a_n_times_to_increment--;
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T, std::size_t N>
__constexpr_imp
    default_enumeration_t<std::array<T, N>>::default_enumeration_t(
        const std::size_t              _a_n_jumps,
        const enumeration_schema_t<T>& _a_enumerate
    )
    : _m_enumerate(_a_enumerate), _m_n_jumps(_a_n_jumps)
{}

template <typename T, std::size_t N>
__constexpr_imp bool
    default_enumeration_t<std::array<T, N>>::less_than(
        const std::array<T, N>& _a_l,
        const std::array<T, N>& _a_r
    ) const noexcept
{
    for (size_t _l_idx{N}; _l_idx > 0; --_l_idx)
    {
        if (_m_enumerate.less_than(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
        {
            return true;
        }
        else if (not _m_enumerate.equal(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
        {
            return false;
        }
    }
    return false;
}

template <typename T, std::size_t N>
__constexpr_imp bool
    default_enumeration_t<std::array<T, N>>::equal(
        const std::array<T, N>& _a_l,
        const std::array<T, N>& _a_r
    ) const noexcept
{
    for (size_t _l_idx{N}; _l_idx > 0; --_l_idx)
    {
        if (_m_enumerate.less_than(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
        {
            return false;
        }
        else if (not _m_enumerate.equal(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
        {
            return false;
        }
    }
    return true;
}

template <typename T, std::size_t N>
__constexpr_imp bool
    default_enumeration_t<std::array<T, N>>::increment(
        std::array<T, N>&                      _a_array,
        enumerate_index_t&                     _a_n_times_to_increment,
        const std::optional<std::array<T, N>>& _a_max_value
    )
{
    while (_a_n_times_to_increment > 0)
    {
        T& _l_elem{_a_array[N - 1]};
        if (_m_enumerate.next_element(_l_elem, _a_n_times_to_increment))
        {
            if (_a_n_times_to_increment == 0)
            {
                return _a_array <= _a_max_value;
            }
            else
            {
                // Set it to min, increment.
                _l_elem = _m_enumerate.min();
                for (size_t _l_idx{N - 1}; _l_idx > 0; --_l_idx)
                {
                    std::size_t _l_arg{1};
                    if (_m_enumerate.next_element(_a_array[_l_idx - 1], _l_arg))
                    {
                        --_a_n_times_to_increment;
                        break;
                    }
                    else
                    {
                        _a_array[_l_idx - 1] = _m_enumerate.min();
                        if (_l_idx == 1)
                        {
                            return false;
                        }
                    }
                }
            }
        }
        else
        {
            // Set it to min, increment.
            _l_elem = _m_enumerate.min();
            for (size_t _l_idx{N - 1}; _l_idx > 0; --_l_idx)
            {
                std::size_t _l_arg{1};
                if (_m_enumerate.next_element(_a_array[_l_idx - 1], _l_arg))
                {
                    --_a_n_times_to_increment;
                    break;
                }
                else
                {
                    _a_array[_l_idx - 1] = _m_enumerate.min();
                    if (_l_idx == 1)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return _a_array <= _a_max_value;
}

template <typename T, std::size_t N>
__constexpr_imp bool
    default_enumeration_t<std::array<T, N>>::decrement(
        std::array<T, N>&                      _a_array,
        enumerate_index_t&                     _a_n_times_to_increment,
        const std::optional<std::array<T, N>>& _a_max_value
    )
{
    while (_a_n_times_to_increment > 0)
    {
        T& _l_elem{_a_array[N - 1]};
        if (_m_enumerate.move_backward(_l_elem, _a_n_times_to_increment))
        {
            if (_a_n_times_to_increment == 0)
            {
                return _a_array >= _a_max_value;
            }
            else
            {
                // Set it to min, increment.
                _l_elem = _m_enumerate.max();
                for (size_t _l_idx{N - 1}; _l_idx > 0; --_l_idx)
                {
                    std::size_t _l_arg{1};
                    if (_m_enumerate.move_backward(
                            _a_array[_l_idx - 1], _l_arg
                        ))
                    {
                        --_a_n_times_to_increment;
                        break;
                    }
                    else
                    {
                        _a_array[_l_idx - 1] = _m_enumerate.max();
                        if (_l_idx == 1)
                        {
                            return false;
                        }
                    }
                }
            }
        }
        else
        {
            _l_elem = _m_enumerate.max();
            for (size_t _l_idx{N - 1}; _l_idx > 0; --_l_idx)
            {
                std::size_t _l_arg{1};
                if (_m_enumerate.move_backward(_a_array[_l_idx - 1], _l_arg))
                {
                    --_a_n_times_to_increment;
                    break;
                }
                else
                {
                    _a_array[_l_idx - 1] = _m_enumerate.max();
                    if (_l_idx == 1)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return _a_array >= _a_max_value;
}

template <typename T>
__constexpr_imp
    default_enumeration_t<std::vector<T>>::default_enumeration_t(
        const enumeration_schema_t<T>& _a_schema
    )
    : default_enumeration_t<std::vector<T>>(std::size_t{1}, _a_schema)
{}

template <typename T>
__constexpr_imp
    default_enumeration_t<std::vector<T>>::default_enumeration_t(
        const std::size_t              _a_n_jumps,
        const enumeration_schema_t<T>& _a_schema
    )
    : _m_enumerate(_a_schema), _m_n_jumps(_a_n_jumps)
{}

template <typename T>
__constexpr_imp bool
    default_enumeration_t<std::vector<T>>::less_than(
        const std::vector<T>& _a_l,
        const std::vector<T>& _a_r
    ) const noexcept
{
    if (_a_l.size() < _a_r.size())
    {
        return true;
    }
    else if (_a_l.size() > _a_r.size())
    {
        return false;
    }
    else
    {
        for (size_t _l_idx{0}; _l_idx < _a_l.size(); ++_l_idx)
        {
            if (_m_enumerate->less_than(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return true;
            }
            else if (not _m_enumerate->equal(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return false;
            }
        }
        return false;
    }
}

template <typename T>
__constexpr_imp bool
    default_enumeration_t<std::vector<T>>::is_return_value_within_range(
        const std::vector<T>&                _a_element,
        const std::optional<std::vector<T>>& _a_opt_max_value
    ) const noexcept
{
    if (_a_opt_max_value.has_value())
    {
        return less_than(_a_element, _a_opt_max_value.value())
               || equal(_a_element, _a_opt_max_value.value());
    }
    else
    {
        return true;
    }
}

template <typename T>
__constexpr_imp void
    default_enumeration_t<std::vector<T>>::
        either_increment_vector_or_create_next_largest_vector(
            std::vector<T>&    _a_vector,
            enumerate_index_t& _a_n_times_to_increment
        ) const noexcept
{
    bool _l_change_made{false};
    if (_a_vector.size() > 0)
    {
        _a_vector.back() = _m_enumerate->start_value();
        for (size_t _l_idx{_a_vector.size() - 1}; _l_idx > 0; --_l_idx)
        {
            enumerate_index_t _l_arg{1};
            if (_m_enumerate->increment(
                    _a_vector[_l_idx - 1], _l_arg, _m_enumerate->end_value()
                ))
            {
                --_a_n_times_to_increment;
                _l_change_made = true;
                break;
            }
            else
            {
                _a_vector[_l_idx - 1] = _m_enumerate->start_value();
            }
        }
    }
    if (_l_change_made == false)
    {
        --_a_n_times_to_increment;
        _a_vector.resize(_a_vector.size() + 1);
        for (size_t _l_idx{0}; _l_idx < _a_vector.size(); ++_l_idx)
        {
            _a_vector[_l_idx] = _m_enumerate->start_value();
        }
    }
}

template <typename T>
__constexpr_imp bool
    default_enumeration_t<std::vector<T>>::equal(
        const std::vector<T>& _a_l,
        const std::vector<T>& _a_r
    ) const noexcept
{
    if (_a_l.size() < _a_r.size())
    {
        return true;
    }
    else if (_a_l.size() > _a_r.size())
    {
        return false;
    }
    else
    {
        for (size_t _l_idx{0}; _l_idx < _a_l.size(); ++_l_idx)
        {
            if (_m_enumerate->less_than(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return false;
            }
            else if (not _m_enumerate->equal(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return false;
            }
        }
        return true;
    }
}

template <typename T>
__constexpr_imp bool
    default_enumeration_t<std::vector<T>>::increment(
        std::vector<T>&                      _a_array,
        enumerate_index_t&                   _a_n_times_to_increment,
        const std::optional<std::vector<T>>& _a_max_value
    )
{
    // Each "increment" contains "_m_n_jumps" in it. Multiply by this amount.
    // Ignore overflows at this point.
    //  Potential options for dealing with them include:
    //  - Use an unbounded unsigned number type.
    //  - Throw an exception.
    //  - Display a warning.
    _a_n_times_to_increment *= _m_n_jumps;
    while (_a_n_times_to_increment > 0)
    {
        if (_a_array.size() == 0)
        {
            either_increment_vector_or_create_next_largest_vector(
                _a_array, _a_n_times_to_increment
            );
            //_a_array.resize(1);
            //_a_n_times_to_increment--;
            //_a_array[0] = _m_enumerate.min();
            // return is_return_value_within_range(_a_array, _a_max_value);
        }
        else
        {
            T& _l_elem{_a_array.back()};
            if (_m_enumerate->increment(
                    _l_elem, _a_n_times_to_increment, _m_enumerate->end_value()
                ))
            {
                if (_a_n_times_to_increment == 0)
                {
                    return is_return_value_within_range(_a_array, _a_max_value);
                }
                else
                {
                    either_increment_vector_or_create_next_largest_vector(
                        _a_array, _a_n_times_to_increment
                    );
                }
            }
            else
            {
                either_increment_vector_or_create_next_largest_vector(
                    _a_array, _a_n_times_to_increment
                );
            }
        }
    }
    return is_return_value_within_range(_a_array, _a_max_value);
}

template <typename T>
__constexpr_imp bool
    default_enumeration_t<std::vector<T>>::decrement(
        std::vector<T>&                      _a_array,
        enumerate_index_t&                   _a_n_times_to_increment,
        const std::optional<std::vector<T>>& _a_max_value
    )
{
    while (_a_n_times_to_increment > 0)
    {
        if (_a_array.size() == 0)
        {
            return false;
        }
        T& _l_elem{_a_array[_a_array.size() - 1]};
        if (_m_enumerate->decrement(
                _l_elem, _a_n_times_to_increment, std::optional<T>()
            ))
        {
            if (_a_n_times_to_increment == 0)
            {
                if (_a_max_value.has_value())
                {
                    return (not less_than(_a_array, _a_max_value.value()))
                           && equal(_a_array, _a_max_value.value());
                }
                else
                {
                    return true;
                }
            }
            else
            {
                // Set it to min, increment.
                _l_elem = _m_enumerate->end_value();
                for (size_t _l_idx{_a_array.size() - 1}; _l_idx > 0; --_l_idx)
                {
                    enumerate_index_t _l_arg{1};
                    if (_m_enumerate->decrement(
                            _a_array[_l_idx - 1], _l_arg, std::optional<T>()
                        ))
                    {
                        --_a_n_times_to_increment;
                        break;
                    }
                    else
                    {
                        _a_array[_l_idx - 1] = _m_enumerate->end_value();
                        if (_l_idx == 1)
                        {
                            // time to reset!
                            --_a_n_times_to_increment;
                            _a_array.resize(_a_array.size() - 1);
                            for (size_t _l_idx{0}; _l_idx < _a_array.size();
                                 ++_l_idx)
                            {
                                _a_array[_l_idx] = _m_enumerate->end_value();
                            }
                        }
                    }
                }
            }
        }
        else
        {
            _l_elem = _m_enumerate->end_value();
            for (size_t _l_idx{_a_array.size() - 1}; _l_idx > 0; --_l_idx)
            {
                enumerate_index_t _l_arg{1};
                if (_m_enumerate->decrement(
                        _a_array[_l_idx - 1], _l_arg, std::optional<T>{}
                    ))
                {
                    --_a_n_times_to_increment;
                    break;
                }
                else
                {
                    _a_array[_l_idx - 1] = _m_enumerate->end_value();
                    if (_l_idx == 1)
                    {
                        // time to reset!
                        --_a_n_times_to_increment;
                        _a_array.resize(_a_array.size() - 1);
                        for (size_t _l_idx{0}; _l_idx < _a_array.size();
                             ++_l_idx)
                        {
                            _a_array[_l_idx] = _m_enumerate->end_value();
                        }
                    }
                }
            }
        }
    }
    if (_a_max_value.has_value())
    {
        return (not less_than(_a_array, _a_max_value.value()))
               && equal(_a_array, _a_max_value.value());
    }
    else
    {
        return true;
    }
}

template <typename... Ts>
struct default_enumeration_t<std::tuple<Ts...>>
    : public enumeration_base_t<std::tuple<Ts...>>
{
public:
    using value_type_t = std::tuple<Ts...>;
    __constexpr
    default_enumeration_t(enumeration_schema_t<Ts>... _a_enumerators) noexcept;
    __constexpr
    default_enumeration_t() noexcept
    requires (std::is_default_constructible_v<enumeration_schema_t<Ts>> && ...);
    __constexpr_imp virtual bool
        less_than(const value_type_t& _a_l, const value_type_t& _a_r)
            const noexcept;
    __constexpr_imp virtual bool
        equal(const value_type_t& _a_l, const value_type_t& _a_r)
            const noexcept;
    __constexpr virtual bool
        increment(
            value_type_t&                      _a_element,
            enumerate_index_t&                 _a_n_times_to_increment,
            const std::optional<value_type_t>& _a_max_value
        );
    __constexpr virtual bool
        decrement(
            value_type_t&                      _a_element,
            enumerate_index_t&                 _a_n_times_to_increment,
            const std::optional<value_type_t>& _a_max_value
        );
    __constexpr virtual enumeration_diff_t
        difference(const value_type_t& _a_arg1, const value_type_t& _a_arg2)
            noexcept;
private:
    std::tuple<enumeration_schema_t<Ts>...> _m_enumeration_schemas;
    template <std::size_t I>
    __constexpr bool
        less_than_(const value_type_t& _a_l, const value_type_t& _a_r)
            const noexcept;
    template <std::size_t I>
    __constexpr bool
        equal_(const value_type_t& _a_l, const value_type_t& _a_r)
            const noexcept;
    template <std::size_t I>
    __constexpr bool
        increment_(
            value_type_t&      _a_element,
            enumerate_index_t& _a_n_times_to_increment
        );
    template <std::size_t I>
    __constexpr bool
        decrement_(
            value_type_t&      _a_element,
            enumerate_index_t& _a_n_times_to_increment
        );
    template <std::size_t I>
    __constexpr void
        difference_(
            const value_type_t& _a_lesser,
            const value_type_t& _a_greater,
            enumeration_diff_t& _a_total_diff,
            enumeration_diff_t& _a_running_total
        ) const noexcept;
    template <std::size_t I>
    __constexpr void
        initialize_divisors(
            std::array<enumerate_index_t, std::tuple_size<value_type_t>{}>&
                _a_divisors
        ) const noexcept;
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
template <typename... Ts>
__constexpr_imp
    default_enumeration_t<std::tuple<Ts...>>::default_enumeration_t(
        enumeration_schema_t<Ts>... _a_enums
    ) noexcept
    : _m_enumeration_schemas(_a_enums...)
{}

template <typename... Ts>
__constexpr_imp
    default_enumeration_t<std::tuple<Ts...>>::default_enumeration_t() noexcept
requires (std::is_default_constructible_v<enumeration_schema_t<Ts>> && ...)
    : _m_enumeration_schemas(
          std::make_tuple(all_values<Ts>(default_enumeration<Ts>())...)
      )
{}

template <typename... Ts>
__constexpr_imp bool
    default_enumeration_t<std::tuple<Ts...>>::less_than(
        const value_type_t& _a_l,
        const value_type_t& _a_r
    ) const noexcept
{
    return less_than_<0>(_a_l, _a_r);
}

template <typename... Ts>
__constexpr_imp bool
    default_enumeration_t<std::tuple<Ts...>>::equal(
        const value_type_t& _a_l,
        const value_type_t& _a_r
    ) const noexcept
{
    return equal_<0>(_a_l, _a_r);
}

template <typename... Ts>
__constexpr bool
    default_enumeration_t<std::tuple<Ts...>>::increment(
        value_type_t&                      _a_element,
        enumerate_index_t&                 _a_n_times_to_increment,
        const std::optional<value_type_t>& _a_max_value
    )
{
    using namespace std;
    while (_a_n_times_to_increment > 0)
    {
        if (not increment_<tuple_size<value_type_t>{} - 1>(
                _a_element, _a_n_times_to_increment
            ))
        {
            return false;
        }
    }
    if (_a_n_times_to_increment == 0)
    {
        return _a_element <= _a_max_value;
    }
    return true;
}

template <typename... Ts>
__constexpr bool
    default_enumeration_t<std::tuple<Ts...>>::decrement(
        value_type_t&                      _a_element,
        enumerate_index_t&                 _a_n_times_to_increment,
        const std::optional<value_type_t>& _a_min_value
    )
{
    using namespace std;
    while (_a_n_times_to_increment > 0)
    {
        if (not decrement_<tuple_size<value_type_t>{} - 1>(
                _a_element, _a_n_times_to_increment
            ))
        {
            return false;
        }
    }
    if (_a_n_times_to_increment == 0)
    {
        return _a_element >= _a_min_value;
    }
    return true;
}

template <typename... Ts>
__constexpr enumeration_diff_t
    default_enumeration_t<std::tuple<Ts...>>::difference(
        const value_type_t& _a_arg1,
        const value_type_t& _a_arg2
    ) noexcept
{
    using namespace std;
    enumeration_diff_t _l_rv{0, 0};
    // Find the lesser of the two arguments.
    const bool _l_arg_1_lesser{less_than(_a_arg1, _a_arg2)};
    auto&      _l_lesser{_l_arg_1_lesser ? _a_arg1 : _a_arg2};
    auto&      _l_greater{_l_arg_1_lesser ? _a_arg2 : _a_arg1};
    // Find out how many advancements per "increment".
    enumeration_diff_t _l_running_total{1, 0};
    difference_<tuple_size<value_type_t>{} - 1>(
        _l_lesser, _l_greater, _l_rv, _l_running_total
    );
    return _l_rv;
}

template <typename... Ts>
template <std::size_t I>
__constexpr bool
    default_enumeration_t<std::tuple<Ts...>>::less_than_(
        const value_type_t& _a_l,
        const value_type_t& _a_r
    ) const noexcept
{
    using namespace std;
    const auto& _l_enum{get<I>(_m_enumeration_schemas)};
    const auto& _l_l_i{get<I>(_a_l)};
    const auto& _l_r_i{get<I>(_a_r)};
    if (_l_enum->less_than(_l_l_i, _l_r_i))
    {
        return true;
    }
    else if (not _l_enum->equal(_l_l_i, _l_r_i))
    {
        return false;
    }
    else if constexpr (I + 1 < tuple_size<value_type_t>{})
    {
        return less_than_<I + 1>(_a_l, _a_r);
    }
    else
    {
        return false;
    }
}

template <typename... Ts>
template <std::size_t I>
__constexpr bool
    default_enumeration_t<std::tuple<Ts...>>::equal_(
        const value_type_t& _a_l,
        const value_type_t& _a_r
    ) const noexcept
{
    using namespace std;
    const auto& _l_enum{get<I>(_m_enumeration_schemas)};
    const auto& _l_l_i{get<I>(_a_l)};
    const auto& _l_r_i{get<I>(_a_r)};
    if (_l_enum->less_than(_l_l_i, _l_r_i))
    {
        return false;
    }
    else if (not _l_enum->equal(_l_l_i, _l_r_i))
    {
        return false;
    }
    else if constexpr (I + 1 < tuple_size<value_type_t>{})
    {
        return equal_<I + 1>(_a_l, _a_r);
    }
    else
    {
        return true;
    }
}

template <typename... Ts>
template <std::size_t I>
__constexpr bool
    default_enumeration_t<std::tuple<Ts...>>::increment_(
        value_type_t&      _a_element,
        enumerate_index_t& _a_n_times_to_increment
    )
{
    using namespace std;
    const auto& _l_enum{get<I>(_m_enumeration_schemas)};
    auto&       _l_elem{get<I>(_a_element)};
    size_t      _l_n_times_to_increment{1};
    if (_l_enum->increment(
            _l_elem, _l_n_times_to_increment, _l_enum->end_value()
        ))
    {
        --_a_n_times_to_increment;
        return true;
    }
    else
    {
        _l_elem = _l_enum->start_value();
        if constexpr (I == 0)
        {
            return false;
        }
        else
        {
            return increment_<I - 1>(_a_element, _a_n_times_to_increment);
        }
    }
}

template <typename... Ts>
template <std::size_t I>
__constexpr bool
    default_enumeration_t<std::tuple<Ts...>>::decrement_(
        value_type_t&      _a_element,
        enumerate_index_t& _a_n_times_to_increment
    )
{
    using namespace std;
    const auto& _l_enum{get<I>(_m_enumeration_schemas)};
    auto&       _l_elem{get<I>(_a_element)};
    size_t      _l_n_times_to_increment{1};
    if (_l_enum->decrement(
            _l_elem, _l_n_times_to_increment, _l_enum->start_value()
        ))
    {
        --_a_n_times_to_increment;
        return true;
    }
    else
    {
        _l_elem = _l_enum->end_value();
        if constexpr (I == 0)
        {
            return false;
        }
        else
        {
            return decrement_<I - 1>(_a_element, _a_n_times_to_increment);
        }
    }
}

template <typename... Ts>
template <std::size_t I>
__constexpr_imp void
    default_enumeration_t<std::tuple<Ts...>>::difference_(
        const value_type_t& _a_lesser,
        const value_type_t& _a_greater,
        enumeration_diff_t& _a_total_diff,
        enumeration_diff_t& _a_running_total
    ) const noexcept
{
    using namespace std;
    using i_type_t = tuple_element_t<I, value_type_t>;
    const i_type_t&                       _l_lesser_elem{get<I>(_a_lesser)};
    const i_type_t&                       _l_greater_elem{get<I>(_a_greater)};
    const enumeration_schema_t<i_type_t>& _l_schema{
        get<I>(_m_enumeration_schemas)
    };
    enumeration_diff_t _l_total_diff = _l_schema->difference(
        _l_schema->start_value(), _l_schema->end_value()
    );
    _l_total_diff.first++;
    if (_l_schema->equal(_l_lesser_elem, _l_greater_elem))
    {
    }
    else if (_l_schema->less_than(_l_lesser_elem, _l_greater_elem))
    {
        enumeration_diff_t _l_local_diff
            = _l_schema->difference(_l_lesser_elem, _l_greater_elem);
        _l_local_diff.first  *= _a_running_total.first;
        _l_local_diff.second *= _a_running_total.second;
        _a_total_diff.first  += _l_local_diff.first;
        _a_total_diff.second += _l_local_diff.second;
    }
    else
    {
        enumeration_diff_t _l_local_diff
            = _l_schema->difference(_l_lesser_elem, _l_greater_elem);
        enumeration_diff_t _l_local_diff_2 = _l_schema->difference(
            _l_schema->start_value(), _l_schema->end_value()
        );
        if (_l_local_diff_2.second > _l_local_diff.second)
        {
            _l_local_diff_2.first--;
            _l_local_diff_2.second
                += _l_schema->n_advancements_per_advancement();
        }
        _l_local_diff_2.second -= _l_local_diff.second;
        _l_local_diff_2.first  -= _l_local_diff.first;
        _a_total_diff.first    += _l_local_diff_2.first;
        _a_total_diff.second   += _l_local_diff_2.second;
    }
    _a_running_total.first  *= _l_total_diff.first;
    _a_running_total.second *= _l_total_diff.second;
    if constexpr (I > 0)
    {
        difference_<I - 1>(
            _a_lesser, _a_greater, _a_total_diff, _a_running_total
        );
    }
}

_END_ABC_DG_NS
