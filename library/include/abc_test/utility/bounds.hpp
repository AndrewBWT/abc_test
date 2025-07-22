#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/max_value_object.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"
#include "abc_test/utility/limits/min_value_object.hpp"
#include "abc_test/utility/printers/default_printer.hpp"

#include <limits>

_BEGIN_ABC_UTILITY_NS

namespace detail
{
/*!
 * @brief This concept denotes the requirements ont he tamplte type for the
 * bounds_t object.
 */
template <typename T>
concept bounds_c = std::integral<T>;
} // namespace detail

/*!
 * @brief An object used to describe the bounds on some templated type. It works
 * out automatically which is the minimum and maximum value, and calculates the
 * difference between the two values.
 * @tparam T
 */
template <typename T>
requires detail::bounds_c<T>
class bounds_t
{
public:
    /*!
     * @brief Default constructor for bounds. Will set the lower value at
     * minvalue_t<T>.min_value() and the max value at
     * max_value_t<T>.max_value().
     */
    __constexpr
    bounds_t() noexcept;
    // requires min_value_c<T> && max_value_c<T>;
    /*!
     * @brief Creates a bounds_t object where the min and max value are set at
     * the same element.
     * @param _a_first_and_second The element to set the min and max values at.
     */
    __constexpr
    bounds_t(const T& _a_first_and_second) noexcept;
    /*!
     * @brief The most commonly used bounds_t constructor.
     *
     * The minimum, maximum and difference values are worked out by comparing
     * the two elements
     *
     * @param _a_first One of the elements to set.
     * @param _a_second The other element to set.
     */
    __constexpr
    bounds_t(const T& _a_first, const T& _a_second) noexcept;
    /*!
     * @brief Gets the minimum of the contained values.
     * @return The minimum of the contained values.
     */
    __constexpr const T&
        lower() const noexcept;
    /*!
     * @brief Gets the maximum of the contained values.
     * @return The maximum of the contained values.
     */
    __constexpr const T&
        higher() const noexcept;
    /*!
     * @brief Gets the differnece between the contained values.
     * @return The difference between the contained values.
     */
    __constexpr const std::size_t&
                      difference() const noexcept;
private:
    T           _m_min;
    T           _m_max;
    std::size_t _m_difference;
    enum class bounds_ordering_e
    {
        first_less_than_second,
        second_less_than_first,
    };
    __constexpr
    bounds_t(
        const T&                _a_first,
        const T&                _a_second,
        const bounds_ordering_e _a_ordering_enum
    ) noexcept;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct default_printer_t<bounds_t<T>> : public printer_base_t<bounds_t<T>>
{
    static constexpr bool is_specialized{true};

    __constexpr           std::u8string
                          run_printer(
                              const bounds_t<T>& _a_object
                          ) const
    {
        using namespace _ABC_NS_UTILITY_STR;
        return object_printer_with_field_names(
            object_printer_parser_t{},
            type_id<bounds_t<T>>(),
            {u8"lower", u8"higher", u8"difference"},
            _a_object.lower(),
            _a_object.higher(),
            _a_object.difference()
        );
    }
};

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_NS
template <typename T>
requires detail::bounds_c<T>
__constexpr_imp
    bounds_t<T>::bounds_t() noexcept
    // requires min_value_c<T> && max_value_c<T>
    : bounds_t(min_value_t<T>{}.min_value(), max_value_t<T>{}.max_value())
{}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp
    bounds_t<T>::bounds_t(
        const T& _a_first_and_second
    ) noexcept
    : _m_min(_a_first_and_second), _m_max(_a_first_and_second), _m_difference(0)
{}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp
    bounds_t<T>::bounds_t(
        const T& _a_first,
        const T& _a_second
    ) noexcept
    : bounds_t(
          _a_first,
          _a_second,
          _a_first < _a_second ? bounds_ordering_e::first_less_than_second
                               : bounds_ordering_e::second_less_than_first
      )
{}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp const T&
    bounds_t<T>::lower() const noexcept
{
    return _m_min;
}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp const T&
    bounds_t<T>::higher() const noexcept
{
    return _m_max;
}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp const std::size_t&
                      bounds_t<T>::difference() const noexcept

{
    return _m_difference;
}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp
    bounds_t<T>::bounds_t(
        const T&                _a_first,
        const T&                _a_second,
        const bounds_ordering_e _a_ordering_enum
    ) noexcept
    : _m_min(
          _a_ordering_enum == bounds_ordering_e::first_less_than_second
              ? _a_first
              : _a_second
      )
    , _m_max(
          _a_ordering_enum == bounds_ordering_e::first_less_than_second
              ? _a_second
              : _a_first
      )
    , _m_difference(
          _a_ordering_enum == bounds_ordering_e::first_less_than_second
              ? static_cast<size_t>(_a_second) - static_cast<size_t>(_a_first)
              : static_cast<size_t>(_a_first) - static_cast<size_t>(_a_second)
      )
{}

_END_ABC_UTILITY_NS
