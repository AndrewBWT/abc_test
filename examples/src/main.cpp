#pragma once
#include "abc_test/core/ds/test_collections/test_list.h"
#include "abc_test/core/test_options.h"
#include "abc_test/core/test_main.h"

// All tests
//#include "abc_test_examples/enumeration.h"
#include "abc_test_examples/basic_tests.h"

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
	using namespace abc;
	using namespace std;
	using namespace abc::reporters;
	using namespace abc::ds;
	test_options_t _l_to;
	test_main_t _l_test_main;
	_l_to._m_threads = 1;
	_l_test_main.set_options(_l_to);
	_l_test_main.add_test_reporter_owned_by_class(new text_test_reporter_t());
	_l_test_main.add_global_test_list();
	_l_test_main.run_tests();
}