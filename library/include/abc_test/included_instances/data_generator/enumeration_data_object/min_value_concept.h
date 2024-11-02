#pragma once
#include "abc_test/included_instances/data_generator/enumeration_data_object/min_value_object.h"

#include <concepts>

_BEGIN_ABC_DG_NS

template <typename T>
concept min_value_c = requires (const min_value_t<T>&_l_mvt) {
    { _l_mvt.min_value() } -> std::same_as<T>;
};
_END_ABC_DG_NS