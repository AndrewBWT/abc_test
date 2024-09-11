#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/assertion.h"
#include "abc_test_test/example_matcher.h"

#include "abc_test_test/pc_test_reporter.h"

#include <functional>

TEST_CASE("Test creation tests", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	test_runner _l_tr;
	pc_test_reporter* _l_pc_tr = new pc_test_reporter();
	_l_tr.add_reporter(_l_pc_tr);
	using unit_test_10 = tuple<string, bool, string,bool,vector<string>,bool>;
	vector<unit_test_10> _l_tests =
	{
		{"Placeholder2",false,"example_matcher()",false,{"Placeholder2"},false},
		{"Placeholder2",true,"example_matcher()",false,{},false},
		{"Placeholder2",false,"example_matcher()",true,{"Placeholder2"},true},
		{"Placeholder2",true,"example_matcher()",true,{},false},
	};
	for (auto& [str1, b1, str2, b2, vect_strs_pp, _l_throw_exception] : _l_tests)
	{
		vector<string> vect_strs;
		for (auto& _l_elem : vect_strs_pp)
		{
			vect_strs.push_back(_l_elem);
			vect_strs.push_back(_l_elem);
		}
		example_matcher ex1(str1, b1);
		if (_l_throw_exception)
		{
			CHECK_THROWS_AS(create_assertion(example_matcher(str1, b1), str2, _l_tr, b2), assertion_exception);
			CHECK_THROWS_AS(create_assertion(ex1, str2, _l_tr, b2), assertion_exception);
		}
		else
		{
			CHECK_NOTHROW(create_assertion(example_matcher(str1, b1), str2, _l_tr, b2));
			CHECK_NOTHROW(create_assertion(ex1, str2, _l_tr, b2));
		}
		const std::vector<error_info>& _l_vect_err_msgs = _l_pc_tr->get_error_info();
		CHECK(_l_vect_err_msgs.size() == vect_strs.size());
		vector<string> _l_res;
		for (auto& _l_str : _l_vect_err_msgs)
		{
			_l_res.push_back(string(_l_str.message()));
		}
		CHECK(_l_res == vect_strs);
		_l_pc_tr->clear_errors();
	}
	test_runner TEST_RUNNER_STR;
	pc_test_reporter* _l_pc_tr2 = new pc_test_reporter();
	TEST_RUNNER_STR.add_reporter(_l_pc_tr2);
	for (auto& [str1, b1, str2, b2, vect_strs, _l_throw_exception] : _l_tests)
	{
		if (_l_throw_exception)
		{
			if (b2)
			{
				CHECK_THROWS_AS(_REQUIRE(example_matcher(str1, b1)), assertion_exception);
			}
			else
			{
				CHECK_NOTHROW(_CHECK(example_matcher(str1, b1)));
			}
		}
		else
		{
			if (b2)
			{
				CHECK_NOTHROW(_REQUIRE(example_matcher(str1, b1)));
			}
			else
			{
				CHECK_NOTHROW(_CHECK(example_matcher(str1, b1)));
			}
		}
		const std::vector<error_info>& _l_vect_err_msgs = _l_pc_tr2->get_error_info();
		CHECK(_l_vect_err_msgs.size() == vect_strs.size());
		vector<string> _l_res;
		for (auto& _l_str : _l_vect_err_msgs)
		{
			_l_res.push_back(string(_l_str.message()));
		}
		CHECK(_l_res == vect_strs);
		_l_pc_tr2->clear_errors();
	}
}