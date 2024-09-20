#pragma once
#include <memory>
#include "abc_test/core/reporters/test_reporter_controller.h"


#include "abc_test/core/test_order_enum.h"

#include "abc_test/core/reporters/after_execution_test_report.h"
#include "abc_test/core/reporters/mid_execution_test_report.h"
#include "abc_test/core/reporters/mid_execution_test_warning.h"


_BEGIN_ABC_NS
struct log_test_msg_t;
//Forward declarations
struct ds::registered_test_data_t;
/*!
* Object which controls the running of tests.
*/
struct test_runner_t
{
public:
	/*!
	* Main constructor used; initialises the test_reporter controller and global test_options
	*/
	__constexpr
		test_runner_t(
			reporters::test_reporter_controller_t& _a_trc,
			const test_options_t& _a_test_options
		) noexcept;
	/*!
	* Registers a source with the object, used when showing information pertaining to errors to the user.
	*/
	__constexpr
		void
		register_tests_most_recent_source(
			const std::source_location& _a_source_location
		) noexcept;
	/*!
	* When a test failure is encountered, this function is used to add that error to a list of current errors.
	*/
	//__constexpr
	//	void
	//	add_error(
	//		const errors::test_failure_info_t& _a_str
	//	) noexcept;
	/*!
	* Used to log information to the test_runner. Returned is an iterator pointiong to the element added.
	*/
	__constexpr
		std::list<const log_test_msg_t*>::iterator
		add_error_info(
			const log_test_msg_t* _a_log
		) noexcept;
	/*!
	* Removes an element from the current log list.
	*/
	__no_constexpr_or_inline
		void
		remove_error_info(
			std::list<const log_test_msg_t*>::iterator _a_itt
		) noexcept;
	constexpr
		void
		add_cached_exception(
			const std::string_view _a_str
		) noexcept
	{
		using namespace std;
		_m_cached_log_msgs.push_back(string(_a_str));
	}
	/*!
	* Gets text representation of all the logs.
	*/
	__no_constexpr_or_inline
		std::vector<std::string>
		get_log_infos(
			const bool _a_get_cached_results
		) noexcept;
	//Can't be constexpr as body must be in cpp file
	/*!
	* Runs a test.
	* 
	* This is the 'hook' from the 'other side'; that is to say, this can be considered the "main"
	* when testing a function.
	*/
	__no_constexpr_or_inline
		void
		run_test(
			const ds::post_setup_test_data_t& _a_post_setup_test_data
		);
	/*!
	* Tells the user how many tests have been ran by this test_runner.
	* 
	* Remember that, due to multi-threading, this is only this instance of test_runner.
	*/
	__constexpr
		std::size_t
		tests_ran(
		) const noexcept;
	/*!
	* Returns to the user the current invoked test for this object instance.
	*/
	__constexpr
		ds::invoked_test_info_t&
		current_test(
		) noexcept;
	/*!
	* Generates a random seed.
	*/
	__constexpr
		utility::seed_t
		generate_random_seeds(
		) noexcept;
	__constexpr
		const std::source_location&
		most_recent_source(
		) const noexcept;
	__constexpr
		void
		add_mid_execution_test_report(
			reporters::mid_execution_test_report_t* _a_metr
		) noexcept;
	__constexpr
		void
		add_mid_execution_test_warning(
			reporters::mid_execution_test_warning_t* _a_metr
		) noexcept;
private:
	std::list<const log_test_msg_t*> _m_current_error_log_msgs;
	std::vector<std::string> _m_cached_log_msgs;
	reporters::after_execution_test_report_t _m_after_execution_test_report;
	//errors::test_failures_info_t _m_error_infos;
	reporters::test_reporter_controller_t* _m_trc;
	std::size_t _m_tests_ran;
	ds::invoked_test_info_t _m_current_test;
	utility::rng _m_random_generator;
	test_order_enum_t _m_test_order;
	const test_options_t& _m_test_options;
	std::source_location _m_tests_most_recent_source;
};
_END_ABC_NS

_BEGIN_ABC_NS
	__constexpr_imp
		test_runner_t::test_runner_t(
			reporters::test_reporter_controller_t& _a_trc,
			const test_options_t& _a_test_options
		) noexcept
		: _m_after_execution_test_report(reporters::after_execution_test_report_t{})
		, _m_trc(&_a_trc)
		, _m_tests_ran{ 0 }
		, _m_current_test(ds::invoked_test_info_t())
		, _m_random_generator(utility::rng(_a_test_options._m_seed_values))
		, _m_test_order(test_order_enum_t::IN_ORDER)
		, _m_test_options(_a_test_options)
	{

	}

	__constexpr_imp
		void
		test_runner_t::register_tests_most_recent_source(
			const std::source_location& _a_source_location
		) noexcept
	{
		_m_tests_most_recent_source = _a_source_location;
	}
	/*__constexpr_imp
		void
		test_runner_t::add_error(
			const errors::test_failure_info_t& _a_str
		) noexcept
	{
		using namespace std;
		using namespace ds;
		_m_after_execution_test_report
		_m_error_infos.add_error_msg(_a_str);
		vector<repetition_data_t> _l_rds{ _m_current_test.for_loop_data_collection().inform_test_generators_of_failure() };
		_m_current_test.update_repetition_tree(_l_rds);
	}*/
	__constexpr_imp
		std::list<const log_test_msg_t*>::iterator
		test_runner_t::add_error_info(
			const log_test_msg_t* _a_log
		) noexcept
	{
		using namespace std;
		_m_current_error_log_msgs.push_back(_a_log);
		return --_m_current_error_log_msgs.end();
	}
	__constexpr_imp
		size_t
		test_runner_t::tests_ran(
		) const noexcept
	{
		return _m_tests_ran;
	}
	__constexpr_imp
		ds::invoked_test_info_t&
		test_runner_t::current_test(
		) noexcept
	{
		return _m_current_test;
	}
	__constexpr_imp
		utility::seed_t
		test_runner_t::generate_random_seeds(
		) noexcept
	{
		using namespace utility;
		const size_t _l_n_elements_used_to_seed_random_generators{ _m_test_options._m_numb_elements_used_to_seed_random_generators };
		seed_t _l_seed(_l_n_elements_used_to_seed_random_generators);
		for (size_t _l_idx{ 0 }; _l_idx < _l_n_elements_used_to_seed_random_generators; ++_l_idx)
		{
			_l_seed[_l_idx] = _m_random_generator();
		}
		return _l_seed;
	}
__constexpr_imp
	const std::source_location&
	test_runner_t::most_recent_source(
	) const noexcept
{
	return _m_tests_most_recent_source;
}
__constexpr_imp
	void
	test_runner_t::add_mid_execution_test_report(
		reporters::mid_execution_test_report_t* _a_metr
	) noexcept
{
	using namespace std;
	using namespace ds;
	vector<repetition_data_t> _l_rds{ _m_current_test.for_loop_data_collection().inform_test_generators_of_failure() };
	_m_current_test.update_repetition_tree(_l_rds);
	_m_after_execution_test_report.add_mid_execution_test_report(_a_metr);
}
__constexpr_imp
	void
	test_runner_t::add_mid_execution_test_warning(
		reporters::mid_execution_test_warning_t* _a_metr
	) noexcept
{
	_m_after_execution_test_report.add_mid_execution_warning(_a_metr);
}
_END_ABC_NS