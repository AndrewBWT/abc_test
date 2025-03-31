#pragma once
 #include "abc_test/core.hpp"
 #include "abc_test/included_instances.hpp"

 #include "abc_test_test/core.hpp"
 #include "abc_test_test/included_instances.hpp"
 #include "abc_test_test/utility.hpp"
#include "windows.h"

#include <cuchar>
#include <fmt/xchar.h>
#include <iostream>

int
    main(
        int   argc,
        char* argv[]
    )
{
    SetConsoleOutputCP(CP_UTF8);
     return abc::run_test_suite_using_command_line_args(argc, argv);
}