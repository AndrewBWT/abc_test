#pragma once

#include <source_location>


#include "abc_test/core/errors/test_failure_info_enum.h"

#include "abc_test/utility/internal/macros.h"
#include <vector>
#include <string>

#include "abc_test/matchers/generic_matcher.h"
#include <map>

_BEGIN_ABC_ERRORS_NS
using str_collection_t = std::vector<std::string>;
/*!
* Object used to store error messages and auxillery information regarding the error's origin,
* and relevant logging data.
*/
struct test_failure_info_t
{
public:
	/*!
	* Constructor for assertions (either REQUIRE's or CHECK's).
	*
	*/
	__constexpr
		test_failure_info_t(
			const std::string_view _a_root_source_code_representation,
			const std::source_location& _a_source_location,
			const generic_matcher_t* _a_generic_matcher,
			const std::string_view _a_error_msg,
			const str_collection_t& _a_log_info,
			const bool _a_early_termination
		) noexcept;
	/*!
	* Constructor for programmer induced failure's (either FAIL or FAIL_CHECK).
	*
	*/
	__constexpr
		test_failure_info_t(
			const std::string_view _a_root_source_code_representation,
			const std::source_location& _a_source_location,
			const str_collection_t& _a_log_info,
			const bool _a_early_termination
		) noexcept;
	/*!
	* Constructor for unhandled exceptions (which inherit from std::exception)
	*
	*/
	__constexpr
		test_failure_info_t(
			const std::source_location& _a_source_location,
			const std::string_view _a_error_msg,
			const str_collection_t& _a_log_info,
			const std::string_view _a_type_id
		) noexcept;
	/*!
	* Constructor for unhandled exceptions (which do not inherit from std::exception).
	*
	*/
	__constexpr
		test_failure_info_t(
			const std::source_location& _a_source_location,
			const str_collection_t& _a_log_info
		) noexcept;
	/*!
	* Returns the error message (if the object has one), otherwise returns nullopt.
	*
	*/
	__constexpr
		const std::string_view
		error_msg(
		) const noexcept;
	/*!
	* Returns the line representation (if the object has one), otherwise returns nullopt.
	*
	*/
	__constexpr
		const std::string_view
		line_str_representation(
		) const noexcept;
	/*!
	* Returns the string form of the underlying exception's type_id (if the object has one),
	* otherwise returns nullopt.
	*/
	__constexpr
		const std::string_view
		type_id(
		) const noexcept;
	__constexpr
		const std::source_location
		source_location(
		) const noexcept;
	__constexpr
		const str_collection_t&
		log_info(
		) const noexcept;
	__constexpr
		test_failure_info_enum_t
		test_failure_info_enum(
		) const noexcept;
	__constexpr
		bool
		early_termination(
		) const noexcept;
	__constexpr
		const matcher_source_map_t&
		source_map(
		) const noexcept;
private:
	std::string _m_error_msg;
	std::string _m_line_str_representation;
	std::string _m_type_id;
	std::source_location _m_source_location;
	str_collection_t _m_log_info;
	test_failure_info_enum_t _m_test_failure_info_enum;
	bool _m_early_termination;
	matcher_source_map_t _m_matcher_source_map;
	__constexpr
		test_failure_info_t(
			const std::string_view _a_line_str_representation,
			const std::source_location& _a_source_location,
			const generic_matcher_t* _a_generic_matcher,
			const std::string_view _a_error_msg,
			const str_collection_t& _a_log_info,
			const test_failure_info_enum_t _a_test_failure_info_enum,
			const bool _a_early_termination,
			const std::string_view _a_type_id
		) noexcept;
};
_END_ABC_ERRORS_NS

_BEGIN_ABC_ERRORS_NS
__constexpr_imp
	test_failure_info_t::test_failure_info_t(
		const std::string_view _a_line_str_representation,
		const std::source_location& _a_source_location,
		const generic_matcher_t* _a_generic_matcher,
		const std::string_view _a_error_msg,
		const str_collection_t& _a_log_info,
		const bool _a_early_termination
	) noexcept
	: test_failure_info_t(
		_a_line_str_representation,
		_a_source_location,
		_a_generic_matcher,
		_a_error_msg,
		_a_log_info,
		test_failure_info_enum_t::TEST_FAILURE,
		_a_early_termination,
		std::string{}
	)
{
}
__constexpr_imp
	test_failure_info_t::test_failure_info_t(
		const std::string_view _a_line_str_representation,
		const std::source_location& _a_source_location,
		const str_collection_t& _a_log_info,
		const bool _a_early_termination
	) noexcept
	: test_failure_info_t(
		_a_line_str_representation,
		_a_source_location,
		nullptr,
		std::string{},
		_a_log_info,
		test_failure_info_enum_t::MANUAL_FAILURE,
		_a_early_termination,
		std::string{}
	)
{
}
__constexpr_imp
	test_failure_info_t::test_failure_info_t(
		const std::source_location& _a_source_location,
		const std::string_view _a_error_msg,
		const str_collection_t& _a_log_info,
		const std::string_view _a_type_id
	) noexcept
	: test_failure_info_t(
		std::string{},
		_a_source_location,
		nullptr,
		_a_error_msg,
		_a_log_info,
		test_failure_info_enum_t::UNHANDLED_EXCEPTION,
		true,
		_a_type_id
	)
{
}
__constexpr_imp
	test_failure_info_t::test_failure_info_t(
		const std::source_location& _a_source_location,
		const str_collection_t& _a_log_info
	) noexcept
	: test_failure_info_t(
		std::string{},
		_a_source_location,
		nullptr,
		std::string{},
		_a_log_info,
		test_failure_info_enum_t::UNHADLED_EXCEPTION_NOT_DERIVED_FROM_EXCEPTION,
		true,
		std::string{}
	)
{

}
__constexpr_imp
	const std::string_view
	test_failure_info_t::error_msg(
	) const noexcept
{
	return _m_error_msg;
}
__constexpr_imp
	const std::string_view
	test_failure_info_t::line_str_representation(
	) const noexcept
{
	return _m_line_str_representation;
}
__constexpr_imp
	const std::string_view
	test_failure_info_t::type_id(
	) const noexcept
{
	return _m_type_id;
}
__constexpr_imp
	const std::source_location
	test_failure_info_t::source_location(
	) const noexcept
{
	return _m_source_location;
}
__constexpr_imp
	const str_collection_t&
	test_failure_info_t::log_info(
	) const noexcept
{
	return _m_log_info;
}
__constexpr_imp
	test_failure_info_enum_t
	test_failure_info_t::test_failure_info_enum(
	) const noexcept
{
	return _m_test_failure_info_enum;
}
__constexpr_imp
	bool
	test_failure_info_t::early_termination(
	) const noexcept
{
	return _m_early_termination;
}
__constexpr_imp
	const matcher_source_map_t&
	test_failure_info_t::source_map(
	) const noexcept
{
	return _m_matcher_source_map;
}
__constexpr_imp
	test_failure_info_t::test_failure_info_t(
		const std::string_view _a_line_str_representation,
		const std::source_location& _a_source_location,
		const generic_matcher_t* _a_generic_matcher,
		const std::string_view _a_error_msg,
		const str_collection_t& _a_log_info,
		const test_failure_info_enum_t _a_test_failure_info_enum,
		const bool _a_early_termination,
		const std::string_view _a_type_id
	) noexcept
	: _m_line_str_representation(_a_line_str_representation)
	, _m_source_location(_a_source_location)
	, _m_error_msg(_a_error_msg)
	, _m_log_info(_a_log_info)
	, _m_test_failure_info_enum(_a_test_failure_info_enum)
	, _m_early_termination(_a_early_termination)
	, _m_type_id(_a_type_id)
	, _m_matcher_source_map(matcher_source_map_t())
{
	if (_a_generic_matcher != nullptr)
	{
		_a_generic_matcher->gather_map_source(_m_matcher_source_map);
	}
}
_END_ABC_ERRORS_NS