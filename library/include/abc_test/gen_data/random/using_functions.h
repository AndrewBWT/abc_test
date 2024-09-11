#pragma once

#include <random>
#include <variant>
#include "abc_test/utility/rng.h"

_BEGIN_ABC_GD_NS
	/*!
	* Type synonym for a function which creates a T value from a random generator.
	*/
	template<
		typename T
	>
	using randomly_generate_fp_t = T(*)(utility::rng&);
	/*!
	* Type synonym for a function which creates a T value from a random generator.
	*/
	template<
		typename T
	>
	using randomly_generate_func_object_t = std::function<T(utility::rng&)>;
	/*!
	* Class used to instantiate objects which can generate random elements of a type T.
	*/
	template<
		typename T
	>
	struct rnd_func
	{
	public:
		__constexpr
			rnd_func(
			) noexcept;
		/*!
		* Constructor using a function pointer for the rnd_func object.
		*/
		__constexpr
			rnd_func(
				const randomly_generate_fp_t<T> _a_rnd_func
			) noexcept;
		/*!
		* Constructor using a std::function object for the rnd_func object.
		*/
		__constexpr
			rnd_func(
				const randomly_generate_func_object_t<T> _a_rnd_func
			) noexcept;
		/*!
		* Operator to actually generate a T element.
		*/
		__constexpr
			virtual
			T 
			operator()(
				utility::rng& _a_rnd_generator
			);
	private:
		std::variant<randomly_generate_fp_t<T>, randomly_generate_func_object_t<T>> _m_rnd_func;
		bool _m_set;
	};
	template<
	>
	struct rnd_func<bool>
	{
	private:
		bool _m_lower;
		bool _m_higher;
	public:
		__constexpr_imp
			rnd_func(
				const bool _a_lower,
				const bool _a_higher
			) noexcept
			: _m_lower(_a_lower < _a_higher ? _a_lower : _a_higher)
			, _m_higher(_a_lower < _a_higher ? _a_higher : _a_lower)
		{

		}
		/*!
		* Operator to actually generate a T element.
		*/
		__constexpr_imp
			virtual
			bool
			operator()(
				utility::rng& _a_rnd_generator
				)
		{
			if (_m_lower == _m_higher)
			{
				return _m_lower;
			}
			else
			{
				size_t _l_element{ _a_rnd_generator() % 2};
				return (_l_element == 0 ? _m_lower : _m_higher);
			}
		}
	};
	_END_ABC_NS
		_END_NS

		_BEGIN_ABC_GD_NS
	template<
		typename T
	>
	__constexpr_imp
		rnd_func<T>::rnd_func(
		) noexcept
		: _m_rnd_func(std::variant<randomly_generate_fp_t<T>, randomly_generate_func_object_t<T>>{})
		, _m_set(false)
	{}
	template<
		typename T
	>
	__constexpr_imp
		rnd_func<T>::rnd_func(
			const randomly_generate_fp_t<T> _a_rnd_func
		) noexcept
		: _m_rnd_func(_a_rnd_func)
		, _m_set(true)
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		rnd_func<T>::rnd_func(
			const randomly_generate_func_object_t<T> _a_rnd_func
		) noexcept
		: _m_rnd_func(_a_rnd_func)
		, _m_set(true)
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		T
		rnd_func<T>::operator()(
			utility::rng& _a_rnd_generator
		) 
	{
		using namespace std;
		if (not _m_set)
		{
			throw exception::test_harness_exception_t(
				fmt::format("rnd_func has not been set with a function."));
		}
		else if (holds_alternative<randomly_generate_fp_t<T>>(_m_rnd_func))
		{
			randomly_generate_fp_t<T> _l_fp{ get<randomly_generate_fp_t<T>>(_m_rnd_func) };
			return _l_fp(_a_rnd_generator);
		}
		else 
		{
			randomly_generate_func_object_t _l_fp{ get< randomly_generate_func_object_t <T>>(_m_rnd_func) };
			return _l_fp(_a_rnd_generator);
		}
	}
	_END_ABC_NS
		_END_NS