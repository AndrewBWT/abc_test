#pragma once
#include "abc_test/included_instances/data_generator/enumeration_data_object/max_value_object.h"

#include <concepts>

_BEGIN_ABC_DG_NS

template <typename T>
concept max_value_c = requires (const max_value_t<T>& _l_mvt) {
    { _l_mvt.max_value() } -> std::same_as<T>;
};
_END_ABC_DG_NS