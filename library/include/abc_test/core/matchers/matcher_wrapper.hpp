#pragma once
#include "abc_test/core/global.hpp"
#include "abc_test/core/matchers/logic_enum.hpp"
#include "abc_test/core/matchers/static_matcher.hpp"
#include "abc_test/utility/ranges.hpp"

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief This class holds all the logic for matchers in abc_test.
 *
 * A matcher is a class which holds the result of a user-written test.
 *
 * @tparam Has_Annotation Whether the matcher_wrapper_t is allowed an annotation
 * with it.
 */
template <bool Has_Annotation>
struct matcher_wrapper_t
{
public:
    /*!
     * @brief Default constructor.
     *
     * Builds a matcher_wrapper_t containing an element which is always true.
     */
    template <bool B = not Has_Annotation, typename = std::enable_if_t<B>>
    __no_constexpr
        matcher_wrapper_t() noexcept;

    template <bool Arg_Has_Annotation>
    __no_constexpr_imp void
        override_matcher(
            const matcher_wrapper_t<Arg_Has_Annotation>& _a_matcher_result
        ) noexcept
    {
        this->_m_test_result = _a_matcher_result.matcher_result();
    }

    /*!
     * @brief Gets the sourses attached to this matcher.
     * @return Cref to the vector of single_source_t objects.
     */
    __constexpr_imp const std::vector<ds::single_source_t>&
                          get_sources() const noexcept;
    /*!
     * @brief Gets the precedence of this matcher's contained expression (if
     * any).
     * @return A cref to the precedence_t of the matcher.
     */
    __constexpr const std::optional<precedence_t>&
                      precedence() const noexcept;
    /*!
     * @brief Gets the matcher's matcher_result_t member variable.
     * @return A cref to the matcher's matcher_result_t variable.
     */
    __constexpr const matcher_result_t&
        matcher_result() const noexcept;
    /*!
     * @brief Resets the matcher's primary source.
     *
     * Sets the internal variable _m_primary_source to nullopt.
     *
     */
    __constexpr_imp void
        remove_primary_source() noexcept;
    /*!
     * @brief Gets the matcher's primary_source member variable.
     * @return A cref to the matcher's primary source.
     */
    __constexpr const std::optional<ds::single_source_t>&
                      primary_source() const noexcept;
    /*!
     * @brief Adds the matcher's sources to the matcher_source_map_t provided as
     * an argument.
     * @param _a_matcher_source_map A ref to the matcher_source_map_t which is
     * used to add the matcher's sources to.
     */
    __no_constexpr_imp void
        gather_map_source(matcher_source_map_t& _a_matcher_source_map
        ) const noexcept;
    /*!
     * @brief Returns a cref of the element's annotation
     *
     * Can only be called when Has_Annotation is true.
     * @return A const std::string_view to the element's internal annotation.
     */
    template <bool B = Has_Annotation, typename = std::enable_if_t<B>>
    __constexpr const std::u8string_view
                      annotation() const noexcept;

    /*!
     * @brief Adds primary source information to a matcher.
     *
     * Specifically, it checks to ensure that the entity being added doesn't
     * superseed the current primary source. By this we mean, if the current
     * primary source is in the form
     *
     * "_MACRO_USED_TO_CALL(internals)"
     *
     * And the new primary source is
     *
     * "_OUTER_MATCHER_CALL(_MACRO_USED_TO_CALL(internals))"
     *
     * Then we say the old source is unrequired, and it is not added to the
     * matchers set of sources.
     *
     * @param _a_macro_str std::string_view representing the macro used in the
     * hierarchy to call this function.
     * @param _a_matcher_str std::string_view representing the string inside the
     * macro used to in the hierarhcy to call this function
     * @param _a_sl std::source_location of where the macro was called from.
     * @return single_source_t representing the new primary source.
     */
    __constexpr abc::ds::single_source_t
                add_source_info(
                    const std::string_view      _a_macro_str,
                    const std::string_view      _a_matcher_str,
                    const std::source_location& _a_sl
                ) noexcept;
    /*!
     * @brief Logical not operator. Nots the result from the internal matcher.
     * @return A new matcher.
     */
    template <bool B = not Has_Annotation, typename = std::enable_if_t<B>>
    __no_constexpr matcher_wrapper_t<Has_Annotation>
                   operator!() const noexcept;
    /*!
     * @brief Logical not operator. Ands the result from the internal matcher.
     * @return A new matcher.
     */
    template <bool B = not Has_Annotation, typename = std::enable_if_t<B>>
    __no_constexpr matcher_wrapper_t
        operator&&(const matcher_wrapper_t& _a_matcher) const noexcept;
    /*!
     * @brief Logical not operator. Ors the result from the internal matcher.
     * @return A new matcher.
     */
    template <bool B = not Has_Annotation, typename = std::enable_if_t<B>>
    __no_constexpr matcher_wrapper_t
        operator||(const matcher_wrapper_t& _a_matcher) const noexcept;
    friend matcher_wrapper_t<not Has_Annotation>;
    /*!
     * @brief Friend function which creates a matcher_wrapper_t from a
     * matcher_result_t variable.
     * @param _a_generic_matcher_ptr The mathcer_result_t to use in the created
     * matcher_t.
     * @return The created matcher_wrapper_t<false>.
     */
    __no_constexpr friend matcher_wrapper_t<false>
        mk_matcher_using_result(const matcher_result_t& _a_generic_matcher_ptr
        ) noexcept;
    /*!
     * @brief Friend function which creates a matcher_wrapper_t from
     * matcher_result_t and precedence_t variables.
     * @param _a_generic_matcher_ptr The matcher_result_t used in the created
     * matcher_wrapper_t<false> object.
     * @param _a_precedence precedence_t entity used as the precedence in the
     * created matcher_wrapper_t<false> object.
     * @return The created matcher_wrapper_t<false> object.
     */
    __no_constexpr friend matcher_wrapper_t<false>
        mk_matcher_using_result_and_precedence(
            const matcher_result_t& _a_generic_matcher_ptr,
            const precedence_t      _a_precedence
        ) noexcept;
    /*!
     * @brief Public method for running the "_MATCHER" macro on a matcher. This
     * macro adds source information to a matcher_t object.
     * @tparam Has_Annotation Bool denoting whether the matcher_wrapper_t object
     * has an annotation.
     * @param _a_element The matcher_wrapper_t input object.
     * @param _a_macro_str std::string_view, representing the name of the macro
     * used to invoke this function.
     * @param _a_matcher_str std::string_view, representing the internal string
     * of the macro.
     * @param _a_sl std::source_location The source location of where the macro
     * was called from.
     * @return A matcher_wrapper_t object whose primary source is set to the
     * macro information given.
     */
    template <bool Arg_Has_Annotation>
    __constexpr friend _ABC_NS_MATCHER::matcher_wrapper_t<Arg_Has_Annotation>
        mk_matcher_from_MATCHER_macro(
            const _ABC_NS_MATCHER::matcher_wrapper_t<Arg_Has_Annotation>&
                                        _a_element,
            const std::string_view      _a_macro_str,
            const std::string_view      _a_matcher_str,
            const std::source_location& _a_sl
        ) noexcept;
    /*!
     * @brief Friend function which adds an annotation to a
     * matcher_wrapper_t<false> object.
     * @param _a_matcher The matcher_wrapper_t<false> object used as the base
     * for the returned matcher_wrapper_t<true> object.
     * @param _a_annotation std::string_view representing a user-provided
     * annotation for the matcher.
     * @return A matcehr_wrapper_t<true> object, based off _a_matcher with
     * _a_annotation used as its annotation.
     */
    __no_constexpr friend matcher_wrapper_t<true>
        mk_matcher_using_matcher_and_annotation(
            const matcher_wrapper_t<false>& _a_matcher,
            const std::u8string_view        _a_annotation
        ) noexcept;
    /*!
     * @brief Creates a matcher_wrapper_t<false> entity representing a binary
     * logical expression.
     * @tparam Logic_Enum The logic_enum_t used as the basis for the logical
     * expression.
     * @param _a_matcher_l The (optional) matcher_wrapper_t<false> left child
     * expression.
     * @param _a_matcher_r The (optional) matcher_wrapper_t<false> right child
     * expression.
     * @param _a_str_representing_nullopt The std::string_view used when a
     * nullopt is found in either left or right child expression.
     * @return A matcher_wrapper_t<false> object represnting a binary logical
     * expression.
     */
    template <logic_enum_t Logic_Enum>
    __constexpr_imp friend matcher_wrapper_t<false>
        mk_matcher_representing_binary_logical_expr(
            const std::optional<matcher_wrapper_t<false>>& _a_matcher_l,
            const std::optional<matcher_wrapper_t<false>>& _a_matcher_r,
            const std::u8string_view _a_str_representing_nullopt
        ) noexcept;
    __constexpr explicit
        operator bool();
private:
    std::conditional_t<Has_Annotation, std::u8string, std::monostate>
                                       _m_annotation;
    matcher_result_t                   _m_test_result;
    std::vector<ds::single_source_t>   _m_sources;
    std::optional<ds::single_source_t> _m_primary_source;
    std::optional<std::size_t>         _m_precedence;
    /*!
     * @brief Main constructor. Every other constructor calls this one
     */
    __constexpr
    matcher_wrapper_t(
        const matcher_result_t&                 _a_matcher_result,
        const std::vector<ds::single_source_t>& _a_matcher_sources
        = std::vector<ds::single_source_t>{},
        const std::optional<ds::single_source_t>& _a_primary_source
        = std::optional<ds::single_source_t>{},
        const std::optional<precedence_t>& _a_precedence
        = std::optional<precedence_t>{}
    ) noexcept;
    /*!
     * @brief Constructor which includes bit-parts of macro which will be
     * primary source.
     */
    __no_constexpr_imp
        matcher_wrapper_t(
            const matcher_wrapper_t<Has_Annotation>& _a_matcher,
            const std::string_view                   _a_macro_str,
            const std::string_view                   _a_matcher_str,
            const std::source_location&              _a_sl
        );
    /*!
     * @brief Constructor for annotated member
     */
    template <bool B = Has_Annotation, typename = std::enable_if_t<B>>
    __constexpr
    matcher_wrapper_t(
        const matcher_wrapper_t<false>& _a_matcher,
        const std::u8string_view        _a_annotation
    ) noexcept;
};

namespace
{
std::pair<const char*, const char*> _c_normal_bracket_pair{"(", ")"};
std::pair<const char*, const char*> _c_empty_bracket_pair{"", ""};
__constexpr                         std::vector<ds::single_source_t>
                                    make_matcher_vector(
                                        const std::vector<ds::single_source_t>&   _a_sources,
                                        const std::optional<ds::single_source_t>& _a_primary_source
                                    ) noexcept;
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
__no_constexpr annotated_matcher_t
    annotate(
        const matcher_t&         _a_matcher,
        const std::u8string_view _a_annotation
    ) noexcept;

__no_constexpr annotated_matcher_t
    annotate(
        const std::u8string_view _a_annotation,
        const matcher_t&         _a_matcher
    ) noexcept;
_END_ABC_NS

_BEGIN_ABC_MATCHER_NS
template <bool Has_Annotation>
template <bool B, typename>
__no_constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t() noexcept
    : matcher_wrapper_t(true_matcher())
{}

template <bool Has_Annotation>
__constexpr_imp const std::vector<ds::single_source_t>&
    matcher_wrapper_t<Has_Annotation>::get_sources() const noexcept
{
    return _m_sources;
}

template <bool Has_Annotation>
__constexpr const std::optional<precedence_t>&
                  matcher_wrapper_t<Has_Annotation>::precedence() const noexcept
{
    return _m_precedence;
}

template <bool Has_Annotation>
__constexpr const matcher_result_t&
    matcher_wrapper_t<Has_Annotation>::matcher_result() const noexcept
{
    return _m_test_result;
}

template <bool Has_Annotation>
__constexpr_imp void
    matcher_wrapper_t<Has_Annotation>::remove_primary_source() noexcept
{
    using namespace std;
    using namespace ds;
    _m_primary_source = optional<single_source_t>{};
}

template <bool Has_Annotation>
__constexpr const std::optional<ds::single_source_t>&
    matcher_wrapper_t<Has_Annotation>::primary_source() const noexcept
{
    return _m_primary_source;
}

template <bool Has_Annotation>
__no_constexpr_imp void
    matcher_wrapper_t<Has_Annotation>::gather_map_source(
        matcher_source_map_t& _a_matcher_source_map
    ) const noexcept
{
    using namespace ds;
    for (const single_source_t& _l_source : _m_sources)
    {
        _a_matcher_source_map.insert(_l_source);
    }
}

template <bool Has_Annotation>
template <bool B, typename>
__constexpr_imp const std::u8string_view
    matcher_wrapper_t<Has_Annotation>::annotation() const noexcept
{
    return _m_annotation;
}

template <bool Has_Annotation>
__constexpr_imp abc::ds::single_source_t
                matcher_wrapper_t<Has_Annotation>::add_source_info(
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
        _ABC_NS_UTILITY_STR::mk_str_appending_function_name_and_function_args(
            _a_macro_str, _a_matcher_str
        ),
        _a_sl
    );
    return _m_primary_source.value();
}

template <bool Has_Annotation>
template <bool B, typename>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator!() const noexcept
{
    using namespace std;
    std::vector<matcher_result_infos_t::tree_structure_t> _l_children;
    bool                    _l_child_passed{false};
    bool                    _l_precdence_less_than_logic_enum{false};
    const matcher_result_t& _l_child_res{_m_test_result};
    auto                    _l_child_str = _l_child_res.str();
    _l_child_passed                      = not _l_child_res.passed();
    _l_precdence_less_than_logic_enum
        = is_precdence_less_than_child<logic_enum_t::NOT>(_m_precedence);
    pair<const char*, const char*>& _l_str_pair{
        _l_precdence_less_than_logic_enum ? _c_normal_bracket_pair
                                          : _c_normal_bracket_pair
    };
    if (not _l_child_passed)
    {
        _l_children.push_back(make_tuple(
            fmt::format(u8"Child"),
            u8"C",
            make_shared<matcher_result_infos_t>(
                _l_child_str
            )
        ));
    }
    return matcher_result_t(
        _l_child_passed,
        matcher_result_infos_t(
            fmt::format(
                u8"{0}{1}{2}{3}",
                logic_str<logic_enum_t::NOT>(),
                abc::utility::str::cast_string_to_u8string(_l_str_pair.first),
                _l_child_res.str().primary_data(),
                abc::utility::str::cast_string_to_u8string(_l_str_pair.second)
            ),
            {},
            _l_children
        )
    );
}

template <bool Has_Annotation>
template <bool B, typename>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator&&(
        const matcher_wrapper_t& _a_matcher
    ) const noexcept
{
    return mk_matcher_representing_binary_logical_expr<logic_enum_t::AND>(
        *this, _a_matcher, u8""
    );
}

template <bool Has_Annotation>
template <bool B, typename>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator||(
        const matcher_wrapper_t& _a_matcher
    ) const noexcept
{
    return mk_matcher_representing_binary_logical_expr<logic_enum_t::OR>(
        *this, _a_matcher, u8""
    );
}

__no_constexpr_imp matcher_wrapper_t<false>
                   mk_matcher_using_result(
                       const matcher_result_t& _a_generic_matcher_ptr
                   ) noexcept
{
    using namespace std;
    return matcher_wrapper_t<false>(
        _a_generic_matcher_ptr,
        std::vector<ds::single_source_t>{},
        std::optional<ds::single_source_t>{},
        std::optional<precedence_t>{}
    );
}

__no_constexpr_imp matcher_wrapper_t<false>
                   mk_matcher_using_result_and_precedence(
                       const matcher_result_t& _a_generic_matcher_ptr,
                       const precedence_t      _a_precedence
                   ) noexcept
{
    return matcher_wrapper_t<false>(
        _a_generic_matcher_ptr,
        std::vector<ds::single_source_t>{},
        std::optional<ds::single_source_t>{},
        std::optional<precedence_t>(_a_precedence)
    );
}

template <bool Arg_Has_Annotation>
__constexpr_imp _ABC_NS_MATCHER::matcher_wrapper_t<Arg_Has_Annotation>
                mk_matcher_from_MATCHER_macro(
                    const _ABC_NS_MATCHER::matcher_wrapper_t<Arg_Has_Annotation>&
                                                _a_element,
                    const std::string_view      _a_macro_str,
                    const std::string_view      _a_matcher_str,
                    const std::source_location& _a_sl
                ) noexcept
{
    return matcher_wrapper_t<Arg_Has_Annotation>(
        _a_element, _a_macro_str, _a_matcher_str, _a_sl
    );
}

__no_constexpr_imp matcher_wrapper_t<true>
                   mk_matcher_using_matcher_and_annotation(
                       const matcher_wrapper_t<false>& _a_matcher,
                       const std::u8string_view        _a_annotation
                   ) noexcept
{
    return matcher_wrapper_t<true>(_a_matcher, _a_annotation);
}

template <logic_enum_t Logic_Enum>
__constexpr_imp matcher_wrapper_t<false>
                mk_matcher_representing_binary_logical_expr(
                    const std::optional<matcher_wrapper_t<false>>& _a_matcher_l,
                    const std::optional<matcher_wrapper_t<false>>& _a_matcher_r,
                    const std::u8string_view                       _a_default_str
                ) noexcept
{
    using namespace std;
    using namespace abc::ds;
    using namespace _ABC_NS_UTILITY_STR;
    std::vector<matcher_result_infos_t::tree_structure_t> _l_infos;
    abc::matcher::matcher_result_infos_t                  _l_left_str{
        _a_matcher_l.has_value()
                             ? abc::matcher::matcher_result_infos_t(
                  _a_matcher_l.value().matcher_result().str()
              )
                             : matcher_result_infos_t(_a_default_str)
    };
    std::vector<matcher_result_infos_t::tree_structure_t> _l_children;
    if (_a_matcher_l.has_value()
        && not _a_matcher_l.value().matcher_result().passed())
    {
        _l_children.push_back(make_tuple(
            fmt::format(u8"Left-hand"),
            u8"L",
            make_shared<matcher_result_infos_t>(
                _a_matcher_l.value().matcher_result().str()
            )
        ));
    }

    matcher_result_infos_t _l_right_str{
        _a_matcher_r.has_value() ? _a_matcher_r.value().matcher_result().str()
                                 : matcher_result_infos_t(_a_default_str)
    };
    if (_a_matcher_r.has_value()
        && not _a_matcher_r.value().matcher_result().passed())
    {
        _l_children.push_back(make_tuple(
            fmt::format(u8"Right-hand"),
            u8"R",
            make_shared<matcher_result_infos_t>(
                _a_matcher_r.value().matcher_result().str()
            )
        ));
    }
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
        matcher_result_infos_t(
            fmt::format(
                u8"{0}{1}{2} {3} {4}{5}{6}",
                cast_string_to_u8string(_l_str_pair_l.first),
                _l_left_str.primary_data(),
                cast_string_to_u8string(_l_str_pair_l.second),
                logic_str<Logic_Enum>(),
                cast_string_to_u8string(_l_str_pair_r.first),
                _l_right_str.primary_data(),
                cast_string_to_u8string(_l_str_pair_r.second)
            ),
            {},
            _l_children
        )
    );
    vector<single_source_t> _l_sources_l
        = _a_matcher_l.has_value() ? make_matcher_vector(
                                         _a_matcher_l.value().get_sources(),
                                         _a_matcher_l.value().primary_source()
                                     )
                                   : vector<single_source_t>{};
    vector<single_source_t> _l_sources_r
        = _a_matcher_r.has_value() ? make_matcher_vector(
                                         _a_matcher_r.value().get_sources(),
                                         _a_matcher_r.value().primary_source()
                                     )
                                   : vector<single_source_t>{};
    vector<single_source_t> _l_sources
        = abc::utility::join<vector<single_source_t>>(
            _l_sources_l, _l_sources_r
        );
    return matcher_wrapper_t<false>(
        _l_mr,
        _l_sources,
        std::optional<ds::single_source_t>(),
        logic_precedence<Logic_Enum>()
    );
}

template <bool Has_Annotation>
__constexpr matcher_wrapper_t<Has_Annotation>::operator bool()
{
    return _m_test_result.passed();
}

template <bool Has_Annotation>
__constexpr
matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
    const matcher_result_t&                   _a_matcher_result,
    const std::vector<ds::single_source_t>&   _a_matcher_sources,
    const std::optional<ds::single_source_t>& _a_primary_source,
    const std::optional<precedence_t>&        _a_precedence
) noexcept
    : _m_test_result(_a_matcher_result)
    , _m_sources(_a_matcher_sources)
    , _m_primary_source(_a_primary_source)
    , _m_precedence(_a_precedence)
{}

template <bool Has_Annotation>
__no_constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
        const matcher_wrapper_t<Has_Annotation>& _a_matcher,
        const std::string_view                   _a_macro_str,
        const std::string_view                   _a_matcher_str,
        const std::source_location&              _a_sl
    )
    : _m_test_result(_a_matcher._m_test_result)
    , _m_sources(_a_matcher._m_sources)
    , _m_primary_source(_a_matcher._m_primary_source)
    , _m_precedence(_a_matcher._m_precedence)
    , _m_annotation(_a_matcher._m_annotation)
{
    add_source_info(_a_macro_str, _a_matcher_str, _a_sl);
}

template <bool Has_Annotation>
template <bool B, typename>
__constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
        const matcher_wrapper_t<false>& _a_matcher,
        const std::u8string_view        _a_annotation
    ) noexcept
    :  _m_annotation(_a_annotation)
    , _m_test_result(_a_matcher._m_test_result)
    , _m_sources(_a_matcher._m_sources)
    , _m_primary_source(_a_matcher._m_primary_source)
    , _m_precedence(_a_matcher._m_precedence)
{}

namespace
{
__constexpr_imp std::vector<ds::single_source_t>
                make_matcher_vector(
                    const std::vector<ds::single_source_t>&   _a_sources,
                    const std::optional<ds::single_source_t>& _a_primary_source
                ) noexcept
{
    using namespace std;
    using namespace ds;
    vector<single_source_t> _l_rv{_a_sources};
    if (_a_primary_source.has_value())
    {
        _l_rv.push_back(_a_primary_source.value());
    }
    return _l_rv;
}
} // namespace

_END_ABC_MATCHER_NS

_BEGIN_ABC_NS
__no_constexpr_imp annotated_matcher_t
    annotate(
        const matcher_t&         _a_matcher,
        const std::u8string_view _a_annotation
    ) noexcept
{
    return mk_matcher_using_matcher_and_annotation(_a_matcher, _a_annotation);
}

__no_constexpr_imp annotated_matcher_t
    annotate(
        const std::u8string_view _a_annotation,
        const matcher_t&         _a_matcher
    ) noexcept
{
    return mk_matcher_using_matcher_and_annotation(_a_matcher, _a_annotation);
}

_END_ABC_NS