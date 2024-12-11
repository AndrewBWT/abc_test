#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include <numeric>
#include <ranges>
#include <vector>

#define __BAD_USER_DEFINED_BBA_1_BEGIN(name)                               \
    _BEGIN_SINGLE_ELEMENT_BBA(name, fmt::format("{} description", #name)); \
    name = _BLOCK_CHECK(abc::bool_matcher(false));

#define __BAD_USER_DEFINED_BBA_1_END(name) _END_BBA_CHECK(name)

#define __USER_DEFINED_BBA_1_BEGIN(name)                \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(            \
        name,                                           \
        fmt::format("{} description", #name),           \
        abc::utility::str::create_string(               \
            {"__USER_DEFINED_BBA_1_BEGIN(", #name, ")"} \
        )                                               \
    )                                                   \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false));

#define __USER_DEFINED_BBA_1_END(name)                \
    _END_BBA_CHECK_CUSTOM_SOURCE(                     \
        name,                                         \
        abc::utility::str::create_string(             \
            {"__USER_DEFINED_BBA_1_END(", #name, ")"} \
        )                                             \
    )

#define __USER_DEFINED_BBA_2(name)                                             \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                                   \
        name,                                                                  \
        fmt::format("{} description", #name),                                  \
        abc::utility::str::create_string({"__USER_DEFINED_BBA_2(", #name, ")"} \
        )                                                                      \
    )                                                                          \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false));                   \
    _END_BBA_CHECK_NO_SOURCE(name)

#define __USER_DEFINED_BBA_3(name)                                         \
    _BEGIN_SINGLE_ELEMENT_BBA(name, fmt::format("{} description", #name)); \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false));               \
    _END_BBA_CHECK(name);

#define __USER_CREATED_BBA_2_BEGIN(name)                \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(            \
        name,                                           \
        fmt::format("{} description", #name),           \
        abc::utility::str::create_string(               \
            {"__USER_CREATED_BBA_2_BEGIN(", #name, ")"} \
        )                                               \
    );

#define __USER_CREATED_BBA_2_END(name)                       \
    name = _BLOCK_CHECK_NO_SOURCE(abc::bool_matcher(false)); \
    _END_BBA_CHECK_CUSTOM_SOURCE(                            \
        name,                                                \
        abc::utility::str::create_string(                    \
            {"__USER_CREATED_BBA_2_END(", #name, ")"}        \
        )                                                    \
    );

_TEST_CASE(
    abc::test_case_t(
        {.name        = "file_06_example_01",
         .description = "Examples showing assertion blocks",
         .path        = "examples::basic_examples::06_manual_block_assertion"}
    )
)
{
    using namespace abc;
    __BAD_USER_DEFINED_BBA_1_BEGIN(test1);
    __BAD_USER_DEFINED_BBA_1_END(test1);
    __USER_DEFINED_BBA_1_BEGIN(test2);
    __USER_DEFINED_BBA_1_END(test2);

    __USER_DEFINED_BBA_2(test3);

    __USER_DEFINED_BBA_3(test4);

    __USER_CREATED_BBA_2_BEGIN(test5);
    __USER_CREATED_BBA_2_END(test5);
}

namespace test
{
void
    test(
        const std::string str
    )
{
    using namespace abc;
    _BEGIN_SINGLE_ELEMENT_BBA(
        exception_test,
        fmt::format("Testing std::stoi function using \"{0}\"", str)
    );
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
void
test2(
    const std::string str,
    abc::multi_element_test_block_t& _a_metb
)
{
    using namespace abc;
    try
    {
        int i = std::stoi(str);
        _a_metb += _BLOCK_CHECK(annotate("Checking i > 0", _EXPR(i > 0)));
    }
    catch (std::invalid_argument const& ex)
    {
        _a_metb += _BLOCK_CHECK(
            annotate("Invalid argument exception thrown", bool_matcher(false))
        );
    }
    catch (std::out_of_range const& ex)
    {
        _a_metb += _BLOCK_CHECK(
            annotate("Integer out of range exception", bool_matcher(false))
        );
    }
}
} // namespace test

// This serves as the testing code.
_TEST_CASE(
    abc::test_case_t({ .name = "Empty BBAs" })
)
{
    _BEGIN_SINGLE_ELEMENT_BBA(test1, "An empty SE-BBA");
    _END_BBA_CHECK(test1);

    _BEGIN_MULTI_ELEMENT_BBA(test2, "An empty ME-BBA");
    _END_BBA_CHECK(test2);
}
_TEST_CASE(
    abc::test_case_t({.name = "Testing stoi"})
)
{
    test::test("1");
    test::test("-1");
    test::test("hello");
    test::test("9999999999999999999999999999999999999");
}
_TEST_CASE(
    abc::test_case_t({ .name = "Testing stoi again" })
)
{
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(stoi_test, "Testing mid point function");
    for (auto&& str : initializer_list<string>{
             "1","-1","hello","999999999999999999999999999999999999999999999"
        })
    {
        test::test2(str, stoi_test);
    }
    _END_BBA_CHECK(stoi_test);
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

_TEST_CASE(
    abc::test_case_t(
        { .name = "file_06_example_03",
         .description = "Included macros using block assertions",
         .path = "examples::basic_examples::06_manual_block_assertion" }
    )
)
{
    using namespace abc;
    using namespace std;
    _BEGIN_NO_THROW(t1)
        int i = stoi("3");
    _END_REQUIRE_NO_THROW(t1);

    //Same check as above. This test will fail. Note that the name of the internal BBA is re-used.
    _BEGIN_NO_THROW(t1)
        int i = stoi("up");
    _END_CHECK_NO_THROW(t1);

    //This test will pass.
    _BEGIN_THROW_ANY(t1)
        int i = stoi("up");
    _END_CHECK_THROW_ANY(t1);

    //This test will pass.
    _BEGIN_EXCEPTION_TYPE(t1)
        int i = stoi("hi");
    _END_CHECK_EXCEPTION_TYPE(t1, std::invalid_argument);

    //As will this. The exception returned is derived from logic error.
    _BEGIN_EXCEPTION_TYPE(t1)
        int i = stoi("hi");
    _END_CHECK_EXCEPTION_TYPE(t1, std::logic_error);

    auto f = []() {throw int(1); };

    //This will also pass. The thrown type does not need to be derived from exception.
    _BEGIN_EXCEPTION_TYPE(t1)
        f();
    _END_CHECK_EXCEPTION_TYPE(t1, int);

    //This will pass
    _BEGIN_EXCEPTION_MSG(t1);
    int i = stoi("hi");
    _END_CHECK_EXCEPTION_MSG(t1,"invalid");

    //This will also pass
    _BEGIN_EXCEPTION_TYPE_AND_MSG(t1);
    int i = stoi("hi");
    _END_CHECK_EXCEPTION_TYPE_AND_MSG(t1,std::logic_error, "invalid");
}

#if 0
_TEST_CASE(
    abc::test_case_t(
        {.name        = "file_06_example_01",
         .description = "Examples showing assertion blocks",
         .path        = "examples::basic_examples::06_manual_block_assertion"}
    )
)
{
    using namespace abc;
    /*!
     * A manual assertion block can be a useful tool when dealing with control
     * flow which may not be sequential; for example, when there are gotos in
     * the code, exceptions being thrown or switches which fall into different
     * cases.
     *
     * We include several different types of assertion blocks in this library.
     * In this section we will concentrate on one of them to show what it can
     * do.
     *
     * This is the MANUAL_CHECK_ASSERTION_BLOCK. Unlike the other assertions we
     * have introduced, this has a beginning and an end using the macros
     * _BEGIN_MANUAL_CHECK_ASSERTION_BLOCK and
     * _END_MANUAL_CHECK_ASSERTION_BLOCK.
     *
     * Each of these take an argument. The object which underpins these blocks
     * is given this name. Below we show its basic use
     */

    /*!
     * Internally, these macros create and close a new scope. The name element
     * (in this case, _l_mn) can be accessed and edited. When the closing macro
     * is called ( in this case, _END_MANUAL_CHECK_ASSERTION_BLOCK), any checks
     * are performed, and output written to the test reproters.
     *
     * The MANUAL_CHECK_ASSERTION_BLOCK variabee can have booleans written to
     * it. It is initially set at true. When the
     * _END_MANUAL_CHECK_ASSERTION_BLOCK is called, if the variable is true,
     * then the assertion passes. If false, it fails.
     *
     * The example above succeeds. The example below fails.
     */

    /*!
     * We can also set the message associated with the assertion block, using
     * the function "set_assertion", as shown below.
     */

    /*
     * Essentially this is all there is to assertion blocks. They are
     * essentially named blocks, which can be accessed and edited.
     *
     * We include four distinct assertion blocks with this library (8 macros, as
     * each has a begninning and end). These are as follows:
     *
     * _BEGIN_MANUAL_CHECK_ASSERTION_BLOCK/_END_MANUAL_CHECK_ASSERTION_BLOCK
     * - Assertions which you can write a bool to. They either pass or fail.
     * _BEGIN_MANUAL_REQUIRE_ASSERTION_BLOCK/_END_MANUAL_REQUIRE_ASSERTION_BLOCK
     * - Assertions which you can write a bool to. They either pass or
     * terminate. _BEGIN_CHECK_ASSERTION_BLOCK/_END_CHECK_ASSERTION_BLOCK
     * - Assertions which you can write a matcher to. They either pass or fail.
     * _BEGIN_REQUIRE_ASSERTION_BLOCK/_END_REQUIRE_ASSERTION_BLOCK
     * - Assertions which you can write a matcher to. They either pass or
     * terminate.
     *
     * Below we show some examples of the use of these.
     */


    _BEGIN_CHECK_ASSERTION_BLOCK(_l_mn, "description");
    _l_mn = _BLOCK_CHECK(matcher_t(false_matcher()));
    _END_BLOCK(_l_mn);
    /*!
     * Another example showing what happens when we use the _MATCHER macro
     */
    _BEGIN_CHECK_ASSERTION_BLOCK(_l_mn, "description");
    _l_mn = _BLOCK_CHECK(matcher_t(false_matcher()));
    _END_BLOCK(_l_mn);
    _BEGIN_CHECK_ASSERTION_BLOCK_(_l_mn2, "description2");
    for (size_t _l_idx{0}; _l_idx < 25; _l_idx++)
    {
        _l_mn2 += _BLOCK_CHECK(annotate(_EXPR(_l_idx == 24), "Checking index"));
    }
    _END_BLOCK(_l_mn2);
}
namespace testing
{
constexpr int
    test_function(
        const int _l_id
    )
{
    if (_l_id == 0)
    {
        throw std::runtime_error("A runtime errorr");
    }
    else if (_l_id == 1)
    {
        throw std::exception("The exception type");
    }
    else if (_l_id == 2)
    {
        throw int(_l_id);
    }
    else
    {
        return 0;
    }
}

inline void
    test_handling_code(
        const int _l_id
    )
{
    using namespace abc;
    using namespace std;
    _BEGIN_CHECK_ASSERTION_BLOCK(
        _l_test_exception_code,
        "Test \"test_function\" return value and exceptions"
    );
    try
    {
        int _l_rv = test_function(_l_id);
        _l_test_exception_code
            = _BLOCK_CHECK(annotate("Testing return value", _EXPR(0 == _l_rv)));
    }
    catch (const std::runtime_error& _l_re)
    {
        _l_test_exception_code = _BLOCK_CHECK(annotate(
            eq<string, string>(_l_re.what(), "The runtime error"),
            "Testing runtime_error what() message is correct"
        ));
    }
    catch (const std::exception& _l_e)
    {
        _l_test_exception_code = _BLOCK_CHECK(annotate(
            eq<string, string>(_l_e.what(), "The exceptioniano type"),
            "Testing exception's what() message is correct"
        ));
    }
    catch (...)
    {
        _l_test_exception_code = _BLOCK_CHECK(annotate(
            false_matcher(),
            "Shouldn't be able to throw any other types of element"
        ));
    }
    _END_BLOCK(_l_test_exception_code);
}
} // namespace testing

_TEST_CASE(
    abc::test_case_t(
        {.name = "file_06_example_02",
         .description
         = "Example Using assertion blocks to deal with excetpions",
         .path = "examples::basic_examples::06_manual_block_assertion"}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace testing;
    /*!
     * The examples above are very simple, and the user may be wondering why
     * these are included. In the function above we show a more complicated
     * example of what assertion blocks can do, and how they can be used to
     * define fine-grained control over what outputs are given from portions of
     * code whose control flow may not be usual.
     *
     * Below we call that function with several arguments so the user can see
     * what output is reported.
     */
    test_handling_code(0);
    test_handling_code(1);
    test_handling_code(2);
    test_handling_code(3);
}

_TEST_CASE(
    abc::test_case_t(
        {.name        = "file_06_example_03",
         .description = "Included macros using block assertions",
         .path        = "examples::basic_examples::06_manual_block_assertion"}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace testing;
    /*!
     * We include some macros with abc_test to allow the user to work with
     * exceptions.
     *
     * However, these are all defined in terms of block assertions.
     *
     * Below we show how the excpetions for checking a code block doens't throw
     * work.
     */
    _BEGIN_CHECK_NO_THROW_BLOCK(_l_name);
    throw std::exception("hello");
    _END_NO_THROW_BLOCK(_l_name);
    _BEGIN_REQUIRE_NO_THROW_BLOCK(_l_name);
    int x = 3;
    _END_NO_THROW_BLOCK(_l_name);

    _BEGIN_CHECK_THROW_ANY(_l_name);
    int x = 3;
    _END_THROW_BLOCK(_l_name);
    _BEGIN_REQUIRE_THROW_ANY(_l_name);
    throw std::exception("hello");
    _END_THROW_BLOCK(_l_name);

    _BEGIN_CHECK_EXCEPTION_TYPE(_l_name);
    int x = 3;
    _END_EXCEPTION_TYPE_BLOCK(_l_name, std::exception);
    _BEGIN_REQUIRE_EXCEPTION_TYPE(_l_name);
    throw std::exception("hello");
    _END_EXCEPTION_TYPE_BLOCK(_l_name, std::exception);

    _BEGIN_CHECK_EXCEPTION_MSG(_l_name);
    throw std::runtime_error("WHAT!");
    _END_EXCEPTION_MSG_BLOCK(_l_name, "WHAT");
    _BEGIN_REQUIRE_EXCEPTION_MSG(_l_name);
    throw std::exception("hello");
    _END_EXCEPTION_MSG_BLOCK(_l_name, "hello");

    _BEGIN_CHECK_EXCEPTION_TYPE_AND_MSG(_l_name);
    throw std::runtime_error("WHAT!");
    _END_EXCEPTION_TYPE_AND_MSG_BLOCK(_l_name, std::runtime_error, "WHAT");
    _BEGIN_REQUIRE_EXCEPTION_TYPE_AND_MSG(_l_name);
    throw std::exception("hello");
    _END_EXCEPTION_TYPE_AND_MSG_BLOCK(_l_name, std::runtime_error, "hello");
}
#endif