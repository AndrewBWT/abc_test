#pragma once
#include "abc_test/core/errors/abc_test_error.hpp"

#include <stdexcept>

_BEGIN_ABC_ERRORS_NS

/*!
 * Exception type for errors encountered when running the test harness. To be
 * clear, these aren't created by user code. Rather, thse are errors through
 * inproper use, or developer errors.
 */
struct abc_test_exception_t : public std::exception
{
public:
    __constexpr
    abc_test_exception_t()
        = delete;
    __no_constexpr
        abc_test_exception_t(const abc_test_error_t& _a_internal_error
        ) noexcept;
    __no_constexpr
        abc_test_exception_t(
            const std::vector<std::u8string>& _a_error_strings,
            const bool             _a_error_terminates_test_framework,
            const std::stacktrace& _a_stacktrace = std::stacktrace::current()
        ) noexcept;
    __no_constexpr virtual const char*
        what() const noexcept override;
    __constexpr const abc_test_error_t&
        internal_error() const noexcept;
private:
    abc_test_error_t _m_internal_error;
    mutable char*             _m_internal_pointer = nullptr;
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
__constexpr abc_test_exception_t
    unaccounted_for_enum_exception(T _a_integer_value) noexcept;
/*!
 * @brief Function to stylize and create a test_library_exception_t object for
 * when an unaccounted for variant is found.
 * @tparam T The type of the variahnt.
 * @param _a_variant The variant itself.
 * @return A test_library_exception_t type describing the error.
 */
template <typename T>
__constexpr abc_test_exception_t
    unaccounted_for_variant_exception(T _a_variant) noexcept;
/*!
 * @brief Function to stylize and create a test_library_exception_t object for
 * when a pointer is found to be null.
 * @tparam T The type of the pointer.
 * @param _a_ptr The nullptr.
 * @return A test_library_exception_t type describing the error.
 */
template <typename T>
__constexpr abc_test_exception_t
    unaccounted_for_nullptr(const T* _a_ptr) noexcept;
_END_ABC_ERRORS_NS

_BEGIN_ABC_ERRORS_NS

__no_constexpr_imp
    abc_test_exception_t::abc_test_exception_t(
        const abc_test_error_t& _a_internal_error
    ) noexcept
    : _m_internal_error(_a_internal_error)
{}

__no_constexpr_imp
    abc_test_exception_t::abc_test_exception_t(
        const std::vector<std::u8string>& _a_error_strings,
        const bool                        _a_error_terminates_test_framework,
        const std::stacktrace&            _a_stacktrace
    ) noexcept
    : abc_test_exception_t(abc_test_error_t(
          _a_error_strings,
          _a_error_terminates_test_framework,
          _a_stacktrace
      ))
{}

__no_constexpr_imp const char*
    abc_test_exception_t::what() const noexcept
{
    using namespace std;
    if (_m_internal_pointer == nullptr)
    {
        u8string _l_str{_m_internal_error.make_single_str()};
        _m_internal_pointer = new char[_l_str.size() + 1];
        std::memcpy(_m_internal_pointer, _l_str.c_str(), _l_str.size() + 1);
    }
    return _m_internal_pointer;
}

__constexpr const abc_test_error_t&
    abc_test_exception_t::internal_error() const noexcept
{
    return _m_internal_error;
}

template <typename T>
requires std::is_enum_v<T>
__constexpr_imp abc_test_exception_t
    unaccounted_for_enum_exception(
        T _a_integer_value
    ) noexcept
{
    using namespace _ABC_NS_UTILITY_STR;
    return abc_test_exception_t(
        {fmt::format(
            u8"Switch does not contain enum value. Enum type = {0}, underlying "
            u8"value = {1}",
            type_id<T>(),
            std::to_underlying(_a_integer_value)
        )},
        true
    );
}

template <typename T>
__constexpr_imp abc_test_exception_t
    unaccounted_for_variant_exception(
        T _a_variant
    ) noexcept
{
    using namespace _ABC_NS_UTILITY_STR;
    return abc_test_exception_t(
        {fmt::format(
            u8"Variant's type is not accounted for. Variant has type {0}",
            type_id<T>()
        )},
        true
    );
}

template <typename T>
__constexpr abc_test_exception_t
    unaccounted_for_nullptr(
        const T* _a_ptr
    ) noexcept
{
    using namespace _ABC_NS_UTILITY_STR;
    return abc_test_exception_t(
        {fmt::format(
            u8"Unaccounted for nullptr encountered. Type is {0}", type_id<T>()
        )},
        true
    );
}

_END_ABC_ERRORS_NS