#pragma once
#include "abc_test/internal/errors/test_library_exception.hpp"
#include "abc_test/internal/global.hpp"
#include "abc_test/internal/matchers/logic/logic_matcher.hpp"
#include "abc_test/internal/matchers/static_matcher.hpp"

_BEGIN_ABC_MATCHER_NS
/*!
 * @brief Type synonym for a pointer to a matcher_base_t.
 */
using matcher_base_ptr_t = std::shared_ptr<matcher_base_t>;

struct simulated_binary_logic_expr_t
{
    logic_enum_t     _m_logic_type_of_matcher;
    matcher_result_t _m_matcher_result_l;
    bool             _m_left_precedence_less_than_logic_enum;
};

/*!
 * @brief One of the core matcher classes; this class is used to contain a
 * matcher, without having to deal with the pointers.
 * @tparam Has_Annotation Whether the matcher_wrapper_t is allowed an annotation
 * with it.
 */
template <bool Has_Annotation>
struct matcher_wrapper_t
{
public:
    __constexpr
    matcher_wrapper_t(
        const matcher_wrapper_t<false>&                _a_matcher_l,
        const std::optional<matcher_wrapper_t<false>>& _a_matcher_r
    )
    {}

    __constexpr_imp const std::vector<ds::single_source_t>&
                          get_sources() const noexcept
    {
        return _m_sources;
    }

    __constexpr const std::optional<precedence_t>&
                      precedence() const noexcept
    {
        return _m_precedence;
    }

    __constexpr const matcher_result_t&
        matcher_result() const noexcept
    {
        return _m_test_result;
    }

    __constexpr_imp void
        remove_primary_source() noexcept
    {
        using namespace std;
        using namespace ds;
        _m_primary_source = optional<single_source_t>{};
    }

    __constexpr const std::optional<ds::single_source_t>&
                      primary_source() const noexcept
    {
        return _m_primary_source;
    }

    __no_constexpr_imp void
        gather_map_source(
            matcher_source_map_t& _a_matcher_source_map
        ) const noexcept
    {
        using namespace ds;
        using namespace std;
        for (const single_source_t& _l_source : _m_sources)
        {
            _a_matcher_source_map.insert(_l_source);
        }
    }

    /*!
     * @brief Default constructor.
     *
     * Builds a matcher_wrapper_t containing an element which is always true.
     */
    template <typename = typename std::enable_if_t<not Has_Annotation>>
    __no_constexpr
        matcher_wrapper_t() noexcept;
    /*!
     * @brief Returns a cref of the element's annotation
     *
     * Can only be called when Has_Annotation is true.
     * @return A const std::string_view to the element's internal annotation.
     */
    template <typename = typename std::enable_if<Has_Annotation>::type>
    __constexpr const std::string_view
                      annotation() const noexcept;
    /*!
     * @brief Returns a reference to the object's mathcer_base_t.
     *
     * If there is no matcher_base_t, this function will throw an exception.
     * @return A reference to the internal matcher_base_t.
     */
    //__constexpr matcher_base_t&
    //    matcher_base() const noexcept;
    /*!
     * @brief Tells the caller whether this object has a non-null
     * matcher_base_t.
     * @return True if there is a non-null matcher_base_t; false otherwise.
     */
    //__constexpr bool
    //    has_matcher_base() const noexcept;
    /*!
     * @brief Returns a cref of the matcher_base_ptr_t to the caller.
     * @return The internal matcher_base_ptr_t to the caller.
     */
    //__constexpr const matcher_base_ptr_t&
    //    internal_matcher() const noexcept;
    /*!
     * @brief This function turns the element into an or statement.
     *
     * The left hand side of the or statement contains what was previously
     * there.
     *
     * The right hand element contains no element.
     *
     * @param _a_single_source The source of the function call.
     * @param _a_test_runner The thread's test runner.
     * @return False if the first element evaluates to true; true otherwise.
     */
    __no_constexpr bool
        or_statement(
            const ds::single_source_t& _a_single_source,
            test_runner_t&             _a_test_runner
            = _ABC_NS_GLOBAL::get_this_threads_test_runner_ref()
        ) noexcept;
    /*!
     * @brief Turns the matcher_t into an and statement.
     * @param _a_single_source The source of the and statment.
     * @param _a_test_runner The thread runner.
     * @return True if the first element is true.
     */
    __no_constexpr bool
        and_statement(
            const ds::single_source_t& _a_single_source,
            test_runner_t&             _a_test_runner
            = _ABC_NS_GLOBAL::get_this_threads_test_runner_ref()
        ) noexcept;
    /*!
     * @brief Processes a secondary element in a logical binary group.
     * @param _a_matcher The matcher to set as the right child.
     * @param _a_single_source The source of the element.
     */
    __constexpr void
        process(
            const matcher_wrapper_t&   _a_matcher,
            const ds::single_source_t& _a_single_source
        );
    /*!
     * @brief Logical not operator. Nots the result from the internal matcher.
     * @return A new matcher.
     */
    __no_constexpr matcher_wrapper_t<Has_Annotation>
                   operator!() const noexcept;
    /*!
     * @brief Logical not operator. Ands the result from the internal matcher.
     * @return A new matcher.
     */
    __no_constexpr matcher_wrapper_t
        operator&&(const matcher_wrapper_t& _a_matcher) const noexcept;
    /*!
     * @brief Logical not operator. Ors the result from the internal matcher.
     * @return A new matcher.
     */
    __no_constexpr matcher_wrapper_t
        operator||(const matcher_wrapper_t& _a_matcher) const noexcept;
    friend matcher_wrapper_t<not Has_Annotation>;
    __no_constexpr friend matcher_wrapper_t<false>
        make_matcher(const matcher_result_t& _a_generic_matcher_ptr) noexcept;
    __no_constexpr friend matcher_wrapper_t<false>
        make_matcher_opt(
            const matcher_result_t&            _a_generic_matcher_ptr,
            const std::optional<precedence_t>& _a_precedence
        ) noexcept;
    template <bool Has_Annotation>
    __constexpr friend _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>
        run_matcher_macro(
            const _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>&
                                        _a_element,
            const std::string_view      _a_macro_str,
            const std::string_view      _a_matcher_str,
            const std::source_location& _a_sl
        ) noexcept;
    __no_constexpr friend matcher_wrapper_t<true>
        annotate_matcher(
            const matcher_wrapper_t<false>& _a_matcher,
            const std::string_view          _a_annotation
        ) noexcept;

    template <bool Has_Annotation, logic_enum_t Logic_Enum>
    __constexpr_imp friend matcher_wrapper_t<Has_Annotation>
        make_strict_logic_matcher_wrapper(
            const std::optional<matcher_wrapper_t<Has_Annotation>>& _a_matcher_l,
            const std::optional<matcher_wrapper_t<Has_Annotation>>& _a_matcher_r,
            const std::string_view                                  _a_default_str
        );

    __constexpr void
        set_result_and_precedence(
            const matcher_result_t& _a_mathcer_result,
            const precedence_t      _a_precedence
        ) noexcept
    {
        _m_test_result = _a_mathcer_result;
        _m_precedence  = _a_precedence;
    }

    __constexpr abc::ds::single_source_t
                add_source_info(
                    const std::string_view      _a_macro_str,
                    const std::string_view      _a_matcher_str,
                    const std::source_location& _a_sl
                ) noexcept
    {
        using namespace std;
        using namespace ds;
        if (_m_primary_source.has_value()
            && _a_matcher_str
                   != _m_primary_source.value().source_code_representation())
        {
            _m_sources.push_back(_m_primary_source.value());
        }
        _m_primary_source = ds::single_source_t(
            abc::utility::str::create_string(
                {_a_macro_str, "(", _a_matcher_str, ")"}
            ),
            _a_sl
        );
        return _m_primary_source.value();
    }

    template <logic_enum_t Logic_Enum>
    requires (Logic_Enum == logic_enum_t::AND || Logic_Enum == logic_enum_t::OR)
    __no_constexpr bool friend simulate_binary_logic_statement(
        matcher_t&                  _a_matcher,
        const std::string_view      _a_macro_str,
        const std::string_view      _a_str_rep,
        const std::source_location& _a_source_location
    ) noexcept;
private:
    std::conditional_t<Has_Annotation, std::string, std::monostate>
                                                 _m_annotation;
    matcher_result_t                             _m_test_result;
    std::vector<ds::single_source_t>             _m_sources;
    std::optional<ds::single_source_t>           _m_primary_source;
    std::optional<std::size_t>                   _m_precedence;
    std::optional<simulated_binary_logic_expr_t> _m_simulated_logic_data;

    __constexpr
    matcher_wrapper_t(
        const matcher_result_t&                   _a_matcher_result,
        const std::vector<ds::single_source_t>&   _a_matcher_sources,
        const std::optional<ds::single_source_t>& _a_primary_source,
        const std::optional<std::size_t>&         _a_precedence,
        const std::optional<simulated_binary_logic_expr_t>&
            _a_simulated_logic_data
    ) noexcept
        : _m_test_result(_a_matcher_result)
        , _m_sources(_a_matcher_sources)
        , _m_primary_source(_a_primary_source)
        , _m_precedence(_a_precedence)
        , _m_simulated_logic_data(_a_simulated_logic_data)
    {}

    template <logic_enum_t Logic_Enum>
    __constexpr bool
        process_(const matcher_wrapper_t& _a_matcher) noexcept;
    /*!
     * @brief Constructor.
     * @param _a_matcher_internal The internal matcher_base_ptr_t to set this
     * element at.
     */
    __no_constexpr
        matcher_wrapper_t(const matcher_result_t& _a_matcher_internal) noexcept;
    __no_constexpr_imp
        matcher_wrapper_t(
            const matcher_wrapper_t<Has_Annotation>& _a_matcher,
            const std::string_view                   _a_macro_str,
            const std::string_view                   _a_matcher_str,
            const std::source_location&              _a_sl
        );
    /*!
     * @brief Constructor.
     * @param _a_matcher The matcher_wrapper_t which is used to build an object
     * with an annotation.
     * @param _a_annotation The annotation itself
     */
    template <typename = typename std::enable_if<Has_Annotation>::type>
    __constexpr
    matcher_wrapper_t(
        const matcher_wrapper_t<false>& _a_matcher,
        const std::string_view          _a_annotation
    ) noexcept;
};

template <logic_enum_t Logic_Enum>
struct simulated_logic_expr_t
{
    __constexpr
    simulated_logic_expr_t(
        const matcher_wrapper_t<false>& _a_matcher
    )
        : _m_matcher_l(_a_matcher)
    {}

    __constexpr void
        reset()
    {
        using namespace std;
        _m_matcher_l = optional<matcher_wrapper_t<false>>{};
        _m_matcher_r = optional<matcher_wrapper_t<false>>{};
    }

    __constexpr const std::optional<matcher_wrapper_t<false>>&
                      left_child() const noexcept
    {
        return _m_matcher_l;
    }

    __constexpr void
        set_left_child(
            const matcher_wrapper_t<false>& _a_matcher
        ) noexcept
    {
        _m_matcher_l = _a_matcher;
    }

    __constexpr void
        set_right_child(
            const matcher_wrapper_t<false>& _a_matcher
        ) noexcept
    {
        _m_matcher_r = _a_matcher;
    }

    __constexpr const std::optional<matcher_wrapper_t<false>>&
                      right_child() const noexcept
    {
        return _m_matcher_r;
    }

    __constexpr explicit
        operator bool()
    {
        if (not _m_matcher_l.has_value() && not _m_matcher_r.has_value())
        {
            return false;
        }
        // Atleast one is set. Prefer the left.
        const matcher_wrapper_t<false>& _l_matcher_ref{
            _m_matcher_l.has_value() ? _m_matcher_l.value()
                                     : _m_matcher_r.value()
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
private:
    std::optional<matcher_wrapper_t<false>> _m_matcher_l;
    std::optional<matcher_wrapper_t<false>> _m_matcher_r;
};

namespace
{
} // namespace

template <logic_enum_t Logic_Enum>
__constexpr bool
    is_precdence_less_than_child(
        const std::optional<precedence_t>& _a_precedence
    ) noexcept
{
    using namespace std;
    return _a_precedence.has_value()
           && logic_precedence<Logic_Enum>() <= _a_precedence.value();
}

_END_ABC_MATCHER_NS
_BEGIN_ABC_NS
using matcher_t           = _ABC_NS_MATCHER::matcher_wrapper_t<false>;
using annotated_matcher_t = _ABC_NS_MATCHER::matcher_wrapper_t<true>;
using simulated_or_expr_t = _ABC_NS_MATCHER::simulated_logic_expr_t<
    _ABC_NS_MATCHER::logic_enum_t::OR>;
using simulated_and_expr_t = _ABC_NS_MATCHER::simulated_logic_expr_t<
    _ABC_NS_MATCHER::logic_enum_t::AND>;
__no_constexpr annotated_matcher_t
    annotate(const matcher_t& _a_matcher, const std::string_view _a_annotation)
        noexcept;

__no_constexpr annotated_matcher_t
    annotate(const std::string_view _a_annotation, const matcher_t& _a_matcher)
        noexcept;
_END_ABC_NS

_BEGIN_ABC_MATCHER_NS
template <bool Has_Annotation>
template <typename>
__no_constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t() noexcept
// : matcher_wrapper_t<Has_Annotation>(bool_matcher(true).matcher_result())
{}

template <bool Has_Annotation>
__no_constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
        const matcher_result_t& _a_matcher_internal
    ) noexcept
    : _m_test_result(_a_matcher_internal)
{}

template <bool Has_Annotation>
template <typename>
__constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
        const matcher_wrapper_t<false>& _a_matcher,
        const std::string_view          _a_annotation
    ) noexcept
    : _m_test_result(_a_matcher._m_test_result)
    , _m_sources(_a_matcher._m_sources)
    , _m_primary_source(_a_matcher._m_primary_source)
    , _m_precedence(_a_matcher._m_precedence)
    , _m_simulated_logic_data(_a_matcher._m_simulated_logic_data)
    , _m_annotation(_a_annotation)
{}

template <bool Has_Annotation>
__no_constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
        const matcher_wrapper_t<Has_Annotation>& _a_matcher,
        const std::string_view                   _a_macro_str,
        const std::string_view                   _a_matcher_str,
        const std::source_location&              _a_sl
        // const ds::single_source_t&               _a_single_source
    )
    : _m_test_result(_a_matcher._m_test_result)
    , _m_sources(_a_matcher._m_sources)
    , _m_primary_source(_a_matcher._m_primary_source)
    , _m_precedence(_a_matcher._m_precedence)
    , _m_simulated_logic_data(_a_matcher._m_simulated_logic_data)
    , _m_annotation(_a_matcher._m_annotation)
{
    add_source_info(_a_macro_str, _a_matcher_str, _a_sl);
}

template <bool Has_Annotation>
template <typename>
__constexpr_imp const std::string_view
    matcher_wrapper_t<Has_Annotation>::annotation() const noexcept
{
    return _m_annotation;
}

/*template <bool Has_Annotation>
__constexpr_imp matcher_base_t&
    matcher_wrapper_t<Has_Annotation>::matcher_base() const noexcept
{
    return *this->_m_matcher_internal;
}

template <bool Has_Annotation>
__constexpr_imp bool
    matcher_wrapper_t<Has_Annotation>::has_matcher_base() const noexcept
{
    return this->_m_matcher_internal != nullptr;
}

template <bool Has_Annotation>
__constexpr_imp const matcher_base_ptr_t&
    matcher_wrapper_t<Has_Annotation>::internal_matcher() const noexcept
{
    return _m_matcher_internal;
}*/

template <bool Has_Annotation>
__no_constexpr_imp bool
    matcher_wrapper_t<Has_Annotation>::or_statement(
        const ds::single_source_t& _a_single_source,
        test_runner_t&             _a_test_runner
    ) noexcept
{
    const bool _l_result{_m_test_result.passed()};
    //*this = augment_matcher_using_binary_logic(logic_enum_t::OR);
    // add_source_info(_a_single_source);
    return not _l_result;
}

template <bool Has_Annotation>
__no_constexpr_imp bool
    matcher_wrapper_t<Has_Annotation>::and_statement(
        const ds::single_source_t& _a_single_source,
        test_runner_t&             _a_test_runner
    ) noexcept
{
    const bool _l_result{_m_test_result.passed()};
    // *this = make_matcher(new logic_matcher_t<AND>(
    //     this->internal_matcher(), std::shared_ptr<matcher_base_t>()
    // ));
    // _m_matcher_internal->add_source_info(_a_single_source);
    return _l_result;
}

template <bool Has_Annotation>
__constexpr_imp void
    matcher_wrapper_t<Has_Annotation>::process(
        const matcher_wrapper_t&   _a_matcher,
        const ds::single_source_t& _a_single_source
    )
{
    using enum logic_enum_t;
    if (not (process_<OR>(_a_matcher) || process_<AND>(_a_matcher)))
    {
        throw errors::test_library_exception_t(
            fmt::format("Could not run process; the parent node we are trying "
                        "to add an OR or AND "
                        "node to is not a logic_matcher_t. Please check your "
                        "code, or contact the developer.")
        );
    }
    else
    {
        // add_source_info(_a_single_source);
    }
}

template <bool Has_Annotation>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator!() const noexcept
{
    using namespace std;
    string                  _l_child_str{"<false>"};
    bool                    _l_child_passed{false};
    bool                    _l_precdence_less_than_logic_enum{false};
    const matcher_result_t& _l_child_res{_m_test_result};
    _l_child_str    = _l_child_res.str();
    _l_child_passed = not _l_child_res.passed();
    _l_precdence_less_than_logic_enum
        = is_precdence_less_than_child<logic_enum_t::NOT>(_m_precedence);
    pair<const char*, const char*>& _l_str_pair{
        _l_precdence_less_than_logic_enum ? _c_normal_bracket_pair
                                          : _c_normal_bracket_pair
    };
    return matcher_result_t(
        _l_child_passed,
        fmt::format(
            "{0}{1}{2}{3}",
            logic_str<logic_enum_t::NOT>(),
            _l_str_pair.first,
            _l_child_str,
            _l_str_pair.second
        )
    );
}

template <bool Has_Annotation>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator&&(
        const matcher_wrapper_t& _a_matcher
    ) const noexcept
{
    return make_strict_logic_matcher_wrapper<Has_Annotation, logic_enum_t::AND>(
        *this, _a_matcher,""
    );
}

template <bool Has_Annotation>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator||(
        const matcher_wrapper_t& _a_matcher
    ) const noexcept
{
    return make_strict_logic_matcher_wrapper<Has_Annotation, logic_enum_t::OR>(
        *this, _a_matcher,""
    );
}

template <logic_enum_t Logic_Enum>
requires (Logic_Enum == logic_enum_t::AND || Logic_Enum == logic_enum_t::OR)
__no_constexpr_imp bool
    simulate_binary_logic_statement(
        matcher_t&                  _a_matcher,
        const std::string_view      _a_macro_str,
        const std::string_view      _a_str_rep,
        const std::source_location& _a_source_location
    ) noexcept
{
    const bool _l_result{_a_matcher.matcher_result().passed()};
    using namespace std;
    string _l_left_str{_a_matcher.matcher_result().str()};
    bool   _l_left_passed{_a_matcher.matcher_result().passed()};
    bool   _l_does_right_need_to_be_evaluated;
    string _l_right_str;
    if constexpr (Logic_Enum == logic_enum_t::OR)
    {
        _l_does_right_need_to_be_evaluated = not _l_left_passed;
    }
    else
    {
        // Logic_Enum == logic_enum_t::AND
        _l_does_right_need_to_be_evaluated = _l_left_passed;
    }
    if (_l_does_right_need_to_be_evaluated)
    {
        _l_right_str = "<unset>";
        logic_enum_t     _m_logic_type_of_matcher;
        matcher_result_t _m_matcher_result_l;
        bool             _m_left_precedence_less_than_logic_enum;
    }
    else
    {
        _l_right_str = "<unevaluated>";
    }
    pair<const char*, const char*>& _l_str_pair_l{
        is_precdence_less_than_child<Logic_Enum>(_a_matcher.precedence())
            ? _c_normal_bracket_pair
            : _c_normal_bracket_pair
    };
    pair<const char*, const char*>& _l_str_pair_r{
        is_precdence_less_than_child<Logic_Enum>(_a_matcher.precedence())
            ? _c_normal_bracket_pair
            : _c_normal_bracket_pair
    };
    matcher_result_t _l_mr(
        compute_logic_result<Logic_Enum>(_l_left_passed, true),
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
    _a_matcher.set_result_and_precedence(_l_mr, Logic_Enum);
    _a_matcher.add_source_info(_a_macro_str, _a_str_rep, _a_source_location);
    return _l_result;
}

template <bool Has_Annotation>
template <logic_enum_t Logic_Enum>
__constexpr_imp bool
    matcher_wrapper_t<Has_Annotation>::process_(
        const matcher_wrapper_t& _a_matcher
    ) noexcept
{
    matcher_base_t*              _l_matcher_ptr{this->internal_matcher().get()};
    logic_matcher_t<Logic_Enum>* _l_ptr_or{
        dynamic_cast<logic_matcher_t<Logic_Enum>*>(_l_matcher_ptr)
    };
    if (_l_ptr_or != nullptr)
    {
        _l_ptr_or->set_right_child(_a_matcher.internal_matcher());
        return true;
    }
    else
    {
        return false;
    }
}

__no_constexpr_imp matcher_wrapper_t<false>
                   make_matcher(
                       const matcher_result_t& _a_generic_matcher_ptr
                   ) noexcept
{
    using namespace std;
    return matcher_wrapper_t<false>(
        _a_generic_matcher_ptr,
        std::vector<ds::single_source_t>{},
        std::optional<ds::single_source_t>{},
        std::optional<precedence_t>{},
        std::optional<simulated_binary_logic_expr_t>{}
    );
}

__no_constexpr_imp matcher_wrapper_t<false>
                   make_matcher_opt(
                       const matcher_result_t&            _a_generic_matcher_ptr,
                       const std::optional<precedence_t>& _a_precedence
                   ) noexcept
{
    return matcher_wrapper_t<false>(
        _a_generic_matcher_ptr,
        std::vector<ds::single_source_t>{},
        std::optional<ds::single_source_t>{},
        _a_precedence,
        std::optional<simulated_binary_logic_expr_t>{}
    );
}

template <bool Has_Annotation>
__constexpr_imp _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>
                run_matcher_macro(
                    const _ABC_NS_MATCHER::matcher_wrapper_t<Has_Annotation>& _a_element,
                    const std::string_view                                    _a_macro_str,
                    const std::string_view      _a_matcher_str,
                    const std::source_location& _a_sl
                ) noexcept
{
    return matcher_wrapper_t<Has_Annotation>(
        _a_element, _a_macro_str, _a_matcher_str, _a_sl
    );
}

__no_constexpr_imp matcher_wrapper_t<true>
                   annotate_matcher(
                       const matcher_wrapper_t<false>& _a_matcher,
                       const std::string_view          _a_annotation
                   ) noexcept
{
    return matcher_wrapper_t<true>(_a_matcher, _a_annotation);
}

template <bool Has_Annotation, logic_enum_t Logic_Enum>
__constexpr_imp matcher_wrapper_t<Has_Annotation>
                make_strict_logic_matcher_wrapper(
                    const std::optional<matcher_wrapper_t<Has_Annotation>>& _a_matcher_l,
                    const std::optional<matcher_wrapper_t<Has_Annotation>>& _a_matcher_r,
                    const std::string_view                                  _a_default_str
                )
{
    using namespace std;
    using namespace abc::ds;
    string _l_left_str{
        _a_matcher_l.has_value() ? _a_matcher_l.value().matcher_result().str()
                                 : _a_default_str
    };
    string _l_right_str{
        _a_matcher_r.has_value() ? _a_matcher_r.value().matcher_result().str()
                                 : _a_default_str
    };
    bool _l_left_passed{
        _a_matcher_l.has_value()
            ? _a_matcher_l.value().matcher_result().passed()
            : false
    };
    bool _l_right_passed{
        _a_matcher_r.has_value()
            ? _a_matcher_r.value().matcher_result().passed()
            : false
    };
    pair<const char*, const char*>& _l_str_pair_l{
        is_precdence_less_than_child<Logic_Enum>(
            _a_matcher_l.has_value() ? _a_matcher_l.value().precedence()
                                     : optional<precedence_t>{}
        )
            ? _c_normal_bracket_pair
            : _c_normal_bracket_pair
    };
    pair<const char*, const char*>& _l_str_pair_r{
        is_precdence_less_than_child<Logic_Enum>(
            _a_matcher_r.has_value() ? _a_matcher_r.value().precedence()
                                     : optional<precedence_t>{}
        )
            ? _c_normal_bracket_pair
            : _c_normal_bracket_pair
    };
    matcher_result_t _l_mr(
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
    vector<single_source_t> _l_sources_l =
        _a_matcher_l.has_value() ?
        make_matcher_vector(
            _a_matcher_l.value().get_sources(), _a_matcher_l.value().primary_source()
        ) : vector<single_source_t>{};
    vector<single_source_t> _l_sources_r =
        _a_matcher_r.has_value() ?
        make_matcher_vector(
            _a_matcher_r.value().get_sources(), _a_matcher_r.value().primary_source()
        ) : vector<single_source_t>{};
    vector<single_source_t> _l_sources =
        abc::utility::join(_l_sources_l, _l_sources_r);
    return matcher_wrapper_t<Has_Annotation>(
        _l_mr,
        _l_sources,
        std::optional<ds::single_source_t>(),
        logic_precedence<Logic_Enum>(),
        std::optional<simulated_binary_logic_expr_t>{}
    );
}

_END_ABC_MATCHER_NS

_BEGIN_ABC_NS
__no_constexpr_imp annotated_matcher_t
    annotate(
        const matcher_t&       _a_matcher,
        const std::string_view _a_annotation
    ) noexcept
{
    return annotate_matcher(_a_matcher, _a_annotation);
}

__no_constexpr_imp annotated_matcher_t
    annotate(
        const std::string_view _a_annotation,
        const matcher_t&       _a_matcher
    ) noexcept
{
    return annotate_matcher(_a_matcher, _a_annotation);
}

_END_ABC_NS