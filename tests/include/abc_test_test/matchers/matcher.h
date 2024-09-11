#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/matcher.h"
#include "abc_test_test/pc_test_reporter.h"
#include "abc_test/core/test_runner.h"
#include "abc_test_test/example_matcher.h"

TEST_CASE("Matcher tests", "ab::test")
{
	using namespace ab::test;
	matcher_t _m_matcher;
	test_runner _l_tr;
	CHECK(_m_matcher.get_failure() == "");
	CHECK(_m_matcher.get_failure_msg() == "");
	CHECK(_m_matcher.passed() == false);
	CHECK(_m_matcher.internal_matcher() == nullptr);
	_m_matcher.run_test(_l_tr);
	CHECK(_m_matcher.get_failure() == "");
	CHECK(_m_matcher.get_failure_msg() == "");
	CHECK(_m_matcher.passed() == false);
	CHECK(_m_matcher.internal_matcher() == nullptr);
	matcher_internal_ptr _l_mip_1(new example_matcher("hello", true));
	matcher_t _m_matcher2(_l_mip_1);
	CHECK(_m_matcher2.get_failure() == "");
	CHECK(_m_matcher2.get_failure_msg() == "");
	CHECK(_m_matcher2.passed() == false);
	CHECK(_m_matcher2.internal_matcher() == _l_mip_1);
	_m_matcher2.run_test(_l_tr);
	CHECK(_m_matcher2.get_failure() == "");
	CHECK(_m_matcher2.get_failure_msg() == "hello");
	CHECK(_m_matcher2.passed() == true);
	CHECK(_m_matcher2.internal_matcher() == _l_mip_1);

	matcher_internal_ptr _l_mip_2(new example_matcher("hello", true));
	_m_matcher2 = matcher_t(_l_mip_2);
	CHECK(_m_matcher2.get_failure() == "");
	CHECK(_m_matcher2.get_failure_msg() == "");
	CHECK(_m_matcher2.passed() == false);
	CHECK(_m_matcher2.internal_matcher() == _l_mip_2);
	_m_matcher2.run_test(_l_tr);
	CHECK(_m_matcher2.get_failure() == "");
	CHECK(_m_matcher2.get_failure_msg() == "hello");
	CHECK(_m_matcher2.passed() == true);
	CHECK(_m_matcher2.internal_matcher() == _l_mip_2);


		/*test_runner _l_tr;
	pc_test_reporter* _l_pc_tr = new pc_test_reporter();
	_l_tr.add_reporter(_l_pc_tr);
	_l_exm.run_test(_l_tr);*/
}