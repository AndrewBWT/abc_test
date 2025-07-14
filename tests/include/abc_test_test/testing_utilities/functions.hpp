#pragma once
#include <ranges>

namespace abc_test::utility
{
    /*!
     * @brief Concept for a container which has three functions; reserve, capacity
     * and max_size. In essence, it describes a reiszable vector-like object. In the
     * STD, this concept is met by std::basic_string and std::vector.
     */
    template <typename T>
    concept sized_and_reservable_range_c
        = std::ranges::sized_range<T>
        && requires (T & _a_range, const std::ranges::range_size_t<T> _a_size) {
            { _a_range.reserve(_a_size) };
            {
                _a_range.capacity()
            } -> std::same_as<std::ranges::range_size_t<T>>;
            {
                _a_range.max_size()
            } -> std::same_as<std::ranges::range_size_t<T>>;
    };
    template <typename New_Type, typename Original_Type>
    New_Type
        static_cast_range(
            const Original_Type& _a_vect
        ) noexcept
    {
        using namespace std;
        using Original_Type_Value = ranges::range_value_t<Original_Type>;
        using New_Type_Value = ranges::range_value_t<New_Type>;
        New_Type _l_rv;
        if constexpr (sized_and_reservable_range_c<New_Type>)
        {
            _l_rv.reserve(ranges::size(_a_vect));
        }
        //ranges::transform(
        //    _a_vect,
        //    detail::get_inserter(_l_rv),
       //     [](const Original_Type_Value& _a_obj)
       //     {
       //         return static_cast<New_Type_Value>(_a_obj);
       //     }
      //  );
        return _l_rv;
    }

}