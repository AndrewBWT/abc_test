#pragma once

#include "abc_test/matchers/comparison/operators.h"

#define _EXPR(Expr) (abc::placeholder_t("EXPR") < Expr)
#define _CHECK_EXPR(Expr) _CHECK(_EXPR(Expr))
#define _REQUIRE_EXPR(Expr) _REQUIRE(_EXPR(Expr))
//#define _CHECK_EXPR(Expr) _INTERNAL_ASSERT(abc::placeholder_t() < Expr, false, "_CHECK_EXPR")
//#define _REQUIRE_EXPR(Expr) _INTERNAL_ASSERT(abc::placeholder_t() < Expr, true, "_REQUIRE_EXPR")
