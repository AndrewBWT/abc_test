#pragma once

#include <list>
#include "abc_test/core/ds/test_data/registered_test_data.h"

_BEGIN_ABC_DS_NS
	/*!
	* A type synonym for a test_list_element.
	*/
	using test_list_element_t = registered_test_data_t;
	/*!
	* Type synonym for a collectin of test_elements.
	*/
	using test_list_t = std::list<test_list_element_t>;
	/*!
	* Get the global test_list.
	*/
	__constexpr
		const test_list_t&
		get_global_test_list(
		) noexcept;
	/*!
	* Add a test to the global test_list.
	*/
	__constexpr
		void
		add_test(
			const test_list_element_t& _a_test_list_internal
		) noexcept;
	/*!
	* Clear the global test_list.
	*/
	__constexpr
		void
		clear_test_list(
		) noexcept;
	namespace
	{
		__constexpr
			test_list_t&
			get_mutable_test_list(
			) noexcept;
	}
	_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
	__constexpr_imp
		const test_list_t&
		get_global_test_list(
		) noexcept
	{
		return get_mutable_test_list();
	}
	__constexpr_imp
		void
		add_test(
			const test_list_element_t& _a_test_list_internal
		) noexcept
	{
		test_list_t& _l_test_list{ get_mutable_test_list() };
		_l_test_list.push_back(_a_test_list_internal);
	}
	__constexpr_imp
		void
		clear_test_list(
		) noexcept
	{
		test_list_t& _l_test_list{ get_mutable_test_list() };
		_l_test_list.clear();
	}
	namespace
	{
		__constexpr_imp
			test_list_t&
			get_mutable_test_list(
			) noexcept
		{
			static test_list_t _static_test_list;
			return _static_test_list;
		}
	}
	_END_ABC_DS_NS