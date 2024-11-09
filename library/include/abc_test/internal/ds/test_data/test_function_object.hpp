#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"
#include <memory>

_BEGIN_ABC_DS_NS

class test_function_object_t
{
public:
    __constexpr virtual void
        run() const
        = 0;
};

using test_function_t = std::shared_ptr<test_function_object_t>;
_END_ABC_DS_NS