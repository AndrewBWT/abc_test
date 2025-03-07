#pragma once

#pragma once

#include "abc_test/utility/io/threaded_text_output_reporter.hpp"
#include "abc_test/core/reporters/error_reporter.hpp"


_BEGIN_ABC_REPORTERS_NS
	/*!
	* Object used to print data about tests to some text output - either the console or a file
	*/
	struct text_error_reporter_t : 
		public error_reporter_t, 
		public utility::io::threated_text_output_reporter_t
	{
	public:
		/*!
		Constructor for std::cerr to be where the output is posted.
		*/
		__no_constexpr
			text_error_reporter_t(
			) noexcept;
		/*!
		Constructor whcih allows a file to be where the output is posted.
		*/
		__no_constexpr
			text_error_reporter_t(
				const std::filesystem::path& _a_file_output
			) noexcept;
		/*!
		* Describes how to deal with an error_t being reported.
		*/
		__no_constexpr
			virtual
			void
			report_error(
				const errors::setup_error_t& _a_error
			) const noexcept;
		/*!
		* Function for dealing with simple information; in this case, it is streamed to the output.
		*/
		__no_constexpr
			virtual
			void
			report_information(
				const std::string_view _a_str
			) const noexcept;
		/*!
		* What to do if the test suite is terminated - e.g. any cleaning up
		*/
		__no_constexpr
			virtual
			void
			exit(
			) const noexcept;
	};
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
	__no_constexpr_imp
		text_error_reporter_t::text_error_reporter_t(
		) noexcept
		: threated_text_output_reporter_t(std::cerr)
	{

	}
__no_constexpr_imp
		text_error_reporter_t::text_error_reporter_t(
			const std::filesystem::path& _a_file_output
		) noexcept
		: threated_text_output_reporter_t(_a_file_output)
	{

	}
__no_constexpr_imp
		void
		text_error_reporter_t::report_error(
			const errors::setup_error_t& _a_setup_test_error
		) const noexcept
	{
		write(fmt::format(
			"ERROR:\n"
			"   Error Message: \"{0}\"\n"
			"   Source of error:\n"
			"      File: {1}\n"
			"      Function: {2}\n"
			"      Line: {3}\n"
			"   Stacktrace: {4}"
			, _a_setup_test_error.error_msg()
			, _a_setup_test_error.source_location().file_name()
			, _a_setup_test_error.source_location().function_name()
			, _a_setup_test_error.source_location().line()
			, _a_setup_test_error.opt_stacktrace().has_value() ?
			 to_string(_a_setup_test_error.opt_stacktrace().value()) :
			"No stacktrace"
			));
	}
__no_constexpr_imp
		void
		text_error_reporter_t::report_information(
			const std::string_view _a_str
		) const noexcept
	{
		write("WARNING: " + std::string(_a_str));
	}
__no_constexpr_imp
		void
		text_error_reporter_t::exit(
		) const noexcept
	{
		write("Preparing to exit...");
	}
	_END_ABC_REPORTERS_NS