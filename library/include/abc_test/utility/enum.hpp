#pragma once
#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/parsers/parser_input.hpp"
#include "abc_test/utility/types.hpp"

#include <algorithm>
#include <concepts>
#include <map>
#include <optional>
#include <vector>


_BEGIN_ABC_UTILITY_NS
template <typename T>
using enum_list_t = std::vector<std::pair<T, std::string_view>>;
template <typename T>
concept enum_has_list_c = std::is_enum_v<T> && requires () {
    { get_enum_list<T>() } -> std::same_as<enum_list_t<T>>;
};
template <typename T>
__constexpr enum_list_t<T>
            get_enum_list() noexcept = delete;
enum class enum_helper_string_case_t
{
    lower,
    upper,
    unchanged
};

template <typename T>
requires enum_has_list_c<T>
struct enumerate_enum_helper_t
{
    __constexpr
    enumerate_enum_helper_t(enum_list_t<T>&& _a_enum_list);
    __constexpr_imp bool
        less_than(const T& _a_l, const T& _a_r) const noexcept;

    __constexpr_imp bool
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
    {
        return _m_elements_to_idx.at(_a_l) == _m_elements_to_idx.at(_a_r);
    }

    __constexpr T
        min() const noexcept;
    __constexpr T
        max() const noexcept;
    __constexpr bool
        increment(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        ) const noexcept;
    ;
    __constexpr bool
        decrement(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        ) const noexcept;
    __constexpr std::size_t
                difference(const T& _a_arg1, const T& _a_arg2) const noexcept;

    __constexpr std::string
                print(
                    const T&                        _a_element,
                    const enum_helper_string_case_t _a_enum_string_case
                    = enum_helper_string_case_t::unchanged
                ) const
    {
        using enum enum_helper_string_case_t;
        switch (_a_enum_string_case)
        {
        case lower:
            return _m_elements_to_lower_string.at(_a_element);
        case upper:
            return _m_elements_to_upper_string.at(_a_element);
        case unchanged:
            return _m_elements_to_string.at(_a_element);
        default:
            throw errors::unaccounted_for_enum_exception(_a_enum_string_case);
        }
    }

    __constexpr result_t<T>
                parse_enum(
                    parser::parser_input_t&         _a_parse_input,
                    const enum_helper_string_case_t _a_enum_string_case
                    = enum_helper_string_case_t::unchanged
                ) const
    {
        using namespace std;
        using enum enum_helper_string_case_t;
        std::reference_wrapper<const std::map<std::string, T>>
            _l_string_to_elements_ref(_m_lower_string_to_elements);
        switch (_a_enum_string_case)
        {
        case lower:
            _l_string_to_elements_ref
                = reference_wrapper(_m_lower_string_to_elements);
            break;
        case upper:
            _l_string_to_elements_ref
                = reference_wrapper(_m_upper_string_to_elements);
            break;
        case unchanged:
            _l_string_to_elements_ref
                = reference_wrapper(_m_string_to_elements);
            break;
        default:
            throw errors::unaccounted_for_enum_exception(_a_enum_string_case);
        }
        for (const auto& [_l_sv, _l_enum] : _l_string_to_elements_ref.get())
        {
            if (_a_parse_input.check_and_advance(_l_sv))
            {
                return _l_enum;
            }
        }
        return unexpected(fmt::format(
            "Could not parse string {0} to enum {1}.",
            _a_parse_input.sv(),
            typeid(T).name()
        ));
    }
private:
    std::map<std::size_t, T> _m_idx_to_elements;
    std::map<T, std::size_t> _m_elements_to_idx;
    std::map<std::string, T> _m_string_to_elements;
    std::map<std::string, T> _m_lower_string_to_elements;
    std::map<std::string, T> _m_upper_string_to_elements;
    std::map<T, std::string> _m_elements_to_string;
    std::map<T, std::string> _m_elements_to_lower_string;
    std::map<T, std::string> _m_elements_to_upper_string;
};

template <typename T>
requires enum_has_list_c<T>
__constexpr const enumerate_enum_helper_t<T>&
                  get_thread_local_enumerate_enum_helper() noexcept;
_END_ABC_UTILITY_NS

#define _ENUM_LIST_ENTRY(x) {x, #x}

_BEGIN_ABC_UTILITY_NS
template <typename T>
requires enum_has_list_c<T>
__constexpr_imp
    enumerate_enum_helper_t<T>::enumerate_enum_helper_t(
        enum_list_t<T>&& _a_enum_list
    )
{
    using namespace _ABC_NS_ERRORS;
    using namespace std;
    for (const auto& [_l_element, _l_str] : _a_enum_list)
    {
        _m_idx_to_elements.insert({_m_idx_to_elements.size(), _l_element});
        _m_elements_to_idx.insert({_l_element, _m_elements_to_idx.size()});
        string _l_cpy{ _l_str };
        _m_string_to_elements.insert({ _l_cpy, _l_element});
        _m_elements_to_string.insert({_l_element, _l_cpy });
        transform(
            _l_cpy.begin(),
            _l_cpy.end(),
            _l_cpy.begin(),
            [](unsigned char c)
            {
                return tolower(c);
            }
        );
        _m_lower_string_to_elements.insert({_l_cpy, _l_element});
        _m_elements_to_lower_string.insert({_l_element, _l_cpy});
        transform(
            _l_cpy.begin(),
            _l_cpy.end(),
            _l_cpy.begin(),
            [](unsigned char c)
            {
                return toupper(c);
            }
        );
        _m_upper_string_to_elements.insert({_l_cpy, _l_element});
        _m_elements_to_upper_string.insert({_l_element, _l_cpy});
    }
    if (_a_enum_list.size() == 0)
    {
        throw test_library_exception_t(fmt::format(
            "Could not create enumerate_enum_helper_t using enum_list of size 0"
        ));
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::less_than(
        const T& _a_l,
        const T& _a_r
    ) const noexcept
{
    return _m_elements_to_idx.at(_a_l) < _m_elements_to_idx.at(_a_r);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp T
    enumerate_enum_helper_t<T>::min() const noexcept
{
    return _m_idx_to_elements.at(0);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp T
    enumerate_enum_helper_t<T>::max() const noexcept
{
    return _m_idx_to_elements.at(_m_idx_to_elements.size() - 1);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::increment(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_max_value
    ) const noexcept
{
    using namespace std;
    const size_t _l_current_idx{_m_elements_to_idx.at(_a_element)};
    const size_t _l_max_times_called{
        _m_elements_to_idx.size() - _l_current_idx - 1
    };
    if (_a_times_called > _l_max_times_called)
    {
        if (_l_max_times_called == 0)
        {
            return false;
        }
        else
        {
            _a_element = _m_idx_to_elements.at(_m_elements_to_idx.size() - 1);
            _a_times_called -= _l_max_times_called;
            return true;
        }
    }
    else
    {
        _a_element = _m_idx_to_elements.at(_l_current_idx + _a_times_called);
        _a_times_called = 0;
        return true;
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::decrement(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_max_value
    ) const noexcept
{
    using namespace std;
    const size_t _l_current_idx{_m_elements_to_idx.at(_a_element)};
    const size_t _l_max_times_called{_l_current_idx};
    if (_a_times_called > _l_max_times_called)
    {
        if (_l_max_times_called == 0)
        {
            return false;
        }
        else
        {
            _a_element       = _m_idx_to_elements.at(0);
            _a_times_called -= _l_max_times_called;
            return true;
        }
    }
    else
    {
        _a_element = _m_idx_to_elements.at(_l_current_idx - _a_times_called);
        _a_times_called = 0;
        return true;
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp std::size_t
                enumerate_enum_helper_t<T>::difference(
        const T& _a_arg1,
        const T& _a_arg2
    ) const noexcept
{
    const size_t _l_arg1_idx{_m_elements_to_idx.at(_a_arg1)};
    const size_t _l_arg2_idx{_m_elements_to_idx.at(_a_arg2)};
    return (_l_arg1_idx < _l_arg2_idx) ? (_l_arg2_idx - _l_arg1_idx)
                                       : (_l_arg1_idx - _l_arg2_idx);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp const enumerate_enum_helper_t<T>&
                      get_thread_local_enumerate_enum_helper() noexcept
{
    thread_local enumerate_enum_helper_t<T> _static_enumerate_enum_helper(
        get_enum_list<T>()
    );
    return _static_enumerate_enum_helper;
}

_END_ABC_UTILITY_NS
