#include "main.h"
#include <iostream>
//#include <scn/all.h>

int main(int argc, char* argv[]) {
    // your setup ...
    fmt::println("hello");
    int result = Catch::Session().run(argc, argv);
    // your clean-up...
    return result;
}