
#pragma once

#include "abc_test/core/ds/test_data/post_setup_test_data.h"

#include "abc_test/core/ds/repetitions/for_loop_data_collection.h"

#include <fmt/ranges.h>

_BEGIN_ABC_DS_NS
	/*!
	* Object used for tests that are currently being ran. Information pertaining to how they
	* perform is stored in instances of this object.
	*/
	struct invoked_test_info_t
	{
	public:
		/*!
		* Default constructor, shouldn't be used
		*/
		__no_constexpr
			invoked_test_info_t(
			) noexcept;
		/*!
		* The generic constructor. Generally this should be used.
		*/
		__no_constexpr
			invoked_test_info_t(
				const utility::seed_t& _a_seed_seq,
				const post_setup_test_data_t& _a_test_info,
				const size_t _a_order_ran_id,
				const test_options_t& _a_options
			) noexcept;
		/*!
		* Returns the path of the object. The path should be a specific, unique location where information
		* can be stored or retrieved for a specific test.
		*/
		__constexpr
			const std::filesystem::path&
			path(
			) const noexcept;
		/*!
		* Gets the tests random generator. Each test is given its own random generator, derived from the global
		* random generator.
		*/
		__constexpr
			utility::rng&
			get_random_generator(
			) noexcept;
		/*!
		* Checks whether the post_setup_test_data is valid.
		*/
		__constexpr
			bool
			has_post_setup_test_data(
			) const noexcept;
		/*!
		* Returns the post_setup_test_data object.
		* It is UB if the object's post_setup_test_data_t object is an invalid pointer.
		*/
		__constexpr
			const post_setup_test_data_t&
			post_setup_test_data(
			) const;
		/*!
		* Returns a reference to the internal for_loop_data_collection_t object.
		*/
		__constexpr
			ds::for_loop_data_collection_t&
			for_loop_data_collection(
			) noexcept;
		/*!
		* Gets the next set of iteration data from the underlying repetition data.
		*
		* Specifically, this is called when a new for loop is created. It gathers the repetition data
		* for that for loop and ensures that the for loop moves to the first element described in the
		* repetition.
		*/
		__constexpr
			std::optional<ds::repetition_data_t>
			get_repetition_iterator_data(
			) noexcept;
		/*!
		* Gets the next set of iteration data from the underlying repetition data.
		*
		* Specifically, this is called when iterating through a for loop. It analyses the current
		* repetition data sequence, and gets the next one for that for loop as described in the
		* repetition tree.
		*/
		__constexpr
			std::optional<ds::repetition_data_t>
			increment_repetition_iterator_data(
			) noexcept;
		/*!
		* Returns true if the current repetition_data_sequence is contained in the repetition_tree.
		*/
		__constexpr
			bool
			is_repetition_to_be_repeated(
			) const noexcept;
		/*!
		* Gets the internal repetition tree.
		*/
		__constexpr
			const ds::repetition_tree_t&
			repetition_tree(
			) const noexcept;
		/*!
		* Updates the current repetition tree.
		*/
		__constexpr
			void
			update_repetition_tree(
				const ds::repetition_data_sequence_t& _a_rds
			) noexcept;
		/*!
		* Getrs the current order_id of the invoked_test_data_t object.
		*/
		__constexpr
			std::size_t
			order_ran_id(
			) const noexcept;
	private:
		const post_setup_test_data_t* _m_post_setup_test_data;
		ds::repetition_tree_t _m_repetition_tree;
		ds::for_loop_data_collection_t _m_for_loop_data_collection;
		std::size_t _m_order_ran_id;
		utility::rng _m_this_tests_random_generator;
		std::filesystem::path _m_path;
		__no_constexpr
			invoked_test_info_t(
				const utility::seed_t& _a_seed_seq,
				const post_setup_test_data_t* _a_post_setup_test_data,
				const size_t _a_order_ran_id,
				const test_options_t& _a_options
			) noexcept;
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
		) noexcept
		: invoked_test_info_t(utility::seed_t(), nullptr, 0, test_options_t())
	{

	}
	__no_constexpr_imp
		invoked_test_info_t::invoked_test_info_t(
			const utility::seed_t& _a_seed_seq,
			const post_setup_test_data_t& _a_test_info,
			const size_t _a_order_ran_id,
			const test_options_t& _a_options
		) noexcept
		: invoked_test_info_t(_a_seed_seq, &_a_test_info, _a_order_ran_id, _a_options)
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
		utility::rng&
		invoked_test_info_t::get_random_generator(
		) noexcept
	{
		return _m_this_tests_random_generator;
	}
	__constexpr_imp
		bool
		invoked_test_info_t::has_post_setup_test_data(
		) const noexcept
	{
		return _m_post_setup_test_data != nullptr;
	}
	__constexpr_imp
		const post_setup_test_data_t&
		invoked_test_info_t::post_setup_test_data(
		) const
	{
		if (_m_post_setup_test_data == nullptr)
		{
			throw errors::test_library_exception_t(
				fmt::format("invoked_test_info_t does not have valid post_setup_test_data elemnet. _m_current_test = {0}",
					*this));
		}
		else
		{
			return *_m_post_setup_test_data;
		}
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
		const optional<repetition_data_t> _l_rv{ _m_post_setup_test_data == nullptr ? optional<repetition_data_t>{} :
			_m_post_setup_test_data->repetition_data().find_next_for_loop(_m_for_loop_data_collection) };
		//_LIBRARY_LOG(REPETITION_INFO, fmt::format("Given _a_new_for_loop argument of , "
		//	"Next repetition data obtained = {1}",
		//	(not _l_rv.has_value() ? "nullopt" :
		//	fmt::format("{0}", _l_rv.value()))));
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
		const optional<repetition_data_t> _l_rv{ _m_post_setup_test_data == nullptr ? optional<repetition_data_t>{} :
			_m_post_setup_test_data->repetition_data().increment_last_index(_m_for_loop_data_collection) };
		return _l_rv;
	}
	__constexpr_imp
		bool
		invoked_test_info_t::is_repetition_to_be_repeated(
		) const noexcept
	{
		return (_m_post_setup_test_data == nullptr) ? false :
			(not _m_post_setup_test_data->has_repetition_data()) ? true :
			_m_post_setup_test_data->repetition_data().is_repetition_to_be_repeated(_m_for_loop_data_collection);
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
		//_m_repetition_tree.add_repetition(_m_for_loop_data_collection.repetition_data_sequence(true));
		_LIBRARY_LOG(REPETITION_INFO, fmt::format("Repetition tree after insertion = {0}", _m_repetition_tree.print_repetition_tree()));
	}
	__constexpr_imp
		std::size_t
		invoked_test_info_t::order_ran_id(
		) const noexcept
	{
		return _m_order_ran_id;
	}
	__no_constexpr_imp
		invoked_test_info_t::invoked_test_info_t(
			const utility::seed_t& _a_seed_seq,
			const post_setup_test_data_t* _a_post_setup_test_data,
			const size_t _a_order_ran_id,
			const test_options_t& _a_options
		) noexcept
		: _m_post_setup_test_data(_a_post_setup_test_data)
		, _m_for_loop_data_collection(ds::for_loop_data_collection_t())
		, _m_order_ran_id(_a_order_ran_id)
		, _m_this_tests_random_generator(_a_seed_seq)
		, _m_path(_a_post_setup_test_data == nullptr ?
			std::filesystem::path() :
			create_test_path(*_a_post_setup_test_data, _a_options))
	{
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
			if (not _a_test_info.has_registered_test_data())
			{
				return path();
			}
			else
			{
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
	string _l_rv{ fmt::format(
		"invoked_test_info_t{{"
		"post_setup_test_data_t = {0}, "
		"repetition_tree_t = {1}, "
		"for_loop_data_collection_t = {2}, "
		"_m_order_ran_id = {3}, "
		"_m_this_tests_random_generator = {4}, "
		"_m_path = {5}}}",
		_a_iti.has_post_setup_test_data() ? fmt::format("{0}", _a_iti.post_setup_test_data()) : "nullptr", 
		_a_iti.repetition_tree(),
		_a_iti.for_loop_data_collection(),
		_a_iti.order_ran_id(),
		"random data generator not written yet",
		_a_iti.path().string()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}