#pragma once
#include <vector>
#include <tuple>

#include "fmt/format.h"
#include "abc_test/utility/internal/macros.h"

#include "abc_test/utility/internal/log.h"

_BEGIN_ABC_DS_NS
/*!
* @brief Object designed to hold specific iteration data about a gen_data for loop -
* the generation collection index (which gen_data in a collection this element is from),
* the mode of the generator which created this element, and any additional data
* associated with the gen_data to create the element.
*/ 
struct for_loop_iteration_data_t
{
public:
	/*!
	 * @brief Equality operator for for_loop_iteration_data_t elements.
	 * @param _a_rhs The element to compare this element to.
	 * @return True if equal, false if not.
	 */
	__constexpr
		bool
		operator==(
			const for_loop_iteration_data_t& _a_rhs
			) const noexcept = default;
	/*!
	 * @brief Spaceship operator for repetition_data_t elements.
	 */
	__constexpr
		auto
		operator<=>(
			const for_loop_iteration_data_t& _a_rhs
			) const noexcept = default;
	std::size_t generation_collection_index = 0;
	std::size_t mode = 0;
	std::string additional_data;
};
_END_ABC_DS_NS

template
<
>
struct fmt::formatter<abc::ds::for_loop_iteration_data_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	//Cannot be constexpr due to use of fmt::format.
	__no_constexpr
		auto
		format(
			abc::ds::for_loop_iteration_data_t _a_rd,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
_END_ABC_DS_NS

__no_constexpr_imp
auto
fmt::formatter<abc::ds::for_loop_iteration_data_t>::format(
	abc::ds::for_loop_iteration_data_t _a_rd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	const string _l_rv{
		fmt::format(
			"{0} {{"
			"{1} = {2}"
			", {3} = {4}"
			", {5} = {6}"
			"}}"
			, typeid(_a_rd).name()
			, "_m_generation_collection_index", _a_rd.generation_collection_index
			, "_m_additional_data", _a_rd.additional_data
			, "_m_mode", _a_rd.mode
		) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}