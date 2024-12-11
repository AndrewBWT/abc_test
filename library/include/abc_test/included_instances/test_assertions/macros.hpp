#pragma once
#include "abc_test/internal/test_assertions/macros.hpp"

#define _BEGIN_NO_THROW(_a_name)                     \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(         \
        _a_name,                                     \
        "Checking code does not throw an exception", \
        _ABC_NS_UTILITY::str::create_string(         \
            {"_BEGIN_NO_THROW(", #_a_name, ")"}      \
        )                                            \
    );                                               \
    try                                              \
    {
#define __INTERNAL_END_NO_THROW(_a_name)                                  \
    _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                       \
        "Code block does not throw any exceptions.", true_matcher()       \
    ));                                                                   \
    }                                                                     \
    catch (...)                                                           \
    {                                                                     \
        _a_name = _BLOCK_CHECK_NO_SOURCE(                                 \
            abc::annotate("Code block threw an entity.", false_matcher()) \
        );                                                                \
    }


#define _END_CHECK_NO_THROW(_a_name)                \
    __INTERNAL_END_NO_THROW(_a_name)                \
    _END_BBA_CHECK_CUSTOM_SOURCE(                   \
        _a_name,                                    \
        _ABC_NS_UTILITY::str::create_string(        \
            {"_END_CHECK_NO_THROW(", #_a_name, ")"} \
        )                                           \
    )
#define _END_REQUIRE_NO_THROW(_a_name)                \
    __INTERNAL_END_NO_THROW(_a_name)                  \
    _END_BBA_REQUIRE_CUSTOM_SOURCE(                   \
        _a_name,                                      \
        _ABC_NS_UTILITY::str::create_string(          \
            {"_END_REQUIRE_NO_THROW(", #_a_name, ")"} \
        )                                             \
    )

#define _BEGIN_THROW_ANY(_a_name)                                      \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                           \
        _a_name,                                                       \
        "Checking code does throw an exception",                       \
        _ABC_NS_UTILITY::str::create_string(                           \
            {"_BEGIN_THROW_ANY(", #_a_name, ")"}                       \
        )                                                              \
    );                                                                 \
    _a_name = _BLOCK_CHECK_NO_SOURCE(                                  \
        annotate("Code does not throw any excpetion", false_matcher()) \
    );                                                                 \
    try                                                                \
    {
#define __INTERNAL_END_THROW_ANY(_a_name)                                \
    }                                                                    \
    catch (...)                                                          \
    {                                                                    \
        _a_name = _BLOCK_CHECK_NO_SOURCE(                                \
            abc::annotate("Code block threw an entity.", true_matcher()) \
        );                                                               \
    }


#define _END_CHECK_THROW_ANY(_a_name)                \
    __INTERNAL_END_THROW_ANY(_a_name)                \
    _END_BBA_CHECK_CUSTOM_SOURCE(                    \
        _a_name,                                     \
        _ABC_NS_UTILITY::str::create_string(         \
            {"_END_CHECK_THROW_ANY(", #_a_name, ")"} \
        )                                            \
    )
#define _END_REQUIRE_THROW_ANY(_a_name)                \
    __INTERNAL_END_THROW_ANY(_a_name)                  \
    _END_BBA_REQUIRE_CUSTOM_SOURCE(                    \
        _a_name,                                       \
        _ABC_NS_UTILITY::str::create_string(           \
            {"_END_REQUIRE_THROW_ANY(", #_a_name, ")"} \
        )                                              \
    )

#define _BEGIN_EXCEPTION_TYPE(_a_name)                                 \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                           \
        _a_name,                                                       \
        "Checking code does throw an exception",                       \
        _ABC_NS_UTILITY::str::create_string(                           \
            {"_BEGIN_EXCEPTION_TYPE(", #_a_name, ")"}                  \
        )                                                              \
    );                                                                 \
    _a_name = _BLOCK_CHECK_NO_SOURCE(                                  \
        annotate("Code does not throw any excpetion", false_matcher()) \
    );                                                                 \
    try                                                                \
    {
#define __INTERNAL_END_EXCEPTION_TYPE(_a_name, _a_exception_type)   \
    }                                                               \
    catch (const _a_exception_type& _l_et)                          \
    {                                                               \
        _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(             \
            "Code block threw the required entity.", true_matcher() \
        ));                                                         \
    }                                                               \
    catch (...)                                                     \
    {                                                               \
        _a_name = _BLOCK_CHECK_NO_SOURCE(annotate(                  \
            "Code throws an exception not of the correct type",     \
            false_matcher()                                         \
        ));                                                         \
    }


#define _END_CHECK_EXCEPTION_TYPE(_a_name, _a_exception_type) \
    __INTERNAL_END_EXCEPTION_TYPE(_a_name, _a_exception_type) \
    _END_BBA_CHECK_CUSTOM_SOURCE(                             \
        _a_name,                                              \
        _ABC_NS_UTILITY::str::create_string(                  \
            {"_END_CHECK_EXCEPTION_TYPE(",                    \
             #_a_name,                                        \
             ",",                                             \
             #_a_exception_type ")"}                          \
        )                                                     \
    )
#define _END_REQUIRE_EXCEPTION_TYPE(_a_name, _a_exception_type) \
    __INTERNAL_END_EXCEPTION_TYPE(_a_name, _a_exception_type)   \
    _END_BBA_REQUIRE_CUSTOM_SOURCE(                             \
        _a_name,                                                \
        _ABC_NS_UTILITY::str::create_string(                    \
            {"_END_REQUIRE_EXCEPTION_TYPE(",                    \
             #_a_name,                                          \
             ",",                                               \
             #_a_exception_type ")"}                            \
        )                                                       \
    )


#define _BEGIN_EXCEPTION_MSG(_a_name)                                     \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                              \
        _a_name,                                                          \
        "Checking code throws an exception who's what() message shows a " \
        "specific message",                                               \
        _ABC_NS_UTILITY::str::create_string(                              \
            {"_BEGIN_EXCEPTION_MSG(", #_a_name, ")"}                      \
        )                                                                 \
    );                                                                    \
    _a_name = _BLOCK_CHECK_NO_SOURCE(                                     \
        annotate("Code does not throw any excpetion", false_matcher())    \
    );                                                                    \
    try                                                                   \
    {
#define __INTERNAL_END_EXCEPTION_MSG(_a_name, _a_msg)                   \
    }                                                                   \
    catch (const std::exception& _l_et)                                 \
    {                                                                   \
        _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                 \
            "Code block threw the required entity.",                    \
            abc::eq<std::string, std::string>(_a_msg, _l_et.what())     \
        ));                                                             \
    }                                                                   \
    catch (...)                                                         \
    {                                                                   \
        _a_name = _BLOCK_CHECK_NO_SOURCE(annotate(                      \
            "Code throws an exception not derived from std::exception", \
            false_matcher()                                             \
        ));                                                             \
    }

#define _END_CHECK_EXCEPTION_MSG(_a_name, _a_msg)                     \
    __INTERNAL_END_EXCEPTION_MSG(_a_name, _a_msg)                     \
    _END_BBA_CHECK_CUSTOM_SOURCE(                                     \
        _a_name,                                                      \
        _ABC_NS_UTILITY::str::create_string(                          \
            {"_END_CHECK_EXCEPTION_MSG(", #_a_name, ",", #_a_msg ")"} \
        )                                                             \
    )
#define _END_REQUIRE_EXCEPTION_MSG(_a_name, _a_msg)                     \
    __INTERNAL_END_EXCEPTION_MSG(_a_name, _a_msg)                       \
    _END_BBA_REQUIRE_CUSTOM_SOURCE(                                     \
        _a_name,                                                        \
        _ABC_NS_UTILITY::str::create_string(                            \
            {"_END_REQUIRE_EXCEPTION_MSG(", #_a_name, ",", #_a_msg ")"} \
        )                                                               \
    )

#define _BEGIN_EXCEPTION_TYPE_AND_MSG(_a_name)                            \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                              \
        _a_name,                                                          \
        "Checking code throws an exception who's what() message shows a " \
        "specific message",                                               \
        _ABC_NS_UTILITY::str::create_string(                              \
            {"_BEGIN_EXCEPTION_TYPE_AND_MSG(", #_a_name, ")"}             \
        )                                                                 \
    );                                                                    \
    _a_name = _BLOCK_CHECK_NO_SOURCE(                                     \
        annotate("Code does not throw any excpetion", false_matcher())    \
    );                                                                    \
    try                                                                   \
    {
#define __INTERNAL_END_EXCEPTION_TYPE_AND_MSG(_a_name, _a_type, _a_msg) \
    }                                                                   \
    catch (const _a_type& _l_et)                                        \
    {                                                                   \
        _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                 \
            "Code block threw the required entity.",                    \
            abc::eq<std::string, std::string>(_a_msg, _l_et.what())     \
        ));                                                             \
    }                                                                   \
    catch (...)                                                         \
    {                                                                   \
        _a_name = _BLOCK_CHECK_NO_SOURCE(annotate(                      \
            "Code throws an exception not derived from std::exception", \
            false_matcher()                                             \
        ));                                                             \
    }

#define _END_CHECK_EXCEPTION_TYPE_AND_MSG(_a_name, _a_type, _a_msg) \
    __INTERNAL_END_EXCEPTION_TYPE_AND_MSG(_a_name, _a_type, _a_msg) \
    _END_BBA_CHECK_CUSTOM_SOURCE(                                   \
        _a_name,                                                    \
        _ABC_NS_UTILITY::str::create_string(                        \
            {"_END_CHECK_EXCEPTION_TYPE_AND_MSG(",                  \
             #_a_name,                                              \
             ",",                                                   \
             #_a_type,                                              \
             ",",                                                   \
             #_a_msg ")"}                                           \
        )                                                           \
    )
#define _END_REQUIRE_EXCEPTION_TYPE_AND_MSG(_a_name, _a_type, _a_msg) \
    __INTERNAL_END_EXCEPTION_TYPE_AND_MSG(_a_name, _a_type, _a_msg)   \
    _END_BBA_REQUIRE_CUSTOM_SOURCE(                                   \
        _a_name,                                                      \
        _ABC_NS_UTILITY::str::create_string(                          \
            {"_END_REQUIRE_EXCEPTION_TYPE_AND_MSG(",                  \
             #_a_name,                                                \
             ",",                                                     \
             #_a_type,                                                \
             ",",                                                     \
             #_a_msg ")"}                                             \
        )                                                             \
    )