#pragma once

#include "abc_test/core/ds/test_data/registered_test_data.h"

/*! Inner macro which declares a test, registers a test, and then provides that test's
* definition.
@param Description The test description
@param Path An identifier for the test, used for organisation.
@param Function_Name The name of the function. When used with _TEST_CASE, this will have been auto-generated.
*/
#define FUNCTION_PREFIX _a_establish

#define _TEST_CASE_1(_a_description,_a_path,_a_function_name,_a_threads_required,\
	 _a_str_representation)\
	static void _a_function_name();\
	abc::ds::registered_test_data_t NAMED_COUNTER(_a_establish)(\
		_a_function_name,\
		abc::ds::user_defined_test_data_t(_a_description, _a_description, _a_path,_a_threads_required),\
		abc::reports::single_source_t(_a_str_representation,std::source_location::current())\
	);\
	static void _a_function_name()

#define TEST_CASE_STR _l_test_case


#define _TEST_CASE(_a_description, _a_path) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		NAMED_COUNTER(TEST_CASE_STR),\
		1,\
		abc::utility::str::create_string({"_TEST_CASE(",#_a_description,",",#_a_path,")"})\
	)


#define _TEST_CASE_THREADED(_a_description, _a_path, _a_threads) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		NAMED_COUNTER(TEST_CASE_STR),\
		_a_threads,\
		abc::utility::str::create_string({"_TEST_CASE_THREADED(",#_a_description,",",#_a_path,",",#_a_threads,")"})\
	)

#define _NAMED_TEST_CASE(_a_name, _a_description, _a_path) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		_a_name,\
		1,\
		abc::utility::str::create_string({"_NAMED_TEST_CASE(",#_a_name,",", #_a_description,",",#_a_path,")"})\
	)

#define _NAMED_TEST_CASE_THREADED(_a_name, _a_description, _a_path, _a_threads) \
	_TEST_CASE_1(\
		_a_description,\
		_a_path,\
		_a_name,\
		_a_threads,\
		abc::utility::str::create_string({"_NAMED_TEST_CASE_THREADED(",#_a_name, ",",#_a_description,",",#_a_path,",",#_a_threads,")"})\
	)