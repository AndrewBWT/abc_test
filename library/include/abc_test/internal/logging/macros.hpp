#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/logging/log_msg.hpp"

/*!
 * @brief Internal macro, used to assign a random name to the annonymous
 * log_msg_t.
 */
#define __ABC_INTERNAL_LOG_VAR_NAME _l_log_var
/*!
 * @brief Internal macro. This macro does the majority fo the work in processing
 * log messages.
 *
 * @param _a_string_to_log The string which is beign logged.
 * @param _a_delete_after_use Denotes whether to remove the log message after it
 * has been used once, or whether to only remove it when it goes out of close.
 * @param _a_str_represntation A string represntation of the line used to call
 * this macro.
 */
#define __ABC_INTERNAL_LOG(                                            \
    _a_string_to_log, _a_delete_after_use, _a_str_representation       \
)                                                                      \
    abc::logging::log_msg_t                                            \
        __ABC_TEST_NAMED_COUNTER(__ABC_INTERNAL_LOG_VAR_NAME)(         \
            abc::ds::single_source_t(                                  \
                _a_str_representation, std::source_location::current() \
            ),                                                         \
            _a_string_to_log,                                          \
            _a_delete_after_use                                        \
        )
/*!
 * @brief Macro used to log a message. This macro will log a message which will
 * persist; any assetiosn in scope which use the thread's log cache will print
 * out messages associated with this log. This continues until the current scope
 * is exited.
 *
 * @param _a_str_to_log The string to log.
 */
#define _TLOG(_a_str_to_log)                                              \
    __ABC_INTERNAL_LOG(                                                   \
        _a_str_to_log,                                                    \
        false,                                                            \
        abc::utility::str::create_string({"_TLOG(", #_a_str_to_log, ")"}) \
    )
/*!
 * @brief Macro used to log a message. This macro will log a message which will
 * be deleted after the first time it is associated with a loggable object.
 *
 * @param _a_str_to_log The string to log.
 */
#define _TLOG_(_a_str_to_log)                                              \
    __ABC_INTERNAL_LOG(                                                    \
        _a_str_to_log,                                                     \
        true,                                                              \
        abc::utility::str::create_string({"_TLOG_(", #_a_str_to_log, ")"}) \
    )
/*!
 * @brief Internal macro which processed a VLOG macro. Specifically it calls
 * fmt_format to format the string argument.
 *
 * @param _a_variable The variable to print. This variable, and a stringified
 * variant of its name, is sent to fmt_foramt.
 * @param _a_delete_after_use Whether to delete this variable after its first
 * used.
 */
#define __ABC_INTERNAL_VLOG(                                 \
    _a_variable, _a_delete_after_use, _a_str_representation  \
)                                                            \
    __ABC_INTERNAL_LOG(                                      \
        fmt::format("{0} = {1}", #_a_variable, _a_variable), \
        _a_delete_after_use,                                 \
        _a_str_representation                                \
    )
/*!
 * @brief Macro used to log a single variable to an assertion.
 * @param _a_var The variable in question.
 */
#define _TVLOG(_a_var)                                              \
    __ABC_INTERNAL_VLOG(                                            \
        _a_var,                                                     \
        false,                                                      \
        abc::utility::str::create_string({"_TVLOG(", #_a_var, ")"}) \
    )
/*!
 * @brief Macro used to log a single variable to an assertion. The log message
 * is deleted after its first used.
 * @param _a_var The variable in question.
 */
#define _TVLOG_(_a_var)                                              \
    __ABC_INTERNAL_VLOG(                                             \
        _a_var,                                                      \
        true,                                                        \
        abc::utility::str::create_string({"_TVLOG_(", #_a_var, ")"}) \
    )