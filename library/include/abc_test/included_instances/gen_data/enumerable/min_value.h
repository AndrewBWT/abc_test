#pragma once

#include "abc_test/internal/utility/internal/macros.h"
#include <concepts>

_BEGIN_ABC_NS
	template<
		typename T
	>
	struct min_value_t
	{
		__constexpr
			T
			min_value(
			) const noexcept;
	};
	template<
		typename T
	>
	concept min_value_c = requires(const min_value_t<T>&_l_mvt)
	{
		{ _l_mvt.min_value() } -> std::same_as<T>;
	};
	_END_ABC_NS

		_BEGIN_ABC_NS
	template<
		typename T
	>
	__constexpr_imp
		T
		min_value_t<T>::min_value(
		) const noexcept
	{
		__STATIC_ASSERT(T, "Function requires a specialization of the function min_value_t<T>::min_value.");
	}
	_END_ABC_NS