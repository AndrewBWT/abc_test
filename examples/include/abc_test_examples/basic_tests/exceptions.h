#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

_TEST_CASE("Example showing what happens when an exception is thrown", "examples::basic_assertions")
{
	using namespace abc;
	/*!
	* This code shows what happens in the output when an exception is thrown from a test.
	*/
	_CHECK(eq(1, 1));
	_CHECK(eq(1, 2));
	throw std::runtime_error("the runtime exception thrown");
}

_TEST_CASE("Example showing what happens when a non-exception entity is thrown", "examples::basic_assertions")
{
	using namespace abc;
	/*!
	* This code shows what happens in the output when an exception is thrown from a test.
	*/
	_CHECK(eq(1, 1));
	_CHECK(eq(1, 2));
	throw std::size_t{ 1 };
}