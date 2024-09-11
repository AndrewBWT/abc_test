#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test_test/pc_test_reporter.h"
#include "abc_test/core/test_list.h"

namespace ab::f
{
	void _f1(test::test_runner_t&)
	{

	}
}

TEST_CASE("Test_list test", "ab::test")
{
	using namespace ab::test;
	clear_test_list();

	const test_list_t& _l_test_list = get_global_test_list();
	CHECK(_l_test_list.size() == 0);
	add_test(registered_test_data_t());
	CHECK(_l_test_list.size() == 1);
	clear_test_list();
	CHECK(_l_test_list.size() == 0);
	add_test(registered_test_data_t("", "", nullptr, std::source_location::current()));
	CHECK(_l_test_list.size() == 1);
	clear_test_list();
	add_test(registered_test_data_t("", "", ab::f::_f1, std::source_location::current()));
	CHECK(_l_test_list.size() == 1);
	clear_test_list();
	std::source_location _l_sl = std::source_location::current();
	add_test(registered_test_data_t("descriptio", "pathy", ab::f::_f1, _l_sl));
	CHECK(_l_test_list.size() == 1);
	registered_test_data_t _l_rt = *_l_test_list.begin();
	CHECK(_l_rt._m_function == ab::f::_f1);
	test_info_t _l_expected("descriptio", "pathy", _l_sl, { });
	CHECK(_l_rt._m_test_info == _l_expected);
	clear_test_list();
	CHECK(_l_test_list.size() == 0);
}