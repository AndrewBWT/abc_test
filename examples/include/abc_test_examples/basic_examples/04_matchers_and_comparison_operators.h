#pragma once
#include "abc_test/included_instances/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions/macros.h"
#include "abc_test/core/ds/test_data/macros.h"
#include "abc_test/core/matchers/matcher_wrapper.h"

#include "abc_test/included_instances/matchers/ranges.h"

#include <ranges>
#include <numeric>


#include "abc_test/core/matchers/comparison.h"

#include "abc_test/core/logging/log_msg.h"

_TEST_CASE(abc::test_data_t({
	.name = "file_04_example_01",
	.description = "Examples revisiting comparision matchers",
	.path = "examples::basic_examples::04_matchers_and_comparison_operators"
	}))
{
	using namespace abc;
	/*!
	* In 01_assertions, we showed the reader a matcher called eq. This
	* tested for equality between elements, calling the in-built equals
	* operator.
	* 
	* There are many other similarly defined matchers, as shown below.
	*/
	_CHECK(abc::eq(1, 1));
	_CHECK(eq(1, 2));
	_CHECK(gt(1, 2));
	_CHECK(lt(2, 1));
	_CHECK(geq(1, 2));
	_CHECK(leq(2, 1));
	_CHECK(annotate(neq(1, 1), "These two numbers aren't equal!"));
}

_TEST_CASE(abc::test_data_t({
	.name = "file_04_example_02",
	.description = "Examples showing comparison matchers using operators",
	.path = "examples::basic_examples::04_matchers_and_comparison_operators"
	}))
{
	using namespace abc;
	using namespace std;
	/*!
	* In abc_test, we have included the functionality to write these comparison
	* based matchers using the comparison operators; that is ==, !=, >, <, >= and <=.
	* 
	* For several reasons, to do so we have to wrap these expressions in an additional macro, called _EXPR. 
	* 
	* Below we show some examples
	*/
	_CHECK(_EXPR(1 != 1));
	_CHECK(_EXPR(1 == 2));
	_CHECK(_EXPR(1 > 2));
	_CHECK(_EXPR(1 > 2));
	_CHECK(_EXPR(1 >= 2));
	_CHECK(_EXPR(1 <= 2));
	/*!
	* We also include _CHECK_EXPR and _REQUIRE_EXPR, which negate
	* requring two macros.
	*/
	_CHECK_EXPR(1 != 2);
	_REQUIRE_EXPR(1 == 1);
	/*!
	* Remember that some checks have to be done more carefully;
	* 
	* _CHECK(_EXPR("1" == "2")) sees both arguments as char arrays.
	* 
	* When you want the types of the arguments to be exact, it is worth
	* using the named constructors. 
	*/
	matcher_wrapper_t _l_1 = abc::eq<string, string>("1", "2");
	/*!
	* But remember that commas in macros create issues. So wrap them in
	* brackets when using macros.
	*/
	_l_1 = _MATCHER((abc::eq<string, string>("1", "2")));
	_CHECK(_l_1 || (abc::eq<string, string>("2", "3")));
}

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
	__no_constexpr_imp
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

_TEST_CASE(abc::test_data_t({
	.name = "file_04_example_03",
	.description = "Using own types with comparison operators",
	.path = "examples::basic_examples::04_matchers_and_comparison_operators"
	}))
{
	using namespace abc;
	using namespace std;
	/*!
	* Above are two types; Test and Test2. Both provide spaceship operator implementations,
	* so they can be used with all of the comparison operators. 
	* 
	* Test2 contains a fmt::format implementation. It is this implementation which 
	* allows print messages to be printed out.
	* 
	* Below we show several examples
	*/
	using namespace testing;
	std::array<int, 3> _l_arr1 = { 100,200,300 };
	std::array<int, 3> _l_arr2 = { 400,500,600 };
	Test _l_t1{ _l_arr1 };
	Test _l_t2{ _l_arr2 };
	/*!
	* The matcher's output message will be in the form [?] operator [?], as there
	* is no fmt::format specialisation of the type Test.
	*/
	_CHECK_EXPR(_l_t1 == _l_t2);
	_CHECK_EXPR(_l_t1 != _l_t2);
	_CHECK_EXPR(_l_t1 < _l_t2);
	_CHECK_EXPR(_l_t1 > _l_t2);

	Test2 _l_t3{ _l_arr1 };
	Test2 _l_t4{ _l_arr2 };
	/*!
	* The matchers for this example will be correct.
	*/
	_CHECK_EXPR(_l_t3 == _l_t4);
	_CHECK_EXPR(_l_t3 != _l_t4);
	_CHECK_EXPR(_l_t3 < _l_t4);
	_CHECK_EXPR(_l_t3 > _l_t4);
}