#pragma once
#include "abc_test/utility/internal/macros.h"
#include <source_location>
#include <stacktrace>

_BEGIN_ABC_ERRORS_NS
	/*!
	* Object which holds and catalogues internal errors; things which should be logged to the user.
	*/
	struct setup_error_t
	{
	public:
		/*!
		* Default constructor. Represents non-failure, or success.
		*/
		__constexpr
			setup_error_t(
			) noexcept;
		/*!
		* Generic failure constructor.
		*/
		__constexpr
			setup_error_t(
				const std::string_view _a_error_msg,
				const bool _a_unrecoverable_error,
				const std::source_location& _a_source_location = std::source_location::current()
			) noexcept;
		__constexpr
			setup_error_t(
				const std::string_view _a_error_msg,
				const bool _a_unrecoverable_error,
				const std::stacktrace& _a_stacktrace,
				const std::source_location& _a_source_location = std::source_location::current()
			) noexcept;
		/*!
		* Checks whether the object contains an error or not.
		*/
		__constexpr
			bool
			has_error(
			) const noexcept;
		/*!
		* Checks whether the error is unrecoverable.
		*/
		__constexpr
			bool
			unrecoverable_error(
			) const noexcept;
		__constexpr
			const std::string_view
			error_msg(
			) const noexcept;
		__constexpr
			const std::optional<std::stacktrace>&
			opt_stacktrace(
			) const noexcept;
		__constexpr
			const std::source_location&
			source_location(
			) const noexcept;
	private:
		std::string _m_error_msg;
		bool _m_unrecoverable_error;
		bool _m_has_error;
		std::optional<std::stacktrace> _m_opt_stacktrace;
		std::source_location _m_source_location;
		__constexpr
			setup_error_t(
				const std::string_view _a_error_msg,
				const bool _a_unrecoverable_error,
				const bool _a_has_error,
				const std::optional<std::stacktrace>& _a_opt_stacktrace,
				const std::source_location& _a_source_location
			) noexcept;
	};
	using opt_setup_error_t = std::optional<setup_error_t>;
	_END_ABC_ERRORS_NS

		_BEGIN_ABC_ERRORS_NS
	__constexpr_imp
		setup_error_t::setup_error_t(
		) noexcept
		: setup_error_t(std::string{},false,false, std::optional<std::stacktrace>{}, std::source_location())
	{

	}
	__constexpr_imp
		setup_error_t::setup_error_t(
			const std::string_view _a_error_msg,
			const bool _a_unrecoverable_error,
			const std::source_location & _a_source_location
		) noexcept
		: setup_error_t(_a_error_msg, _a_unrecoverable_error, true, std::optional<std::stacktrace>{},_a_source_location)
	{

	}
	__constexpr_imp
		setup_error_t::setup_error_t(
			const std::string_view _a_error_msg,
			const bool _a_unrecoverable_error,
			const std::stacktrace& _a_stacktrace,
			const std::source_location& _a_source_location
		) noexcept
		: setup_error_t(_a_error_msg, _a_unrecoverable_error, true, _a_stacktrace,_a_source_location)
	{

	}
	__constexpr_imp
		bool
		setup_error_t::has_error(
		) const noexcept
	{
		return _m_has_error;
	}
	__constexpr_imp
		bool
		setup_error_t::unrecoverable_error(
		) const noexcept
	{
		return _m_unrecoverable_error;
	}
	__constexpr_imp
		const std::string_view
		setup_error_t::error_msg(
		) const noexcept
	{
		return _m_error_msg;
	}
	__constexpr_imp
		const std::optional<std::stacktrace>&
		setup_error_t::opt_stacktrace(
		) const noexcept
	{
		return _m_opt_stacktrace;
	}
	__constexpr_imp
		const std::source_location&
		setup_error_t::source_location(
		) const noexcept
	{
		return _m_source_location;
	}
	__constexpr_imp
		setup_error_t::setup_error_t(
			const std::string_view _a_error_msg,
			const bool _a_unrecoverable_error,
			const bool _a_has_error,
			const std::optional<std::stacktrace>& _a_opt_stacktrace,
			const std::source_location& _a_source_location
		) noexcept
		: _m_error_msg(_a_error_msg)
		, _m_unrecoverable_error(_a_unrecoverable_error)
		, _m_has_error(_a_has_error)
		, _m_opt_stacktrace(_a_opt_stacktrace)
		, _m_source_location(_a_source_location)
	{

	}
	_END_ABC_ERRORS_NS