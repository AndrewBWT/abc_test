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
#include "abc_test/core/ds/repetitions/repetition_data.h"
#include "abc_test/utility/io/file/file_rw.h"
#include "abc_test/core/ds/types.h"

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
		ds::repetition_data_t*
		repetition_data_ptr(
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
		ds::test_failure_func_t
		create_test_failure_function(
		) noexcept = 0;
	__constexpr
		virtual
		void
		finish_setup(
		) noexcept;
	__constexpr
		std::string
		virtual
		get_rep_string(
		) const = 0;
	__constexpr
		std::size_t
		virtual
		determine_mode(
		)const noexcept= 0;
protected:
	ds::repetition_data_t _m_repetition_data;
};
template<
	typename T
>
using gen_data_ptr_t = std::shared_ptr<gen_data_base_t<T>>;
_END_ABC_NS

_BEGIN_ABC_NS

template<
	typename T
>
__constexpr_imp
	gen_data_base_t<T>::gen_data_base_t(
	) noexcept
	: _m_repetition_data(ds::repetition_data_t())
{

}
template<
	typename T
>
__constexpr_imp
	ds::repetition_data_t*
	gen_data_base_t<T>::repetition_data_ptr(
	) noexcept
{
	return &_m_repetition_data;
}
template<
	typename T
>
__constexpr_imp
	std::size_t
	gen_data_base_t<T>::mode(
	) const noexcept
{
	return _m_repetition_data.mode();
}
template<
	typename T
>
__constexpr_imp
	void
	gen_data_base_t<T>::finish_setup(
	) noexcept
{
	_m_repetition_data.set_mode(determine_mode());
	_m_repetition_data.set_string(get_rep_string());
}
_END_ABC_NS