#pragma once

#include "abc_test/core/ds/test_collections/test_list.h"

#include "abc_test/core/ds/test_collections/test_tree_iterator.h"

_BEGIN_ABC_DS_NS
	using test_lists_t = std::vector<utility::const_shared_and_raw_ptr<test_list_t>>;
	using unique_id_t = std::size_t;
	/*!
	* The object which holds the collection of tests. 
	* It also constructs the test_tree.
	*/
	struct test_collection_t
	{
	public:
		/*!
		* Given a test_options file and a test_list, initialises a test_collection object
		*/
		__constexpr
			test_collection_t(
				const test_options_t& _a_test_options,
				const reporters::error_reporter_controller_t& _a_error_reporter_controller
			) noexcept;
		/*!
		* Adds a set of test_lists to the object.
		*/
		__constexpr
			void
			add_tests(
				const test_lists_t& _a_test_lists
			) noexcept;
		/*!
		* Gets an iterator to the internal tree object
		*/
		__constexpr
			test_tree_iterator_t
			begin(
			) const noexcept;
		/*!
		* Gets an end iterator to the internal tree object, of const type.
		*/
		__constexpr
			test_tree_iterator_t
			end(
			) const noexcept;
	private:
		test_tree_t _m_test_tree;
		reporters::error_reporter_controller_t _m_error_reporter_controller;
		unique_id_t _m_test_discovery_id;
		const test_options_t& _m_options;
	};
	_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
	__constexpr_imp
		test_collection_t::test_collection_t(
			const test_options_t& _a_test_options,
			const reporters::error_reporter_controller_t& _a_error_reporter_controller
		) noexcept
		: _m_test_tree(test_tree_t())
		, _m_test_discovery_id(unique_id_t{ 1 })
		, _m_options(_a_test_options)
	{
	}
	__constexpr_imp
		void
		test_collection_t::add_tests(
			const test_lists_t& _a_test_list_collection
		) noexcept
	{
		using namespace std;
		using namespace ds;
		using namespace utility;
		using namespace errors;
		for (const const_shared_and_raw_ptr<test_list_t>& _l_test_list_element : _a_test_list_collection)
		{
			const_ptr_t<test_list_t> _l_tl{ get_ptr(_l_test_list_element) };
			for (const test_list_element_t& _l_test_element : *_l_tl)
			{
				const repetition_tree_t* _l_reps{
					_m_options.get_test_repetition_configurations().contains(_m_test_discovery_id) ?
					&_m_options.get_test_repetition_configurations().at(_m_test_discovery_id) :
					nullptr
				};
				const opt_setup_error_t _l_res{ _m_test_tree.add_test(post_setup_test_data_t(
						_l_test_element,
						_m_options._m_path_delimiter,
						_m_test_discovery_id,
						_m_options.check_if_test_is_to_be_ran(_m_test_discovery_id),
						_l_reps,
						_m_options._m_threads),_m_options) };
				if (_l_res.has_value())
				{
					_m_error_reporter_controller.report_error(_l_res.value());
				}
				_m_test_discovery_id++;
			}
		}
	}
	__constexpr_imp
		test_tree_iterator_t
		test_collection_t::begin(
		) const noexcept
	{
		return std::begin(_m_test_tree);
	}
	__constexpr_imp
		test_tree_iterator_t
		test_collection_t::end(
		) const noexcept
	{
		return std::end(_m_test_tree);
	}
	_END_ABC_DS_NS