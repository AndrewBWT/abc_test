#pragma once

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/matcher/comparison/comparison_enum.h"

namespace
{
	using unit_test = std::tuple<int, int, bool, bool, bool, bool, bool, bool>;
	std::vector<unit_test> get_tests_()
	{
		return
		{
			{1,1,true,false,false,true,false,true},
			{1,2,false,true,true,true,false,false},
			{2,1,false,true,false,false,true,true},
			{2,2,true,false,false,true,false,true}
		};
	}
	template<
		ab::test::cmp::comparison_enum_t Comparison_Enum
	>
	void
		cmp_test(int& _a_elem_l, int& _a_elem_r, bool _a_res)
	{
		using namespace ab::test::cmp;
		CHECK(cmp<int, int, Comparison_Enum>(int(_a_elem_l),int(_a_elem_r)) == _a_res);
		CHECK(cmp<int&, int, Comparison_Enum>(_a_elem_l, int(_a_elem_r)) == _a_res);
		CHECK(cmp<int, int&, Comparison_Enum>(int(_a_elem_l), _a_elem_r) == _a_res);
		CHECK(cmp<int&, int&, Comparison_Enum>(_a_elem_l, _a_elem_r) == _a_res);
	}
}

TEST_CASE("Comparison enum tests", "ab::test")
{
	using namespace std;
	using namespace ab::test::cmp;
	using enum comparison_enum_t;
	for (auto& [_l_lval, _l_rval, _l_eq_res, _l_neq_res, _l_lt_res, _l_leq_res, _l_gt_res, _l_geq_res] : get_tests_())
	{
		cmp_test<EQ>(_l_lval, _l_rval, _l_eq_res);
		cmp_test<NEQ>(_l_lval, _l_rval, _l_neq_res);
		cmp_test<LT>(_l_lval, _l_rval, _l_lt_res);
		cmp_test<LEQ>(_l_lval, _l_rval, _l_leq_res);
		cmp_test<GT>(_l_lval, _l_rval, _l_gt_res);
		cmp_test<GEQ>(_l_lval, _l_rval, _l_geq_res);
	}


	CHECK(precedence<EQ>() == 10);
	CHECK(precedence<NEQ>() == 10);
	CHECK(precedence<LT>() == 9);
	CHECK(precedence<LEQ>() == 9);
	CHECK(precedence<GT>() == 9);
	CHECK(precedence<GEQ>() == 9);

	CHECK(string(str<EQ>()) == "==");
	CHECK(string(str<NEQ>()) == "!=");
	CHECK(string(str<LT>()) == "<");
	CHECK(string(str<LEQ>()) == "<=");
	CHECK(string(str<GT>()) == ">");
	CHECK(string(str<GEQ>()) == ">=");
}