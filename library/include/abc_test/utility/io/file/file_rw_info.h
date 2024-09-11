#pragma once
#include "abc_test/utility/str/rw_info.h"
#include "abc_test/utility/io/file/file_name.h"

_BEGIN_ABC_UTILITY_IO_NS
	template<
		typename T
	>
	struct file_rw_info_t
	{
	public:
		__constexpr
			file_rw_info_t(
				const file_name_t& _a_file_name,
				const str::rw_info_t<T>& _a_rw_info = str::rw_info_t<T>()
			) noexcept;
		__constexpr
			const file_name_t&
			file_name(
			) const noexcept;
		__constexpr
			const str::parser_t<T>&
			parser(
			) const noexcept;
		__constexpr
			const str::printer_t<T>&
			printer(
			) const noexcept;
	private:
		str::rw_info_t<T> _m_rw_info;
		file_name_t _m_file_name;
	};
	template<
		typename T
	>
	using opt_file_rw_info_t = std::optional<file_rw_info_t<T>>;
_END_ABC_NS
_END_NS

_BEGIN_ABC_UTILITY_IO_NS
	template<
		typename T
	>
	__constexpr_imp
		file_rw_info_t<T>::file_rw_info_t(
			const file_name_t& _a_file_name,
			const str::rw_info_t<T>& _a_rw_info
		) noexcept
		: _m_file_name(_a_file_name)
		, _m_rw_info(_a_rw_info)
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		const file_name_t&
		file_rw_info_t<T>::file_name(
		) const noexcept
	{
		return _m_file_name;
	}
	template<
		typename T
	>
	__constexpr_imp
		const str::parser_t<T>&
		file_rw_info_t<T>::parser(
		) const noexcept
	{
		return _m_rw_info.parser();
	}
	template<
		typename T
	>
	__constexpr_imp
		const str::printer_t<T>&
		file_rw_info_t<T>::printer(
		) const noexcept
	{
		return _m_rw_info.printer();
	}
_END_ABC_NS
_END_NS