#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

namespace t
{
	struct ab
	{
		int x;
	};
	struct bc : public ab
	{
		std::string yx;
	};
	inline void run(const ab& x)
	{
		std::cout << typeid(ab).name() << std::endl;
	}
	template<
		bool B
	>
	struct x
	{
		std::conditional_t<B, int, void*> _l_member;
		template< typename = typename std::enable_if<B>::type>
		int get_int()
		{
			return _l_member;
		}
	};
}

_TEST_CASE("Example showing _CHECK_NO_EXCEPTION_BEGIN and END", "examples::basic_tests")
{
	t::x<true> l1;
	l1.get_int();
	t::ab l2;
	t::run(l2);
	t::bc l3;
	t::run(l3);
//	using namespace abc;
//	_BEGIN_BLOCK(except, true, true);
//	try
//	{
//		_CHECK(eq(1,2));
//	}
//	catch (std::exception& _l_e)
	//{
	//}
	//_END_BLOCK(except);
	//_END()
}

_TEST_CASE("Example showing what happens when a non-exception entity is thrown", "examples::basic_assertions")
{
	using namespace abc;
}