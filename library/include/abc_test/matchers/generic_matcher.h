#pragma once
#include <optional>
#include <string>

#include "abc_test/core/test_runner.h"
#include "abc_test/matchers/matcher_result.h"

_BEGIN_ABC_NS
/*!
* Type synonym for the message contained in a matcher object.
*/
using matcher_msg_t = std::string;
/*!
* Type synonym for a reference to a matcher_msg, described above.
*/
using matcher_msg_ref_t = std::string_view;
using matcher_result_const_ref_t = const matcher_result_t&;
struct generic_matcher_t
{
public:
	/*!
	* Default constructor
	*/
	__constexpr
		generic_matcher_t(
		) noexcept;
	/*!
	* Function to run the given test. This calls the virtual function run()
	*/
	__constexpr
		void
		run_test(
			test_runner_ref_t _a_test_runner
		) noexcept;
	/*!
	* Determines whether a test has passed or not.
	*/
	__constexpr
		bool
		passed(
		) const noexcept;
	/*!
	* Gets a reference to the error message if the test failed. 
	* 
	* This function assumes that the test failed.
	*/
	__constexpr
		matcher_msg_ref_t
		get_failure_msg(
		) const noexcept;
	__constexpr
		matcher_result_const_ref_t
		test_result(
		) const noexcept;
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_ref_t
		) = 0;
protected:
	matcher_result_t _m_test_result;
};
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	generic_matcher_t::generic_matcher_t(
	) noexcept
	: _m_test_result{ matcher_result_t{} }
{

}
__constexpr_imp
	void
	generic_matcher_t::run_test(
		test_runner_ref_t _a_test_runner
	) noexcept
{
	_m_test_result = run(_a_test_runner);
}
__constexpr_imp
	bool
	generic_matcher_t::passed(
	) const noexcept
{
	return _m_test_result.passed();
}
__constexpr_imp
	matcher_msg_ref_t
	generic_matcher_t::get_failure_msg(
	) const noexcept
{
	return _m_test_result.str();
}
__constexpr_imp
	matcher_result_const_ref_t
	generic_matcher_t::test_result(
	) const noexcept
{
	return _m_test_result;
}
_END_ABC_NS