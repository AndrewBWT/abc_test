#pragma once

#include "abc_test/utility/internal/macros.h"
#include <functional>

_BEGIN_ABC_DS_NS
using test_failure_func_t = std::function<ds::repetition_data_t()>;
_END_ABC_DS_NS