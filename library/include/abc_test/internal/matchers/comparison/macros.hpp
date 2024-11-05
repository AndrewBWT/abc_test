#pragma once
#include "abc_test/internal/matchers/comparison/operators.hpp"

/*!
* @brief Macro for writing expressions. This macro will return a matcher_t element.
* @param Expr The comparison expression used (e.g. 1==2).
* @return matcher_t type representing the comparison.
*/
#define _EXPR(Expr)         (_ABC_NS_MATCHER::comparison_placeholder_t("EXPR") < Expr)
/*!
* @brief Macro for creating a _CHECK expression which only checks a single expression.
* 
* This creates a full assertion; the underlying matcher_t cannot be extracted.
* 
* @param Expr The comparison expression used.
*/
#define _CHECK_EXPR(Expr)   _CHECK(_EXPR(Expr))
/*!
* @brief Macro for creating a _REQUIRE expression which only checks a single expression.
*
* This creates a full assertion; the underlying matcher_t cannot be extracted.
*
* @param Expr The comparison expression used.
*/
#define _REQUIRE_EXPR(Expr) _REQUIRE(_EXPR(Expr))
