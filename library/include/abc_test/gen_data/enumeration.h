#pragma once

#include "abc_test/gen_data/enumerable.h"
#include "abc_test/gen_data/gen_data_with_repetition_type_and_element.h"
#include "abc_test/gen_data/collection.h"

_BEGIN_ABC_NS
struct elements_t
{
public:
	enumerate_index_t _m_elements;
};
template<
	typename T
>
class enumerate_t
{
public:
	__constexpr_imp
		bool
		less_than(
			const T& _a_l,
			const T& _a_r
		) const noexcept
	{
		return _m_enumerable->less_than(_a_l, _a_r);
	}
	__constexpr_imp
		bool
		equal(
			const T& _a_l,
			const T& _a_r
		) const noexcept
	{
		return _m_enumerable->equal(_a_l, _a_r);
	}
	__constexpr
		enumerate_t(
			enumerable_t<T>* _a_enumerable,
			const T& _a_start_value,
			const elements_t _a_elements,
			const bool _a_forward_direction
		) noexcept;
	__constexpr
		enumerate_t(
			enumerable_t<T>* _a_enumerable,
			const T& _a_start_value,
			const T& _a_end_value
		) noexcept;
	__constexpr
		enumerate_t(
		) noexcept = delete;
	__constexpr
		bool
		next_element(
			T& _a_element,
			enumerate_index_t& _a_times_called = enumerate_index_t{ 1 }
		) const noexcept;
	__constexpr
		bool
		move_forward(
			T& _a_element,
			enumerate_index_t& _a_times_called = enumerate_index_t{ 1 }
		) const noexcept;
	__constexpr
		bool
		move_backward(
			T& _a_element,
			enumerate_index_t& _a_times_called = enumerate_index_t{ 1 }
		) const noexcept;
	__constexpr_imp
		T
		min(
		) const noexcept
	{
		return _m_start_value;
	}
	__constexpr_imp
		T
		max(
		) const noexcept
	{
		return _m_end_value;
	}
private:
	enumerable_t<T>* _m_enumerable;
	T _m_start_value;
	T _m_end_value;
	bool _m_forward;
	bool _m_has_any_elements;
};
using enumerate_data_id_type_t = std::size_t;
template<
	typename T
>
class enumerate_data_t : public gen_data_with_repetition_type_and_element_t<T,enumerate_data_id_type_t>
{
public:
	__constexpr
		enumerate_data_t(
			const enumerate_t<T>& _a_enumerate,
			const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
			const utility::io::opt_file_name_t& _a_enumerate_data_file_name,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	__constexpr
		bool
		subclass_has_current_element(
		) const noexcept override;
	//__constexpr
	//	std::string
	//	subclass_get_repetition_data_as_string(
	//	) const noexcept override;
	__constexpr
		bool
		are_failed_values_written_to_files(
		) const noexcept;
	__constexpr_imp
		void
		virtual
		subclass_set_data_using_mode_and_repetition_data(
			const std::size_t _a_idx,
			const enumerate_data_id_type_t _a_data
		) override final
	{
		auto _l_data{ _a_data };
		_m_has_current_element = _m_enumerable.move_forward(this->_m_element, _l_data);
	}
	__constexpr
		enumerate_data_id_type_t
		subclass_get_repetition_data(
		) const noexcept
	{
		return enumerate_data_id_type_t();
	}
	//__constexpr
	//	void
	//	increment_using_additional_data_(
	//		const std::size_t _a_idx,
	//		const std::string_view _a_additional_data
	//	) override;
	__constexpr
		virtual
		void
		subclass_reset_data(
		) noexcept final
	{
		_m_has_current_element = true;
		this->_m_element = _m_enumerable.min();
	}
	__constexpr
		bool
		subclass_generate_next(
		) override;
private:
	enumerate_t<T> _m_enumerable;
	bool _m_has_current_element;
};
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	enumerate_data(
		const enumerate_t<T>& _a_enumerate
	) noexcept;
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	enumerate_data_using_file_type_1(
		const enumerate_t<T>& _a_enumerate,
		const utility::io::file_rw_info_t<T>& _a_file_data
	) noexcept;
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	enumerate_data_using_file_type_2(
		const enumerate_t<T>& _a_enumerate,
		const utility::io::file_name_t& _a_file_name
	) noexcept;
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	enumerate_data_using_file_type_3(
		const enumerate_t<T>& _a_enumerate,
		const utility::io::file_rw_info_t<T>& _a_file_data,
		utility::io::file_name_t& _a_file_name
	) noexcept;
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	randomly_probe_enumerated_data(
		const enumerate_t<T>& _a_enumerate
	) noexcept;
template<
	typename T
>
	requires max_value_c<T> && min_value_c<T>
__constexpr_imp
	enumerate_t<T>
	all_values(
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept
{
	return all_values_moving_forward<T>(_a_enumerable);
}
template<
	typename T
>
__constexpr
	enumerate_t<T>
	from_m_to_n(
		const T& _a_lower,
		const T& _a_upper,
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T
>
__constexpr
	enumerate_t<T>
	from_m_enumerate_k_elements(
		const T& _a_start_value,
		const elements_t _a_elements,
		const bool _a_forward_direction,
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T
>
requires max_value_c<T>
__constexpr
	enumerate_t<T>
	from_val_to_max(
		const T& _a_value,
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T
>
requires max_value_c<T>
__constexpr
	enumerate_t<T>
	from_max_to_val(
		const T& _a_value,
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T
>
requires max_value_c<T>
__constexpr
	enumerate_t<T>
	from_val_to_min(
		const T& _a_value,
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T
>
requires max_value_c<T>
__constexpr
	enumerate_t<T>
	from_min_to_val(
	const T& _a_value,
	enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
) noexcept;
template<
	typename T
>
__constexpr
	enumerate_t<T>
	from_m_move_forward_k_elements(
		const T& _a_start_value,
		const elements_t _a_elements,
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T
>
__constexpr
	enumerate_t<T>
	from_m_move_backward_k_elements(
		const T& _a_start_value,
		const elements_t _a_elements,
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T	 
>
requires max_value_c<T>&& min_value_c<T>
__constexpr
	enumerate_t<T>
	all_values_moving_forward(
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
template<
	typename T	 
>
requires max_value_c<T>&& min_value_c<T>
__constexpr
	enumerate_t<T>
	all_values_moving_backward(
		enumerable_t<T>* _a_enumerable = new enumerable_t<T>()
	) noexcept;
__constexpr_imp
	const std::string_view
	enumerate_data_file_type(
	) noexcept
{
	return global::get_global_test_options_ptr()->_m_enumerate_data_file_type;
}
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename T
>
__constexpr_imp
	enumerate_t<T>::enumerate_t(
		enumerable_t<T>* _a_enumerable,
		const T& _a_start_value,
		const elements_t _a_elements,
		const bool _a_forward_direction
	) noexcept
	: _m_start_value(_a_start_value)
	, _m_end_value(_a_start_value)
	, _m_forward(_a_forward_direction)
	, _m_enumerable(_a_enumerable)
{
	enumerate_index_t _l_n_times_to_call{ _a_elements._m_elements };
	if constexpr (max_value_c<T>)
	{
		if constexpr (min_value_c<T>)
		{
			bool _l_pc = _m_forward ?
				_m_enumerable->increment(_m_end_value, _l_n_times_to_call, max_value_t<T>().max_value()) :
				_m_enumerable->decrement(_m_end_value, _l_n_times_to_call, min_value_t<T>().min_value());
		}
		else
		{
			bool _l_pc = _m_forward ?
				_m_enumerable->increment(_m_end_value, _l_n_times_to_call, max_value_t<T>().max_value()) :
				_m_enumerable->decrement(_m_end_value, _l_n_times_to_call);
		}
	}
	else
	{
		if constexpr (min_value_c<T>)
		{
			bool _l_pc = _m_forward ?
				_m_enumerable->increment(_m_end_value, _l_n_times_to_call) :
				_m_enumerable->decrement(_m_end_value, _l_n_times_to_call, min_value_t<T>().min_value());
		}
		else
		{
			bool _l_pc = _m_forward ?
				_m_enumerable->increment(_m_end_value, _l_n_times_to_call) :
				_m_enumerable->decrement(_m_end_value, _l_n_times_to_call);
		}
	}
}
template<
	typename T
>
__constexpr_imp
	enumerate_t<T>::enumerate_t(
		enumerable_t<T>* _a_enumerable,
		const T& _a_start_value,
		const T& _a_end_value
	) noexcept
	: _m_start_value(_a_start_value)
	, _m_end_value(_a_end_value)
	, _m_forward(_a_enumerable->less_than(_a_start_value,_a_end_value))
	, _m_enumerable(_a_enumerable)
{

}
template<
	typename T
>
__constexpr_imp
	bool
	enumerate_t<T>::next_element(
		T& _a_element,
		enumerate_index_t& _a_times_called
	) const noexcept
{
	return (_m_forward) ?
		_m_enumerable->increment(_a_element, _a_times_called, _m_end_value) :
		_m_enumerable->decrement(_a_element, _a_times_called, _m_end_value);
}
template<
	typename T
>
__constexpr_imp
	bool
	enumerate_t<T>::move_forward(
		T& _a_element,
		enumerate_index_t& _a_times_called
	) const noexcept
{
	return next_element(_a_element, _a_times_called);
}
template<
	typename T
>
__constexpr_imp
	bool
	enumerate_t<T>::move_backward(
		T& _a_element,
		enumerate_index_t& _a_times_called
	) const noexcept
{
	return (_m_forward) ?
		_m_enumerable->decrement(_a_element, _a_times_called, _m_start_value) :
		_m_enumerable->increment(_a_element, _a_times_called, _m_start_value);
}
template<
	typename T
>
__constexpr_imp
	enumerate_data_t<T>::enumerate_data_t(
		const enumerate_t<T>& _a_enumerate,
		const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
		const utility::io::opt_file_name_t& _a_enumerate_opt_data_file_name,
		const test_options_t* _a_test_options
	) noexcept
	: gen_data_with_repetition_type_and_element_t<T, enumerate_data_id_type_t>(0, 
		_a_templated_file_rw, {},_a_test_options->_m_enumerate_data_file_type, 
		_a_enumerate_opt_data_file_name, _a_enumerate.min(), _a_test_options)
	, _m_enumerable(_a_enumerate)
	, _m_has_current_element(true)
{

}
template<
	typename T
>
__constexpr_imp
	bool
	enumerate_data_t<T>::subclass_has_current_element(
	) const noexcept
{
	return _m_has_current_element;
}
/*template<
	typename T
>
__constexpr_imp
	std::string
	enumerate_data_t<T>::subclass_get_repetition_data_as_string(
	) const noexcept
{
	return "";
	//return bigUnsignedToString(BigUnsigned(0));
}*/
template<
	typename T
>
__constexpr_imp
	bool
	enumerate_data_t<T>::are_failed_values_written_to_files(
	) const noexcept
{
	return true;
}
template<
	typename T
>
__constexpr_imp
	bool
	enumerate_data_t<T>::subclass_generate_next(
	)
{
	if (_m_has_current_element)
	{
		this->_m_elements_generated++;
		enumerate_index_t _l_adx{ 1 };
		_m_has_current_element = _m_enumerable.next_element(this->_m_element, _l_adx);
	}
	return _m_has_current_element;
}
template<
	typename T
>
__constexpr_imp
	gen_data_collection_t<T>
	enumerate_data(
		const enumerate_t<T>& _a_enumerate
	) noexcept
{
	using namespace utility::io;
	return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate,
		opt_file_rw_info_t<T>{}, opt_file_name_t{}));
}
template<
	typename T
>
__constexpr_imp
	gen_data_collection_t<T>
	enumerate_data_using_file_type_1(
		const enumerate_t<T>& _a_enumerate,
		const utility::io::file_rw_info_t<T>& _a_file_data
	) noexcept
{
	using namespace utility::io;
	return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate,
		opt_file_rw_info_t<T>{_a_file_data}, opt_file_name_t{}));
}
template<
	typename T
>
__constexpr_imp
		gen_data_collection_t<T>
	enumerate_data_using_file_type_2(
		const enumerate_t<T>& _a_enumerate,
		const utility::io::file_name_t& _a_file_name
	) noexcept
{
	using namespace utility::io;
	return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate,
		opt_file_rw_info_t<T>{}, opt_file_name_t{ _a_file_name }));
}
template<
	typename T
>
__constexpr_imp
	gen_data_collection_t<T>
	enumerate_data_using_file_type_3(
		const enumerate_t<T>& _a_enumerate,
		const utility::io::file_rw_info_t<T>& _a_file_data,
		utility::io::file_name_t& _a_file_name
	) noexcept
{
	using namespace utility::io;
	return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate,
		opt_file_rw_info_t<T>{_a_file_data}, opt_file_name_t{ _a_file_name }));
}
template<
	typename T
>
__constexpr_imp
	gen_data_collection_t<T>
	randomly_probe_enumerated_data(
		const enumerate_t<T>& _a_enumerate
	) noexcept
{
	using namespace std;
	using namespace utility::io;
	return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate,
		opt_file_rw_info_t<T>{}, opt_file_name_t{}));
}
template<
	typename T
>
__constexpr_imp
	enumerate_t<T>
	from_m_to_n(
		const T& _a_lower,
		const T& _a_upper,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	using namespace std;
	return enumerate_t<T>(_a_enumerable, _a_lower, _a_upper);
}
template<
	typename T
>
__constexpr_imp
	enumerate_t<T>
	from_m_enumerate_k_elements(
		const T& _a_start_value,
		const elements_t _a_elements,
		const bool _a_forward_direction,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return enumerate_t<T>(_a_enumerable, _a_start_value, _a_elements, _a_forward_direction);
}
template<
	typename T
>
requires max_value_c<T>
__constexpr_imp
	enumerate_t<T>
	from_val_to_max(
		const T& _a_value,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_to_n(_a_value, max_value_t<T>().max_value(), _a_enumerable);
}
template<
	typename T
>
requires max_value_c<T>
__constexpr_imp
	enumerate_t<T>
	from_max_to_val(
		const T& _a_value,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_to_n(max_value_t<T>().max_value(), _a_value, _a_enumerable);
}
template<
	typename T
>
requires max_value_c<T>
__constexpr_imp
	enumerate_t<T>
	from_val_to_min(
		const T& _a_value,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_to_n(_a_value, min_value_t<T>().min_value(), _a_enumerable);
}
template<
	typename T	
>
	requires max_value_c<T>
__constexpr_imp
	enumerate_t<T>
	from_min_to_val(
		const T& _a_value,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_to_n(min_value_t<T>().min_value(), _a_value, _a_enumerable);
}
template<
	typename T
>
__constexpr_imp
	enumerate_t<T>
	from_m_move_forward_k_elements(
		const T& _a_start_value,
		const elements_t _a_elements,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_enumerate_k_elements(_a_start_value, _a_elements, true, _a_enumerable);
}
template<
	typename T
>
__constexpr_imp
	enumerate_t<T>
	from_m_move_backward_k_elements(
		const T& _a_start_value,
		const elements_t _a_elements,
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_enumerate_k_elements(_a_start_value, _a_elements, false, _a_enumerable);
}
template<
	typename T
>
	requires max_value_c<T>&& min_value_c<T>
__constexpr_imp
	enumerate_t<T>
	all_values_moving_forward(
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_to_n(min_value_t<T>().min_value(), max_value_t<T>().max_value(), _a_enumerable);
}
template<
	typename T
>
requires max_value_c<T>&& min_value_c<T>
__constexpr_imp
	enumerate_t<T>
	all_values_moving_backward(
		enumerable_t<T>* _a_enumerable
	) noexcept
{
	return from_m_to_n(max_value_t<T>().max_value(), min_value_t<T>().min_value(), _a_enumerable);
}
_END_ABC_NS