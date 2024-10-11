#pragma once

#include "abc_test/utility/io/file/file_rw.h"

_BEGIN_ABC_UTILITY_IO_NS
template<
	typename T,
	std::size_t Mode
>
class opt_file_rw_t
{
public:
	template<
		typename R = std::vector<T>
	>
	__constexpr
		opt_file_rw_t(
			const opt_file_rw_info_t<T>& _a_opt_file_rw_info,
			const std::string_view _a_str,
			R&& _a_range = {},
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	__constexpr
		bool
		has_current_element(
			const std::size_t _a_mode
		) const noexcept;
	__constexpr
		const T&
		current_element(
		) const;
	__constexpr
		bool
		generate_next(
			const std::size_t _a_mode
		) noexcept;
	__constexpr
		std::optional<ds::repetition_data_t>
		log_failure_and_return_rep_data(
			bool& _a_has_current_element_been_written_to_file,
			const ds::repetition_data_t& _a_rep_data
		) noexcept;
	__constexpr
		std::size_t
		elements_read(
		) const noexcept;
private:
	std::optional<file_rw_t<T>> _m_file_rw;
};
_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
template<
	typename T,
	std::size_t Mode
>
template<
	typename R
>
__constexpr_imp
	opt_file_rw_t<T, Mode>::opt_file_rw_t(
		const opt_file_rw_info_t<T>& _a_opt_file_rw_info,
		const std::string_view _a_str,
		R&& _a_range,
		const test_options_t* _a_test_options
	) noexcept
	: _m_file_rw(_a_opt_file_rw_info.has_value() ?
		std::optional<file_rw_t<T>>(file_rw_t<T>(_a_opt_file_rw_info.value(), _a_str, 
			_a_test_options,_a_range)) :
		std::optional<file_rw_t<T>>{})
{
}
template<
	typename T,
	std::size_t Mode
>
__constexpr_imp
	bool
	opt_file_rw_t<T, Mode>::has_current_element(
		const std::size_t _a_mode
	) const noexcept
{
	return (
		_m_file_rw.has_value() &&
		//Mode == _a_mode &&
		_m_file_rw.value().has_current_element()
		);
}
template<
	typename T,
	std::size_t Mode
>
__constexpr_imp
const T&
opt_file_rw_t<T, Mode>::current_element(
) const
{
	using namespace errors;
	if (_m_file_rw.has_value())
	{
		return _m_file_rw.value().current_element();
	}
	else
	{
		throw test_library_exception_t(fmt::format(
			"Tried to get the current element in a std::optional<file_rw_t<{0}> object, "
			"when the optional object has no value.", typeid(T).name()
		));
	}
}
template<
	typename T,
	std::size_t Mode
>
__constexpr_imp
	bool
	opt_file_rw_t<T, Mode>::generate_next(
		const std::size_t _a_mode
	) noexcept
{
	if (_m_file_rw.has_value())
	{
		return _m_file_rw.value().read_next_element();
	}
	else
	{
		return false;
	}
}
template<
	typename T,
	std::size_t Mode
>
__constexpr_imp
std::optional<ds::repetition_data_t>
	opt_file_rw_t<T, Mode>::log_failure_and_return_rep_data(
		bool& _a_has_current_element_been_written_to_file,
		const ds::repetition_data_t& _a_rep_data
	) noexcept
{
	using namespace ds;
	using namespace std;
	if (not _a_has_current_element_been_written_to_file)
	{
		//Can we write to file?
		if (_m_file_rw.has_value())
		{
			//Should we write to file?
			if (_a_rep_data.for_loop_iteration_data.flied.mode == 2)
			{

			}
		}
	}
	return optional<repetition_data_t>{};
}
template<
	typename T,
	std::size_t Mode
>
__constexpr_imp
	std::size_t
	opt_file_rw_t<T, Mode>::elements_read(
) const noexcept
{
	return _m_file_rw.has_value() ? _m_file_rw.value().elements_read() : 0;
}
_END_ABC_UTILITY_IO_NS