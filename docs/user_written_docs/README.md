# abc test #

abc_test is a C++ testing library. 

abc_test's core features are:
- It is a matcher-based testing library, similar to GTest or Catch2. Like those other testing libraries abc_test's matchers can be extended by the user. abc_test's matchers can also be composed together with logical operators.
- abc_test encourages the use of specially formulated data generators. These allow the user to loop through data using a for loop. We include data generators based around static data, data read from a file, randomly generated data, or based around the enumeration of a data type's values.
- abc_test uses a system for repeating failing test cases which takes into account data generators - when prompted, the data generators will only generate those data-values which previously caused a test failure. This allows the user to concentrate on the error itself, rather than configuring the test-suite to isolate the error.
- The included data generators come built in with the ability to write problematic data values to files. This allows the user to retain problematic data either for use in other tests, or to ensure that the data-value is always ran on the test.
- We allow the user to run their test-code in parallel.

This is a passion/personal project currently, and I would really appreciate any feedback. If any of these features have piqued your interest, please see below for some more detailed examples!

# Basic example

Below is a basic but complete example, which shows how the library can be used to test the output of the fibonacci function.
```
#include "abc_test/core.h"
#include "abc_test/included_instances.h"
constexpr int fib(int n) {
    if (fib <= 1) {return n;}
    return fib(n - 1) + fib(n - 2);
}

_TEST_CASE(
    abc::test_data_t{
        .name        = "Example 1",
        .path        = "examples::basic_examples::001"
    }
)
{
    _CHECK_EXPR(fib(6) == 8);
	_CHECK_EXPR(fib(7) == 14);
}

int
    main(
        int   argc,
        char* argv[]
    )
{
    return abc::run_test_suite_using_command_line_args(argc, argv);
}

```
Let us talk through each part of the code:
- The test function itself is created using the macro ```_TEST_CASE```. This test has two associated data-points with it; a name and a path. While the name is self explanatory, the path can be thought of as a folder for the test. We use it to organise tests into groups which may not reflect their source code position.
- The macro ```_CHECK_EXPR``` calls code which runs the assertions. This specific macro is able to process comparison operators. This allow the user to write human-readable tests using abc test. 
- The main calls a function which processes the command line arguments.

Below is the output from this test.

[Test output](docs/output1.png)

In the remaining examples in this README, we will omit the ```main``` portion of the code.

# Data generators

Below we show code which tests the same ```fib``` function. However, it uses a collection to store the values.

```
_TEST_CASE(
    abc::test_data_t(
        {.name = "Example 2", .path = "examples::basic_examples::001"}
    )
)
{
    using namespace std;
    using namespace abc;
    using test_data_t = pair<int, int>;
    for (auto& [_l_input, _l_output] : iterate_over<test_data_t>({
             {6,  8  },
             {7,  14 },
             {8,  21 },
             {12, 144},
             {15, 610}
    }))
    {
        _CHECK_EXPR(fib(_l_input) == _l_output);
    }
}

```

This runs the same tests as the first example, however we have been able to add more test cases. These are easily amended, allowing the user to test more values quickly without having to change the code drastically.

There is also another data generator based on a file, which reads the test data from the file instead of from the source code. Below is an example using this generator.

```
_TEST_CASE(
    abc::test_data_t(
        {.name = "Example 2", .path = "examples::basic_examples::001"}
    )
)
{
    using namespace std;
    using namespace abc;
    using test_data_t = pair<int, int>;
    for (auto& [_l_input, _l_output] :
         read_data_from_file<test_data_t>(general_data_file("file1")))
    {
        _CHECK_EXPR(fib(_l_input) == _l_output);
    }
}

```

Where ```file1``` looks like this

```
#struct std::pair<int,int>
(6, 8)
(7, 14)
(8, 21)
(12, 144)
(15, 610)
(16, 988)

```

This way, the tests can be written to the file. It defaults to using the scn and fmt libraries to do this.

We can also write tests using random generators. The below example tests an algorithm to find all factors of a given number. The testing code then randomly generates integers to check the results.

```
inline std::set<int>
    all_factors(
        const int _a_element
    )
{
    using namespace std;
    set<int> _l_all_factors;
    for (int _l_idx{1}; _l_idx <= sqrt(_a_element); ++_l_idx)
    {
        if ((_a_element % _l_idx) == 0)
        {
            int _l_divisor{_a_element / _l_idx};
            _l_all_factors.insert(_l_idx);
            if (_l_divisor != _l_idx)
            {
                _l_all_factors.insert(_l_divisor);
            }
        }
    }
    return _l_all_factors;
}

_TEST_CASE(
    abc::test_data_t(
        {.name = "Example 4", .path = "examples::basic_examples::001"}
    )
)
{
    using namespace std;
    using namespace abc;
    using test_data_t = int;
    function<set<int>(const int)> slow_all_factors{
        [](const int _a_input)

        {
            set<int> _l_rv;
            if (_a_input > 0)
            {
                _l_rv.insert(_a_input);
            }
            for (int i = 1; i <= _a_input / 2; ++i)
            {
                if ((_a_input % i) == 0)
                {
                    _l_rv.insert(i);
                }
            }
            return _l_rv;
        }
    };
    for (auto& _l_input : generate_data_randomly<test_data_t>())
    {
        _CHECK(annotate(
            _EXPR(all_factors(_l_input) == slow_all_factors(_l_input)),
            fmt::format("_l_input = {0}", _l_input)
        ));
    }
}
```

When writing this example, the code went through several revisions as edge-cases were caught.

We can also use an enumeration data generator. In the example below, we do this for values from 0 to 5000. We actually used this to find the a bug in our code which was not obvious when using random data.

```
_TEST_CASE(
    abc::test_data_t(
        { .name = "Example 5", .path = "examples::basic_examples::001" }
    )
)
{
    using namespace std;
    using namespace abc;
    using test_data_t = int;
    function<set<int>(const int)> slow_all_factors{
        [](const int _a_input)

        {
            set<int> _l_rv;
            if (_a_input > 0)
            {
                _l_rv.insert(_a_input);
            }
            for (int i = 1; i <= _a_input / 2; ++i)
            {
                if ((_a_input % i) == 0)
                {
                    _l_rv.insert(i);
            }
        }
    };
    for (auto& _l_input : enumerate_data<test_data_t>(from_m_to_n(0,5000)))
    {
        _CHECK(annotate(
            _EXPR(all_factors(_l_input) == slow_all_factors(_l_input)),
            fmt::format("_l_input = {0}", _l_input)
        ));
    }
}
```

There are lots more features in abc_test. We haven't covered logic operators for matchers, automatic file reading/writing, testing for exceptions. 
