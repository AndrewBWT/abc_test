
#pragma once

#include "abc_test/core/ds/test_data/post_setup_test_data.h"

#include "abc_test/core/ds/repetitions/for_loop_data_collection.h"

#include <fmt/ranges.h>

_BEGIN_ABC_DS_NS
/*!
 * @brief Object used to represent a test currently being ran.
 */
struct invoked_test_info_t
{
public:
	/*!
	 * @brief Constructor
	 * @param _a_seed_seq The seed for this test. 
	 * @param _a_test_info The post_setup_test_data_t component of this test.
	 * @param _a_order_ran_id The ID representing the order this test was ran in.
	 * @param _a_options The options_t object used to setup this object.
	 */
	__no_constexpr
		invoked_test_info_t(
			const utility::seed_t& _a_seed_seq,
			const post_setup_test_data_t& _a_test_info,
			const size_t _a_order_ran_id,
			const test_options_t& _a_options
		) noexcept;
	/*!
	 * @brief Returns the path of the test to the caller.
	 * 
	 * Each test has a path, built from the user-defined path, and the test's name.
	 * 
	 * It is used to store data, such as failed test results.
	 * 
	 * @return The path of the test.
	 */
	__constexpr
		const std::filesystem::path&
		path(
		) const noexcept;
	/*!
	 * @brief Returns the random generator to the caller.
	 * @return Test's random generator.
	 */
	__constexpr
		const utility::rng&
		get_random_generator(
		) noexcept;
	/*!
	 * @brief Returns the test's post_setup_tst_data_t object to the caller.
	 * @return The test's post_setup_test_data_t object.
	 */
	__constexpr
		const post_setup_test_data_t&
		post_setup_test_data(
		) const;
	/*!
	 * @brief Gets the test's for_loop_data_collection_t object.
	 * @return A reference to the tests for_loop_data_collection_t object.
	 */
	__constexpr
		ds::for_loop_data_collection_t&
		for_loop_data_collection(
		) noexcept;
	/*!
	* @brief Gets the next set of repetition_data from the underlying repetition_data
	* and the current for_loop_data_collection structure.
	* 
	* If there is no next element, returns an empty optional.
	* 
	* @return An optional element, representing the next repetition_data_t element.
	*/
	__constexpr
		std::optional<ds::repetition_data_t>
		get_repetition_iterator_data(
		) noexcept;
	/*!
	 * @brief Gets the next "level" of iteration data, based on the current position
	 * in the iteration data.
	 * 
	 * Specifically, it is used when a new for loop is encountered, getting the first
	 * element in the repetition data for that for loop. If there isn't one, it returns
	 * an empty optional.
	 * 
	 * @return An optional with the repetition_data_t object, representing the first
	 * element to be repeated in the next for loop.
	 */
	__constexpr
		std::optional<ds::repetition_data_t>
		increment_repetition_iterator_data(
		) noexcept;
	/*!
	 * @brief Tells the caller if the current repetition_data_sequence_t is contained
	 * in the test's reptition_tree.
	 * 
	 * Essentially this is used in manual for loops, to check whether the current
	 * element is to be ran or not.
	 * 
	 * @return True if it is in the test's repetition tree, false if not.
	 */
	__constexpr
		bool
		is_repetition_to_be_repeated(
		) const noexcept;
	/*!
	 * @brief Returns the internal repetition_tree to the caller.
	 * 
	 * To be clear, this is the repetition_tree_t created by the test, not the one
	 * given to the test as a parameter.
	 * 
	 * @return The test's repetition_tree.
	 */
	__constexpr
		const ds::repetition_tree_t&
		repetition_tree(
		) const noexcept;
	/*!
	 * @brief Updates the test's repetition tree with a new repetition_data_sequence_t.
	 * 
	 */
	__constexpr
		void
		update_repetition_tree(
			const ds::repetition_data_sequence_t& _a_rds
		) noexcept;
	/*!
	 * @brief Gets the current order ran ID.
	 * 
	 * This is the order the tests are ran in. Note that this is given to the test
	 * before it is spawned in its own thread, so it may be the case that the test_reporter
	 * gets the tests in a different order to the order they were ran in.
	 * 
	 * @return Integer representing the order the test was ran in.
	 */
	__constexpr
		std::size_t
		order_ran_id(
		) const noexcept;
private:
	const post_setup_test_data_t& _m_post_setup_test_data;
	ds::repetition_tree_t _m_repetition_tree;
	ds::for_loop_data_collection_t _m_for_loop_data_collection;
	std::size_t _m_order_ran_id;
	utility::rng _m_this_tests_random_generator;
	std::filesystem::path _m_path;
};
namespace
{
	__no_constexpr
		std::filesystem::path
		create_test_path(
			const post_setup_test_data_t& _a_test_info,
			const test_options_t& _a_options
		) noexcept;
}
_END_ABC_DS_NS

/*!
* formatter for post_setup_test_ata object.
*/
template
<
>
struct fmt::formatter<abc::ds::invoked_test_info_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__no_constexpr
		auto format(
			abc::ds::invoked_test_info_t _a_iti,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
__no_constexpr_imp
invoked_test_info_t::invoked_test_info_t(
	const utility::seed_t& _a_seed_seq,
	const post_setup_test_data_t& _a_post_setup_test_data,
	const size_t _a_order_ran_id,
	const test_options_t& _a_options
) noexcept
	: _m_post_setup_test_data(_a_post_setup_test_data)
	, _m_for_loop_data_collection(ds::for_loop_data_collection_t())
	, _m_order_ran_id(_a_order_ran_id)
	, _m_this_tests_random_generator(_a_seed_seq)
	, _m_path(create_test_path(_a_post_setup_test_data, _a_options))
{
}
__constexpr_imp
const std::filesystem::path&
invoked_test_info_t::path(
) const noexcept
{
	return _m_path;
}
__constexpr_imp
const utility::rng&
invoked_test_info_t::get_random_generator(
) noexcept
{
	return _m_this_tests_random_generator;
}
__constexpr_imp
const post_setup_test_data_t&
invoked_test_info_t::post_setup_test_data(
) const
{
	return _m_post_setup_test_data;
}
__constexpr_imp
ds::for_loop_data_collection_t&
invoked_test_info_t::for_loop_data_collection(
) noexcept
{
	return _m_for_loop_data_collection;
}
__constexpr_imp
std::optional<ds::repetition_data_t>
invoked_test_info_t::get_repetition_iterator_data(
) noexcept
{
	using namespace std;
	using namespace ds;
	using enum utility::internal::internal_log_enum_t;
	const optional<repetition_data_t> _l_rv{
		_m_post_setup_test_data.repetition_data().find_next_for_loop(_m_for_loop_data_collection.repetition_data_sequence()) };
	return _l_rv;
}
__constexpr_imp
std::optional<ds::repetition_data_t>
invoked_test_info_t::increment_repetition_iterator_data(
) noexcept
{
	using namespace std;
	using namespace ds;
	using enum utility::internal::internal_log_enum_t;
	const optional<repetition_data_t> _l_rv{
		_m_post_setup_test_data.repetition_data().increment_last_index(_m_for_loop_data_collection.repetition_data_sequence()) };
	return _l_rv;
}
__constexpr_imp
bool
invoked_test_info_t::is_repetition_to_be_repeated(
) const noexcept
{
	return (not _m_post_setup_test_data.has_repetition_data()) ? true :
		_m_post_setup_test_data.repetition_data().is_repetition_to_be_repeated(_m_for_loop_data_collection.repetition_data_sequence());
}
__constexpr_imp
const ds::repetition_tree_t&
invoked_test_info_t::repetition_tree(
) const noexcept
{
	return _m_repetition_tree;
}
__constexpr_imp
void
invoked_test_info_t::update_repetition_tree(
	const ds::repetition_data_sequence_t& _a_rds
) noexcept
{
	using enum utility::internal::internal_log_enum_t;
	_m_repetition_tree.add_repetition(_a_rds);
	_LIBRARY_LOG(REPETITION_INFO, fmt::format("Repetition tree after insertion = {0}", _m_repetition_tree.print_repetition_tree()));
}
__constexpr_imp
std::size_t
invoked_test_info_t::order_ran_id(
) const noexcept
{
	return _m_order_ran_id;
}
namespace
{
	__no_constexpr_imp
		std::filesystem::path
		create_test_path(
			const post_setup_test_data_t& _a_test_info,
			const test_options_t& _a_options
		) noexcept
	{
		using namespace std::filesystem;
		using namespace utility;
		path_t _l_path{ _a_options._m_test_data_root_path };
		for (const test_path_element_ref_t& _a_test_path_component : _a_test_info.test_path_hierarchy())
		{
			_l_path /= _a_test_path_component;
		}
		_l_path /= _a_test_info.registered_test_data()._m_user_data.name;
		if (not exists(_l_path))
		{
			create_directories(_l_path);
		}
		return _l_path;
	}
}
_END_ABC_DS_NS

__no_constexpr_imp
auto
fmt::formatter<abc::ds::invoked_test_info_t>::format(
	abc::ds::invoked_test_info_t _a_iti,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	const string _l_rv{ fmt::format(
		"{0} {{"
		"{1} = {2}, "
		"{3} = {4}, "
		"{5} = {6}, "
		"{7} = {8}, "
		"{9} = {10}, "
		"{11} = {12}}}",
		typeid(_a_iti).name(),
		"_m_post_setup_test_data", _a_iti.post_setup_test_data(),
		"_m_repetition_tree", _a_iti.repetition_tree(),
		"_m_for_loop_data_collection", _a_iti.for_loop_data_collection(),
		"_m_order_ran_id", _a_iti.order_ran_id(),
		"_m_this_tests_random_generator","<unwritten>",
		"_m_path", _a_iti.path().string()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}