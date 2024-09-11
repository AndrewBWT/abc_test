#pragma once
#include "abc_test/utility/internal/macros.h"
#include <concepts>

_BEGIN_ABC_GD_NS
	template<
		typename T
	>
	struct max_value_t
	{
		__constexpr
			T
			max_value(
			) const noexcept;
	};
	template<
		typename T
	>
	concept max_value_c = requires(const max_value_t<T>&_l_mvt)
	{
		{ _l_mvt.max_value() } -> std::same_as<T>;
	};
	_END_ABC_NS
		_END_NS

		_BEGIN_ABC_GD_NS
	template<
		typename T
	>
	__constexpr_imp
		T
		max_value_t<T>::max_value(
		) const noexcept
	{
		static_assert(false, "Function requires a specialization of the function max_value_t<T>::max_value.");
	}
	_END_ABC_NS
		_END_NS