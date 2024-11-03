#pragma once

#include "abc_test/included_instances/data_generator/enumeration_data_object.h"
#include "abc_test/included_instances/data_generator/enumeration_data_object/max_value_concept.h"
#include "abc_test/included_instances/data_generator/enumeration_data_object/min_value_concept.h"

#include <array>

_BEGIN_ABC_DG_NS

template <typename T>
requires min_value_c<T>
struct min_value_t<std::vector<T>>
{
    __constexpr_imp std::vector<T>
                    min_value() const noexcept;
};

template <typename T>
struct enumeration_data_object_t<std::vector<T>>
{
private:
    enumeration_data_object_t<T> _m_enumerate;
    std::size_t                  _m_n_jumps;
public:
    __constexpr_imp
        enumeration_data_object_t(
            const std::size_t            _a_n_jumps   = std::size_t{75},
            enumeration_data_object_t<T> _a_enumerate = all_values<T>()
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
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS

template <typename T>
    requires min_value_c<T>
__constexpr_imp std::vector<T>
                min_value_t<std::vector<T>>::min_value() const noexcept
{
    using namespace std;
    vector<T> _l_rv;
    return _l_rv;
}

template <typename T>
__constexpr_imp
    enumeration_data_object_t<std::vector<T>>::enumeration_data_object_t(
        const std::size_t            _a_n_jumps,
        enumeration_data_object_t<T> _a_enumerate
    )
    : _m_enumerate(_a_enumerate), _m_n_jumps(_a_n_jumps)
{}

template <typename T>
__constexpr_imp bool
    enumeration_data_object_t<std::vector<T>>::less_than(
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
            if (_m_enumerate.less_than(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return true;
            }
            else if (not _m_enumerate.equal(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return false;
            }
        }
        return false;
    }
}

template <typename T>
__constexpr_imp bool
    enumeration_data_object_t<std::vector<T>>::is_return_value_within_range(
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
    enumeration_data_object_t<std::vector<T>>::
        either_increment_vector_or_create_next_largest_vector(
            std::vector<T>& _a_vector,
            std::size_t&    _a_n_times_to_increment
        ) const noexcept
{
    bool _l_change_made{false};
    if (_a_vector.size() > 0)
    {
        _a_vector.back() = _m_enumerate.min();
        for (size_t _l_idx{_a_vector.size() - 1}; _l_idx > 0; --_l_idx)
        {
            std::size_t _l_arg{1};
            if (_m_enumerate.next_element(_a_vector[_l_idx - 1], _l_arg))
            {
                --_a_n_times_to_increment;
                _l_change_made = true;
                break;
            }
            else
            {
                _a_vector[_l_idx - 1] = _m_enumerate.min();
            }
        }
    }
    if (_l_change_made == false)
    {
        --_a_n_times_to_increment;
        _a_vector.resize(_a_vector.size() + 1);
        for (size_t _l_idx{0}; _l_idx < _a_vector.size(); ++_l_idx)
        {
            _a_vector[_l_idx] = _m_enumerate.min();
        }
    }
}

template <typename T>
__constexpr_imp bool
    enumeration_data_object_t<std::vector<T>>::equal(
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
            if (_m_enumerate.less_than(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return false;
            }
            else if (not _m_enumerate.equal(_a_l[_l_idx], _a_r[_l_idx]))
            {
                return false;
            }
        }
        return true;
    }
}

template <typename T>
__constexpr_imp bool
    enumeration_data_object_t<std::vector<T>>::increment(
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
            if (_m_enumerate.next_element(_l_elem, _a_n_times_to_increment))
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
    enumeration_data_object_t<std::vector<T>>::decrement(
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
        if (_m_enumerate.move_backward(_l_elem, _a_n_times_to_increment))
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
                _l_elem = _m_enumerate.max();
                for (size_t _l_idx{_a_array.size() - 1}; _l_idx > 0; --_l_idx)
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
                            // time to reset!
                            --_a_n_times_to_increment;
                            _a_array.resize(_a_array.size() - 1);
                            for (size_t _l_idx{0}; _l_idx < _a_array.size();
                                 ++_l_idx)
                            {
                                _a_array[_l_idx] = _m_enumerate.max();
                            }
                        }
                    }
                }
            }
        }
        else
        {
            _l_elem = _m_enumerate.max();
            for (size_t _l_idx{_a_array.size() - 1}; _l_idx > 0; --_l_idx)
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
                        // time to reset!
                        --_a_n_times_to_increment;
                        _a_array.resize(_a_array.size() - 1);
                        for (size_t _l_idx{0}; _l_idx < _a_array.size();
                             ++_l_idx)
                        {
                            _a_array[_l_idx] = _m_enumerate.max();
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