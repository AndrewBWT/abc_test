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
    __constexpr matcher_base_t&
        matcher_base() const noexcept;
    /*!
     * @brief Tells the caller whether this object has a non-null
     * matcher_base_t.
     * @return True if there is a non-null matcher_base_t; false otherwise.
     */
    __constexpr bool
        has_matcher_base() const noexcept;
    /*!
     * @brief Returns a cref of the matcher_base_ptr_t to the caller.
     * @return The internal matcher_base_ptr_t to the caller.
     */
    __constexpr const matcher_base_ptr_t&
        internal_matcher() const noexcept;
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
    __no_constexpr_imp friend matcher_wrapper_t<false>
        make_matcher(matcher_base_t* _a_generic_matcher_ptr) noexcept;
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
            const matcher_wrapper_t<false>&       _a_matcher,
            const std::string_view _a_annotation
        ) noexcept;
private:
    std::conditional_t<Has_Annotation, std::string, std::monostate>
                       _m_annotation;
    matcher_base_ptr_t _m_matcher_internal;
    template <logic_enum_t Logic_Enum>
    __constexpr bool
        process_(const matcher_wrapper_t& _a_matcher) noexcept;
    /*!
     * @brief Constructor.
     * @param _a_matcher_internal The internal matcher_base_ptr_t to set this
     * element at.
     */
    __no_constexpr
        matcher_wrapper_t(matcher_base_ptr_t _a_matcher_internal) noexcept;
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

_END_ABC_MATCHER_NS
_BEGIN_ABC_NS
using matcher_t           = _ABC_NS_MATCHER::matcher_wrapper_t<false>;
using annotated_matcher_t = _ABC_NS_MATCHER::matcher_wrapper_t<true>;

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
    : matcher_wrapper_t<Has_Annotation>(
          matcher_base_ptr_t(new static_matcher_t<reports::pass_t>())
      )
{}

template <bool Has_Annotation>
__no_constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
        matcher_base_ptr_t _a_matcher_internal
    ) noexcept
    : _m_matcher_internal(_a_matcher_internal)
{}

template <bool Has_Annotation>
template <typename>
__constexpr_imp
    matcher_wrapper_t<Has_Annotation>::matcher_wrapper_t(
        const matcher_wrapper_t<false>& _a_matcher,
        const std::string_view          _a_annotation
    ) noexcept
    : _m_matcher_internal(
          std::make_shared<matcher_base_t>(_a_matcher.internal_matcher())

      )
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
    : _m_matcher_internal(
          make_shared<matcher_base_t>(_a_matcher.internal_matcher())

      )
    , _m_annotation(_a_matcher._m_annotation)
{
    _m_matcher_internal->add_source_info(_a_macro_str, _a_matcher_str, _a_sl);
}

template <bool Has_Annotation>
template <typename>
__constexpr_imp const std::string_view
    matcher_wrapper_t<Has_Annotation>::annotation() const noexcept
{
    return _m_annotation;
}

template <bool Has_Annotation>
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
}

template <bool Has_Annotation>
__no_constexpr_imp bool
    matcher_wrapper_t<Has_Annotation>::or_statement(
        const ds::single_source_t& _a_single_source,
        test_runner_t&             _a_test_runner
    ) noexcept
{
    const bool _l_result{_m_matcher_internal->matcher_result().passed()};
    *this = make_matcher(new logic_matcher_t<OR>(
        this->internal_matcher(), std::shared_ptr<matcher_base_t>()
    ));
    _m_matcher_internal->add_source_info(_a_single_source);
    return not _l_result;
}

template <bool Has_Annotation>
__no_constexpr_imp bool
    matcher_wrapper_t<Has_Annotation>::and_statement(
        const ds::single_source_t& _a_single_source,
        test_runner_t&             _a_test_runner
    ) noexcept
{
    const bool _l_result{_m_matcher_internal->matcher_result().passed()};
    *this = make_matcher(new logic_matcher_t<AND>(
        this->internal_matcher(), std::shared_ptr<matcher_base_t>()
    ));
    _m_matcher_internal->add_source_info(_a_single_source);
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
        this->_m_matcher_internal->add_source_info(_a_single_source);
    }
}

template <bool Has_Annotation>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator!() const noexcept
{
    return matcher_wrapper_t<Has_Annotation>(
        matcher_base_ptr_t(new logic_matcher_t<NOT>(this->internal_matcher()))
    );
}

template <bool Has_Annotation>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator&&(
        const matcher_wrapper_t& _a_matcher
    ) const noexcept
{
    return make_matcher(new logic_matcher_t<AND>(
        this->internal_matcher(), _a_matcher.internal_matcher()
    ));
}

template <bool Has_Annotation>
__no_constexpr_imp matcher_wrapper_t<Has_Annotation>
                   matcher_wrapper_t<Has_Annotation>::operator||(
        const matcher_wrapper_t& _a_matcher
    ) const noexcept
{
    return make_matcher(new logic_matcher_t<OR>(
        this->internal_matcher(), _a_matcher.internal_matcher()
    ));
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

__no_constexpr_imp matcher_t
    make_matcher(
        matcher_base_t* _a_generic_matcher_ptr
    ) noexcept
{
    return matcher_t(matcher_base_ptr_t(_a_generic_matcher_ptr));
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

__no_constexpr_imp  matcher_wrapper_t<true>
    annotate_matcher(
        const  matcher_wrapper_t<false>&       _a_matcher,
        const std::string_view _a_annotation
    ) noexcept
{
    return matcher_wrapper_t<true>(_a_matcher, _a_annotation);
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