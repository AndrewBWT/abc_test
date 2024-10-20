#pragma once

#include "abc_test/gen_data/random/base.h"
#include "concepts"

_BEGIN_ABC_NS
	template<>
	struct random_generator_t<bool>
	{
		__constexpr_imp
			virtual
			bool
			operator()(
				utility::rng& _a_rnd_generator
				)
		{
			using namespace std;
			size_t _l_val{ _a_rnd_generator() % 2 };
			return (_l_val == 0 ? false : true);
		}
	};
	template<
		typename T
	>
	concept integral_c = std::integral<T>;
	template<
		typename T
	>
		requires integral_c<T>
	struct random_generator_t<T>
	{
		T _m_min;
		T _m_max;
		T _m_range;
		__constexpr
			random_generator_t(
				const T& _a_min = std::numeric_limits<T>::min(),
				const T& _a_max = std::numeric_limits<T>::max()
			) noexcept
			: _m_min(_a_min)
			, _m_max(_a_max)
			, _m_range(_m_max - _m_min)
		{

		}
		__constexpr_imp
			virtual
			T
			operator()(
				utility::rng& _a_rnd_generator
				)
		{
			using namespace std;
			T _l_val{ _a_rnd_generator() % _m_range };
			return _m_min + _l_val;
		}
	};
	_END_ABC_NS