#pragma once
#include "abc_test/included_instances/data_generator/enumeration/enumeration_base.hpp"

_BEGIN_ABC_DG_NS

template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
requires std::is_invocable_r_v<bool, Compare_Func, const T&, const T&>
         && std::is_invocable_r_v<
             bool,
             Increment_Or_Decrement_Func,
             T&,
             enumerate_index_t,
             std::optional<T>&>
         && std::
             is_invocable_r_v<std::size_t, Difference_Func, const T&, const T&>
struct enumeration_from_functions_t : public enumeration_base_t<T>
{
private:
    Compare_Func                _m_callable_less_than;
    Compare_Func                _m_callable_equal;
    Increment_Or_Decrement_Func _m_callable_increment;
    Increment_Or_Decrement_Func _m_callable_decrement;
    Difference_Func             _m_callable_difference;
public:
    __constexpr
    enumeration_data_object_t()
        = delete;
    __constexpr
    enumeration_data_object_t(
        Compare_Func                _a_equal_func,
        Compare_Func                _a_less_than_func,
        Increment_Or_Decrement_Func _a_increment_func,
        Increment_Or_Decrement_Func _a_decrement_func,
        Difference_Func             _a_difference_func
    ) noexcept;
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

    __constexpr virtual std::size_t
        difference(const T& _a_arg1, const T& _a_arg2) noexcept;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
requires std::is_invocable_r_v<bool, Compare_Func, const T&, const T&>
         && std::is_invocable_r_v<
             bool,
             Increment_Or_Decrement_Func,
             T&,
             data_gen::enumerate_index_t,
             std::optional<T>&>
         && std::
             is_invocable_r_v<std::size_t, Difference_Func, const T&, const T&>
__constexpr data_gen::enumeration_t<T>
            mk_enumeration_function(
                Compare_Func                _a_equal_func,
                Compare_Func                _a_less_than_func,
                Increment_Or_Decrement_Func _a_increment_func,
                Increment_Or_Decrement_Func _a_decrement_func,
                Difference_Func             _a_difference_func
            ) noexcept;
_END_ABC_NS

_BEGIN_ABC_DG_NS
template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
requires std::is_invocable_r_v<bool, Compare_Func, const T&, const T&>
         && std::is_invocable_r_v<
             bool,
             Increment_Or_Decrement_Func,
             T&,
             data_gen::enumerate_index_t,
             std::optional<T>&>
         && std::
             is_invocable_r_v<std::size_t, Difference_Func, const T&, const T&>
__constexpr
enumeration_from_functions_t<
    T,
    Compare_Func,
    Increment_Or_Decrement_Func,
    Difference_Func>::
    enumeration_data_object_t(
        Compare_Func                _a_equal_func,
        Compare_Func                _a_less_than_func,
        Increment_Or_Decrement_Func _a_increment_func,
        Increment_Or_Decrement_Func _a_decrement_func,
        Difference_Func             _a_difference_func
    ) noexcept
             : _m_equal_func(_a_equal_func)
             , _m_less_than_func(_a_less_than_func)
             , _m_increment_func(_a_increment_func)
             , _m_decrement_func(_a_decrment_func)
             , _m_difference_func(_a_difference_func)
{}

template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
inline __constexpr_imp bool
    enumeration_from_functions_t<
        T,
        Compare_Func,
        Increment_Or_Decrement_Func,
        Difference_Func>::
        less_than(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
{
    return std::invoke_r(_m_callable_less_than, _a_l, _a_r);
}

template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
inline __constexpr_imp bool
    enumeration_from_functions_t<
        T,
        Compare_Func,
        Increment_Or_Decrement_Func,
        Difference_Func>::
        equal(
            const T& _a_l,
            const T& _a_r
        ) const noexcept
{
    return return std::invoke_r(_m_callable_equal, _a_l, _a_r);
}

template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
requires std::is_invocable_r_v<bool, Compare_Func, const T&, const T&>
         && std::is_invocable_r_v<
             bool,
             Increment_Or_Decrement_Func,
             T&,
             enumerate_index_t,
             std::optional<T>&>
         && std::
             is_invocable_r_v<std::size_t, Difference_Func, const T&, const T&>
__constexpr_imp bool
    enumeration_from_functions_t<
        T,
        Compare_Func,
        Increment_Or_Decrement_Func,
        Difference_Func>::
        increment(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        )
{
    return std::invoke_r(
        _m_callable_increment, _a_element, _a_n_times_to_increment, _a_max_value
    );
}

template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
requires std::is_invocable_r_v<bool, Compare_Func, const T&, const T&>
         && std::is_invocable_r_v<
             bool,
             Increment_Or_Decrement_Func,
             T&,
             enumerate_index_t,
             std::optional<T>&>
         && std::
             is_invocable_r_v<std::size_t, Difference_Func, const T&, const T&>
__constexpr_imp bool
    enumeration_from_functions_t<
        T,
        Compare_Func,
        Increment_Or_Decrement_Func,
        Difference_Func>::
        decrement(
            T&                      _a_element,
            enumerate_index_t&      _a_n_times_to_increment,
            const std::optional<T>& _a_max_value
        )
{
    return std::invoke_r(
        _m_callable_decrement, _a_element, _a_n_times_to_increment, _a_max_value
    );
}

template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
inline __constexpr std::size_t
                   enumeration_from_functions_t<
                       T,
                       Compare_Func,
                       Increment_Or_Decrement_Func,
                       Difference_Func>::
        difference(
            const T& _a_arg1,
            const T& _a_arg2
        ) noexcept
{
    return std::invoke_r(_m_callable_difference, _a_arg1, _a_arg2);
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <
    typename T,
    typename Compare_Func,
    typename Increment_Or_Decrement_Func,
    typename Difference_Func>
requires std::is_invocable_r_v<bool, Compare_Func, const T&, const T&>
         && std::is_invocable_r_v<
             bool,
             Increment_Or_Decrement_Func,
             T&,
             data_gen::enumerate_index_t,
             std::optional<T>&>
         && std::
             is_invocable_r_v<std::size_t, Difference_Func, const T&, const T&>
__constexpr data_gen::enumeration_t<T>
            mk_enumeration_function(
                Compare_Func                _a_equal_func,
                Compare_Func                _a_less_than_func,
                Increment_Or_Decrement_Func _a_increment_func,
                Increment_Or_Decrement_Func _a_decrement_func,
                Difference_Func             _a_difference_func
            ) noexcept
{
    using namespace data_gen;
    return make_shared < enumeration_from_functions_t<
               T,
               Compare_Func,
               Increment_Or_Decrement_Func,
               Difference_Func>(
               _a_equal_func,
               _a_less_than_func,
               _a_increment_func,
               _a_decrement_func,
               _a_difference_func
           );
}

_END_ABC_NS