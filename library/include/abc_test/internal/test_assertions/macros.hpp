#pragma once
#include "abc_test/internal/test_assertions/functions.hpp"
/*!
 * @brief Internal macro. Creates an assertion
 * @param _a_matcher The matcher to create an assertion from.
 * @param _a_assertion_type The assertion status type.
 * @param _a_str_representation_of_line The string representation of the line
 */
#define __ABC_TEST_INTERNAL_CREATE_ASSERTION(                   \
    _a_matcher, _a_assertion_type, _a_macro_str, _a_matcher_str \
)                                                               \
    abc::create_assertion<_a_assertion_type>(                   \
        _a_matcher,                                             \
        _a_macro_str,                                           \
        _a_matcher_str,                                         \
        std::source_location::current(),                        \
        _ABC_NS_GLOBAL::get_this_threads_test_runner_ref()      \
    )
/*!
 * @brief The check macro.
 *
 * This macro checks some matcher to see whether it is true or false. This
 * assertion will never terminate the test; failure will simply register as a
 * failed assertion.
 *
 * @param _a_matcher The matcher to check. Note that this can also be a
 * matcher_with_annotation, if the user wants to annotate the assertion.
 * @return True if the matcher passes. False if it fails.
 */
#define _CHECK(_a_matcher)                                                 \
    __ABC_TEST_INTERNAL_CREATE_ASSERTION(                                  \
        _a_matcher, _ABC_NS_REPORTS::pass_or_fail_t, "_CHECK", #_a_matcher \
    )
/*!
 * @brief The REQUIRE macro.
 *
 * This macro checks some matcher to see whether it is true or false. This
 * assertion will terminate the test function if it fails.
 *
 * @param _a_matcher The matcher to check. Note that this can also be a
 * matcher_with_annotation, if the user wants to annotate the assertion.
 * @return True if the matcher passes. False if it fails (though it will also
 * throw an exception as well).
 */
#define _REQUIRE(_a_matcher)                  \
    __ABC_TEST_INTERNAL_CREATE_ASSERTION(     \
        _a_matcher,                           \
        _ABC_NS_REPORTS::pass_or_terminate_t, \
        "_REQUIRE",                           \
        #_a_matcher                           \
    )
/*!
 * @brief Internal macro. Used when creating static assertions.
 * @param _a_assertion_type The status type of the assertion.
 * @param _a_msg The message of the static assertion.
 * @param _a_str_representation The string representation of the line.
 */
#define __ABC_TEST_INTERNAL_STATIC_ASSERTION(                      \
    _a_assertion_type, _a_msg, _a_str_representation               \
)                                                                  \
    create_static_assertion<_a_assertion_type>(                    \
        _a_msg,                                                    \
        _ABC_NS_DS::single_source_t(                               \
            _a_str_representation, std::source_location::current() \
        ),                                                         \
        _ABC_NS_GLOBAL::get_this_threads_test_runner_ref()         \
    )
/*!
 * @brief Macro used to raise a static failure assertion.
 *
 * This macro will register a failed assertion for the test it is used in. It
 * will not terminate the test function.
 */
#define _FAIL()                                                         \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(                               \
        _ABC_NS_REPORTS::fail_t, std::optional<std::string>(), "FAIL()" \
    )
/*!
 * @brief Macro used to raise a static termination assertion.
 *
 * This macro will register a failed assertion for the test it is used in, and
 * terminate the test.
 */
#define _TERMINATE()                      \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION( \
        _ABC_NS_REPORTS::terminate_t,     \
        std::optional<std::string>(),     \
        "_TERMINATE()"                    \
    )
/*!
 * @brief Macro used to raise a succeeded assertion.
 *
 * This macro will register a passed assertion with the test_runner_t.
 */
#define _SUCCEED()                                                          \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(                                   \
        _ABC_NS_REPORTS::pass_t, std::optional<std::string>(), "_SUCCEED()" \
    )
/*!
 * @brief Macro used to raise a static failure assertion with a message used as
 * the assertion's annotation.
 *
 * @param _a_msg The message used as an annotation.
 *
 * This macro will register a failed assertion for the test it is used in. It
 * will not terminate the test function.
 */
#define _FAIL_WITH_MSG(_a_msg)                                                 \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(                                      \
        _ABC_NS_REPORTS::fail_t,                                               \
        std::optional<std::string>(_a_msg),                                    \
        _ABC_NS_UTILITY::str::create_string({"_FAIL_WITH_MSG(", #_a_msg, ")"}) \
    )
/*!
 * @brief Macro used to raise a static termination assertion with the message
 * used as the macro's annotation.
 *
 * @param _a_msg The message used as an annotation.
 *
 * This macro will register a failed assertion for the test it is used in, and
 * terminate the test.
 */
#define _TERMINATE_WITH_MSG(_a_msg)                \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(          \
        _ABC_NS_REPORTS::terminate_t,              \
        std::optional<std::string>(_a_msg),        \
        _ABC_NS_UTILITY::str::create_string(       \
            {"_TERMINATE_WITH_MSG(", #_a_msg, ")"} \
        )                                          \
    )
/*!
 * @brief Macro used to raise a succeeded assertion with the string argument
 * used as an assertion annotation.
 *
 * @param _a_msg The message used as an annotation.
 *
 * This macro will register a passed assertion with the test_runner_t.
 */
#define _SUCCEED_WITH_MSG(_a_msg)                \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(        \
        _ABC_NS_REPORTS::pass_t,                 \
        std::optional<std::string>(_a_msg),      \
        _ABC_NS_UTILITY::str::create_string(     \
            {"_SUCCEED_WITH_MSG(", #_a_msg, ")"} \
        )                                        \
    )

/*!
 * @brief Macro used to register source code with a matcher. By using it in the
 * form matcher_t a = _MATCHER(<matcher code in here>) a matcher's source code
 * can be registered on a different line to that it is used on.
 *
 * @param _a_matcher The matcher_t (or annotated_matcher_t) to annotate.
 * @return matcher_t (or annotated_matcher_t) with a source.
 */
#define _MATCHER(_a_matcher)                                                 \
    abc::matcher_macro(                                                      \
        _a_matcher, "_MATCHER", #_a_matcher, std::source_location::current() \
    )

/*!
 * @brief Macro used to help to use an or statement with a matcher.
 *
 * @param _a_matcher The matcher_t to use.
 */
#define _IF_OR_OPERATOR(_a_matcher, _a_logic_matcher_name) \
    if (_ABC_NS_MATCHER::simulated_logic_expr_t<           \
            _ABC_NS_MATCHER::logic_enum_t::OR>             \
            _a_logic_matcher_name(                         \
                _a_matcher,                                \
                "_IF_OR_OPERATOR",                         \
                #_a_matcher,                               \
                std::source_location::current()            \
            );                                             \
        not _a_logic_matcher_name.passed())


/*!
 * @brief Macro used to help to use an and statement with a matcher.
 *
 * @param _a_matcher The matcher_t to use.
 */
#define _IF_AND_OPERATOR(_a_matcher)                          \
    if (_a_matcher.and_statement(_ABC_NS_DS::single_source_t( \
            _ABC_NS_UTILITY::str::create_string(              \
                {"_IF_AND_OPERATOR(", #_a_matcher, ")"}       \
            ),                                                \
            std::source_location::current()                   \
        )))

/*!
 * @brief Macro used to process a matcher_t with a pre-prepared matcher_t, for
 * use with a binary expression.
 *
 * @param _a_matcher The matcher_t to use
 * @param _a_matcher_to_add_as_expression The matcher_t to add as the
 * right-child of _a_matcher.
 */
#define _PROCESS(_a_matcher, _a_matcher_to_add_as_expression) \
    _a_matcher.process(                                       \
        _a_matcher_to_add_as_expression,                      \
        _ABC_NS_DS::single_source_t(                          \
            _ABC_NS_UTILITY::str::create_string(              \
                {"_PROCESS(",                                 \
                 #_a_matcher,                                 \
                 ",",                                         \
                 #_a_matcher_to_add_as_expression,            \
                 ")"}                                         \
            ),                                                \
            std::source_location::current()                   \
        )                                                     \
    )

#define __GENERIC_MATCHER_BBA_TRANSFORM(                                   \
    _a_matcher, _a_assertion_status, _a_str_representation                 \
)                                                                          \
    abc::make_entity_bba_compatable<_a_assertion_status>(                  \
        _a_matcher, _a_str_representation, std::source_location::current() \
    )

#define __GENERIC_STATIC_BBA_TRANSFORM(                           \
    _a_pass, _a_opt_msg, _a_assertion_type, _a_str_representation \
)                                                                 \
    abc::make_entity_bba_compatable<_a_assertion_type>(           \
        _a_pass,                                                  \
        _a_opt_msg,                                               \
        _a_str_representation,                                    \
        std::source_location::current()                           \
    )


#define _BLOCK_REQUIRE(_a_matcher)                \
    __GENERIC_MATCHER_BBA_TRANSFORM(              \
        _a_matcher,                               \
        _ABC_NS_REPORTS::pass_or_terminate_t,     \
        _ABC_NS_UTILITY::str::create_string(      \
            {"_BLOCK_REQUIRE(", #_a_matcher, ")"} \
        )                                         \
    )

#define _BLOCK_SUCCESS()                                          \
    __GENERIC_STATIC_BBA_TRANSFORM(                               \
        true,                                                     \
        optional<std::string>{},                                  \
        _ABC_NS_REPORTS::pass_t,                                  \
        _ABC_NS_UTILITY::str::create_string({"_BLOCK_SUCCESS()"}) \
    )

#define _BLOCK_FAIL()                                          \
    __GENERIC_STATIC_BBA_TRANSFORM(                            \
        false,                                                 \
        optional<std::string>{},                               \
        _ABC_NS_REPORTS::fail_t,                               \
        _ABC_NS_UTILITY::str::create_string({"_BLOCK_FAIL()"}) \
    )

#define _BLOCK_TERMINATE()                                          \
    __GENERIC_STATIC_BBA_TRANSFORM(                                 \
        false,                                                      \
        optional<std::string>{},                                    \
        _ABC_NS_REPORTS::terminate_t,                               \
        _ABC_NS_UTILITY::str::create_string({"_BLOCK_TERMINATE()"}) \
    )

#define _BLOCK_SUCCESS_WITH_MSG(_a_msg)                \
    __GENERIC_STATIC_BBA_TRANSFORM(                    \
        true,                                          \
        optional<std::string>{_a_msg},                 \
        _ABC_NS_REPORTS::pass_t,                       \
        _ABC_NS_UTILITY::str::create_string(           \
            {"_BLOCK_SUCCESS_WITH_MSG(", #_a_msg, ")"} \
        )                                              \
    )

#define _BLOCK_FAIL_WITH_MSG(_a_msg)                \
    __GENERIC_STATIC_BBA_TRANSFORM(                 \
        false,                                      \
        optional<std::string>{_a_msg},              \
        _ABC_NS_REPORTS::fail_t,                    \
        _ABC_NS_UTILITY::str::create_string(        \
            {"_BLOCK_FAIL_WITH_MSG(", #_a_msg, ")"} \
        )                                           \
    )

#define _BLOCK_TERMINATE_WITH_MSG(_a_msg)                \
    __GENERIC_STATIC_BBA_TRANSFORM(                      \
        false,                                           \
        optional<std::string>{_a_msg},                   \
        _ABC_NS_REPORTS::terminate_t,                    \
        _ABC_NS_UTILITY::str::create_string(             \
            {"_BLOCK_TERMINATE_WITH_MSG(", #_a_msg, ")"} \
        )                                                \
    )

#define _BLOCK_CHECK(_a_matcher)                \
    __GENERIC_MATCHER_BBA_TRANSFORM(            \
        _a_matcher,                             \
        _ABC_NS_REPORTS::pass_or_fail_t,        \
        _ABC_NS_UTILITY::str::create_string(    \
            {"_BLOCK_CHECK(", #_a_matcher, ")"} \
        )                                       \
    )

#define __GENERIC_MATCHER_BBA_TRANSFORM_NO_SOURCE( \
    _a_matcher, _a_assertion_status                \
)                                                  \
    abc::make_entity_bba_compatable<_a_assertion_status>(_a_matcher)

#define _BLOCK_CHECK_NO_SOURCE(_a_matcher)          \
    __GENERIC_MATCHER_BBA_TRANSFORM_NO_SOURCE(      \
        _a_matcher, _ABC_NS_REPORTS::pass_or_fail_t \
    )

#define _BLOCK_REQUIRE_NO_SOURCE(_a_matcher)             \
    __GENERIC_MATCHER_BBA_TRANSFORM_NO_SOURCE(           \
        _a_matcher, _ABC_NS_REPORTS::pass_or_terminate_t \
    )


#define __INTERNAL_BBA_NO_SOURCE(_a_name, _a_assertion_type)        \
    abc::create_assertion_block<_a_assertion_type>(                 \
        _a_name, _ABC_NS_GLOBAL::get_this_threads_test_runner_ref() \
    );                                                              \
    }
/*!
 * @brief Macro to end a testing block.
 * @param _a_name The name of the macro being ended.
 */
#define _END_BBA_CHECK_NO_SOURCE(_a_name) \
    __INTERNAL_BBA_NO_SOURCE(_a_name, abc::reports::pass_or_fail_t)

#define _END_BBA_REQUIRE_NO_SOURCE(_a_name) \
    __INTERNAL_BBA_NO_SOURCE(_a_name, abc::reports::pass_or_terminate_t)

#define __INTERNAL_BBA_CUSTOM_SOURCE(                       \
    _a_name, _a_assertion_type, _a_source_representation    \
)                                                           \
    abc::create_assertion_block<_a_assertion_type>(         \
        _a_name,                                            \
        _ABC_NS_GLOBAL::get_this_threads_test_runner_ref(), \
        _a_source_representation,                           \
        std::source_location::current()                     \
    );                                                      \
    }


#define _END_BBA_CHECK_CUSTOM_SOURCE(_a_name, _a_source_representation) \
    __INTERNAL_BBA_CUSTOM_SOURCE(                                       \
        _a_name, abc::reports::pass_or_fail_t, _a_source_representation \
    )

#define _END_BBA_REQUIRE_CUSTOM_SOURCE(_a_name, _a_source_representation)    \
    __INTERNAL_BBA_CUSTOM_SOURCE(                                            \
        _a_name, abc::reports::pass_or_terminate_t, _a_source_representation \
    )

#define _END_BBA_CHECK(_a_name)                                                \
    _END_BBA_CHECK_CUSTOM_SOURCE(                                              \
        _a_name,                                                               \
        _ABC_NS_UTILITY::str::create_string({"_END_BBA_CHECK(", #_a_name, ")"} \
        )                                                                      \
    )

#define _END_BBA_REQUIRE(_a_name)                                              \
    _END_BBA_REQUIRE_CUSTOM_SOURCE(                                            \
        _a_name,                                                               \
        _ABC_NS_UTILITY::str::create_string({"_END_BBA_CHECK(", #_a_name, ")"} \
        )                                                                      \
    )

/*!
 * @brief Inner macro which creates test blocks.
 *
 * @param _a_name The name of the block.
 * @param _a_description The description of the test block.
 * @param _a_assertion_type The type of the assertion.
 * @param _a_str_representation The string representation of the macro.
 */
#define __ABC_TEST_INNER_BEGIN_BLOCK(                                  \
    _a_class, _a_name, _a_description, _a_str_representation           \
)                                                                      \
    {                                                                  \
        _a_class _a_name(                                              \
            _a_description,                                            \
            _ABC_NS_DS::single_source_t(                               \
                _a_str_representation, std::source_location::current() \
            )                                                          \
        );

#define _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(                             \
    _a_name, _a_description, _a_source                                       \
)                                                                            \
    __ABC_TEST_INNER_BEGIN_BLOCK(                                            \
        abc::single_element_test_block_t, _a_name, _a_description, _a_source \
    )

/*!
 * @brief Macro to begin a testing block, which can either pass or fail (but not
 * terminate).
 *
 * @param _a_name The name of the testing block.
 * @param _a_description The annotation of the block assertion.
 */
#define _BEGIN_SINGLE_ELEMENT_BBA(_a_name, _a_description) \
    _BEGIN_SINGLE_ELEMENT_BBA_CUSTOM_SOURCE(               \
        _a_name,                                           \
        _a_description,                                    \
        _ABC_NS_UTILITY::str::create_string(               \
            {"_BEGIN_SINGLE_ELEMENT_BBA(", #_a_name, ")"}  \
        )                                                  \
    )
#define _BEGIN_MULTI_ELEMENT_BBA_CUSTOM_SOURCE(                             \
    _a_name, _a_description, _a_source                                      \
)                                                                           \
    __ABC_TEST_INNER_BEGIN_BLOCK(                                           \
        abc::multi_element_test_block_t, _a_name, _a_description, _a_source \
    )

#define _BEGIN_MULTI_ELEMENT_BBA(_a_name, _a_description) \
    _BEGIN_MULTI_ELEMENT_BBA_CUSTOM_SOURCE(               \
        _a_name,                                          \
        _a_description,                                   \
        _ABC_NS_UTILITY::str::create_string(              \
            {"_BEGIN_MULTI_ELEMENT_BBA(", #_a_name, ")"}  \
        )                                                 \
    )

/*!
 * @brief Macro to begin a testing block, which can either pass or fail (but not
 * terminate).
 *
 * @param _a_name The name of the testing block.
 * @param _a_description The annotation of the block assertion.
 */