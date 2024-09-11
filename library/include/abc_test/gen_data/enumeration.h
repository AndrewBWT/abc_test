#pragma once

#include "abc_test/gen_data/enumerable.h"
#include "abc_test/gen_data/gen_data_base.h"
#include "abc_test/gen_data/collection.h"

_BEGIN_ABC_GD_NS
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
	template<
		typename T
	>
	class enumerate_data_t : public gen_data_base_t<T>
	{
	public:
		__constexpr
			enumerate_data_t(
				enumerate_t<T>& _a_enumeration
			) noexcept;
		__constexpr
			enumerate_data_t(
				enumerate_t<T>&& _a_enumeration
			) noexcept;
		__constexpr
			bool
			has_current_element_(
			) const noexcept override;
		__constexpr
			std::string
			get_additional_string_data_(
			) const noexcept override;
		__constexpr
			bool
			are_failed_values_written_to_files(
			) const noexcept override;
		__constexpr
			void
			increment_using_additional_data_(
				const std::size_t _a_idx,
				const std::string_view _a_additional_data
			) override;
		__constexpr
			const T&
			current_element_(
			) const noexcept override;
		__constexpr
			bool
			generate_next_(
			) override;
	private:
		enumerate_t<T> _m_enumerable;
		bool _m_has_current_element;
		T _m_current_element;
	};
	template<
		typename T
	>
	__constexpr
		gen_data_collection_t<T>
		enumerate_data(
			enumerate_t<T>& _a_enumerate
		) noexcept;
	template<
		typename T
	>
	__constexpr
		gen_data_collection_t<T>
		enumerate_data(
			enumerate_t<T>&& _a_enumerate
		) noexcept;
	template<
		typename T
	>
	__constexpr
		gen_data_collection_t<T>
		randomly_probe_enumerated_data(
			enumerate_t<T>& _a_enumerate
		) noexcept;
	template<
		typename T
	>
	__constexpr
		gen_data_collection_t<T>
		randomly_probe_enumerated_data(
			enumerate_t<T>&& _a_enumerate
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
_END_ABC_NS
_END_NS

_BEGIN_ABC_GD_NS
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
		return next_element(_a_element, _a_times_called, _m_end_value);
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
			enumerate_t<T>& _a_enumeration
		) noexcept
		: gen_data_base_t<T>(0, utility::io::opt_file_rw_info_t<T>(),
			"",
			{})
		, _m_enumerable(_a_enumeration)
		, _m_current_element(_a_enumeration.min())
		, _m_has_current_element(true)
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		enumerate_data_t<T>::enumerate_data_t(
			enumerate_t<T>&& _a_enumeration
		) noexcept
		: gen_data_base_t<T>(0, utility::io::opt_file_rw_info_t<T>(),
			"",
			{})
		, _m_enumerable(_a_enumeration)
		, _m_current_element(_a_enumeration.min())
		, _m_has_current_element(true)
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		enumerate_data_t<T>::has_current_element_(
		) const noexcept
	{
		return _m_has_current_element;
	}
	template<
		typename T
	>
	__constexpr_imp
		std::string
		enumerate_data_t<T>::get_additional_string_data_(
		) const noexcept
	{
		return "";
		//return bigUnsignedToString(BigUnsigned(0));
	}
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
		void
		enumerate_data_t<T>::increment_using_additional_data_(
			const std::size_t _a_idx,
			const std::string_view _a_additional_data
		)
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		const T&
		enumerate_data_t<T>::current_element_(
		) const noexcept
	{
		return _m_current_element;
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		enumerate_data_t<T>::generate_next_(
		)
	{
		if (_m_has_current_element)
		{
			this->_m_elements_generated++;
			enumerate_index_t _l_adx{ 1 };
			_m_has_current_element = _m_enumerable.next_element(_m_current_element, _l_adx);
		}
		return _m_has_current_element;
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>
		enumerate_data(
			enumerate_t<T>& _a_enumerate
		) noexcept
	{
		using namespace std;
		return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate));
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>
		enumerate_data(
			enumerate_t<T>&& _a_enumerate
		) noexcept
	{
		using namespace std;
		return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate));
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>
		randomly_probe_enumerated_data(
			enumerate_t<T>& _a_enumerate
		) noexcept
	{
		using namespace std;
		return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate));
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>
		randomly_probe_enumerated_data(
			enumerate_t<T>&& _a_enumerate
		) noexcept
	{
		using namespace std;
		return unary_collection<T>(new enumerate_data_t<T>(_a_enumerate));
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
_END_NS