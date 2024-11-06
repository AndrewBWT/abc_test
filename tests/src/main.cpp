#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_test/internal/ds/test_data/user_defined_test_data.hpp"

int main(
        int   argc,
        char* argv[]
    )
{
    return abc::run_test_suite_using_command_line_args(argc, argv);
}