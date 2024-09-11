#pragma once

#include <functional>
#include "abc_test/core/ds/repetitions/repetition_data.h"
#include "fmt/base.h"

_BEGIN_ABC_DS_NS
	/*!
	* Structure to contain for loop data, itself used for storing data used when repeating
	* parts of tests created around the gen_data data type. 
	* 
	* Each element holds an index (representing the element generated from the for loop).
	* A function which is called if a failure is encountered; typically these ensure that data regarding
	* this repetition is retained in a data structure.
	* repetition_data. This holds the specific set of choices made to get to this test.
	*/
	struct for_loop_data_t
	{
	public:
		/*!
		* Main constructor. This is the one that should be used by the user.
		*/
		__constexpr
			for_loop_data_t(
				const std::size_t _a_for_loop_index,
				const std::function<repetition_data_t()> _a_on_error_function,
				const size_t _a_generation_collection_index,
				repetition_data_t* _a_repetition_data
			) noexcept;
		/*!
		* Default constructor. _m_for_loop_index is set at zero, funciton is void and repetition_data is empty.
		*/
		__constexpr
			for_loop_data_t(
			) noexcept;
		/*!
		* Gets the for lopo index of the object
		*/
		__constexpr
			std::size_t
			for_loop_index(
			) const noexcept;
		/*!
		* Gets the repeition data of the object
		*/
		__constexpr
			const repetition_data_t&
			repetition_data(
			) const noexcept;
		/*!
		* Updates the object using the given data.
		* 
		* The on_error_function provides the new repetition_data_t object, which should encode the alternate
		* repetition data for rewriting the test (assuming it fails). By having al alternate encoding, 
		* it is possible to provide a "short cut" when re-running a test. For example, when using random
		* data to generate some test value, by writing that value to a file, it can be re-run. In future tests,
		* it is then possible to re-run these "problematic" values first, while using a different random seed
		* to run different tests. 
		*/
		__constexpr
			void
			update(
				const std::function<repetition_data_t()> _a_on_error_function,
				const std::size_t _a_generation_collection_index,
				repetition_data_t* _a_repetition_data
			) noexcept;
		/*!
		* Calls the on_failure_function of the object. It also returns the repetition_data_t object
		* which can be used to re-run the test. The return object encodes information about how to re-run
		* the test, not the specific circumstances concerning how the test was ran in this instance.
		* 
		*/
		__constexpr
			const ds::repetition_data_t
			inform_test_generator_of_failure_and_generate_alternate_repetition_data(
			) const noexcept;
	private:
		std::size_t _m_for_loop_index;
		std::function<repetition_data_t()> _m_on_failure_function;
		repetition_data_t* _m_repetition_data;
	};
_END_ABC_DS_NS

template
<
>
struct fmt::formatter<abc::ds::for_loop_data_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__constexpr
		auto
		format(
			abc::ds::for_loop_data_t _a_fld,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
	__constexpr_imp
		for_loop_data_t::for_loop_data_t(
		) noexcept
		: for_loop_data_t(0, std::function<repetition_data_t()>{}, 0, nullptr)
	{

	}
	__constexpr_imp
		for_loop_data_t::for_loop_data_t(
			const std::size_t _a_for_loop_index,
			const std::function<repetition_data_t()> _a_on_error_function,
			const size_t _a_generation_collection_index,
			repetition_data_t* _a_repetition_data
		) noexcept
		: _m_for_loop_index(_a_for_loop_index)
		, _m_on_failure_function(_a_on_error_function)
		, _m_repetition_data(_a_repetition_data)
	{
		_m_repetition_data->set_for_loop_and_generation_collection_indexs(_a_for_loop_index, _a_generation_collection_index);
	}
	__constexpr_imp
		std::size_t
		for_loop_data_t::for_loop_index(
		) const noexcept
	{
		return _m_for_loop_index;
	}
	__constexpr_imp
		const repetition_data_t&
		for_loop_data_t::repetition_data(
		) const noexcept
	{
		return *_m_repetition_data;
	}
	__constexpr_imp
		void
		for_loop_data_t::update(
			const std::function<repetition_data_t()> _a_on_error_function,
			const std::size_t _a_generation_collection_index,
			repetition_data_t* _a_repetition_data
		) noexcept
	{
		using namespace std;
		_m_on_failure_function = _a_on_error_function;
		const size_t _l_for_loop_index{ _m_repetition_data->for_loop_index() };
		_m_repetition_data = _a_repetition_data;
		_m_repetition_data->set_for_loop_and_generation_collection_indexs(_l_for_loop_index, _a_generation_collection_index);
	}
	__constexpr_imp
		const ds::repetition_data_t
		for_loop_data_t::inform_test_generator_of_failure_and_generate_alternate_repetition_data(
		) const noexcept
	{
		return _m_on_failure_function();
	}
	_END_ABC_DS_NS

__constexpr_imp
auto
fmt::formatter<abc::ds::for_loop_data_t>::format(
	abc::ds::for_loop_data_t _a_fld,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_rv{ fmt::format(
		"for_loop_data_t {{"
		"_m_for_loop_index = {0},"
		"_m_repetition_data = {1}}}",
		_a_fld.for_loop_index(),
		_a_fld.repetition_data()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}