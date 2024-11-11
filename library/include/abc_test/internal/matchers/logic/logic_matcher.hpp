#pragma once

#include "abc_test/internal/global.hpp"
#include "abc_test/internal/matchers/logic/logic_enum.hpp"
#include "abc_test/internal/reporters/error_reporter_controller.hpp"

#include <memory>
#include <type_traits>
#include "abc_test/internal/utility/ranges.hpp"

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief This object contains the logic that allows us to compose matcher's
 * using boolean logic operators.
 * @tparam Logic_Enum
 */
template <logic_enum_t Logic_Enum>
class logic_matcher_t : public operator_based_matcher_t
{
public:
    __constexpr
    logic_matcher_t() noexcept
        = delete;
    /*!
     * @brief Constructor.
     *
     * Can only be used with logic_enum_t::NOT.
     *
     * @param _a_arg The shared_ptr to use to setup the object.
     */
    template <
        typename = typename std::enable_if_t<Logic_Enum == logic_enum_t::NOT>>
    __constexpr
    logic_matcher_t(const std::shared_ptr<matcher_base_t>& _a_arg) noexcept;
    /*!
     * @brief Constructor.
     *
     * Can only be used when logic_Enum == OR or AND.
     *
     * @param _a_matcher_l The left child of the binary operator.
     * @param _a_matcher_r The right child of the binary operator.
     */
    template <
        typename = typename std::enable_if_t<
            Logic_Enum == logic_enum_t::AND || Logic_Enum == logic_enum_t::OR>>
    __constexpr
    logic_matcher_t(
        const std::shared_ptr<matcher_base_t>& _a_matcher_l,
        const std::shared_ptr<matcher_base_t>& _a_matcher_r
    ) noexcept;
    /*!
     * @brief Returns the precedence_t of the object's type parameter to the
     * caller.
     * @return precedence_t of the object's type parameter.
     */
    __constexpr virtual precedence_t
        get_precedence() const noexcept override final;
    /*!
     * @brief Sets the right sub-child of the object using the given argument.
     * @param _a_matcher The entity to be made the right-child of this object.
     */
    template <
        typename = typename std::enable_if_t<
            Logic_Enum == logic_enum_t::AND || Logic_Enum == logic_enum_t::OR>>
    __constexpr void
        set_right_child(const std::shared_ptr<matcher_base_t>& _a_matcher
        ) noexcept;
private:
    std::optional<matcher_result_t>  _m_matcher_result_l;
    std::optional<matcher_result_t>  _m_matcher_result_r;
    bool                             _m_left_precedence_less_than_logic_enum;
    bool                             _m_right_precedence_less_than_logic_enum;
    std::size_t _m_boundary_of_sources;
    // std::shared_ptr<matcher_base_t> _m_matcher_l;
    // std::shared_ptr<matcher_base_t> _m_matcher_r;
    // __constexpr virtual matcher_result_t
    //    run(test_runner_t& _a_test_runner) override;
    template <
        typename = typename std::enable_if_t<
            Logic_Enum == logic_enum_t::AND || Logic_Enum == logic_enum_t::OR>>
    __constexpr
    logic_matcher_t(
        const std::optional<matcher_result_t>& _a_matcher_result_l,
        const std::optional<matcher_result_t>& _a_matcher_result_r,
        const bool _a_left_precdence_less_than_logic_enum,
        const bool _a_right_precedence_less_than_logic_enum,
        const std::vector<ds::single_source_t>& _a_sources_l,
        const std::vector<ds::single_source_t>& _a_sources_r
    ) noexcept;
};

namespace
{
template <
    logic_enum_t Logic_Enum,
    typename = typename std::enable_if_t<Logic_Enum == logic_enum_t::NOT>>
__constexpr matcher_result_t
    make_matcher_result(const std::shared_ptr<matcher_base_t>& _a_matcher
    ) noexcept;
template <
    logic_enum_t Logic_Enum,
    bool         Result_On_First_Mathcer_To_Move_Into_If,
    typename = typename std::enable_if_t<
        Logic_Enum == logic_enum_t::AND || Logic_Enum == logic_enum_t::OR>>
__constexpr matcher_result_t
    make_matcher_result(
        const bool _a_left_precdence_less_than_logic_enum,
        const bool _a_right_precedence_less_than_logic_enum,
        const std::optional<matcher_result_t>& _a_matcher_result_l,
        const std::optional<matcher_result_t>& _a_matcher_result_r
    ) noexcept;
template <logic_enum_t Logic_Enum>
__constexpr bool
    is_precdence_less_than_child(
        const std::shared_ptr<matcher_base_t>& _a_matcher
    ) noexcept;
__constexpr matcher_result_t
                                    run();
std::pair<const char*, const char*> _c_normal_bracket_pair{"(", ")"};
std::pair<const char*, const char*> _c_empty_bracket_pair{"", ""};
template <logic_enum_t Logic_Enum>
__constexpr std::string
            make_str(
                std::shared_ptr<matcher_base_t>& _a_ptr,
                const std::string_view           _a_str
            ) noexcept;
} // namespace

_END_ABC_MATCHER_NS

_BEGIN_ABC_MATCHER_NS
template <logic_enum_t Logic_Enum>
template <typename>
__constexpr_imp
    logic_matcher_t<Logic_Enum>::logic_matcher_t(
        const std::shared_ptr<matcher_base_t>& _a_matcher
    ) noexcept
    : operator_based_matcher_t(make_matcher_result<Logic_Enum>(_a_matcher))
{}

template <logic_enum_t Logic_Enum>
template <typename>
__constexpr_imp
    logic_matcher_t<Logic_Enum>::logic_matcher_t(
        const std::shared_ptr<matcher_base_t>& _a_matcher_l,
        const std::shared_ptr<matcher_base_t>& _a_matcher_r
    ) noexcept
    : logic_matcher_t<Logic_Enum>(
          _a_matcher_l == nullptr ? std::optional<matcher_result_t>()
                                  : _a_matcher_l->matcher_result(),
          _a_matcher_r == nullptr ? std::optional<matcher_result_t>()
                                  : _a_matcher_r->matcher_result(),
          is_precdence_less_than_child<Logic_Enum>(_a_matcher_l),
          is_precdence_less_than_child<Logic_Enum>(_a_matcher_r),
          _a_matcher_l == nullptr ? std::vector<ds::single_source_t>{}
                                  : _a_matcher_l->get_sources(),
          _a_matcher_r == nullptr ? std::vector<ds::single_source_t>{}
                                  : _a_matcher_r->get_sources()
      )
{}

template <logic_enum_t Logic_Enum>
__constexpr_imp precedence_t
    logic_matcher_t<Logic_Enum>::get_precedence() const noexcept
{
    return logic_precedence<Logic_Enum>();
}

template <logic_enum_t Logic_Enum>
template <typename>
__constexpr_imp void
    logic_matcher_t<Logic_Enum>::set_right_child(
        const std::shared_ptr<matcher_base_t>& _a_matcher
    ) noexcept
{
    using namespace std;
    if constexpr (Logic_Enum == NOT)
    {
        __STATIC_ASSERT(
            Logic_Enum, "Cannot set the right child of a NOT logic statement"
        );
    }
    else
    {
        _m_matcher_result_r = _a_matcher->matcher_result();
        _m_right_precedence_less_than_logic_enum
            = is_precdence_less_than_child<Logic_Enum>(_a_matcher);
        this->_m_sources.erase(this->_m_sources.begin() + _m_boundary_of_sources,
            this->_m_sources.end());
        this->_m_sources.append_range(_a_matcher->get_sources());
        this->_m_test_result = make_matcher_result < Logic_Enum,
        Logic_Enum
            == logic_enum_t::OR
                   > (_m_left_precedence_less_than_logic_enum,
                      _m_right_precedence_less_than_logic_enum,
                      _m_matcher_result_l,
                      _m_matcher_result_r);
    }
}

template <logic_enum_t Logic_Enum>
template <typename>
__constexpr_imp
    logic_matcher_t<Logic_Enum>::logic_matcher_t(
        const std::optional<matcher_result_t>& _a_matcher_result_l,
        const std::optional<matcher_result_t>& _a_matcher_result_r,
        const bool _a_left_precedence_less_than_logic_enum,
        const bool _a_right_precedence_less_than_logic_enum,
        const std::vector<ds::single_source_t>& _a_sources_l,
        const std::vector<ds::single_source_t>& _a_sources_r
    ) noexcept
    : operator_based_matcher_t(
          make_matcher_result<Logic_Enum, Logic_Enum == logic_enum_t::OR>(
              _a_left_precedence_less_than_logic_enum,
              _a_right_precedence_less_than_logic_enum,
              _a_matcher_result_l,
              _a_matcher_result_r
          ),
        abc::utility::join(_a_sources_l, _a_sources_r)
    )
    , _m_matcher_result_l(_a_matcher_result_l)
    , _m_matcher_result_r(_a_matcher_result_r)
    , _m_left_precedence_less_than_logic_enum(
          _a_left_precedence_less_than_logic_enum
      )
    , _m_right_precedence_less_than_logic_enum(
          _a_right_precedence_less_than_logic_enum
      )
{
}

namespace
{
template <logic_enum_t Logic_Enum, typename>
__constexpr matcher_result_t
    make_matcher_result(
        const std::shared_ptr<matcher_base_t>& _a_matcher
    ) noexcept
{
    using namespace std;
    string _l_child_str{"<false>"};
    bool   _l_child_passed{false};
    bool   _l_precdence_less_than_logic_enum{false};
    if (_a_matcher != nullptr)
    {
        const matcher_result_t& _l_child_res{_a_matcher->matcher_result()};
        _l_child_str    = _l_child_res.str();
        _l_child_passed = not _l_child_res.passed();
        _l_precdence_less_than_logic_enum
            = is_precdence_less_than_child<Logic_Enum>(_a_matcher);
    }
    pair<const char*, const char*>& _l_str_pair{
        _l_precdence_less_than_logic_enum ? _c_normal_bracket_pair
                                          : _c_normal_bracket_pair
    };
    return matcher_result_t(
        _l_child_passed,
        fmt::format(
            "{0}{1}{2}{3}",
            logic_str<Logic_Enum>(),
            _l_str_pair.first,
            _l_child_str,
            _l_str_pair.second
        )
    );
}

template <
    logic_enum_t Logic_Enum,
    bool         Result_On_First_Mathcer_To_Move_Into_If,
    typename>
__constexpr matcher_result_t
    make_matcher_result(
        const bool _a_left_precedence_less_than_logic_enum,
        const bool _a_right_precedence_less_than_logic_enum,
        const std::optional<matcher_result_t>& _a_matcher_result_l,
        const std::optional<matcher_result_t>& _a_matcher_result_r
    ) noexcept
{
    using namespace std;
    string _l_left_str{"<false>"};
    string _l_right_str{"<unevaluated>"};
    bool   _l_left_passed{Result_On_First_Mathcer_To_Move_Into_If};
    bool   _l_right_passed{false};
    if (not _a_matcher_result_l.has_value())
    {
        matcher_source_map_t _l_msm{};
        // this->gather_map_source(_l_msm);
        reporters::error_reporter_controller_t& _l_erc{
            global::get_global_error_reporter_controller()
        };
        _l_erc.report_information(fmt::format(
            "logical_matcher_t expected left-child in binary expression, but "
            "has none. "
            "Degrading expression to {0} {1} right-child. The following source "
            "locations and "
            "source code representation's were associated with this error:{2}",
            _l_left_passed,
            logic_str<Logic_Enum>(),
            _l_msm.gather_list_of_sources_and_representations()
        ));
    }
    else
    {
        const matcher_result_t& _l_child_res_l{_a_matcher_result_l.value()};
        _l_left_str    = _l_child_res_l.str();
        _l_left_passed = _l_child_res_l.passed();
    }
    if (_l_left_passed == Result_On_First_Mathcer_To_Move_Into_If)
    {
        if (not _a_matcher_result_r.has_value())
        {
            matcher_source_map_t _l_msm{};
            // this->gather_map_source(_l_msm);
            reporters::error_reporter_controller_t& _l_erc{
                global::get_global_error_reporter_controller()
            };
            _l_erc.report_information(fmt::format(
                "logical_matcher_t expected right-child in binary expression, "
                "but has none. "
                "Degrading expression to left-child {0} {1}. The following "
                "source locations and "
                "source code representation's were associated with this "
                "error:{2}",
                logic_str<Logic_Enum>(),
                _l_right_passed,
                _l_msm.gather_list_of_sources_and_representations()
            ));
            _l_right_passed = false;
            _l_right_str    = "<false>";
        }
        else
        {
            const matcher_result_t& _l_child_res_r{_a_matcher_result_r.value()};
            _l_right_passed = _l_child_res_r.passed();
            _l_right_str    = _l_child_res_r.str();
        }
    }
    pair<const char*, const char*>& _l_str_pair_l{
        _a_left_precedence_less_than_logic_enum ? _c_normal_bracket_pair
                                                : _c_normal_bracket_pair
    };
    pair<const char*, const char*>& _l_str_pair_r{
        _a_right_precedence_less_than_logic_enum ? _c_normal_bracket_pair
                                                 : _c_normal_bracket_pair
    };
    return matcher_result_t(
        compute_logic_result<Logic_Enum>(_l_left_passed, _l_right_passed),
        fmt::format(
            "{0}{1}{2} {3} {4}{5}{6}",
            _l_str_pair_l.first,
            _l_left_str,
            _l_str_pair_l.second,
            logic_str<Logic_Enum>(),
            _l_str_pair_r.first,
            _l_right_str,
            _l_str_pair_r.second
        )
    );
}

template <logic_enum_t Logic_Enum>
__constexpr bool
    is_precdence_less_than_child(
        const std::shared_ptr<matcher_base_t>& _a_matcher
    ) noexcept
{
    using namespace std;
    if (_a_matcher == nullptr)
    {
        return true;
    }
    operator_based_matcher_t* _l_ptr{
        dynamic_cast<operator_based_matcher_t*>(_a_matcher.get())
    };
    return _l_ptr != nullptr
           && logic_precedence<Logic_Enum>() <= _l_ptr->get_precedence();
}

template <logic_enum_t Logic_Enum>
__constexpr_imp std::string
                make_str(
                    std::shared_ptr<matcher_base_t>& _a_ptr,
                    const std::string_view           _a_str
                ) noexcept
{
    using namespace std;
    operator_based_matcher_t* _l_ptr{
        dynamic_cast<operator_based_matcher_t*>(_a_ptr.get())
    };
    const pair<const char*, const char*>& _l_bracket_pair{
        _l_ptr != nullptr
                && logic_precedence<Logic_Enum>() <= _l_ptr->get_precedence()
            ? _c_normal_bracket_pair
            : _c_empty_bracket_pair
    };
    return fmt::format(
        "{0}{1}{2}", _l_bracket_pair.first, _a_str, _l_bracket_pair.second
    );
}
} // namespace

_END_ABC_MATCHER_NS