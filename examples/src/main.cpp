#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

// #include "abc_test_examples/basic_examples.hpp"
#include "abc_test_examples/documentation.hpp"
//#include "abc_test_examples/data_generator_examples.hpp"

/*constexpr int
    fib(
        int n
    )
{
    if (n <= 1)
    {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Example 1", .path = "examples::basic_examples::001"}
    )
)
{
    _CHECK_EXPR(fib(6) == 8);
    _CHECK_EXPR(fib(7) == 14);
}

_TEST_CASE(
    abc::test_case_t(
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
             {15, 610},
             {16, 988}
    }))
    {
        _CHECK_EXPR(fib(_l_input) == _l_output);
    }
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Example 3", .path = "examples::basic_examples::001"}
    )
)
{
    using namespace std;
    using namespace abc;
    using test_data_t = pair<int, int>;
    for (auto& [_l_input, _l_output] : read_data_from_file<test_data_t>(
             general_data_file("file1"),
             {
                 {6,  8  },
                 {7,  14 },
                 {8,  21 },
                 {12, 144},
                 {15, 610},
                 {16, 988}
    }
         ))
    {
        _CHECK_EXPR(fib(_l_input) == _l_output);
    }
}

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
    abc::test_case_t(
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

_TEST_CASE(
    abc::test_case_t(
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
            return _l_rv;
        }
    };
    for (auto& _l_input : enumerate_data<test_data_t>(from_m_to_n(0,5000)))
    {
        _CHECK(annotate(
            _EXPR(all_factors(_l_input) == slow_all_factors(_l_input)),
            fmt::format("_l_input = {0}", _l_input)
        ));
    }
}*/

int
    main(
        int   argc,
        char* argv[]
    )
{
    using namespace abc;
    return run_test_suite_using_command_line_args(argc, argv);
}
