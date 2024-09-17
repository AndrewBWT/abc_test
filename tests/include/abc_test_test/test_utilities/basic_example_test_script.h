#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/core/test_main.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/matchers/comparison.h"
#include "abc_test/core/test_assertions.h"

#include <functional>
#include <initializer_list>

#include "abc_test/gen_data/collection.h"
#include "abc_test/gen_data/static_data.h"
#include "abc_test/gen_data/file.h"
#include "abc_test/gen_data/random_data.h"

#include "abc_test/utility/str.h"
#include "abc_test/core/reporters/text_test_reporter.h"
#include "abc_test/core/log_test_msg.h"
#include "abc_test/gen_data/collection_iterator.h"

#include "abc_test/gen_data/manual_for_loop.h"

#include "abc_test/gen_data/enumerable/specializations.h"

#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/gen_data/enumerable/specializations/array.h"
#include "abc_test/gen_data/enumerable/specializations/vector.h"
#include "abc_test/gen_data/random/specializations/fundamental_types.h"

namespace abi
{
	struct special_bool_t : abc::random_generator_t<bool>
	{

	};
	inline
		bool
		gen2(
			abc::utility::rng& _a_random
		)
	{
		return true;
	}
	std::tuple<int, abc::utility::str_t>
		gen(
			abc::utility::rng& _a_random
		)
	{
		std::tuple<int, abc::utility::str_t> _l_rv;
		int i = _a_random();
		get<0>(_l_rv) = _a_random.calls();
		std::cout << _a_random.calls() << std::endl;
		get<1>(_l_rv) = abc::utility::str_t(std::to_string(_a_random.calls() + 1));
		return _l_rv;
	}
}

_TEST_CASE("Basic numerical tests", "abcs::numeric")
{
	using namespace abc;
	_CHECK(neq(1, 2));
	int x = 1348567;
//	auto _l_1 = cmp::neq(1, 2);
//	auto _l_n = _l_1 && _l_1;
	_TLOG("hello info");
	_TLOG_("hello special info");
	_TVLOG(x);
	//_TVAR(x);
	_CHECK_EXPR(1 == 2);
	_CHECK(neq(1, 2));
	_CHECK(lt(3, 2));
	_CHECK(EXPR(1 == 2));
	_CHECK(EXPR(3 == 2));
	_CHECK(EXPR(1 == 3));
	_CHECK(EXPR(1 == 2));
	_CHECK(EXPR(3 == 2));
	_CHECK(EXPR(1 == 3));
	_CHECK(EXPR(1 == 2));
	_CHECK(EXPR(3 == 2));
	_CHECK(EXPR(1 == 3));
	//_REQUIRE(EXPR(1 == 2));
	_CHECK(EXPR(1 == 3));
	throw std::runtime_error("hello runtime");//throw int();// throw std::exception("hello");
}

_TEST_CASE("Basic numerical tests3", "abcs::numeric")
{
	using namespace abc;
	_CHECK(EXPR(1 == 1));
}

_TEST_CASE("Basic numerical tests2", "abcs::numeric")
{
	using namespace abc;
	_CHECK(neq(1, 2));
	_CHECK(neq(1, 2));
	_CHECK(lt(3, 2));
	_CHECK(EXPR(1 == 2));
	_CHECK(EXPR(3 == 2));
}

_TEST_CASE("Static generator", "abcs::static")
{
	using namespace abc;
	using namespace std;
	using namespace abc::utility::io;
	using namespace abc::utility;
	using namespace abc::utility::str;
	_CHECK_EXPR("3" == to_string(2));
	using unit_test = tuple<int, str_t>;
	using btest = bool;
	str_t _l_x = "";
	initializer_list<unit_test> _l_inits = {
			{ 1, "1" },
			{ 2,"2" },
		{3, "2"}
	};
	initializer_list<unit_test> _l_inits2 = {
		{1,"2"},
	{43, "42"}
	};
	vector<unit_test> _l_vect;
	std::function<unit_test(abc::test_runner_t&)> _l_rand_func;
//	std::vector<std::unique_ptr<gen_data<unit_test>>> _l_lsts;
	//_REQUIRE(EXPR(1 == 2));
	std::function<bool(utility::rng&)> _l_f1 = [](auto&) {
		return true;
		};
	parser_fp_t<bool> _l_parser = nullptr;
	printer_fo_t<bool> _l_printer;
	for (auto&& _l_bool : gen_data_collection_t<bool>(
	//	static_data_t<unit_test>(_l_inits2),
	//	static_data(_l_inits2),
	//	file_data<unit_test>(file_name("file_data3"), _l_inits)
		//random_data(rnd_func(abi::gen), tempalted_file_rw_info<unit_test>("file_data_4"))
		random_data<bool>()
		//, random_data<bool>(using_function(abi::gen2))
		//, random_data<bool>(using_function(_l_f1))
		//, random_data<bool>(new abi::special_bool_t())
		//, random_data<bool>("hello")
		//, random_data<bool>(new abi::special_bool_t(),file_name_t("hello2"))
		//, random_data<bool>(new abi::special_bool_t(), rw_info_t<bool>(_l_parser))
		//, random_data<bool>(new abi::special_bool_t(), rw_info_t<bool>(_l_printer))
		//, random_data<bool>(new abi::special_bool_t(), rw_info_t<bool>(_l_parser,_l_printer))
		//, random_data<bool>(new bool_random_data())
		//, random_data<bool>(new bool_random_data(), file_name_t("hello2")) //Same as above but hello2
		//, random_data<bool>(file_name_t("hello","hello\goodbye")) //Will read/write using hello\goodbye\hello.txt
		//, random_data<bool>(file_name_t("hello.txt","hello\goodbye")) //Using hello\goodbye\hello.txt
		//, random_data<bool>(file_name_t("hello"),rw_info_t<bool>())
	))
	{
		std::cout << fmt::format("{0}", _l_bool) << std::endl;
		//_TVLOG(_l_int);
		//_TVLOG(_l_string);
		//_CHECK(EXPR(std::to_string(_l_int) == _l_string._m_str));//5 which fail. 6 total.
	}
	std::exit(-1);
//	_REQUIRE(EXPR(1 == 2));
	for (auto&& [_l_int, _l_string] : gen_data_collection_t<unit_test>(
		iterate_over(_l_inits2),
		file_data<unit_test>(file_name_t("file_data3"), _l_inits)
	//	random_data(rnd_func(abi::gen)),
	//	random_data(rnd_func(abi::gen), tempalted_file_rw_info<unit_test>("file_data_4"))
	))
	{
		std::cout << _l_int << " xx==xx \"" << _l_string._m_str << "\"" << std::endl;
		_CHECK_EXPR(to_string(_l_int) == _l_string._m_str);//16 which fail. 10 from randoms. 1 from random file. 3 from file. 2 from static. 2 from file which pass. 22 total
		for (auto&& [_l_int1, _l_string1] : gen_data_collection_t<unit_test>(
			iterate_over(_l_inits)
		//	file_data<unit_test>(file_name("file_data3"), _l_inits),
		//	random_data(rnd_func(abi::gen)),
		//	random_data(rnd_func(abi::gen), file_name("file_data_4"))
		))
		{
			//if (_l_int == 3 && _l_string == "2")
		//	{
	//			throw std::runtime_error("hello");
		//	}
			std::cout << _l_int1 << " == \"" << _l_string1._m_str << "\"" << std::endl;
			_CHECK_EXPR(to_string(_l_int1) == _l_string1._m_str);//1 from static. called 18 times. 40 total.
		}
		std::cout << "twice" << std::endl;
	}
	for (auto&& [_l_int, _l_string] :
		//abc::gen_data_collection<unit_test>())
		gen_data_collection_t<unit_test>(
			iterate_over(_l_inits),
			file_data<unit_test>(file_name_t("file_data3"), _l_inits)
	//		random_data(rnd_func(abi::gen)),
	//		random_data(rnd_func(abi::gen), tempalted_file_rw_info<unit_test>("file_data_4"))
		))
	{
		std::cout << _l_int << " == \"" << _l_string._m_str << "\"" << std::endl;
		_CHECK_EXPR(to_string(_l_int) == _l_string._m_str);// 10 from randoms. 1 from random file. 3 from file. 1 from static. 15 total. takes us to 55
	}
}

_TEST_CASE("Basic failure", "abcs::numeric")
{
	using namespace abc;
	_FAIL("helo");
	_FAIL("");
}

_TEST_CASE("Static generator", "abcs::static")
{
	using namespace abc;
	using namespace std;
	using namespace abc::utility;
	using namespace abc::utility::str;
	_CHECK_EXPR("3" == to_string(2));//1
	using unit_test = tuple<int, str_t>;
	str_t _l_x = "";
	initializer_list<unit_test> _l_inits = {
			{ 1, "1" },
			{ 2,"2" },
		{3, "2"}
	};
	initializer_list<unit_test> _l_inits2 = {
		{1,"2"},
	{43, "42"}
	};
	vector<unit_test> _l_vect;
	std::function<unit_test(abc::test_runner_t&)> _l_rand_func;
	//	std::vector<std::unique_ptr<gen_data<unit_test>>> _l_lsts;
		//_REQUIRE(EXPR(1 == 2));
	for (auto&& [_l_int, _l_string] : gen_data_collection_t<unit_test>(
		//	static_data_t<unit_test>(_l_inits2),
		//	static_data(_l_inits2),
		//	file_data<unit_test>(file_name("file_data3"), _l_inits)
	//	random_data(rnd_func(abi::gen), tempalted_file_rw_info<unit_test>("file_data_4"))
	))
	{
		_TVLOG(_l_int);
		_TVLOG(_l_string);
		_CHECK(EXPR(std::to_string(_l_int) == _l_string._m_str));//5 which fail. 6 total.
	}
	//	_REQUIRE(EXPR(1 == 2));
	for (auto&& [_l_int, _l_string] : gen_data_collection_t<unit_test>(
	//	static_data(_l_inits2),
	//	file_data<unit_test>(file_name("file_data3"), _l_inits),
	//	random_data(rnd_func(abi::gen)),
	//	random_data(rnd_func(abi::gen), tempalted_file_rw_info<unit_test>("file_data_4"))
	))
	{
		std::cout << _l_int << " xx==xx \"" << _l_string._m_str << "\"" << std::endl;
		_CHECK_EXPR(to_string(_l_int) == _l_string._m_str);//16 which fail. 10 from randoms. 1 from random file. 3 from file. 2 from static. 2 from file which pass. 22 total
		for (auto&& [_l_int1, _l_string1] : gen_data_collection_t<unit_test>(
		//	static_data(_l_inits)
			//	file_data<unit_test>(file_name("file_data3"), _l_inits),
			//	random_data(rnd_func(abi::gen)),
			//	random_data(rnd_func(abi::gen), file_name("file_data_4"))
		))
		{
		//	if (_l_int == 3 && _l_string == "2")
		//	{
				//			throw std::runtime_error("hello");
		//	}
			std::cout << _l_int1 << " == \"" << _l_string1._m_str << "\"" << std::endl;
			_CHECK_EXPR(to_string(_l_int1) == _l_string1._m_str);//1 from static. called 18 times. 40 total.
		}
		std::cout << "twice" << std::endl;
	}
	for (auto&& [_l_int, _l_string] :
		//abc::gen_data_collection<unit_test>())
		gen_data_collection_t<unit_test>(
	//		static_data(_l_inits),
	//		file_data<unit_test>(file_name("file_data3"), _l_inits),
	//		random_data(rnd_func(abi::gen)),
	//		random_data(rnd_func(abi::gen), tempalted_file_rw_info<unit_test>("file_data_4"))
		))
	{
		std::cout << _l_int << " == \"" << _l_string._m_str << "\"" << std::endl;
		_CHECK_EXPR(to_string(_l_int) == _l_string._m_str);// 10 from randoms. 1 from random file. 3 from file. 1 from static. 15 total. takes us to 55
	}
}


namespace typed
{
	template<
		typename T,
		typename U
	>
	void mf3(
	) 
	{
		using namespace abc;
		std::cout << typeid(T).name() << " " <<
			typeid(U).name() << std::endl;
		T _l_one{};
		U _l_two{};
		_CHECK_EXPR(fmt::format("{0}",_l_one) == fmt::format("{0}",_l_two));
	}
	template<
		typename T
	>
	void mf1()
	{
		using namespace abc;
		manual_for_loop_t _l_mfl;
		RUN(_l_mfl,(mf3<T, int>()));
		RUN(_l_mfl, (mf3<T, char>()));
		RUN(_l_mfl, (mf3<T, bool>()));
		std::string _l_x, _l_y;
		_l_x = "hello";
		_TLOG(typeid(T).name());
		_l_x = typeid(T).name();
		if (_l_x == "int")
		{
			_l_y = "123";
		}
		else
		{
			_FAIL("hello");
		}
	}
	template<
		typename T
	>
	void mf()
	{
		std::cout << typeid(T).name() << std::endl;
	}
}

_TEST_CASE(
	"Types in the void",""
)
{
	using namespace abc;
	{
		using namespace typed;
		manual_for_loop_t _l_mfl;
		RUN(_l_mfl, mf1<int>());
		RUN(_l_mfl, mf1<char>());
		RUN(_l_mfl, mf1<bool>());
	}
}

/*_TEST_CASE(
	"Enumerable", "enumerable"
)
{
	using namespace abc;
	using unit_test = uint16_t;
	using gdc_t = gen_data_collection_t<unit_test>;
	for (auto&& _l_int : gdc_t(
	//	enumeration<unit_test>()
	//	enumeration<unit_test>(bounds<unit_test>(1,100)),
	//	enumeration<unit_test>(bounds< unit_test>()),
	////	enumeration<unit_test>(bounds_lower< unit_test>(1)),
	//	enumeration<unit_test>(bounds_upper< unit_test>(100))
		))
	{
		std::cout << _l_int << std::endl;
	}
}*/

namespace x2
{
	enum class X2 { A, B, C, D, F, G, I = 100, J = 7 };
}
template<
>
__constexpr_imp
abc::enum_list_t<x2::X2> abc::get_enum_list() noexcept
{
	using enum x2::X2;
	return { A,B,C,D,F,G,I,J };
}

//_BEGIN_ABC_NS
/*namespace abc
{
	inline namespace v0
	{
		namespace gd
		{
		//	enum class X2 { A, B, C, D, F, G, I = 100, J = 7 };
			template<>
			__constexpr_imp
				enum_list_t<x2::X2>
				get_enum_list(
				) noexcept
			{
				using enum x2::X2;
				return { A,B,C,D,F,G,I,J };
			}
		}
	}
}*/
//_END_ABC_NS
//_END_NS

template
<
>
struct fmt::formatter<x2::X2> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__constexpr_imp
		auto
		format(
			x2::X2 _a_rd,
			format_context& _a_cxt
		) const
		->format_context::iterator
	{
		//A, B, C, D, F, G, I = 100, J = 7 };
		using enum x2::X2;
		using namespace std;
		string _l_str{};
		switch (_a_rd)
		{
		case A:
			_l_str = "A";
			break;
		case B:
			_l_str = "B";
			break;
		case C:
			_l_str = "C";
			break;
		case D:
			_l_str = "D";
			break;
		case F:
			_l_str = "F";
			break;
		case G:
			_l_str = "G";
			break;
		case I:
			_l_str = "I";
			break;
		case J:
			_l_str = "J";
			break;
		}
		return formatter<string_view>::format(_l_str, _a_cxt);
	}
};

namespace typed
{
	template<
		typename T,
		bool T_Constructor_Allowed = true
	>
	void
		enumeration_test(
			bool _a_print_results,
			bool _a_run_default_expected,
			std::vector<T> _a_lower_values,
			std::vector<T> _a_upper_values,
			std::vector<std::pair<T, T>> _a_lower_and_upper_values,
			std::vector<std::pair<T, std::size_t>> _a_move_back_value_with_n_elements,
			std::vector<std::pair<T, std::size_t>> _a_move_forward_value_with_n_elements,
			std::vector<std::pair<T, T>> _a_lower_values_with_custom_difference,
			std::vector<std::pair<T, T>> _a_upper_values_with_custom_difference,
			std::vector<std::tuple<T, T, T>> _a_lower_and_upper_values_with_custom_difference,
			std::vector<std::tuple<T, std::size_t, T>> _a_move_back_value_with_n_elements_and_custom_difference,
			std::vector<std::tuple<T, std::size_t,T>> _a_move_forward_value_with_n_elements_and_custom_difference,
			std::vector<T> _a_generate_all_with_custom_difference
		) noexcept;
	template<
		typename T
	>
	void
		enumeration_test_2(
			bool _a_print_results,
			bool _a_run_default_expected,
			std::vector<T> _a_lower_values,
			std::vector<T> _a_upper_values,
			std::vector<std::pair<T, T>> _a_lower_and_upper_values,
			std::vector<std::pair<T, std::size_t>> _a_move_back_value_with_n_elements,
			std::vector<std::pair<T, std::size_t>> _a_move_forward_value_with_n_elements,
			std::vector<std::pair<T, std::size_t>> _a_lower_values_with_custom_difference,
			std::vector<std::pair<T, std::size_t>> _a_upper_values_with_custom_difference,
			std::vector<std::tuple<T, T, std::size_t>> _a_lower_and_upper_values_with_custom_difference,
			std::vector<std::tuple<T, std::size_t, std::size_t>> _a_move_back_value_with_n_elements_and_custom_difference,
			std::vector<std::tuple<T, std::size_t, std::size_t>> _a_move_forward_value_with_n_elements_and_custom_difference,
			std::vector<std::size_t> _a_generate_all_with_custom_difference
		) noexcept;
	void
		enumeration_test_3(
		);
	template<
		typename T
	>
	void
		enumerate_test_4(
			T _a_element
		);
	template<
		typename T
	>
	void
		enum_enumeration_test(
		) noexcept;
	void
		collections() noexcept;
}

_TEST_CASE(
	"Enumerable3", "enumerable"
)
{
	using namespace abc;
	using namespace typed;
	std::cout << std::boolalpha;
	bool _l_print_results = true;
	{
		manual_for_loop_t _l_mfl;
		//RUN(_l_mfl, (enumeration_test<bool, false>(_l_print_results, true,
		//	{ false }, { true }, { {false,true},{true,false} }, { {true, 100} }, { {false,100} }, {}, {}, {}, {}, {}, {})));
		//RUN(_l_mfl, (enumeration_test<std::array<bool,3>>(true, {}, { })));
		//RUN(_l_mfl, enumeration_test<signed char>(_l_print_results, true, { 124 }, { -120 }, { {10,-10},{100,125},{-74,-52} }, { {100,5},{-150,17} },
		//	{ {100,52},{74,14} }, { { 100,17 } }, { {100,7} }, { { 50, 100, 5 } }, {{ -125, 117, 37 }}, { {100,11,7} }, { 17 }));
		//RUN(_l_mfl, enumeration_test<unsigned char>(_l_print_results, true, {250}, {10}, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test<short int>(_l_print_results, false, {32757}, {-32755}, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test<unsigned short int>(_l_print_results, false, {65500}, {10}, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test<int>(_l_print_results, false, {std::numeric_limits<int>::max() - 10}, { std::numeric_limits<int>::min() + 10 }, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test<unsigned int>(_l_print_results, false, { std::numeric_limits<unsigned int>::max() - 10 }, {10}, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test<long int>(_l_print_results, false, { std::numeric_limits<long int>::max() - 10 }, { std::numeric_limits<long int>::min() + 10 }, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test<unsigned long int>(_l_print_results, false, { std::numeric_limits<unsigned long int>::max() - 10 }, {10}, {}, {}, {}, {}, {}, {}, {}, {}, {}));
	 	//RUN(_l_mfl, enumeration_test<long long int>(_l_print_results, false, { std::numeric_limits<long long int>::max() - 10 }, { std::numeric_limits<long long int>::min() + 10 }, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test<unsigned long long int>(_l_print_results, false, { std::numeric_limits<unsigned long long int>::max() - 10 }, {10}, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test_2<X2>(_l_print_results, true, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}));
		//RUN(_l_mfl, enumeration_test_3());
		//RUN(_l_mfl, (enumerate_test_4<signed char>()));
		//RUN(_l_mfl, (enumerate_test_4<std::array<bool, 3>>()));
		//RUN(_l_mfl, (enumerate_test_4<std::array<std::array<bool,2>, 3>>()));
		//RUN(_l_mfl, (enumerate_test_4<std::vector<X2>>({X2::A,X2::A,X2::F})));
		RUN(_l_mfl, (enumerate_test_4<std::vector<uint8_t>>({ 5,1,100 })));
		//RUN(_l_mfl, (enumerate_test_4<std::array<unsigned char, 2>>()));
	}
}

_TEST_CASE(
	"Enumerable2", "enumerable"
)
{
	using namespace abc;
	std::vector<bool> _l_bools;
	uint8_t _l_x{ 100 };
	std::string _l_s{ std::to_string(_l_x) };
	/*for (auto&& _l_int : gen_data_collection_t<bool>(
		static_data(_l_bools)
		, enumeration<bool>()
	))
	{
		std::cout << fmt::format("bool value = {}", _l_int) << std::endl;
	}
	std::vector<uint8_t> _l_inits;
	for (auto&& _l_int : gen_data_collection_t<uint8_t>(
		static_data(_l_inits)
		, enumeration<uint8_t>()
		, enumeration(using_lower_bound<uint8_t>(10))
		, enumeration(using_upper_bound<uint8_t>(100))
	))
	{
		std::cout << fmt::format("uint8_t value = {}", _l_int) << std::endl;
	}*/
	/*for (auto&& _l_int : gen_data_collection_t<std::vector<uint8_t>>(
		enumeration<std::vector<uint8_t>>(),
		enumeration< std::vector<uint8_t>>(lower_bound(10)),
		enumeration< std::vector<uint8_t>>(upper_bound({ 100 }))
	))
	{
		std::cout << fmt::format("{}", _l_int) << std::endl;
	}
	for (auto&& [_l_int1,_l_int2] : gen_data_collection_t<std::pair<uint8_t,bool>>(
		enumeration<std::pair<uint8_t, bool>>(),
		enumeration<std::pair<uint8_t, bool>>(lower_bound({ 10,true })),
		enumeration< std::pair<uint8_t, bool>>(upper_bound({ 100 ,false}))
	))
	{
		std::cout << fmt::format("{}", _l_int) << std::endl;
	}*/
}

TEST_CASE("Basic test script", "abc")
{
	using namespace abc;
	using namespace std;
	using namespace reporters;
	ds::test_list_t _l_tl = { 
		//Establish40, 
		//Establish68, 
		//Establish74, 
	//	Establish106, 
		//Establish151
	//	Establish168,
	//	Establish307,
	//	Establish510
	};
	test_options_t _l_to;
	//ds::repetition_tree_t _l_rt;
	//_l_rt.add_repetition({ repetition_data_t(1, 0, "0") });
	//_l_rt.add_repetition({ repetition_data_t(1, 0, "0"),repetition_data_t(0,1,"2")});
	//_l_rt.add_repetition({ repetition_data_t(2,0,"2") });
	//_l_rt.add_repetition({ repetition_data_t(2,0,"2") });
	//_l_rt.add_repetition({ repetition_data_t(48,5,"62m ") });
	//_l_to._m_test_repetition_configurations.insert({ 4,_l_rt });
	//_l_to._m_print_seeds_as_text = true;
	//_l_to._m_repeat_tests_string = "100:1:5b28302c5b28302c302c2230222c5b28302c5b28302c302c2231222c5b5d292c28302c302c2232222c5b5d295d295d292c28302c302c2231222c5b28302c5b28302c302c2230222c5b5d292c28302c302c2232222c5b5d292c28302c302c2233222c5b5d295d295d292c28302c302c2232222c5b28302c5b28302c302c2230222c5b5d292c28302c302c2231222c5b5d292c28302c302c2233222c5b5d295d295d295d295d";
	//_l_to._m_repeat_tests_string = "100:1:5b28302c5b28302c302c2231222c5b5d292c28302c302c2232222c5b5d295d295d";
	// 	//_l_to._m_repeat_tests_string = "100:1:5b28302c5b28302c302c2231222c5b5d292c28302c312c2231222c5b5d292c28302c312c2232222c5b5d292c28302c312c2233222c5b5d292c28302c312c2234222c5b5d292c28302c312c2235222c5b5d295d295d";
//	_l_to._m_repeat_tests_string = "100:1:5b28302c5b28302c302c2231222c5b5d292c28302c302c2232222c5b5d292c28302c302c2233222c5b5d292c28302c302c2234222c5b5d292c28302c302c2235222c5b5d295d295d";
	// 	// 	// 	// 	// 	// 	// 	//hello
	if (_l_to._m_repeat_tests_string == "")
	{
		_l_to._m_write_random_data_to_file = true;
	}
	else
	{
		_l_to._m_write_random_data_to_file = false;
	}
	//_l_to._m_test_data_root_path = "awheipvuch34r";
	std::cout << std::filesystem::current_path() << std::endl;
	_l_to._m_test_data_root_path = "..\\..\\test_data\\test_dir1";
	_l_to._m_create_test_folders_if_dont_exist = true;
	_l_to._m_internal_logging = true;
	test_main_t _l_test_main;
	_l_to._m_threads = 1;
	_l_test_main.set_options(_l_to);
	reporters::text_test_reporter_t _l_i;
	_l_test_main.add_test_reporter_owned_by_class(new text_test_reporter_t());
	_l_test_main.add_global_test_list();
	_l_test_main.run_tests();
}

namespace typed
{
	struct X3
	{

	};
}

namespace typed
{
	struct int_enumerate_t : public abc::enumerable_t<X3>
	{
		__constexpr_imp
			bool
			less_than(
				const X3&,
				const X3&
			) const noexcept
		{
			return true;
		}
		__constexpr_imp
			bool
			equal(
				const X3&,
				const X3&
			) const noexcept
		{
			return true;
		}
		__constexpr_imp
			int_enumerate_t(

			) noexcept
		{

		}
		__constexpr_imp
			int_enumerate_t(
				const int _a_difference_1,
				const int _a_difference_2
			) noexcept
		{

		}
		__constexpr_imp
			bool
			increment(
				X3& _a_element,
				std::size_t& _a_n_times_to_increment,
				const std::optional<X3>& _a_max_value
			)
		{
			return false;
		}
		__constexpr_imp
			bool
			decrement(
				X3& _a_element,
				std::size_t& _a_n_times_to_increment,
				const std::optional<X3>& _a_max_value
			)
		{
			return false;
		}
	};
	template<
		typename T
	>
	void
		run_comparison_enum_and_printer(
			std::function<void(const std::string&)> _a_print_f,
			bool _a_print_results,
			std::vector<abc::enumerate_t<T>>& _a_enumeration_tests
		)
	{
		using namespace abc;
		using namespace std;
		vector<vector<T>> _l_results;
		_l_results.push_back(vector<T>());
		size_t _l_idx{ 0 };
		for (auto& _l_element : _a_enumeration_tests)
		{
			_l_results.push_back(vector<T>());
			for (auto&& _l_int : gen_data_collection_t<T>(
				enumerate_data(_l_element)))
			{
				_l_results.back().push_back(_l_int);
				if (_a_print_results && _l_idx == 0)
				{
					_a_print_f(fmt::format("{0}, ", _l_int));
				}
			}
			++_l_idx;
		}
		_l_idx = 0;
		for (auto& _l_result : _l_results)
		{
			if (_l_idx == 0)
			{
				continue;
			}
			REQUIRE(_l_results[0] == _l_result);
		}
	}
}

namespace typed
{
	template<
		typename T,
		bool T_Constructor_Allowed
	>
	inline
		void
		enumeration_test(
			bool _a_print_results,
			bool _a_run_default_expected,
			std::vector<T> _a_lower_values,
			std::vector<T> _a_upper_values,
			std::vector<std::pair<T,T>> _a_lower_and_upper_values,
			std::vector<std::pair<T,std::size_t>> _a_move_back_value_with_n_elements,
			std::vector<std::pair<T,std::size_t>> _a_move_forward_value_with_n_elements,
			std::vector<std::pair<T,T>> _a_lower_values_with_custom_difference,
			std::vector<std::pair<T,T>> _a_upper_values_with_custom_difference,
			std::vector<std::tuple<T, T,T>> _a_lower_and_upper_values_with_custom_difference,
			std::vector<std::tuple<T, std::size_t,T>> _a_move_back_value_with_n_elements_and_custom_difference,
			std::vector<std::tuple<T, std::size_t,T>> _a_move_forward_value_with_n_elements_and_custom_difference,
			std::vector<T> _a_generate_all_with_custom_difference
		) noexcept
	{
		using namespace abc;
		using namespace std;
		using namespace abc::gd;
		vector<T> _l_inits;
		function<void(const string&)> _l_print_f = [&](const string& _a_str)
			{
				if (_a_print_results)
				{
					std::cout << _a_str;
				}
			};
		if (_a_run_default_expected)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating all values (forward) for type {0}: ",
					typeid(T).name()));
			}
			//_l_tests.push_back(enumerate<T>());
			_l_tests.push_back(all_values<T>(new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(all_values<T>(new enumerable_t<T>(1)));
			}
			_l_tests.push_back(all_values_moving_forward<T>());
			_l_tests.push_back(all_values_moving_forward<T>(new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(all_values_moving_forward<T>(new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value()));
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				true));
			_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				true, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
					true, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value()));
			_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value()));
			_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(),elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating all values (backward) for type {0}: ",
					typeid(T).name()));
			}
			_l_tests.push_back(all_values_moving_backward<T>());
			_l_tests.push_back(all_values_moving_backward<T>(new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(all_values_moving_backward<T>(new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value()));
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				false));
			_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				false, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
					false, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value()));
			_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value()));
			_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(),
				elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}
		for (const T& _l_value : _a_lower_values)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from {0} to max for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value, max_value_t<T>().max_value()));
			_l_tests.push_back(from_m_to_n(_l_value, max_value_t<T>().max_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(_l_value, max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				true));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				true, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
					true, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_val_to_max(_l_value));
			_l_tests.push_back(from_val_to_max(_l_value, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_val_to_max(_l_value, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_forward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from max to {0} for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value));
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_max_to_val(_l_value));
			_l_tests.push_back(from_max_to_val(_l_value, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_max_to_val(_l_value, new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const T& _l_value : _a_upper_values)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from min to {0} for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value));
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_min_to_val(_l_value));
			_l_tests.push_back(from_min_to_val(_l_value, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_min_to_val(_l_value, new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from {0} to min for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value, min_value_t<T>().min_value()));
			_l_tests.push_back(from_m_to_n(_l_value, min_value_t<T>().min_value(), new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(_l_value, min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				false));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				false, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
					false, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_val_to_min(_l_value));
			_l_tests.push_back(from_val_to_min(_l_value, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_val_to_min(_l_value, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_backward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::pair<T,T>& _l_value : _a_lower_and_upper_values)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value.first, _l_value.second));
			_l_tests.push_back(from_m_to_n(_l_value.first, _l_value.second, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(_l_value.first, _l_value.second, new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to {1} for type {2}: ",_l_value.second,
					_l_value.first,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value.second, _l_value.first));
			_l_tests.push_back(from_m_to_n(_l_value.second, _l_value.first, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(_l_value.second, _l_value.first, new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::pair<T, std::size_t>& _l_value : _a_move_back_value_with_n_elements)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating backwards from {0} using {1} elements, for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				false));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				false, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
					false, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second)));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second),
				new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second),
					new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_tests.clear();
			_l_print_f("\n");
		}
		for (const std::pair<T, std::size_t>& _l_value : _a_move_forward_value_with_n_elements)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating forward from {0} using {1} elements, for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				true));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				true, new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
					true, new enumerable_t<T>(1)));
			}
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second)));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second),
				new enumerable_t<T>()));
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second),
					new enumerable_t<T>(1)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_tests.clear();
			_l_print_f("\n");
		}

		for (const std::pair<T, T>& _l_value : _a_lower_values_with_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to max using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(_l_value.first, max_value_t<T>().max_value(), new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					true, new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_val_to_max(_l_value.first, new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_forward_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(_l_value.second)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from max to {0} using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value.first, new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_max_to_val(_l_value.first, new enumerable_t<T>(_l_value.second)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::pair<T, T>& _l_value : _a_upper_values_with_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from min to {0} using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value.first, new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_min_to_val(_l_value.first, new enumerable_t<T>(_l_value.second)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to min using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(_l_value.first, min_value_t<T>().min_value(), new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					false, new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_val_to_min(_l_value.first, new enumerable_t<T>(_l_value.second)));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(_l_value.second)));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}
		for (const std::tuple<T, T, T>& _l_value : _a_lower_and_upper_values_with_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to {1} "
					"using custom difference {2} for type {3}: ",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(get<0>(_l_value), get<1>(_l_value), new enumerable_t<T>(get<2>(_l_value))));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {1} to {0} "
					"using custom difference {2} for type {3}: ",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_to_n(get<1>(_l_value), get<0>(_l_value), new enumerable_t<T>(get<2>(_l_value))));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::tuple<T, std::size_t, T>& _l_value : _a_move_back_value_with_n_elements_and_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} backwards "
					"using custom difference {2} doing so {1} times using type {3}",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					false, new enumerable_t<T>(get<2>(_l_value))));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_backward_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					new enumerable_t<T>(get<2>(_l_value))));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::tuple<T, std::size_t, T>& _l_value : _a_move_forward_value_with_n_elements_and_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} forwards "
					"using custom difference {2} doing so {1} times using type {3}",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_enumerate_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					true, new enumerable_t<T>(get<2>(_l_value))));
			}
			if constexpr (T_Constructor_Allowed)
			{
				_l_tests.push_back(from_m_move_forward_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					new enumerable_t<T>(get<2>(_l_value))));
			}
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}
		if (_a_run_default_expected)
		{
			for (const T& _l_value : _a_generate_all_with_custom_difference)
			{
				vector<enumerate_t<T>> _l_tests;
				if (_a_print_results)
				{
					_l_print_f(fmt::format("Enumerating all values (forward) with custom difference {0} for type {1}: ",
						_l_value,
						typeid(T).name()));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(all_values<T>(new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(all_values_moving_forward<T>(new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value(), new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						true, new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value(), new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value(), new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						new enumerable_t<T>(_l_value)));
				}
				run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
				_l_print_f("\n");
				_l_tests.clear();
				if (_a_print_results)
				{
					_l_print_f(fmt::format("Enumerating all values (backward) with custom difference {0} for type {1}: ",
						_l_value,
						typeid(T).name()));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(all_values_moving_backward<T>(new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value(), new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						false, new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value(), new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value(), new enumerable_t<T>(_l_value)));
				}
				if constexpr (T_Constructor_Allowed)
				{
					_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						new enumerable_t<T>(_l_value)));
				}
				run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
				_l_print_f("\n");
				_l_tests.clear();
			}
		}
	}
	template<
		typename T
	>
	void
		enumeration_test_2(
			bool _a_print_results,
			bool _a_run_default_expected,
			std::vector<T> _a_lower_values,
			std::vector<T> _a_upper_values,
			std::vector<std::pair<T, T>> _a_lower_and_upper_values,
			std::vector<std::pair<T, std::size_t>> _a_move_back_value_with_n_elements,
			std::vector<std::pair<T, std::size_t>> _a_move_forward_value_with_n_elements,
			std::vector<std::pair<T, std::size_t>> _a_lower_values_with_custom_difference,
			std::vector<std::pair<T, std::size_t>> _a_upper_values_with_custom_difference,
			std::vector<std::tuple<T, T, std::size_t>> _a_lower_and_upper_values_with_custom_difference,
			std::vector<std::tuple<T, std::size_t, std::size_t>> _a_move_back_value_with_n_elements_and_custom_difference,
			std::vector<std::tuple<T, std::size_t, std::size_t>> _a_move_forward_value_with_n_elements_and_custom_difference,
			std::vector<std::size_t> _a_generate_all_with_custom_difference
		) noexcept
	{
		using namespace abc;
		using namespace std;
		using namespace abc::gd;
		vector<T> _l_inits;
		function<void(const string&)> _l_print_f = [&](const string& _a_str)
			{
				if (_a_print_results)
				{
					std::cout << _a_str;
				}
			};
		if (_a_run_default_expected)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating all values (forward) for type {0}: ",
					typeid(T).name()));
			}
			_l_tests.push_back(all_values<T>());
			_l_tests.push_back(all_values<T>(new enumerable_t<T>()));
			_l_tests.push_back(all_values<T>(new enumerable_t<T>(1)));
			_l_tests.push_back(all_values_moving_forward<T>());
			_l_tests.push_back(all_values_moving_forward<T>(new enumerable_t<T>()));
			_l_tests.push_back(all_values_moving_forward<T>(new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value()));
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				true));
			_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				true, new enumerable_t<T>()));
			_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				true, new enumerable_t<T>(1)));
			_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value()));
			_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value()));
			_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating all values (backward) for type {0}: ",
					typeid(T).name()));
			}
			_l_tests.push_back(all_values_moving_backward<T>());
			_l_tests.push_back(all_values_moving_backward<T>(new enumerable_t<T>()));
			_l_tests.push_back(all_values_moving_backward<T>(new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value()));
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				false));
			_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				false, new enumerable_t<T>()));
			_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				false, new enumerable_t<T>(1)));
			_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value()));
			_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value()));
			_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(),
				elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}
		for (const T& _l_value : _a_lower_values)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from {0} to max for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value, max_value_t<T>().max_value()));
			_l_tests.push_back(from_m_to_n(_l_value, max_value_t<T>().max_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(_l_value, max_value_t<T>().max_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				true));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				true, new enumerable_t<T>()));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				true, new enumerable_t<T>(1)));
			_l_tests.push_back(from_val_to_max(_l_value));
			_l_tests.push_back(from_val_to_max(_l_value, new enumerable_t<T>()));
			_l_tests.push_back(from_val_to_max(_l_value, new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from max to {0} for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value));
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value, new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value, new enumerable_t<T>(1)));
			_l_tests.push_back(from_max_to_val(_l_value));
			_l_tests.push_back(from_max_to_val(_l_value, new enumerable_t<T>()));
			_l_tests.push_back(from_max_to_val(_l_value, new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const T& _l_value : _a_upper_values)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from min to {0} for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value));
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value, new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value, new enumerable_t<T>(1)));
			_l_tests.push_back(from_min_to_val(_l_value));
			_l_tests.push_back(from_min_to_val(_l_value, new enumerable_t<T>()));
				_l_tests.push_back(from_min_to_val(_l_value, new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating values from {0} to min for type {1}: ",
					_l_value,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value, min_value_t<T>().min_value()));
			_l_tests.push_back(from_m_to_n(_l_value, min_value_t<T>().min_value(), new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(_l_value, min_value_t<T>().min_value(), new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				false));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				false, new enumerable_t<T>()));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
					false, new enumerable_t<T>(1)));
			_l_tests.push_back(from_val_to_min(_l_value));
			_l_tests.push_back(from_val_to_min(_l_value, new enumerable_t<T>()));
			_l_tests.push_back(from_val_to_min(_l_value, new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max())));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
				new enumerable_t<T>()));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::pair<T, T>& _l_value : _a_lower_and_upper_values)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value.first, _l_value.second));
			_l_tests.push_back(from_m_to_n(_l_value.first, _l_value.second, new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(_l_value.first, _l_value.second, new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to {1} for type {2}: ", _l_value.second,
					_l_value.first,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value.second, _l_value.first));
			_l_tests.push_back(from_m_to_n(_l_value.second, _l_value.first, new enumerable_t<T>()));
			_l_tests.push_back(from_m_to_n(_l_value.second, _l_value.first, new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::pair<T, std::size_t>& _l_value : _a_move_back_value_with_n_elements)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating backwards from {0} using {1} elements, for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				false));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				false, new enumerable_t<T>()));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
					false, new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second)));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second),
				new enumerable_t<T>()));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(_l_value.second),
					new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_tests.clear();
			_l_print_f("\n");
		}
		for (const std::pair<T, std::size_t>& _l_value : _a_move_forward_value_with_n_elements)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating forward from {0} using {1} elements, for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				true));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
				true, new enumerable_t<T>()));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(_l_value.second),
					true, new enumerable_t<T>(1)));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value.first, elements_t(_l_value.second)));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value.first, elements_t(_l_value.second),
				new enumerable_t<T>()));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value.first, elements_t(_l_value.second),
					new enumerable_t<T>(1)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_tests.clear();
			_l_print_f("\n");
		}

		for (const std::pair<T, std::size_t>& _l_value : _a_lower_values_with_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to max using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value.first, max_value_t<T>().max_value(), new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					true, new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_val_to_max(_l_value.first, new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_m_move_forward_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(_l_value.second)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from max to {0} using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), _l_value.first, new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_max_to_val(_l_value.first, new enumerable_t<T>(_l_value.second)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::pair<T, std::size_t>& _l_value : _a_upper_values_with_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from min to {0} using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), _l_value.first, new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_min_to_val(_l_value.first, new enumerable_t<T>(_l_value.second)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to min using custom difference {1} for type {2}: ",
					_l_value.first,
					_l_value.second,
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(_l_value.first, min_value_t<T>().min_value(), new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_m_enumerate_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					false, new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_val_to_min(_l_value.first, new enumerable_t<T>(_l_value.second)));
			_l_tests.push_back(from_m_move_backward_k_elements(_l_value.first, elements_t(std::numeric_limits<std::size_t>::max()),
					new enumerable_t<T>(_l_value.second)));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}
		for (const std::tuple<T, T, std::size_t>& _l_value : _a_lower_and_upper_values_with_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} to {1} "
					"using custom difference {2} for type {3}: ",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(get<0>(_l_value), get<1>(_l_value), new enumerable_t<T>(get<2>(_l_value))));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();

			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {1} to {0} "
					"using custom difference {2} for type {3}: ",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_to_n(get<1>(_l_value), get<0>(_l_value), new enumerable_t<T>(get<2>(_l_value))));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::tuple<T, std::size_t, std::size_t>& _l_value : _a_move_back_value_with_n_elements_and_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} backwards "
					"using custom difference {2} doing so {1} times using type {3}",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					false, new enumerable_t<T>(get<2>(_l_value))));
			_l_tests.push_back(from_m_move_forward_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					new enumerable_t<T>(get<2>(_l_value))));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}

		for (const std::tuple<T, std::size_t, std::size_t>& _l_value : _a_move_forward_value_with_n_elements_and_custom_difference)
		{
			vector<enumerate_t<T>> _l_tests;
			if (_a_print_results)
			{
				_l_print_f(fmt::format("Enumerating from {0} forwards "
					"using custom difference {2} doing so {1} times using type {3}",
					get<0>(_l_value),
					get<1>(_l_value),
					get<2>(_l_value),
					typeid(T).name()));
			}
			_l_tests.push_back(from_m_enumerate_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					true, new enumerable_t<T>(get<2>(_l_value))));
			_l_tests.push_back(from_m_move_backward_k_elements(get<0>(_l_value), elements_t(get<1>(_l_value)),
					new enumerable_t<T>(get<2>(_l_value))));
			run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
			_l_print_f("\n");
			_l_tests.clear();
		}
		if (_a_run_default_expected)
		{
			for (const std::size_t& _l_value : _a_generate_all_with_custom_difference)
			{
				vector<enumerate_t<T>> _l_tests;
				if (_a_print_results)
				{
					_l_print_f(fmt::format("Enumerating all values (forward) with custom difference {0} for type {1}: ",
						_l_value,
						typeid(T).name()));
				}
				_l_tests.push_back(all_values<T>(new enumerable_t<T>(_l_value)));
				_l_tests.push_back(all_values_moving_forward<T>(new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value(), new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_m_enumerate_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						true, new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_val_to_max(min_value_t<T>().min_value(), new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_min_to_val(max_value_t<T>().max_value(), new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_m_move_forward_k_elements(min_value_t<T>().min_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						new enumerable_t<T>(_l_value)));
				run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
				_l_print_f("\n");
				_l_tests.clear();
				if (_a_print_results)
				{
					_l_print_f(fmt::format("Enumerating all values (backward) with custom difference {0} for type {1}: ",
						_l_value,
						typeid(T).name()));
				}
				_l_tests.push_back(all_values_moving_backward<T>(new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value(), new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_m_enumerate_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						false, new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_val_to_min(max_value_t<T>().max_value(), new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_max_to_val(min_value_t<T>().min_value(), new enumerable_t<T>(_l_value)));
				_l_tests.push_back(from_m_move_backward_k_elements(max_value_t<T>().max_value(), elements_t(std::numeric_limits<std::size_t>::max()),
						new enumerable_t<T>(_l_value)));
				run_comparison_enum_and_printer(_l_print_f, _a_print_results, _l_tests);
				_l_print_f("\n");
				_l_tests.clear();
			}
		}
	}
	template<
		typename T
	>
	inline
		void
		enumerate_test_4(
			T _a_element
		)
	{
		using namespace abc;
		using namespace typed;
		using namespace std;
		//std::cout <<
		//	fmt::format("enumerate_data<{0}>(all_values<{0}>()", typeid(T).name()) << std::endl;
		//for (const T& _l_int : gen_data_collection_t<T>(
		//	enumerate_data<T>(all_values<T>())
		//))
	//	{
	//			std::cout << fmt::format("{}", _l_int) << std::endl;
	//	}
		//std::cout <<
		///	fmt::format("enumerate_data<{0}>(all_values_moving_backward<{0}>()", typeid(T).name()) << std::endl;
		//for (const T& _l_int : gen_data_collection_t<T>(
		//	enumerate_data<T>(all_values_moving_backward<T>())
		//))
		//{
	//		std::cout << fmt::format("{}", _l_int) << std::endl;
	//	}
		//std::cout <<
		//	fmt::format("enumerate_data<{0}>(all_values_moving_backward<{0}>(new enumerable_t<{0}>("
		//		"1, all_values_moving_backward<bool>())))", typeid(T).name()) << std::endl;
		//for (const T& _l_int : gen_data_collection_t<T>(
		//	enumerate_data<T>(all_values_moving_backward<T>())
		//))
	//	{
	//		std::cout << fmt::format("{}, ", _l_int);
	//	}
	//	std::cout << std::endl;
		cout <<
			fmt::format("enumerate_data<{0}>(from_min_to_val<{0}>({1})", typeid(T).name(), _a_element) << endl;
		for (const T& _l_int : gen_data_collection_t<T>(
			enumerate_data<T>(from_min_to_val<T>(_a_element))
		))
		{
			std::cout << fmt::format("{}, ", _l_int);
		}
		std::cout << std::endl;
		cout <<
			fmt::format("enumerate_data<{0}>(from_min_to_val<{0}>({1})", typeid(T).name(), _a_element) << endl;
		for (const T& _l_int : gen_data_collection_t<T>(
			randomly_probe_enumerated_data<T>(from_min_to_val<T>(_a_element))
		))
		{
			std::cout << fmt::format("{}, ", _l_int);
		}
		std::cout << std::endl;
	}
	inline
		void
		enumeration_test_3(
		)
	{
		using namespace abc;
		using namespace typed;
		for (auto&& _l_int : gen_data_collection_t<X3>(
		//	enumerate_data(from_m_to_n(X3{}, X3{}, new typed::int_enumerate_t(2, 1)))
			))
		{
		//	std::cout << fmt::format("{}", _l_int) << std::endl;
		}
	}
	template<
		typename T
	>
	inline
		void
		enum_enumeration_test(
		) noexcept
	{
		int _l_chars [3] = { 1,2,3 };
		std::cout << fmt::format("{}", _l_chars) << std::endl;
		using namespace abc;
		std::vector<T> _l_inits;
			std::cout << "Enumerating all values" << std::endl;
			for (auto&& _l_int : gen_data_collection_t<T>(
				static_data(_l_inits)
			//	, enumeration<T>(finitely_enumerable<T>())
			))
			{
			//	std::cout << fmt::format("{0} value = {1}", typeid(T).name(), _l_int) << std::endl;
			}
	}
	inline
		void
		collections(
		) noexcept
	{
		using namespace abc;
		using T = std::array<uint8_t,2>;
		std::vector<T> _l_inits;
		std::cout << "Enumerating all values" << std::endl;
		T _l_min;
		_l_min[0] = 254;
		_l_min[1] = 254;
		T _l_max;
		_l_max[0] = 255;
		_l_max[1] = 255;
		for (auto&& _l_int : gen_data_collection_t<T>(
		//	iterate_over(_l_inits)
			//Min and max bound. Direction and number of elemnets can be worked out automatically.
		//	, enumeration<T>(*(new enumerable_array_t<uint8_t, 2>(finitely_enumerable<uint8_t>(), enum_range(_l_min, _l_max))))
			//Bounds is automatic, because there is a min and max value.
		//	, enumeration<T>(*(new enumerable_array_t<uint8_t, 2>(finitely_enumerable<uint8_t>())))
			// A start point and a number of elements to generate. Direction is assumed to be forward
		//	, enumeration<T>(*(new enumerable_array_t<uint8_t, 2>(finitely_enumerable<uint8_t>(), enum_range(_l_min, elements(100))))
				// Start point and number of elements to generate
		//	, enumeration<T>(*(new enumerable_array_t<uint8_t, 2>(finitely_enumerable<uint8_t>(), enum_range(_l_min, elements(100), forwards)))))
		//	, enumeration<T>(*(new enumerable_array_t<uint8_t, 2>(finitely_enumerable<uint8_t>(), enum_range(_l_max, elements(100), backwards)))))
			//Starts from the default start
		//	, enumeration<T>(*(new enumerable_array_t<uint8_t, 2>(finitely_enumerable<uint8_t>(), enum_range(elements(100))))))
			//Starts from the default end
		//	, enumeration<T>(*(new enumerable_array_t<uint8_t, 2>(finitely_enumerable<uint8_t>(), enum_range(elements(100), backwards)))))
		))
		{
				std::cout << fmt::format("{0} value = {1}", typeid(T).name(), _l_int) << std::endl;
		}
	}
}