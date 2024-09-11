#pragma once

#include "abc_test/utility/internal/macros.h"
#include <syncstream>

#include <fstream>

_BEGIN_ABC_UTILITY_IO_NS
	struct threaded_ostream_output_reporter_t
	{
	public:
		__constexpr
			threaded_ostream_output_reporter_t(
			) noexcept = delete;
		__constexpr
			threaded_ostream_output_reporter_t(
				std::ostream& _a_ostream
			) noexcept;
		__constexpr
			void
			write(
				const std::string_view _a_line
			) noexcept;
	protected:
		std::osyncstream _m_output_stream;
	};
_END_ABC_NS
_END_NS

_BEGIN_ABC_UTILITY_IO_NS
	__constexpr_imp
		threaded_ostream_output_reporter_t::threaded_ostream_output_reporter_t(
			std::ostream& _a_ostream
		) noexcept
		: _m_output_stream(_a_ostream)
	{

	}
	__constexpr_imp
		void
		threaded_ostream_output_reporter_t::write(
			const std::string_view _a_line
		) noexcept
	{
		using namespace std;
		_m_output_stream << _a_line << endl;
		_m_output_stream.flush();
		_m_output_stream.emit();
	}
_END_ABC_NS
_END_NS