#pragma once

#include "abc_test/included_instances/data_generator/enumeration_data_object.h"
#include "abc_test/internal/errors/test_library_exception.h"

#include <limits>

_BEGIN_ABC_DG_NS
template <typename T>
concept has_limits_c = (not std::is_enum_v<T>) && requires () {
    { std::numeric_limits<T>::is_specialized == true };
};

template <typename T>
requires has_limits_c<T>
struct min_value_t<T>
{
    __constexpr T
        min_value() const noexcept
    {
        return std::numeric_limits<T>::min();
    }
};

template <typename T>
requires has_limits_c<T>
struct max_value_t<T>
{
    __constexpr T
        max_value() const noexcept
    {
        return std::numeric_limits<T>::max();
    }
};

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
requires has_addition_c<T> && has_subtraction_c<T> && has_less_than_c<T>
         && has_equal_c<T> && (not std::is_enum_v<T>)
struct enumeration_data_object_t<T>
{
    __constexpr virtual std::pair<std::size_t, std::size_t>
        difference(
            const T& _a_arg1,
            const T& _a_arg2
        ) noexcept
    {
        const T _l_straight_difference{
            less_than(_a_arg1, _a_arg2) ? static_cast<T>(_a_arg2 - _a_arg1)
                                        : static_cast<T>(_a_arg1 - _a_arg2)
        };
        const T _l_divisor{static_cast<T>(_l_straight_difference / _m_difference)};
        const T _l_remainder{static_cast<T>(_l_straight_difference % _m_difference)};
        return {
            static_cast<std::size_t>(_l_divisor),
            static_cast<std::size_t>(_l_remainder)
        };
    }

    __constexpr
    enumeration_data_object_t(
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
            std::size_t&            _a_times_called,
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
            std::size_t&            _a_times_called,
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

template <>
struct enumeration_data_object_t<bool>
{
    __constexpr_imp virtual bool
        less_than(const bool& _a_l, const bool& _a_r) const noexcept;
    __constexpr_imp virtual bool
        equal(const bool& _a_l, const bool& _a_r) const noexcept;
    __constexpr bool
        increment(
            bool&                      _a_element,
            std::size_t&               _a_n_times_to_increment,
            const std::optional<bool>& _a_max_value
        );
    __constexpr bool
        decrement(
            bool&                      _a_element,
            std::size_t&               _a_n_times_to_increment,
            const std::optional<bool>& _a_max_value
        );
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
    enumeration_data_object_t<bool>::less_than(
        const bool& _a_l,
        const bool& _a_r
    ) const noexcept
{
    return _a_l < _a_r;
}

__constexpr_imp bool
    enumeration_data_object_t<bool>::equal(
        const bool& _a_l,
        const bool& _a_r
    ) const noexcept
{
    return _a_l == _a_r;
}

__constexpr_imp bool
    enumeration_data_object_t<bool>::increment(
        bool&                      _a_element,
        std::size_t&               _a_n_times_to_increment,
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

__constexpr_imp bool
    enumeration_data_object_t<bool>::decrement(
        bool&                      _a_element,
        std::size_t&               _a_n_times_to_increment,
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

_END_ABC_DG_NS