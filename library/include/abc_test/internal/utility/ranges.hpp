#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"
#include <vector>

_BEGIN_ABC_UTILITY_NS
template <typename T>
__constexpr std::vector<T>
    join(const std::vector<T>& _a_vect1, const std::vector<T>& _a_vect_2)
        noexcept;
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
template <typename T>
__constexpr_imp std::vector<T>
join(const std::vector<T>& _a_vect1, const std::vector<T>& _a_vect_2)
noexcept
{
    using namespace std;
    vector<T> _l_rv{ _a_vect1 };
    _l_rv.append_range(_a_vect_2);
    return _l_rv;
}
_END_ABC_UTILITY_NS