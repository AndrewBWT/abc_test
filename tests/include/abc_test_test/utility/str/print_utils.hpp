#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test/utility/str/concepts.hpp"

namespace nameity
{
class X
{};

struct Y
{};

template <typename T>
struct Z
{};
} // namespace nameity

_TEST_CASE(
    abc::test_case_t(
        {.name             = "Tests for type_id",
         .path             = "abc_test_test::utility::str::print_utils",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace abc::data_gen;
    using namespace std;
    using namespace utility;
    using namespace abc::utility::io;
    using namespace _ABC_NS_UTILITY_STR;
    using namespace nameity;
    auto _l_func = [&]<typename T>(const std::u8string& _a_str)
    {
        _CHECK_EXPR(type_id<T>() == _a_str);
    };
    manual_data_generator_t _l_mdg;
    RUN(_l_mdg, (_l_func.operator()<char>(u8"char")));
    RUN(_l_mdg, (_l_func.operator()<X>(u8"class nameity::X")));
    RUN(_l_mdg, (_l_func.operator()<Y>(u8"struct nameity::Y")));
    RUN(_l_mdg,
        (_l_func.operator()<Z<X>>(u8"struct nameity::Z<class nameity::X>")));
    RUN(_l_mdg, (_l_func.operator()<int>(u8"int")));
}