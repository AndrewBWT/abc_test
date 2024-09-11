#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/utility/combinatorics.h"
#include <map>
#include <iterator>

#include <iostream>
#include <functional>

namespace test
{
	template<
		typename T
	>
	void
		check_results(
			const std::vector<std::vector<T>>& _a_expected,
			const std::vector<std::vector<T>>& _a_actual,
			const std::string_view _a_info
		)
	{
		using namespace std;
		if (_a_actual != _a_expected)
		{
			INFO(format("_a_info = {0}", _a_info));
			string info;
			if (_a_actual.size() != _a_expected.size())
			{
				info.append(format("Size not the same: actual = {0}, expected = {1}\n",
					_a_actual.size(), _a_expected.size()));
			}
			for (size_t _l_idx{ 0 }; _l_idx < std::min(_a_actual.size(), _a_expected.size()); _l_idx++)
			{
				if (_a_actual[_l_idx] != _a_expected[_l_idx])
				{
					info.append(format("At index {0} ", _l_idx));
					string _l_actual, _l_expected;
					for (auto&& _l_element : _a_actual[_l_idx])
					{
						_l_actual.append(format("{0},", _l_element));
					}
					_l_actual.append("}");
					for (auto&& _l_element : _a_expected[_l_idx])
					{
						_l_expected.append(format("{0},", _l_element));
					}
					_l_expected.append("}");
					info.append(format("{0} vs {1}", _l_actual, _l_expected));
					break;
				}
			}
			INFO(info);
			REQUIRE(_a_actual == _a_expected);
		}
		CHECK(_a_actual == _a_expected);
	}
	template<
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations, 
		bool Is_Forward_Iterator
	>
	void
		check_iterator(
			ab::test::utility::combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>& _a_lhs,
			ab::test::utility::combinatorics_iterator<T, Allow_Duplicates, Generate_Permutations, Is_Forward_Iterator>& _a_rhs
		)
	{
		if (_a_lhs != _a_rhs)
		{
			REQUIRE(_a_lhs == _a_rhs);
		}
	}
	template<
		typename T,
		typename Iterator_Start,
		typename Iterator_End,
		size_t Increment_Type
	>
	void
		combinatorics_tests_5b(
			Iterator_Start _a_start,
			Iterator_End _a_end,
			const std::string_view _a_str
		)
	{
		using namespace std;
		INFO(format("String identifier = {0}", _a_str));
		std::cout << format("String identifier = {0}", _a_str) << std::endl;
		//Figure out size.
		vector<vector<Iterator_Start>> _l_itts;
		size_t _l_size{ 0 };
	//	std::cout << "+++" << std::endl;
		for (auto _l_itt = _a_start; _l_itt != _a_end; _l_itt++)
		{
			_l_size++;
		}
	//	std::cout << "===" << std::endl;
		size_t _l_idx{ 0 };
		std::function<void(Iterator_Start&)> _l_f_incr;
		std::function<void(Iterator_Start&)> _l_f_decr;
		if constexpr (Increment_Type == 0)
		{
			_l_f_incr = [](auto& _l_itt) {
				_l_itt++;
				};
			_l_f_decr = [](auto& _l_itt) {
				_l_itt--;
				};
		}
		else if constexpr (Increment_Type == 1)
		{
			_l_f_incr = [](auto& _l_itt) {
				++_l_itt;
				};
			_l_f_decr = [](auto& _l_itt) {
				_l_itt--;
				};
		}
		else if constexpr (Increment_Type == 2)
		{
			_l_f_incr = [](auto& _l_itt) {
				_l_itt++;
				};
			_l_f_decr = [](auto& _l_itt) {
				--_l_itt;
				};
		}
		else if constexpr (Increment_Type == 3)
		{
			_l_f_incr = [](auto& _l_itt) {
				++_l_itt;
				};
			_l_f_decr = [](auto& _l_itt) {
				--_l_itt;
				};
		}
		for (auto _l_itt = _a_start; _l_itt != _a_end; _l_f_incr(_l_itt))
		{
		//	_l_itt++;
		//	_l_idx = 1;
			//std::cout << format("idx = {0}", _l_idx) << std::endl;
			INFO(format("idx = {0}", _l_idx));
			for (size_t _l_jdx{_l_idx + 1}; _l_jdx > 0; _l_jdx--)
			{
		//		std::cout << format("Minus jdx = {0}", _l_jdx) << std::endl;
				INFO(format("Minus jdx = {0}", _l_jdx));
				//Go down by _l_jdx, then back up by _l_jdx.
				auto _l_itt_cpy{ _l_itt };
				for (size_t _l_kdx{ 0 }; _l_kdx < _l_jdx - 1; _l_kdx++)
				{
					_l_f_decr(_l_itt_cpy);
				}
				for (size_t _l_kdx{ 0 }; _l_kdx < _l_jdx - 1; _l_kdx++)
				{
					_l_f_incr(_l_itt_cpy);
				}
				REQUIRE(_l_itt_cpy == _l_itt);
			}
			const size_t _l_cmp_value{ _l_size - _l_idx };
			for (size_t _l_jdx{ 0 }; _l_jdx < _l_cmp_value; _l_jdx++)
			{
			//	std::cout << format("Plus jdx = {0}", _l_jdx) << std::endl;
				INFO(format("Plus jdx = {0}", _l_jdx));
				//Go down by _l_jdx, then back up by _l_jdx.
				auto _l_itt_cpy{ _l_itt };
				vector< Iterator_Start> _l_itts1;
				for (size_t _l_kdx{ 0 }; _l_kdx < _l_jdx; _l_kdx++)
				{
					_l_itts1.push_back(_l_itt_cpy);
					_l_f_incr(_l_itt_cpy);
				}
				_l_itts1.push_back(_l_itt_cpy);
				vector< Iterator_Start> _l_itts;
				auto cmp = _l_itts1.rbegin();
				auto _l_ldx = _l_itts1.size();
				auto _l_ldx_start = _l_ldx;
				CHECK(*cmp == _l_itt_cpy);
				++cmp;
				_l_itts.push_back(_l_itt_cpy);
				for (size_t _l_kdx{ 0 }; _l_kdx < _l_jdx; _l_kdx++)
				{
					_l_f_decr(_l_itt_cpy);
					_l_itts.push_back(_l_itt_cpy);
					INFO(format("_l_ldx = {0}", _l_ldx));
					auto _l_check = *cmp;
					if (_l_itt_cpy != _l_check)
					{
						REQUIRE(_l_itt_cpy == _l_check);
					}
					cmp++;
				}
				if (_l_itt_cpy != _l_itt)
				{
					REQUIRE(_l_itt_cpy == _l_itt);
				}
			}
			_l_idx++;
		}
	}
	template<
		typename T,
		typename Iterator_Start,
		typename Iterator_End,
		size_t Increment_Type
	>
	void
		combinatorics_tests_5(
			Iterator_Start _a_start,
			Iterator_End _a_end,
			std::vector < std::vector<T>>& _a_expected_results,
			const std::string_view _a_str
		)
	{
		using namespace std;
		vector<vector<T>> _l_actual_results;
		if constexpr (Increment_Type == 0)
		{
			for (auto _l_itt = _a_start; _l_itt != _a_end; ++_l_itt)
			{
	//			_l_itt--;
				_l_actual_results.push_back(*_l_itt);
			}
		}
		else if constexpr (Increment_Type == 1)
		{
			for (auto _l_itt = _a_start; _l_itt != _a_end; _l_itt++)
			{
				_l_actual_results.push_back(*_l_itt);
			}
		}
		else if constexpr (Increment_Type == 2)
		{
			auto _l_itt = _a_end;
			size_t _l_idx = 0;
			if (_l_itt != _a_start)
			{
				do
				{
					--_l_itt;
					_l_actual_results.push_back(*_l_itt);
					if (_a_expected_results[_l_idx++] != *_l_itt)
					{
						FAIL();
					}
				} while (_l_itt != _a_start);
			}
		}
		else if constexpr (Increment_Type == 3)
		{
			auto _l_itt = _a_end;
			if (_l_itt != _a_start)
			{
				do
				{
					_l_itt--;
					_l_actual_results.push_back(*_l_itt);
				} while (_l_itt != _a_start);
			}
		}
		check_results(_a_expected_results, _l_actual_results, _a_str);
	}
	template<
		typename T,
		typename Combinatorics_Class,
		size_t Increment_Type
	>
	void
		combinatorics_tests_4(
			Combinatorics_Class _l_comb,
			std::vector < std::vector<T>>& _a_expected_results
		)
	{
		using namespace std;
		using C = std::remove_reference<Combinatorics_Class>::type;
		using Itt = C::iterator;
		using Rev = C::reverse_iterator;
		auto _l_reverse_results = _a_expected_results;
		ranges::reverse(_l_reverse_results);
			combinatorics_tests_5<T, Itt, Itt, Increment_Type>(_l_comb.begin(), _l_comb.end(), _a_expected_results, "forward");
			combinatorics_tests_5<T, Itt, Itt, Increment_Type>(_l_comb.cbegin(), _l_comb.cend(), _a_expected_results, "const forward");
		combinatorics_tests_5<T, Rev, Rev, Increment_Type>(_l_comb.rbegin(), _l_comb.rend(), _l_reverse_results, "backward");
			combinatorics_tests_5<T, Rev, Rev, Increment_Type>(_l_comb.crbegin(), _l_comb.crend(), _l_reverse_results, "const backward");

		combinatorics_tests_5<T, Itt, Itt, Increment_Type>(std::ranges::begin(_l_comb), std::ranges::end(_l_comb), _a_expected_results, "ranges forward");
		combinatorics_tests_5<T, Itt, Itt, Increment_Type>(std::ranges::cbegin(_l_comb), std::ranges::cend(_l_comb), _a_expected_results, "ranges const forward");
			combinatorics_tests_5<T, Rev, Rev, Increment_Type>(std::ranges::rbegin(_l_comb), std::ranges::rend(_l_comb), _l_reverse_results, "ranges backward");
			combinatorics_tests_5<T, Rev, Rev, Increment_Type>(std::ranges::crbegin(_l_comb), std::ranges::crend(_l_comb), _l_reverse_results, "ranges const backward");

		combinatorics_tests_5<T, Itt, Itt, Increment_Type>(std::begin(_l_comb), std::end(_l_comb), _a_expected_results, "std forward");
		combinatorics_tests_5<T, Itt, Itt, Increment_Type>(std::cbegin(_l_comb), std::cend(_l_comb), _a_expected_results, "ranges std forward");
			combinatorics_tests_5<T, Rev, Rev, Increment_Type>(std::rbegin(_l_comb), std::rend(_l_comb), _l_reverse_results, "std backward");
			combinatorics_tests_5<T, Rev, Rev, Increment_Type>(std::crbegin(_l_comb), std::crend(_l_comb), _l_reverse_results, "ranges std backward");
	}
	template<
		typename T,
		typename Combinatorics_Class,
		size_t Increment_Type
	>
	void
		combinatorics_tests_4b(
			Combinatorics_Class _l_comb
		)
	{
		using namespace std;
		using C = std::remove_reference<Combinatorics_Class>::type;
		using Itt = C::iterator;
		using Rev = C::reverse_iterator;
		vector<pair<Itt, string>> _l_begins =
		{
			{_l_comb.begin(), "begin"},
			{_l_comb.cbegin(), "cbegin"},
			{std::begin(_l_comb), "std::begin"},
			{std::cbegin(_l_comb), "std::cbegin"},
			{std::ranges::begin(_l_comb), "std::ranges::begin"},
			{std::ranges::cbegin(_l_comb), "std::ranges::cbegin"},
		};
		for (auto&& _l_begin1 : _l_begins)
		{
			for (auto&& _l_begin2 : _l_begins)
			{
				if (_l_begin1.first != _l_begin2.first)
				{
					REQUIRE(_l_begin1.first == _l_begin2.first);
				}
			}
		}
		vector<pair<Itt, string>> _l_ends =
		{
			{_l_comb.end(), "end"},
			{_l_comb.cend(), "cend"},
			{std::end(_l_comb), "std::end"},
			{std::cend(_l_comb), "std::cend"},
			{std::ranges::end(_l_comb), "std::ranges::end"},
			{std::ranges::cend(_l_comb), "std::ranges::cend"},
		};
		for (auto&& _l_begin1 : _l_ends)
		{
			for (auto&& _l_begin2 : _l_ends)
			{
				if (_l_begin1.first != _l_begin2.first)
				{
					REQUIRE(_l_begin1.first == _l_begin2.first);
				}
			}
		}
		auto&& [_l_begin, _l_begin_str] {_l_begins[0]};
		auto&& [_l_end, _l_end_str]{_l_ends[0]};
		combinatorics_tests_5b<T, Itt, Itt, Increment_Type>(_l_begin, _l_end, _l_begin_str + " " + _l_end_str);
		for (auto&& [_l_begin, _l_begin_str] : _l_begins)
		{
			for (auto&& [_l_end, _l_end_str] : _l_ends)
			{
		//		combinatorics_tests_5b<T, Itt, Itt, Increment_Type>(_l_begin, _l_end, _l_begin_str + " " + _l_end_str);
			}
		}

		vector<pair<Rev, string>> _l_rbegins =
		{
			{_l_comb.rbegin(), "rbegin"},
			{_l_comb.crbegin(), "crbegin"},
			{std::rbegin(_l_comb), "std::rbegin"},
			{std::crbegin(_l_comb), "std::crbegin"},
			{std::ranges::rbegin(_l_comb), "std::ranges::rbegin"},
			{std::ranges::crbegin(_l_comb), "std::ranges::crbegin"},
		};
		for (auto&& _l_begin1 : _l_rbegins)
		{
			for (auto&& _l_begin2 : _l_rbegins)
			{
				if (_l_begin1.first != _l_begin2.first)
				{
					REQUIRE(_l_begin1.first == _l_begin2.first);
				}
			}
		}
		vector<pair<Rev, string>> _l_rends =
		{
			{_l_comb.rend(), "rend"},
			{_l_comb.crend(), "crend"},
			{std::rend(_l_comb), "std::rend"},
			{std::crend(_l_comb), "std::crend"},
			{std::ranges::rend(_l_comb), "std::ranges::rend"},
			{std::ranges::crend(_l_comb), "std::ranges::crend"},
		};
		for (auto&& _l_begin1 : _l_rends)
		{
			for (auto&& _l_begin2 : _l_rends)
			{
				if (_l_begin1.first != _l_begin2.first)
				{
					REQUIRE(_l_begin1.first == _l_begin2.first);
				}
			}
		}
		//for (auto&& [_l_begin, _l_begin_str] : _l_rbegins)
		{
		//	for (auto&& [_l_end, _l_end_str] : _l_rends)
			{
		//		combinatorics_tests_5b<T, Rev, Rev, Increment_Type>(_l_begin, _l_end, _l_begin_str + " " + _l_end_str);
			}
		}
		{
			auto&& [_l_begin, _l_begin_str] {_l_rbegins[0]};
			auto&& [_l_end, _l_end_str] {_l_rends[0]};
			combinatorics_tests_5b<T, Rev, Rev, Increment_Type>(_l_begin, _l_end, _l_begin_str + " " + _l_end_str);
		}
	}
	template<
		typename T,
		typename Combinatorics_Class
	>
	void
		combinatorics_tests_3(
			Combinatorics_Class _l_comb,
			std::vector < std::vector<T>>&_a_expected_results
		)
	{
		using namespace std;
		using namespace ab::test::utility;
		using C = std::remove_reference<Combinatorics_Class>::type;
		using Itt = C::iterator;
		INFO(format("Combinatorics_Class = {0}", typeid(Combinatorics_Class).name()));
		std::cout << format("Combinatorics_Class = {0}", typeid(Combinatorics_Class).name()) << std::endl;
		vector<vector<T>> _l_actual_results;
		for (auto&& _l_element : _l_comb)
		{
			_l_actual_results.push_back(_l_element);
		}
		auto _l_reverse_results = _a_expected_results;
		ranges::reverse(_l_reverse_results);
		check_results(_a_expected_results, _l_actual_results, "range based for loop");
		{
			INFO("Increment = ++it, forward for loop");
			combinatorics_tests_4<T, Combinatorics_Class, 0>(_l_comb, _a_expected_results);
		}
		{
			INFO("Increment = it++, forward for loop");
			combinatorics_tests_4<T, Combinatorics_Class, 1>(_l_comb, _a_expected_results);
		}
		{
			INFO("Increment = --it, backward do-while loop");
			combinatorics_tests_4<T, Combinatorics_Class, 2>(_l_comb, _l_reverse_results);
		}
		{
			INFO("Increment = it--, backward do-while loop");
			combinatorics_tests_4<T, Combinatorics_Class, 3>(_l_comb, _l_reverse_results);
		}
		{
			INFO("Increment = ++it, Decrement = --it");
			combinatorics_tests_4b<T, Combinatorics_Class, 0>(_l_comb);
		}
		{
			INFO("Increment = ++it, Decrement = it--");
			combinatorics_tests_4b<T, Combinatorics_Class, 1>(_l_comb);
		}
		{
			INFO("Increment = it++, Decrement = --it");
			combinatorics_tests_4b<T, Combinatorics_Class, 2>(_l_comb);
		}
		{
			INFO("Increment = it++, Decrement = it--");
			combinatorics_tests_4b<T, Combinatorics_Class, 3>(_l_comb);
		}
	}
	template<
		typename T,
		typename R
	>
	std::vector<T> to_vector(
		const R& _a_r
	) {
		using namespace std;
		vector<T> _l_rv;
		for (auto&& _l_element : _a_r)
		{
			_l_rv.push_back(_l_element);
		}
		return _l_rv;
	}
	template<
		typename R,
		typename T,
		bool Allow_Duplicates,
		bool Generate_Permutations
	>
	void
		combinatorics_tests_2(
			const R& _a_r,
			const std::map<size_t, std::vector<std::vector<T>>>& _a_result_type
		)
	{
		using namespace std;
		using namespace ab::test::utility;
		INFO(format("Allow_Duplicates = {0}, Generate_Permutations = {1}",
			Allow_Duplicates, Generate_Permutations));
		vector<T> _l_r_to_vect = to_vector<T,R>(_a_r);
		combinatorics<T, Allow_Duplicates, Generate_Permutations, R> _l_comb_static;
		_l_comb_static.elements = _a_r;
		for (size_t _l_start_idx = 0; _l_start_idx < _a_result_type.size(); _l_start_idx++)
		{
			for (size_t _l_end_idx = _l_start_idx; _l_end_idx < _a_result_type.size(); _l_end_idx++)
			{
				std::cout << format("_l_start_idx = {0}, _l_end_idx = {1}", _l_start_idx, _l_end_idx) << std::endl;
				INFO(format("_l_start_idx = {0}, _l_end_idx = {1}", _l_start_idx, _l_end_idx));
				vector<vector<T>> _l_expected_results;
				for (size_t _l_idx = _l_start_idx; _l_idx <= _l_end_idx; _l_idx++)
				{
					auto _l_idx_result{ _a_result_type.at(_l_idx) };
					ranges::copy(_l_idx_result, std::back_inserter(_l_expected_results));
				}
				_l_comb_static.min_number_of_elements_to_generate = _l_start_idx;
				_l_comb_static.max_number_of_elements_to_generate = _l_end_idx;
				combinatorics_tests_3<T, combinatorics<int, Allow_Duplicates, Generate_Permutations, R>&>(_l_comb_static, _l_expected_results);
				combinatorics_tests_3<T, const combinatorics<int, Allow_Duplicates, Generate_Permutations, R>&>(_l_comb_static, _l_expected_results);
			}
		}
	}
	template<
		typename R,
		typename T
	>
	void
	combinatorics_tests_1(
	)
	{
		using namespace std;
		using namespace ab::test::utility;
		using result_type = map<size_t,vector<vector<T>>>;
		using unit_test = tuple<R,result_type, result_type, result_type, result_type>;
		vector<unit_test> _l_tests =
		{
			{
				{1,1,2,3,4},
				{
					{0,{{}}},
					{1,{{1},{2},{3},{4}}},
					{2,{{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}}},
					{3,{{1,2,3},{1,2,4},{1,3,4},{2,3,4}}},
					{4,{{1,2,3,4}}},
				},
				{
					{0,{{}}},
					{1,{{1},{1}, { 2 },{3},{4}}},
					{2,{{1,1},{1,2},{1,3},{1,4},{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}}},
					{3,{{1,1,2},{1,1,3},{1,1,4},{1,2,3},{1,2,4},{1,3,4},{1,2,3},{1,2,4},{1,3,4},{2,3,4}}},
					{4,{{1,1,2,3},{1,1,2,4},{1,1,3,4},{1,2,3,4},{1,2,3,4}}},
					{5,{{1,1,2,3,4}}}
				},
				{
					{0,{{}}},
					{1,{{1},{2},{3},{4}}},
					{2,{{1,2},{2,1}, { 1,3 },{3,1}, { 1,4 },{4,1},
						{ 2,3 },{3,2},{2,4},{4,2}, { 3,4 },{4,3}}},
					{3,{{1,2,3},{2,1,3},{3,1,2},{1,3,2},{2,3,1},{3,2,1},
						{1,2,4},{2,1,4},{4,1,2},{1,4,2},{2,4,1},{4,2,1},
						{1,3,4},{3,1,4},{4,1,3},{1,4,3},{3,4,1},{4,3,1},
						{2,3,4},{3,2,4},{4,2,3},{2,4,3},{3,4,2},{4,3,2}
						}
					},
					{4,{{1,2,3,4},{2,1,3,4},{3,1,2,4},{1,3,2,4},{2,3,1,4},{3,2,1,4},
						{4,2,1,3},{2,4,1,3},{1,4,2,3},{4,1,2,3},{2,1,4,3},{1,2,4,3},
						{1,3,4,2},{3,1,4,2},{4,1,3,2},{1,4,3,2},{3,4,1,2},{4,3,1,2},
						{4,3,2,1},{3,4,2,1},{2,4,3,1},{4,2,3,1},{3,2,4,1},{2,3,4,1},
						}
					},
				},
				{
					{0,{{}}},
					{1,{{1},{1}, { 2 },{3},{4}}},
					{2,{
						{1,1},{1,1},
						{1,2},{2,1},
						{1,3},{3,1},
						{1,4},{4,1},
						{1,2},{2,1},
						{1,3},{3,1},
						{1,4},{4,1},
						{2,3},{3,2},
						{2,4},{4,2},
						{3,4},{4,3}
						}
					},
					{3,{
						{1,1,2},{1,1,2},{2,1,1},{1,2,1},{1,2,1},{2,1,1},
						{1,1,3},{1,1,3},{3,1,1},{1,3,1},{1,3,1},{3,1,1},
						{1,1,4},{1,1,4},{4,1,1},{1,4,1},{1,4,1},{4,1,1},
						{1,2,3},{2,1,3},{3,1,2},{1,3,2},{2,3,1},{3,2,1},
						{1,2,4},{2,1,4},{4,1,2},{1,4,2},{2,4,1},{4,2,1},
						{1,3,4},{3,1,4},{4,1,3},{1,4,3},{3,4,1},{4,3,1},
						{1,2,3},{2,1,3},{3,1,2},{1,3,2},{2,3,1},{3,2,1},
						{1,2,4},{2,1,4},{4,1,2},{1,4,2},{2,4,1},{4,2,1},
						{1,3,4},{3,1,4},{4,1,3},{1,4,3},{3,4,1},{4,3,1},
						{2,3,4},{3,2,4},{4,2,3},{2,4,3},{3,4,2},{4,3,2}
						}
					},
					{4,{
						{1,1,2,3},{1,1,2,3},{2,1,1,3},{1,2,1,3},{1,2,1,3},{2,1,1,3},
						{3,1,1,2},{1,3,1,2},{1,3,1,2},{3,1,1,2},{1,1,3,2},{1,1,3,2},
						{1,2,3,1},{2,1,3,1},{3,1,2,1},{1,3,2,1},{2,3,1,1},{3,2,1,1},
						{3,2,1,1},{2,3,1,1},{1,3,2,1},{3,1,2,1},{2,1,3,1},{1,2,3,1},

						{1,1,2,4},{1,1,2,4},{2,1,1,4},{1,2,1,4},{1,2,1,4},{2,1,1,4},
						{4,1,1,2},{1,4,1,2},{1,4,1,2},{4,1,1,2},{1,1,4,2},{1,1,4,2},
						{1,2,4,1},{2,1,4,1},{4,1,2,1},{1,4,2,1},{2,4,1,1},{4,2,1,1},
						{4,2,1,1},{2,4,1,1},{1,4,2,1},{4,1,2,1},{2,1,4,1},{1,2,4,1},

						{1,1,3,4},{1,1,3,4},{3,1,1,4},{1,3,1,4},{1,3,1,4},{3,1,1,4},
						{4,1,1,3},{1,4,1,3},{1,4,1,3},{4,1,1,3},{1,1,4,3},{1,1,4,3},
						{1,3,4,1},{3,1,4,1},{4,1,3,1},{1,4,3,1},{3,4,1,1},{4,3,1,1},
						{4,3,1,1},{3,4,1,1},{1,4,3,1},{4,1,3,1},{3,1,4,1},{1,3,4,1},


						{1,2,3,4},{2,1,3,4},{3,1,2,4},{1,3,2,4},{2,3,1,4},{3,2,1,4},
						{4,2,1,3},{2,4,1,3},{1,4,2,3},{4,1,2,3},{2,1,4,3},{1,2,4,3},
						{1,3,4,2},{3,1,4,2},{4,1,3,2},{1,4,3,2},{3,4,1,2},{4,3,1,2},
						{4,3,2,1},{3,4,2,1},{2,4,3,1},{4,2,3,1},{3,2,4,1},{2,3,4,1},


						{1,2,3,4},{2,1,3,4},{3,1,2,4},{1,3,2,4},{2,3,1,4},{3,2,1,4},
						{4,2,1,3},{2,4,1,3},{1,4,2,3},{4,1,2,3},{2,1,4,3},{1,2,4,3},
						{1,3,4,2},{3,1,4,2},{4,1,3,2},{1,4,3,2},{3,4,1,2},{4,3,1,2},
						{4,3,2,1},{3,4,2,1},{2,4,3,1},{4,2,3,1},{3,2,4,1},{2,3,4,1},
						},
					},
					{5,{
						{1,1,2,3,4},{1,1,2,3,4},{2,1,1,3,4},{1,2,1,3,4},{1,2,1,3,4},{2,1,1,3,4},
						{3,1,1,2,4},{1,3,1,2,4},{1,3,1,2,4},{3,1,1,2,4},{1,1,3,2,4},{1,1,3,2,4},
						{1,2,3,1,4},{2,1,3,1,4},{3,1,2,1,4},{1,3,2,1,4},{2,3,1,1,4},{3,2,1,1,4},
						{3,2,1,1,4},{2,3,1,1,4},{1,3,2,1,4},{3,1,2,1,4},{2,1,3,1,4},{1,2,3,1,4},

						{4,2,3,1,1},{2,4,3,1,1},{3,4,2,1,1},{4,3,2,1,1},{2,3,4,1,1},{3,2,4,1,1},
						{1,2,4,3,1},{2,1,4,3,1},{4,1,2,3,1},{1,4,2,3,1},{2,4,1,3,1},{4,2,1,3,1},
						{4,3,1,2,1},{3,4,1,2,1},{1,4,3,2,1},{4,1,3,2,1},{3,1,4,2,1},{1,3,4,2,1},
						{1,3,2,4,1},{3,1,2,4,1},{2,1,3,4,1},{1,2,3,4,1},{3,2,1,4,1},{2,3,1,4,1},
						
						{1,3,1,4,2},{3,1,1,4,2},{1,1,3,4,2},{1,1,3,4,2},{3,1,1,4,2},{1,3,1,4,2},
						{4,3,1,1,2},{3,4,1,1,2},{1,4,3,1,2},{4,1,3,1,2},{3,1,4,1,2},{1,3,4,1,2},
						{1,1,4,3,2},{1,1,4,3,2},{4,1,1,3,2},{1,4,1,3,2},{1,4,1,3,2},{4,1,1,3,2},
						{4,1,3,1,2},{1,4,3,1,2},{3,4,1,1,2},{4,3,1,1,2},{1,3,4,1,2},{3,1,4,1,2},

						{2,1,4,1,3},{1,2,4,1,3},{4,2,1,1,3},{2,4,1,1,3},{1,4,2,1,3},{4,1,2,1,3},
						{1,1,2,4,3},{1,1,2,4,3},{2,1,1,4,3},{1,2,1,4,3},{1,2,1,4,3},{2,1,1,4,3},
						{2,4,1,1,3},{4,2,1,1,3},{1,2,4,1,3},{2,1,4,1,3},{4,1,2,1,3},{1,4,2,1,3},
						{1,4,1,2,3},{4,1,1,2,3},{1,1,4,2,3},{1,1,4,2,3},{4,1,1,2,3},{1,4,1,2,3},
						
						{3,4,1,2,1},{4,3,1,2,1},{1,3,4,2,1},{3,1,4,2,1},{4,1,3,2,1},{1,4,3,2,1},
						{2,4,3,1,1},{4,2,3,1,1},{3,2,4,1,1},{2,3,4,1,1},{4,3,2,1,1},{3,4,2,1,1},
						{3,1,2,4,1},{1,3,2,4,1},{2,3,1,4,1},{3,2,1,4,1},{1,2,3,4,1},{2,1,3,4,1},
						{2,1,4,3,1},{1,2,4,3,1},{4,2,1,3,1},{2,4,1,3,1},{1,4,2,3,1},{4,1,2,3,1}
						}
					}
				}
			},
			{
				{1,2,3,4,5},
				{
					{0,{{}}},
					{1,{{1},{2},{3},{4},{5}}},
					{2,{{1,2},{1,3},{1,4},{1,5}, { 2,3 },{2,4},{2,5}, { 3,4 },{3,5},{4,5}}},
					{3,{{1,2,3},{1,2,4},{1,2,5}, { 1,3,4 },{1,3,5}, {1,4,5}, { 2,3,4 },{2,3,5},{2,4,5},{3,4,5}}},
					{4,{{1,2,3,4},{1,2,3,5},{1,2,4,5},{1,3,4,5},{2,3,4,5}}},
					{5,{{1,2,3,4,5}}}
				},
				{
					{0,{{}}},
					{1,{{1},{2},{3},{4},{5}}},
					{2,{{1,2},{1,3},{1,4},{1,5}, { 2,3 },{2,4},{2,5}, { 3,4 },{3,5},{4,5}}},
					{3,{{1,2,3},{1,2,4},{1,2,5}, { 1,3,4 },{1,3,5}, {1,4,5}, { 2,3,4 },{2,3,5},{2,4,5},{3,4,5}}},
					{4,{{1,2,3,4},{1,2,3,5},{1,2,4,5},{1,3,4,5},{2,3,4,5}}},
					{5,{{1,2,3,4,5}}}
				},
				{
					{0,{{}}},
					{1,{{1},{2},{3},{4},{5}}},
					{2,{
						{1,2},{2,1},
						{1,3},{3,1},
						{1,4},{4,1},
						{1,5},{5,1},
						{2,3},{3,2},
						{2,4},{4,2},
						{2,5},{5,2},
						{3,4},{4,3},
						{3,5},{5,3},
						{4,5},{5,4},
						}
					},
					{3,{
						{1,2,3},{2,1,3},{3,1,2},{1,3,2},{2,3,1},{3,2,1},
						{1,2,4},{2,1,4},{4,1,2},{1,4,2},{2,4,1},{4,2,1},
						{1,2,5},{2,1,5},{5,1,2},{1,5,2},{2,5,1},{5,2,1},
						{1,3,4},{3,1,4},{4,1,3},{1,4,3},{3,4,1},{4,3,1},
						{1,3,5},{3,1,5},{5,1,3},{1,5,3},{3,5,1},{5,3,1},
						{1,4,5},{4,1,5},{5,1,4},{1,5,4},{4,5,1},{5,4,1},

						{2,3,4},{3,2,4},{4,2,3},{2,4,3},{3,4,2},{4,3,2},
						{2,3,5},{3,2,5},{5,2,3},{2,5,3},{3,5,2},{5,3,2},
						{2,4,5},{4,2,5},{5,2,4},{2,5,4},{4,5,2},{5,4,2},

						{3,4,5},{4,3,5},{5,3,4},{3,5,4},{4,5,3},{5,4,3},
						}
					},
					{4,{

						{1,2,3,4},{2,1,3,4},{3,1,2,4},{1,3,2,4},{2,3,1,4},{3,2,1,4},
						{4,2,1,3},{2,4,1,3},{1,4,2,3},{4,1,2,3},{2,1,4,3},{1,2,4,3},
						{1,3,4,2},{3,1,4,2},{4,1,3,2},{1,4,3,2},{3,4,1,2},{4,3,1,2},
						{4,3,2,1},{3,4,2,1},{2,4,3,1},{4,2,3,1},{3,2,4,1},{2,3,4,1},


						{1,2,3,5},{2,1,3,5},{3,1,2,5},{1,3,2,5},{2,3,1,5},{3,2,1,5},
						{5,2,1,3},{2,5,1,3},{1,5,2,3},{5,1,2,3},{2,1,5,3},{1,2,5,3},
						{1,3,5,2},{3,1,5,2},{5,1,3,2},{1,5,3,2},{3,5,1,2},{5,3,1,2},
						{5,3,2,1},{3,5,2,1},{2,5,3,1},{5,2,3,1},{3,2,5,1},{2,3,5,1},

						{1,2,4,5},{2,1,4,5},{4,1,2,5},{1,4,2,5},{2,4,1,5},{4,2,1,5},
						{5,2,1,4},{2,5,1,4},{1,5,2,4},{5,1,2,4},{2,1,5,4},{1,2,5,4},
						{1,4,5,2},{4,1,5,2},{5,1,4,2},{1,5,4,2},{4,5,1,2},{5,4,1,2},
						{5,4,2,1},{4,5,2,1},{2,5,4,1},{5,2,4,1},{4,2,5,1},{2,4,5,1},

						{1,3,4,5},{3,1,4,5},{4,1,3,5},{1,4,3,5},{3,4,1,5},{4,3,1,5},
						{5,3,1,4},{3,5,1,4},{1,5,3,4},{5,1,3,4},{3,1,5,4},{1,3,5,4},
						{1,4,5,3},{4,1,5,3},{5,1,4,3},{1,5,4,3},{4,5,1,3},{5,4,1,3},
						{5,4,3,1},{4,5,3,1},{3,5,4,1},{5,3,4,1},{4,3,5,1},{3,4,5,1},

						{2,3,4,5},{3,2,4,5},{4,2,3,5},{2,4,3,5},{3,4,2,5},{4,3,2,5},
						{5,3,2,4},{3,5,2,4},{2,5,3,4},{5,2,3,4},{3,2,5,4},{2,3,5,4},
						{2,4,5,3},{4,2,5,3},{5,2,4,3},{2,5,4,3},{4,5,2,3},{5,4,2,3},
						{5,4,3,2},{4,5,3,2},{3,5,4,2},{5,3,4,2},{4,3,5,2},{3,4,5,2},
						},
					},
					{5,{
						{1,2,3,4,5},{2,1,3,4,5},{3,1,2,4,5},{1,3,2,4,5},{2,3,1,4,5},{3,2,1,4,5},
						{4,2,1,3,5},{2,4,1,3,5},{1,4,2,3,5},{4,1,2,3,5},{2,1,4,3,5},{1,2,4,3,5},
						{1,3,4,2,5},{3,1,4,2,5},{4,1,3,2,5},{1,4,3,2,5},{3,4,1,2,5},{4,3,1,2,5},
						{4,3,2,1,5},{3,4,2,1,5},{2,4,3,1,5},{4,2,3,1,5},{3,2,4,1,5},{2,3,4,1,5},

						{5,3,4,1,2},{3,5,4,1,2},{4,5,3,1,2},{5,4,3,1,2},{3,4,5,1,2},{4,3,5,1,2},
						{1,3,5,4,2},{3,1,5,4,2},{5,1,3,4,2},{1,5,3,4,2},{3,5,1,4,2},{5,3,1,4,2},
						{5,4,1,3,2},{4,5,1,3,2},{1,5,4,3,2},{5,1,4,3,2},{4,1,5,3,2},{1,4,5,3,2},
						{1,4,3,5,2},{4,1,3,5,2},{3,1,4,5,2},{1,3,4,5,2},{4,3,1,5,2},{3,4,1,5,2},

						{2,4,1,5,3},{4,2,1,5,3},{1,2,4,5,3},{2,1,4,5,3},{4,1,2,5,3},{1,4,2,5,3},
						{5,4,2,1,3},{4,5,2,1,3},{2,5,4,1,3},{5,2,4,1,3},{4,2,5,1,3},{2,4,5,1,3},
						{2,1,5,4,3},{1,2,5,4,3},{5,2,1,4,3},{2,5,1,4,3},{1,5,2,4,3},{5,1,2,4,3},
						{5,1,4,2,3},{1,5,4,2,3},{4,5,1,2,3},{5,4,1,2,3},{1,4,5,2,3},{4,1,5,2,3},

						{3,1,5,2,4},{1,3,5,2,4},{5,3,1,2,4},{3,5,1,2,4},{1,5,3,2,4},{5,1,3,2,4},
						{2,1,3,5,4},{1,2,3,5,4},{3,2,1,5,4},{2,3,1,5,4},{1,3,2,5,4},{3,1,2,5,4},
						{3,5,2,1,4},{5,3,2,1,4},{2,3,5,1,4},{3,2,5,1,4},{5,2,3,1,4},{2,5,3,1,4},
						{2,5,1,3,4},{5,2,1,3,4},{1,2,5,3,4},{2,1,5,3,4},{5,1,2,3,4},{1,5,2,3,4},

						{4,5,2,3,1},{5,4,2,3,1},{2,4,5,3,1},{4,2,5,3,1},{5,2,4,3,1},{2,5,4,3,1},
						{3,5,4,2,1},{5,3,4,2,1},{4,3,5,2,1},{3,4,5,2,1},{5,4,3,2,1},{4,5,3,2,1},
						{4,2,3,5,1},{2,4,3,5,1},{3,4,2,5,1},{4,3,2,5,1},{2,3,4,5,1},{3,2,4,5,1},
						{3,2,5,4,1},{2,3,5,4,1},{5,3,2,4,1},{3,5,2,4,1},{2,5,3,4,1},{5,2,3,4,1}
						}
					}
				},
				{
					{0,{{}}},
					{1,{{1},{2},{3},{4},{5}}},
					{2,{
						{1,2},{2,1},
						{1,3},{3,1},
						{1,4},{4,1},
						{1,5},{5,1},
						{2,3},{3,2},
						{2,4},{4,2},
						{2,5},{5,2},
						{3,4},{4,3},
						{3,5},{5,3},
						{4,5},{5,4},
						}
					},
					{3,{
						{1,2,3},{2,1,3},{3,1,2},{1,3,2},{2,3,1},{3,2,1},
						{1,2,4},{2,1,4},{4,1,2},{1,4,2},{2,4,1},{4,2,1},
						{1,2,5},{2,1,5},{5,1,2},{1,5,2},{2,5,1},{5,2,1},
						{1,3,4},{3,1,4},{4,1,3},{1,4,3},{3,4,1},{4,3,1},
						{1,3,5},{3,1,5},{5,1,3},{1,5,3},{3,5,1},{5,3,1},
						{1,4,5},{4,1,5},{5,1,4},{1,5,4},{4,5,1},{5,4,1},

						{2,3,4},{3,2,4},{4,2,3},{2,4,3},{3,4,2},{4,3,2},
						{2,3,5},{3,2,5},{5,2,3},{2,5,3},{3,5,2},{5,3,2},
						{2,4,5},{4,2,5},{5,2,4},{2,5,4},{4,5,2},{5,4,2},

						{3,4,5},{4,3,5},{5,3,4},{3,5,4},{4,5,3},{5,4,3},
						}
					},
					{4,{

						{1,2,3,4},{2,1,3,4},{3,1,2,4},{1,3,2,4},{2,3,1,4},{3,2,1,4},
						{4,2,1,3},{2,4,1,3},{1,4,2,3},{4,1,2,3},{2,1,4,3},{1,2,4,3},
						{1,3,4,2},{3,1,4,2},{4,1,3,2},{1,4,3,2},{3,4,1,2},{4,3,1,2},
						{4,3,2,1},{3,4,2,1},{2,4,3,1},{4,2,3,1},{3,2,4,1},{2,3,4,1},


						{1,2,3,5},{2,1,3,5},{3,1,2,5},{1,3,2,5},{2,3,1,5},{3,2,1,5},
						{5,2,1,3},{2,5,1,3},{1,5,2,3},{5,1,2,3},{2,1,5,3},{1,2,5,3},
						{1,3,5,2},{3,1,5,2},{5,1,3,2},{1,5,3,2},{3,5,1,2},{5,3,1,2},
						{5,3,2,1},{3,5,2,1},{2,5,3,1},{5,2,3,1},{3,2,5,1},{2,3,5,1},

						{1,2,4,5},{2,1,4,5},{4,1,2,5},{1,4,2,5},{2,4,1,5},{4,2,1,5},
						{5,2,1,4},{2,5,1,4},{1,5,2,4},{5,1,2,4},{2,1,5,4},{1,2,5,4},
						{1,4,5,2},{4,1,5,2},{5,1,4,2},{1,5,4,2},{4,5,1,2},{5,4,1,2},
						{5,4,2,1},{4,5,2,1},{2,5,4,1},{5,2,4,1},{4,2,5,1},{2,4,5,1},

						{1,3,4,5},{3,1,4,5},{4,1,3,5},{1,4,3,5},{3,4,1,5},{4,3,1,5},
						{5,3,1,4},{3,5,1,4},{1,5,3,4},{5,1,3,4},{3,1,5,4},{1,3,5,4},
						{1,4,5,3},{4,1,5,3},{5,1,4,3},{1,5,4,3},{4,5,1,3},{5,4,1,3},
						{5,4,3,1},{4,5,3,1},{3,5,4,1},{5,3,4,1},{4,3,5,1},{3,4,5,1},

						{2,3,4,5},{3,2,4,5},{4,2,3,5},{2,4,3,5},{3,4,2,5},{4,3,2,5},
						{5,3,2,4},{3,5,2,4},{2,5,3,4},{5,2,3,4},{3,2,5,4},{2,3,5,4},
						{2,4,5,3},{4,2,5,3},{5,2,4,3},{2,5,4,3},{4,5,2,3},{5,4,2,3},
						{5,4,3,2},{4,5,3,2},{3,5,4,2},{5,3,4,2},{4,3,5,2},{3,4,5,2},
						},
					},
					{5,{
						{1,2,3,4,5},{2,1,3,4,5},{3,1,2,4,5},{1,3,2,4,5},{2,3,1,4,5},{3,2,1,4,5},
						{4,2,1,3,5},{2,4,1,3,5},{1,4,2,3,5},{4,1,2,3,5},{2,1,4,3,5},{1,2,4,3,5},
						{1,3,4,2,5},{3,1,4,2,5},{4,1,3,2,5},{1,4,3,2,5},{3,4,1,2,5},{4,3,1,2,5},
						{4,3,2,1,5},{3,4,2,1,5},{2,4,3,1,5},{4,2,3,1,5},{3,2,4,1,5},{2,3,4,1,5},

						{5,3,4,1,2},{3,5,4,1,2},{4,5,3,1,2},{5,4,3,1,2},{3,4,5,1,2},{4,3,5,1,2},
						{1,3,5,4,2},{3,1,5,4,2},{5,1,3,4,2},{1,5,3,4,2},{3,5,1,4,2},{5,3,1,4,2},
						{5,4,1,3,2},{4,5,1,3,2},{1,5,4,3,2},{5,1,4,3,2},{4,1,5,3,2},{1,4,5,3,2},
						{1,4,3,5,2},{4,1,3,5,2},{3,1,4,5,2},{1,3,4,5,2},{4,3,1,5,2},{3,4,1,5,2},

						{2,4,1,5,3},{4,2,1,5,3},{1,2,4,5,3},{2,1,4,5,3},{4,1,2,5,3},{1,4,2,5,3},
						{5,4,2,1,3},{4,5,2,1,3},{2,5,4,1,3},{5,2,4,1,3},{4,2,5,1,3},{2,4,5,1,3},
						{2,1,5,4,3},{1,2,5,4,3},{5,2,1,4,3},{2,5,1,4,3},{1,5,2,4,3},{5,1,2,4,3},
						{5,1,4,2,3},{1,5,4,2,3},{4,5,1,2,3},{5,4,1,2,3},{1,4,5,2,3},{4,1,5,2,3},

						{3,1,5,2,4},{1,3,5,2,4},{5,3,1,2,4},{3,5,1,2,4},{1,5,3,2,4},{5,1,3,2,4},
						{2,1,3,5,4},{1,2,3,5,4},{3,2,1,5,4},{2,3,1,5,4},{1,3,2,5,4},{3,1,2,5,4},
						{3,5,2,1,4},{5,3,2,1,4},{2,3,5,1,4},{3,2,5,1,4},{5,2,3,1,4},{2,5,3,1,4},
						{2,5,1,3,4},{5,2,1,3,4},{1,2,5,3,4},{2,1,5,3,4},{5,1,2,3,4},{1,5,2,3,4},

						{4,5,2,3,1},{5,4,2,3,1},{2,4,5,3,1},{4,2,5,3,1},{5,2,4,3,1},{2,5,4,3,1},
						{3,5,4,2,1},{5,3,4,2,1},{4,3,5,2,1},{3,4,5,2,1},{5,4,3,2,1},{4,5,3,2,1},
						{4,2,3,5,1},{2,4,3,5,1},{3,4,2,5,1},{4,3,2,5,1},{2,3,4,5,1},{3,2,4,5,1},
						{3,2,5,4,1},{2,3,5,4,1},{5,3,2,4,1},{3,5,2,4,1},{2,5,3,4,1},{5,2,3,4,1}
						}
					}
				},
			}
		};
		size_t _l_idx{ 0 };
		for (auto&& [_l_r, _l_no_repeats_or_perms, _l_repeats_no_perms,
			_l_perms_no_repeats,_l_repeats_and_perms] : _l_tests)
		{
			INFO(format("_l_idx = {0}", _l_idx));
			std::cout << format("_l_idx = {0}", _l_idx) << std::endl;
			combinatorics_tests_2<R, T, false, false>(_l_r, _l_no_repeats_or_perms);
			combinatorics_tests_2<R, T, true, false>(_l_r, _l_repeats_no_perms);
			combinatorics_tests_2<R, T, false, true>(_l_r, _l_perms_no_repeats);
			combinatorics_tests_2<R, T, true, true>(_l_r, _l_repeats_and_perms);
			_l_idx++;
		}
	}

	void print( std::vector<int>& a)
	{
		std::cout << "a = [";
		for (auto& _l_c : a)
		{
			std::cout << _l_c << ",";
		}
		std::cout << "]" << std::endl;
	}

	void print(std::vector<size_t>& is, std::vector<size_t>& ks, std::vector<int>& a, size_t _l_idx)
	{
		std::cout << "_l_idx = " << _l_idx;
		/*for (auto& _l_c : is)
		{
			std::cout << _l_c << ",";
		}
		std::cout << "]";
		std::cout << ", ks = [";
		for (auto& _l_c : ks)
		{
			std::cout << _l_c << ",";
		}
		std::cout << "]";*/
		std::cout << ", a = [";
		for (auto& _l_c : a)
		{
			std::cout << _l_c << ",";
		}
		std::cout << "]" << std::endl;
	}
	std::vector<std::vector<int>> in_place_arg_backwards(std::vector<int>& a)
	{
		std::vector<std::vector<int>> _l_rt;
		auto n = a.size();
		size_t _l_idx = 1;
		std::vector<size_t> c(n);
		std::vector<size_t> is = { 1 };
	//	print(is, c, a, _l_idx++);
		_l_rt.push_back(a);
		if (n > 0)
		{
			for (size_t i = 0; i < n; ++i)
			{
				c[i] = i;
			}
			c[0] = 100;
			size_t i = 1;
			while (i < n)
			{
				auto& ci = c[i];
		//		std::cout << "State before loop itt. i = " << i << std::endl;
			//	for (size_t i = 0; i < n; i++)
			//	{
			//		std::cout << "c[" << i << "] = " << c[i] << std::endl;
			//	}
				if (c[i] > 0)
				{
					if ((i % 2) == 0)
					{
				//		std::cout << "Even: Swap a[0] and a[" << i << "]" << std::endl;
						std::swap(a[0], a[i]);
					}
					else
					{
				//		std::cout << "Odd: Swap a[" << (c[i] - 1) << "] and a[" << i << "]" << std::endl;
						std::swap(a[c[i] - 1], a[i]);
					}
				//	print(is, c, a, _l_idx++);
					_l_rt.push_back(a);
					c[i]--;
					i = 1;
					is.pop_back();
					is.push_back(i);
				}
				else
				{
					c[i] = i;
					i++;
					is.pop_back();
					is.push_back(i);
				}
			}
		//	std::cout << "Final state: " << std::endl;
		//	for (size_t i = 0; i < n; i++)
		//	{
		//		std::cout << "c[" << i << "] = " << c[i] << std::endl;
		//	}
		}
		return _l_rt;
	}

	std::vector<std::vector<int>> in_place_alg(std::vector<int>& a)
	{
		std::vector<std::vector<int>> _l_rt;
		auto n = a.size();
		size_t _l_idx = 1;
		std::vector<size_t> c(n);
		std::vector<size_t> is = { 1 };
		print(is, c, a, _l_idx++);
		_l_rt.push_back(a);
		if (n > 0)
		{
			for (size_t i = 0; i < n; ++i)
			{
				c[i] = 0;
			}
			c[0] = 100;
			size_t i = 1;
			while (i < n)
			{
				auto& ci = c[i];
			//	std::cout << "State before loop itt. i = " << i << std::endl;
			//	for (size_t i = 0; i < n; i++)
			//	{
			//		std::cout << "c[" << i << "] = " << c[i] << std::endl;
			//	}
				if (c[i] < i)
				{
					if ((i % 2) == 0)
					{
				//		std::cout << "Even: Swap a[0] and a[" << i << "]" << std::endl;
						std::swap(a[0], a[i]);
					}
					else
					{
				//		std::cout << "Odd: Swap a[" << c[i] << "] and a[" << i << "]" << std::endl;
						std::swap(a[c[i]], a[i]);
					}
			//		print(is, c, a, _l_idx++);
					_l_rt.push_back(a);
					c[i]++;
					i = 1;
					is.pop_back();
					is.push_back(i);
				}
				else
				{
					c[i] = 0;
					i++;
					is.pop_back();
					is.push_back(i);
				}
			}
		//	std::cout << "Final state: " << std::endl;
		//	for (size_t i = 0; i < n; i++)
		//	{
		//		std::cout << "c[" << i << "] = " << c[i] << std::endl;
		//	}
		}
		return _l_rt;
	}

	std::vector<std::vector<int>> recursive(std::vector<size_t> is, std::vector<size_t> ks, std::vector<int>& a, size_t& _l_idx)
	{
		std::vector<std::vector<int>> _l_rt;
		auto k = ks[ks.size() - 1];
	//	std::cout << "k = " << k << std::endl;
		if (k <= 1)
		{
	//		print(is, ks, a, _l_idx++);
			_l_rt.push_back(a);
		}
		else
		{
			auto knext = ks;
			knext.push_back(k - 1);
			auto isnext = is;
			isnext.push_back(0);
			auto _l_rt1 = recursive(isnext, knext, a, _l_idx);
			_l_rt.insert(_l_rt.end(), _l_rt1.begin(), _l_rt1.end());
		//	std::cout << "k = " << k << std::endl;
			for (size_t i = 1; i < k; ++i)
			{
		//		std::cout << "i = " << (i - 1) << std::endl;
				if ((k % 2) == 0)
				{
		//			std::cout << "Even: Swap a[" << (i-1) << "] and a[" << (k-1) << "]" << std::endl;
					std::swap(a[i-1], a[k - 1]);
				}
				else
				{
		//			std::cout << "Odd: Swap a[" << 0 << "] and a[" << (k - 1) << "]" << std::endl;
					std::swap(a[0], a[k - 1]);
				}
				knext = ks;
				knext.push_back(k - 1);
				isnext = is;
				isnext.push_back(i);
				_l_rt1 = recursive(isnext, knext, a, _l_idx);
				_l_rt.insert(_l_rt.end(), _l_rt1.begin(), _l_rt1.end());
			//	std::cout << "k = " << k << std::endl;
			}
		}
		return _l_rt;
	}
}

TEST_CASE("Generic combinatorics tests for all constructors", "ab::test")
{
	using namespace test;
	using namespace std;
	using namespace ab::test::utility;
	/*combinatorics<size_t, true, true> _l_ex1;
	vector<size_t> _l_vect;
	for (size_t _l_idx{0}; _l_idx < 0; _l_idx++)
	{
		_l_ex1.min_number_of_elements_to_generate = _l_idx;
		_l_vect.push_back(_l_idx-1);
		_l_ex1.elements = _l_vect;
		_l_ex1.max_number_of_elements_to_generate = _l_idx;
		vector<size_t> _l_last;
		for (auto&& _l_comb : _l_ex1)
		{
			_l_last = _l_comb;
		}
		std::cout << "[";
		for (auto&& _l_element : _l_last)
		{
			std::cout << _l_element << ", ";
		}
		std::cout << "]" << std::endl;
		vector<size_t> _l_check;
		if ((_l_idx % 2) == 0)
		{
			//Its even. The numbers are in the following order:
			//n-2,n-1,2 .. n-3,n,1.
			if (_l_idx > 0)
			{
				if (_l_idx > 2)
				{
					_l_check.push_back(_l_idx - 3);
					_l_check.push_back(_l_idx - 2);
					for (size_t _l_jdx{ 2 }; _l_jdx <= _l_idx - 3; _l_jdx++)
					{
						_l_check.push_back(_l_jdx-1);
					}
				}
				_l_check.push_back(_l_idx-1);
				_l_check.push_back(0);
			}
		}
		else
		{
			//Its odd. The numbers are in the following order:
			//n,2 .. n-1,1.
			_l_check.push_back(_l_idx-1);
			if (_l_idx > 1)
			{
				for (size_t _l_jdx{ 2 }; _l_jdx <= _l_idx - 1; _l_jdx++)
				{
					_l_check.push_back(_l_jdx-1);
				}
				_l_check.push_back(1-1);
			}
		}
		REQUIRE(_l_check == _l_last);
	}*/
	/*combinatorics<int, true, true> _l_combx;
	_l_combx.elements = { 4,3,2,1 };
	_l_combx.min_number_of_elements_to_generate = 4;
	_l_combx.max_number_of_elements_to_generate = 4;
	for (auto& _l_element : _l_combx)
	{
		std::cout << "[";
		for (auto& _l_c : _l_element)
		{
			std::cout << _l_c << ",";
		}
		std::cout << "]" << std::endl;
	}*/
	combinatorics_tests_1<vector<int>, int>();
	/*size_t high = 50;
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	for (size_t _l_idx = 1; _l_idx < high; _l_idx++)
	{
		std::cout << "begin i = " << _l_idx << std::endl;
		vector<int> _l_start;
		for (size_t _l_jdx = 1; _l_jdx < _l_idx; _l_jdx++)
		{
			_l_start.push_back(_l_jdx);
		}
		vector<int> _l_vect = _l_start;
		size_t idx = 1;
		auto t1 = high_resolution_clock::now();
	//	auto _l_results1 = recursive({}, { _l_vect.size() }, _l_vect, idx);
		auto t2 = high_resolution_clock::now();
		auto ms_int = duration_cast<milliseconds>(t2 - t1);
		duration<double, std::milli> ms_double = t2 - t1;

		std::cout << "recursive " << ms_int.count() << "ms\n";
		std::cout << ms_double.count() << "ms\n";
		std::cout << "====" << std::endl;
		_l_vect = _l_start;
		t1 = high_resolution_clock::now();
		auto _l_results2 = in_place_alg(_l_vect);
		t2 = high_resolution_clock::now();

		ms_int = duration_cast<milliseconds>(t2 - t1);

		ms_double = t2 - t1;

		std::cout << "In-place " << ms_int.count() << "ms\n";
		std::cout << ms_double.count() << "ms\n";

	//	CHECK(_l_results1 == _l_results2);
	//	std::ranges::reverse(_l_results2);
		auto arg = _l_results2[_l_results2.size() - 1];
		std::cout << "====" << std::endl;
		t1 = high_resolution_clock::now();
		auto _l_results3 = in_place_arg_backwards(arg);
		t2 = high_resolution_clock::now();

		ms_int = duration_cast<milliseconds>(t2 - t1);

		ms_double = t2 - t1;

		std::cout << "Reverse " << ms_int.count() << "ms\n";
		std::cout << ms_double.count() << "ms\n";

	//	CHECK(_l_results3 == _l_results2);
		std::cout << "total size = " << _l_results3.size() << std::endl;
		std::cout << "end i = " << _l_idx << std::endl;
	}*/
}

TEST_CASE("Combination tests for default constructor", "ab::test")
{
	/*using namespace ab::test::utility;
	using namespace std;
	combinatorics<int, true, true> _l_comb;
	vector<int> _l_new_container;
	CHECK(_l_comb.elements == _l_new_container);
	CHECK(_l_comb.max_number_of_elements_to_generate == 0);
	CHECK(_l_comb.min_number_of_elements_to_generate == 0);
	CHECK(_l_comb.has_at_least_one_result() == true);
	vector<combinatorics_iterator<int, true, true>::return_type> _l_results;
	for (auto&& _l_combination : _l_comb)
	{
		_l_results.push_back(_l_combination);
	}
	vector<combinatorics_iterator<int, true, true>::return_type> _l_single_vector = { {} };
	CHECK(_l_results == _l_single_vector);*/
}