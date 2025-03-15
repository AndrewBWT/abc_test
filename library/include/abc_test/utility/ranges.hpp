#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <vector>

_BEGIN_ABC_UTILITY_NS
template <typename T>
__constexpr std::vector<T>
    join(const std::vector<T>& _a_vect1, const std::vector<T>& _a_vect_2)
        noexcept;
template <typename New_Type, typename Original_Type>
__constexpr New_Type
    convert_type(const Original_Type& _a_vect) noexcept;
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
__constexpr New_Type
    convert_type(
        const Original_Type& _a_vect
    ) noexcept
{
    using namespace std;
    using Original_Type_Value = std::ranges::range_value_t<Original_Type>;
    using New_Type_Value      = std::ranges::range_value_t<New_Type>;
    auto _l_to_new_type       = [](const Original_Type_Value& _a_obj)
    {
        return static_cast<New_Type_Value>(_a_obj);
    };
    New_Type _l_rv(std::ranges::size(_a_vect), New_Type_Value{});
    transform(begin(_a_vect), end(_a_vect), begin(_l_rv), _l_to_new_type);
    return _l_rv;
}

_END_ABC_UTILITY_NS