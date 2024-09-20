
#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/core/reporters/mid_execution_test_warning.h"

#include <source_location>
#include <string>

_BEGIN_ABC_REPORTERS_NS
class malformed_matcher_warning_t : public mid_execution_test_warning_t
{
public:
	__constexpr
		malformed_matcher_warning_t(
			const std::string_view _a_str_representation,
			const std::source_location& _a_source_location
		) noexcept;
};
_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS
__constexpr_imp
	malformed_matcher_warning_t::malformed_matcher_warning_t(
		const std::string_view _a_str_representation,
		const std::source_location& _a_source_location
) noexcept
{

}
_END_ABC_REPORTERS_NS