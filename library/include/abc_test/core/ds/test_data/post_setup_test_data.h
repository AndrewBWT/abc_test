#pragma once
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/core/test_options.h"
#include "abc_test/utility/str/string_utility.h"
#include <vector>


_BEGIN_ABC_DS_NS
//! Type synonym for a test_path_hierarchy_t element.
using test_path_element_t = std::string;
//! A reference to a test_path_hierarchy_element_t. 
using test_path_element_ref_t = std::string_view;
//! Synonym for a vector of strings, used to represent the hierarchy a test path represents.
using test_path_hierarchy_t = std::vector<test_path_element_t>;
/*!
 * @brief Object represents a test after it has been processed by the runtime system;
 * that is, registered, its test path hierarchy calculated, 
 * its discovery id found, 
 * its repetition tree (if it has one) found from the input paramters,
 * and a normalised number of threads allowed for the test.
 * 
 */
struct post_setup_test_data_t
{
public:
	/*!
	 * @brief Constructor
	 * @param _a_registered_test_data Underlying registered_test_data_t object.
	 * @param _a_test_hierarchy_delimiter The delimiter used to split the test path.
	 * @param _a_discovery_id The discovery ID of the test.
	 * @param _a_is_test_to_be_ran Whether the test is to be ran.
	 * @param _a_repetition_data The repetition data (if any) associated with the test
	 * @param _a_maximum_threads_allowed The normalised number of threads allowed.
	 */
	__constexpr
		post_setup_test_data_t(
			const registered_test_data_t& _a_registered_test_data,
			const test_path_delimiter_ref _a_test_hierarchy_delimiter,
			const size_t _a_discovery_id,
			const bool _a_is_test_to_be_ran,
			const ds::repetition_tree_t* _a_repetition_data,
			const size_t _a_maximum_threads_allowed
		) noexcept;
	/*!
	 * @brief Gets the internal registered_test_data object.
	 * @return Internal registered_test_data_t object.
	 */
	__constexpr
		const registered_test_data_t&
		registered_test_data(
		) const;
	/*!
	 * @brief Gets the internal test_path_hierarchy of the test.
	 * @return A cref to the test's test_path_hierarchy_t object.
	 */
	__constexpr
		const test_path_hierarchy_t&
		test_path_hierarchy(
		) const noexcept;
	/*!
	 * @brief Tells the caller whether the test is to be ran.
	 * @return True if the test is to be ran, false otherwise. 
	 */
	__constexpr
		bool
		is_test_to_be_ran(
		) const noexcept;
	/*!
	 * @brief Tells the caller this test's discovery ID.
	 * @return The test's discovery ID.
	 */
	__constexpr
		std::size_t
		discovery_id(
		) const noexcept;
	/*!
	 * @brief Tells the caller whether this test has repetition data associated with it.
	 * @return True if it does, false if it does not.
	 */
	__constexpr
		bool
		has_repetition_data(
		) const noexcept;
	/*!
	 * @brief Gets the internal repetition tree of the test.
	 * 
	 * Note this function will throw an exception if the internal repetition_tree_t
	 * object is a nullptr.
	 * 
	 * @return Gets a cref to the internal repetition tree of the test. 
	 */
	__constexpr
		const ds::repetition_tree_t&
		repetition_data(
		) const;
	/*!
	 * @brief Gets the number of thread resourses required by the test.
	 * @return std::size_t representing the number of thread resourses required by the test.
	 */
	__constexpr
		std::size_t
		thread_resourses_required(
		) const noexcept;
	/*!
	 * @brief Equality operator.
	 * @param _a_rhs The post_setup_test_data_t object to compare this object to.
	 * @return True if equal, false if not.
	 */
	__constexpr
		bool
		operator==(
			const post_setup_test_data_t& _a_rhs
		) const noexcept;
private:
	const registered_test_data_t& _m_registered_test_data;
	test_path_hierarchy_t _m_test_path_hierarchy;
	size_t _m_discovery_id;
	bool _m_is_test_to_be_ran;
	size_t _m_thread_resourses_required;
	const ds::repetition_tree_t* _m_repetition_data;
};
_END_ABC_DS_NS

/*!
* formatter for post_setup_test_ata object.
*/
template
<
> 
struct fmt::formatter<abc::ds::post_setup_test_data_t> : formatter<string_view> 
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__no_constexpr
		auto 
		format(
			abc::ds::post_setup_test_data_t _a_pstd, 
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp
post_setup_test_data_t::post_setup_test_data_t(
	const registered_test_data_t& _a_registered_test_data,
	const test_path_delimiter_ref _a_test_hierarchy_delimiter,
	const size_t _a_discovery_id,
	const bool _a_is_test_to_be_ran,
	const ds::repetition_tree_t* _a_repetition_data,
	const size_t _a_maximum_threads_allowed
) noexcept
	: _m_registered_test_data(_a_registered_test_data)
	, _m_test_path_hierarchy(utility::str::split_string(_a_registered_test_data._m_user_data.path, _a_test_hierarchy_delimiter))
	, _m_discovery_id(_a_discovery_id)
	, _m_is_test_to_be_ran(_a_is_test_to_be_ran)
	, _m_repetition_data(_a_repetition_data)
	, _m_thread_resourses_required(
		_a_registered_test_data._m_user_data.threads_required == 0 ?
		_a_maximum_threads_allowed : 
		std::min(_a_maximum_threads_allowed, _a_registered_test_data._m_user_data.threads_required))
{

}
__constexpr_imp
const registered_test_data_t&
post_setup_test_data_t::registered_test_data(
) const
{
	return _m_registered_test_data;
}
__constexpr_imp
const test_path_hierarchy_t&
post_setup_test_data_t::test_path_hierarchy(
) const noexcept
{
	return _m_test_path_hierarchy;
}
__constexpr_imp
bool
post_setup_test_data_t::is_test_to_be_ran(
) const noexcept
{
	return _m_is_test_to_be_ran;
}
__constexpr_imp
size_t
post_setup_test_data_t::discovery_id(
) const noexcept
{
	return _m_discovery_id;
}
__constexpr_imp
bool
post_setup_test_data_t::has_repetition_data(
) const noexcept
{
	return _m_repetition_data != nullptr;
}
__constexpr_imp
const ds::repetition_tree_t&
post_setup_test_data_t::repetition_data(
) const
{
	if (_m_repetition_data == nullptr)
	{
		throw abc::errors::test_library_exception_t(
			"Attempted to access post_setup_test_data_t's repetition data. "
			"However, the reptition_data contains a nullptr. "
			"Use has_repetition_data to check this."
		);
	}
	else
	{
		return *_m_repetition_data;
	}
}
__constexpr_imp
std::size_t
post_setup_test_data_t::thread_resourses_required(
) const noexcept
{
	return _m_thread_resourses_required;
}
__constexpr_imp
bool
post_setup_test_data_t::operator==(
	const post_setup_test_data_t& _a_rhs
) const noexcept
{
	__cmp_test(_m_test_path_hierarchy);
	__cmp_test(_m_registered_test_data);
	__cmp_test(_m_discovery_id);
	__cmp_test(_m_is_test_to_be_ran);
	__cmp_test(_m_repetition_data);
	__cmp_test(_m_thread_resourses_required);
	return true;
}
_END_ABC_DS_NS

__no_constexpr_imp
auto 
fmt::formatter<abc::ds::post_setup_test_data_t>::format(
	abc::ds::post_setup_test_data_t _a_pstd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	const string _l_rv{ fmt::format(
		"post_setup_test_data_t {{"
		"_m_registered_test_data = {0}, "
		"_m_test_path_hierarchy = {1}, "
		"_m_discovery_id = {2}, "
		"_m_is_test_to_be_ran = {3}, "
		"_m_thread_resourses_required = {4}, "
		"_m_repetition_data = {5}}}",
		fmt::format("{0}", _a_pstd.registered_test_data()),
		_a_pstd.test_path_hierarchy(),
		_a_pstd.discovery_id(),
		_a_pstd.is_test_to_be_ran(),
		_a_pstd.thread_resourses_required(),
		_a_pstd.has_repetition_data() ? _a_pstd.repetition_data().print_repetition_tree() : "nullptr"
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}
