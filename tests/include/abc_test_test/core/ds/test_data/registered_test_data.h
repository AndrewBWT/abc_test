#pragma once
#include "abc_test_test/test_utilities/register_test.h"

#include "abc_test_test/pc_test_reporter.h"
#include "abc_test/core/register_test.h"

#include <optional>

namespace ab::test
{
	void _l_f3(test_runner_t&) {}
}

TEST_CASE("Register test default", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	register_test_t _l_rg;
	check_register_test(_l_rg, true, test_info_t());
}

TEST_CASE("Register test", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	source_location _l_source_loc = source_location::current();
	void(*_l_f1)(test_runner_t&) = nullptr;
	using unit_test = std::tuple<string,string, void(*)(test_runner_t&),source_location,vector<string>>;
	vector<unit_test> _l_tests =
	{
		{"hello","path::to::test",_l_f1,_l_source_loc,{"path","to","test"}},
		{"","",_l_f2,_l_source_loc,{}},
		{"hello","path::to::test",_l_f3,_l_source_loc,{"path","to","test"}}
	};
	for (auto& [_l_desc, _l_path, _l_f, _l_source, _l_vect] : _l_tests)
	{
		test_info_t _l_ti_1(_l_desc, _l_path, _l_source, {});
		test_info_t _l_ti_2(_l_desc, _l_path, _l_source, _l_vect);
		register_test_t _l_rt(_l_desc, _l_path, _l_f, _l_source);
		check_register_test(_l_rt, _l_f == nullptr, _l_ti_1);
		_l_rt.initialise_test_hierarchy("::");
		check_register_test(_l_rt, _l_f == nullptr, _l_ti_2);
	}
}