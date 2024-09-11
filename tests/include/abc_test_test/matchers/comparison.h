#pragma once

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/matcher/comparison.h"
#include "abc_test/matcher/comparison/placeholder.h"
#include "abc_test/matcher/comparison/operators.h"
#include "abc_test/matcher/logic/operators.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/core/test_main.h"
#include "abc_test/core/register_test.h"
#include "abc_test/matcher/comparison.h"
#include "abc_test/matcher/logic.h"
#include "abc_test/matcher/logic/operators.h"
#include "abc_test/assertion.h"

#include <functional>
#include <initializer_list>

_TEST_CASE("Basic numerical tests", "ab::tests::numeric")
{
	using namespace ab::test;
	_CHECK(cmp::neq(1, 2));
	//	auto _l_1 = cmp::neq(1, 2);
	//	auto _l_n = _l_1 && _l_1;
	_CHECK(cmp::neq(1, 2) && cmp::lt(3, 2));
	_CHECK(EXPR(1 == 2) && EXPR(3 == 2));
}


namespace ab::test
{
	using unit_test3 = std::tuple<int, int, bool, bool, bool, bool, bool, bool, std::string, std::string>;
	std::vector<unit_test3> get_tests4()
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
		run_test4(int& _a_elem_l, int& _a_elem_r, bool _a_res,
			const std::string_view _a_l_str, const std::string_view _a_r_str,
			size_t _a_pred, const std::string_view _a_cmp_str)
	{
		using namespace ab::test::cmp;
		using namespace ab::test;
		using namespace std;
		using enum ab::test::cmp::comparison_enum_t;
		string _l_cmp_str = string(_a_l_str) + " " + string(_a_cmp_str) + " " + string(_a_r_str);
		vector< matcher_t> _l_matchers;
		if (Comparison_Enum == EQ)
		{
			_l_matchers.push_back(EXPR(int(_a_elem_l) == int(_a_elem_r)));
			_l_matchers.push_back(EXPR(_a_elem_l == int(_a_elem_r)));
			_l_matchers.push_back(EXPR(int(_a_elem_l) == _a_elem_r));
			_l_matchers.push_back(EXPR(_a_elem_l == _a_elem_r));
		}
		else if (Comparison_Enum == NEQ)
		{
			_l_matchers.push_back(EXPR(int(_a_elem_l) != int(_a_elem_r)));
			_l_matchers.push_back(EXPR(_a_elem_l != int(_a_elem_r)));
			_l_matchers.push_back(EXPR(int(_a_elem_l) != _a_elem_r));
			_l_matchers.push_back(EXPR(_a_elem_l != _a_elem_r));
		}
		else if (Comparison_Enum == LT)
		{
			_l_matchers.push_back(EXPR(int(_a_elem_l) < int(_a_elem_r)));
			_l_matchers.push_back(EXPR(_a_elem_l < int(_a_elem_r)));
			_l_matchers.push_back(EXPR(int(_a_elem_l) < _a_elem_r));
			_l_matchers.push_back(EXPR(_a_elem_l < _a_elem_r));
		}
		else if (Comparison_Enum == LEQ)
		{
			_l_matchers.push_back(EXPR(int(_a_elem_l) <= int(_a_elem_r)));
			_l_matchers.push_back(EXPR(_a_elem_l <= int(_a_elem_r)));
			_l_matchers.push_back(EXPR(int(_a_elem_l) <= _a_elem_r));
			_l_matchers.push_back(EXPR(_a_elem_l <= _a_elem_r));
		}
		else if (Comparison_Enum == GT)
		{
			_l_matchers.push_back(EXPR(int(_a_elem_l) > int(_a_elem_r)));
			_l_matchers.push_back(EXPR(_a_elem_l > int(_a_elem_r)));
			_l_matchers.push_back(EXPR(int(_a_elem_l) > _a_elem_r));
			_l_matchers.push_back(EXPR(_a_elem_l > _a_elem_r));
		}
		else if (Comparison_Enum == GEQ)
		{
			_l_matchers.push_back(EXPR(int(_a_elem_l) >= int(_a_elem_r)));
			_l_matchers.push_back(EXPR(_a_elem_l >= int(_a_elem_r)));
			_l_matchers.push_back(EXPR(int(_a_elem_l) >= _a_elem_r));
			_l_matchers.push_back(EXPR(_a_elem_l >= _a_elem_r));
		}
		for (auto& _l_matcher2 : _l_matchers)
		{
			using namespace ab::test;
			operator_based_matcher_t* _l_matcher3 = (dynamic_cast<operator_based_matcher_t*>(_l_matcher2.internal_matcher().get()));
			operator_based_matcher_t& _l_matcher = *_l_matcher3;
			CHECK(_l_matcher.get_failure() == "");
			CHECK(_l_matcher.get_failure_msg() == "");
			CHECK(_l_matcher.passed() == false);
			CHECK(_l_matcher.get_precedence() == _a_pred);
			test_runner _l_t1;
			_l_matcher.run_test(_l_t1);
			if (_a_res)
			{
				CHECK(_l_matcher.get_failure() == "");
				CHECK(_l_matcher.passed() == true);
			}
			else
			{
				CHECK(_l_matcher.get_failure_msg() == _l_cmp_str);
				CHECK(_l_matcher.passed() == false);
			}
			CHECK(_l_matcher.get_failure_msg() == _l_cmp_str);
			CHECK(_l_matcher.get_precedence() == _a_pred);
		}
	}
}

TEST_CASE("EXPR macro tests", "ab::test")
{
	using namespace ab::test::cmp;
	using namespace ab::test;
	using enum comparison_enum_t;
	for (auto& [_l_arg1, _l_arg2, _l_eq_res, _l_neq_res, _l_lt_res, _l_leq_res,
		_l_gt_res, _l_geq_res, _l_l_str, _l_r_str] : get_tests4())
	{
		run_test4<EQ>(_l_arg1, _l_arg2, _l_eq_res, _l_l_str, _l_r_str, 10, "==");
		run_test4<NEQ>(_l_arg1, _l_arg2, _l_neq_res, _l_l_str, _l_r_str, 10, "!=");
		run_test4<LT>(_l_arg1, _l_arg2, _l_lt_res, _l_l_str, _l_r_str, 9, "<");
		run_test4<LEQ>(_l_arg1, _l_arg2, _l_leq_res, _l_l_str, _l_r_str, 9, "<=");
		run_test4<GT>(_l_arg1, _l_arg2, _l_gt_res, _l_l_str, _l_r_str, 9, ">");
		run_test4<GEQ>(_l_arg1, _l_arg2, _l_geq_res, _l_l_str, _l_r_str, 9, ">=");
	}
}