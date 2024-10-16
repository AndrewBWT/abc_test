#pragma once
#include "abc_test/core/options/validated_test_options.h"
#include "abc_test/core/test_main.h"

// All tests
//#include "abc_test_examples/enumeration.h"
#include "abc_test_examples/basic_examples.h"
#include "abc_test_examples/gen_data_examples.h"

int main(int argc, char* argv[])
{
    _CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
    using namespace abc;
    using namespace std;
    using namespace abc::reporters;
    using namespace abc::ds;
    test_options_base_t _l_to;
    _l_to.threads = 1;
    auto _l_validated_test_options{ validated_test_options_t::validate_test_options(_l_to) };
    if (_l_validated_test_options.has_value())
    {
        test_main_t _l_test_main(_l_validated_test_options.value());
        _l_test_main.run_tests();
    }
    else
    {
        std::cout << fmt::format("Error(s) encountered when validating test_options_t. "
            "The following string was returned from the validation function: \"{0}\". "
            "The program will now terminate.. test_options_base_t = {1}",
            _l_validated_test_options.error(), _l_to);
    }
}