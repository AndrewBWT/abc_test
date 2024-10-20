#pragma once

#include "abc_test/gen_data/enumeration.h"
#include <array>

_BEGIN_ABC_NS
	template<
		typename T,
		std::size_t N
	>
	requires min_value_c<T>
	struct min_value_t<std::array<T, N>>
	{
		__constexpr_imp
			std::array<T,N>
			min_value(
			) const noexcept
		{
			using namespace std;
			array<T, N> _l_rv;
			for (size_t _l_idx{ 0 }; _l_idx < N; ++_l_idx)
			{
				_l_rv[_l_idx] = min_value_t<T>().min_value();
			}
			return _l_rv;
		}
	};
	template<
		typename T,
		std::size_t N
	>
	requires max_value_c<T>
	struct max_value_t<std::array<T, N>>
	{
		__constexpr_imp
			std::array<T, N>
			max_value(
			) const noexcept
		{
			using namespace std;
			array<T, N> _l_rv;
			for (size_t _l_idx{ 0 }; _l_idx < N; ++_l_idx)
			{
				_l_rv[_l_idx] = max_value_t<T>().max_value();
			}
			return _l_rv;
		}
	};
	template<
		typename T,
		std::size_t N
	>
	struct enumerable_t<std::array<T, N>>
	{
	private:
		enumerate_t<T> _m_enumerate;
		std::size_t _m_n_jumps;
	public:
		__constexpr_imp
			virtual
			bool
			less_than(
				const std::array<T,N>& _a_l,
				const std::array<T, N>& _a_r
			) const noexcept
		{
			for (size_t _l_idx{ N }; _l_idx > 0; --_l_idx)
			{
				if (_m_enumerate.less_than(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
				{
					return true;
				}
				else if (not _m_enumerate.equal(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
				{
					return false;
				}
			}
			return false;
		}
		__constexpr_imp
			virtual
			bool
			equal(
				const std::array<T, N>& _a_l,
				const std::array<T, N>& _a_r
			) const noexcept
		{
			for (size_t _l_idx{ N }; _l_idx > 0; --_l_idx)
			{
				if (_m_enumerate.less_than(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
				{
					return false;
				}
				else if (not _m_enumerate.equal(_a_l[_l_idx - 1], _a_r[_l_idx - 1]))
				{
					return false;
				}
			}
			return true;
		}
		__constexpr_imp
			enumerable_t(
				const std::size_t _a_n_jumps = std::size_t{ 1 },
				enumerate_t<T> _a_enumerate = all_values<T>()
			)
			: _m_enumerate(_a_enumerate)
			, _m_n_jumps(_a_n_jumps)
		{
			
		}
		__constexpr_imp
			bool
			increment(
				std::array<T,N>& _a_array,
				std::size_t& _a_n_times_to_increment,
				const std::optional<std::array<T, N>>& _a_max_value
			)
		{
			while (_a_n_times_to_increment > 0)
			{
				T& _l_elem{ _a_array[N - 1] };
				if (_m_enumerate.next_element(_l_elem, _a_n_times_to_increment))
				{
					if (_a_n_times_to_increment == 0)
					{
						return (_a_array <= _a_max_value);
					}
					else
					{
						//Set it to min, increment.
						_l_elem = _m_enumerate.min();
						for (size_t _l_idx{ N - 1 }; _l_idx > 0; --_l_idx)
						{
							std::size_t _l_arg{ 1 };
							if (_m_enumerate.next_element(_a_array[_l_idx-1], _l_arg))
							{
								--_a_n_times_to_increment;
								break;
							}
							else
							{
								_a_array[_l_idx-1] = _m_enumerate.min();
								if (_l_idx == 1)
								{
									return false;
								}
							}
						}
					}
				}
				else
				{
					//Set it to min, increment.
					_l_elem = _m_enumerate.min();
					for (size_t _l_idx{ N - 1 }; _l_idx > 0; --_l_idx)
					{
						std::size_t _l_arg{ 1 };
						if (_m_enumerate.next_element(_a_array[_l_idx-1], _l_arg))
						{
							--_a_n_times_to_increment;
							break;
						}
						else
						{
							_a_array[_l_idx-1] = _m_enumerate.min();
							if (_l_idx == 1)
							{
								return false;
							}
						}
					}
				}
			}
			return (_a_array <= _a_max_value);
		}
		__constexpr_imp
			bool
			decrement(
				std::array<T, N>& _a_array,
				std::size_t& _a_n_times_to_increment,
				const std::optional<std::array<T, N>>& _a_max_value
			)
		{
			while (_a_n_times_to_increment > 0)
			{
				T& _l_elem{ _a_array[N - 1] };
				if (_m_enumerate.move_backward(_l_elem, _a_n_times_to_increment))
				{
					if (_a_n_times_to_increment == 0)
					{
						return (_a_array >= _a_max_value);
					}
					else
					{
						//Set it to min, increment.
						_l_elem = _m_enumerate.max();
						for (size_t _l_idx{ N - 1 }; _l_idx > 0; --_l_idx)
						{
							std::size_t _l_arg{ 1 };
							if (_m_enumerate.move_backward(_a_array[_l_idx-1], _l_arg))
							{
								--_a_n_times_to_increment;
								break;
							}
							else
							{
								_a_array[_l_idx-1] = _m_enumerate.max();
								if (_l_idx == 1)
								{
									return false;
								}
							}
						}
					}
				}
				else
				{
					_l_elem = _m_enumerate.max();
					for (size_t _l_idx{ N - 1 }; _l_idx > 0; --_l_idx)
					{
						std::size_t _l_arg{ 1 };
						if (_m_enumerate.move_backward(_a_array[_l_idx-1], _l_arg))
						{
							--_a_n_times_to_increment;
							break;
						}
						else
						{
							_a_array[_l_idx-1] = _m_enumerate.max();
							if (_l_idx == 1)
							{
								return false;
							}
						}
					}
				}
			}
			return (_a_array >= _a_max_value);
		}
	};
	_END_ABC_NS