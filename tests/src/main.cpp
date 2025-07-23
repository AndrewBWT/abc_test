 #include "abc_test/core.hpp"
 #include "abc_test/included_instances.hpp"

 #include "abc_test_test/core.hpp"
 #include "abc_test_test/included_instances.hpp"
 #include "abc_test_test/utility.hpp"
#include "abc_test_test/component_tests.hpp"
 #if defined(_WIN32)
#include "windows.h"
#endif

#include <cuchar>
#include <fmt/xchar.h>
#include <iostream>

int
    main(
        int   argc,
        char* argv[]
    )
{
	 #if defined(_WIN32)
    SetConsoleOutputCP(CP_UTF8);
    #endif
     return abc::run_test_suite_using_command_line_args(argc, argv);
}
