#pragma once
#include "abc_test/core/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_MATCHER_NS

template <logic_enum_t Logic_Enum>
struct simulated_logic_expr_t
{
    __constexpr
    simulated_logic_expr_t()
        = delete;

    simulated_logic_expr_t&
        operator=(
            const matcher_wrapper_t<false>& _a_matcher
        )
    {
        _m_right_arm_of_matcher = _a_matcher;
        return *this;
    }

    __constexpr ~simulated_logic_expr_t()
    {
        _m_attached_matcher
            = mk_matcher_representing_binary_logical_expr<Logic_Enum>(
                _m_attached_matcher, _m_right_arm_of_matcher, u8"<unevaluated>"
            );
    }

    __constexpr explicit simulated_logic_expr_t(
        matcher_wrapper_t<false>&   _a_matcher,
        const std::string           _a_macro_str,
        const std::string           _a_macro_args,
        const std::source_location& _a_source_loc
    )
        : _m_attached_matcher(_a_matcher)
    {
        _m_attached_matcher.add_source_info(
            _a_macro_str, _a_macro_args, _a_source_loc
        );
    }

    __constexpr explicit simulated_logic_expr_t(
        const matcher_wrapper_t<false>& _a_matcher
    );
    __constexpr const std::optional<matcher_wrapper_t<false>>&
                      left_child() const noexcept;
    __constexpr void
        set_left_child(const matcher_wrapper_t<false>& _a_matcher) noexcept;
    template <logic_enum_t Arg_Logic_Enum>
    __constexpr void
        set_left_child(const simulated_logic_expr_t<Arg_Logic_Enum>& _a_matcher
        ) noexcept;
    __constexpr void
        set_right_child(const matcher_wrapper_t<false>& _a_matcher) noexcept;
    template <logic_enum_t Arg_Logic_Enum>
    __constexpr void
        set_right_child(const simulated_logic_expr_t<Arg_Logic_Enum>& _a_matcher
        ) noexcept;
    __constexpr const std::optional<matcher_wrapper_t<false>>&
                      right_child() const noexcept;
    __constexpr explicit
                operator bool();
    __constexpr matcher_wrapper_t<false>
                matcher() const noexcept;
private:
    matcher_wrapper_t<false>&               _m_attached_matcher;
    std::optional<matcher_wrapper_t<false>> _m_right_arm_of_matcher;
    std::optional<matcher_wrapper_t<false>> _m_matcher_l;
    std::optional<matcher_wrapper_t<false>> _m_matcher_r;
};

_END_ABC_MATCHER_NS
_BEGIN_ABC_NS
using simulated_or_expr_t = _ABC_NS_MATCHER::simulated_logic_expr_t<
    _ABC_NS_MATCHER::logic_enum_t::OR>;
using simulated_and_expr_t = _ABC_NS_MATCHER::simulated_logic_expr_t<
    _ABC_NS_MATCHER::logic_enum_t::AND>;
_END_ABC_NS

_BEGIN_ABC_MATCHER_NS
template <logic_enum_t Logic_Enum>
__constexpr_imp
    simulated_logic_expr_t<Logic_Enum>::simulated_logic_expr_t(
        const matcher_wrapper_t<false>& _a_matcher
    )
    : _m_matcher_l(_a_matcher)
    , _m_matcher_r(std::optional<matcher_wrapper_t<false>>{})
{}

template <logic_enum_t Logic_Enum>
__constexpr_imp const std::optional<matcher_wrapper_t<false>>&
    simulated_logic_expr_t<Logic_Enum>::left_child() const noexcept
{
    return _m_matcher_l;
}

template <logic_enum_t Logic_Enum>
__constexpr_imp void
    simulated_logic_expr_t<Logic_Enum>::set_left_child(
        const matcher_wrapper_t<false>& _a_matcher
    ) noexcept
{
    _m_matcher_l = _a_matcher;
}

template <logic_enum_t Logic_Enum>
template <logic_enum_t Arg_Logic_Enum>
__constexpr_imp void
    simulated_logic_expr_t<Logic_Enum>::set_left_child(
        const simulated_logic_expr_t<Arg_Logic_Enum>& _a_matcher
    ) noexcept
{
    set_left_child(_a_matcher.matcher());
}

template <logic_enum_t Logic_Enum>
template <logic_enum_t Arg_Logic_Enum>
__constexpr_imp void
    simulated_logic_expr_t<Logic_Enum>::set_right_child(
        const simulated_logic_expr_t<Arg_Logic_Enum>& _a_matcher
    ) noexcept
{
    set_right_child(_a_matcher.matcher());
}

template <logic_enum_t Logic_Enum>
__constexpr_imp void
    simulated_logic_expr_t<Logic_Enum>::set_right_child(
        const matcher_wrapper_t<false>& _a_matcher
    ) noexcept
{
    _m_matcher_r = _a_matcher;
}

template <logic_enum_t Logic_Enum>
__constexpr_imp const std::optional<matcher_wrapper_t<false>>&
    simulated_logic_expr_t<Logic_Enum>::right_child() const noexcept
{
    return _m_matcher_r;
}

template <logic_enum_t Logic_Enum>
__constexpr_imp simulated_logic_expr_t<Logic_Enum>::operator bool()
{
    // Both unset, return false.
    if (not _m_matcher_l.has_value() && not _m_matcher_r.has_value())
    {
        return false;
    }
    // Both set, return false.
    if (_m_matcher_l.has_value() && _m_matcher_r.has_value())
    {
        return false;
    }
    // Only one element is set.
    const matcher_wrapper_t<false>& _l_matcher_ref{
        _m_matcher_l.has_value() ? _m_matcher_l.value() : _m_matcher_r.value()
    };
    if constexpr (Logic_Enum == logic_enum_t::OR)
    {
        return not _l_matcher_ref.matcher_result().passed();
    }
    else if constexpr (Logic_Enum == logic_enum_t::AND)
    {
        return _l_matcher_ref.matcher_result().passed();
    }
    else
    {
        throw std::runtime_error("Unknown result");
    }
}

template <logic_enum_t Logic_Enum>
__constexpr matcher_wrapper_t<false>
            simulated_logic_expr_t<Logic_Enum>::matcher() const noexcept
{
    return mk_matcher_representing_binary_logical_expr<Logic_Enum>(
        _m_matcher_l, _m_matcher_r, "<unevaluated>"
    );
}

_END_ABC_MATCHER_NS