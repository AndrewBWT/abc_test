#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/matcher/generic_matcher.h"
#include "abc_test_test/example_matcher.h"

#include "abc_test_test/pc_test_reporter.h"

TEST_CASE("Internal catcher example extension tests", "ab::test")
{
	using namespace ab::test;
	example_matcher _l_exm;
	CHECK(_l_exm.get_failure() == "");
	CHECK(_l_exm.get_failure_msg() == "");
	CHECK(_l_exm.passed() == false);
	test_runner _l_tr;
	pc_test_reporter* _l_pc_tr = new pc_test_reporter();
	_l_tr.add_reporter(_l_pc_tr);
	_l_exm.run_test(_l_tr);
	CHECK(_l_exm.get_failure() == "");
	CHECK(_l_exm.get_failure_msg() == "");
	CHECK(_l_exm.passed() == false);

	_l_exm = example_matcher("Placeholder", false);
	CHECK(_l_exm.get_failure() == "");
	CHECK(_l_exm.get_failure_msg() == "");
	CHECK(_l_exm.passed() == false);
	_l_exm.run_test(_l_tr);
	CHECK(_l_exm.get_failure() == "Placeholder");
	CHECK(_l_exm.get_failure_msg() == "Placeholder");
	CHECK(_l_exm.passed() == false);
	_l_pc_tr->clear_errors();

	_l_exm = example_matcher("Placeholder", true);
	CHECK(_l_exm.get_failure() == "");
	CHECK(_l_exm.get_failure_msg() == "");
	CHECK(_l_exm.passed() == false);
	_l_exm.run_test(_l_tr);
	CHECK(_l_exm.get_failure() == "");
	CHECK(_l_exm.get_failure_msg() == "Placeholder");
	CHECK(_l_exm.passed() == true);
}