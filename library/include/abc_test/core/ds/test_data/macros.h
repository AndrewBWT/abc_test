#pragma once

#include "abc_test/core/ds/test_data/checked_user_defined_test_data.h"

/*!
 * @brief Defines the name of the prefix for function names in the macros in
 * this file.
 */
#define __ABC_TEST_FUNCTION_PREFIX            _l_establish
/*!
 * @brief Defines the name of the prefix for user-defined test data strings used
 * in the macros in this file.
 */
#define __ABC_TEST_USER_DEFINED_TEST_DATA_STR _l_user_defiend_test_data_str

/*!
 * @brief Defines the name of the prefix used for test case strings, used in the
 * macros in this file.
 */
#define __ABC_TEST_TEST_CASE_STR              _l_test_case

/*!
 * @brief Inner macro used to define a test case.
 *
 * @param _a_user_defined_test_data_name The variable name used for the
 * checked_user_defined_test_data_t object.
 *
 * Essentially, this macro works as follows:
 * - The function to be tested is forward declared.
 * - The test_data_t is then ran through the test_data function. As this is
 * contexpr, if there are any issues with the test_data_t object, it will not
 * compile.
 * - Finally, the test is then created using the object registered_test_data_t,
 * then it is registered with the system in a set of all test functions, using
 * the function add_test. Finally, the macro begins the implementation of the
 * function to be created. All the user needs to do is add curly braces.
 *
 * @param _a_function_name The variable name of the function the test is
 * testing.
 * @param _a_str_representation A string represetnation of how the outer macro
 * was called.
 */


/*!
 * @brief Macro used to define test cases.
 *
 * Essentially it does four things:
 * - Forward-declares the function.
 * - Runs the user-defined test data through a contexpr function to run
 * compile-time checks on it.
 * - Regiters the test using the add_test function.
 * - Finally begins the implemtnation of hte test function, allowing the user to
 * finish it off.
 *
 * @param _a_user_defined_test_data_name The name of the function
 * user_defined_test_data_t object created.
 * @param _a_function_name. The name of the test function to be created.
 * @param _a_user_data The user-defined test data. This should be in an object
 * of type abc::test_data_t.
 * @param _a_str_representation A string representation of the line of code
 * declaring the test.
 */
#define __ABC_TEST_INNER_TEST_CASE(                             \
    _a_user_defined_test_data_name,                             \
    _a_function_name,                                           \
    _a_user_data,                                               \
    _a_str_representation                                       \
)                                                               \
    static void _a_function_name();                             \
    namespace abc::ds                                           \
    {                                                           \
    constexpr ds::checked_user_defined_test_data_t              \
        _a_user_defined_test_data_name                          \
        = abc::ds::checked_user_defined_test_data_t::test_data( \
            user_defined_test_data_t(_a_user_data)              \
        );                                                      \
    auto __ABC_TEST_NAMED_COUNTER(__ABC_TEST_FUNCTION_PREFIX    \
    ){global::add_test(registered_test_data_t(                  \
        _a_function_name,                                       \
        _a_user_defined_test_data_name,                         \
        ds::single_source_t(                                    \
            _a_str_representation,                              \
            std::source_location::current()                     \
        )                                                       \
    ))};                                                        \
    }                                                           \
    static void _a_function_name()

/*!
 * @brief Macro used to declare a test case, with a name chosen by the user.
 *
 * @param _a_name The name of the test case.
 * @param _a_user_data The user-defined test data, which should be of type
 * abc::test_data_t.
 */
#define _NAMED_TEST_CASE(_a_name, _a_user_data)                          \
    __ABC_TEST_INNER_TEST_CASE(                                          \
        __ABC_TEST_NAMED_COUNTER(__ABC_TEST_USER_DEFINED_TEST_DATA_STR), \
        _a_name,                                                         \
        _a_user_data,                                                    \
        abc::utility::str::create_string(                                \
            {"_NAMED_TEST_CASE(", #_a_name, ",", #_a_user_data, ")"}     \
        )                                                                \
    )

/*!
 * @brief Macor used to define a test case, where the name of the test function
 * is automatically created.
 *
 * @param _a_user_data The user-defined test data, which should be of type
 * abc::test_data_t.
 */
#define _TEST_CASE(_a_user_data)                                              \
    __ABC_TEST_INNER_TEST_CASE(                                               \
        __ABC_TEST_NAMED_COUNTER(__ABC_TEST_USER_DEFINED_TEST_DATA_STR),      \
        __ABC_TEST_NAMED_COUNTER(__ABC_TEST_TEST_CASE_STR),                   \
        _a_user_data,                                                         \
        abc::utility::str::create_string({"_TEST_CASE(", #_a_user_data, ")"}) \
    )
