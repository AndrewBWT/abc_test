#pragma once
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_MATCHER_NS
	struct placeholder_t
	{
	public:
		/*!
		* Default constructor for a placeholder object.
		*/
		__constexpr
			placeholder_t(
			) noexcept;
		/*!
		* A special constructor. This form is used within several constructors to store a specific
		* string, which is extracted out and used to augment given source code so that when a line is reported
		* as failing a test, the exact line is repeated to the user of the library.
		*/
		__constexpr
			placeholder_t(
				const std::string_view _a_str
			) noexcept;
		__constexpr
			bool
			operator==(
				const placeholder_t&
				) const = default;
	};
_END_ABC_NS
_END_NS

_BEGIN_ABC_MATCHER_NS
	__constexpr_imp
		placeholder_t::placeholder_t(
		) noexcept
	{

	}
	__constexpr_imp
		placeholder_t::placeholder_t(
			const std::string_view _a_str
		) noexcept
	{

	}
_END_ABC_NS
_END_NS