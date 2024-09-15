#pragma once

#include "abc_test/gen_data/gen_data_with_repetition_type.h"
#include "abc_test/utility/io/file/opt_file_rw.h"

_BEGIN_ABC_NS
template<
	typename T,
	typename Rep_Data
>
class gen_data_with_repetition_type_and_element_t : public gen_data_with_repetition_type_t<T,Rep_Data>
{
	//using gen_data_with_repetition_type_t<T, Rep_Data>::gen_data_with_repetition_type_t;
public:
	template<
		typename R = std::vector<T>
	>
	__constexpr
		gen_data_with_repetition_type_and_element_t(
			const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
			R&& _a_elements,
			const std::string_view _a_rep_data_file_extension,
			const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
			const T& _a_element,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	__constexpr
		virtual
		const T&
		subclass_current_element(
		) const noexcept override final;
protected:
	T _m_element;
};

_END_ABC_NS

_BEGIN_ABC_NS

template<
	typename T,
	typename Rep_Data
>
template<
	typename R
>
__constexpr
	gen_data_with_repetition_type_and_element_t<T, Rep_Data>::gen_data_with_repetition_type_and_element_t(
		const size_t _a_elements_generated,
		const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
		R&& _a_elements,
		const std::string_view _a_rep_data_file_extension,
		const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
		const T& _a_element,
		const test_options_t* _a_test_options
	) noexcept
	: gen_data_with_repetition_type_t<T, Rep_Data>(_a_elements_generated,_a_core_data_rw_info,std::forward<R>(_a_elements),
		_a_rep_data_file_extension,_a_rep_data_rw_info,_a_test_options)
	, _m_element(_a_element)
{

}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	const T&
	gen_data_with_repetition_type_and_element_t<T, Rep_Data>::subclass_current_element(
	) const noexcept
{
	return _m_element;
}
_END_ABC_NS