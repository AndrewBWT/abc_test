#pragma once

#include <random>
#include "abc_test/internal/utility/internal/macros.h"

_BEGIN_ABC_UTILITY_NS
	using seed_t = std::vector<uint32_t>;
	struct rng
	{
	public:
		using result_type = std::mt19937_64::result_type;
		static result_type
			min(
		) noexcept
		{
			return std::mt19937_64::min();
		}
		static result_type
			max(
			) noexcept
		{
			return std::mt19937_64::max();
		}
		inline
			void
			progress(
				const size_t _a_expected_calls
			) noexcept
		{
			if (_m_calls > _a_expected_calls)
			{
				std::seed_seq _l_seed_seq(_m_seed.begin(), _m_seed.end());
				_m_rnd = std::mt19937_64(_l_seed_seq);
				_m_calls = 0;
				while (_m_calls < _a_expected_calls)
				{
					++_m_calls;
					auto _l_res = _m_rnd();
				}
			}
			while (_m_calls < _a_expected_calls)
			{
				++_m_calls;
				auto _l_res = _m_rnd();
			}
		}
		inline
			result_type
			operator()
			() 
		{
			++_m_calls;
			return _m_rnd();
		}
		inline
			rng(
			) noexcept
			: rng(seed_t())
		{

		}
		inline
			rng(
				const seed_t& _a_seed
			) noexcept
			: _m_rnd()
			, _m_seed(_a_seed)
			, _m_calls(0)
		{
			std::seed_seq _l_seed_seq(_m_seed.begin(), _m_seed.end());
			_m_rnd = std::mt19937_64(_l_seed_seq);
		}
		constexpr
			size_t
			calls(
			) const noexcept
		{
			return _m_calls;
		}
	private:
		std::mt19937_64 _m_rnd;
		utility::seed_t _m_seed;
		size_t _m_calls;
	};
	_END_ABC_UTILITY_NS