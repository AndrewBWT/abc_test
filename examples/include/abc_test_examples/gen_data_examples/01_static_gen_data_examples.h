#pragma once
#include "abc_test/core/data_generator/data_generator_collection_iterator.h"
#include "abc_test/core/ds/test_data/macros.h"
#include "abc_test/core/test_assertions/macros.h"
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/gen_data/static_data.h"
#include "abc_test/matchers/comparison.h"
#include "abc_test/matchers/comparison/constructors.h"
#include "abc_test/matchers/matcher.h"
#include "abc_test/matchers/ranges.h"

#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>

_TEST_CASE(
    abc::test_data_t(
        {.name             = "file_01_example_01",
         .description      = "A simple static gen data examples",
         .path             = "examples::gen_data_examples::static_gen_data",
         .threads_required = 2}
    )
)
{
    using namespace abc;
    using namespace std;
    using test_data_t        = pair<int, int>;
    vector<test_data_t> _l_x = {
        {1,  1  },
        {1,  2  },
        {1,  3  },
        {1,  5  },
        {2,  2  },
        {24, 243}
    };
    for (auto&& [_l_int1, _l_int2] :
         iterate_over<test_data_t, vector<test_data_t>>({}))
    {
        _CHECK(annotate(_EXPR(_l_int1 == _l_int2), "Testing _l_int1 == _l_int2")
        );
    }
    for (auto&& [_l_int1, _l_int2] : iterate_over<test_data_t>({
             {1,      2     },
             {2,      3     },
             {4,      51    },
             {19'956, 27'234}
    }))
    {
        _CHECK(annotate(_EXPR(_l_int1 == _l_int2), "Testing _l_int1 == _l_int2")
        );
    }

    for (auto&& [_l_int1, _l_int2] : 
        data_generator_collection<test_data_t>(
             iterate_over<test_data_t>({
                 {1,      2     },
                 {2,      3     },
                 {4,      51    },
                 {19'956, 27'234}
             }),
             iterate_over<test_data_t>(_l_x)
        ))
    {
        _CHECK(annotate(_EXPR(_l_int1 == _l_int2), "Testing _l_int1 == _l_int2")
        );
    }

    for (auto&& [_l_int1, _l_int2] : data_generator_collection<test_data_t>())
    {
        _CHECK(annotate(_EXPR(_l_int1 == _l_int2), "Testing _l_int1 == _l_int2")
        );
    }
}