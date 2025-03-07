#pragma once
#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief Wrapper object for an arbitrary type.
 * @tparam T The type of the object to be wrapped.
 */
template <typename T>
struct comparison_wrapper_t
{
public:
    __constexpr
    comparison_wrapper_t() noexcept
        = delete;
    /*!
     * @brief Constructor.
     * @param _a_t The element to be in the container.
     */
    __constexpr
    comparison_wrapper_t(T&& _a_t) noexcept;
    /*!
     * @brief Returns a reference of the internal object to the caller.
     * @return A reference of the internal object to the caller.
     */
    __constexpr T&
        object() noexcept;
private:
    T _m_obj;
};

_END_ABC_MATCHER_NS

_BEGIN_ABC_MATCHER_NS
template <typename T>
__constexpr_imp
    comparison_wrapper_t<T>::comparison_wrapper_t(
        T&& _a_obj
    ) noexcept
    : _m_obj(_a_obj)
{}

template <typename T>
__constexpr_imp T&
    comparison_wrapper_t<T>::object() noexcept
{
    return _m_obj;
}

_END_ABC_MATCHER_NS