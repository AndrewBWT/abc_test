#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_test/included_instances/data_generator/enumeration/default_enumeration.hpp"

int main(
        int   argc,
        char* argv[]
    )
{
    return abc::run_test_suite_using_command_line_args(argc, argv);
}