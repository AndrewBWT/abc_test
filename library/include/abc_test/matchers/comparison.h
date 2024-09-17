#pragma once

#include "abc_test/matchers/comparison/operators.h"

#define EXPR(Expr) (abc::placeholder_t("EXPR") < Expr)
#define _CHECK_EXPR(Expr) _INTERNAL_ASSERT(abc::placeholder_t() < Expr, false, true, "_CHECK_EXPR")
