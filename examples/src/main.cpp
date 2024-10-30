#pragma once
#include "abc_test/included_instances/test_options/included_instances_test_options.h"
#include "abc_test/internal/test_main.h"

// All tests
// #include "abc_test_examples/enumeration.h"
#include "abc_test/included_instances/reporters/text_error_reporter.h"
#include "abc_test/included_instances/reporters/text_test_reporter.h"
//#include "abc_test_examples/basic_examples.h"
#include "abc_test_examples/gen_data_examples.h"
#include "abc_test/included_instances/test_options/command_line_parser.h"

int
    main(
        int   argc,
        char* argv[]
    )
{
    _CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
    using namespace abc;
    using namespace std;
    using namespace _ABC_NS_REPORTERS;
    using namespace _ABC_NS_DS;
    using namespace _ABC_NS_UTILITY;
    included_instances_test_options_t _l_iito;
    CLI::App& _l_app{*build_command_line_parser(_l_iito) };
    CLI11_PARSE(_l_app, argc, argv);
    _l_iito.threads = 1;
    auto _l_validated_test_options{validated_test_options_t<
        included_instances_test_options_t>::validate_test_options(_l_iito)};
    if (_l_validated_test_options.has_value())
    {
        test_main_t _l_test_main(_l_validated_test_options.value());
        _l_test_main.run_tests();
    }
    else
    {
        str::string_table_t _l_st({1});
        for (size_t _l_idx{0};
             string & _l_error : _l_validated_test_options.error())
        {
            _l_st.push_back(fmt::format(" {0})  ", _l_idx++));
            _l_st.push_back(_l_error);
            _l_st.new_line();
        }
       // std::cout << fmt::format(
       //     "Error(s) encountered when validating test_options_t. "
        //    "The following errors were returned from the validation "
        //    "function:\n{0}\n"
        //    "The program will now terminate. test_options_base_t = {1}",
        //    _l_st(),
       //     _l_to
       // );
    }
}