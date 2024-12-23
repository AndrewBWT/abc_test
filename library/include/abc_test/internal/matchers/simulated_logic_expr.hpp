#pragma once
#include "abc_test/internal/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_MATCHER_NS

template <logic_enum_t Logic_Enum>
struct simulated_logic_expr_t
{
    __constexpr
    simulated_logic_expr_t(const matcher_wrapper_t<false>& _a_matcher);
    __constexpr const std::optional<matcher_wrapper_t<false>>&
                      left_child() const noexcept;
    __constexpr void
        set_left_child(const matcher_wrapper_t<false>& _a_matcher) noexcept;

    __constexpr void
        set_right_child(const matcher_wrapper_t<false>& _a_matcher) noexcept;
    __constexpr const std::optional<matcher_wrapper_t<false>>&
                      right_child() const noexcept;
    __constexpr explicit
        operator bool();
private:
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
    if (not _m_matcher_l.has_value() && not _m_matcher_r.has_value())
    {
        return false;
    }
    // Atleast one is set. Prefer the left.
    const matcher_wrapper_t<false>& _l_matcher_ref{
        _m_matcher_l.has_value() ? _m_matcher_l.value() : _m_matcher_r.value()
    };
    if constexpr (Logic_Enum == logic_enum_t::OR)
    {
        return not _l_matcher_ref.matcher_result().passed();
    }
    else
    {
        return _l_matcher_ref.matcher_result().passed();
    }
}

_END_ABC_MATCHER_NS