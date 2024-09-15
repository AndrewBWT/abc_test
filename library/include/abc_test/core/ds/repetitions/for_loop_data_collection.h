#pragma once

#include "abc_test/core/ds/repetitions/for_loop_data.h"
#include "abc_test/core/errors/test_library_exception.h"
#include "fmt/base.h"

#include "fmt/ranges.h"


_BEGIN_ABC_DS_NS
	/*!
	* Object which holds ma colleciton of for loop data, which allows us to represent an arbitrarily deep
	* set of gen_data for loops. These mechanisms allows tests to be re-run effeciently, without having
	* to go through additional use-cases. At any point, a current test will have a stack of for loops
	* it is navigating with distinct values generated from those for-loops. This object encodes that data
	*/
	struct for_loop_data_collection_t
	{
	public:
		/*!
		* Default constructor
		*/
		__constexpr
			for_loop_data_collection_t(
			) noexcept;
		/*!
		* Function adds an element to the internal stack.
		* 
		* Arguments are the on_error_function for the current for loop value,
		* the current generation_collection index, and a pointer to the repetition data.
		* 
		* We use the pointer as we cannot have a pointer/reference to the gen_data object, as it
		* would have a template parameter we cannot reasonably be expected to know. This way,
		* we can retain enough information to encode any repetition data. 
		*/
		__constexpr
			void
			increment(
				const test_failure_func_t _a_on_error_function,
				const size_t _a_generation_collection_index,
				repetition_data_t* _a_repetition_data
			) noexcept;
		/*!
		* Removes the current element from the for loop stack, and updates the for loop counter.
		*/
		__constexpr
			void
			decrement(
			);
		/*!
		* Updates the last elemnet in the internal stack with the given data.
		*/
		__constexpr
			void
			update(
				const test_failure_func_t _a_on_error_function,
				const size_t _a_generation_collection_index,
				repetition_data_t* _a_repetition_data
			);
		/*!
		* Calls all the for loop element's failure functions.
		* 
		* Also generates the repetition_data_sequence_t object for this specific error. This in turn is 
		* added to the current tests repetition data, which in turn is delivered to the user to aid in
		* debugging failing test cases.
		*/
		__constexpr
			ds::repetition_data_sequence_t
			inform_test_generators_of_failure(
			) const noexcept;
		/*!
		* Builds a repetition sequence from the data contained within.
		*/
		__constexpr
			repetition_data_sequence_t
			repetition_data_sequence(
			) const noexcept;
		/*!
		* Gets a reference to the underlying for_loop_data_t vector.
		*/
		__constexpr
			const std::vector<for_loop_data_t>&
			for_loop_data_collection(
			) const noexcept;
		/*!
		* Gets the current for loop index. This value represents the next value any new for loop
		* should take in the current stack of for loops; making a new for loop will reset this back to zero,
		* and unwinding the current for loop will take the previous index and increment it.
		*/
		__constexpr
			std::size_t
			current_for_loop_index(
			) const noexcept;
	private:
		std::vector<for_loop_data_t> _m_for_loop_data_collection;
		std::size_t _m_current_for_loop_index;
	};
_END_ABC_DS_NS

template
<
>
struct fmt::formatter<abc::ds::for_loop_data_collection_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__constexpr
		auto
		format(
			abc::ds::for_loop_data_collection_t _a_rt,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
	__constexpr_imp
		for_loop_data_collection_t::for_loop_data_collection_t(
		) noexcept
		: _m_for_loop_data_collection(std::vector<for_loop_data_t>())
		, _m_current_for_loop_index(0)
	{

	}
	__constexpr_imp
		void
		for_loop_data_collection_t::increment(
			const test_failure_func_t _a_on_error_function,
			const size_t _a_generation_collection_index,
			repetition_data_t* _a_repetition_data
		) noexcept
	{
		_m_for_loop_data_collection.push_back(
			for_loop_data_t(_m_current_for_loop_index, _a_on_error_function, _a_generation_collection_index,
				_a_repetition_data));
		_m_current_for_loop_index = 0;
	}
	__constexpr_imp
		void
		for_loop_data_collection_t::decrement(
		) 
	{
		using namespace errors;
		if (_m_for_loop_data_collection.size() > 0)
		{
			_m_current_for_loop_index = (_m_for_loop_data_collection.back().for_loop_index()) + 1;
			_m_for_loop_data_collection.pop_back();
		}
		else
		{
			throw test_library_exception_t(
				fmt::format("decrement called when _m_for_loop_data_collection size == 0"));
		}
	}
	__constexpr_imp
		void
		for_loop_data_collection_t::update(
			const test_failure_func_t _a_on_error_function,
			const size_t _a_generation_collection_index,
			repetition_data_t* _a_repetition_data
		//	const std::size_t _a_mode,
		//	const std::string_view _a_additional_data
		)
	{
		using namespace errors;
		if (_m_for_loop_data_collection.size() > 0)
		{
			_m_for_loop_data_collection.back().update(
				_a_on_error_function, _a_generation_collection_index, _a_repetition_data);
			//	_a_on_error_function, _a_generation_collection_index, _a_mode,_a_additional_data);
			_m_current_for_loop_index = 0;
		}
		else
		{
			throw test_library_exception_t(
				fmt::format("update called when _m_for_loop_data_collection size == 0"));
		}
	}
	__constexpr_imp
		ds::repetition_data_sequence_t
		for_loop_data_collection_t::inform_test_generators_of_failure(
		) const noexcept
	{
		using namespace std;
		using namespace ds;
		repetition_data_sequence_t _l_rds{};
		for (const for_loop_data_t& _l_fld : _m_for_loop_data_collection)
		{
			auto k = _l_fld.inform_test_generator_of_failure_and_generate_alternate_repetition_data();
			_l_rds.push_back(_l_fld.inform_test_generator_of_failure_and_generate_alternate_repetition_data());
		}
		return _l_rds;
	}
	__constexpr_imp
		repetition_data_sequence_t
		for_loop_data_collection_t::repetition_data_sequence(
		) const noexcept
	{
		repetition_data_sequence_t _l_rds{};
		for (const for_loop_data_t& _l_fld : _m_for_loop_data_collection)
		{
			_l_rds.push_back(_l_fld.repetition_data());
		}
		return _l_rds;
	}
	__constexpr_imp
		const std::vector<for_loop_data_t>&
		for_loop_data_collection_t::for_loop_data_collection(
		) const noexcept
	{
		return _m_for_loop_data_collection;
	}
	__constexpr_imp
		std::size_t
		for_loop_data_collection_t::current_for_loop_index(
		) const noexcept
	{
		return _m_current_for_loop_index;
	}
	_END_ABC_DS_NS

__constexpr_imp
auto
fmt::formatter<abc::ds::for_loop_data_collection_t>::format(
	abc::ds::for_loop_data_collection_t _a_fldc,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_rv{ fmt::format(
		"for_loop_data_collection_t {{"
		"_m_for_loop_data_collection = {0},"
		"_m_current_for_loop_index = {1}}}",
		_a_fldc.for_loop_data_collection(),
		_a_fldc.current_for_loop_index()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}