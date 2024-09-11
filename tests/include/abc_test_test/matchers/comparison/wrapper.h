#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/matcher/comparison/wrapper.h"
#include <type_traits>


TEST_CASE("Wrapper tests", "ab::test")
{
	using namespace ab::test::cmp;
	wrapper<int> _l_pl_1(5);
	if (true)
	{
		_l_pl_1 = wrapper<int>(5);
	}
	CHECK(std::is_lvalue_reference<decltype(_l_pl_1.object())>::value);
	CHECK(_l_pl_1.object() == 5);
	int _l_pl_2_arg = 10;
	wrapper<int&> _l_pl_2(_l_pl_2_arg);
	CHECK(_l_pl_2.object() == 10);
	CHECK(std::is_lvalue_reference<decltype(_l_pl_1.object())>::value);
	_l_pl_2_arg = 20;
	CHECK(_l_pl_2.object() == 20);
}