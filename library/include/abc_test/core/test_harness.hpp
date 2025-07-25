#pragma once

#include "abc_test/core/global.hpp"

_BEGIN_ABC_NS
class test_harness_t
{
public:
    __constexpr void run_tests() const noexcept;
private:
    size_t                               _m_thread_pool;
};
_END_ABC_NS

_BEGIN_ABC_NS

_END_ABC_NS