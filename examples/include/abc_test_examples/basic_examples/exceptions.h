#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/macros.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

_TEST_CASE("", "examples::basic_tests")
{

}

_TEST_CASE("Example showing what happens when a non-exception entity is thrown", "examples::basic_assertions")
{
	using namespace abc;
	throw int(2);
}


_TEST_CASE("Example showing what happens when an exception entity is thrown", "examples::basic_assertions")
{
	using namespace abc;
	_CHECK(eq(1, 2));
	throw std::runtime_error("runtime error");
}