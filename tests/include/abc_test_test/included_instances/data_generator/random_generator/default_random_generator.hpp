#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"


_TEST_CASE(
    abc::test_case_t(
        { .name = "Testing random_data_generator_t for std::string type",
         .description
         = "abc_test_test::included_instances::data_generator::enumeration",
         .threads_required = 1 }
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace test;
    default_random_generator_t<std::string> _l_rdg;
}