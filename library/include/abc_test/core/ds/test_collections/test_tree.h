#pragma once

#include <memory>
#include <vector>
#include "abc_test/core/ds/test_data/post_setup_test_data.h"
#include "abc_test/core/test_options.h"
#include <memory>

_BEGIN_ABC_DS_NS
	class test_tree_t;
	struct test_tree_iterator_t;
	using test_tree_child_node = std::unique_ptr<test_tree_t>;
	using test_tree_child_nodes = std::vector<test_tree_child_node>;
	using node_t = std::shared_ptr<post_setup_test_data_t>;
	using add_element_t = post_setup_test_data_t;
	using flat_test_set = std::vector<node_t>;
	/*!
	* Class for describing test trees. The idea behind the test tree is
	* that each test is associated with a string which describes where in the test
	* tree it goes; using the delimiter ".", the test "r1.r2.r3" would be placed as a child of "r3", itself
	* a child of "r2", in turn a child of "r1", which would be a child of the root, which itself has no
	* identifier.
	*/
	class test_tree_t
	{
	public:
		/*!
		* Default constructor
		*/
		__constexpr
			test_tree_t(
			) noexcept;
		/*!
		* Adds a test to the test_tree, using the delimiter provied as an argument.
		* @param _a_test The test to be registered.
		*/
		__no_constexpr
			errors::opt_setup_error_t
			add_test(
				const node_t& _a_test,
				const test_options_t& _a_options
			) noexcept;
		/*!
		* Gets a const begin iterator for the underlying tree.
		*/
		__no_constexpr_or_inline
			test_tree_iterator_t
			begin(
			) const noexcept;
		/*!
		* Gets a const end iterator for the underlying tree.
		*/
		__no_constexpr_or_inline
			test_tree_iterator_t
			end(
			) const noexcept;
		friend class test_tree_iterator_t;
	private:
		test_path_element_t _m_node_name;
		flat_test_set _m_nodes_tests;
		test_tree_child_nodes _m_nodes_child_nodes;
		__constexpr
			bool
			has_test_index_less_than(
				const size_t _a_idx
			) const noexcept;
		__constexpr
			errors::opt_setup_error_t
			add_test(
				const node_t& _a_test,
				const size_t _a_idx
			) noexcept;
		__constexpr
			test_tree_t(
				const test_path_element_ref_t _a_node_name
			) noexcept;
		__constexpr
			bool
			has_test_children(
			) const noexcept;
	};
	using test_tree_ref_t = test_tree_t&;
	using test_tree_const_ptr_t = const test_tree_t*;
	_END_ABC_DS_NS

_BEGIN_ABC_DS_NS
	__constexpr_imp
		test_tree_t::test_tree_t(
		) noexcept
		: _m_node_name(test_path_element_t())
		, _m_nodes_tests(flat_test_set())
		, _m_nodes_child_nodes(test_tree_child_nodes())
	{
	}
	__no_constexpr_imp
		errors::opt_setup_error_t
		test_tree_t::add_test(
			const node_t& _a_test,
			const test_options_t& _a_options
		) noexcept
	{
		using namespace std;
		using namespace errors;
		if (_a_test == nullptr)
		{
			return errors::opt_setup_error_t();
		}
		else
		{
			const post_setup_test_data_t& _l_test{ *_a_test };
			//if (not _a_test.has_registered_test_data())
			//{
			//	return opt_setup_error_t(setup_error_t(fmt::format(
			//		"setup_test_error: post_setup_test_data_t object has a nullptr registered_test_data_t variable. "
		//			"post_setup_test_data_t = {0}",_a_test
		//		),false));
		//	}
			if (_l_test.thread_resourses_required() > _a_options._m_threads)
			{
				return opt_setup_error_t(setup_error_t(fmt::format(
					"setup_test_error: post_setup_test_data_t's required thread resourses greater than those allocated to the system. "
					"post_setup_test_data_t requires {0} threads, while the system has {1} threads available. "
					"post_setup_test_data_t = {2}",
					_l_test.thread_resourses_required(),
					_a_options._m_threads,
					*_a_test
				), false));
			}
			return add_test(_a_test, 0);
		}
	}
	__constexpr_imp
		bool
		test_tree_t::has_test_index_less_than(
			const size_t _a_idx
		) const noexcept
	{
		return _a_idx + 1 < _m_nodes_tests.size();
	}
	__constexpr_imp
		errors::opt_setup_error_t
		test_tree_t::add_test(
			const node_t& _a_test,
			const size_t _a_idx
		) noexcept
	{
		using namespace std;
		using namespace errors;
		using namespace utility;
		const test_path_hierarchy_t& _l_test_hierarchy{ _a_test->test_path_hierarchy()};
		if (_a_idx >= _l_test_hierarchy.size())
		{
			if (_m_nodes_tests.size() > 0)
			{
				auto _l_x = upper_bound(_m_nodes_tests.begin(), _m_nodes_tests.end(), _a_test,
					[](const node_t& _a_left, const node_t& _a_right) {
						return _a_left->registered_test_data()._m_user_data.name <
							_a_right->registered_test_data()._m_user_data.name;
					}
				);
				if (_l_x != _m_nodes_tests.end() && 
					_l_x->get()->registered_test_data()._m_user_data.name ==
					_a_test->registered_test_data()._m_user_data.name)
				{
					return opt_setup_error_t(setup_error_t(fmt::format(
						"setup_test_error: post_setup_test_data_t's registered_test_data has the same description as a "
						"current entry in the test_tree_t object. "
						"The post_setup_test_data object we are attempting to insert is {0}, "
						"while the post_setup_test_data_t object blocking its insertion is {1}. ",
						*_a_test,
						(*_l_x->get())
					), false));
				}
				_m_nodes_tests.insert(_l_x, _a_test);
			}
			else
			{
				_m_nodes_tests.push_back(_a_test);
			}
			return opt_setup_error_t();
		}
		else
		{
			const test_path_element_ref_t _l_current_test_str{ _l_test_hierarchy[_a_idx] };
			const size_t _l_next_idx{ _a_idx + 1 };
			for (test_tree_child_node& _l_child_ptr : _m_nodes_child_nodes)
			{
				test_tree_ref_t _l_child = *_l_child_ptr.get();
				if (_l_child._m_node_name == _l_current_test_str)
				{
					return _l_child.add_test(_a_test, _l_next_idx);
				}
			}
			//Element not added
			test_tree_child_node _l_element_to_insert(test_tree_child_node(new test_tree_t(_l_current_test_str)));
			auto _l_x = upper_bound(_m_nodes_child_nodes.begin(), _m_nodes_child_nodes.end(), std::move(_l_element_to_insert),
				[](const test_tree_child_node& _a_left, const test_tree_child_node& _a_right) {return _a_left->_m_node_name < _a_right->_m_node_name; });
			auto _l_itt = _m_nodes_child_nodes.insert(
				_l_x,
				std::move(_l_element_to_insert)
			);
			return _l_itt->get()->add_test(_a_test, _l_next_idx);
		}
	}
	__constexpr_imp
		test_tree_t::test_tree_t(
			const test_path_element_ref_t _a_node_name
		) noexcept
		: _m_node_name(test_path_element_t(_a_node_name))
	{

	}
	__constexpr_imp
		bool
		test_tree_t::has_test_children(
		) const noexcept
	{
		return _m_nodes_tests.size() > 0;
	}
	_END_ABC_DS_NS