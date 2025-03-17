#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include "abc_test_test/core.hpp"
#include "abc_test_test/included_instances.hpp"
#include "abc_test_test/utility.hpp"

namespace xi
{
    struct XI
    {
        virtual auto get_element() -> std::size_t = 0;
    };
}
int main(
        int   argc,
        char* argv[]
    )
{
    return abc::run_test_suite_using_command_line_args(argc, argv);
}