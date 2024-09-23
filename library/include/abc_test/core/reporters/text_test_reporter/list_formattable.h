#pragma once
#include "abc_test/utility/internal/macros.h"
#include <vector>
#include "abc_test/core/reporters/text_test_reporter/enum_print_data.h"
#include <string>

_BEGIN_ABC_REPORTERS_NS
template<
	typename T
>
using decider_print_pair_t = std::pair<T, enum_print_data_t>;
template<
	typename T,
	typename Formattable_Identifier
>
struct list_formattable_t
{
public:
	__constexpr
		std::vector<std::string>
		make_list() const = 0;
private:
	__constexpr
		bool
		check_data(
			const decider_print_pair_t<Formattable_Identifier>& _a_printer_pair,
			const T& _a_element
		) const;
	__constexpr
		check_data_fid(
			const Formattable_Identifier& _a_fid,
			const T& _a_element
		) const = 0;
};
_END_ABC_REPORTERS_NS
_BEGIN_ABC_REPORTERS_NS
template<
	typename T,
	typename Formattable_Identifier
>
__constexpr_imp
	bool
	list_formattable_t<T,Formattable_Identifier>::check_data(
		const decider_print_pair_t<Formattable_Identifier>& _a_printer_pair,
		const T& _a_element
	) const
{
	return always_print(_a_printer_pair.second) ||
		(constexual_print(_a_printer_pair.second) && 
		check_data_fid(_a_printer_pair.first, _a_report));
}
_END_ABC_REPORTERS_NS