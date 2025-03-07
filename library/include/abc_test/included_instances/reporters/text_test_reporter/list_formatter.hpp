#pragma once
#include "abc_test/utility/internal/macros.hpp"
#include <vector>
#include "abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp"
#include <string>
#include <variant>

#include "abc_test/core/ds/source/single_source.hpp"

#include "abc_test/core/errors/test_library_exception.hpp"

_BEGIN_ABC_REPORTERS_NS
template<
	typename T,
	typename Formattable_Identifier,
	typename Print_Config
>
struct list_formattable_t
{
public:
	__constexpr
		virtual
		std::vector<std::string>
		get_data(
			const Formattable_Identifier& _a_collection,
			const T& _a_element,
			const Print_Config& _a_pc
		) const = 0;
protected:
	__constexpr
		virtual
		bool
		check_data(
			const Formattable_Identifier& _a_fid,
			const T& _a_element
		) const = 0;
};
template<
	bool Return_Value_Is_One_Dimensional_Vector = true,
	typename Formattable_Identifier,
	typename T,
	typename Print_Config,
	typename Class
>
__constexpr_imp
std::conditional_t< Return_Value_Is_One_Dimensional_Vector,
	std::vector<std::string>,
	std::vector<std::vector<std::string>>>
	get_all_data(
		const enum_print_pair_collection_t<Formattable_Identifier>& _a_collection,
		const T& _a_element,
		const Print_Config& _a_pc,
		const Class& _a_class
	);
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template<
	bool Return_Value_Is_One_Dimensional_Vector,
	typename Formattable_Identifier,
	typename T,
	typename Print_Config,
	typename Class
>
__constexpr_imp
std::conditional_t< Return_Value_Is_One_Dimensional_Vector,
	std::vector<std::string>,
	std::vector<std::vector<std::string>>>
	get_all_data(
		const enum_print_pair_collection_t<Formattable_Identifier>& _a_collection,
		const T& _a_element,
		const Print_Config& _a_pc,
		const Class& _a_class
	)
{
	using RV = std::conditional_t< Return_Value_Is_One_Dimensional_Vector,
		std::vector<std::string>,
		std::vector<std::vector<std::string>>>;
	using namespace std;
	RV _l_rv{};

	for (const enum_print_pair_t<Formattable_Identifier>& _l_pair : _a_collection)
	{
		if (always_print(_l_pair.second) ||
			(constexual_print(_l_pair.second) &&
				_a_class.check_data(_l_pair.first, _a_element)))
		{
			if constexpr (Return_Value_Is_One_Dimensional_Vector)
			{
				_l_rv.append_range(_a_class.get_data(_l_pair.first, _a_element, _a_pc));
			}
			else
			{
				_l_rv.push_back(_a_class.get_data(_l_pair.first, _a_element, _a_pc));
			}
		}
	}
	return _l_rv;
}
_END_ABC_REPORTERS_NS