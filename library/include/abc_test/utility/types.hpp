#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include <expected>
#include <string>

_BEGIN_ABC_NS
template<typename T>
using result_t = std::expected<T, std::string>;
_END_ABC_NS