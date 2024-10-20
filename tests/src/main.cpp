#include <catch2/catch_session.hpp>
#include "abc_test_test/internal/errors/test_assertion_exception.h"
#include "abc_test_test/test_utilities/basic_example_test_script.h"
#include <iostream>

int main(int argc, char* argv[]) {
    int result = Catch::Session().run(argc, argv);
    return result;
}