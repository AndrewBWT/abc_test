#pragma once
#include "abc_test/utility/io/file/file_name.h"

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
		__constexpr
			bool
			exists(
			) const noexcept;
	};
_END_ABC_NS
_END_NS
/*!
* formatter for post_setup_test_ata object.
*/
template
<
>
struct fmt::formatter<abc::utility::io::file_name_with_extension_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__constexpr
		auto
		format(
			abc::utility::io::file_name_with_extension_t _a_fnwe,
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
	__constexpr_imp
		bool
		file_name_with_extension_t::exists(
		) const noexcept
	{
		return std::filesystem::exists(_m_file_path);
	}
_END_ABC_NS
_END_NS

__constexpr_imp
auto
fmt::formatter<abc::utility::io::file_name_with_extension_t>::format(
	abc::utility::io::file_name_with_extension_t _a_fn,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	using namespace abc::utility::io;
	string _l_rv{ fmt::format("{0}{{_m_file_path = {1}}}",
		typeid(file_name_with_extension_t).name(), _a_fn.file_path().string()) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}
