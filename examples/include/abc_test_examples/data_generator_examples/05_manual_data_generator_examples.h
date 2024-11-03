#pragma once
#include "abc_test/core.h"
#include "abc_test/included_instances.h"

namespace t1
{
    template<typename T>
    void f1(const std::vector<std::pair<T,T>>& _a_pairs)
    {
        using namespace abc;
        for (const auto& [_l_input, _l_output] : iterate_over<std::pair<T,T>>(_a_pairs))
        {
            _CHECK(_EXPR(_l_input == _l_output));
        }
    }
}

_TEST_CASE(
    abc::test_data_t(
        { .name = "file_05_example_01",
         .description = "Manual data generator examples.",
         .path = "examples::gen_data_examples::manual_data_generator",
         .threads_required = 1 }
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace t1;
    manual_data_generator_t _l_mgd;
    RUN(_l_mgd, (f1<int>({ {1,2} })));
    RUN(_l_mgd, (f1<char>({ {1,2} })));
}
