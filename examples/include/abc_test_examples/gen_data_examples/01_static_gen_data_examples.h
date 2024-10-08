#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/macros.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/comparison/constructors.h"

#include "abc_test/matchers/ranges.h"

#include <ranges>
#include <numeric>

#include <tuple>
#include <vector>

#include "abc_test/gen_data/static_data.h"

#include "abc_test/gen_data/collection_iterator.h"
#include "abc_test/matchers/comparison.h"


_TEST_CASE(abc::test_data_t({
	.name = "file_01_example_01",
	.description = "A simple static gen data examples",
	.path = "examples::gen_data_examples::static_gen_data",
	.threads_required = 2
	}))
{
	using test_data_t = std::pair<int, int>;
	std::vector<test_data_t> _l_x =
	{
		{1,1},{1,2},{1,3},{1,5},{2,2},{24,243}
	};
	for (auto&& [_l_int1, _l_int2] : abc::iterate_over<test_data_t>(_l_x))
	{
		_CHECK(annotate(_EXPR(_l_int1 == _l_int2), "Testing _l_int1 == _l_int2"));
	}
}