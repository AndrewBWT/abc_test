#pragma once

#include "abc_test/core/ds/test_collections/test_collection.h"

_BEGIN_ABC_DS_NS
	using post_setup_test_data_collection_t = std::list<post_setup_test_data_t>;
	using post_setup_test_data_collection_itterator_t = post_setup_test_data_collection_t::iterator;
	/*!
	* Objcet representing a collection of tests, after all processing has been done and they are ready
	* to be ran.
	*/
	struct finalised_test_collection_t
	{
	public:
		/*!
		* Default constructor, represents empty collection.
		*/
		__no_constexpr
			finalised_test_collection_t(
			) noexcept;
		/*!
		* Adds all test collection elemnets to collection
		*/
		__no_constexpr
			finalised_test_collection_t(
				const test_collection_t& _a_tc
			) noexcept;
		__no_constexpr
			finalised_test_collection_t(
				test_collection_t&& _a_tc
			) noexcept;
		/*!
		* Tells how many elemnets are left in the collection
		*/
		__constexpr
			std::size_t
			remaining_elements(
			) noexcept;
		/*!
		* Gets the next element in the collection
		*/
		__no_constexpr
			const post_setup_test_data_t&
			next(
			) noexcept;
	private:
		post_setup_test_data_collection_t _m_tests;
		post_setup_test_data_collection_itterator_t _m_itt;
		template<
			typename R
		>
		__constexpr
			void
			process_test_collection(
				R&& _a_tc
			) noexcept;
	};
	_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
	__no_constexpr_imp
		finalised_test_collection_t::finalised_test_collection_t(
		) noexcept
		: _m_tests(post_setup_test_data_collection_t{})
		, _m_itt(_m_tests.begin())
	{

	}
	__no_constexpr_imp
		finalised_test_collection_t::finalised_test_collection_t(
			const test_collection_t& _a_tc
		) noexcept
		: finalised_test_collection_t()
	{
		process_test_collection(_a_tc);
	}
	__no_constexpr_imp
		finalised_test_collection_t::finalised_test_collection_t(
			test_collection_t&& _a_tc
		) noexcept
		: finalised_test_collection_t()
	{
		process_test_collection(std::move(_a_tc));
	}
	__constexpr_imp
		std::size_t
		finalised_test_collection_t::remaining_elements(
		) noexcept
	{
		return std::distance(_m_itt, _m_tests.end());
	}
	__no_constexpr_imp
		const post_setup_test_data_t&
		finalised_test_collection_t::next(
		) noexcept
	{
		const post_setup_test_data_t& _l_rv{ *_m_itt };
		++_m_itt;
		return _l_rv;
	}
	template<
		typename R
	>
	__constexpr_imp
		void
		finalised_test_collection_t::process_test_collection(
			R&& _a_tc
		) noexcept
	{
		for (auto&& _l_test : _a_tc)
		{
			if (_l_test.is_test_to_be_ran())
			{
				_m_tests.push_back(_l_test);
			}
		}
		_m_itt = _m_tests.begin();
	}
	_END_ABC_DS_NS