#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <exception>
#include <fmt/ranges.h>
#include <optional>
#include <stacktrace>
#include <type_traits>
#include "abc_test/utility/types.hpp"

#include <fmt/xchar.h>

_BEGIN_ABC_ERRORS_NS

/*!
 * Exception type for errors encountered when running the test harness. To be
 * clear, these aren't created by user code. Rather, thse are errors through
 * inproper use, or developer errors.
 */
struct test_library_exception_t : public std::runtime_error
{
public:
    __constexpr
    test_library_exception_t()
        = delete;
    /*!
     * @brief Constructor.
     * @param _a_error Text representation of the error.
     * @param _a_stacktrace Stacktrace of where the error originated from.
     */
    __no_constexpr
        test_library_exception_t(
            const std::u8string    _a_error,
            const std::stacktrace& _a_stacktrace = std::stacktrace::current()
        ) noexcept;
    /*!
     * @brief Gets a cref to the underlying test_library_exception_t's
     * stacktrace.
     * @return A cref to the underlying test_library_excpetion_t's stacktrace.
     */
    __constexpr const std::stacktrace&
                      stacktrace() const noexcept;
private:
    std::stacktrace _m_stacktrace;
};

/*!
 * @brief Function to stylize and create a test_library_exception_t object for
 * when a switch hits a default case, when it should not be able to.
 * @tparam T The enum type.
 * @param _a_integer_value The enum value.
 * @return A test_library_exception_t type describing the error.
 */
template <typename T>
requires std::is_enum_v<T>
__constexpr test_library_exception_t
    unaccounted_for_enum_exception(T _a_integer_value) noexcept;
/*!
 * @brief Function to stylize and create a test_library_exception_t object for
 * when an unaccounted for variant is found.
 * @tparam T The type of the variahnt.
 * @param _a_variant The variant itself.
 * @return A test_library_exception_t type describing the error.
 */
template <typename T>
__constexpr test_library_exception_t
    unaccounted_for_variant_exception(T _a_variant) noexcept;
/*!
 * @brief Function to stylize and create a test_library_exception_t object for
 * when a pointer is found to be null.
 * @tparam T The type of the pointer.
 * @param _a_ptr The nullptr.
 * @return A test_library_exception_t type describing the error.
 */
template <typename T>
__constexpr test_library_exception_t
    unaccounted_for_nullptr(const T* _a_ptr) noexcept;
_END_ABC_ERRORS_NS

_BEGIN_ABC_ERRORS_NS

__no_constexpr_imp
    test_library_exception_t::test_library_exception_t(
        const std::u8string    _a_error,
        const std::stacktrace& _a_stacktrace
    ) noexcept
    : std::runtime_error(pack_u8string_into_string(_a_error))
    , _m_stacktrace(_a_stacktrace)
{}

__constexpr_imp const std::stacktrace&
                      test_library_exception_t::stacktrace() const noexcept
{
    return _m_stacktrace;
}

template <typename T>
requires std::is_enum_v<T>
__constexpr_imp test_library_exception_t
    unaccounted_for_enum_exception(
        T _a_integer_value
    ) noexcept
{
    return test_library_exception_t(fmt::format(
        u8"Switch does not contain enum value. Enum type = {0}, underlying "
        u8"value = {1}",
        type_id<T>(),
        std::to_underlying(_a_integer_value)
    ));
}

template <typename T>
__constexpr_imp test_library_exception_t
    unaccounted_for_variant_exception(
        T _a_variant
    ) noexcept
{
    return test_library_exception_t(fmt::format(
        u8"Variant's type is not accounted for. Variant has type {0}",
        type_id<T>()
    ));
}

template <typename T>
__constexpr test_library_exception_t
    unaccounted_for_nullptr(
        const T* _a_ptr
    ) noexcept
{
    return test_library_exception_t(fmt::format(
        u8"Unaccounted for nullptr encountered. Type is {0}", 
        type_id<T>()
    ));
}

_END_ABC_ERRORS_NS