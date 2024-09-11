#pragma once

#include "abc_test/matchers/comparison/operators.h"

#define EXPR(Expr) abc::matchers::placeholder_t("EXPR") < Expr
#define _CHECK_EXPR(Expr) _INTERNAL_ASSERT(abc::matchers::placeholder_t() < Expr, false, "_CHECK_EXPR")
