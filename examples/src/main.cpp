#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#include "abc_test_examples/documentation.hpp"
#include "abc_test_examples/readme.hpp"
int
    main(
        int   argc,
        char* argv[]
    )
{
    using namespace abc;
    return run_test_suite_using_command_line_args(argc, argv);
}
