/*#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/utility/combinatorics/combinations.h"
#include <algorithm>

namespace test
{
	template<
		typename T
	> 
	using comb_test = std::tuple<std::vector<std::vector<T>>, size_t>;
	template<
		typename R,
		typename T
	>
	using unit_test_x = std::tuple < R, std::vector<comb_test<T>>,
		std::vector<comb_test<T>>>;
	template<
		typename T,
		bool Duplicates_Allowed
	>
	void combination_test2(
	ab::test::utility::combinations<T, Duplicates_Allowed>& _a_combinations,
		const size_t _a_base_comb_val,
		const std::vector<std::vector<T>>& _a_base_comb_vect
	) {

		using namespace std;
		vector<vector<T>> _l_base_comb_vect_sorted = _a_base_comb_vect;
		for (auto& _l_element : _l_base_comb_vect_sorted)
		{
			std::ranges::sort(_l_element);
		}
		std::ranges::sort(_l_base_comb_vect_sorted);
		const vector<int>& _l_comb_ref = _a_combinations.get_result_reference();
		CHECK(_a_base_comb_val == _a_combinations.get_numb_elements_in_result_combination());
		vector<vector<int>> _l_results;
		if (not _a_combinations.is_finished())
		{
			do
			{
				_l_results.push_back(_l_comb_ref);
				CHECK(_a_base_comb_val == _a_combinations.get_numb_elements_in_result_combination());
			} while (_a_combinations.generate_next_result());
		}
		for (auto& _l_element : _l_results)
		{
			std::ranges::sort(_l_element);
		}
		std::ranges::sort(_l_results);
		CHECK(_l_results == _l_base_comb_vect_sorted);
	}
	template<
		typename R,
		typename T,
		bool Duplicates_Allowed
	>
	void combination_test3(
		const R _a_elements,
		const std::vector<comb_test<T>>& _a_comb_test_data
	) {
		using namespace std;
		using namespace ab::test::utility;
		if (_a_comb_test_data.size() > 0)
		{
			auto&& [_l_base_comb_vect, _l_base_comb_val] {_a_comb_test_data[0]};
			combinations<T, Duplicates_Allowed> _l_comb(_a_elements, _l_base_comb_val);
			combination_test2(_l_comb, _l_base_comb_val, _l_base_comb_vect);
			_l_comb.reset();
			combination_test2(_l_comb, _l_base_comb_val, _l_base_comb_vect);
			for (auto&& [_l_res, _l_reset_val] : _a_comb_test_data)
			{
				_l_comb.reset(_l_reset_val);
				combination_test2(_l_comb, _l_reset_val, _l_res);
				_l_comb.reset();
				combination_test2(_l_comb, _l_reset_val, _l_res);
			}
		}
	}
	template<
		typename R,
		typename T
	>
	void combination_test(

	) noexcept
	{
		using namespace std;
		using namespace ab::test::utility;
		vector<unit_test_x<R, T>> _l_tests =
		{
			{
				{1,2,3,4},
				{
					{
						{
							{1,2,3,4},
						},
						4
					},
					{
						{
							{1,2,3},
							{1,2,4},
							{2,3,4},
							{1,3,4}
						},
						3
					},
					{
						{
							{1,2},
							{1,3},
							{1,4},
							{2,3},{2,4},{3,4}
						},
						2
					},
					{
						{
							{1},{2},{3},{4}
						},
						1
					},
					{
						{
							{}
						},
						0
					},
				},
				{
					{
						{
							{1,2,3,4}
						},
						4
					},
					{
						{
							{1,2,3},
							{1,2,4},
							{2,3,4},
							{1,3,4}
						},
						3
					},
					{
						{
							{1,2},
							{1,3},
							{1,4},
							{2,3},{2,4},{3,4}
						},
						2
					},
					{
						{
							{1},{2},{3},{4}
						},
						1
					},
					{
						{
							{}
						},
						0
					},
				},
			},
			{
				{ 1,2,3,4,1 },
				{
					{
						{
							{1,2,3,4}
						},
						4
					},
					{
						{
							{1,2,3},
							{1,2,4},
							{2,3,4},
							{1,3,4}
						},
						3
					},
					{
						{
							{1,2},
							{1,3},
							{1,4},
							{2,3},{2,4},{3,4}
						},
						2
					},
					{
						{
							{1},{2},{3},{4}
						},
						1
					},
					{
						{
							{}
						},
						0
					},
				},
				{
					{
						{
							{1,1,2,3,4},
						},
						5
					},
					{
						{
							{1,2,3,1},
							{1,1,4,3},
							{1,1,2,4},
							{1,2,3,4},
							{1,2,3,4}
						},
						4
					},
					{
						{
							{1,1,2},
							{ 1,1,3 },{1,1,4},
							{1,2,3},{1,2,4},{1,2,3},{1,2,4},{1,3,4},{1,3,4},
							{ 2,3,4 }
						},
						3
					},
					{
						{
							{1,1},
							{ 1,2 },{1,2},
							{ 1,3 },{1,3},
							{ 1,4 },{1,4},
							{2,3},{2,4},{3,4}
						},
						2
					},
					{
						{
							{1},{2},{3},{4},{1}
						},
						1
					},
					{
						{
							{}
						},
						0
					},
				},
			}
		};
		for (auto& [_l_r,_l_comb_tests_false, _l_comb_tests_true] : _l_tests)
		{
			combination_test3<R, T, false>(_l_r, _l_comb_tests_false);
			combination_test3<R, T, true>(_l_r, _l_comb_tests_true);
		}
	}
}

TEST_CASE("Combination tests for well constructed output", "ab::test")
{
	using namespace ab::test::utility;
	using namespace std;
	using namespace test;
	combination_test<vector<int>,int>();
}

TEST_CASE("Combination tests for default constructor", "ab::test")
{
	using namespace ab::test::utility;
	using namespace std;
	combinations<int> _l_comb;
	vector<int> empty;
	CHECK(_l_comb.get_current_result() == empty);
	CHECK(_l_comb.get_elements() == empty);
	CHECK(_l_comb.get_numb_elements_in_result_combination() == 0);
	CHECK(_l_comb.get_result_reference() == empty);
	CHECK(_l_comb.is_finished() == false);
	CHECK(_l_comb.generate_next_result() == false);
	CHECK(_l_comb.is_finished() == true);
	CHECK(_l_comb.reset() == true);
	CHECK(_l_comb.get_current_result() == empty);
	CHECK(_l_comb.get_elements() == empty);
	CHECK(_l_comb.get_numb_elements_in_result_combination() == 0);
	CHECK(_l_comb.get_result_reference() == empty);
	CHECK(_l_comb.is_finished() == false);
	CHECK(_l_comb.generate_next_result() == false);
	CHECK(_l_comb.is_finished() == true);
	CHECK(_l_comb.reset(1) == false);
	CHECK(_l_comb.get_current_result() == empty);
	CHECK(_l_comb.get_elements() == empty);
	CHECK(_l_comb.get_numb_elements_in_result_combination() == 0);
	CHECK(_l_comb.get_result_reference() == empty);
	CHECK(_l_comb.is_finished() == false);
	CHECK(_l_comb.generate_next_result() == false);
	CHECK(_l_comb.is_finished() == true);
}*/