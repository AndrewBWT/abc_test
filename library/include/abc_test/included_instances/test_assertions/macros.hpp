#pragma once
#include "abc_test/internal/test_assertions/macros.hpp"

#define _BEGIN_CHECK_NO_THROW_BLOCK(_a_name)                 \
    _BEGIN_CHECK_ASSERTION_BLOCK(                            \
        _a_name, "Checking code does not throw an exception" \
    );                                                       \
    try                                                      \
    {
#define _BEGIN_REQUIRE_NO_THROW_BLOCK(_a_name)               \
    _BEGIN_REQUIRE_ASSERTION_BLOCK(                          \
        _a_name, "Checking code does not throw an exception" \
    );                                                       \
    try                                                      \
    {
#define _END_NO_THROW_BLOCK(_a_name)                                      \
    }                                                                     \
    catch (...)                                                           \
    {                                                                     \
        _a_name = _MATCHER(                                               \
            abc::annotate("Code block threw an entity.", false_matcher()) \
        );                                                                \
    }                                                                     \
    _END_BLOCK(_a_name);

#define _BEGIN_CHECK_THROW_ANY(_a_name)                                       \
    _BEGIN_CHECK_ASSERTION_BLOCK(                                             \
        _a_name, "Checking code does throw an exception"                      \
    );                                                                        \
    _a_name = annotate("Code does not throw any excpetion", false_matcher()); \
    try                                                                       \
    {
#define _BEGIN_REQUIRE_THROW_ANY(_a_name)                                    \
    _BEGIN_REQUIRE_ASSERTION_BLOCK(                                          \
        _a_name, "Checking code does throw an exception"                     \
    );                                                                       \
    _a_name = annotate("Code does not throw an excpetion", false_matcher()); \
    try                                                                      \
    {
#define _END_THROW_BLOCK(_a_name)                                        \
    }                                                                    \
    catch (...)                                                          \
    {                                                                    \
        _a_name = _MATCHER(                                              \
            abc::annotate("Code block threw an entity.", true_matcher()) \
        );                                                               \
    }                                                                    \
    _END_BLOCK(_a_name);

#define _BEGIN_CHECK_EXCEPTION_TYPE(_a_name)                                  \
    _BEGIN_CHECK_ASSERTION_BLOCK(                                             \
        _a_name, "Checking code throws a specific exception type"             \
    );                                                                        \
    _a_name = annotate("Code does not throw any excpetion", false_matcher()); \
    try                                                                       \
    {
#define _BEGIN_REQUIRE_EXCEPTION_TYPE(_a_name)                                \
    _BEGIN_REQUIRE_ASSERTION_BLOCK(                                           \
        _a_name, "Checking code throws a specific exception type"             \
    );                                                                        \
    _a_name = annotate("Code does not throw any excpetion", false_matcher()); \
    try                                                                       \
    {
#define _END_EXCEPTION_TYPE_BLOCK(_a_name, _a_exception_type)       \
    }                                                               \
    catch (const _a_exception_type& _l_et)                          \
    {                                                               \
        _a_name = _MATCHER(abc::annotate(                           \
            "Code block threw the required entity.", true_matcher() \
        ));                                                         \
    }                                                               \
    catch (...)                                                     \
    {                                                               \
        _a_name = annotate(                                         \
            "Code throws an exception not of the correct type",     \
            false_matcher()                                         \
        );                                                          \
    }                                                               \
    _END_BLOCK(_a_name);


#define _BEGIN_CHECK_EXCEPTION_MSG(_a_name)                                   \
    _BEGIN_CHECK_ASSERTION_BLOCK(                                             \
        _a_name,                                                              \
        "Checking code throws an exception who's what() message shows a "     \
        "specific message"                                                    \
    );                                                                        \
    _a_name = annotate("Code does not throw any excpetion", false_matcher()); \
    try                                                                       \
    {
#define _BEGIN_REQUIRE_EXCEPTION_MSG(_a_name)                                 \
    _BEGIN_REQUIRE_ASSERTION_BLOCK(                                           \
        _a_name,                                                              \
        "Checking code throws an exception who's what() message shows a "     \
        "specific message"                                                    \
    );                                                                        \
    _a_name = annotate("Code does not throw any excpetion", false_matcher()); \
    try                                                                       \
    {
#define _END_EXCEPTION_MSG_BLOCK(_a_name, _a_msg)                       \
    }                                                                   \
    catch (const std::exception& _l_et)                                 \
    {                                                                   \
        _a_name = _MATCHER(abc::annotate(                               \
            "Code block threw the required entity.",                    \
            abc::eq<std::string, std::string>(_a_msg, _l_et.what())     \
        ));                                                             \
    }                                                                   \
    catch (...)                                                         \
    {                                                                   \
        _a_name = annotate(                                             \
            "Code throws an exception not derived from std::exception", \
            false_matcher()                                             \
        );                                                              \
    }                                                                   \
    _END_BLOCK(_a_name);

#define _BEGIN_CHECK_EXCEPTION_TYPE_AND_MSG(_a_name)                          \
    _BEGIN_CHECK_ASSERTION_BLOCK(                                             \
        _a_name,                                                              \
        "Checking code throws an exception who's what() message shows a "     \
        "specific message"                                                    \
    );                                                                        \
    _a_name = annotate("Code does not throw any excpetion", false_matcher()); \
    try                                                                       \
    {
#define _BEGIN_REQUIRE_EXCEPTION_TYPE_AND_MSG(_a_name)                        \
    _BEGIN_REQUIRE_ASSERTION_BLOCK(                                           \
        _a_name,                                                              \
        "Checking code throws an exception who's what() message shows a "     \
        "specific message"                                                    \
    );                                                                        \
    _a_name = annotate("Code does not throw any excpetion", false_matcher()); \
    try                                                                       \
    {
#define _END_EXCEPTION_TYPE_AND_MSG_BLOCK(_a_name, _a_type, _a_msg)     \
    }                                                                   \
    catch (const _a_type& _l_et)                                        \
    {                                                                   \
        _a_name = _MATCHER(abc::annotate(                               \
            "Code block threw the required entity.",                    \
            abc::eq<std::string, std::string>(_a_msg, _l_et.what())     \
        ));                                                             \
    }                                                                   \
    catch (...)                                                         \
    {                                                                   \
        _a_name = annotate(                                             \
            "Code throws an exception not derived from std::exception", \
            false_matcher()                                             \
        );                                                              \
    }                                                                   \
    _END_BLOCK(_a_name);