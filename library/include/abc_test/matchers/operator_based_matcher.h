#pragma once

#include "abc_test/matchers/generic_matcher.h"

_BEGIN_ABC_NS
using precedence_t = std::size_t;
struct operator_based_matcher_t : public generic_matcher_t
{
public:
	__constexpr
		virtual
		precedence_t
		get_precedence(
		) const noexcept = 0;
};
_END_ABC_NS