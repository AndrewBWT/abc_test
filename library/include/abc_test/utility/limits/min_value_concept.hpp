#pragma once
#include "abc_test/utility/limits/min_value_object.hpp"

#include <concepts>

_BEGIN_ABC_UTILITY_NS

template <typename T>
concept has_min_value_expr = requires {
    min_value_t<T>().min_value();
};

template <typename T>
concept returns_exact_type = requires {
    requires std::same_as<decltype(min_value_t<T>().min_value()), T>;
};

template <typename T>
concept min_value_c = has_min_value_expr<T> && returns_exact_type<T>;

/*template <typename T>
concept min_value_c = requires {
    { min_value_t<T>().min_value() } -> std::same_as<T>;
};*/
_END_ABC_UTILITY_NS