#pragma once

#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/limits/max_value_object.hpp"
#include "abc_test/utility/limits/min_value_object.hpp"
#include "abc_test/utility/printers/default_printer.hpp"

_BEGIN_ABC_UTILITY_NS

namespace detail
{
template <typename T>
concept bounds_c = std::unsigned_integral<T>; // && (not std::same_as<T, bool>);
} // namespace detail

template <typename T>
requires detail::bounds_c<T>
class bounds_t
{
public:
    __constexpr
    bounds_t();
    __constexpr
    bounds_t(const T& _a_first_and_second) noexcept;
    __constexpr
    bounds_t(const T& _a_first, const T& _a_second) noexcept;
    __constexpr const T&
        lower() const noexcept;
    __constexpr const T&
        higher() const noexcept;
    __constexpr const T&
        difference() const noexcept;
private:
    T _m_min;
    T _m_max;
    T _m_difference;
    enum class bounds_ordering_enum_t
    {
        first_less_than_second,
        second_less_than_first,
    };
    __constexpr
    bounds_t(
        const T&                     _a_first,
        const T&                     _a_second,
        const bounds_ordering_enum_t _a_ordering_enum
    ) noexcept;
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

template <typename T>
struct default_printer_t<bounds_t<T>> : public printer_base_t<bounds_t<T>>
{
    static constexpr bool is_specialized{true};

    __constexpr           std::string
                          run_printer(
                              const bounds_t<T>& _a_object
                          ) const
    {
        return object_printer_with_field_names(
            object_printer_parser_t{},
            type_id<decltype(_a_object)>(),
            {"lower", "higher", "difference"},
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
    bounds_t<T>::bounds_t()
    : bounds_t(min_value_t<T>{}.min_value(), max_value_t<T>{}.max_value())
{}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp
    bounds_t<T>::bounds_t(
        const T& _a_first_and_second
    ) noexcept
    : bounds_t(
          _a_first_and_second,
          _a_first_and_second,
          bounds_ordering_enum_t::first_less_than_second
      )
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
          _a_first < _a_second ? bounds_ordering_enum_t::first_less_than_second
                               : bounds_ordering_enum_t::second_less_than_first
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
__constexpr_imp const T&
    bounds_t<T>::difference() const noexcept

{
    return _m_difference;
}

template <typename T>
requires detail::bounds_c<T>
__constexpr_imp
    bounds_t<T>::bounds_t(
        const T&                     _a_first,
        const T&                     _a_second,
        const bounds_ordering_enum_t _a_ordering_enum
    ) noexcept
    : _m_min(
          _a_ordering_enum == bounds_ordering_enum_t::first_less_than_second
              ? _a_first
              : _a_second
      )
    , _m_max(
          _a_ordering_enum == bounds_ordering_enum_t::first_less_than_second
              ? _a_second
              : _a_first
      )
    , _m_difference(
          _a_ordering_enum == bounds_ordering_enum_t::first_less_than_second
              ? _a_second - _a_first
              : _a_first - _a_second
      )
{}

_END_ABC_UTILITY_NS