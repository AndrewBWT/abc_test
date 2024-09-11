#pragma once

#include "abc_test/gen_data/enumerable.h"
#include <map>

_BEGIN_ABC_GD_NS
	template<
		typename T
	>
	using enum_list_t = std::vector<T>;
	template<
		typename T
	>
	concept enum_has_list_c = std::is_enum_v<T> && requires()
	{
		{ get_enum_list<T>() } -> std::same_as<enum_list_t<T>>;
	};
	template<
		typename T
	>
	__constexpr
		enum_list_t<T>
		get_enum_list(
		) noexcept = delete;
	template<
		typename T
	>
		requires enum_has_list_c<T>
	struct enumerate_enum_helper_t
	{
		__constexpr_imp
			bool
			less_than(
				const T& _a_l,
				const T& _a_r
			) const noexcept
		{
			return _m_elements_to_idx.at(_a_l) < _m_elements_to_idx.at(_a_r);
		}
		__constexpr
			enumerate_enum_helper_t(
				enum_list_t<T>&& _a_enum_list
			);
		__constexpr
			T
			min(
			) const noexcept;
		__constexpr
			T
			max(
			) const noexcept;
		__constexpr
			bool
			increment(
				T& _a_element,
				std::size_t& _a_times_called,
				const std::optional<T>& _a_max_value
			) const noexcept;;
		__constexpr
			bool
			decrement(
				T& _a_element,
				std::size_t& _a_times_called,
				const std::optional<T>& _a_max_value
			) const noexcept;
	private:
		std::map<std::size_t, T> _m_idx_to_elements;
		std::map<T, std::size_t> _m_elements_to_idx;
	};
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr
		const enumerate_enum_helper_t<T>&
		get_thread_local_enumerate_enum_helper(
		) noexcept;
	template<
		typename T
	>
		requires enum_has_list_c<T>
	struct min_value_t<T>
	{
		__constexpr_imp
			T
			min_value(
			) const noexcept
		{
			return get_thread_local_enumerate_enum_helper<T>().min();
		}
	};
	template<
		typename T
	>
		requires enum_has_list_c<T>
	struct max_value_t<T>
	{
		__constexpr_imp
			T
			max_value(
			) const noexcept
		{
			return get_thread_local_enumerate_enum_helper<T>().max();
		}
	};
	template<
		typename T
	>
		requires enum_has_list_c<T>
	struct enumerable_t<T>
	{
		__constexpr_imp
			virtual
			bool
			less_than(
				const T& _a_l,
				const T& _a_r
			) const noexcept
		{
			return get_thread_local_enumerate_enum_helper<T>().less_than(_a_l, _a_r);
		}
		__constexpr_imp
			virtual
			bool
			equal(
				const T& _a_l,
				const T& _a_r
			) const noexcept
		{
			return _a_l == _a_r;
		}
		__constexpr_imp
			enumerable_t(
				const std::size_t _a_difference = std::size_t(1)
			) noexcept
			: _m_difference(_a_difference)
		{

		}
		__constexpr_imp
			bool
			increment(
				T& _a_element,
				std::size_t& _a_times_called,
				const std::optional<T>& _a_max_value
			)
		{
			return get_thread_local_enumerate_enum_helper<T>().increment(_a_element, _a_times_called, _a_max_value);
		}
		__constexpr_imp
			bool
			decrement(
				T& _a_element,
				std::size_t& _a_times_called,
				const std::optional<T>& _a_max_value
			)
		{
			return get_thread_local_enumerate_enum_helper<T>().decrement(_a_element, _a_times_called, _a_max_value);
		}
	private:
		std::size_t _m_difference;
	};
	_END_ABC_NS
		_END_NS

		_BEGIN_ABC_GD_NS
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		enumerate_enum_helper_t<T>::enumerate_enum_helper_t(
			enum_list_t<T>&& _a_enum_list
		)
		: _m_idx_to_elements(std::map<std::size_t, T>())
		, _m_elements_to_idx(std::map<T, std::size_t>())
	{
		using namespace errors;
		for (const T& _a_element : _a_enum_list)
		{
			_m_idx_to_elements.insert({ _m_idx_to_elements.size(),_a_element });
			_m_elements_to_idx.insert({ _a_element, _m_elements_to_idx.size() });
		}
		if (_a_enum_list.size() == 0)
		{
			throw test_library_exception_t(
				fmt::format("Could not create enumerate_enum_helper_t using enum_list of size 0"));
		}
	}
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		T
		enumerate_enum_helper_t<T>::min(
		) const noexcept
	{
		return _m_idx_to_elements.at(0);
	}
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		T
		enumerate_enum_helper_t<T>::max(
		) const noexcept
	{
		return _m_idx_to_elements.at(_m_idx_to_elements.size() - 1);
	}
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		bool
		enumerate_enum_helper_t<T>::increment(
			T& _a_element,
			std::size_t& _a_times_called,
			const std::optional<T>& _a_max_value
		) const noexcept
	{
		using namespace std;
		const size_t _l_current_idx{ _m_elements_to_idx.at(_a_element) };
		const size_t _l_max_times_called{ _m_elements_to_idx.size() - _l_current_idx - 1};
		if (_a_times_called > _l_max_times_called)
		{
			if (_l_max_times_called == 0)
			{
				return false;
			}
			else
			{
				_a_element = _m_idx_to_elements.at(_m_elements_to_idx.size() - 1);
				_a_times_called -= _l_max_times_called;
				return true;
			}
		}
		else
		{
			_a_element = _m_idx_to_elements.at(_l_current_idx + _a_times_called);
			_a_times_called = 0;
			return true;
		}
	}
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		bool
		enumerate_enum_helper_t<T>::decrement(
			T& _a_element,
			std::size_t& _a_times_called,
			const std::optional<T>& _a_max_value
		) const noexcept
	{
		using namespace std;
		const size_t _l_current_idx{ _m_elements_to_idx.at(_a_element) };
		const size_t _l_max_times_called{_l_current_idx};
		if (_a_times_called > _l_max_times_called)
		{
			if (_l_max_times_called == 0)
			{
				return false;
			}
			else
			{
				_a_element = _m_idx_to_elements.at(0);
				_a_times_called -= _l_max_times_called;
				return true;
			}
		}
		else
		{
			_a_element = _m_idx_to_elements.at(_l_current_idx - _a_times_called);
			_a_times_called = 0;
			return true;
		}
	}
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		const enumerate_enum_helper_t<T>&
		get_thread_local_enumerate_enum_helper(
		) noexcept
	{
		thread_local enumerate_enum_helper_t<T> _static_enumerate_enum_helper(get_enum_list<T>());
		return _static_enumerate_enum_helper;
	}
	/*template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		T
		min_value_t<T>::min_value(
		) const noexcept
	{
		return get_thread_local_enumerate_enum_helper<T>().min();
	}*/
	/*template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		T
		max_value_t<T>::max_value(
		) const noexcept
	{
		return get_thread_local_enumerate_enum_helper<T>().max();
	}*/
	/*template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		enumerable_t<T>::enumerable_t(
			const std::size_t _a_difference
		) noexcept
		: _m_difference(_a_difference)
	{

	}
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		bool
		enumerable_t<T>::increment(
			T& _a_element
		)
	{
		return get_thread_local_enumerate_enum_helper<T>().increment(_a_element);
	}
	template<
		typename T
	>
		requires enum_has_list_c<T>
	__constexpr_imp
		bool
		enumerable_t<T>::decrement(
			T& _a_element
		)
	{
		return get_thread_local_enumerate_enum_helper<T>().decrement(_a_element);
	}*/
	_END_ABC_NS
		_END_NS