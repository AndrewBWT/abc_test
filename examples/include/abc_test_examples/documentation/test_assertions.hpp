#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t({.name = "Assertion example"})
)
{
    int i = 2;
    // The assertion below passes.
    _CHECK(_EXPR(i == 2));
}

_TEST_CASE(
    abc::test_case_t({.name = "Terminating test"})
)
{
    int i = 2;

    // This assertion will fail and the test will terminate.
    _REQUIRE(_EXPR(i == 1));

    // This assertion will never be ran. If it were to be ran, it would pass.
    _CHECK(_EXPR(i == 2));
}

_TEST_CASE(
    abc::test_case_t({.name = "Simple assertions"})
)
{
    // Will register a success.
    _SUCCEED();
    // Will register a failure.
    _FAIL();
    // Will terminate the test.
    _TERMINATE();
    // Will never be reached.
    _SUCCEED();
}

_TEST_CASE(
    abc::test_case_t({.name = "Simple assertions with output"})
)
{
    // Will register a success.
    _SUCCEED_WITH_MSG("This message will be seen");
    // Will register a failure.
    _FAIL_WITH_MSG("As will this one");
    // Will terminate the test.
    _TERMINATE_WITH_MSG("This one will too!");
    // Will never be reached.
    _SUCCEED_WITH_MSG("This one won't");
}

_TEST_CASE(
    abc::test_case_t({.name = "Simple matcher examples"})
)
{
    using namespace abc;
    // matcher_t is the object which holds the result of a user-defined test. A
    // default matcher has a result value of true.
    matcher_t matcher_1;
    // The function bool_matcher creates a matcher_t whose result is the bool
    // argument given to bool_matcher. matcher_2 has a result value of true.
    matcher_t matcher_2 = bool_matcher(true);
    // matcher_3 has a result value of false.
    matcher_t matcher_3 = bool_matcher(false);
    int       i         = 2;
    // Expressions can be encoded in matchers using the macro _EXPR. matcher_4
    // has a result value of true, as it checks if i==2 (which it does).
    matcher_t matcher_4 = _EXPR(i == 2);
    i                   = 8;
    // matcher_5 has a result value of false, as i <=7 is false.
    matcher_t matcher_5 = _EXPR(i <= 7);
}

_TEST_CASE(
    abc::test_case_t({.name = "Testing assertions"})
)
{
    using namespace abc;
    // This will register as a passed assertion.
    _CHECK(bool_matcher(true));
    // This will register as a failed assertion.
    _CHECK(bool_matcher(false));
    int i = 2;
    // This will register as a passed assertion.
    _CHECK(_EXPR(i == 2));
    // This will register as a failed assertion.
    _CHECK(_EXPR(i == 3));
    // This will register as a passed assertion.
    _REQUIRE(_EXPR(i < 3));
    // This will register as a failed assertion, and the test will terminate.
    _REQUIRE(_EXPR(i >= 3));
    // This test won't be ran.
    _CHECK(_EXPR(i == 2));
}

_TEST_CASE(
    abc::test_case_t({.name = "Testing assertions again"})
)
{
    int i = 2;
    _CHECK_EXPR(i == 2);
    _CHECK_EXPR(i == 3);
    _REQUIRE_EXPR(i < 3);
    _REQUIRE_EXPR(i >= 3);
    _CHECK_EXPR(i == 2);
}

_TEST_CASE(
    abc::test_case_t({.name = "Simple BBA example"})
)
{
    _BEGIN_SINGLE_ELEMENT_BBA(block_example, "A descriptive description");
    block_example = _BLOCK_CHECK(_EXPR(1 == 2));
    block_example = _BLOCK_CHECK(_EXPR(2 == 2));
    _END_BBA_CHECK(block_example);
}

_TEST_CASE(
    abc::test_case_t({.name = "_BLOCK_REQUIRE example"})
)
{
    _BEGIN_SINGLE_ELEMENT_BBA(test1, "Single-element BBA description");
    // This matcher will be assigned to the BBA.
    test1 = _BLOCK_CHECK(_EXPR(1 == 2));
    // But it is overridden by this matcher. After this line the test function
    // will terminate.
    test1 = _BLOCK_REQUIRE(_EXPR(2 == 3));
    // And this matcher is never assigned to the BBA.
    test1 = _BLOCK_CHECK(_EXPR(3 == 4));
    _END_BBA_CHECK(test1);

    _BEGIN_MULTI_ELEMENT_BBA(test2, "Multi-element BBA description");
    // This matcher will be assigned to the BBA.
    test2 += _BLOCK_CHECK(_EXPR(1 == 1));
    // This matcher is also assigned to the BBA. However as its result is false,
    // the test function is immediatley terminated. In the test results, the BBA
    // will only have 2 matchers assigned to it.
    test2 += _BLOCK_REQUIRE(_EXPR(2 == 3));
    // And this matcher is never assigned to the BBA.
    test2 += _BLOCK_CHECK(_EXPR(3 == 4));
    _END_BBA_CHECK(test2);
}

_TEST_CASE(
    abc::test_case_t({.name = "Source modification BBA macro example 1"})
)
{
    _BEGIN_SINGLE_ELEMENT_BBA(test1, "test1 description");
    test1 = _BLOCK_CHECK(abc::bool_matcher(false));
    _END_BBA_CHECK(test1);
}

#define __USER_DEFINED_BBA_1_BEGIN(name)                                   \
    _BEGIN_SINGLE_ELEMENT_BBA(name, fmt::format("{} description", #name)); \
    name = _BLOCK_CHECK(abc::bool_matcher(false));

#define __USER_DEFINED_BBA_1_END(name) _END_BBA_CHECK(name)

_TEST_CASE(
    abc::test_case_t({.name = "USER_DEFINED_BBA_1 example"})
)
{
    __USER_DEFINED_BBA_1_BEGIN(test1);
    __USER_DEFINED_BBA_1_END(test1);
}

#define __USER_DEFINED_BBA_2_BEGIN(name)                \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(            \
        name,                                           \
        fmt::format("{} description", #name),           \
        abc::utility::str::create_string(               \
            {"__USER_DEFINED_BBA_2_BEGIN(", #name, ")"} \
        )                                               \
    )                                                   \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false));

#define __USER_DEFINED_BBA_2_END(name)                \
    _END_BBA_CHECK_CUSTOM_SOURCE(                     \
        name,                                         \
        abc::utility::str::create_string(             \
            {"__USER_DEFINED_BBA_2_END(", #name, ")"} \
        )                                             \
    )

_TEST_CASE(
    abc::test_case_t({.name = "USER_DEFINED_BBA_2 example"})
)
{
    __USER_DEFINED_BBA_2_BEGIN(test1);
    __USER_DEFINED_BBA_2_END(test1);
}

#define __USER_DEFINED_BBA_3(name)                                             \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                                   \
        name,                                                                  \
        fmt::format("{} description", #name),                                  \
        abc::utility::str::create_string({"__USER_DEFINED_BBA_3(", #name, ")"} \
        )                                                                      \
    )                                                                          \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false));                   \
    _END_BBA_CHECK_NO_SOURCE(name)

_TEST_CASE(
    abc::test_case_t({.name = "__USER_DEFINED_BBA_3 example"})
)
{
    __USER_DEFINED_BBA_3(test1);
}

inline void
    test(
        const std::string str
    )
{
    using namespace abc;
    _BEGIN_SINGLE_ELEMENT_BBA(exception_test, "Testing std::stoi function")
    try
    {
        int i          = std::stoi(str);
        exception_test = _BLOCK_CHECK(annotate("Checking i > 0", _EXPR(i > 0)));
    }
    catch (std::invalid_argument const & ex)
    {
        exception_test = _BLOCK_CHECK(
            annotate("Invalid argument exception thrown", bool_matcher(false))
        );
    }
    catch (std::out_of_range const & ex)
    {
        exception_test = _BLOCK_CHECK(
            annotate("Integer out of range exception", bool_matcher(false))
        );
    }
    _END_BBA_CHECK(exception_test);
}

// This serves as the testing code.
_TEST_CASE(
    abc::test_case_t({.name = "Testing stoi"})
)
{
    test("1");
    test("-1");
    test("hello");
    test("9999999999999999999999999999999999999");
}

_TEST_CASE(
    abc::test_case_t({.name = "Testing midpoint"})
)
{
    using namespace std;
    function<int(int, int)> f = [](int x, int y)
    {
        return (x + y) / 2;
    };
    _BEGIN_MULTI_ELEMENT_BBA(mid_point_test, "Testing mid point function");
    for (auto&& [integer1, integer2] : initializer_list<pair<int, int>>{
             {0,             1            },
             {100,           200          },
             {500,           200          },
             {700,           1'234        },
             {2'147'483'647, 1'141'481'537}
    })
    {
        mid_point_test += _BLOCK_CHECK(
            _EXPR(f(integer1, integer2) == std::midpoint(integer1, integer2))
        );
    }
    _END_BBA_CHECK(mid_point_test);
}

inline void
    test2(
        const std::string                str,
        abc::multi_element_test_block_t& _a_metb
    )
{
    using namespace abc;
    try
    {
        int i    = std::stoi(str);
        _a_metb += _BLOCK_CHECK(annotate("Checking i > 0", _EXPR(i > 0)));
    }
    catch (std::invalid_argument const & ex)
    {
        _a_metb += _BLOCK_CHECK(
            annotate("Invalid argument exception thrown", bool_matcher(false))
        );
    }
    catch (std::out_of_range const & ex)
    {
        _a_metb += _BLOCK_CHECK(
            annotate("Integer out of range exception", bool_matcher(false))
        );
    }
}

_TEST_CASE(
    abc::test_case_t({.name = "Testing stoi again"})
)
{
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(stoi_test, "Testing std::stoi function again");
    for (auto&& str : initializer_list<string>{
             "1", "-1", "hello", "999999999999999999999999999999999999999999999"
         })
    {
        test2(str, stoi_test);
    }
    _END_BBA_CHECK(stoi_test);
}

_TEST_CASE(
    abc::test_case_t({.name = "Testing exception macros"})
)
{
    using namespace std;
    _BEGIN_NO_THROW(t1)
    int i = stoi("3");
    _END_REQUIRE_NO_THROW(t1);

    // Same check as above. This test will fail. Note that the name of the
    // internal BBA is re-used.
    _BEGIN_NO_THROW(t1)
    int i = stoi("up");
    _END_CHECK_NO_THROW(t1);

    // This test will pass.
    _BEGIN_THROW_ANY(t1)
    int i = stoi("up");
    _END_CHECK_THROW_ANY(t1);

    // This test will pass.
    _BEGIN_EXCEPTION_TYPE(t1)
    int i = stoi("hi");
    _END_CHECK_EXCEPTION_TYPE(t1, std::invalid_argument);

    // As will this. The exception returned is derived from logic error.
    _BEGIN_EXCEPTION_TYPE(t1)
    int i = stoi("hi");
    _END_CHECK_EXCEPTION_TYPE(t1, std::logic_error);

    auto f = []()
    {
        throw int(1);
    };

    // This will also pass. The thrown type does not need to be derived from
    // exception.
    _BEGIN_EXCEPTION_TYPE(t1)
    f();
    _END_CHECK_EXCEPTION_TYPE(t1, int);

    // This will pass
    _BEGIN_EXCEPTION_MSG(t1);
    int i = stoi("hi");
    _END_CHECK_EXCEPTION_MSG(t1, "invalid");

    // This will also pass
    _BEGIN_EXCEPTION_TYPE_AND_MSG(t1);
    int i = stoi("hi");
    _END_CHECK_EXCEPTION_TYPE_AND_MSG(t1, std::logic_error, "invalid");
}