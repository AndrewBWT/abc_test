#pragma once
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_NS
	template<
		typename T
	>
	struct wrapper
	{
	public:
		constexpr
			wrapper(
			) noexcept;
		constexpr
			wrapper(
				T&& _a_t
			) noexcept;
		constexpr
			T&
			object(
			) noexcept;
	private:
		T _m_obj;
	};
_END_ABC_NS

_BEGIN_ABC_NS
	template<
		typename T
	>
	inline constexpr
		wrapper<T>::wrapper(
		) noexcept
		: _m_obj(T())
	{

	}
	template<
		typename T
	>
	inline constexpr
		wrapper<T>::wrapper(
			T&& _a_obj
		) noexcept
		: _m_obj(_a_obj)
	{

	}
	template<
		typename T
	>
	constexpr
		T&
		wrapper<T>::object(
		) noexcept
	{
		return _m_obj;
	}
_END_ABC_NS