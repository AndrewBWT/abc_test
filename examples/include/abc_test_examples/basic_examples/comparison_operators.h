#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/comparison.h"

#include "abc_test/core/log_test_msg.h"

namespace testing
{
	struct Test
	{
		std::array<int, 3> _m_array;
		auto operator<=>(const Test& _a_obj) const = default;
	};
	struct Test2
	{
		std::array<int, 3> _m_array;
		auto operator<=>(const Test2& _a_obj) const = default;
	};
}

template
<
>
struct fmt::formatter<testing::Test2> : formatter<string_view>
{
	__constexpr_imp
		auto
		format(
			testing::Test2 _a_rd,
			format_context& _a_cxt
		) const
		->format_context::iterator
	{
		using namespace std;
		string _l_str{ fmt::format("{}",_a_rd._m_array) };
		return formatter<string_view>::format(_l_str, _a_cxt);
	}
};

_TEST_CASE("Examples using the comparative operators.", "examples::basic_assertions")
{
	using namespace abc;
	//This will pass.
	_CHECK(EXPR(1 == 1));
	//All of these will fail.
	_CHECK(EXPR(1 == 2));
	_CHECK(EXPR(1 > 2));
	_CHECK(EXPR(2 < 1));
	_CHECK(EXPR(1 >= 2));
	_CHECK(EXPR(1 <= 2));
	_CHECK(EXPR(1 != 1));
	//We also include named constructors in matchers/comparison/constructros.h
	_CHECK(abc::eq(1,1));
	_TLOG("LOGGING");
	_CHECK(eq(1,2));
	_CHECK(gt(1, 2));
	_CHECK(lt(2, 1));
	_CHECK(geq(1, 2));
	_CHECK(leq(2, 1));
	_CHECK_WITH_MSG(neq(1, 1),"These two numbers aren't equal!");

	//How it works.
	/*!
	* The EXPR(A CMP B) (where A and B are elements to compare, and CMP is the comparison operator)
	* macro creates an expression in the form abc::placeholder() < A CMP B.
	*
	* Due to how operators are handled by C++, this becomes (abc::placeholder() < A) CMP B.
	* The operator < for abc::placeholder and any T (located in matchers/comparison/operators.h)
	* creates a dummy object of type wrapper<T>. So the expression now reads:
	*
	* wrapper<decltype(A)> CMP B
	*
	* All the operators which we can use in EXPR expressions are defined as in-class functions of
	* the wrapper class. It then simply calls the built-in comparison's, and runs that code.
	*
	* Together with all the machinery to store the line representation.
	*/

	//Note the following won't work. _CHECK must take something of type generic_type*, which
	// is not returned from 'a'=='b'. 
	//_CHECK('a' == 'b');

	//However, we do provide a simplified solution.
	_CHECK_EXPR(1 == 2);

	//Finally, to show these work with requires.
	_REQUIRE(EXPR(1 == 1));
	_REQUIRE_EXPR(1 == 2);
}

_TEST_CASE("Examples using user-defined type", "examples::basic_assertions")
{
	/*!
	* Note that we defined the spaceship operator, and can do all of these comparisons.
	* 
	* The type Test is not formattable, so it cannot be printed. 
	*/
	using namespace testing;
	Test _l_t1;
	_l_t1._m_array[0] = 100;
	_l_t1._m_array[1] = 200;
	_l_t1._m_array[2] = 300;
	Test _l_t2;
	_l_t2._m_array[0] = 400;
	_l_t2._m_array[1] = 500;
	_l_t2._m_array[2] = 600;
	_CHECK_EXPR(_l_t1 == _l_t2);
	_CHECK_EXPR(_l_t1 != _l_t2);
	_CHECK_EXPR(_l_t1 < _l_t2);
	_CHECK_EXPR(_l_t1 > _l_t2);
	/*!
	* However type Test2 is formattable, so it can be printed.
	*/
	Test2 _l_t3;
	_l_t3._m_array[0] = 100;
	_l_t3._m_array[1] = 200;
	_l_t3._m_array[2] = 300;
	Test2 _l_t4;
	_l_t4._m_array[0] = 400;
	_l_t4._m_array[1] = 500;
	_l_t4._m_array[2] = 600;
	_CHECK_EXPR(_l_t3 == _l_t4);
	_CHECK_EXPR(_l_t3 != _l_t4);
	_CHECK_EXPR(_l_t3 < _l_t4);
	_CHECK_EXPR(_l_t3 > _l_t4);
}

_TEST_CASE("Examples using named constructors", "examples::basic_assertions")
{
	using namespace abc;
	//We also include named constructors in matchers/comparison/constructros.h
	_CHECK(abc::eq(1, 1));
	_CHECK(eq(1, 2));
	_CHECK(gt(1, 2));
	_CHECK(lt(2, 1));
	_CHECK(geq(1, 2));
	_CHECK(leq(2, 1));
	_CHECK(neq(1, 1));
}