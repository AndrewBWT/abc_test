#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <vector>

_BEGIN_ABC_UTILITY_NS

namespace detail
{

/*!
 * @brief Returns either a std::inserter, std::back_inserter or
 * std::front_inserter. If the template cannot issue any of these objects, the
 * function fails to compile.
 */
template <typename T>
requires has_inserter_c<T>
__constexpr auto
    get_inserter(T& _a_range) noexcept;
/*!
 * @brief Function which appends the second argument to the first. 
 */
template <typename R1, typename R2>
requires std::ranges::range<R1> && std::ranges::range<R2>
         && std::convertible_to<
             std::ranges::range_value_t<R2>,
             std::ranges::range_value_t<R1>>
__constexpr void
    append_range(R1& _a_range1, R2&& _a_range2) noexcept;
} // namespace detail

/*!
 * @brief Function which joins two ranges into a third range.
 * @tparam R The return type.
 * @tparam R1 The type of the first range.
 * @tparam R2 The type of the second range.
 * @param _a_vect1 The first range.
 * @param _a_vect_2 The second range.
 * @return The first and second range combined.
 */
template <typename R, typename R1, typename R2>
requires std::ranges::range<R> && std::ranges::range<R1>
         && std::ranges::range<R2>
         && std::convertible_to<
             std::ranges::range_value_t<R1>,
             std::ranges::range_value_t<R>>
         && std::convertible_to<
             std::ranges::range_value_t<R2>,
             std::ranges::range_value_t<R>>
__constexpr R
    join(R1&& _a_vect1, R2&& _a_vect_2) noexcept;
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

template <typename R1, typename R2>
requires std::ranges::range<R1> && std::ranges::range<R2>
         && std::convertible_to<
             std::ranges::range_value_t<R2>,
             std::ranges::range_value_t<R1>>
__constexpr void
    append_range(
        R1&  _a_range1,
        R2&& _a_range2
    ) noexcept
{
    using namespace std;
    if constexpr (has_append_range_c<R1, R2>)
    {
        _a_range1.append_range(std::forward<R2>(_a_range2));
    }
    else if constexpr (has_inserter_c<R1>)
    {
        auto _l_inserter{detail::get_inserter(_a_range1)};
        for (auto&& _l_range2_element : _a_range2)
        {
            _l_inserter = (_l_range2_element);
        }
    }
    else
    {
        __STATIC_ASSERT(R1, "append_range not defined for this type");
    }
}
} // namespace detail

template <typename R, typename R1, typename R2>
requires std::ranges::range<R> && std::ranges::range<R1>
         && std::ranges::range<R2>
         && std::convertible_to<
             std::ranges::range_value_t<R1>,
             std::ranges::range_value_t<R>>
         && std::convertible_to<
             std::ranges::range_value_t<R2>,
             std::ranges::range_value_t<R>>
__constexpr_imp R
    join(
        R1&& _a_r1,
        R2&& _a_r2
    ) noexcept
{
    using namespace std;
    R _l_rv;
    if constexpr (same_as<R, R1>)
    {
        _l_rv = forward<R1>(_a_r1);
    }
    else
    {
        detail::append_range(_l_rv, std::forward<R1>(_a_r1));
    }
    detail::append_range(_l_rv, std::forward<R2>(_a_r2));
    return _l_rv;
}

_END_ABC_UTILITY_NS