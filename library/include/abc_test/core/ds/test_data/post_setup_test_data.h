#pragma once
#include "abc_test/core/ds/test_data/registered_test_data.h"
#include "abc_test/core/test_options.h"
#include "abc_test/utility/str/string_utility.h"
#include <vector>


_BEGIN_ABC_DS_NS
	/*!
	* Object containing data pertainig to a test after it has been processed.
	*/
	using test_path_element_t = std::string;
	using test_path_element_ref_t = std::string_view;
	using test_path_hierarchy_t = std::vector<test_path_element_t>;
	struct post_setup_test_data_t
	{
	public:
		/*!
		* Default constructor. Is generally designed not to be used.
		*/
		__constexpr
			post_setup_test_data_t(
			) noexcept;
		/*!
		* Main constructor.
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
		* Returns true if the object has an associated registered_Test_data.
		*/
		__constexpr
			bool
			has_registered_test_data(
			) const noexcept;
		/*!
		* Returns the internal registered_Test_data associated with this test.
		*
		* If there is no registered_test_Data, then this function is UB.
		*/
		__constexpr
			const registered_test_data_t&
			registered_test_data(
			) const;
		/*!
		* Gets the test path hierarchy.
		*/
		__constexpr
			const test_path_hierarchy_t&
			test_path_hierarchy(
			) const noexcept;
		/*!
		* returns true if the test designated to be ran.
		*/
		__constexpr
			bool
			is_test_to_be_ran(
			) const noexcept;
		/*!
		* Returns the discovery id; the unique number allocated to each test, based on the order
		* it was encountered in.
		*/
		__constexpr
			size_t
			discovery_id(
			) const noexcept;
		/*!
		* Return true if the element has repeittion data.
		*/
		__constexpr
			bool
			has_repetition_data(
			) const noexcept;
		/*!
		* Gets the internal repetition tree associated with this element, if there is one.
		*
		* If no repetition tree is associated with this element, this function is UB.
		*/
		__constexpr
			const ds::repetition_tree_t&
			repetition_data(
			) const noexcept;
		/*!
		* Returns the numebr of thread resourses allocated to the object
		*/
		__constexpr
			std::size_t
			thread_resourses_required(
			) const noexcept;
		/*!
		* Function to comapre elements.
		*/
		__constexpr
			bool
			operator==(
				const post_setup_test_data_t& _a_rhs
				) const noexcept;
	private:
		const registered_test_data_t* _m_registered_test_data;
		test_path_hierarchy_t _m_test_path_hierarchy;
		size_t _m_discovery_id;
		bool _m_is_test_to_be_ran;
		size_t _m_thread_resourses_required;
		const ds::repetition_tree_t* _m_repetition_data;
		__constexpr
			post_setup_test_data_t(
				const registered_test_data_t* _a_registered_test_data,
				const test_path_hierarchy_t& _a_test_path_hierarchy,
				const size_t _a_discovery_id,
				const bool _a_is_test_to_be_ran,
				const ds::repetition_tree_t* _a_repetition_data,
				const size_t _a_thread_resourses_required
			) noexcept;
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
		) noexcept
		: post_setup_test_data_t(nullptr, test_path_hierarchy_t(), 0, false, nullptr, 0)
	{

	}
	__constexpr_imp
		post_setup_test_data_t::post_setup_test_data_t(
			const registered_test_data_t& _a_registered_test_data,
			const test_path_delimiter_ref _a_test_hierarchy_delimiter,
			const size_t _a_discovery_id,
			const bool _a_is_test_to_be_ran,
			const ds::repetition_tree_t* _a_repetition_data,
			const size_t _a_maximum_threads_allowed
		) noexcept
		: post_setup_test_data_t(&_a_registered_test_data,
			utility::str::split_string(_a_registered_test_data._m_user_data.path, _a_test_hierarchy_delimiter)
			, _a_discovery_id, _a_is_test_to_be_ran, _a_repetition_data,
			_a_registered_test_data._m_user_data.threads_required == 0 ?
			_a_maximum_threads_allowed : std::min(_a_maximum_threads_allowed, _a_registered_test_data._m_user_data.threads_required))
	{

	}
	__constexpr_imp
		bool
		post_setup_test_data_t::has_registered_test_data(
		) const noexcept
	{
		return _m_registered_test_data != nullptr;
	}
	__constexpr_imp
		const registered_test_data_t&
		post_setup_test_data_t::registered_test_data(
		) const
	{
		if (_m_registered_test_data == nullptr)
		{
			throw errors::test_library_exception_t(
				fmt::format("post_setup_test_data_t does not have valid post_setup_test_data elemnet. "
					"_a_post_setup_test_data = {0}",
					*this));
		}
		return *_m_registered_test_data;
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
		) const noexcept
	{
		return *_m_repetition_data;
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
	__constexpr_imp
		post_setup_test_data_t::post_setup_test_data_t(
			const registered_test_data_t* _a_registered_test_data,
			const test_path_hierarchy_t& _a_test_path_hierarchy,
			const size_t _a_discovery_id,
			const bool _a_is_test_to_be_ran,
			const ds::repetition_tree_t* _a_repetition_data,
			const size_t _a_thread_resourses_required
		) noexcept
		: _m_registered_test_data(_a_registered_test_data)
		, _m_test_path_hierarchy(_a_test_path_hierarchy)
		, _m_discovery_id(_a_discovery_id)
		, _m_is_test_to_be_ran(_a_is_test_to_be_ran)
		, _m_repetition_data(_a_repetition_data)
		, _m_thread_resourses_required(_a_thread_resourses_required)
	{

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
	string _l_rv{ fmt::format(
		"post_setup_test_data_t {{"
		"_m_registered_test_data = {0}, "
		"_m_test_path_hierarchy = {1}, "
		"_m_discovery_id = {2}, "
		"_m_is_test_to_be_ran = {3}, "
		"_m_thread_resourses_required = {4}, "
		"_m_repetition_data = {5}}}",
		_a_pstd.has_registered_test_data() ? fmt::format("{0}", _a_pstd.registered_test_data()) : "nullptr",
		_a_pstd.test_path_hierarchy(),
		_a_pstd.discovery_id(),
		_a_pstd.is_test_to_be_ran(),
		_a_pstd.thread_resourses_required(),
		_a_pstd.has_repetition_data() ? _a_pstd.repetition_data().print_repetition_tree() : "nullptr"
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}
