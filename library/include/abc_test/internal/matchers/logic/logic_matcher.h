#pragma once

#include "abc_test/internal/global.h"
#include "abc_test/internal/matchers/logic/logic_enum.h"
#include "abc_test/internal/reporters/error_reporter_controller.h"

#include <memory>
#include <type_traits>

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
     * @brief Returns the precedence_t of the object's type parameter to the caller.
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
    /*!
     * @brief Adds the object's sources (and its childrens) to the reference argument.
     * @param _a_matcher_source_map The entity to add the object's sources to.
     */
    __constexpr virtual void
        gather_map_source(matcher_source_map_t& _a_matcher_source_map
        ) const noexcept override final;
private:
    std::shared_ptr<matcher_base_t> _m_matcher_l;
    std::shared_ptr<matcher_base_t> _m_matcher_r;
    __constexpr virtual matcher_result_t
        run(test_runner_t& _a_test_runner) override;
    template <bool Result_On_First_Mathcer_To_Move_Into_If>
    __constexpr matcher_result_t
        run_binary_logic_matcher(test_runner_t& _a_test_runner);
};

namespace
{
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
    : _m_matcher_l(_a_matcher)
{}

template <logic_enum_t Logic_Enum>
template <typename>
__constexpr_imp
    logic_matcher_t<Logic_Enum>::logic_matcher_t(
        const std::shared_ptr<matcher_base_t>& _a_matcher_l,
        const std::shared_ptr<matcher_base_t>& _a_matcher_r
    ) noexcept
    : _m_matcher_l(_a_matcher_l), _m_matcher_r(_a_matcher_r)
{}

template <logic_enum_t Logic_Enum>
__constexpr_imp precedence_t
    logic_matcher_t<Logic_Enum>::get_precedence() const noexcept
{
    return logic_precedence<Logic_Enum>();
}

template <logic_enum_t Logic_Enum>
template<typename>
__constexpr_imp void
    logic_matcher_t<Logic_Enum>::set_right_child(
        const std::shared_ptr<matcher_base_t>& _a_matcher
    ) noexcept
{
    if constexpr (Logic_Enum == NOT)
    {
        __STATIC_ASSERT(
            Logic_Enum, "Cannot set the right child of a NOT logic statement"
        );
    }
    else
    {
        _m_matcher_r = _a_matcher;
    }
}

template <logic_enum_t Logic_Enum>
__constexpr_imp matcher_result_t
    logic_matcher_t<Logic_Enum>::run(
        test_runner_t& _a_test_runner
    )
{
    using namespace std;
    using enum logic_enum_t;
    if constexpr (Logic_Enum == NOT)
    {
        string _l_child_str{"<false>"};
        bool   _l_child_passed{false};
        if (_m_matcher_l != nullptr)
        {
            const matcher_result_t& _l_child_res{
                _m_matcher_l->run_test(_a_test_runner)
            };
            _l_child_str    = _l_child_res.str();
            _l_child_passed = not _l_child_res.passed();
        }
        return matcher_result_t(
            true,
            _l_child_passed,
            fmt::format(
                "{0}{1}",
                logic_str<Logic_Enum>(),
                make_str<Logic_Enum>(_m_matcher_l, _l_child_str)
            )
        );
    }
    else if constexpr (Logic_Enum == OR)
    {
        return run_binary_logic_matcher<false>(_a_test_runner);
    }
    else if constexpr (Logic_Enum == AND)
    {
        return run_binary_logic_matcher<true>(_a_test_runner);
    }
    else
    {
        __STATIC_ASSERT(
            Logic_Enum,
            "logic_matcher_t::run function does not work for given template "
            "type."
        );
        return matcher_result_t();
    }
}

template <logic_enum_t Logic_Enum>
__constexpr_imp void
    logic_matcher_t<Logic_Enum>::gather_map_source(
        matcher_source_map_t& _a_matcher_source_map
    ) const noexcept
{
    matcher_base_t::gather_map_source(_a_matcher_source_map);
    if (_m_matcher_l != nullptr)
    {
        _m_matcher_l->gather_map_source(_a_matcher_source_map);
    }
    if (_m_matcher_r != nullptr)
    {
        _m_matcher_r->gather_map_source(_a_matcher_source_map);
    }
}

template <logic_enum_t Logic_Enum>
template <bool Result_On_First_Mathcer_To_Move_Into_If>
__constexpr_imp matcher_result_t
    logic_matcher_t<Logic_Enum>::run_binary_logic_matcher(
        test_runner_t& _a_test_runner
    )
{
    using namespace std;
    string _l_left_str{"<false>"};
    string _l_right_str{"<unevaluated>"};
    bool   _l_left_passed{Result_On_First_Mathcer_To_Move_Into_If};
    bool   _l_right_passed{false};
    if (_m_matcher_l == nullptr)
    {
        matcher_source_map_t _l_msm{};
        this->gather_map_source(_l_msm);
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
        const matcher_result_t& _l_child_res_l{
            _m_matcher_l->run_test(_a_test_runner)
        };
        _l_left_str    = _l_child_res_l.str();
        _l_left_passed = _l_child_res_l.passed();
    }
    if (_l_left_passed == Result_On_First_Mathcer_To_Move_Into_If)
    {
        if (_m_matcher_r == nullptr)
        {
            matcher_source_map_t _l_msm{};
            this->gather_map_source(_l_msm);
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
            _m_matcher_r->run_test(_a_test_runner);
            const matcher_result_t& _l_child_res_r{
                _m_matcher_r->run_test(_a_test_runner)
            };
            _l_right_passed = _l_child_res_r.passed();
            _l_right_str
                = make_str<Logic_Enum>(_m_matcher_r, _l_child_res_r.str());
        }
    }
    return matcher_result_t(
        true,
        compute_logic_result<Logic_Enum>(_l_left_passed, _l_right_passed),
        fmt::format(
            "{0} {1} {2}",
            make_str<Logic_Enum>(_m_matcher_l, _l_left_str),
            logic_str<Logic_Enum>(),
            make_str<Logic_Enum>(_m_matcher_r, _l_right_str)
        )
    );
}

namespace
{
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