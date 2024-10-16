#pragma once
#include "abc_test/core/test_assertions/functions.h"
/*!
 * @brief Internal macro. Creates an assertion
 * @param _a_matcher The matcher to create an assertion from.
 * @param _a_assertion_type The assertion status type.
 * @param _a_str_representation_of_line The string representation of the line
 */
#define __ABC_TEST_INTERNAL_CREATE_ASSERTION(                              \
    _a_matcher, _a_assertion_type, _a_str_representation_of_line           \
)                                                                          \
    abc::create_assertion<_a_assertion_type>(                              \
        _a_matcher,                                                        \
        abc::ds::single_source_t(                                          \
            _a_str_representation_of_line, std::source_location::current() \
        ),                                                                 \
        abc::global::get_this_threads_test_runner_ref()                    \
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
#define _CHECK(_a_matcher)                                              \
    __ABC_TEST_INTERNAL_CREATE_ASSERTION(                               \
        _a_matcher,                                                     \
        abc::reports::pass_or_fail_t,                                   \
        abc::utility::str::create_string({"_CHECK(", #_a_matcher, ")"}) \
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
#define _REQUIRE(_a_matcher)                                              \
    __ABC_TEST_INTERNAL_CREATE_ASSERTION(                                 \
        _a_matcher,                                                       \
        abc::reports::pass_or_terminate_t,                                \
        abc::utility::str::create_string({"_REQUIRE(", #_a_matcher, ")"}) \
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
        abc::ds::single_source_t(                                  \
            _a_str_representation, std::source_location::current() \
        ),                                                         \
        abc::global::get_this_threads_test_runner_ref()            \
    )
/*!
 * @brief Macro used to raise a static failure assertion.
 *
 * This macro will register a failed assertion for the test it is used in. It
 * will not terminate the test function.
 */
#define _FAIL()                                                           \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(                                 \
        abc::reports::fail_t, std::optional<std::string_view>(), "FAIL()" \
    )
/*!
 * @brief Macro used to raise a static termination assertion.
 *
 * This macro will register a failed assertion for the test it is used in, and
 * terminate the test.
 */
#define _TERMINATE()                       \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(  \
        abc::reports::terminate_t,         \
        std::optional<std::string_view>(), \
        "_TERMINATE()"                     \
    )
/*!
 * @brief Macro used to raise a succeeded assertion.
 *
 * This macro will register a passed assertion with the test_runner_t.
 */
#define _SUCCEED()                                                            \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(                                     \
        abc::reports::pass_t, std::optional<std::string_view>(), "_SUCCEED()" \
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
#define _FAIL_WITH_MSG(_a_msg)                                              \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(                                   \
        abc::reports::fail_t,                                               \
        std::optional<std::string_view>(_a_msg),                            \
        abc::utility::str::create_string({"_FAIL_WITH_MSG(", #_a_msg, ")"}) \
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
        abc::reports::terminate_t,                 \
        std::optional<std::string_view>(_a_msg),   \
        abc::utility::str::create_string(          \
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
#define _SUCCEED_WITH_MSG(_a_msg)                                              \
    __ABC_TEST_INTERNAL_STATIC_ASSERTION(                                      \
        abc::reports::pass_t,                                                  \
        std::optional<std::string_view>(_a_msg),                               \
        abc::utility::str::create_string({"_SUCCEED_WITH_MSG(", #_a_msg, ")"}) \
    )

/*!
 * @brief Macro used to register source code with a matcher. By using it in the
 * form matcher_t a = _MATCHER(<matcher code in here>) a matcher's source code
 * can be registered on a different line to that it is used on.
 *
 * @param _a_matcher The matcher_t (or annotated_matcher_t) to annotate.
 * @return matcher_t (or annotated_matcher_t) with a source.
 */
#define _MATCHER(_a_matcher)                                                   \
    abc::matcher_macro(                                                        \
        _a_matcher,                                                            \
        abc::ds::single_source_t(                                              \
            abc::utility::str::create_string({"_MATCHER(", #_a_matcher, ")"}), \
            std::source_location::current()                                    \
        )                                                                      \
    )

/*!
 * @brief Macro used to help to use an or statement with a matcher.
 *
 * @param _a_matcher The matcher_t to use.
 */
#define _IF_OR_STATEMENT(_a_matcher)                           \
    if (_a_matcher.or_statement(abc::ds::single_source_t( \
            abc::utility::str::create_string(                  \
                {"_IF_OR_STATEMENT(", #_a_matcher, ")"}        \
            ),                                                 \
            std::source_location::current()                    \
        )))
/*!
 * @brief Macro used to help to use an and statement with a matcher.
 *
 * @param _a_matcher The matcher_t to use.
 */
#define _IF_AND_STATEMENT(_a_matcher)                           \
    if (_a_matcher.and_statement(abc::ds::single_source_t( \
            abc::utility::str::create_string(                   \
                {"_IF_OR_STATEMENT(", #_a_matcher, ")"}         \
            ),                                                  \
            std::source_location::current()                     \
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
        abc::ds::single_source_t(                        \
            abc::utility::str::create_string(                 \
                {"_PROCESS(",                                 \
                 #_a_matcher,                                 \
                 ",",                                         \
                 #_a_matcher_to_add_as_expression,            \
                 ")"}                                         \
            ),                                                \
            std::source_location::current()                   \
        )                                                     \
    );
/*!
 * @brief Inner macro which creates test blocks.
 *
 * @param _a_name The name of the block.
 * @param _a_description The description of the test block.
 * @param _a_assertion_type The type of the assertion.
 * @param _a_str_representation The string representation of the macro.
 */
#define __ABC_TEST_INNER_BEGIN_BLOCK(                                  \
    _a_name, _a_description, _a_assertion_type, _a_str_representation  \
)                                                                      \
    {                                                                  \
        abc::test_block_t<_a_assertion_type> _a_name(                  \
            _a_description,                                            \
            abc::ds::single_source_t(                                  \
                _a_str_representation, std::source_location::current() \
            )                                                          \
        );

/*!
 * @brief Macro to end a testing block.
 * @param _a_name The name of the macro being ended.
 */
#define _END_BLOCK(_a_name)                                               \
    _a_name.register_end(abc::ds::single_source_t(                   \
        abc::utility::str::create_string({"_END_BLOCK(", #_a_name, ")"}), \
        std::source_location::current()                                   \
    ));                                                                   \
    _a_name.set_processed();                                              \
    abc::create_assertion_block(                                          \
        _a_name, abc::global::get_this_threads_test_runner_ref()          \
    );                                                                    \
    }

/*!
 * @brief Macro to begin a testing block, which can either pass or fail (but not
 * terminate).
 *
 * @param _a_name The name of the testing block.
 * @param _a_description The annotation of the block assertion.
 */
#define _BEGIN_CHECK_ASSERTION_BLOCK(_a_name, _a_description) \
    __ABC_TEST_INNER_BEGIN_BLOCK(                             \
        _a_name,                                              \
        _a_description,                                       \
        abc::reports::pass_or_fail_t,                         \
        abc::utility::str::create_string(                     \
            {"_BEGIN_CHECK_ASSERTION_BLOCK(", #_a_name, ")"}  \
        )                                                     \
    );

/*!
 * @brief Macro to begin a testing block, which can either pass or terminate.
 *
 * @param _a_name The name of the testing block.
 * @param _a_description The annotation of the block assertion.
 */
#define _BEGIN_REQUIRE_ASSERTION_BLOCK(_a_name, _a_description) \
    __ABC_TEST_INNER_BEGIN_BLOCK(                               \
        _a_name,                                                \
        _a_description,                                         \
        abc::reports::pass_or_terminate_t,                      \
        abc::utility::str::create_string(                       \
            {"_BEGIN_CHECK_ASSERTION_BLOCK(", #_a_name, ")"}    \
        )                                                       \
    );
