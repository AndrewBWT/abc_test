#pragma once

#include "abc_test/gen_data/gen_data_base.h"
#include "abc_test/gen_data/collection.h"


_BEGIN_ABC_NS
struct test_runner_t;
	//Forward declarations
	template<
		typename T
	>
	struct gen_data_collection_t;
	/*!
	* Iterator object for a gen_data_collection_t object.
	*/
	template<
		typename T
	>
	struct gen_data_collection_iterator_t
	{
	public:
		//! Difference type for iterator
		using difference_type = std::ptrdiff_t;
		//! Value type for iterator
		using value_type = T;
		//! The internal iterator type of the gne_data_collection_t ype
		using gdc_iterator = gen_data_collection_t<T>::gen_data_collection_internal_t::iterator;
		//The value type of the gen_data_collection_iterator_t type
		using reference = const gen_data_collection_iterator_t<T>::value_type&;
		/*!
		* Constructor
		*/
		__constexpr
			gen_data_collection_iterator_t(
				gdc_iterator _a_begin_iterator,
				gdc_iterator _a_end_iterator,
				gdc_iterator _a_this_iterator,
				const bool _a_add_repeatable_test_config,
				test_runner_t* _a_test_runner
			);
		/*!
		* Destructor
		*/
		__constexpr
			~gen_data_collection_iterator_t(
			);
		/*!
		* Increment element
		*/
		__constexpr
			gen_data_collection_iterator_t<T>&
			operator++(
				);
		/*!
		* Increment element
		*/
		__constexpr
			gen_data_collection_iterator_t<T>
			operator++(
				int
			);
		/*!
		* Equality comparison
		*/
		__constexpr
			bool
			operator==(
				const gen_data_collection_iterator_t<T>& _a_rhs
			) const noexcept;
		/*!
		* Inequality comparison
		*/
		__constexpr
			bool
			operator!=(
				const gen_data_collection_iterator_t<T>& _a_rhs
			) const noexcept;
		/*!
		* Dereference operator
		*/
		__constexpr
			reference
			operator*(
			) const;
		/*!
		* Increment iterator using an optional set of repetition data.
		*/
		__constexpr
			void
			increment_iterator(
				const std::optional<ds::repetition_data_t>& _a_opt_repetition_data
			);
	private:
		gdc_iterator _m_begin_iterator;
		gdc_iterator _m_end_iterator;
		gdc_iterator _m_this_iterator;
		size_t _m_this_iterators_index;
		bool _m_add_repeatable_test_config;
		test_runner_t* _m_test_runner;
	};
	_END_ABC_NS

		_BEGIN_ABC_NS
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_iterator_t<T>::gen_data_collection_iterator_t(
			gdc_iterator _a_begin_iterator,
			gdc_iterator _a_end_iterator,
			gdc_iterator _a_this_iterator,
			const bool _a_add_repeatable_test_config,
			test_runner_t* _a_test_runner
		)
		: _m_begin_iterator(_a_begin_iterator)
		, _m_end_iterator(_a_end_iterator)
		, _m_this_iterator(_a_this_iterator)
		, _m_this_iterators_index{ 0 }
		, _m_add_repeatable_test_config(_a_add_repeatable_test_config)
		, _m_test_runner(_a_test_runner)
	{
		using namespace ds;
		using namespace errors;
		invoked_test_info_t& _l_current_test{ _m_test_runner->current_test() };
		if (_m_add_repeatable_test_config)
		{
			if (_l_current_test.has_post_setup_test_data())
			{
				if (_l_current_test.post_setup_test_data().has_repetition_data())
				{
					increment_iterator(_l_current_test.get_repetition_iterator_data());
				}
			}
			else
			{
				throw test_library_exception_t(fmt::format(
					"invoked_test_info_t has no post_setup_test_data element. "
					"invoked_test_info_t = {0}.",
					_l_current_test));
			}
			if (_m_this_iterator != _m_end_iterator)
			{
				auto& _l_this_iterator_ref{ *_m_this_iterator };
				_l_current_test.for_loop_data_collection().increment(
					_l_this_iterator_ref->create_test_failure_function(),
					std::distance(_m_begin_iterator, _m_this_iterator),
					_l_this_iterator_ref->repetition_data_ptr());
			}
			else
			{
				_m_add_repeatable_test_config = false;
			}
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_iterator_t<T>::~gen_data_collection_iterator_t(
		)
	{
		if (_m_add_repeatable_test_config)
		{
			_m_test_runner->current_test().for_loop_data_collection().decrement();
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_iterator_t<T>&
		gen_data_collection_iterator_t<T>::operator++(
		)
	{
		using namespace std;
		using namespace ds;
		invoked_test_info_t& _l_current_test{ _m_test_runner->current_test() };
		if (_l_current_test.has_post_setup_test_data())
		{
			if (_l_current_test.post_setup_test_data().has_repetition_data())
			{
				increment_iterator(_l_current_test.increment_repetition_iterator_data());
			}
			else
			{
				const bool _l_has_next_element{ (*_m_this_iterator)->generate_next() };
				if (_l_has_next_element)
				{
					++_m_this_iterators_index;
				}
				else
				{
					//Check to ensure the new iterator actually has elements
					_m_this_iterators_index = 0;
					bool _l_iterator_has_elements{ false };
					do
					{
						++_m_this_iterator;
						if (_m_this_iterator == _m_end_iterator)
						{
							break;
						}
						if ((*_m_this_iterator)->has_current_element())
						{
							_l_iterator_has_elements = true;
						}
					} while (not _l_iterator_has_elements);
				}
			}
			if (_m_this_iterator != _m_end_iterator)
			{
				auto& _l_this_iterator_ref{ *_m_this_iterator };
				_l_current_test.for_loop_data_collection().update(_l_this_iterator_ref->create_test_failure_function(),
					std::distance(_m_begin_iterator, _m_this_iterator),
					_l_this_iterator_ref->repetition_data_ptr());
					//_l_this_iterator_ref->infer_correct_mode(),
					//_l_this_iterator_ref->get_additinoal_string_data());
			}
		}
		return *this;
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_iterator_t<T>
		gen_data_collection_iterator_t<T>::operator++(
			int
			)
	{
		auto _l_cpy{ *this };
		++(*this);
		return _l_cpy;
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		gen_data_collection_iterator_t<T>::operator==(
			const gen_data_collection_iterator_t<T>& _a_rhs
			) const noexcept
	{
		if (_m_begin_iterator != _a_rhs._m_begin_iterator)
		{
			return false;
		}
		if (_m_end_iterator != _a_rhs._m_end_iterator)
		{
			return false;
		}
		if (_m_this_iterator != _a_rhs._m_this_iterator)
		{
			return false;
		}
		//	if (_m_this_iterators_index != _a_rhs._m_this_iterators_index)
		//	{
		//		return false;
		//	}
		if (_m_test_runner != _a_rhs._m_test_runner)
		{
			return false;
		}
		return true;
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		gen_data_collection_iterator_t<T>::operator!=(
			const gen_data_collection_iterator_t<T>& _a_rhs
			) const noexcept
	{
		return !(*this == _a_rhs);
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_iterator_t<T>::reference
		gen_data_collection_iterator_t<T>::operator*(
			) const
	{
		if (_m_this_iterator == _m_end_iterator ||
			(*_m_this_iterator)->has_current_element() == false)
		{
			auto _l_x = (*_m_this_iterator)->has_current_element();
			throw std::runtime_error("Could not deference iterator");
		}
		else
		{
			auto& _l_this_iterator_ref{ *_m_this_iterator };
			int64_t _l_si{ std::distance(_m_begin_iterator, _m_this_iterator) };
			reference _l_current_element{ _l_this_iterator_ref->current_element() };
		//	_m_test_runner->current_test().for_loop_data_collection().update(
			//	_l_this_iterator_ref->get_on_error_function(),
		//		std::distance(_m_begin_iterator, _m_this_iterator),
		//		_l_this_iterator_ref->get_additinoal_string_data());
			return _l_current_element;
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		void
		gen_data_collection_iterator_t<T>::increment_iterator(
			const std::optional<ds::repetition_data_t>& _a_opt_repetition_data
		) 
	{
		using namespace ds;
		if (_a_opt_repetition_data.has_value())
		{
			const repetition_data_t& _l_rd{ _a_opt_repetition_data.value() };
			size_t _l_generation_collection_index{ _l_rd.generation_collection_index() };
			int64_t _l_current{ std::distance(_m_begin_iterator, _m_this_iterator) };
			while (std::distance(_m_begin_iterator, _m_this_iterator) < _l_generation_collection_index)
			{
				++_m_this_iterator;
			}
			auto& _l_this_iterator_ref{ *_m_this_iterator };
			_l_this_iterator_ref->set_data_using_mode_and_string_representing_repetition_data(
				_l_rd.mode(),_l_rd.additional_data());
		}
		else
		{
			_m_this_iterator = _m_end_iterator;
		}
	}
	_END_ABC_NS