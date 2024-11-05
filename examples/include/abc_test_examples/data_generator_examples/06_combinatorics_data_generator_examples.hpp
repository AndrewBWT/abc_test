#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_data_t(
        {.name        = "file_06_example_01",
         .description = "Combinatoris examples.",
         .path        = "examples::gen_data_examples::manual_data_generator",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using test_data_t = int;
    vector<test_data_t> _l_init = { 1,2,3,1 };
    for (auto& _l_vect : using_combinatorics_generator<test_data_t>(_l_init))
    {
        std::cout << fmt::format("{0}",_l_vect) << std::endl;
    }
    std::cout << "<false,false,false>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, false, false, false>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }
    std::cout << "<false,true,false>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t,false,true,false>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }
    std::cout << "<true,false,false>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, true, false, false>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }
    std::cout << "<false,false,true>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, false, false, true>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }

    std::cout << "<true,false,true>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, true, false, true>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }
    std::cout << "<false,true,true>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, false, true, true>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }
    std::cout << "<true,true,false>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, true, true, false>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }

    std::cout << "<true,true,true>" << std::endl;
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, true, true, true>(_l_init))
    {
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }

    std::cout << "<true,true,true>" << std::endl;
    vector<test_data_t> _l_vector = { 1, 2, 3, 4, 5 };
    for (auto& _l_vect : using_combinatorics_generator<test_data_t, true, true, true>(_l_init,tertiary_data_file("hugh")))
    {
        _CHECK(_EXPR(_l_vect == _l_vector));
        std::cout << fmt::format("{0}", _l_vect) << std::endl;
    }
}
