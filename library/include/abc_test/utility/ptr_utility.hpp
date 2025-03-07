#pragma once
#include <variant>
#include <memory>
#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_UTILITY_NS
	template<
		typename T
	>
	using shared_and_raw_ptr = std::variant<T*, std::shared_ptr<T>>;
	template<
		typename T
	>
	using const_shared_and_raw_ptr = std::variant<const T*, std::shared_ptr<const T>>;
	template<
		typename T
	>
	using const_ptr_t = const T*;
	template<
		typename T
	>
	__constexpr
		T*
		get_ptr(
			shared_and_raw_ptr<T>& _a_ptr
		) noexcept;
	template<
		typename T
	>
	__constexpr
		T*
		get_ptr(
			const shared_and_raw_ptr<T>& _a_ptr
		) noexcept;
	template<
		typename T
	>
	__constexpr
		const T*
		get_ptr(
			const_shared_and_raw_ptr<T>& _a_ptr
		) noexcept;
	template<
		typename T
	>
	__constexpr
		const T*
		get_ptr(
			const const_shared_and_raw_ptr<T>& _a_ptr
		) noexcept;
	_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS
	template<
		typename T
	>
	__constexpr_imp
		T*
		get_ptr(
			shared_and_raw_ptr<T>& _a_ptr
		) noexcept
	{
		using namespace std;
		return holds_alternative< shared_ptr<T>>(_a_ptr) ?
			get<shared_ptr<T>>(_a_ptr).get() :
			get< T*>(_a_ptr);
	}
	template<
		typename T
	>
	__constexpr_imp
		T*
		get_ptr(
			const shared_and_raw_ptr<T>& _a_ptr
		) noexcept
	{
		using namespace std;
		return holds_alternative< shared_ptr<T>>(_a_ptr) ?
			get<shared_ptr<T>>(_a_ptr).get() :
			get< T*>(_a_ptr);
	}
	template<
		typename T
	>
	__constexpr_imp
		const T*
		get_ptr(
			const_shared_and_raw_ptr<T>& _a_ptr
		) noexcept
	{
		using namespace std;
		return holds_alternative< shared_ptr<const T>>(_a_ptr) ?
			get<shared_ptr<const T>>(_a_ptr).get() :
			get<const T*>(_a_ptr);
	}
	template<
		typename T
	>
	__constexpr_imp
		const T*
		get_ptr(
			const const_shared_and_raw_ptr<T>& _a_ptr
		) noexcept
	{
		using namespace std;
		return holds_alternative< shared_ptr<const T>>(_a_ptr) ?
			get<shared_ptr<const T>>(_a_ptr).get() :
			get<const T*>(_a_ptr);
	}
	_END_ABC_UTILITY_NS