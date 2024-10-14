#pragma once
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <functional>
#include <scn/scan.h>
#include "abc_test/global.h"
#include <expected>

#include "abc_test/utility/io/file/file_name.h"
#include "abc_test/utility/io/file/file_reader.h"
#include "abc_test/utility/io/file/file_writer.h"
#include "abc_test/core/ds/gen_data_memoization/for_loop_creation_data.h"
#include "abc_test/utility/io/file/file_rw.h"
#include "abc_test/core/ds/type_synonyms.h"

_BEGIN_ABC_NS
/*!
* This type synonym refers to the function used when a test is failed;
* Under some circumstances, the failed repetition_data_t element used to
* log this error is different to that used to create the test point;
* for example when failures are written to a file, to repeat that test
* the file must be navigated.
*/
template<
	typename T
>
struct gen_data_base_t
{
public:
	__constexpr
		gen_data_base_t(
		) noexcept;
	__constexpr
		ds::gen_data_creation_data_t*
		for_loop_iteration_element_data_ptr(
		) noexcept;
	__constexpr
		ds::gen_data_creation_data_t&
		for_loop_iteration_element_data_ref(
		) noexcept;
	__constexpr
		std::size_t
		mode(
		) const noexcept;
	__constexpr
		virtual
		bool
		has_current_element(
		) const noexcept = 0;
	__constexpr
		virtual
		const T&
		current_element(
		) const noexcept = 0;
	__constexpr
		virtual
		bool
		generate_next(
		) = 0;
	__constexpr
		virtual
		void
		set_data_using_mode_and_string_representing_repetition_data(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		) = 0;
	__constexpr
		virtual
		void
		finish_setup(
		) noexcept;
	__constexpr virtual ds::gen_data_creation_data_t
		generate_repeat_repetition_data() const noexcept
		= 0;
	__constexpr virtual ds::gen_data_creation_data_t
		generate_repetition_data() const noexcept
		= 0;
	__constexpr virtual std::string
		get_rep_string(
		) const = 0;
protected:
	std::size_t _m_mode;
	ds::gen_data_creation_data_t _m_flied;
};
template<
	typename T
>
using gen_data_ptr_t = std::shared_ptr<gen_data_base_t<T>>;

__no_constexpr errors::test_library_exception_t
unsupported_mode_exception(
	const std::string_view                    _a_type,
	const std::size_t                         _a_mode,
	const std::initializer_list<std::size_t>& _a_supported_elements
);
_END_ABC_NS

_BEGIN_ABC_NS

template<
	typename T
>
__constexpr_imp
	gen_data_base_t<T>::gen_data_base_t(
	) noexcept
	: _m_flied(ds::gen_data_creation_data_t())
{

}
template<
	typename T
>
__constexpr_imp
	ds::gen_data_creation_data_t*
	gen_data_base_t<T>::for_loop_iteration_element_data_ptr(
	) noexcept
{
	return &_m_flied;
}
template<
	typename T
>
__constexpr_imp
ds::gen_data_creation_data_t&
gen_data_base_t<T>::for_loop_iteration_element_data_ref(
) noexcept
{
	return _m_flied;
}
template<
	typename T
>
__constexpr_imp
	std::size_t
	gen_data_base_t<T>::mode(
	) const noexcept
{
	return _m_flied.mode;
}
template<
	typename T
>
__constexpr_imp
	void
	gen_data_base_t<T>::finish_setup(
	) noexcept
{
//	_m_flied.mode = determine_mode();
//	_m_flied.additional_data = get_rep_string();
}
__no_constexpr_imp errors::test_library_exception_t
unsupported_mode_exception(
	const std::string_view                    _a_type,
	const std::size_t                         _a_mode,
	const std::initializer_list<std::size_t>& _a_supported_elements
)
{
	using namespace errors;
	return test_library_exception_t(fmt::format(
		"{0} does not support mode {1}. {0} only supports the following modes: "
		"{2}",
		_a_type,
		_a_mode,
		_a_supported_elements
	));
}
_END_ABC_NS