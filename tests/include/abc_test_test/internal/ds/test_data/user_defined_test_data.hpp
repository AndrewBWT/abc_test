#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_data_t(
        {.name        = "Test equality of user_defined_test_data_t",
         .description = "Checks that the equality of user_defined_test_data_t "
                        "performs as expected.",
         .path = "abc_test::internal::ds::test_data::user_defined_test_data",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::ds;
    using test_data_t = tuple<string, optional<string>, string, size_t>;
    for (auto& _l_test_name : generate_data_randomly<string>())
    {
        for (auto& _l_test_description :
             generate_data_randomly<optional<string>>())
        {
            for (auto& _l_path : generate_data_randomly<string>())
            {
                for (auto& _l_n_threads : generate_data_randomly<std::size_t>())
                {
                    user_defined_test_data_t _l_udtd{
                        _l_test_name, _l_test_description, _l_path, _l_n_threads
                    };
                    user_defined_test_data_t _l_udtd2{
                        _l_test_name, _l_test_description, _l_path, _l_n_threads
                    };
                    _CHECK(_EXPR(_l_udtd == _l_udtd2));

                    string _l_rv{ fmt::format("{}",_l_udtd) };
                }
            }
        }
    }
}