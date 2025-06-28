#pragma once
#include "abc_test/utility/internal/macros.hpp"

#include <cuchar>
#include <expected>
#include <fmt/xchar.h>
#include <string>
#include <vector>

_BEGIN_ABC_NS

template <typename T>
using result_t = std::expected<T, std::u8string>;
_END_ABC_NS