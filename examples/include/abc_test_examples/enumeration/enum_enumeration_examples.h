#pragma once
#include "abc_test/gen_data/enumerable/specializations/enum.h"
#include "abc_test/core/test_assertions.h"
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/gen_data/enumeration.h"
#include "abc_test/gen_data/collection.h"
#include "abc_test/gen_data/collection_iterator.h"

// Define an enum
namespace examples
{
	enum class X2 { A, B, C, D, F, G, I = 100, J = 7 };
}

// This is all that is required to create an enumerable enum using the machinery we have created.
// IF WE USED THE INLINE NAMESPACE, WE WOULD NOT BE ABLE TO DEFINE THIS FUNCTION WITHIN THIS NAMESPACE - ATLEAST
// IN VISUAL STUDIO. An alternative way of writing this function is commented out below this function.
namespace abc
{
	template<>
	auto get_enum_list() -> enum_list_t<examples::X2>
	{
		using enum examples::X2;
		return { A,B,C,D,F,G,I,J };
	}
}
/*
template<
>
__constexpr_imp
abc::gd::enum_list_t<examples::X2> abc::gd::get_enum_list() noexcept
{
	using enum examples::X2;
	return { A,B,C,D,F,G,I,J };
}*/



template
<
>
struct fmt::formatter<examples::X2> : formatter<string_view>
{
	__constexpr_imp
		auto
		format(
			examples::X2 _a_rd,
			format_context& _a_cxt
		) const
		->format_context::iterator
	{
		using enum examples::X2;
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

_TEST_CASE("Enumerating an enum for fun!", "examples::enumeration")
{
	using namespace abc;
	using namespace examples;
//	using namespace abc::utility::io;
	using enum X2;
	fmt::print("Enumerating from F to max. "
		"Note enumerate_data and from_val_to_max don't require type arguments. "
		"from_val_to_max's can be inferred from the argument, and enumerate_data's "
		"can be inferred from the return type of from_val_to_max. Results: [");
	for (auto& _l_enum : enumerate_data(from_val_to_max(F)))
	{
		fmt::print("{} ", _l_enum);
	}
	fmt::println("]");
	fmt::print("Enumerating from F to min, enumerating backwards. Results [");
	for (auto& _l_enum : enumerate_data(from_val_to_min(F)))
	{
		fmt::print("{} ", _l_enum);
	}
	fmt::println("]");
	fmt::print("Enumerating over all X2 values. Note that all_values does require a type argument, as its type "
		"cannot be inferred automatically. ");
	for (auto& _l_enum : enumerate_data(all_values<X2>()))
	{
		fmt::print("{} ", _l_enum);
	}
	fmt::println("]");
	fmt::print("Enumerating a range of X2 values. Note that the range is going backwards "
		"Any tests which fail are written to the file \"fail_data\". Results = [");
	for (auto& _l_enum : enumerate_data_using_file_type_2(from_m_to_n(G,A),"fail_data"))
	{
		fmt::print("{} ", _l_enum);
	}
	fmt::println("]");
}