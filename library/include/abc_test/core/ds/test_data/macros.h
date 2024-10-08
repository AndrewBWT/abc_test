#pragma once

#include "abc_test/core/ds/test_data/checked_user_defined_test_data.h"

/*!
* Macro defining the string used for the function prefix, used in the macros in this file.
*/
#define FUNCTION_PREFIX _l_establish
/*!
* Macro defining the string used for the user defined test data string, 
* used in the macros in this file.
*/
#define USER_DEFINED_TEST_DATA_STR _l_user_defiend_test_data_str

/*!
* Macro defining the string used for the test case str.
*/
#define TEST_CASE_STR _l_test_case

/*!
* @brief Inner macro used to define a test case. 
* 
* @param _a_user_defined_test_data_name The variable name used for the
* checked_user_defined_test_data_t object.
* 
* Essentially, this macro works as follows:
* - The function to be tested is forward declared.
* - The test_data_t is then ran through the test_data function. As this is contexpr,
* if there are any issues with the test_data_t object, it will not compile.
* - Finally, the test is then created using the object registered_test_data_t, then
* it is registered with the system in a set of all test functions, using the function
* add_test.
* Finally, the macro begins the implementation of the function to be created. All
* the user needs to do is add curly braces.
* 
* @param _a_function_name The variable name of the function the test is testing.
* @param _a_str_representation A string represetnation of how the outer macro was called.
*/
#define __INNER_TEST_CASE(_a_user_defined_test_data_name,\
	_a_function_name,_a_user_data,_a_str_representation)\
	static void _a_function_name();\
	namespace abc::ds {\
	constexpr ds::checked_user_defined_test_data_t _a_user_defined_test_data_name = \
		abc::ds::checked_user_defined_test_data_t::test_data(user_defined_test_data_t(_a_user_data));\
	auto NAMED_COUNTER(FUNCTION_PREFIX){add_test(registered_test_data_t(\
		_a_function_name,\
		_a_user_defined_test_data_name,\
		reports::single_source_t(_a_str_representation,std::source_location::current())\
	))};\
	}\
	static void _a_function_name()

/*!
* @brief Function used to declare a test. 
* 
* @param _a_name. The name of the function to be tested.
* @param _a_user_data. The test_data_t object describing data about the test.
*/
#define _NAMED_TEST_CASE(_a_name, _a_user_data)\
	__INNER_TEST_CASE(NAMED_COUNTER(USER_DEFINED_TEST_DATA_STR),_a_name,_a_user_data,\
		abc::utility::str::create_string({ "_NAMED_TEST_CASE(",#_a_name,",",#_a_user_data,")" }))

/*!
* @brief Function used to declare a test.
*
* @param _a_user_data. The test_data_t object describing data about the test.
*/
#define _TEST_CASE(_a_user_data)\
	__INNER_TEST_CASE(NAMED_COUNTER(USER_DEFINED_TEST_DATA_STR), NAMED_COUNTER(TEST_CASE_STR),_a_user_data,\
		abc::utility::str::create_string({ "_TEST_CASE(",#_a_user_data,")" }))