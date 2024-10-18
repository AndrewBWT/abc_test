#pragma once

#include "abc_test/utility/internal/macros.h"
#include <syncstream>

#include <fstream>
#include <iostream>

_BEGIN_ABC_UTILITY_IO_NS
struct threaded_ostream_output_reporter_t
{
public:
	__constexpr
		threaded_ostream_output_reporter_t(
		) noexcept = delete;
	//Cannot be constexpr due to use of std::osyncstream.
	__no_constexpr
		threaded_ostream_output_reporter_t(
			std::ostream& _a_ostream
		) noexcept;
	//Cannot be constexpr due to use of std::osyncstream.
	__no_constexpr
		void
		write(
			const std::string_view _a_line
		) const noexcept;
protected:
	mutable std::osyncstream _m_output_stream;
	mutable std::mutex _l_mutex;
};
_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
__no_constexpr_imp
	threaded_ostream_output_reporter_t::threaded_ostream_output_reporter_t(
		std::ostream& _a_ostream
	) noexcept
	: _m_output_stream(_a_ostream)
{
}
__no_constexpr_imp
	void
	threaded_ostream_output_reporter_t::write(
		const std::string_view _a_line
	) const noexcept
{
	using namespace std;
	unique_lock _l_ul(_l_mutex);
	_m_output_stream << _a_line << endl;
	_m_output_stream.flush();
	_m_output_stream.emit();
	_l_ul.unlock();
}
_END_ABC_UTILITY_IO_NS