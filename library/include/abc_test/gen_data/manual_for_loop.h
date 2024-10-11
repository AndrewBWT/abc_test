#pragma once

#include "abc_test/utility/internal/macros.h"
#include "abc_test/gen_data/collection.h"

#include "abc_test/gen_data/gen_data_base.h"

#include "abc_test/core/errors/test_library_exception.h"

/*!
* Code to run a manual for loop iteration. The manual_for_loop object is given as the first argument
* and the code to be ran as the second.
*/
#define RUN(manual_for_loop, code) if (manual_for_loop.run_next()) {code;} ++manual_for_loop;



_BEGIN_ABC_NS
	struct manual_for_loop_t
	{
	public:
		/*!
		* Default constructor for the manual_for_loop_t object.
		*/
		__constexpr
			manual_for_loop_t(
			) noexcept;
		/*!
		* Destructor for the object.
		* 
		* Behind the scenes, this function tidies up the for loop data which stores repetition data.
		*/
		__constexpr
			~manual_for_loop_t(
			) noexcept;
		/*!
		* Tells the caller whether the next test is to be ran.
		*/
		__constexpr
			bool
			run_next(
			);
		/*!
		* Increment element
		*/
		__constexpr
			manual_for_loop_t&
			operator++(
				);
		__constexpr
			manual_for_loop_t
			operator++(
				int
				);
	private:
		std::size_t _m_idx;
		bool _m_should_idx_be_ran;
		ds::for_loop_creation_data_t _m_repetition_data;
	};
_END_ABC_NS

_BEGIN_ABC_NS
	__constexpr_imp
		manual_for_loop_t::manual_for_loop_t(
		) noexcept
		: _m_idx{ 0 }
		, _m_should_idx_be_ran{ false }
		, _m_repetition_data(ds::for_loop_creation_data_t(0,0,0,std::to_string(0)))
	{
		using namespace global;
		using namespace ds;
		invoked_test_info_t& _l_current_test{ get_this_threads_current_test() };
		_l_current_test.for_loop_data_collection().increment(
			[&]() {return _m_repetition_data; },
			0,
			&_m_repetition_data);
		_m_should_idx_be_ran = _l_current_test.is_repetition_to_be_repeated();
	}
	__constexpr_imp
		manual_for_loop_t::~manual_for_loop_t(
		) noexcept
	{
		global::get_this_threads_current_test().for_loop_data_collection().decrement();
	}
	__constexpr_imp
		bool
		manual_for_loop_t::run_next(
		)
	{
		bool _l_rv{ _m_should_idx_be_ran };
		return _l_rv;
	}
	/*!
	* Increment element
	*/
	__constexpr_imp
		manual_for_loop_t&
		manual_for_loop_t::operator++(
			)
	{
		using namespace std;
		using namespace global;
		using namespace ds;
		_m_idx++;
		_m_repetition_data.set_string(to_string(_m_idx));
		invoked_test_info_t& _l_current_test{ get_this_threads_current_test()};
		_l_current_test.for_loop_data_collection().update(
			[&]() {return _m_repetition_data; },
			0,
			&_m_repetition_data);
		_m_should_idx_be_ran = _l_current_test.is_repetition_to_be_repeated();
		return *this;
	}
	/*!
	* Increment element
	*/
	__constexpr_imp
		manual_for_loop_t
		manual_for_loop_t::operator++(
			int
			)
	{
		auto _l_cpy{ *this };
		++(*this);
		return _l_cpy;
	}
	_END_ABC_NS