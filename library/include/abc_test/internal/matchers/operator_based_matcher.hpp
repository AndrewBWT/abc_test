#pragma once

#include "abc_test/internal/matchers/matcher_base.hpp"

_BEGIN_ABC_MATCHER_NS
/*!
 * @brief Type synonym for precedence type.
 */
using precedence_t = std::size_t;
/*!
 * @brief Abstract object which represents a matcher which models some operator.
 */
struct operator_based_matcher_t : public matcher_base_t
{
public:
	using matcher_base_t::matcher_base_t;
	/*!
	 * @brief Gets the objects precedence_t.
	 * @return The object's precedence_t.
	 */
	__constexpr
		virtual
		precedence_t
		get_precedence(
		) const noexcept = 0;
};
_END_ABC_MATCHER_NS