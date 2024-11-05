#pragma once
#include "abc_test/internal/utility/limits/min_value_object.h"
#include "abc_test/internal/utility/limits/max_value_object.h"

_BEGIN_ABC_UTILITY_NS

template <typename T>
    requires min_value_c<T>
struct min_value_t<std::vector<T>>
{
    __constexpr_imp std::vector<T>
        min_value() const noexcept;
};
_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

template <typename T>
    requires min_value_c<T>
__constexpr_imp std::vector<T>
min_value_t<std::vector<T>>::min_value() const noexcept
{
    using namespace std;
    vector<T> _l_rv;
    return _l_rv;
}
_END_ABC_UTILITY_NS