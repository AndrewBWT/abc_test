#pragma once
#include "abc_test/included_instances/data_generator/enumeration/enumeration_base.hpp"
#include "abc_test/included_instances/data_generator/enumeration/enumeration_schema_base.hpp"

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
    __constexpr virtual std::pair<std::size_t, std::size_t>
        difference(const T& _a_arg1, const T& _a_arg2) noexcept;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename... Ts>
__constexpr data_gen::enumeration_t<T>
            default_enumeration(Ts... _a_elements) noexcept;
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
}

template <typename T>
__constexpr std::pair<std::size_t,std::size_t>
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
struct default_enumeration_t<T> : public enumeration_base_t<T>
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
        const T _l_divisor{
            static_cast<T>(_l_straight_difference / _m_difference)
        };
        const T _l_remainder{
            static_cast<T>(_l_straight_difference % _m_difference)
        };
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
struct default_enumeration_t<bool> : public enumeration_base_t<bool>
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
    __constexpr std::pair<std::size_t, std::size_t>
        difference(
            const bool& _a_arg1,
            const bool& _a_arg2
        ) noexcept
    {
        return { 0,0 };
    }
};

template <typename T, std::size_t N>
struct default_enumeration_t<std::array<T, N>>
    : public enumeration_base_t<std::array<T, N>>
{
private:
    enumeration_t<T> _m_enumerate;
    std::size_t      _m_n_jumps;
public:
    __constexpr_imp
        default_enumeration_t(
            const std::size_t       _a_n_jumps   = std::size_t{1},
            const enumeration_t<T>& _a_enumerate = all_values<T>()
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
            std::size_t&                           _a_n_times_to_increment,
            const std::optional<std::array<T, N>>& _a_max_value
        );
    __constexpr_imp bool
        decrement(
            std::array<T, N>&                      _a_array,
            std::size_t&                           _a_n_times_to_increment,
            const std::optional<std::array<T, N>>& _a_max_value
        );
    __constexpr virtual std::pair<std::size_t,std::size_t> difference(
        const std::array<T,N>& _a_arg1,
        const std::array<T,N>& _a_arg2
    ) noexcept
    {
        return { 0,0 };
    }
};

template <typename T>
struct default_enumeration_t<std::vector<T>>
    : public enumeration_base_t<std::vector<T>>
{
private:
    enumeration_t<T>        _m_enumerate;
    enumeration_schema_t<T> _m_enumeration_schema;
    std::size_t             _m_n_jumps;
public:
    __constexpr_imp
        default_enumeration_t(
            const std::size_t       _a_n_jumps   = std::size_t{1},
            const enumeration_t<T>& _a_enumerate = default_enumeration<T>(),
            const enumeration_schema_t<T> _a_schema = all_values<T>()
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
            std::vector<T>& _a_vector,
            std::size_t&    _a_n_times_to_increment
        ) const noexcept;
    __constexpr_imp virtual bool
        equal(const std::vector<T>& _a_l, const std::vector<T>& _a_r)
            const noexcept;

    __constexpr_imp bool
        increment(
            std::vector<T>&                      _a_array,
            std::size_t&                         _a_n_times_to_increment,
            const std::optional<std::vector<T>>& _a_max_value
        );
    __constexpr_imp bool
        decrement(
            std::vector<T>&                      _a_array,
            std::size_t&                         _a_n_times_to_increment,
            const std::optional<std::vector<T>>& _a_max_value
        );
    __constexpr virtual std::pair<std::size_t,std::size_t> difference(
        const std::vector<T>& _a_arg1,
        const std::vector<T>& _a_arg2
    ) noexcept
    {
        return { 0,0 };
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

__constexpr_imp bool
    default_enumeration_t<bool>::increment(
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
    default_enumeration_t<bool>::decrement(
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

template <typename T, std::size_t N>
__constexpr_imp
    default_enumeration_t<std::array<T, N>>::default_enumeration_t(
        const std::size_t       _a_n_jumps,
        const enumeration_t<T>& _a_enumerate
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
        std::size_t&                           _a_n_times_to_increment,
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
        std::size_t&                           _a_n_times_to_increment,
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
        const std::size_t       _a_n_jumps,
        const enumeration_t<T>& _a_enumerate,
        const enumeration_schema_t<T> _a_schema
    )
    : _m_enumerate(_a_enumerate), _m_n_jumps(_a_n_jumps), 
    _m_enumeration_schema(_a_schema)
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
            std::vector<T>& _a_vector,
            std::size_t&    _a_n_times_to_increment
        ) const noexcept
{
    bool _l_change_made{false};
    if (_a_vector.size() > 0)
    {
        _a_vector.back() = _m_enumeration_schema->start_value();
        for (size_t _l_idx{_a_vector.size() - 1}; _l_idx > 0; --_l_idx)
        {
            std::size_t _l_arg{1};
            if (_m_enumerate->increment(_a_vector[_l_idx - 1], _l_arg,std::optional<T>()))
            {
                --_a_n_times_to_increment;
                _l_change_made = true;
                break;
            }
            else
            {
                _a_vector[_l_idx - 1] = _m_enumeration_schema->start_value();
            }
        }
    }
    if (_l_change_made == false)
    {
        --_a_n_times_to_increment;
        _a_vector.resize(_a_vector.size() + 1);
        for (size_t _l_idx{0}; _l_idx < _a_vector.size(); ++_l_idx)
        {
            _a_vector[_l_idx] = _m_enumeration_schema->start_value();
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
        std::size_t&                         _a_n_times_to_increment,
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
                    _l_elem, _a_n_times_to_increment, std::optional<T>()
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
        std::size_t&                         _a_n_times_to_increment,
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
                _l_elem = _m_enumeration_schema->end_value(_m_enumerate);
                for (size_t _l_idx{_a_array.size() - 1}; _l_idx > 0; --_l_idx)
                {
                    std::size_t _l_arg{1};
                    if (_m_enumerate->decrement(
                            _a_array[_l_idx - 1], _l_arg, std::optional<T>()
                        ))
                    {
                        --_a_n_times_to_increment;
                        break;
                    }
                    else
                    {
                        _a_array[_l_idx - 1]
                            = _m_enumeration_schema->end_value(_m_enumerate);
                        if (_l_idx == 1)
                        {
                            // time to reset!
                            --_a_n_times_to_increment;
                            _a_array.resize(_a_array.size() - 1);
                            for (size_t _l_idx{0}; _l_idx < _a_array.size();
                                 ++_l_idx)
                            {
                                _a_array[_l_idx]
                                    = _m_enumeration_schema->end_value(
                                        _m_enumerate
                                    );
                            }
                        }
                    }
                }
            }
        }
        else
        {
            _l_elem = _m_enumeration_schema->end_value(_m_enumerate);
            for (size_t _l_idx{_a_array.size() - 1}; _l_idx > 0; --_l_idx)
            {
                std::size_t _l_arg{1};
                if (_m_enumerate
                    ->decrement(_a_array[_l_idx - 1], _l_arg, std::optional<T>{}))
                {
                    --_a_n_times_to_increment;
                    break;
                }
                else
                {
                    _a_array[_l_idx - 1]
                        = _m_enumeration_schema->end_value(_m_enumerate);
                    if (_l_idx == 1)
                    {
                        // time to reset!
                        --_a_n_times_to_increment;
                        _a_array.resize(_a_array.size() - 1);
                        for (size_t _l_idx{0}; _l_idx < _a_array.size();
                             ++_l_idx)
                        {
                            _a_array[_l_idx]
                                = _m_enumeration_schema->end_value(_m_enumerate
                                );
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

_END_ABC_DG_NS

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