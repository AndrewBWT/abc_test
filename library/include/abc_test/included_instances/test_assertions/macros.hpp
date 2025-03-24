#pragma once
#include "abc_test/core/test_assertions/macros.hpp"

#define _BEGIN_NO_THROW_MATCHER(_a_matcher_name)                  \
    abc::matcher_t _a_matcher_name{abc::expected_no_exception()}; \
    _a_matcher_name.add_source_info(                              \
        "_BEGIN_NO_THROW_MATCHER",                                \
        #_a_matcher_name,                                         \
        std::source_location::current()                           \
    );                                                            \
    try                                                           \
    {
#define _END_NO_THROW_MATCHER(_a_matcher_name)            \
    _a_matcher_name.add_source_info(                      \
        "_END_NO_THROW_MATCHER",                          \
        #_a_matcher_name,                                 \
        std::source_location::current()                   \
    );                                                    \
    }                                                     \
    catch (const std::exception& _a_exception)            \
    {                                                     \
        _a_matcher_name.override_matcher(                 \
            unexpected_exception_thrown(_a_exception)     \
        );                                                \
        _a_matcher_name.add_source_info(                  \
            "_END_NO_THROW_MATCHER",                      \
            #_a_matcher_name,                             \
            std::source_location::current()               \
        );                                                \
    }                                                     \
    catch (...)                                           \
    {                                                     \
        _a_matcher_name.override_matcher(                 \
            unexpected_exception_of_unknown_type_thrown() \
        );                                                \
        _a_matcher_name.add_source_info(                  \
            "_END_NO_THROW_MATCHER",                      \
            #_a_matcher_name,                             \
            std::source_location::current()               \
        );                                                \
    }

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
#define __INTERNAL_END_NO_THROW(_a_name)                                       \
    _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                            \
        "Code block does not throw any exceptions.", abc::true_matcher()       \
    ));                                                                        \
    }                                                                          \
    catch (...)                                                                \
    {                                                                          \
        _a_name = _BLOCK_CHECK_NO_SOURCE(                                      \
            abc::annotate("Code block threw an entity.", abc::false_matcher()) \
        );                                                                     \
    }

#define _NO_THROW_BBA_BEGIN() \
    try                       \
    {
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

#define _BEGIN_THROW_ANY(_a_name)                                 \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                      \
        _a_name,                                                  \
        "Checking code does throw an exception",                  \
        _ABC_NS_UTILITY::str::create_string(                      \
            {"_BEGIN_THROW_ANY(", #_a_name, ")"}                  \
        )                                                         \
    );                                                            \
    _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(               \
        "Code does not throw any excpetion", abc::false_matcher() \
    ));                                                           \
    try                                                           \
    {
#define __INTERNAL_END_THROW_ANY(_a_name)                                     \
    }                                                                         \
    catch (...)                                                               \
    {                                                                         \
        _a_name = _BLOCK_CHECK_NO_SOURCE(                                     \
            abc::annotate("Code block threw an entity.", abc::true_matcher()) \
        );                                                                    \
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

#define _BEGIN_EXCEPTION_TYPE(_a_name)                            \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                      \
        _a_name,                                                  \
        "Checking code does throw an exception",                  \
        _ABC_NS_UTILITY::str::create_string(                      \
            {"_BEGIN_EXCEPTION_TYPE(", #_a_name, ")"}             \
        )                                                         \
    );                                                            \
    _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(               \
        "Code does not throw any excpetion", abc::false_matcher() \
    ));                                                           \
    try                                                           \
    {
#define __INTERNAL_END_EXCEPTION_TYPE(_a_name, _a_exception_type)        \
    }                                                                    \
    catch (const _a_exception_type& _l_et)                               \
    {                                                                    \
        _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                  \
            "Code block threw the required entity.", abc::true_matcher() \
        ));                                                              \
    }                                                                    \
    catch (...)                                                          \
    {                                                                    \
        _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                  \
            "Code throws an exception not of the correct type",          \
            abc::false_matcher()                                         \
        ));                                                              \
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
    _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                       \
        "Code does not throw any excpetion", abc::false_matcher()         \
    ));                                                                   \
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
        _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                 \
            "Code throws an exception not derived from std::exception", \
            abc::false_matcher()                                        \
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
    _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                       \
        "Code does not throw any excpetion", abc::false_matcher()         \
    ));                                                                   \
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
        _a_name = _BLOCK_CHECK_NO_SOURCE(abc::annotate(                 \
            "Code throws an exception not derived from std::exception", \
            abc::false_matcher()                                        \
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