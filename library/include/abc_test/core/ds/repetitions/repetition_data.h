#pragma once
#include "abc_test/core/ds/repetitions/for_loop_iteration_data.h"

_BEGIN_ABC_DS_NS
/*!
* Structure specificlaly designed to describe how a gen_data for loop has been navigated. 
* 
* It consists of the index in the for loop, the collection index (in case a collection of gen_datas was used)
* and an additional data part, represented as a string.
*/
/*!
* @brief Object designed to describe an element in a gen_data for loop. This is used
* when creating repetition data when wanting to repeat tests.
*/
struct repetition_data_t
{
public:
	/*!
	 * @brief Equality operator for repetition_data_t elements.
	 * @param _a_rhs The element to compare this element to.
	 * @return True if equal, false if not.
	 */
	__constexpr
		bool
		operator==(
			const repetition_data_t& _a_rhs
			) const noexcept = default;
	/*!
	 * @brief Spaceship operator for repetition_data_t elements.
	 */
	__constexpr
		auto
		operator<=>(
			const repetition_data_t& _a_rhs
			) const noexcept = default;
	std::size_t for_loop_index = 0;
	for_loop_iteration_data_t for_loop_iteration_data;
};
using repetition_data_sequence_t = std::vector<repetition_data_t>;
using opt_repetition_data_t = std::optional<repetition_data_t>;
_END_ABC_DS_NS

template
<
>
struct fmt::formatter<abc::ds::repetition_data_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	//Cannot be constexpr due to use of fmt::format.
	__no_constexpr
		auto
		format(
			abc::ds::repetition_data_t _a_rd,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

__no_constexpr_imp
auto
fmt::formatter<abc::ds::repetition_data_t>::format(
	abc::ds::repetition_data_t _a_rd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	const string _l_rv{ 
		fmt::format(
			"{0} {{"
			"{1} = {2}"
			", {3} = {4}}}"
			, typeid(_a_rd).name()
			, "_m_for_loop_index", _a_rd.for_loop_index
			, "_m_flid", _a_rd.for_loop_iteration_data
		)};
	return formatter<string_view>::format(_l_rv, _a_ctx);
}