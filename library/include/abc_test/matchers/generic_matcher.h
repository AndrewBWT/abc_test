#pragma once
#include <optional>
#include <string>

//#include "abc_test/core/test_runner.h"
#include "abc_test/matchers/matcher_result.h"
#include <source_location>
#include <map>

#include "abc_test/matchers/source_map.h"


_BEGIN_ABC_NS
//Forward declaration
struct test_runner_t;
/*!
* Type synonym for the message contained in a matcher object.
*/
using matcher_msg_t = std::string;
/*!
* Type synonym for a reference to a matcher_msg, described above.
*/
using matcher_msg_ref_t = std::string_view;
struct generic_matcher_t
{
public:
	/*!
	* Default constructor
	*/
	__constexpr
		generic_matcher_t(
		) noexcept;
	__constexpr
		generic_matcher_t(
			const std::source_location& _a_source_location
		) noexcept;
	__constexpr
		generic_matcher_t(
			const std::string_view _a_str_representation,
			const std::source_location& _a_source_location
		) noexcept;
	/*!
	* Function to run the given test. This calls the virtual function run()
	*/
	__constexpr
		const matcher_result_t&
		run_test(
			test_runner_t& _a_test_runner
		);
	__constexpr
		virtual
		void
		gather_map_source(
			matcher_source_map_t& _a_matcher_source_map
		) const noexcept;
	__constexpr
		void
		add_source_info(
			const std::string_view _a_str_representation,
			const std::source_location& _a_source_location
		) noexcept;
	__constexpr
		void
		add_source_info(
			const std::source_location& _a_source_location
		) noexcept;
	/*!
	* Determines whether a test has passed or not.
	*/
	/*__constexpr
		bool
		passed(
		) const noexcept;
	__constexpr
		matcher_msg_ref_t
		get_failure_msg(
		) const noexcept;
	__constexpr
		const matcher_result_t&
		test_result(
		) const noexcept;*/
protected:
	matcher_result_t _m_test_result;
	std::optional<std::string> _m_str_representation;
	std::optional<std::source_location> _m_source_location;
private:
	__constexpr
		virtual
		matcher_result_t
		run(
			test_runner_t& _a_test_runner
		) = 0;
	__constexpr
		generic_matcher_t(
			const std::optional<std::string>& _a_str_representation,
			const std::optional<std::source_location>& _a_source_location
		) noexcept;
};
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	generic_matcher_t::generic_matcher_t(
	) noexcept
	: generic_matcher_t(std::optional<std::string>(),
		std::optional<std::source_location>())
{

}
__constexpr_imp
	generic_matcher_t::generic_matcher_t(
		const std::source_location& _a_source_location
	) noexcept
	: generic_matcher_t( std::optional<std::string>(),
		std::optional<std::source_location>(_a_source_location))
{

}
__constexpr_imp
	generic_matcher_t::generic_matcher_t(
		const std::string_view _a_str_representation,
		const std::source_location& _a_source_location
	) noexcept
	: generic_matcher_t(std::optional<std::string>(_a_str_representation),
		std::optional<std::source_location>(_a_source_location))
{

}
__constexpr_imp
	const matcher_result_t&
	generic_matcher_t::run_test(
		test_runner_t& _a_test_runner
	)
{
	if (not _m_test_result.ran())
	{
		_m_test_result = run(_a_test_runner);
	}
	return _m_test_result;
}
__constexpr_imp
	generic_matcher_t::generic_matcher_t(
		const std::optional<std::string>& _a_str_representation,
		const std::optional<std::source_location>& _a_source_location
	) noexcept
	: _m_str_representation(_a_str_representation)
	, _m_source_location(_a_source_location)
	, _m_test_result{ matcher_result_t{} }
{

}
__constexpr_imp
	void
	generic_matcher_t::gather_map_source(
		matcher_source_map_t& _a_matcher_source_map
	) const noexcept
{
	using namespace std;
	_a_matcher_source_map.insert(_m_source_location, _m_str_representation);
}
__constexpr_imp
	void
	generic_matcher_t::add_source_info(
		const std::string_view _a_str_representation,
		const std::source_location& _a_source_location
	) noexcept
{
	_m_str_representation = _a_str_representation;
	_m_source_location = _a_source_location;
}
__constexpr_imp
	void
	generic_matcher_t::add_source_info(
		const std::source_location& _a_source_location
	) noexcept
{
	_m_source_location = _a_source_location;
}
/*__constexpr_imp
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
	const matcher_result_t&
	generic_matcher_t::test_result(
	) const noexcept
{
	return _m_test_result;
}*/
_END_ABC_NS