#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <vector>

_BEGIN_ABC_UTILITY_NS

namespace detail
{
/*!
 * @brief A concept for describing ranges which can be used with the
 * static_cast_range function.
 */
template <typename New_Type, typename Original_Type>
concept static_cast_range_c
    // Check both are ranges
    = std::ranges::range<New_Type>
      && std::ranges::range<Original_Type>
      // Check the New_Type has an inserter
      && has_inserter_c<New_Type>
      // Check static cast works
      && requires (std::ranges::range_value_t<Original_Type> _a_object) {
             {
                 static_cast<std::ranges::range_value_t<New_Type>>(_a_object)
             } -> std::same_as<std::ranges::range_value_t<New_Type>>;
         };
/*!
 * @brief Returns either a std::inserter, std::back_inserter or
 * std::front_inserter. If the template cannot issue any of these objects, the
 * function fails to compile.
 */
template <typename T>
requires has_inserter_c<T>
__constexpr auto
    get_inserter(T& _a_range) noexcept;
} // namespace detail

template <typename R, typename R1, typename R2>
requires std::ranges::range<R> && std::ranges::range<R1>
         && std::ranges::range<R2>
         && std::same_as<
             std::ranges::range_value_t<R>,
             std::ranges::range_value_t<R1>>
         && std::same_as<
             std::ranges::range_value_t<R1>,
             std::ranges::range_value_t<R2>>
__constexpr R
    join(const R1& _a_vect1, const R2& _a_vect_2) noexcept;
template <typename R1, typename R2>
requires std::ranges::range<R1> && std::ranges::range<R2>
         && std::same_as<
             std::ranges::range_value_t<R1>,
             std::ranges::range_value_t<R2>>
__constexpr void
    append_range(R1& _a_range1, R2&& _a_range2) noexcept;
template <typename New_Type, typename Original_Type>
requires detail::static_cast_range_c<New_Type, Original_Type>
__constexpr New_Type
    static_cast_range(const Original_Type& _a_vect) noexcept;
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

namespace detail
{
template <typename T>
requires has_inserter_c<T>
__constexpr_imp auto
    get_inserter(
        T& _a_range
    ) noexcept
{
    using namespace std;
    if constexpr (has_back_inserter_c<T>)
    {
        return back_inserter(_a_range);
    }
    else if constexpr (has_front_inserter_c<T>)
    {
        return front_inserter(_a_range);
    }
    else if constexpr (has_unordered_inserter_c<T>)
    {
        return inserter(_a_range, end(_a_range));
    }
    else
    {
        __STATIC_ASSERT(T, "Type does not have an inserter defined for it");
    }
}
} // namespace detail

template <typename R, typename R1, typename R2>
requires std::ranges::range<R> && std::ranges::range<R1>
         && std::ranges::range<R2>
         && std::same_as<
             std::ranges::range_value_t<R>,
             std::ranges::range_value_t<R1>>
         && std::same_as<
             std::ranges::range_value_t<R1>,
             std::ranges::range_value_t<R2>>
__constexpr_imp R
    join(
        const R1& _a_r1,
        const R2& _a_r2
    ) noexcept
{
    using namespace std;
    R _l_rv;
    if constexpr (same_as<R, R1>)
    {
        R _l_rv = _a_r1;
    }
    else
    {
        append_range(_l_rv, _a_r1);
    }
    append_range(_l_rv, _a_r2);
    return _l_rv;
}

template <typename R1, typename R2>
requires std::ranges::range<R1> && std::ranges::range<R2>
         && std::same_as<
             std::ranges::range_value_t<R1>,
             std::ranges::range_value_t<R2>>
__constexpr void
    append_range(
        R1&  _a_range1,
        R2&& _a_range2
    ) noexcept
{
    if constexpr (has_append_range_c<R1,R2>)
    {
        _a_range1.append_range(_a_range2);
    }
    else if constexpr (has_inserter_c<R1>)
    {
        auto _l_inserter{detail::get_inserter(_a_range1)};
        for (auto&& _l_range2_element : _a_range2)
        {
            _l_inserter = _l_range2_element;
        }
    }
    else
    {
        __STATIC_ASSERT(R1, "append_range not defined for this type");
    }
}

template <typename New_Type, typename Original_Type>
requires detail::static_cast_range_c<New_Type, Original_Type>
__constexpr New_Type
    static_cast_range(
        const Original_Type& _a_vect
    ) noexcept
{
    using namespace std;
    using Original_Type_Value = ranges::range_value_t<Original_Type>;
    using New_Type_Value      = ranges::range_value_t<New_Type>;
    New_Type _l_rv;
    if constexpr (sized_and_reservable_range_c<New_Type>)
    {
        _l_rv.reserve(ranges::size(_a_vect));
    }
    ranges::transform(
        _a_vect,
        detail::get_inserter(_l_rv),
        [](const Original_Type_Value& _a_obj)
        {
            return static_cast<New_Type_Value>(_a_obj);
        }
    );
    return _l_rv;
}

_END_ABC_UTILITY_NS