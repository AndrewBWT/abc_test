#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <vector>

_BEGIN_ABC_UTILITY_NS

namespace detail
{
template <typename New_Type, typename Original_Type>
concept statically_convertable_range_c
    // Check both are ranges
    = std::ranges::range<New_Type> && std::ranges::range<Original_Type>
   // && (has_back_inserter_c<New_Type> || has_inserter_c<New_Type>)
    // Check static cast works
      && requires (std::ranges::range_value_t<Original_Type> _a_object) {
             {
                 static_cast<std::ranges::range_value_t<New_Type>>(_a_object)
             } -> std::same_as<std::ranges::range_value_t<New_Type>>;
         };

template <typename T>
__constexpr auto
    get_inserter(
        T& _a_range
    )
{
    if constexpr (has_back_inserter_c<T>)
    {
        return std::back_inserter(_a_range);
    }
    else //if constexpr (has_inserter_c<T>)
    {
        return std::inserter(_a_range, std::end(_a_range));
    }
   // else
   // {
   //     __STATIC_ASSERT(T, "Type does not have an inserter defined for it");
  //  }
}
} // namespace detail

template <typename T>
__constexpr std::vector<T>
    join(const std::vector<T>& _a_vect1, const std::vector<T>& _a_vect_2)
        noexcept;
template <typename New_Type, typename Original_Type>
requires detail::statically_convertable_range_c<New_Type, Original_Type>
__constexpr New_Type
    static_cast_range(const Original_Type& _a_vect) noexcept;
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
template <typename T>
__constexpr_imp std::vector<T>
                join(
                    const std::vector<T>& _a_vect1,
                    const std::vector<T>& _a_vect_2
                ) noexcept
{
    using namespace std;
    vector<T> _l_rv{_a_vect1};
    _l_rv.append_range(_a_vect_2);
    return _l_rv;
}

template <typename New_Type, typename Original_Type>
requires detail::statically_convertable_range_c<New_Type, Original_Type>
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