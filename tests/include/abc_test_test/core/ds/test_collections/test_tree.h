#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test_test/pc_test_reporter.h"
#include "abc_test/internal/test_tree.h"
#include "abc_test_test/pc_test_reporter.h"
#include "abc_test/assertion.h"
#include "abc_test_test/example_matcher.h"
#include <set>

#define SPEC_FUNCTION_CHECK(integer) void _f_check_##integer(test::test_runner_t& _a_runner) \
{\
	_CHECK(ab::test::example_matcher(std::to_string(integer), false));\
}

namespace ab::f
{
	SPEC_FUNCTION_CHECK(1);
	SPEC_FUNCTION_CHECK(2);
	SPEC_FUNCTION_CHECK(3);
	SPEC_FUNCTION_CHECK(4);
	SPEC_FUNCTION_CHECK(5);
	SPEC_FUNCTION_CHECK(6);
	SPEC_FUNCTION_CHECK(7);
	SPEC_FUNCTION_CHECK(8);
}

TEST_CASE("test tree test", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	using unit_test = tuple<
		vector<vector<registered_test_data_t>>,
		std::string_view,
		vector<size_t>>;
	std::source_location _l_source_loc;
	vector< unit_test> _l_tests
	{
		{
			{
				{
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_1,_l_source_loc),
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_2,_l_source_loc),
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_3,_l_source_loc),
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_4,_l_source_loc),
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_5,_l_source_loc),
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_6,_l_source_loc),
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_7,_l_source_loc),
					registered_test_data_t("first","hello::goodbye",ab::f::_f_check_8,_l_source_loc),
				}
			},
			"::",{1,2,3,4,5,6,7,8}
		}
	};
	for (auto& [_l_register_tests, _l_delimiter, _l_results] : _l_tests)
	{
		test_tree_t _l_test;
		vector<string> _l_numbs_as_strs;
		for (auto& _l_result : _l_results)
		{
			_l_numbs_as_strs.push_back(to_string(_l_result));
		}
		for (auto& _l_register_test_set : _l_register_tests)
		{
			for (auto& _l_register_test : _l_register_test_set)
			{
				_l_register_test.initialise_test_hierarchy(_l_delimiter);
				_l_test.add_test(_l_register_test);
			}
		}
		test_runner_t _l_test_runner;
		pc_test_reporter* _l_pc_test_reporter = new pc_test_reporter();
		_l_test_runner.add_reporter(_l_pc_test_reporter);
		_l_test.run(_l_test_runner);
		const vector<error_info_t>& _l_test_reporter_results = _l_pc_test_reporter->get_error_info();
		vector<string> _l_returned_results;
		for (auto& _l_error_info : _l_test_reporter_results)
		{
			_l_returned_results.push_back(string(_l_error_info.error_msg()));
		}
		CHECK(_l_returned_results == _l_numbs_as_strs);

	}
}