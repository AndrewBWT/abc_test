#pragma once
#include <exception>
#include <stacktrace>
#include <optional>
#include "abc_test/utility/internal/macros.h"
#include "fmt/ranges.h"

_BEGIN_ABC_ERRORS_NS
	/*!
	* Exception type for errors encountered when running the test harness. To be clear, these aren't
	* created by user code. Rather, thse are errors through inproper use, or developer errors.
	*/
	struct test_library_exception_t : public std::exception
	{
	public:
		/*!
		* For when a simple error has been created. Only contains a stacktrace.
		*/
		__constexpr
			test_library_exception_t(
				const std::stacktrace& _a_stacktrace = std::stacktrace::current()
			) noexcept;
		/*!
		* Simple constructor with a stactracek and a user-defined string describing the error.
		*/
		__constexpr
			test_library_exception_t(
				const std::string_view _a_error,
				const std::stacktrace& _a_stacktrace = std::stacktrace::current()
			) noexcept;
		/*!
		* Returns the underlying stacktrace of the test_harness_exception_t.
		*/
		__constexpr
			const std::stacktrace&
			stacktrace(
			) const noexcept;
		/*!
		* Returns the underlying string error of the test_harness_exception_t.
		*/
		__constexpr
			const std::string_view
			error(
			) noexcept;
	private:
		std::string _m_error;
		std::stacktrace _m_stacktrace;
	};
	__constexpr
		test_library_exception_t
		unsupported_mode_exception(
			const std::string_view _a_type,
			const std::size_t _a_mode,
			const std::initializer_list<std::size_t>& _a_supported_elements
		);
	_END_ABC_ERRORS_NS

		_BEGIN_ABC_ERRORS_NS
	__constexpr_imp
		test_library_exception_t::test_library_exception_t(
			const std::stacktrace& _a_stacktrace
		) noexcept
		: test_library_exception_t("",_a_stacktrace)
	{

	}
	__constexpr_imp
		test_library_exception_t::test_library_exception_t(
			const std::string_view _a_error,
			const std::stacktrace& _a_stacktrace
		) noexcept
		: _m_error(_a_error)
		, _m_stacktrace(_a_stacktrace)
	{

	}
	__constexpr_imp
		const std::stacktrace&
		test_library_exception_t::stacktrace(
		) const noexcept
	{
		return _m_stacktrace;
	}
	__constexpr_imp
		const std::string_view
		test_library_exception_t::error(
		) noexcept
	{
		return _m_error;
	}
	__constexpr_imp
		test_library_exception_t
		unsupported_mode_exception(
			const std::string_view _a_type,
			const std::size_t _a_mode,
			const std::initializer_list<std::size_t>& _a_supported_elements
		)
	{
		return test_library_exception_t(
			fmt::format(
				"{0} does not support mode {1}. {0} only supports the following modes: {2}",
				_a_type,_a_mode,_a_supported_elements
			)
		);
	}
	_END_ABC_ERRORS_NS