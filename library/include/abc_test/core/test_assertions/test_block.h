#pragma once
#include "abc_test/core/ds/source/single_source.h"
#include "abc_test/core/matchers/matcher_wrapper.h"
#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/matchers/static_matcher.h"

#include <optional>

_BEGIN_ABC_NS

/*!
 * @brief Structure used when creating test blocks; testing apparatus in which
 * an assertion is declared in a block, and its only checked when destroyed.
 * @tparam Assertion_Type The assertion type. Must be derived from
 * dynamic_status_t.
 */
template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
class test_block_t
{
public:
    __constexpr
    test_block_t() noexcept
        = delete;
    /*!
     * @brief Constructor
     * @param _a_test_annotation The block's annotation.
     * @param _a_source The blocks' beginning source.
     */
    __constexpr
    test_block_t(
        const std::string_view     _a_test_annotation,
        const ds::single_source_t& _a_source
    ) noexcept;
    /*!
     * @brief Destructor.
     *
     * The destructor will issue a warning to the test_runner_t if its matcher
     * has not been checked. If the user is using the macros, this will be done
     * when ending a test block.
     */
    __constexpr ~test_block_t() noexcept;
    /*!
     * @brief Equals operator taking a matcher_wrapper_t. This is used when
     * wanting to attach a matcher_wrapper_t to this object.
     * @tparam Boolean denoting whether the argument matcher_wrapper_t has an
     * annotation.
     * @param _a_element The matcher_wrapper_t to attach to this object.
     * @return Reference to test_block_t with matcher_wrapper_t attached.
     */
    template <bool Has_Annotation>
    __constexpr_imp test_block_t<Assertion_Type>&
        operator=(const matcher_wrapper_t<Has_Annotation>& _a_element) noexcept;
    /*!
     * @brief Returns a cref to the object's matcher_base_ptr_t to the caller.
     * @return A cref to the object's matcher_base_ptr_t.
     */
    __constexpr const matcher_base_ptr_t&
        matcher() const noexcept;
    /*!
     * @brief Returns a cref to the object's optional test annotation.
     * @return A cref to the object's optional test annotation.
     */
    __constexpr const std::optional<std::string>&
                      test_annotation() const noexcept;
    /*!
     * @brief Returns a cref to the object's optional matcher annotation.
     * @return A cref to the object's optional matcher annotation.
     */
    __constexpr const std::optional<std::string>&
                      matcher_annotation() const noexcept;
    /*!
     * @brief Registers an end source with the object.
     *
     * This should be called by the macro _END_BLOCK.
     *
     * @param _a_end_source The source location being registered to this object.
     */
    __constexpr void
        register_end(const ds::single_source_t& _a_end_source) noexcept;
    /*!
     * @brief Gets a cref to the object's ds::source_pair_t.
     * @return A cref to the object's ds::source_pair_t.
     */
    __constexpr const ds::source_pair_t&
                      source() const noexcept;
    /*!
     * @brief Tells the caller whether the object has been processed - that is,
     * the function register_end called on the object.
     * @return True if the object has been processed; false otherwise.
     */
    __constexpr void
        set_processed() noexcept;
private:
    ds::source_pair_t          _m_source;
    std::optional<std::string> _m_test_annotation;
    std::optional<std::string> _m_matcher_annotation;
    matcher_base_ptr_t         _m_matcher;
    bool                       _m_processed;
};

_END_ABC_NS

_BEGIN_ABC_NS
template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp
    test_block_t<Assertion_Type>::test_block_t(
        const std::string_view     _a_test_annotation,
        const ds::single_source_t& _a_source
    ) noexcept
    : _m_source(ds::source_pair_t(_a_source))
    , _m_test_annotation(_a_test_annotation)
    , _m_processed(false)
    , _m_matcher(matcher_base_ptr_t(new static_matcher_t<reports::pass_t>()))
{}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp test_block_t<Assertion_Type>::~test_block_t() noexcept
{
    using namespace global;
    using namespace std;
    using namespace reports;
    if (not _m_processed)
    {
        get_this_threads_test_runner_ref().add_text_warning(fmt::format(
            "test_block_t is being deleted, however it has not been "
            "processed for its true or false status. "
        ));
    }
}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
template <bool Has_Annotation>
__constexpr_imp test_block_t<Assertion_Type>&
                test_block_t<Assertion_Type>::operator=(
        const matcher_wrapper_t<Has_Annotation>& _a_element
    ) noexcept
{
    this->_m_matcher = _a_element.internal_matcher();
    if constexpr (Has_Annotation)
    {
        this->_m_matcher_annotation
            = std::optional<std::string>(_a_element.annotation());
    }
    else
    {
        this->_m_matcher_annotation = std::optional<std::string>();
    }
    return *this;
}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp const matcher_base_ptr_t&
    test_block_t<Assertion_Type>::matcher() const noexcept
{
    return _m_matcher;
}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp const std::optional<std::string>&
    test_block_t<Assertion_Type>::test_annotation() const noexcept
{
    return _m_test_annotation;
}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp const std::optional<std::string>&
    test_block_t<Assertion_Type>::matcher_annotation() const noexcept
{
    return _m_matcher_annotation;
}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp void
    test_block_t<Assertion_Type>::register_end(
        const ds::single_source_t& _a_end_source
    ) noexcept
{
    _m_source.set_end_source(_a_end_source);
}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp const ds::source_pair_t&
                      test_block_t<Assertion_Type>::source() const noexcept
{
    using namespace reports;
    return _m_source;
}

template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, reports::dynamic_status_t>
__constexpr_imp void
    test_block_t<Assertion_Type>::set_processed() noexcept
{
    _m_processed = true;
}

_END_ABC_NS
