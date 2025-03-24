#pragma once
#include "abc_test/included_instances/data_generator/enumeration/enumeration_base.hpp"

#include <map>
#include <set>

_BEGIN_ABC_DG_NS

template <typename T>
struct enumeration_from_list : public enumeration_base_t<T>
{
private:
    std::vector<T>           _m_elements;
    std::map<T, std::size_t> _m_indexes;
    std::map<std::size_t, T> _m_values;
    T                        _m_min_value;
    T                        _m_max_value;
public:
    __constexpr
    enumeration_from_list()
        = delete;
    template <typename R>
    __constexpr
    enumeration_from_list(R&& _a_elements) noexcept;
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
template <typename T, typename R = std::initializer_list<T>>
__constexpr data_gen::enumeration_t<T>
            mk_enumeration_list(R&& _a_elements) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
template <typename R>
__constexpr
enumeration_from_list<T>::enumeration_from_list(
    R&& _a_elements
) noexcept
{
    using namespace std;
    set<T> _l_added_elements;
    for (const T& _l_element : _a_elements)
    {
        if (not _l_added_elements.contains(_l_element))
        {
            _l_added_elements.insert(_l_element);
        }
    }
    _m_elements = ranges::to<vector<T>>(_l_added_elements);
    for (const T& _l_element : _m_elements)
    {
        _m_indexes.insert({_l_element, _m_indexes.size()});
        _m_values.insert({_m_values.size(), _l_element});
    }
    _m_min_value = _m_values[0];
    _m_max_value = _m_values[_m_values.size() - 1];
}

template <typename T>
__constexpr_imp bool
    enumeration_from_list<T>::less_than(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    return _m_indexes.at(_a_l) < _m_indexes.at(_a_r);
}

template <typename T>
__constexpr_imp bool
    enumeration_from_list<T>::equal(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    return _m_indexes.at(_a_l) == _m_indexes.at(_a_r);
}

template <typename T>
__constexpr bool
    enumeration_from_list<T>::increment(
        T&                      _a_element,
        enumerate_index_t&      _a_n_times_to_increment,
        const std::optional<T>& _a_max_value
    )
{
    T      _l_max_value{};
    size_t _l_current_index{_m_indexes[_a_element]};
    if (_a_max_value.has_value())
    {
        if (_m_indexes.contains(_a_max_value.value()))
        {
            _l_max_value = _a_max_value.value();
        }
        else
        {
            _l_max_value = _m_max_value;
        }
    }
    else
    {
        _l_max_value = _m_max_value;
    }
    size_t _l_last_value_index{_m_indexes[_l_max_value]};
    size_t _l_difference{_l_last_value_index - _l_current_index};
    if (_a_n_times_to_increment > _l_difference)
    {
        _a_n_times_to_increment -= _l_difference;
        _a_element               = _l_max_value;
        return false;
    }
    else
    {
        _a_n_times_to_increment = 0;
        _a_element              = _m_values[_l_current_index + _l_difference];
        return true;
    }
}

template <typename T>
__constexpr bool
    enumeration_from_list<T>::decrement(
        T&                      _a_element,
        enumerate_index_t&      _a_n_times_to_increment,
        const std::optional<T>& _a_min_value
    )
{
    T      _l_min_value{};
    size_t _l_current_index{_m_indexes[_a_element]};
    if (_a_min_value.has_value())
    {
        if (_m_indexes.contains(_a_min_value.value()))
        {
            _l_min_value = _a_min_value.value();
        }
        else
        {
            _l_min_value = _m_min_value;
        }
    }
    else
    {
        _l_min_value = _m_min_value;
    }
    size_t _l_first_value_index{_m_indexes[_l_min_value]};
    size_t _l_difference{_l_current_index - _l_first_value_index};
    if (_a_n_times_to_increment > _l_difference)
    {
        _a_n_times_to_increment -= _l_difference;
        _a_element               = _l_min_value;
        return false;
    }
    else
    {
        _a_n_times_to_increment = 0;
        _a_element              = _m_values[_l_current_index - _l_difference];
        return true;
    }
}

template <typename T>
inline __constexpr std::pair<std::size_t, std::size_t>
                   enumeration_from_list<T>::difference(
        const T& _a_arg1,
        const T& _a_arg2
    ) noexcept
{
    using namespace std;
    const size_t _l_arg1_idx{_m_indexes[_a_arg1]};
    const size_t _l_arg2_idx{_m_indexes[_a_arg2]};
    return {
        (_l_arg1_idx > _l_arg2_idx) ? (_l_arg1_idx - _l_arg2_idx)
                                    : (_l_arg2_idx - _l_arg1_idx),
        0
    };
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename R>
__constexpr data_gen::enumeration_t<T>
            mk_enumeration_list(
                R&& _a_elements
            ) noexcept
{
    using namespace data_gen;
    return make_shared<enumeration_from_list<T>>(_a_elements);
}

_END_ABC_NS