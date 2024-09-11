#pragma once
#include "abc_test_test/test_utilities/register_test.h"

#include "abc_test/core/test_tree/iterator.h"

#include <optional>

namespace ab::test
{
	void _l_f2(test_runner_t&) {}
}

TEST_CASE("Test tree iterator default", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	test_tree_iterator_t _l_tti;
	test_tree_t _l_tt;
	source_location _l_sl;
	_l_tt.add_test(register_test_t("hello", "ex", _l_f2, _l_sl));
	test_tree_iterator_t _l_tti4(&_l_tt);
	CHECK_THROWS_AS(*_l_tti,std::runtime_error);
	CHECK_THROWS_AS(_l_tti++, std::runtime_error);
	CHECK_THROWS_AS(++_l_tti, std::runtime_error);
	CHECK(_l_tti == _l_tti);
	CHECK(_l_tti != _l_tti4);
}

TEST_CASE("Test tree iterator tests", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	source_location _l_source = source_location::current();
	using unit_test = std::tuple<std::vector<register_test_t>>;
	vector<unit_test> _l_tests =
	{
		{
			{
				register_test_t("hello1","hi::one",_l_f2,_l_source),
				register_test_t("hello1","hi::two",_l_f2,_l_source)
			}
		}
	};

	test_tree_iterator_t _l_end_itt;
	for (auto& [_l_register_tests] : _l_tests)
	{
		test_tree_t _l_tt;
		for (auto&& _l_register_test : _l_register_tests)
		{
			_l_register_test.initialise_test_hierarchy("::");
			_l_tt.add_test(_l_register_test);
		}
		test_tree_iterator_t _l_tti(&_l_tt);
		vector<register_test_t> _l_results, _l_results2;
		while (_l_tti != _l_end_itt)
		{
			_l_results.push_back(*_l_tti);
			_l_tti++;
		}
		test_tree_iterator_t _l_tti2(&_l_tt);
		while (_l_tti2 != _l_end_itt)
		{
			_l_results2.push_back(*_l_tti2);
			++_l_tti2;
		}
		CHECK(_l_results2.size() == _l_results.size());
		for (size_t _l_idx{ 0 }; _l_idx < _l_results2.size(); _l_idx++)
		{
			check_register_test(_l_results2[_l_idx], false, _l_results[_l_idx]._m_test_info);
		}
		CHECK(_l_results2.size() == _l_register_tests.size());
		for (size_t _l_idx{ 0 }; _l_idx < _l_results2.size(); _l_idx++)
		{
			check_register_test(_l_results2[_l_idx], false, _l_register_tests[_l_idx]._m_test_info);
		}
	}
}