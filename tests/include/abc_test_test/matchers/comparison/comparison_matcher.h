#pragma once

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/matcher/comparison/comparison_matcher.h"

namespace
{
	using unit_test1 = std::tuple<int, int, bool, bool, bool, bool, bool, bool,std::string,std::string>;
	std::vector<unit_test1> get_tests()
	{
		return
		{
			{1,1,true,false,false,true,false,true,"1","1"},
			{1,2,false,true,true,true,false,false,"1","2"},
			{2,1,false,true,false,false,true,true,"2","1"},
			{2,2,true,false,false,true,false,true,"2","2"}
		};
	}
	template<
		ab::test::cmp::comparison_enum_t Comparison_Enum
	>
	void
		run_test(int& _a_elem_l, int& _a_elem_r, bool _a_res,
			const std::string_view _a_l_str,const std::string_view _a_r_str,
			size_t _a_pred,const std::string_view _a_cmp_str)
	{
		using namespace ab::test::cmp;
		using namespace ab::test;
		using namespace std;
		string _l_cmp_str = string(_a_l_str) + " " + string(_a_cmp_str) + " " + string(_a_r_str);
		vector< shared_ptr<generic_matcher_t>> _l_matchers =
		{
			shared_ptr<generic_matcher_t>(new comparison_matcher<int,int,Comparison_Enum>(int(_a_elem_l),int(_a_elem_r))),
			shared_ptr<generic_matcher_t>(new comparison_matcher<int&,int,Comparison_Enum>(_a_elem_l,int(_a_elem_r))),
			shared_ptr<generic_matcher_t>(new comparison_matcher<int,int&,Comparison_Enum>(int(_a_elem_l),_a_elem_r)),
			shared_ptr<generic_matcher_t>(new comparison_matcher<int&,int&,Comparison_Enum>(_a_elem_l,_a_elem_r)),
		};
		for (auto& _l_matcher2 : _l_matchers)
		{
			using namespace ab::test;
			operator_based_matcher_t* _l_matcher = (dynamic_cast<operator_based_matcher_t*>(_l_matcher2.get()));
			CHECK(_l_matcher->get_failure() == "");
			CHECK(_l_matcher->get_failure_msg() == "");
			CHECK(_l_matcher->passed() == false);
			CHECK(_l_matcher->get_precedence() == _a_pred);
			test_runner _l_t1;
			_l_matcher->run_test(_l_t1);
			if (_a_res)
			{
				CHECK(_l_matcher->get_failure() == "");
				CHECK(_l_matcher->passed() == true);
			}
			else
			{
				CHECK(_l_matcher->get_failure_msg() == _l_cmp_str);
				CHECK(_l_matcher->passed() == false);
			}
			CHECK(_l_matcher->get_failure_msg() == _l_cmp_str);
			CHECK(_l_matcher->get_precedence() == _a_pred);
		}
	}
}

TEST_CASE("Comparison matcher tests", "ab::test")
{
	using namespace ab::test::cmp;
	using enum comparison_enum_t;
	for (auto& [_l_arg1, _l_arg2, _l_eq_res, _l_neq_res, _l_lt_res, _l_leq_res,
		_l_gt_res, _l_geq_res, _l_l_str, _l_r_str] : get_tests())
	{
		run_test<EQ>(_l_arg1, _l_arg2, _l_eq_res, _l_l_str, _l_r_str,10, "==");
		run_test<NEQ>(_l_arg1, _l_arg2, _l_neq_res, _l_l_str, _l_r_str,10, "!=");
		run_test<LT>(_l_arg1, _l_arg2, _l_lt_res, _l_l_str, _l_r_str, 9, "<");
		run_test<LEQ>(_l_arg1, _l_arg2, _l_leq_res, _l_l_str, _l_r_str, 9, "<=");
		run_test<GT>(_l_arg1, _l_arg2, _l_gt_res, _l_l_str, _l_r_str, 9, ">");
		run_test<GEQ>(_l_arg1, _l_arg2, _l_geq_res, _l_l_str, _l_r_str, 9, ">=");
	}
}