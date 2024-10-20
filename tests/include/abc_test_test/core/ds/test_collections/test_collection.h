#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test_test/pc_test_reporter.h"
#include "abc_test/internal/test_collection.h"
#include "abc_test/utility/combinatorics.h"

TEST_CASE("Test collection default", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	test_collection_t _l_tc;
	test_runner_t _l_tr;
	_l_tc.run(_l_tr);
	CHECK(_l_tr.tests_ran() == 0);
}

#define SPEC_FUNCTION_CHECK2(integer) void _f_check_##integer(test::test_runner_t& _a_runner) \
{}

namespace ab::f2
{
	SPEC_FUNCTION_CHECK2(1);
	SPEC_FUNCTION_CHECK2(2);
	SPEC_FUNCTION_CHECK2(3);
	SPEC_FUNCTION_CHECK2(4);
	SPEC_FUNCTION_CHECK2(5);
	SPEC_FUNCTION_CHECK2(6);
	SPEC_FUNCTION_CHECK2(7);
	SPEC_FUNCTION_CHECK2(8);
}

TEST_CASE("Test collection", "ab::test")
{
	using namespace ab::test;
	using namespace std;
	using enum setup_test_error_enum_t;
	using unit_test = std::tuple<std::vector<std::pair<registered_test_data_t,optional<setup_test_error_enum_t>>>,test_options_t>;
	std::source_location _l_source_loc;
	vector<unit_test> _l_tests =
	{
		{
			{
				{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_1,_l_source_loc),{}},
					{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_2,_l_source_loc),{}},
					{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_3,_l_source_loc),{}},
					{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_4,_l_source_loc),{} },
					{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_5,_l_source_loc),{}},
					{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_6,_l_source_loc), {}},
					{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_7,_l_source_loc), {}},
					{registered_test_data_t("first","hello::goodbye",ab::f2::_f_check_8,_l_source_loc), {}},
			},test_options_t()
		},
		{
			{
					{registered_test_data_t("first","hello::ab::ac",ab::f2::_f_check_1,_l_source_loc),{}},
					{registered_test_data_t("first","hello::ab::ac",ab::f2::_f_check_2,_l_source_loc),{}},
					{registered_test_data_t("first","hello::ac::ab",ab::f2::_f_check_3,_l_source_loc),{}},
					{registered_test_data_t("first","hello::ac::ab::dc",ab::f2::_f_check_4,_l_source_loc),{}},
					{registered_test_data_t("first","hello::ac::ab::dc",ab::f2::_f_check_5,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de",ab::f2::_f_check_6,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de::fg",ab::f2::_f_check_7,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de::fg::hi::jk",ab::f2::_f_check_8,_l_source_loc),{}},
			},test_options_t()
		},
		{
			{
					{registered_test_data_t("first","hello::ab::ac",ab::f2::_f_check_1,_l_source_loc),{}},
					{registered_test_data_t("first2","hello::ab::ac",ab::f2::_f_check_2,_l_source_loc),{}},
					{registered_test_data_t("first ","hello::ac::ab",ab::f2::_f_check_3,_l_source_loc),{}},
					{registered_test_data_t("1","hello::ac::ab::dc",ab::f2::_f_check_4,_l_source_loc),{}},
					{registered_test_data_t("2","hello::ac::ab::dc",ab::f2::_f_check_5,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de",ab::f2::_f_check_6,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de::fg",ab::f2::_f_check_7,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de::fg::hi::jk",ab::f2::_f_check_8,_l_source_loc),{}},
			},test_options_t()
		},
		{
			{
					{registered_test_data_t("first","hello::ab::ac",nullptr,_l_source_loc),{FUNCTION_IS_NULLPTR}},
					{registered_test_data_t("first2","hello::ab::ac",ab::f2::_f_check_2,_l_source_loc),{}},
					{registered_test_data_t("first ","hello::ac::ab",ab::f2::_f_check_3,_l_source_loc),{}},
					{registered_test_data_t("1","hello::ac::ab::dc",ab::f2::_f_check_4,_l_source_loc),{}},
					{registered_test_data_t("1","hello::ac::ab::dc",ab::f2::_f_check_5,_l_source_loc),{DUPLICATE_TEST_ENTRY}},
					{registered_test_data_t("first","hello::de",ab::f2::_f_check_6,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de::fg",ab::f2::_f_check_7,_l_source_loc),{}},
					{registered_test_data_t("first","hello::de::fg::hi::jk",ab::f2::_f_check_8,_l_source_loc),{}},
			},test_options_t()
		}
	};
	for (auto& [_l_vector, _l_opts] : _l_tests)
	{
		setup_test_errors_t _l_expected_setup_test_errors;
		vector<test_info_t> _l_expected_info_ti;
		vector<registered_test_data_t> _l_data_vector;
		for (auto& [_l_rg, _l_enum] : _l_vector)
		{
			if (_l_enum.has_value())
			{
				_l_expected_setup_test_errors.push_back(setup_test_error_t(_l_enum.value(), _l_rg._m_test_info));
			}
			else
			{
				_l_data_vector.push_back(_l_rg);
				_l_expected_info_ti.push_back(_l_rg._m_test_info);
			}
		}
		ab::test::utility::combinatorics<registered_test_data_t,true,true> _l_comb;
		_l_comb.elements = _l_data_vector;
		_l_comb.min_number_of_elements_to_generate = _l_vector.size();
		_l_comb.max_number_of_elements_to_generate = _l_vector.size();
		auto _l_end{ _l_comb.end() };
		auto _l_begin{ _l_comb.begin() };
		for (auto&& _l_c : _l_comb)
		{
			list<registered_test_data_t> _l_list;
			std::ranges::copy(_l_c, std::back_inserter(_l_list));
			test_runner_t _l_tr;
			pc_test_reporter* _l_pc = new pc_test_reporter();
			_l_tr.add_reporters({ test_reporter_ptr_t(_l_pc) });
			setup_tests_log_t _l_ste;
			_l_ste.add_reporters({});
			test_collection_t _l_tc(_l_opts, _l_ste);
			test_lists_element_t _l_list_ = &_l_list;
			test_lists_t _l_tlt;
			_l_tlt.push_back(_l_list_);
			_l_tc.add_tests(_l_tlt);
			_l_tc.run(_l_tr);
			auto& _l_res{ _l_pc->get_all_test_infos() };
			CHECK(_l_res == _l_expected_info_ti);
		//	CHECK(_l_pc-> == _l_expected_setup_test_errors);
		}
	}
}