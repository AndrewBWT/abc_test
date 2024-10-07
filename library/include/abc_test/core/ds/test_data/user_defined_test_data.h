#pragma once
#include "abc_test/utility/internal/macros.h"
#include <string>

_BEGIN_ABC_DS_NS
using test_description_t = std::string;
using test_description_ref_t = std::string_view;
using test_path_t = std::string;
using test_path_ref_t = std::string_view;
class user_defined_test_data_t
{
public:
	__constexpr
		user_defined_test_data_t(
		) noexcept = delete;
	__constexpr
		user_defined_test_data_t(
			const std::string_view _a_test_name,
			const std::string_view _a_test_description,
			const std::string_view _a_test_path,
			const std::size_t _a_thread_resourses_required
		) noexcept;
	__constexpr
		const std::string_view
		test_name(
		) const noexcept;
	__constexpr
		const std::optional<std::string>&
		description(
		) const noexcept;
	__constexpr
		const std::string_view
		test_path(
		) const noexcept;
	__constexpr
		std::size_t
		thread_resourses_required(
		) const noexcept;
	__constexpr
		bool
		operator==(
			const user_defined_test_data_t& _a_rhs
			) const noexcept;
private:
	std::string _m_test_name;
	/*!
	* The description of the test.
	*/
	std::optional<std::string> _m_test_description;
	/*!
	* The test path of the test. In the form "aa::bb::cc" (where :: is a delimiter chosen by the user)
	*/
	std::string _m_test_path;
	/*!
	* The number of thread resourses required by the test.
	*/
	std::size_t _m_thread_resourses_required;
};
_END_ABC_DS_NS

/*!
* formatter for registered_test_data_t
*/
template
<
>
__constexpr
struct fmt::formatter<abc::ds::user_defined_test_data_t> : formatter<string_view> {
	// parse is inherited from formatter<string_view>.

	auto
		format(
			abc::ds::user_defined_test_data_t _a_rtd,
			format_context& _a_ctx
		) const
		->format_context::iterator;
};

_BEGIN_ABC_DS_NS
__constexpr_imp
user_defined_test_data_t::user_defined_test_data_t(
	const std::string_view _a_test_name,
	const std::string_view _a_test_description,
	const std::string_view _a_test_path,
	const std::size_t _a_thread_resourses_required
) noexcept
	: _m_test_name(_a_test_name)
	, _m_test_description(_a_test_description)
	, _m_test_path(_a_test_path)
	, _m_thread_resourses_required(_a_thread_resourses_required)
{

}
__constexpr_imp
const std::string_view
user_defined_test_data_t::test_name(
) const noexcept
{
	return _m_test_name;
}
__constexpr_imp
const std::optional<std::string>&
user_defined_test_data_t::description(
) const noexcept
{
	return _m_test_description;
}
__constexpr_imp
const std::string_view
user_defined_test_data_t::test_path(
) const noexcept
{
	return _m_test_path;
}
__constexpr_imp
std::size_t
user_defined_test_data_t::thread_resourses_required(
) const noexcept
{
	return _m_thread_resourses_required;
}
__constexpr_imp
bool
user_defined_test_data_t::operator==(
	const user_defined_test_data_t& _a_rhs
	) const noexcept
{
	__cmp_test(_m_test_name);
	__cmp_test(_m_test_description);
	__cmp_test(_m_test_path);
	__cmp_test(_m_thread_resourses_required);
	return true;
}
_END_ABC_DS_NS

__constexpr_imp
auto
fmt::formatter<abc::ds::user_defined_test_data_t>::format(
	abc::ds::user_defined_test_data_t _a_rtd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_rv{ fmt::format(
		"{0}{{"
		"{1} = {2},"
		"{3} = {4},"
		"{5} = {6},"
		"{7} = {8}"
		"}}",
		typeid(_a_rtd).name(),
		"_m_test_name", _a_rtd.test_name(),
		"_m_description", _a_rtd.description(),
		"_m_test_path", _a_rtd.test_path(),
		"_m_thread_resourses_required", _a_rtd.thread_resourses_required()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}