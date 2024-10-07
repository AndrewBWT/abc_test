#pragma once

#include "abc_test/core/ds/test_data/checked_user_defined_test_data.h"

/*! Inner macro which declares a test, registers a test, and then provides that test's
* definition.
@param Description The test description
@param Path An identifier for the test, used for organisation.
@param Function_Name The name of the function. When used with _TEST_CASE, this will have been auto-generated.
*/
#define FUNCTION_PREFIX _l_establish
#define USER_DEFINED_TEST_DATA_STR _l_user_defiend_test_data_str

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

#define _NAMED_TEST_CASE(_a_name, _a_user_data)\
	__INNER_TEST_CASE(NAMED_COUNTER(USER_DEFINED_TEST_DATA_STR),_a_name,_a_user_data,\
		abc::utility::str::create_string({ "_NAMED_TEST_CASE(",#_a_name,",",#_a_user_data,")" }))

#define _TEST_CASE(_a_user_data)\
	__INNER_TEST_CASE(NAMED_COUNTER(USER_DEFINED_TEST_DATA_STR), NAMED_COUNTER(TEST_CASE_STR),_a_user_data,\
		abc::utility::str::create_string({ "_TEST_CASE(",#_a_user_data,")" }))

#define TEST_CASE_STR _l_test_case