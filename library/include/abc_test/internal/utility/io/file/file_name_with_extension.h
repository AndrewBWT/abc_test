#pragma once
#include "abc_test/internal/utility/io/file/file_name.h"

_BEGIN_ABC_UTILITY_IO_NS
	struct file_name_with_extension_t : public file_name_t
	{
		__constexpr
			file_name_with_extension_t(
			) noexcept;
		__constexpr
			file_name_with_extension_t(
				const file_name_t& _a_file_name,
				const std::string_view _a_file_extension
			) noexcept;
		__no_constexpr
			bool
			exists(
			) const noexcept;
	};
_END_ABC_UTILITY_IO_NS
/*!
* formatter for post_setup_test_ata object.
*/
template
<
>
struct fmt::formatter<_ABC_NS_UTILITY::io::file_name_with_extension_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__no_constexpr
		auto
		format(
			_ABC_NS_UTILITY::io::file_name_with_extension_t _a_fnwe,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

_BEGIN_ABC_UTILITY_IO_NS
	__constexpr_imp
		file_name_with_extension_t::file_name_with_extension_t(
		) noexcept
		: file_name_t()
	{

	}
	__constexpr_imp
		file_name_with_extension_t::file_name_with_extension_t(
			const file_name_t& _a_file_name,
			const std::string_view _a_file_extension
		) noexcept
		: file_name_t(
			_a_file_name.file_path().string() + std::string(".") + std::string(_a_file_extension))
	{

	}
	__no_constexpr_imp
		bool
		file_name_with_extension_t::exists(
		) const noexcept
	{
		return std::filesystem::exists(_m_file_path);
	}
	_END_ABC_UTILITY_IO_NS

__no_constexpr_imp
auto
fmt::formatter<_ABC_NS_UTILITY::io::file_name_with_extension_t>::format(
	_ABC_NS_UTILITY::io::file_name_with_extension_t _a_fn,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	using namespace _ABC_NS_UTILITY::io;
	string _l_rv{ fmt::format("{0}{{_m_file_path = {1}}}",
		typeid(file_name_with_extension_t).name(), _a_fn.file_path().string()) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}
