#pragma once
#include "abc_test/core/ds/test_collections/test_list.h"
#include "abc_test/core/test_options.h"
#include "abc_test/core/test_main.h"

// All tests
#include "abc_test_examples/enumeration.h"

int main(int argc, char* argv[])
{
	using namespace abc;
	using namespace std;
	using namespace abc::reporters;
	ds::test_list_t _l_tl = {
		Establish88
	};
	test_options_t _l_to;
	test_main_t _l_test_main;
	_l_to._m_threads = 24;
	_l_test_main.set_options(_l_to);
	text_test_reporter_t _l_i;
	_l_test_main.add_test_reporter(
		//Owned
		std::shared_ptr<text_test_reporter_t>(new text_test_reporter_t())
	);
	_l_test_main.add_test_list(&_l_tl);
	_l_test_main.run_tests();
}