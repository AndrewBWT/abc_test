#pragma once

#include "abc_test/utility/rng.h"

_BEGIN_ABC_NS
	template<
		typename T
	>
	using random_func_t = std::function<T(utility::rng&)>;
	template<
		typename T
	>
	using random_func_ptr_t = T(*)(utility::rng&);
	template<
		typename T
	>
	struct random_generator_t
	{
	private:
		random_func_t<T> _m_rnd_gen;
	public:
		__constexpr_imp
			random_generator_t(
				random_func_t<T>& _a_rnd_func
			) noexcept
		{

		}
		__constexpr_imp
			virtual
			T
			operator()(
				utility::rng& _a_rnd_generator
				)
		{
			return _m_rnd_gen(_a_rnd_generator);
		}
	};
	template<
		typename T
	>
	struct random_generator_using_func_t
	{

	};
	template<
		typename T
	>
	__constexpr_imp
		random_generator_t<T>*
		using_function(
			random_func_t<T>& _a_func
		) noexcept
	{
		return nullptr;
	}
	template<
		typename T
	>
	random_generator_t<T>*
		using_function(
			random_func_ptr_t<T> _a_func
		) noexcept
	{
		return nullptr;
	}
	_END_ABC_NS